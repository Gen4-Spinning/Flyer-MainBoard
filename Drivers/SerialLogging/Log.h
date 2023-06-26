/*
 * Log.h
 *
 *  Created on: May 13, 2023
 *      Author: harsha
 */

#ifndef LOG_H_
#define LOG_H_

#include "stdio.h"
#include "Struct.h"
#include "StateMachine.h"
#include "main.h"
#include "machineSettings.h"

extern UART_HandleTypeDef huart2;
extern char LogBuffer[2048];

#define BUFFER_LOG_SIZE 2048
#define PACKET_SIZE_NORMAL_MOTOR 55
#define PACKET_SIZE_LIFT_MOTOR 64
#define PACKET_SIZE_SETTINGS 60
#define PACKET_SIZE_LAYER 34
#define PACKET_SIZE_RUNSTATE 19

typedef struct motorLog{
	uint8_t motorRunning;
	uint16_t loggedRdngNo;
	uint16_t newData;
}MLog;

typedef struct LogData{
	MLog mLog[6];
	uint16_t bufferIdx;
	uint8_t loggingMotor;
	uint8_t DMA_transferOver;
	uint8_t logRdg;
	uint8_t logLayerChange;
	uint8_t logRunStateChange;
	uint8_t flushBuffer;
}Log;


extern Log L;

uint8_t LOG_checkNewData(RunTime_TypeDef *r,Log *l,uint8_t motorID);
uint8_t Log_addDataToBuffer(RunTime_TypeDef *r,uint16_t bufferLocation,uint8_t motorID);
uint8_t Log_changeLoggingMotor(Log *l);


void Log_setUpLogging(Log *l,uint8_t *motorList,uint8_t noOfMotors);
void Log_disableLogging(Log *l);
void Log_DoOneCycle(void);
void Log_ResetRunTimeRdngNos(void);
uint8_t Log_addSettingsDataToBuffer(machineSettingsTypeDef *m,uint16_t bufferLocation);
void Log_ResetBufferIndex(Log *l);

#endif /* LOG_H_ */
