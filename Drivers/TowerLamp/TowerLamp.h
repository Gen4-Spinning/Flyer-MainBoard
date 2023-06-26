/*
 * TowerLamp.h
 *
 *  Created on: 10-May-2023
 *      Author: harsha
 */

#ifndef TOWERLAMP_H_
#define TOWERLAMP_H_

#include "mcp23017.h"

#define TOWER_BUZZER 1
#define TOWER_GREEN 2
#define TOWER_RED 3
#define TOWER_AMBER 4


#define BUZZER_ON 1
#define BUZZER_OFF 0

#define GREEN_ON 1
#define GREEN_OFF 0

#define RED_ON 1
#define RED_OFF 0

#define AMBER_ON 1
#define AMBER_OFF 0

void TowerLamp_SetState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb,int8_t buzzerOn,int8_t redOn, int8_t greenOn,int8_t amberOn);
void TowerLamp_ApplyState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb);
void TowerLamp_NegateState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb,uint8_t light);


#endif /* TOWERLAMP_H_ */
