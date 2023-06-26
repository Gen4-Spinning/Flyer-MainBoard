/*
 * SMPS.c
 *
 *  Created on: 13-May-2023
 *      Author: harsha
 */

#include "SMPS.h"
void SMPS_TurnOn(void){
	HAL_GPIO_WritePin(SMPS_CNTRL_GND_GPIO_Port, SMPS_CNTRL_GND_Pin, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(SMPS_CNTRL_OP_GPIO_Port, SMPS_CNTRL_OP_Pin, GPIO_PIN_SET);
	S.SMPS_cntrl = 1;
}

void SMPS_TurnOff(void){
	HAL_GPIO_WritePin(SMPS_CNTRL_GND_GPIO_Port, SMPS_CNTRL_GND_Pin, GPIO_PIN_SET);
	HAL_GPIO_WritePin(SMPS_CNTRL_OP_GPIO_Port, SMPS_CNTRL_OP_Pin, GPIO_PIN_RESET);
	S.SMPS_cntrl = 0;
}
