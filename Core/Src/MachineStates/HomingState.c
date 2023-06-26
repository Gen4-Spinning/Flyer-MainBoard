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
#include "MachineErrors.h"
#include "StateMachine.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "MotorComms.h"
#include "Ack.h"
#include "userButtons.h"
#include "SysObserver.h"
#include "BT_Fns.h"
#include "TowerLamp.h"
#include "mcp23017.h"
#include "Log.h"


extern UART_HandleTypeDef huart1;

void HomingState(void){
	uint8_t noOfMotors = 0;
	uint8_t response = 0;
	uint8_t BTpacketSize = 0;
	uint8_t towerLampCount = 0;
	while(1){

		/* start homing when u come in.
		 * rotary switch does nothing. pause button does nothing.
		 * stop button stops homing and takes u back to idle
		 * when homing is done you go back to idle.
		 * TODO any error stops homing and takes you to the error state
		 *
		 * response for emergency stop doesnt need to be handled per se, in that the stop SMPS
		 * will trigger if the ack is not recieved.
		 * lift relative position checking. if it throws an error we send the stop command,
		 * and if its doesnt recieve an ACK we switch off the SMPS
		 */

		if (S.oneTime == 1){
			ResetHomingState(&S);
			//send Homing Command
			uint8_t motors[] = {LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 2;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,HOMING);
			if (response!= 2){
				SO_enableCANObservers(&SO,motors,noOfMotors);
			}

			Log_setUpLogging(&L,motors,noOfMotors);
			L.logRunStateChange = 1;

			SO_Start_LiftRelativeError_Delay();
			//switch off tower Lamps here.it will toggle in the 500ms section below
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			S.oneTime = 0;
		}

		if (S.HomingDone == 1){
			//TODO - later make it such that once the CAN starts it never needs to stop.

			SO_disableAndResetCANObservers(&SO);
			Log_disableLogging(&L);

			TowerLamp_SetState(&hmcp,&mcp_portB,BUZZER_ON,RED_OFF,GREEN_OFF,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			HAL_Delay(1000); // to hear the beep
			SO_Reset_InitialLiftPosRecieved(&SO);
			ChangeState(&S, IDLE_STATE);
			break;
		}

		if (usrBtns.redBtn == BTN_PRESSED){
			usrBtns.redBtn = BTN_IDLE;
			//Send STOP Command to all motors
			uint8_t motors[] = {LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 2;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);

			SO_disableAndResetCANObservers(&SO);
			Log_disableLogging(&L);

			SO_Reset_InitialLiftPosRecieved(&SO);
			ChangeState(&S, IDLE_STATE);
			break;
		}

		if (S.LOG_enabled){
			Log_DoOneCycle();
		}

		//--------ways to go into Error State--------

		if (response == 2){
			//Setup command Error. Send a snackbar to the app
		}

		if (SO.liftRelativePosError == 1){
			uint8_t motors[] = {LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 2;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			ME.ErrorFlag = 1;
			ME_addErrors(&ME,ERR_SYSTEM_LEVEL_SOURCE,SYS_LIFT_RELATIVE_ERROR, ERROR_SOURCE_LIFTS, 0);
		}

		//Error State
		if(ME.ErrorFlag == 1){
			ChangeState(&S,ERROR_STATE);
			break;
		}

		//--------sending BT info--------
		// 500ms timer.
		if (S.BT_sendState == 1){
			BTpacketSize = BT_MC_generateStatusMsg(BT_HOMING);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_sendState = 0;
			towerLampCount ++;
			if (towerLampCount == 2){
				TowerLamp_NegateState(&hmcp,&mcp_portB,TOWER_AMBER);
				TowerLamp_ApplyState(&hmcp,&mcp_portB);
				towerLampCount = 0;
			}
		}

	}//closes while

}

