/*
 * SettingsState.c
 *
 *  Created on: Mar 10, 2023
 *      Author: harsha
 */

#include "Struct.h"
#include "BT_Machine.h"
#include "MachineErrors.h"
#include "CommonConstants.h"
#include "stm32g4xx_hal.h"
#include "StateMachine.h"

extern UART_HandleTypeDef huart1;

void SettingsState(void){

	uint8_t success = 0;

	if (BT.information == REQ_SETTINGS_FROM_APP){
		  uint8_t packetSize = BT_MC_generateSettingsMsg(&msp);
		  while(S.BT_transmission_over != 1){};
		  HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,packetSize);
		  S.BT_transmission_over = 0;
	}

	if (BT.information == SETTINGS_FROM_APP){
	  if (S.BT_dataOK){
		  success = BT_MC_Save_Settings();
		  if (success == 0){
			MBE.AppSettings_eepromWriteFailure += 1;
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGFAILURE,6);
			S.BT_transmission_over = 1;
		 }else{
			 // send success msg to App
			 HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGSUCCESS,6);
			 S.BT_transmission_over = 1;
		 }
		  S.BT_dataOK = 0;
	  }else{
		  S.BT_transmission_over = 0;
		  MBE.AppSettings_parsingFailure += 1;
		  HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGFAILURE,6);
	  }
	}
	  /* go back to the idle state. the new values get integrated into the
	   * machine parameters struct in the idle state when you first enter it
	   */

	  ChangeState(&S,S.prev_state);
}
