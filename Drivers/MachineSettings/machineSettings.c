/*
 * machineSettings.c
 *
 *  Created on: Apr 11, 2023
 *      Author: harsha
 */

#include "machineSettings.h"

void InitializeMachineSettings(machineSettingsTypeDef *ms){
    ms->spindleSpeed = 650;
    ms->tensionDraft = 8;
    ms->tpi = 1.1;
    ms->rtf = 1.0;
    ms->targetLayers = 600 ;
    ms->contentHeight = 345;
    ms->rovingWidth = 2.5;
    ms->deltaBobbinDia = 1.5;
    ms->bareBobbinDia = 48;
    ms->rampUpTime = 15;
    ms->rampDownTime = 10;
    ms->coneAngleFactor = 0.5; //added setting that is hard coded
}

void CalculateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *mp){
	mp->flyerMotorRPM = ms->spindleSpeed*SPINDLEMOTOR_TO_SPINDLE_RATIO;
	mp->delivery_mtr_min = (ms->spindleSpeed/ms->tpi) * 0.0254;
	mp->FR_RPM = (float)(mp->delivery_mtr_min*1000)/FR_CIRCUMFERENCE;
	mp->FR_MotorRPM = (uint16_t)(mp->FR_RPM *FRMOTOR_TO_FR_RATIO);
	mp->BR_MotorRPM = (uint16_t)((mp->FR_RPM * BR_CONSTANT)/(ms->tensionDraft/1.5));

	uint16_t maxLayers_1 = (uint16_t)(ms->contentHeight/ms->rovingWidth); // for stroke Ht != 0
	uint16_t maxLayers_2 = (uint16_t)((MAX_CONTENT_DIA_MM - ms->bareBobbinDia)/ms->deltaBobbinDia); // for bobbin Circumference= max Width
	if (maxLayers_1 >= maxLayers_2){
		mp->maxLayers = maxLayers_2  - OFFSET_FROM_MAXLAYERS;
	}else{
		mp->maxLayers = maxLayers_1  - OFFSET_FROM_MAXLAYERS;
	}

	mp->bobbinDia = ms->bareBobbinDia + mp->currentLayer*ms->deltaBobbinDia;
	mp->deltaRpm_Spindle_Bobbin = (mp->delivery_mtr_min*1000)/(mp->bobbinDia * 3.14159);
	mp->bobbinRPM = ms->spindleSpeed + mp->deltaRpm_Spindle_Bobbin;
	mp->bobbinMotorRPM = mp->bobbinRPM * BOBBINMOTOR_TO_BOBBIN_RATIO;

	mp->strokeHeight = ms->contentHeight - ((ms->rovingWidth*ms->coneAngleFactor) * mp->currentLayer);
	mp->strokeDistperSec = (mp->deltaRpm_Spindle_Bobbin * ms->rovingWidth)/60.0;
	mp->strokeTime = mp->strokeHeight/mp->strokeDistperSec;
	mp->liftMotorRPM = (uint16_t)(((float)(mp->strokeDistperSec *60.0)/LIFT_SCREW_PITCH_MM)*LIFT_MOTOR_TO_LIFT_SCREW_RATIO);

	mp->targetAbsPosition = ms->contentHeight;
}

void SetupMachineParametersForFirstLayer(machineParamsTypeDef *mp){
	mp->currentDir = LIFT_UP;
	mp->currentLayer = 0;
}

void UpdateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *mp,uint8_t layerNo){
	mp->currentLayer = layerNo;
	mp->bobbinDia = ms->bareBobbinDia + mp->currentLayer*ms->deltaBobbinDia;
	mp->deltaRpm_Spindle_Bobbin = (mp->delivery_mtr_min*1000)/(mp->bobbinDia * 3.14159);
	mp->bobbinRPM = ms->spindleSpeed + mp->deltaRpm_Spindle_Bobbin;
	mp->bobbinMotorRPM = mp->bobbinRPM * BOBBINMOTOR_TO_BOBBIN_RATIO;

	mp->strokeHeight = ms->contentHeight - ((ms->rovingWidth*ms->coneAngleFactor) * mp->currentLayer);
	mp->strokeDistperSec = (mp->deltaRpm_Spindle_Bobbin * ms->rovingWidth)/60.0;
	mp->strokeTime = mp->strokeHeight/mp->strokeDistperSec;
	mp->liftMotorRPM = (uint16_t)(((float)(mp->strokeDistperSec *60.0)/LIFT_SCREW_PITCH_MM)*LIFT_MOTOR_TO_LIFT_SCREW_RATIO);

	if(mp->currentDir == LIFT_DOWN){
		mp->targetAbsPosition = mp->targetAbsPosition - mp->strokeHeight;
	}else{
		mp->targetAbsPosition = mp->targetAbsPosition + mp->strokeHeight;
	}
}

void CalculateTotalRunTime(machineSettingsTypeDef *ms ,machineParamsTypeDef *mp){

	float bobbinDia;
	float deltaRPM;

	float strokeHeight;
	float strokeDist_sec;
	float strokeTime;
	float totalTime_s;

	totalTime_s = 0;
	for (int i=0;i<mp->maxLayers; i++){
		bobbinDia = ms->bareBobbinDia + i*ms->deltaBobbinDia;
		deltaRPM = (mp->delivery_mtr_min*1000)/(bobbinDia * 3.14159);
		strokeHeight = ms->contentHeight - ((ms->rovingWidth*ms->coneAngleFactor) * i);
		strokeDist_sec = (deltaRPM * ms->rovingWidth)/60.0;
		strokeTime = strokeHeight/strokeDist_sec;
		totalTime_s += strokeTime;
	}
	mp->totalRunTime_Min = totalTime_s/60.0;
}


uint8_t getMotorCANAddress(uint8_t motor){
	if (motor == FLYER){
		return 2;
	}else if (motor == BOBBIN){
		return 3;
	}else if (motor == FR){
		return 6;
	}else if (motor == BR){
		return 7;
	}else{
		return 0;
	}
}

uint8_t getLiftMotorCANAddress(uint8_t motor){
	if (motor == LEFT_LIFT){
		return 4;
	}else if (motor == RIGHT_LIFT){
		return 5;
	}else{
		return 0;
	}
}

uint8_t GetMotorID_from_CANAddress(uint8_t canAddress){
	if (canAddress == FLYER_ADDRESS){
		return FLYER;
	}else if (canAddress == BOBBIN_ADDRESS){
		return BOBBIN;
	}else if (canAddress == FRONTROLLER_ADDRESS){
		return FR;
	}else if (canAddress == BACKROLLER_ADDRESS){
		return BR;
	}else if (canAddress == LEFTLIFT_ADDRESS){
		return LEFT_LIFT;
	}else if (canAddress == RIGHTLIFT_ADDRESS){
		return RIGHT_LIFT;
	}
	return 6; // return
}
