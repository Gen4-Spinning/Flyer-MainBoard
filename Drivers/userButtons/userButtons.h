/*
 * userButtons.h
 *
 *  Created on: Apr 21, 2023
 *      Author: harsha
 */

#ifndef USERBUTTONS_H_
#define USERBUTTONS_H_

#include "stm32g4xx_hal.h"
// Code setup to only read one button at a time. Combinations cant be done. Thinking that needs two timers.
#define BTN_PRESSED 0
#define BTN_IDLE 1

#define ROTARY_SWITCH_OFF 1
#define ROTARY_SWITCH_ON 0

typedef struct userButtons{
	uint8_t greenBtn;
	uint16_t redBtn;
	uint16_t yellowBtn;
	uint8_t rotarySwitch;

	uint8_t initialState;
	uint8_t logicApplied;

}userBtns;

extern userBtns usrBtns;

void UsrBtns_SetInitialStates(void);

#endif /* USERBUTTONS_H_ */
