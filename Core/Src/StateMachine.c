/*
 * StateMachine.c
 *
 *  Created on: May 2, 2023
 *      Author: harsha
 */

#ifndef STATEMACHINE_C_
#define STATEMACHINE_C_

#include "StateMachine.h"
#include "BT_Fns.h"

void InitializeState_TypeDef(StateTypeDef *s){
	s->current_state = IDLE_STATE;
	s->prev_state = INITIAL_STATE;

	s->idleMode  = 0;
	s->oneTime = 0;

	s->BT_paired = 0;
	s->BT_dataOK = 0;
	s->BT_pauseReason = 0;
	s->BT_runInfoToSend = RUN_PRODUCTION_DATA;
	s->BT_runInfowhichMotor = 0;
	s->runMode = 0;

	s->SMPS_switchOff = 0;
	s->SMPS_OK_signal = 0;
	s->SMPS_cntrl = 0;;

}

void ChangeState(StateTypeDef *s, uint8_t newState){
	s->prev_state = s->current_state;
	s->current_state = newState;
	s->oneTime = 1;
}

 void ResetHomingState(StateTypeDef *s){
	 s->HomingDone  = 0;
	 s->HomingDoneCounter = 0;
 }

#endif /* STATEMACHINE_C_ */
