/*
 * accelerometer.c
 *
 *  Created on: Jun 26, 2018
 *      Author: tudatn
 */
#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <math.h>
#include <errno.h>
#include <time.h>
#include <pthread.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c.h>
#include <linux/i2c-dev.h>
#include "accelerometer.h"
#include "beat.h"
#include "support.h"

#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define ADC_PATH "/sys/devices/platform/bone_capemgr/slots"
#define I2C_DEVICE_ADDRESS 0x1c
#define REG_STATUS 0x00
#define NUM_REGS 7
#define REG_XMSB 1
#define REG_XLSB 2
#define REG_YMSB 3
#define REG_YLSB 4
#define REG_ZMSB 5
#define REG_ZLSB 6
#define X_THRESHOLD 1
#define Y_THRESHOLD 1
#define Z_THRESHOLD 1.5
#define CTRL_REG1 0x2A
#define NUM_BITS_VAL 12
#define POLL_uS 10000
#define DEBOUNCE 200000

static pthread_t accelerometer_id;
static int i2cFileDesc;
static int isPlaying = 1;

// get 2's complement of numberBits from 16 bits values
static int16_t getAccelerometerVal(int16_t readVal, int numberBits) {
	int16_t result = readVal;
	if ((result & (1 << (numberBits - 1))) != 0)
		result = result - (1 << numberBits);
	return result;
}

static int initI2cBus(char* bus, int address) {
	int i2cFileDesc = open(bus, O_RDWR);
	if (i2cFileDesc < 0) {
		printf("I2C DRV: Unable to open bus for read/write (%s)\n", bus);
		perror("Error is:");
		exit(-1);
	}

	int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
	if (result < 0) {
		perror("Unable to set I2C device to slave address.");
		exit(-1);
	}
	return i2cFileDesc;
}

static void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value) {
	unsigned char buff[2];
	buff[0] = regAddr;
	buff[1] = value;
	int res = write(i2cFileDesc, buff, 2);
	if (res != 2) {
		perror("Unable to write i2c register");
		printf("%d\n", value);
		exit(-1);
	}
}

static unsigned char* readI2cRegs(int i2cFileDesc, unsigned char regAddr, int numberOfBytes)
{
	// To read a register, must first write the address
	int res = write(i2cFileDesc, &regAddr, sizeof(regAddr));
	if (res != sizeof(regAddr)) {
		perror("Unable to write i2c register.");
		exit(-1);
	}

	// Now read the value and return it
//	char value = 0;
	unsigned char *value = malloc(numberOfBytes * sizeof(*value));
	res = read(i2cFileDesc, value, numberOfBytes);
	if (res != numberOfBytes) {
		perror("Unable to read i2c register");
		exit(-1);
	}

	return value;
}

static void getXYZ(unsigned char* buff, float *x, float *y, float *z) {
	*x = (float) getAccelerometerVal(((buff[REG_XMSB] << 8) | (buff[REG_XLSB])) >> 4, NUM_BITS_VAL) / 1024;
	*y = (float) getAccelerometerVal(((buff[REG_YMSB] << 8) | (buff[REG_YLSB])) >> 4, NUM_BITS_VAL) / 1024;
	*z = (float) getAccelerometerVal(((buff[REG_ZMSB] << 8) | (buff[REG_ZLSB])) >> 4, NUM_BITS_VAL) / 1024;
}

void *readingAccelerometer(void *args) {
	// change to ACTIVE mode
	writeToFile(ADC_PATH, "BB-I2C1");
	i2cFileDesc = initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
	writeI2cReg(i2cFileDesc, CTRL_REG1, 1);
	float xAccel, yAccel, zAccel;

	unsigned char *buff = readI2cRegs(i2cFileDesc, REG_STATUS, NUM_REGS);
	while (isPlaying) {
		// TODO: should calibrate: ask TA for help
		buff = readI2cRegs(i2cFileDesc, REG_STATUS, NUM_REGS);
		getXYZ(buff, &xAccel, &yAccel, &zAccel);
//		printf("x_ac = %.2f, y_ac = %.2f, z_ac = %.2f\n", xAccel, yAccel, zAccel);
		if (fabs(xAccel) >  X_THRESHOLD) {
			Beat_playSound(Hihat_sound);
			usleep(DEBOUNCE);
		}
		if (fabs(yAccel) > Y_THRESHOLD) {
			Beat_playSound(Snare_sound);
			usleep(DEBOUNCE);
		}
		if (fabs(zAccel) > Z_THRESHOLD) {
			Beat_playSound(Base_sound);
			usleep(DEBOUNCE);
		}
		usleep(POLL_uS);
	}
	return NULL;
}

void Accelerometer_Init(void) {
	pthread_create(&accelerometer_id, NULL, &readingAccelerometer, NULL);
}
void Accelerometer_Close(void) {
	pthread_join(accelerometer_id, NULL);
}
