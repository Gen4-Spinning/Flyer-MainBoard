/*
 * Ack.h
 *
 *  Created on: 10-Apr-2023
 *      Author: Jonathan
 */

#ifndef INC_ACK_H_
#define INC_ACK_H_

#include "stdio.h"
#include "stm32g4xx_hal.h"

extern TIM_HandleTypeDef htim17;

#define CRITICAL_ACK 1
#define NON_CRITICAL_ACK 2

#define ACK_SUCCESS 1
#define ACK_FAIL 2

//ackforWhats
#define ACK_FOR_SETUP_MM 1
#define ACK_FOR_START_MM 2
#define ACK_FOR_RD_MM 3
#define ACK_FOR_RU_MM 4
#define ACK_FOR_HOMING_MM 5
#define ACK_FOR_STOP_MM 6
#define ACK_FOR_DIAG_SETUP 7
#define ACK_FOR_DIAG_START 8
#define ACK_FOR_DIAG_STOP 9


typedef struct ACK_Struct{
	uint16_t motorAcksCheck;
	uint16_t motorAcksRecvd;
	uint8_t ackChkType;
	char waitingForAckResult;
	char ackResult;
	char ackForWhat;
	uint8_t motorNACKID;
	uint8_t ackErrorMotor;
}ACKTypeDef;

void reset_ACKs(void);
void ACK_startCheck(uint16_t ackCheck,char ackForWhat,uint8_t ackType);


extern ACKTypeDef ack;

#endif /* INC_ACK_H_ */
