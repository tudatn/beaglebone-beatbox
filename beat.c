/*
 * beat.c
 *
 *  Created on: Jun 23, 2018
 *      Author: tudatn
 */
#define _BSD_SOURCE
#include <unistd.h>

#include "beat.h"
#include <stdio.h>
#include <unistd.h>
#include <pthread.h>
#include "audioMixer_template.h"
#include "joystick.h"

// File used for play-back:
// If cross-compiling, must have this file available, via this relative path,
// on the target when the application is run. This example's Makefile copies the wave-files/
// folder along with the executable to ensure both are present.
#define BASE_SOURCE_FILE "beatbox-wave-files/100051__menegass__gui-drum-bd-hard.wav"
#define HI_SOURCE_FILE "beatbox-wave-files/100053__menegass__gui-drum-cc.wav"
#define SNARE_SOURCE_FILE "beatbox-wave-files/100059__menegass__gui-drum-snare-soft.wav"
#define CYN_SOURCE_FILE "beatbox-wave-files/100056__menegass__gui-drum-cyn-hard.wav"
#define SPLASH_SOURCE_FILE "beatbox-wave-files/100060__menegass__gui-drum-splash-hard.wav"
#define TOM_SOURCE_FILE "beatbox-wave-files/100062__menegass__gui-drum-tom-hi-hard.wav"

#define NUM_MODES 3
#define SEC_PER_MINUTE 60
#define MICROSEC_PER_SEC 1000000


static enum beatmode_t mode = rockBeat;

static int isPlaying = 1;

static wavedata_t Hihat, Base, Snare, Cyn, Splash, Tom;

static int getHaftBeat(int tempoPBM) {
	return (int) ((float) SEC_PER_MINUTE  * MICROSEC_PER_SEC / tempoPBM / 2);
}

void Beat_getSounds() {
	AudioMixer_readWaveFileIntoMemory(HI_SOURCE_FILE, &Hihat);
	AudioMixer_readWaveFileIntoMemory(BASE_SOURCE_FILE, &Base);
	AudioMixer_readWaveFileIntoMemory(SNARE_SOURCE_FILE, &Snare);
	AudioMixer_readWaveFileIntoMemory(CYN_SOURCE_FILE, &Cyn);
	AudioMixer_readWaveFileIntoMemory(SPLASH_SOURCE_FILE, &Splash);
	AudioMixer_readWaveFileIntoMemory(TOM_SOURCE_FILE, &Tom);
}

void Beat_freeSounds() {
	AudioMixer_freeWaveFileData(&Base);
	AudioMixer_freeWaveFileData(&Hihat);
	AudioMixer_freeWaveFileData(&Snare);
	AudioMixer_freeWaveFileData(&Cyn);
	AudioMixer_freeWaveFileData(&Splash);
	AudioMixer_freeWaveFileData(&Tom);
}

static void playRockBeat() {
	int tempoPBM = getHaftBeat(Joystick_getTempoBPM());
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Snare);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Snare);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
}

static void playMyBeat() {
	int tempoPBM = getHaftBeat(Joystick_getTempoBPM());
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Snare);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Base);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	AudioMixer_queueSound(&Snare);
	usleep(tempoPBM);
	AudioMixer_queueSound(&Hihat);
	usleep(tempoPBM);
}

char *Beat_getMode() {
	switch (mode) {
		case rockBeat: return "rockBeat";
		case myBeat: return "myBeat";
		case none: return "none";
	}
	return NULL;
}

void Beat_changeMode() {
	mode = (mode + 1) % NUM_MODES;
}

void Beat_setMode(enum beatmode_t newMode) {
	mode = newMode;
}

void Beat_playSound(enum sound_t sound) {
	switch (sound) {
		case Hihat_sound: AudioMixer_queueSound(&Hihat); break;
		case Snare_sound: AudioMixer_queueSound(&Snare); break;
		case Base_sound: AudioMixer_queueSound(&Base); break;
		case Cyn_sound: AudioMixer_queueSound(&Cyn); break;
		case Splash_sound: AudioMixer_queueSound(&Splash); break;
		case Tom_sound: AudioMixer_queueSound(&Tom); break;
		default: break;
	}
}

void Beat_playMode() {
	while (isPlaying) {
		switch (mode) {
		case rockBeat: playRockBeat(); break;
		case myBeat: playMyBeat(); break;
		case none: // do nothing
		default: break;
		}
	}
}
