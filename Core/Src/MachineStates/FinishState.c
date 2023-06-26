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
#include "SMPS.h"
#include "mcp23017.h"
#include "Log.h"

extern UART_HandleTypeDef huart1;

void FinishState(void){

	/* when you enter this state, start the motors. if the rotary switch is on,
	 * When you press the yellow button, Pause.When u press the green, Resume.
	 * The red btn, doesnt care about the position of the rotary switch and will stop the run
	 * and go back to idle.
	 */
	uint8_t noOfMotors = 0;
	uint8_t BTpacketSize = 0;
	uint8_t buzzerCounter = 0;

	while(1){

		if (S.oneTime){
			uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};
			noOfMotors = 6;
			SendCommands_To_MultipleMotors(motors,noOfMotors,RAMPDOWN_STOP);

			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_ON,GREEN_ON,AMBER_ON);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			S.oneTime = 0;
		}


		// 1s timer.
		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			BTpacketSize = BT_MC_generateStatusMsg(BT_FINISH);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
			if (buzzerCounter == 2){
				TowerLamp_NegateState(&hmcp,&mcp_portB,TOWER_BUZZER);
				TowerLamp_ApplyState(&hmcp,&mcp_portB);
				buzzerCounter = 0;
			}
		}

		//CANT EXIT FROM THIS STATE

	}//closes while

}

