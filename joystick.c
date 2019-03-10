/*
 * joystick.c
 *
 *  Created on: Jun 22, 2018
 *      Author: tudatn
 */

#define _BSD_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dirent.h>
#include <errno.h>
#include "joystick.h"
#include "network.h"
#include "support.h"
#include "audioMixer_template.h"
#include "beat.h"
#include <unistd.h>

#define GPIO_FILE_PATH "/sys/class/gpio/gpio"
#define GPIO_EXPORT "/sys/class/gpio/export"
#define MAX_LENGTH 1024
#define POLL_uS 10000
#define DEBOUNCE_uS 200000 // can be smaller but my IN joystick is too sensitive
#define MAX_TEMPO 300
#define MIN_TEMPO 40
#define DELTA_TEMPO 5

enum direction_t {LEFT, RIGHT, UP, DOWN, IN, NONE};

static int tempoBPM = 120;

static int isPlaying = 1;

static enum direction_t joystickDirection = NONE;

static pthread_t joystick_id;

static int isGpioActive(int gpioNumber) {
	char gpioNth[MAX_LENGTH];
	sprintf(gpioNth, "%d", gpioNumber);
	char readValue[MAX_LENGTH];
	char file[MAX_LENGTH];

	// handle if gpio is exported
	DIR *dir = opendir(getFileName(gpioNumber, GPIO_FILE_PATH, "", file));
	if (errno == ENOENT) {
		// write gpio pin number to export file
		writeToFile(GPIO_EXPORT, gpioNth);
	}
	closedir(dir);

	writeToFile(getFileName(gpioNumber, GPIO_FILE_PATH, "/direction", file), "in");
	readFromFile(getFileName(gpioNumber, GPIO_FILE_PATH, "/value", file), readValue);
	if (strcmp(readValue, "0") == 0)
		return 1;
	return 0;
}

static enum direction_t getJoystickDirection() {
	// gpio number: up: 26; right: 47; down: 46; left: 65; push: 27
	enum direction_t joystickDirection = NONE;
	if (isGpioActive(26)) return joystickDirection = UP;
	if (isGpioActive(47)) return joystickDirection = RIGHT;
	if (isGpioActive(46)) return joystickDirection = DOWN;
	if (isGpioActive(65)) return joystickDirection = LEFT;
	if (isGpioActive(27)) return joystickDirection = IN;
	return joystickDirection;
}

static void *joystick(void *args) {
	while (isPlaying) {
		joystickDirection = getJoystickDirection();
		switch (joystickDirection) {
			case IN:
				Beat_changeMode(); break;
			case UP:
				AudioMixer_upVolume(); break;
			case DOWN:
				AudioMixer_downVolume(); break;
			case RIGHT:
				Joystick_upTempoBPM(); break;
			case LEFT:
				Joystick_downTempoBPM(); break;
			default: break;
		}
		if (joystickDirection != NONE) usleep(DEBOUNCE_uS);
		else usleep(POLL_uS);
	}
	return NULL;
}

int Joystick_getTempoBPM(void) {
	return tempoBPM;
}

void Joystick_setTempoBPM(int newTempo) {
	tempoBPM = newTempo;
}

void Joystick_upTempoBPM() {
	tempoBPM = ((tempoBPM + DELTA_TEMPO) > MAX_TEMPO) ? MAX_TEMPO : (tempoBPM + DELTA_TEMPO);
}

void Joystick_downTempoBPM() {
	tempoBPM = ((tempoBPM - DELTA_TEMPO) < MIN_TEMPO) ? MIN_TEMPO : (tempoBPM - DELTA_TEMPO);
}

enum direction_t Joystick_getDirection(void) {
	return joystickDirection;
}

void Joystick_Init(void) {
	pthread_create(&joystick_id, NULL, &joystick, NULL);
}

void Joystick_Close(void) {
	pthread_join(joystick_id, NULL);
}
