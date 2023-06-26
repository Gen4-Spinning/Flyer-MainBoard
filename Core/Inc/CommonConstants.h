/*
 * CommonConstants.h
 *
 *  Created on: 07-Mar-2023
 *      Author: harsha
 */
#include "main.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "Struct.h"
#ifndef INC_COMMONCONSTANTS_H_
#define INC_COMMONCONSTANTS_H_

// State machine defines
#define INITIAL_STATE 0
#define DEBUG_STATE 1
#define IDLE_STATE 2
#define RUN_STATE 3
#define DIAGNOSTICS_STATE 4
#define PAUSE_STATE 5
#define FINISHED_STATE 6
#define ERROR_STATE 7
#define SETTINGS_STATE 8
#define HOMING_STATE 9
#define GB_CALIB_STATE 10

//FOR DIAGNOSTICS AND DEBUGGING
#define OPENLOOP 1
#define CLOSEDLOOP 2

#define MOVEUP 1
#define MOVEDOWN 0

//unrecoverable Errors that go into Error Handler
#define CAN_START_ERROR 1
#define CAN_IT_ACTIVATE_ERROR 2
#define CAN_FILTER_CONFIG_ERROR 3

#define MOTORBRD_CURRENT_GAIN 0.00672
#define MOTORBRD_VOLTAGE_GAIN 0.017

//MSGS TO SEND TO MOTORS
#define EMERGENCY_STOP 1 // Emergency Stop
#define START 2 		// Ramp and start
#define RAMPDOWN_STOP 3 // RAMP and stop
#define IDLE 4
#define HOMING 5
#define RESUME 6

void SetupState(void);
void DiagnosticsState(void);
void SettingsState(void);

void calculateRunValues(void);
//void reCalculateRunValues(RunSetupDataTypDef *RD, LiftRunSetupDataTypDef *LRD, LiftRunTime_TypeDef *LR);

#endif /* INC_COMMONCONSTANTS_H_ */
