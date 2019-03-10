/*
 * joystick.h
 *
 *  Created on: Jun 22, 2018
 *      Author: tudatn
 */

#ifndef JOYSTICK_H_
#define JOYSTICK_H_

void Joystick_Init(void);
void Joystick_Close(void);

int Joystick_getTempoBPM(void);
void Joystick_setTempoBPM(int newTempo);
void Joystick_upTempoBPM(void);
void Joystick_downTempoBPM(void);

#endif /* JOYSTICK_H_ */
