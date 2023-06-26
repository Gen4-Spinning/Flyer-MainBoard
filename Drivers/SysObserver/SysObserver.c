/*
 * SysObserver.c
 *
 *  Created on: 05-May-2023
 *      Author: harsha
 */

#include "SysObserver.h"
#include "Struct.h"

extern TIM_HandleTypeDef htim6;
extern TIM_HandleTypeDef htim16;

uint8_t SO_CheckLiftRelativeError(SysObserver *so,RunTime_TypeDef *leftLift_r,RunTime_TypeDef *rightLift_r){
	if (so->initialLiftPosRecieved == 1){
		so->liftsPosDifference = leftLift_r->presentPosition - rightLift_r->presentPosition;
		if (fabs(so->liftsPosDifference) > 4){
			so->liftRelativePosError = 1;
		}
	}
	return so->liftRelativePosError;
}

void SO_Start_LiftRelativeError_Delay(void){
	htim6.Instance->SR &= ~TIM_SR_UIF; // Clear pending flag of timer6
	HAL_TIM_Base_Start_IT(&htim6); // start the 500ms tim6
}

void SO_Reset_InitialLiftPosRecieved(SysObserver *so){
	so->initialLiftPosRecieved = 0;
}


void SO_enableCANObservers(SysObserver *so,uint8_t *motorList,uint8_t noOfMotors){
	for (int i=0;i<noOfMotors;i++){
		so->CO[motorList[i]].enable = 1;
		so->CO[motorList[i]].canDataCount = 0;
	}

	__HAL_TIM_SET_COUNTER(&htim16,0);
	htim16.Instance->SR &= ~TIM_SR_UIF; // Clear pending flag of timer16
	HAL_TIM_Base_Start_IT(&htim16); // start the 1s tim16 in which we check the CAN states

}

void SO_disableAndResetCANObservers(SysObserver *so){
	for (int i=0;i<6;i++){
		so->CO[i].enable = 0;
		so->CO[i].canDataCount = 0;
	}
	HAL_TIM_Base_Stop_IT(&htim16);
}

void SO_incrementCANCounter(SysObserver *so,uint8_t motorID){
	so->CO[motorID].canDataCount++;
}

uint8_t SO_checkCanObservers(SysObserver *so){
	//at40ms ->about 25 data per sec, at 60ms -> about18 data per sec
	//so conservatively ten.
	//We found that when the lifts stop the data rate can go very low. so we just do 5.
	//returns i, the motor which has failed. if nothing has failed returns 99- NO FAIL
	for (int i = 0;i<6;i++){
		if (so->CO[i].enable == 1){
			so->CO[i].lastCount = so->CO[i].canDataCount;
			if (so->CO[i].canDataCount <= 5){
				return i;
			}
			so->CO[i].canDataCount = 0;
		}
	}
	return ALL_CANS_HEALTHY;
}


