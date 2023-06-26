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
#include "BT_Machine.h"
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

void ErrorState(void){

	/* when you enter this state, start the motors. if the rotary switch is on,
	 * When you press the yellow button, Pause.When u press the green, Resume.
	 * The red btn, doesnt care about the position of the rotary switch and will stop the run
	 * and go back to idle.
	 */
	uint8_t noOfMotors = 0;
	uint8_t BTpacketSize = 0;
	uint8_t SMPS_turnOff_oneTime = 1;
	uint8_t BTmotorID = 0;
	while(1){

		if (S.oneTime){
			/* System Errors like ACK, CAN CUT error and SMPS errors take
			 * precedence over motor Errors and lift relative errors. these errors means we have
			 * to switch off the SMPS, so we check if any of the two errors are that.We also then
			 * set the BT msg to indicate that command, instead of the motor command.
			 */
			if (ME.errType1 == ERR_SYSTEM_LEVEL_SOURCE){
				if (ME.errReason1 != SYS_LIFT_RELATIVE_ERROR){
					SMPS_TurnOff(); // ACK err, CAN cut error and SMPS Err
					S.SMPS_switchOff = 0;
				}
				SetBTErrors(&ME,ME.errReason1,ME.errSource1,ME.errCode1);

			}else if (ME.errType2 == ERR_SYSTEM_LEVEL_SOURCE){
				if (ME.errReason1 != SYS_LIFT_RELATIVE_ERROR){
					SMPS_TurnOff(); // ACK err, CAN cut error and SMPS Err
					S.SMPS_switchOff = 0;
				}
				SetBTErrors(&ME,ME.errReason2,ME.errSource2,ME.errCode2);

			}else if (ME.errType1 == ERR_MOTOR_SOURCE){

				//send the Stop commands
				uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};
				noOfMotors = 6;
				SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);

				BTmotorID = Get_BTMotorID_from_Motor_ID(ME.errSource1);
				SetBTErrors(&ME,ME.errReason1,BTmotorID,ME.errCode1);

			}else if (ME.errType2 == ERR_MOTOR_SOURCE){
				//send the Stop commands
				uint8_t motors[] = {FLYER,BOBBIN,FR,BR,LEFT_LIFT,RIGHT_LIFT};
				noOfMotors = 6;
				SendCommands_To_MultipleMotors(motors,noOfMotors,EMERGENCY_STOP);

				BTmotorID = Get_BTMotorID_from_Motor_ID(ME.errSource2);
				SetBTErrors(&ME,ME.errReason2,BTmotorID,ME.errCode2);
			}
			TowerLamp_SetState(&hmcp, &mcp_portB,BUZZER_OFF,RED_ON,GREEN_OFF,AMBER_OFF);
			TowerLamp_ApplyState(&hmcp,&mcp_portB);

			L.logRunStateChange = 1;

			S.oneTime = 0;
		}


		//comes From Ack and SMPS short after the initial error has run through
		// for instance the lift relative error ack error comes
		if ((S.SMPS_switchOff) && (SMPS_turnOff_oneTime == 1)){
			SMPS_TurnOff();
			SMPS_turnOff_oneTime = 0;
		}

		// 1s timer.
		if ((S.BT_sendState == 1) && (S.BT_transmission_over == 1)){
			BTpacketSize = BT_MC_generateStatusMsg(BT_STOP);
			HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,BTpacketSize);
			S.BT_transmission_over = 0;
			S.BT_sendState = 0;
		}

		if (S.LOG_enabled){
			Log_DoOneCycle();
		}




		//CANT EXIT FROM THIS STATE

	}//closes while

}

