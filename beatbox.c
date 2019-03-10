/*
 *  Small program to read a 16-bit, signed, 44.1kHz wave file and play it.
 *  Written by Brian Fraser, heavily based on code found at:
 *  http://www.alsa-project.org/alsa-doc/alsa-lib/_2test_2pcm_min_8c-example.html
 */
#define _BSD_SOURCE

#include <alsa/asoundlib.h>
#include "audioMixer_template.h"
#include "joystick.h"
#include "beat.h"
#include "network.h"
#include "accelerometer.h"

int main(void)
{
	printf("Beginning play-back\n");
	Beat_getSounds();
	AudioMixer_init();
	Joystick_Init();
	Network_startListening();
	Accelerometer_Init();

	// play beats
	Beat_playMode();

	// Cleanup, letting the music in buffer play out (drain), then close and free.
	AudioMixer_cleanup();
	Joystick_Close();
	Network_stopListening();
	Beat_freeSounds();
	Accelerometer_Close();
	printf("Done!\n");

	return 0;
}

