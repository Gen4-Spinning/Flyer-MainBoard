/*
 * userButtons.c
 *
 *  Created on: Apr 21, 2023
 *      Author: harsha
 */

#include "userButtons.h"
#include "main.h"


void UsrBtns_SetInitialStates(void){
	// pulled high , when idle
	usrBtns.redBtn = BTN_IDLE;
	usrBtns.greenBtn = BTN_IDLE;
	usrBtns.yellowBtn = BTN_IDLE;
	usrBtns.rotarySwitch = HAL_GPIO_ReadPin(ROTARY_GPIO_Port,ROTARY_Pin);

}
