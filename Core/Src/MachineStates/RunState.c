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
#include "BT_Fns.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "MotorComms.h"
#include "Ack.h"
#include "userButtons.h"
#include "SysObserver.h"
#include "TowerLamp.h"
#include "mcp23017.h"
#include "MachineSensors.h"
#include "Log.h"

extern UART_HandleTypeDef huart1;

void RunState(void){

	/* when you enter this state, start the motors. if the rotary switch is on,
	 * When you press the yellow button, Pause.When u press the green, Resume.
	 * The red btn, doesnt care about the position of the rotary switch and will stop the run
	 * and go back to idle.
	 */
	uint8_t response = 0;
	uint8_t noOfMotors = 0;
	uint8_t BTpacketSize = 0;
	long currentTime;
	while(1){

		if (S.oneTime){
			//send the start commands
			uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};//{FLYER,BOBBIN,FR,BR};//,LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 6;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,START);
			if (response!= 2){
				SO_enableCANObservers(&SO,motors,noOfMotors);
			}

			SO_Start_LiftRelativeError_Delay();
			S.runMode = RUN_OPERATING;

			Log_setUpLogging(&L,motors,noOfMotors);
			Log_ResetBufferIndex(&L);
			if (S.LOG_enabled){
				L.bufferIdx += Log_addSettingsDataToBuffer(&msp,L.bufferIdx);
				L.logLayerChange = 1; //one time set it here so that when we start we get 0th layer rdngs.
				L.logRunStateChange = 1;
			}
			//when u start your in the run mode
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			S.oneTime = 0;
		}

		if (usrBtns.rotarySwitch == ROTARY_SWITCH_ON){
			if ((usrBtns.yellowBtn == BTN_PRESSED) && (S.runMode == RUN_OPERATING)){
				usrBtns.yellowBtn = BTN_IDLE;
				//Pause
				uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};//{};//,LEFT_LIFT,RIGHT_LIFT};//FLYER,BOBBIN,FR,BR};
				noOfMotors = 6;
				response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
				TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
				TowerLamp_ApplyState(&hmcp,&mcp_portB);

				//TODO - later make it such that once the CAN starts it never needs to stop.
				SO_disableAndResetCANObservers(&SO);

				S.runMode = RUN_PAUSED;
				S.BT_pauseReason = BT_PAUSE_USER_PRESSED;
				L.logRunStateChange = 1;
			}

			if ((usrBtns.greenBtn == BTN_PRESSED)  && (S.runMode == RUN_PAUSED)){
				usrBtns.greenBtn = BTN_IDLE;
				//RESUME
				uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};//{FLYER,BOBBIN,FR,BR};//,LEFT_LIFT,RIGHT_LIFT};//FLYER,BOBBIN,FR,BR};
				noOfMotors = 6;
				response = SendCommands_To_MultipleMotors(motors,noOfMotors,RESUME);

				//TODO - later make it such that once the CAN starts it never needs to stop.
				// so we will not have this statement.
				if (response!= 2){
					SO_enableCANObservers(&SO,motors,noOfMotors);
				}
				S.runMode = RUN_OPERATING;

				TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_ON,AMBER_OFF);
				TowerLamp_ApplyState(&hmcp,&mcp_portB);
				L.logRunStateChange = 1;
			}
		}

		// stop btn
		if (usrBtns.redBtn == BTN_PRESSED){
			usrBtns.redBtn = BTN_IDLE;
			//STOP
			uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};//{FLYER,BOBBIN,FR,BR};//,LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 6;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			S.runMode = RUN_STOPPED ;
			S.BT_pauseReason = 0;

			SO_disableAndResetCANObservers(&SO);

			//beep once when we go to idle
			TowerLamp_SetState(&hmcp,&mcp_portB,BUZZER_ON,RED_OFF,GREEN_OFF,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);
			HAL_Delay(1000); // to hear the beep

			ChangeState(&S,IDLE_STATE);
			SO_Reset_InitialLiftPosRecieved(&SO);
			break;
		}

		if (S.oneSecTimer != currentTime){
			mcParams.totalPower = R[0].power + R[1].power + R[2].power + R[3].power + R[4].power + R[5].power;
			currentTime = S.oneSecTimer;
		}

		/*
		if (sensor.latchedCreelSensor){
			//Pause
			sensor.latchedCreelSensor = 0;
			uint8_t motors[] = {FR,BR,LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 4;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_OFF,GREEN_OFF,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			//TODO - later make it such that once the CAN starts it never needs to stop.
			SO_disableAndResetCANObservers(&SO);

			S.runMode = RUN_PAUSED;
			S.BT_pauseReason = BT_PAUSE_CREEL_SLIVER_CUT;
			L.logRunStateChange = 1;
		}*/

		//--------ways to go into Error State--------

		if (SO.liftRelativePosError == 1){
			uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};//{FLYER,BOBBIN,FR,BR};//,LEFT_LIFT,RIGHT_LIFT};//FLYER,BOBBIN,FR,BR};
			noOfMotors = 6;
			response = SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);
			ME.ErrorFlag = 1;
			ME_addErrors(&ME,ERR_SYSTEM_LEVEL_SOURCE, SYS_LIFT_RELATIVE_ERROR, ERROR_SOURCE_LIFTS, 0);
		}

		//Error State
		if(ME.ErrorFlag == 1){
			ChangeState(&S,ERROR_STATE);
			break;
		}

		if (S.LOG_enabled){
			Log_DoOneCycle();
		}

		//--------sending BT info--------
		// 500ms timer.
		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			if (S.runMode == RUN_OPERATING ){
				BTpacketSize = BT_MC_generateStatusMsg(BT_RUN);
			}else{
				BTpacketSize = BT_MC_generateStatusMsg(BT_PAUSE);
			}
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		/* allow for getting settings during runtime */
		if (S.switchState == TO_SETTINGS){
			ChangeState(&S,SETTINGS_STATE);
			S.switchState = 0;
			break;
		}

		if (mcParams.currentLayer >= msp.targetLayers){
			//Wait for a while for the layer to change and then stop. Otherwise the lift change direction
			// msg doesnt seem to go..
			ChangeState(&S,FINISHED_STATE);
			break;
		}



	}//closes while

}

