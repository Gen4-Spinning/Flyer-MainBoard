/*
 * IntDriveComms.c
 *
 *  Created on: 17-Apr-2023
 *      Author: harsha
 */

#include "MotorComms.h"
#include "stdio.h"
#include "Struct.h"
#include "CommonConstants.h"
#include "CAN_MotherBoard.h"
#include "Ack.h"

void ReadySetupCommand_AllMotors(machineSettingsTypeDef *ms,machineParamsTypeDef *mp){

	SU[0].RPM = mp->flyerMotorRPM;
	SU[1].RPM = mp->bobbinMotorRPM;
	SU[2].RPM = mp->FR_MotorRPM;
	SU[3].RPM = mp->BR_MotorRPM;
	for (int i= 0;i<4;i++){
		SU[i].RDT = ms->rampDownTime;
		SU[i].RUT = ms->rampUpTime;
	}

	SU[4].StrokeLength = mp->strokeHeight;
	SU[4].StrokeTime = mp->strokeTime;
	SU[5].StrokeLength = mp->strokeHeight;
	SU[5].StrokeTime = mp->strokeTime;
	for (int i=4;i<6;i++){
		SU[i].direction = LIFT_UP;
		SU[i].liftRUT = ms->rampUpTime; // these are two are sent in seconds
		SU[i].liftRDT = ms->rampDownTime;
		SU[i].liftCRT = ms->changeLayerTime; // this is sent as uint16
	}

}

uint8_t  SendCommands_To_MultipleMotors(uint8_t *motorList,uint8_t motorArraySize,uint8_t command){
	/* Returns 0 if the command has ran into Ack errors and we have finally sent a STOP command/ or we ve just send a stop Command.
	 * Returns 1 if the command has run properly and we ve recieved the correct no of Acks for Setup/start/RD and RU
	 * Return 2 is the Setup Command has not recieved the right no of ACKS
	 *
	 * The CAN timer is a 15ms interrupt which is very large! when compared to the time
	 * needed to actually get an ack, so if we havent recieved its quite certain theres
	 * some fault. The CAN interrupt for the ack msg handles the success case by stopping
	 * the timer if all the acks are recieved.
	 */

	uint8_t noOfMotors = 0;
	uint16_t motorAcksCheck = 0;
	uint8_t canID;
	uint8_t motor;
	uint8_t motorAddresses[6]={};

	noOfMotors = motorArraySize;

	for (int i=0;i<noOfMotors;i++){
		canID = getMotorCANAddress(motorList[i]);
		if (canID == 0){
			canID = getLiftMotorCANAddress(motorList[i]);
		}
		motorAddresses[i] = canID;
		motorAcksCheck |= (1<< (canID-2));
	}

	if (command == START){
		reset_ACKs();
		//say ack for what - so that if it fails we know what it was looking for when it failed.
		ACK_startCheck(motorAcksCheck,ACK_FOR_SETUP_MM,NON_CRITICAL_ACK); // StartMM - MM is for multipleMotors
		for (int i=0;i<noOfMotors;i++){
			canID = motorAddresses[i];
			motor = motorList[i];
			if ((canID != 4) && (canID != 5)){
				FDCAN_sendSetUp_ToMotor(canID,SU[motor]);
			}else{
				FDCAN_sendSetUp_ToLiftMotor(canID,SU[motor]);
			}
			HAL_Delay(1); // is needed here or the second msg doesnt seem to go!
		}

		while(ack.waitingForAckResult){};

		if (ack.ackResult == ACK_SUCCESS){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_START_MM, NON_CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, START);
			}
			while(ack.waitingForAckResult){};

			if (ack.ackResult ==  ACK_FAIL){
				reset_ACKs();
				ACK_startCheck(motorAcksCheck,ACK_FOR_START_MM,CRITICAL_ACK);
				for (int i=0;i<noOfMotors;i++){
					canID = motorAddresses[i];
					FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
				}
				return 0;
			}
			return 1;
		}
		else{	// Didnt recieve Ack for Setup
			return 2;
			}

	} // closes START

	if (command == RAMPDOWN_STOP){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_RD_MM, NON_CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, RAMPDOWN_STOP);
			}
			while(ack.waitingForAckResult){};

			if (ack.ackResult ==  ACK_FAIL){
				reset_ACKs();
				ACK_startCheck(motorAcksCheck,ACK_FOR_RD_MM,CRITICAL_ACK);
				for (int i=0;i<noOfMotors;i++){
					canID = motorAddresses[i];
					FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
				}
				return 0;
			}
			return 1;
		}

		if (command == RESUME){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_RU_MM, NON_CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, RESUME);
			}
			while(ack.waitingForAckResult){};
			if (ack.ackResult ==  ACK_FAIL){
				reset_ACKs();
				ACK_startCheck(motorAcksCheck,ACK_FOR_RU_MM,CRITICAL_ACK);
				for (int i=0;i<noOfMotors;i++){
					canID = motorAddresses[i];
					FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
				}
				return 0;
			}
			return 1;
		}

		if (command == HOMING){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_HOMING_MM, NON_CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, HOMING);
			}
			while(ack.waitingForAckResult){};
			if (ack.ackResult ==  ACK_FAIL){
				reset_ACKs();
				ACK_startCheck(motorAcksCheck,ACK_FOR_HOMING_MM,CRITICAL_ACK);
				for (int i=0;i<noOfMotors;i++){
					canID = motorAddresses[i];;
					FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
				}
				return 0;
			}
			return 1;
		}

		if (command == EMERGENCY_STOP){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_STOP_MM,CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
			}
			return 0;
		}

	//should never reach here
	return 0;
}

