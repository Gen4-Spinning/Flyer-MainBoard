/*
 * IntDriveComms.h
 *
 *  Created on: 17-Apr-2023
 *      Author: harsha
 */

#ifndef MOTORCOMMS_H_
#define MOTORCOMMS_H_

#include "machineSettings.h"

typedef struct Setup_NormalMotor_Struct{
	uint16_t RUT;
	uint16_t RDT;
	uint16_t RPM;
	float StrokeLength;
	float StrokeTime;
	uint8_t direction;
	uint8_t liftRUT;
	uint8_t liftRDT;
	uint16_t liftCRT;
}SetupMotor; // Extended. Modify for other machines

extern SetupMotor SU[6];

void ReadySetupCommand_AllMotors(machineSettingsTypeDef *ms,machineParamsTypeDef *mp);
uint8_t SendCommands_To_MultipleMotors(uint8_t *motorList,uint8_t motorArraySize,uint8_t command);
uint8_t Send_DiagCommands_To_MultipleMotors(uint8_t *motorList,uint8_t motorArraySize,uint8_t command);

void Modify_Setup_ForInching(void);
void Reset_SetupCommands_afterInching(machineSettingsTypeDef *ms);

#endif /* MOTORCOMMS_H_ */
