/*
 * SysObserver.h
 *
 *  Created on: 05-May-2023
 *      Author: harsha
 */

#ifndef SYSOBSERVER_H_
#define SYSOBSERVER_H_

#include "machineSettings.h"
#include "Struct.h"
#include "math.h"

#define ALL_CANS_HEALTHY 99

typedef struct canObserverStruct{
	uint8_t canDataCount;  //we expect only about 20 data per sec (40 and 60 ms)
	uint8_t enable;
	uint8_t lastCount;
}canObserver;

typedef struct SysObser_Struct{

	char initialLiftPosRecieved;
	float liftsPosDifference;
	uint8_t liftRelativePosError;
	char liftStrokeOverMsgsRecieved;

	char canOverallStatus;
	canObserver CO[6];

}SysObserver;

extern SysObserver SO;


uint8_t SO_CheckLiftRelativeError(SysObserver *so,RunTime_TypeDef *leftLift_r,RunTime_TypeDef *rightLift_r);
void SO_Start_LiftRelativeError_Delay(void);
void SO_Reset_InitialLiftPosRecieved(SysObserver *so);

void SO_enableCANObservers(SysObserver *so,uint8_t *motorList,uint8_t noOfMotors);
void SO_disableAndResetCANObservers(SysObserver *so);
void SO_incrementCANCounter(SysObserver *so,uint8_t motorID);
uint8_t SO_checkCanObservers(SysObserver *so);

#endif /* SYSOBSERVER_H_ */
