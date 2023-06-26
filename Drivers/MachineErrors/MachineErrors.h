/*
 * MachineErrors.h
 *
 *  Created on: 08-May-2023
 *      Author: harsha
 */

#ifndef MACHINEERRORS_H_
#define MACHINEERRORS_H_

#include "stdio.h"

//errType
#define ERR_MOTOR_SOURCE 1
#define ERR_SYSTEM_LEVEL_SOURCE 2

//errReason
#define EMPTY_ERROR 0
#define ERR_OC_SHIFT 1	//over current
#define ERR_OV_SHIFT 2	// over Voltage
#define ERR_UV_SHIFT 3  // under Voltage
#define ERR_MTF_SHIFT 4	// motor Thermistor Fault
#define ERR_FTF_SHIFT 5	// Fet thermistor Fault
#define ERR_MOT_SHIFT 6	// motor over temperature
#define ERR_FOT_SHIFT 7 // fet over temperature
#define ERR_EWE_SHIFT 8 // eeprom writing error
#define ERR_EBV_SHIFT 9 // eeprom Bad Values
#define ERR_TE_SHIFT 10 // tracking error
#define ERR_MES_SHIFT 11 // motor encoder setup fail
#define ERR_LTE_SHIFT 12 // lift tracking error
#define ERR_LSF_SHIFT 13 // Lift GB-motorEncoder pos Calculation synchrocity fail
#define ERR_LOB_SHIFT 14 // Lift out of bounds
#define ERR_EBH_SHIFT 15 // eeprom Bad homing values
#define SYS_LIFT_RELATIVE_ERROR 99
#define SYS_CAN_CUT_ERROR 98
#define SYS_ACK_ERROR 97
#define SYS_SMPS_ERROR 96

//errSource
#define ERROR_SOURCE_MOTHERBOARD 11
#define ERROR_SOURCE_CAN_BUS 12 // NOT USED
#define ERROR_SOURCE_LIFTS 13
#define ERROR_SOURCE_SYSTEM 14

// MotherBoard Error
typedef struct MotherBoardErrors_Struct
{
	char EepromLoadValsGood; // we got good readings when we started from the eeprom
	char defaults_eepromWriteFailed; // we got bad readings but the defaults were written properly
	char AppSettings_parsingFailure;
	char AppSettings_eepromWriteFailure;
}MBErrorsTypeDef;


typedef struct MachineErrors
{
	char ErrorFlag;

	uint8_t errType1;
	uint16_t errSource1;
	uint16_t errReason1;
	uint16_t errCode1;

	uint8_t errType2;
	uint16_t errSource2;
	uint16_t errReason2;
	uint16_t errCode2;

	uint16_t errCount;

	char BT_errorSrc;
	uint16_t BT_errorReason;
	uint16_t BT_errorCode;
}MachineErrorsTypedef;

extern MachineErrorsTypedef ME;
extern MBErrorsTypeDef MBE;

void InitializeMotherBoardErrors_TypeDef(MBErrorsTypeDef *e);
uint16_t FindTopMotorError(MachineErrorsTypedef *m , uint16_t errorCode);
void SetBTErrors(MachineErrorsTypedef *m,uint16_t errorReason,uint16_t errorSource,uint16_t errCode);
void ME_addErrors(MachineErrorsTypedef *m,uint8_t errType,uint16_t errorReason,uint16_t errorSource, uint16_t errorCode);
#endif /* MACHINEERRORS_MACHINEERRORS_H_ */
