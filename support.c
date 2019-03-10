/*
 * support.c
 *
 *  Created on: Jun 8, 2018
 *      Author: tudatn
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "support.h"

#define MAX_LENGTH 1024

static int lock = 1;

int getLock(void) {
	return lock;
}
void unLock(void) {
	lock = 0;
}

void printArray(int *array, int size) {
	for (int i = 0; i < size; i++) {
		if (i%10 == 0) printf("\n");
		printf("%d, ", array[i]);
	}
}

void readFromFile(char *fileName, char *readValue) {
	FILE *file = fopen(fileName, "r");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for read\n", fileName);
		exit(-1);
	}
	// Reading string (line) and remove new line character
	if (fgets(readValue, MAX_LENGTH, file)) {
		char *newLineChar = strchr(readValue, '\n');
		if (newLineChar)
			*newLineChar = '\0';
	}
	fclose(file);
}

void writeToFile(char *fileName, const char *content) {
	FILE *file = fopen(fileName, "w");
	if (file == NULL) {
		printf("ERROR: Unable to open file (%s) for write\n", fileName);
		exit(-1);
	}
	int charWritten = fprintf(file, "%s", content);
	if (charWritten <= 0)
		printf("ERROR WRITING DATA");
	fclose(file);
}

char* getFileName(int nth, char *generalPath, char *subFile, char *returnedFilePath) {
	strcpy(returnedFilePath, generalPath);
	char number[MAX_LENGTH];
	// note: sprintf is not safe
	sprintf(number, "%d", nth);
	strcat(returnedFilePath, number);
	strcat(returnedFilePath, subFile);
	return returnedFilePath;
}

void swap(int *a, int *b) {
	int temp = *a;
	*a = *b;
	*b = temp;
}

int* getRandomArray(int size) {
	time_t t;
	srand((unsigned) time(&t));

	int *array = malloc(size * sizeof(*array));
	if (array == NULL) {
		printf("ERROR: failed to allocate memory!\n");
		exit(-1);
	}
	for (int i = 0; i < size; i++)
		array[i] = i + 1;
	for (int i = 0; i < size; i++) {
		swap(&array[i], &array[rand() % size]);
	}
	return array;
}

int interpolate(const int *x, const int *y, int size, int value) {
	int left = 0;
	int right = size - 1;
	int delta = right - left;
	while (delta > 1) {
		int mid = left + (right - left) / 2;
		if (value < x[mid])
			right = mid;
		else
			left = mid;
		delta = right - left;
	}
	return (int) (y[left] + (double)(y[right] - y[left]) / (x[right] - x[left]) * (value - x[left]));
}

void writeArrayToString(int *array, int size, char *message, int length) {
	if (length > size || length < 1) sprintf(message, "Invalid argument. Must be between 1 and %d (array length).\n", size);
	else {
		char temp[MAX_LENGTH];
		if (size > 1) {
			sprintf(message, "%d, ", array[0]);
			for (int i = 1; i < length - 1; i++) {
				if (i % 10 == 0)
					sprintf(temp, "\n%d, ", array[i]);
				else
					sprintf(temp, "%d, ", array[i]);
				strcat(message, temp);
			}
			if ((length - 1) % 10 == 0) sprintf(temp, "\n%d", array[length - 1]);
			else sprintf(temp, "%d", array[length - 1]);
			strcat(message, temp);
		} else
			sprintf(message, "%d", array[0]);
	}
}

int sanitize(char *input, char **commands, int maxNumberOfArguments) {
	int numberOfArguments = 0;
	if (strlen(input)) {
		int i = 0;
		char temp;
		while ((temp = input[i]) != 0) {
				int j = 0;
				if (temp != ' ') {
					numberOfArguments++;

					if (numberOfArguments > maxNumberOfArguments) return maxNumberOfArguments;

					while ((temp = input[i]) != 0 && temp != ' ' ) {
						commands[numberOfArguments - 1][j++] = temp;
						i++;
					}
					commands[numberOfArguments - 1][j] = 0;
				}
				if (temp != 0) i++;
		}
	}
	return numberOfArguments;
}

