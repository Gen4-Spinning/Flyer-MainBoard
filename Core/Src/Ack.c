/*
 * Ack.c
 *
 *  Created on: 10-Apr-2023
 *      Author: Jonathan
 */

#include "Ack.h"
#include "main.h"
#include "string.h"

void reset_ACKs(void){
 ack.motorAcksCheck = 0;
 ack.motorAcksRecvd = 0;
 ack.ackResult = 0;
 ack.ackChkType = 0;
}

//starts the timer that overflows by which time the acks should have come.
// also sets whether the ack failing should stop the smps or not, and what motors the ack is expected to come from.
void ACK_startCheck(uint16_t ackCheck,char ackForWhat, uint8_t ackType){
	ack.motorAcksCheck = ackCheck;
	ack.waitingForAckResult = 1;
	ack.ackChkType = ackType;
	ack.ackForWhat = ackForWhat;

	__HAL_TIM_SET_COUNTER(&htim17,0);
	htim17.Instance->SR &= ~TIM_SR_UIF; // Clear pending flag of timer15
	HAL_TIM_Base_Start_IT(&htim17); // start the 8ms tim17;
	//tim17 priority has to be much lower than CAN priority- ive kept 1 and 5

}






