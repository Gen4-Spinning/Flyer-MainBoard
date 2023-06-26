/*
 * DiagnosticsState.c
 *
 *  Created on: Mar 7, 2023
 *      Author: harsha
 */

#include "Struct.h"
#include "MachineErrors.h"
#include "StateMachine.h"
#include "CommonConstants.h"
#include "FDCAN.h"
#include "CAN_MotherBoard.h"
#include "BT_Machine.h"
#include "userButtons.h"
#include "SysObserver.h"
#include "Log.h"


extern TIM_HandleTypeDef htim15;

void DiagnosticsState(){
	uint8_t motorID = 0;
	uint8_t noOfMotors = 0;
	uint8_t response = 0;
	uint8_t motors[2] = {0,0};

	/* lift diagnostics -> running only left lift or right lift by default the type of control is
	 * open loop. Though the app allows distance from 1 - 280, if you move one side by more than 4 mm
	 * the lift relative error automatically will stop the motor.
	 *
	 * for lift both, if you have a small distance, say less than 5 mm, then automatically this code
	 * selects a very slow speed, 1mm/sec and sends the closed loop speed. Otherwise the speed is
	 * 2mm/sec for distances greater
	 *
	 * lift relative error is always on. if the stop commands doesnt get the ack the SMPS is switched
	 * off. Pressing stop on the switch box also sends the stop command immediately, and again
	 * if ack is not recieved , smps is switched off.
	 *
	 * if response is 2 for a setup msg, we just need to send a msg to the app , with some error.
	 *
	 */

	while (1){

		if (usrBtns.redBtn == BTN_PRESSED){
			usrBtns.redBtn = BTN_IDLE;
			//STOP
			S.oneTime = 1;
			D.stopTest = 1;
			}

		if(S.oneTime == 1){
			if (D.stopTest == 0){	// if stop == 0, means its a start command.
				if (D.motorID <= 4){		// if single Motor
					motorID = GetMotorID_from_BTMotor_ID(D.motorID);
					motors[0] = motorID;
					noOfMotors = 1;
					response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
					if (response!= 2){
						SO_enableCANObservers(&SO,motors,noOfMotors);
						Log_setUpLogging(&L,motors,noOfMotors);
					}
					//TODO - what do you want to do if you get a 2?
				}else if (D.motorID == BT_DRAFTING){	// if drafting or winding
					motors[0] = FR;
					motors[1] = BR;
					noOfMotors = 2;
					response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
					if (response!= 2){
						SO_enableCANObservers(&SO,motors,noOfMotors);
						Log_setUpLogging(&L,motors,noOfMotors);
					}
				}else if (D.motorID == BT_WINDING){	// if drafting or winding
					motors[0] = FLYER;
					motors[1] = BOBBIN;
					noOfMotors = 2;
					response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
					if (response!= 2){
						SO_enableCANObservers(&SO,motors,noOfMotors);
						Log_setUpLogging(&L,motors,noOfMotors);
					}
				}else{
					if (D.motorID == BT_LIFT_LEFT){
						motors[0] = LEFT_LIFT;
						noOfMotors = 1;
						response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
						if (response!= 2){
							SO_enableCANObservers(&SO,motors,noOfMotors);
							Log_setUpLogging(&L,motors,noOfMotors);
						}
					}else if (D.motorID == BT_LIFT_RIGHT){
						motors[0] = RIGHT_LIFT;
						noOfMotors = 1;
						response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
						if (response!= 2){
							SO_enableCANObservers(&SO,motors,noOfMotors);
							Log_setUpLogging(&L,motors,noOfMotors);
						}
					}else if (D.motorID == BT_LIFT_BOTH){
						motors[0] = LEFT_LIFT;
						motors[1] = RIGHT_LIFT;
						noOfMotors = 2;
						response = Send_DiagCommands_To_MultipleMotors(motors,noOfMotors,START);
						SO_Start_LiftRelativeError_Delay();
						if (response!= 2){
							SO_enableCANObservers(&SO,motors,noOfMotors);
							Log_setUpLogging(&L,motors,noOfMotors);
						}
					}else{
						//do nothing
					}
					// Lift Motors
				}
				//Start Timer that sends runtime Data to app
				htim15.Instance->SR &= ~TIM_SR_UIF; // Clear pending flag of timer15
				HAL_TIM_Base_Start_IT(&htim15);
				S.oneTime = 0;
				S.BT_dataOK = 0;
			}else{
				//if we get a stop command, and its not for lift, send a stop to only the motors
				// weve started , otherwise we ll get no Ack error, and then go back to idle
				HAL_TIM_Base_Stop_IT(&htim15); // stop sending data to the app
				//calculate which motors need to be stopped
				uint8_t motorStop_Array[noOfMotors];
				for (int i=0;i<noOfMotors;i++){
					motorStop_Array[i] = motors[i];
				}
				Send_DiagCommands_To_MultipleMotors(motorStop_Array,noOfMotors,EMERGENCY_STOP);
				SO_disableAndResetCANObservers(&SO);
				Log_disableLogging(&L);
				D.stopTest = 0;
				S.oneTime = 0;
				S.switchState = 0; // needed for the bad case when u disconnect while within the diag state and need to come out
				SO_Reset_InitialLiftPosRecieved(&SO);
				ChangeState(&S,IDLE_STATE);
				break;
			}
		} // closes one Time


		if (SO.liftRelativePosError == 1){
			HAL_TIM_Base_Stop_IT(&htim15);
			//calculate which motors need to be stopped
			uint8_t motorStop_Array[noOfMotors];
			for (int i=0;i<noOfMotors;i++){
				motorStop_Array[i] = motors[i];
			}
			SendCommands_To_MultipleMotors(motorStop_Array,noOfMotors,EMERGENCY_STOP);
			ME.ErrorFlag = 1;
			ME_addErrors(&ME,ERR_SYSTEM_LEVEL_SOURCE, SYS_LIFT_RELATIVE_ERROR, ERROR_SOURCE_LIFTS, 0);
		}


		//Error State
		if(ME.ErrorFlag == 1){
			ChangeState(&S,ERROR_STATE);
			break;
		}

		//UartLogging
		//if logging Enabled, no transmission is ongoing (cos we dont want to touch the rewrite the buffer
		//when transmission is ongoing), check if theres few data to add to the buffer. if there is , add it
		//to the buffer, and increment the buffer size. if there is still space for atleast one lift data frame,
		// then just go increment the motor your looking for new data from, otherwise transmit the data, and
		//wait for it to complete before you again start transmitting data.
		// we re sending about 2016 bytes in At 2Mb/s, and that takes about 1 ms. We re adding data at about
		//300 bytes every 50ms.so every 300ms we have 2000bytes of data and we do a transmission.
		if (S.LOG_enabled){
			Log_DoOneCycle();
		}


	} // closes while loop

}


//TODO: something if acks give issues.
/*if (response == 0){
	// start command didnt get an ack
}else if (response == 2){
	//setup command didnt get an ack
}*/
