/*
 * MachineSensors.h
 *
 *  Created on: May 11, 2023
 *      Author: harsha
 */

#ifndef MACHINESENSORS_H_
#define MACHINESENSORS_H_

#include "mcp23017.h"

#define UNKNOWN_SENSOR 0
#define SLIVER_CUT_SENSOR 1

typedef struct {
	int8_t creelSensor;
	uint8_t latchedCreelSensor;
} SensorTypeDef;

extern SensorTypeDef sensor;

uint8_t Sensor_whichTriggered(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *whichSensor);
void Sensor_resetTriggeredStates(MCP23017_PortB *whichSensor);
int8_t Sensor_GetTriggerValue(MCP23017_HandleTypeDef *mcp, MCP23017_PortB *sensorVal,uint8_t sensor);



#endif /* MACHINESENSORS_H_ */
