/*
 * MB_LEDs.c
 *
 *  Created on: 09-May-2023
 *      Author: harsha
 */

#include "MB_LEDs.h"

uint8_t runLedFirstTime = 0;
uint8_t stopLEDFirstTime = 1;
void Toggle_State_LEDs(StateTypeDef *s){
	if (s->current_state == IDLE_STATE){
		HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET);
		runLedFirstTime = 1;
	}else if (s->current_state == HOMING_STATE){
		HAL_GPIO_TogglePin(LED2_GPIO_Port, LED2_Pin);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET);
	}else if (s->current_state == PAUSE_STATE){
		HAL_GPIO_TogglePin(LED3_GPIO_Port, LED3_Pin);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET);
	}else if (s->current_state == DIAGNOSTICS_STATE){
		HAL_GPIO_TogglePin(CAN_LED_GPIO_Port, CAN_LED_Pin);
		HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
		HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET);
	}else if (s->current_state == ERROR_STATE){
		if (stopLEDFirstTime == 1){
			HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin,GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			stopLEDFirstTime = 0;
		}
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			HAL_GPIO_TogglePin(CAN_LED_GPIO_Port,CAN_LED_Pin);
	}
	else if (s->current_state == RUN_STATE){
		if (runLedFirstTime == 1){
			HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED3_GPIO_Port, LED3_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(LED2_GPIO_Port, LED2_Pin, GPIO_PIN_SET);
			runLedFirstTime = 0;
		}
			HAL_GPIO_TogglePin(LED1_GPIO_Port,LED1_Pin);
			HAL_GPIO_TogglePin(LED2_GPIO_Port,LED2_Pin);
			HAL_GPIO_TogglePin(LED3_GPIO_Port,LED3_Pin);
			HAL_GPIO_WritePin(CAN_LED_GPIO_Port, CAN_LED_Pin, GPIO_PIN_SET);
			HAL_GPIO_WritePin(FAULT_LED_GPIO_Port, FAULT_LED_Pin, GPIO_PIN_SET);
		}
}
