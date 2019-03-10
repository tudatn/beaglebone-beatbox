/*
 * beat.h
 *
 *  Created on: Jun 23, 2018
 *      Author: tudatn
 */

#ifndef BEAT_H_
#define BEAT_H_

enum beatmode_t {rockBeat, myBeat, none};
enum sound_t {Hihat_sound, Snare_sound, Base_sound, Cyn_sound, Splash_sound, Tom_sound};

void Beat_getSounds();
void Beat_freeSounds();
void Beat_changeMode();
char *Beat_getMode();
void Beat_setMode(enum beatmode_t mode);
void Beat_playSound(enum sound_t sound);
void Beat_playMode();

#endif /* BEAT_H_ */
