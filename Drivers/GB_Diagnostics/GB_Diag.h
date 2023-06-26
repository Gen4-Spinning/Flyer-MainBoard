/*
 * GB_Diag.h
 *
 *  Created on: 04-May-2023
 *      Author: harsha
 */

#ifndef GB_DIAG_H_
#define GB_DIAG_H_
#include "stm32g4xx_hal.h"
#include "stdio.h"

#define SETHOMINGPOS 4
#define ONESHOT 3
#define CONTINUOUS	2
#define CONTINUOUS_STOP	 1


typedef struct GBDiagStruct
{
    uint16_t leftLiftPWM;
    float leftLift_DutyCycle;
	float liftLift_AbsPos;
	uint8_t homeSetupleftLift;

	uint8_t homeSetupSuccess;

	uint16_t rightLiftPWM;
	float rightLift_DutyCycle;
	float rightLift_AbsPos;
	uint8_t homeSetuprightLift;

	uint8_t BT_command;


}GB_diag;

extern GB_diag GBD;

#endif /* GB_DIAGNOSTICS_GB_DIAG_H_ */
