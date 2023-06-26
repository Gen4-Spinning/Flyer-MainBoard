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
#include "BT_Fns.h"
#include "TowerLamp.h"
#include "mcp23017.h"
#include "Log.h"

extern UART_HandleTypeDef huart1;

void IdleState(void){

	/* depending on rotary switch you have two states - idle SETUP or idle RUN
	 * in idle SETUP : you can do homing or inching.
	 * if you press homing you just go into homing state and there u do homing.
	 * For inching, when u release the button inching stops.
	 * red button does nothing in this state.
	 *
	 * in idle RUN :
	 * green button starts the setup only if youve done Homing
	 * yellow does nothing
	 * red does nothing
	 *
	 * also from this state only, you can go to the diagnostics or change the settings
	 * and save. Only for Carding you can change even in run mode
	 *
	 * When u press the red Btn in runState you come back to idle. At that point we want to clear
	 * homing flag and any runTime vars so that user is force to start a new run.
	 */
	uint8_t inchingHappening = 0;
	uint8_t response = 0;
	uint8_t noOfMotors = 0;
	uint8_t BTpacketSize = 0;
	while(1){

		if (S.oneTime){
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			if (S.prev_state != HOMING_STATE){
				ResetHomingState(&S);
				CalculateMachineParameters(&msp,&mcParams);
				SetupMachineParametersForFirstLayer(&mcParams);
				CalculateTotalRunTime(&msp ,&mcParams);
				ReadySetupCommand_AllMotors(&msp,&mcParams);
				//Reset Run machine Parameters
			}
			L.logRunStateChange = 1;
			L.flushBuffer = 1;
			S.oneTime  = 0;
		}

		if (S.idleMode == IDLE_SETUP){
			if (usrBtns.greenBtn == BTN_PRESSED){
				usrBtns.greenBtn = BTN_IDLE;
				Log_ResetRunTimeRdngNos();
				ChangeState(&S,HOMING_STATE);
				break;
			}

			if ((usrBtns.yellowBtn == BTN_PRESSED) && (usrBtns.logicApplied == 0)){
				usrBtns.logicApplied = 1;
				inchingHappening = 1;
				uint8_t motors[] = {FR,BR};
				noOfMotors = 2;
				response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);

			}
			if ((usrBtns.yellowBtn == BTN_IDLE) && (usrBtns.logicApplied == 1)){
				usrBtns.logicApplied = 0;
				inchingHappening = 0;
				// PAUSE INCHING
				uint8_t motors[] = {FR,BR};
				noOfMotors = 2;
				response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			}

			if (usrBtns.rotarySwitch == ROTARY_SWITCH_ON){
				S.idleMode = IDLE_RUN;
			}
		}

		if (S.idleMode == IDLE_RUN){
			if (usrBtns.greenBtn == BTN_PRESSED){
				usrBtns.greenBtn = BTN_IDLE;
				if (S.HomingDone){
					Log_ResetRunTimeRdngNos(); //
					ChangeState(&S,RUN_STATE);
					break;
				}
			}

			if (usrBtns.rotarySwitch == ROTARY_SWITCH_OFF){
				S.idleMode = IDLE_SETUP;
			}
		}

		//----------- go to other places-------

		if (S.switchState == TO_SETTINGS){
			ChangeState(&S,SETTINGS_STATE);
			S.switchState = 0;
			break;
		}else if (S.switchState == TO_DIAGNOSTICS){
			ChangeState(&S,DIAGNOSTICS_STATE);
			S.switchState = 0;
			break;
		}else if (S.switchState == TO_GB_CALIBRATION){
			ChangeState(&S,GB_CALIB_STATE);
			S.switchState = 0;
			break;
		}

		//--------ways to go into Error State--------

		if (response == 2){
			//Setup command Error. No motor would have started. just
			//Send a snackbar to the app
		}

		//Error State
		if(ME.ErrorFlag == 1){
			ChangeState(&S,ERROR_STATE);
			break;
		}

		//--------sending BT info--------
		// 500ms timer.
		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			if (inchingHappening){
				BTpacketSize = BT_MC_generateStatusMsg(BT_INCHING);
			}else{
				BTpacketSize = BT_MC_generateStatusMsg(BT_IDLE);
			}
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		//in idle only the state change gets logged. no motor running
		if (S.LOG_enabled){
			Log_DoOneCycle();
		}

		//-------for Manual change--------
		if (S.current_state != IDLE_STATE){
			break;
		}


	}//closes while

}

