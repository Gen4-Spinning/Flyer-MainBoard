/*
 * StateMachine.h
 *
 *  Created on: May 2, 2023
 *      Author: harsha
 */

#ifndef INC_STATEMACHINE_H_
#define INC_STATEMACHINE_H_

#include "stdio.h"
#include "CommonConstants.h"

#define IDLE_SETUP 1
#define IDLE_RUN 2


#define RUN_OPERATING 1
#define RUN_PAUSED	2
#define RUN_STOPPED 3
#define RUN_OVER 4

#define TO_SETTINGS 1
#define TO_DIAGNOSTICS 2
#define TO_GB_CALIBRATION 3
#define TO_IDLE_FROM_GB_CALIB 4
#define TO_GB_CALIB_SAVE_LEFT 5
#define TO_GB_CALIB_SAVE_RIGHT 6

typedef struct StateStruct
{
	char oneTime;
	char current_state;
	char prev_state;
	char switchState;

	char idleMode;
	char runMode;

	char BT_dataOK;
	char BT_Paired;

	char HomingDoneCounter;
	char HomingDone;

	long oneSecTimer;
	char BT_sendState;
	char BT_pauseReason;
	char BT_runInfoToSend;
	uint8_t BT_runInfowhichMotor;
	char BT_paired;
	char BT_transmission_over;
	char BT_send_GBcalibData;

	char SMPS_OK_signal;
	char SMPS_cntrl;
	char SMPS_switchOff;

	char LOG_enabled;
}StateTypeDef;

extern StateTypeDef S;

void DebugState(void);
void RunState(void);
void SetupState(void);
void HomingState(void);
void IdleState(void);
void ErrorState(void);
void GB_CalibrationState(void);
void FinishState(void);

void InitializeState_TypeDef(StateTypeDef *s);
void ChangeState(StateTypeDef *s, uint8_t newState);
void ResetHomingState(StateTypeDef *s);

#endif /* INC_STATEMACHINE_H_ */
