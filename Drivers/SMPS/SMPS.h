/*
 * SMPS.h
 *
 *  Created on: 13-May-2023
 *      Author: harsha
 */

#ifndef SMPS_H_
#define SMPS_H_

#include "main.h"
#include "StateMachine.h"

#define SMPS_TURNEDON 1
#define SMPS_TURNEDOFF 0

#define SMPS_ON 0
#define SMPS_OFF 1

void SMPS_TurnOn(void);
void SMPS_TurnOff(void);


#endif /* SMPS_H_ */
