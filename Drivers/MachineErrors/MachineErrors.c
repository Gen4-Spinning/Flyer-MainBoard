/*
 * MachineErrors.c
 *
 *  Created on: 08-May-2023
 *      Author: harsha
 */

#include "MachineErrors.h"

void InitializeMotherBoardErrors_TypeDef(MBErrorsTypeDef *e){
	e->EepromLoadValsGood = 0;
	e->defaults_eepromWriteFailed=0;
	e->AppSettings_parsingFailure = 0;
	e->AppSettings_eepromWriteFailure = 0;
}

void InitializeMachineErrors_TypeDef(MachineErrorsTypedef *m){
	m->ErrorFlag = 0;
}

uint16_t FindTopMotorError(MachineErrorsTypedef *m , uint16_t errorCode){
	for (int i=0;i<16;i++){
		if (((errorCode >> i) & 1) == 1){
			return 1<<i;
			break;
		}
	}
	return 0;
}

void ME_addErrors(MachineErrorsTypedef *m,uint8_t errType,uint16_t errorReason,uint16_t errorSource, uint16_t errorCode){
	if (m->errReason1 == EMPTY_ERROR){
		m->errType1 = errType;
		m->errReason1 = errorReason;
		m->errSource1 = errorSource;
		m->errCode1 = errorCode;
	}else if (m->errReason2 == EMPTY_ERROR){
		m->errType2 = errType;
		m->errReason2 = errorReason;
		m->errSource2 = errorSource;
		m->errCode2 = errorCode;
	}else{
		// if we get one of the errors that involve turning off the SMPS , and we dont have that in two, add it in
	}
}

void SetBTErrors(MachineErrorsTypedef *m,uint16_t errorReason,uint16_t errorSource,uint16_t errCode){
 m->BT_errorCode = errCode;
 m->BT_errorReason = errorReason;
 m->BT_errorSrc = errorSource;
}
