/*
 * network.c
 *
 *  Created on: Jun 24, 2018
 *      Author: tudatn
 */

#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include <math.h>
#include <netdb.h>
#include <string.h>			// for strncmp()
#include <unistd.h>			// for close()

#include "network.h"
#include "beat.h"
#include "audioMixer_template.h"
#include "joystick.h"

#define MSG_MAX_LEN 1024
#define PORT 12345


static int isListening = 1;
static pthread_t netid;

static void *network(void *args) {
	// Buffer to hold packet data:
	char message[MSG_MAX_LEN];

	// Address

	struct sockaddr_in sin;
	memset(&sin, 0, sizeof(sin));
	sin.sin_family = AF_INET;                   // Connection may be from network
	sin.sin_addr.s_addr = htonl(INADDR_ANY);    // Host to Network long
	sin.sin_port = htons(PORT);                 // Host to Network short

	// Create the socket for UDP
	int socketDescriptor = socket(PF_INET, SOCK_DGRAM, 0);

	// Bind the socket to the port (PORT) that we specify
	bind (socketDescriptor, (struct sockaddr*) &sin, sizeof(sin));

	while (isListening) {
		// Get the data (blocking)
		// Will change sin (the address) to be the address of the client.
		// Note: sin passes information in and out of call!
		unsigned int sin_len = sizeof(sin);
		int bytesRx = recvfrom(socketDescriptor, message, MSG_MAX_LEN, 0,
				(struct sockaddr *) &sin, &sin_len);
		message[bytesRx] = 0;
		char *newLineChar = strchr(message, '\n');
		if (newLineChar)
			*newLineChar = 0;
		printf("Message received (%d bytes): \n\n'%s'\n", bytesRx, message);

		if (strcmp(message, "modeNone") == 0)
			Beat_setMode(none);
		else if (strcmp(message, "rockBeat") == 0)
			Beat_setMode(rockBeat);
		else if (strcmp(message, "myBeat") == 0)
			Beat_setMode(myBeat);
		else if (strcmp(message, "volumeDown") == 0)
			AudioMixer_downVolume();
		else if (strcmp(message, "volumeUp") == 0)
			AudioMixer_upVolume();
		else if (strcmp(message, "tempoDown") == 0)
			Joystick_downTempoBPM();
		else if (strcmp(message, "tempoUp") == 0)
			Joystick_upTempoBPM();
		else if (strcmp(message, "Hihat") == 0)
			Beat_playSound(Hihat_sound);
		else if (strcmp(message, "Snare") == 0)
			Beat_playSound(Snare_sound);
		else if (strcmp(message, "Base") == 0)
			Beat_playSound(Base_sound);

		sprintf(message, "beatMode %s volume %d tempo %d OK\n", Beat_getMode(), AudioMixer_getVolume(), Joystick_getTempoBPM());

		// Transmit a reply:
		sin_len = sizeof(sin);
		sendto( socketDescriptor,
				message, strlen(message),
				0,
				(struct sockaddr *) &sin, sin_len);
	}

	close(socketDescriptor);
	return NULL;
}

void Network_startListening(void) {
	pthread_create(&netid, NULL, &network, NULL);
}

void Network_stopListening(void) {
	pthread_join(netid, NULL);
}