uint8_t  Send_DiagCommands_To_MultipleMotors(uint8_t *motorList,uint8_t motorArraySize,uint8_t command){
	/* Returns 0 if the command has ran into Ack errors and we have finally sent a STOP command/ or we ve just send a stop Command.
	 * Returns 1 if the command has run properly and we ve recieved the correct no of Acks for Setup/start/RD and RU
	 * Return 2 is the Setup Command has not recieved the right no of ACKS
	 */

	uint8_t noOfMotors = 0;
	uint16_t motorAcksCheck = 0;
	uint8_t canID;
	uint8_t motorAddresses[6]={};

	noOfMotors = motorArraySize;

	for (int i=0;i<noOfMotors;i++){
		canID = getMotorCANAddress(motorList[i]);
		if (canID == 0){
			canID = getLiftMotorCANAddress(motorList[i]);
		}
		motorAddresses[i] = canID;
		motorAcksCheck |= (1<< (canID-2));
	}

	if (command == START){
		reset_ACKs();
		//say ack for what - so that if it fails we know what it was looking for when it failed.
		ACK_startCheck(motorAcksCheck,ACK_FOR_DIAG_SETUP,NON_CRITICAL_ACK);
		for (int i=0;i<noOfMotors;i++){
			canID = motorAddresses[i];
			if ((canID != 4) && (canID != 5)){
				FDCAN_SendDiagnostics_ToMotor(canID,&D);
			}else{
				FDCAN_SendDiagnostics_ToLiftMotor(canID,&D); //lift
			}
			HAL_Delay(1); // is needed here or the second msg doesnt seem to go!
		}


		while(ack.waitingForAckResult){};

		if (ack.ackResult == ACK_SUCCESS){
			reset_ACKs();
			ACK_startCheck(motorAcksCheck,ACK_FOR_DIAG_START, NON_CRITICAL_ACK);
			for (int i=0;i<noOfMotors;i++){
				canID = motorAddresses[i];
				FDCAN_sendCommand_ToMotor(canID, START);
			}
			while(ack.waitingForAckResult){};

			if (ack.ackResult ==  ACK_FAIL){
				//TODO detect which motor Failed.
				reset_ACKs();
				ACK_startCheck(motorAcksCheck,ACK_FOR_DIAG_START,CRITICAL_ACK);
				for (int i=0;i<noOfMotors;i++){
					canID = motorAddresses[i];
					FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
				}
				return 0;
			}
			return 1;
		}
		else{	// Didnt recieve Ack for Setup
			return 2;
			}

	} // closes START

	if (command == EMERGENCY_STOP){
		reset_ACKs();
		ACK_startCheck(motorAcksCheck,ACK_FOR_DIAG_STOP,CRITICAL_ACK);
		for (int i=0;i<noOfMotors;i++){
			canID = motorAddresses[i];
			FDCAN_sendCommand_ToMotor(canID, EMERGENCY_STOP);
		}
		return 0;
	}

	//should never reach here
	return 0;
}

