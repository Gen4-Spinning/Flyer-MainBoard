/*
 * Log.c
 *
 *  Created on: May 13, 2023
 *      Author: harsha
 */

#include "Log.h"

uint8_t LOG_checkNewData(RunTime_TypeDef *r,Log *l,uint8_t motorID){
	if (r->rdngNo != l->mLog[motorID].loggedRdngNo){
		return 1;
	}
	return 0;
}


uint8_t Log_addDataToBuffer(RunTime_TypeDef *r,uint16_t bufferLocation,uint8_t motorID){
	if (motorID < 4){
		sprintf(LogBuffer + bufferLocation,"D,%01d,%05d,%04d,%04d,%04d,%05.02f,%06.02f,%03d,%03d,%05d,E\r\n",motorID,r->rdngNo,r->targetRPM,r->presentRPM,r->pwm,r->currentA,r->power,r->motorTemp,r->mosfetTemp,r->motorError);
		return PACKET_SIZE_NORMAL_MOTOR;
	}else{
		sprintf(LogBuffer +bufferLocation,"D,%01d,%05d,%06.02f,%06.02f,%04d,%04d,%05.02f,%06.02f,%03d,%03d,%05d,E\r\n",motorID,r->rdngNo,r->targetPosition,r->presentPosition,r->presentRPM,r->pwm,r->currentA,r->power,r->motorTemp,r->mosfetTemp,r->motorError);
		return PACKET_SIZE_LIFT_MOTOR;
	}
	return 0;
}

uint8_t Log_addSettingsDataToBuffer(machineSettingsTypeDef *m,uint16_t bufferLocation){
	sprintf(LogBuffer + bufferLocation,"S,%03d,%05.02f,%04.02f,%04.02f,%04.02f,%02d,%03d,%02d,%02d,%02d,%03d,%04.02f,%05.03f,E\r\n",m->spindleSpeed,m->tensionDraft,m->tpi,m->rovingWidth,m->deltaBobbinDia,m->bareBobbinDia,m->contentHeight,m->targetLayers,m->rampUpTime,m->rampDownTime,m->changeLayerTime,m->rtf,m->coneAngleFactor);
	return PACKET_SIZE_SETTINGS;
}

uint8_t Log_LayerChangeDataToBuffer(machineParamsTypeDef *mp,uint16_t bufferLocation){
	sprintf(LogBuffer + bufferLocation,"L,%02d,%06.02f,%06.02f,%06.02f,%04.02f,E\r\n",mp->currentLayer,mp->bobbinDia,mp->strokeHeight,mp->strokeTime,mp->strokeDistperSec);
	return PACKET_SIZE_LAYER;
}

uint8_t Log_StateChangeDataToBuffer(StateTypeDef *s,uint16_t bufferLocation){
	sprintf(LogBuffer + bufferLocation,"R,%02d,%01d,%01d,%06lu,E\r\n",s->current_state,s->runMode,s->BT_pauseReason,s->oneSecTimer);
	return PACKET_SIZE_RUNSTATE;
}

//this assumes atleast one motor is setup for logging.
uint8_t Log_changeLoggingMotor(Log *l){
	uint8_t currentMotor = l->loggingMotor ;

	while (1){
		currentMotor = currentMotor + 1;
		if (currentMotor > 5){
			currentMotor = 0;
		}
		if (l->mLog[currentMotor].motorRunning == 1){ // we have the next motor we want to log
			break;
		}
		if (currentMotor == l->loggingMotor){	// weve checked all motors, (in the case of only 1 motor being logged
			break;
		}
	}

	return currentMotor;
}


void Log_setUpLogging(Log *l,uint8_t *motorList,uint8_t noOfMotors){
	Log_disableLogging(l);
	for (int i=0;i<noOfMotors;i++){
		l->mLog[motorList[i]].motorRunning = 1;
		l->mLog[motorList[i]].newData = 0;
		l->mLog[motorList[i]].loggedRdngNo = 0;
	}
	L.loggingMotor = motorList[0];
	L.DMA_transferOver = 1;
}

void Log_disableLogging(Log *l){
	for (int i=0;i<6;i++){
		l->mLog[i].motorRunning = 0;
		l->mLog[i].newData = 0;
		l->mLog[i].loggedRdngNo = 0;
	}
}


void Log_DoOneCycle(void){
	/* if DMA is not happening, check if buffer has space for the largest packet. if Yes, if we need to send a
	 * layer log, send that, else send the motor Log.check if we have new data, if yes, add data to the buffer,
	 * and change the motor. if no new data keep waiting.
	 */
	if (L.DMA_transferOver == 1){
		if ((BUFFER_LOG_SIZE - L.bufferIdx) > PACKET_SIZE_LIFT_MOTOR){
			if (L.logLayerChange==1){
				L.bufferIdx  += Log_LayerChangeDataToBuffer(&mcParams,L.bufferIdx);
				L.logLayerChange = 0;
			}else if (L.logRunStateChange == 1){
				L.bufferIdx  += Log_StateChangeDataToBuffer(&S,L.bufferIdx);
				L.logRunStateChange = 0;
			}
			else if (L.flushBuffer){ // to be used only in idle mode and error Mode so that we can get all the data in the buffer
				//transmit Whatever Data is there.
				HAL_UART_Transmit_DMA(&huart2, (uint8_t *)LogBuffer, L.bufferIdx);
				L.DMA_transferOver = 0;
				L.bufferIdx = 0;
				L.flushBuffer = 0;
			}
			else{
				L.mLog[L.loggingMotor].newData = LOG_checkNewData(&R[L.loggingMotor],&L,L.loggingMotor);
				if(L.mLog[L.loggingMotor].newData){
					L.bufferIdx  += Log_addDataToBuffer(&R[L.loggingMotor],L.bufferIdx,L.loggingMotor);
					L.mLog[L.loggingMotor].loggedRdngNo = R[L.loggingMotor].rdngNo;
					// go to next motor.
					L.loggingMotor = Log_changeLoggingMotor(&L);
				} //closes new data
			} // closes layer or motor
		} //closes buffer size
		else{
			//transmitData.
			HAL_UART_Transmit_DMA(&huart2, (uint8_t *)LogBuffer, L.bufferIdx);
			L.DMA_transferOver = 0;
			L.bufferIdx = 0;
		}
	}
}


void Log_ResetRunTimeRdngNos(void){
	for (int i=0;i<6;i++){
		R[i].rdngNo = 0;
	}
}

void Log_ResetBufferIndex(Log *l){
	l->bufferIdx  = 0;
}



