/*
 * EepromFns.c
 *
 *  Created on: 06-Mar-2023
 *      Author: harsha
 */

#include "Eeprom.h"
#include "machineSettings.h"
#include "machineEepromSettings.h"

void ReadMachineSettingsFromEeprom(machineSettingsTypeDef *m)
{
	m->spindleSpeed = EE_ReadInteger(FF_SPINDLE_SPEED_ADDR);
	m->tensionDraft  = EE_ReadFloat(FF_TENSION_DRAFT_ADDR);
	m->tpi = EE_ReadFloat(FF_TPI_ADDR);
	m->rtf = EE_ReadFloat(FF_RTF_ADDR);
	m->targetLayers = EE_ReadInteger(FF_TARGET_LAYERS_ADDR);
	m->contentHeight = EE_ReadInteger(FF_BOBBIN_HEIGHT_ADDR);
	m->rovingWidth = EE_ReadFloat(FF_ROVING_WIDTH_ADDR);
	m->deltaBobbinDia = EE_ReadFloat(FF_DELTA_BOBBIN_DIA_ADDR);
	m->bareBobbinDia = EE_ReadInteger(FF_BARE_BOBBIN_DIA_ADDR);
	m->rampUpTime = EE_ReadInteger(RAMPUP_TIME_ADDR);
	m->rampDownTime = EE_ReadInteger(RAMPDOWN_TIME_ADDR);
	m->changeLayerTime = EE_ReadInteger(CHANGE_LAYER_RAMP_TIME_ADDR);
	m->coneAngleFactor = EE_ReadFloat(FF_CONE_ANGLE_FACTOR);
}


uint8_t WriteMachineSettingsIntoEeprom(machineSettingsTypeDef *m)
{
	uint8_t dataWritten = 0;
    dataWritten += EE_WriteInteger(m->spindleSpeed,FF_SPINDLE_SPEED_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->tensionDraft,FF_TENSION_DRAFT_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->tpi,FF_TPI_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->rtf,FF_RTF_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->targetLayers,FF_TARGET_LAYERS_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->contentHeight,FF_BOBBIN_HEIGHT_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->rovingWidth,FF_ROVING_WIDTH_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->deltaBobbinDia,FF_DELTA_BOBBIN_DIA_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->bareBobbinDia,FF_BARE_BOBBIN_DIA_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->rampUpTime,RAMPUP_TIME_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->rampDownTime,RAMPDOWN_TIME_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteInteger(m->changeLayerTime,CHANGE_LAYER_RAMP_TIME_ADDR);
    HAL_Delay(2);
    dataWritten += EE_WriteFloat(m->coneAngleFactor,FF_CONE_ANGLE_FACTOR);
       HAL_Delay(2);
    if (dataWritten == 13)
    	{return 0;}
    else{
    	return 1;}

}


uint8_t CheckMachineSettings(machineSettingsTypeDef* m){
	//typically when something goes wrong with the eeprom you get a value that is very high..
	//to allow for changes place to place without changing this code, we just set the thresholds to  2* maxRange.
	// dont expect in any place the nos to go higher than this..NEED TO PUT LOWER BOUNDS FOR EVERYTHING
	if ((m->spindleSpeed > MAX_SPINDLE_SPEED ) || (m->spindleSpeed < MIN_SPINDLE_SPEED)){
		return 0;
	}
	if (m->tensionDraft > 16.0f){
		return 0;
	}
	if (m->tpi > 15.0f){
		return 0;
	}
	if (m->rtf > 4.0f){
		return 0;
	}
	if (m->targetLayers > 100 || (m->targetLayers <= 0)){
		return 0;
	}
	if ((m->contentHeight > 500) || (m->contentHeight < 50)){
		return 0;
	}
	if (m->rovingWidth > 5){
		return 0;
	}
	if (m->deltaBobbinDia > 5){
		return 0;
	}
	if ((m->bareBobbinDia > 60) || (m->bareBobbinDia <44)){
		return 0;
	}
	if ((m->rampUpTime > 20) || (m->rampUpTime < 5)){
		return 0;
	}
	if ((m->rampDownTime > 20) || (m->rampDownTime < 5)){
		return 0;
	}
	if ((m->changeLayerTime > 3000) || (m->changeLayerTime < 200)){
		return 0;
	}
	if ((m->coneAngleFactor > 3) || (m->changeLayerTime < 0.1)){
		return 0;
	}
	return 1;
}

void LoadDefaultMachineSettings(machineSettingsTypeDef* m){
	m->spindleSpeed = DEFAULT_SPINDLESPEED;
	m->tensionDraft = DEFAULT_DRAFT;
	m->tpi = DEFAULT_TPI;
	m->rtf = DEFAULT_RTF;
	m->targetLayers = DEFAULT_ROVINGLAYERS;
	m->contentHeight = DEFAULT_MAXCONTENT_HEIGHT;
	m->rovingWidth = DEFAULT_ROVINGWIDTH;
	m->deltaBobbinDia = DEFAULT_DELTA_BOBBIN_DIA;
	m->bareBobbinDia = DEFAULT_BARE_BOBBIN_DIA;
	m->rampUpTime = DEFAULT_RAMPUP_TIME;
	m->rampDownTime = DEFAULT_RAMPDOWN_TIME;
	m->changeLayerTime = DEFAULT_CHANGE_LAYER_RAMP_TIME;
	m->coneAngleFactor = DEFAULT_CONE_ANGLE_FACTOR;
}
