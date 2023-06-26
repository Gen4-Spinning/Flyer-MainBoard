/*
 * IdleState.c
 *
 *  Created on: 15-Apr-2023
 *      Author: harsha
 *
 *  Things to Do in Idle State for Flyer:
 *  1. Detect Button Press and do homing/ inching and start the full run cycle
 *  2. Save New settings from the app when we get them
 *  3. Write the Dbg codes to start motors. Dbg Stop is in the while loop.
 */

#include "stdio.h"
#include "BT_Machine.h"
#include "MachineErrors.h"
#include "StateMachine.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "MotorComms.h"
#include "Ack.h"
#include "userButtons.h"
#include "BT_Fns.h"
#include "TowerLamp.h"
#include "mcp23017.h"

extern UART_HandleTypeDef huart1;

void GB_CalibrationState(void){

	/*
	 */

	uint8_t BTpacketSize = 0;
	while(1){

		if (S.oneTime){
			S.oneTime  = 0;
			if (GBD.BT_command == BT_GB_CALIB_START){
				FDCAN_sendGBCalibCommand_ToLiftMotor(RIGHTLIFT_ADDRESS, CONTINUOUS);
				FDCAN_sendGBCalibCommand_ToLiftMotor(LEFTLIFT_ADDRESS, CONTINUOUS);
			}
			else if (GBD.BT_command == BT_GB_CALIB_SAVE_LEFT){
				//FDCAN_sendGBCalibCommand_ToLiftMotor(LEFTLIFT_ADDRESS, SETHOMINGPOS);
				S.oneSecTimer = 0;
				while(S.oneSecTimer < 1){};
				GBD.homeSetupSuccess = 1;
				if (GBD.homeSetupSuccess){
					 HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGSUCCESS,6);
					 S.BT_transmission_over = 1;
				}else{
					HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGFAILURE,6);
					S.BT_transmission_over = 1;
				}
				GBD.homeSetupSuccess = 0;
				S.switchState = TO_IDLE_FROM_GB_CALIB;
			}
			else if (GBD.BT_command == BT_GB_CALIB_SAVE_RIGHT){
				//FDCAN_sendGBCalibCommand_ToLiftMotor(RIGHTLIFT_ADDRESS, SETHOMINGPOS);
				S.oneSecTimer = 0;
				while(S.oneSecTimer < 1){};
				GBD.homeSetupSuccess = 1;
				if (GBD.homeSetupSuccess){
					 HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGSUCCESS,6);
					 S.BT_transmission_over = 1;
				}else{
					HAL_UART_Transmit_IT(&huart1,(uint8_t*)SAVINGFAILURE,6);
					S.BT_transmission_over = 1;
				}
				GBD.homeSetupSuccess = 0;
				S.switchState = TO_IDLE_FROM_GB_CALIB;
			}
			S.oneTime = 0;
		}

		//send GB_info
		// 500ms timer.
		if (GBD.BT_command == BT_GB_CALIB_START){
			if ((S.BT_send_GBcalibData == 1) && (S.BT_transmission_over == 1)){
				BTpacketSize = BT_generate_GbCalibMsg(&GBD);
				HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
				S.BT_transmission_over = 0;
				S.BT_send_GBcalibData = 0;
			}
		}


		if (S.switchState == TO_IDLE_FROM_GB_CALIB){
			FDCAN_sendGBCalibCommand_ToLiftMotor(RIGHTLIFT_ADDRESS, CONTINUOUS_STOP);
			FDCAN_sendGBCalibCommand_ToLiftMotor(LEFTLIFT_ADDRESS, CONTINUOUS_STOP);
			HAL_Delay(1);
			ChangeState(&S,IDLE_STATE);
			S.switchState = 0;
			break;
		}


	}//closes while

}

