/*
 * InitializeTypeDefs.c
 *
 *  Created on: 06-Mar-2023
 *      Author: harsha
 */

#include "Struct.h"
#include "CommonConstants.h"
#include "BT_Machine.h"

void InitializeDiagnostic_TypeDef(DiagnosticsTypeDef *d){
	d->motorID = 0;
	d->targetDuty=0;
	d->runTime = 0;
	d->typeofTest = 0;
	d->direction = BT_DEFAULT_DIR;
	d->bedDist = 0;
}


void InitializeRunTime_TypeDef(void){
	for (int i=0;i<6;i++){
		R[i].targetRPM = 0;
		R[i].presentRPM = 0;
		R[i].pwm = 0;
		R[i].mosfetTemp = 0;
		R[i].motorTemp = 0;
		R[i].currentRaw = 0;
		R[i].voltageRaw = 0;
		R[i].currentA = 0;
		R[i].voltageV = 0;
		R[i].proportionalTerm = 0;
		R[i].integralTerm = 0;
		R[i].feedForwardTerm = 0;
	}
}


