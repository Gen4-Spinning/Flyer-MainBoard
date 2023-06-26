/*
 * TowerLamp.c
 *
 *  Created on: 10-May-2023
 *      Author: harsha
 */

#include "TowerLamp.h"

void TowerLamp_SetState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb,int8_t buzzerOn,int8_t redOn, int8_t greenOn,int8_t amberOn){
	mcp23017_read(mcp, MCP_GPIOB,mcp->outputGPIO);
	pb->raw = mcp->outputGPIO[0];

	if (buzzerOn != -1){
		pb->values.towerBuzzer = buzzerOn;
	}
	if (redOn != -1){
		pb->values.towerRed = redOn;
	}
	if (greenOn != -1){
		pb->values.towerGreen = greenOn;
	}
	if (amberOn != -1){
		pb->values.towerAmber = amberOn;
	}
}

void TowerLamp_ApplyState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb){
	 mcp->outputGPIO[0] = pb->raw;
	 mcp23017_write(mcp, OLATB,mcp->outputGPIO);
}


void TowerLamp_NegateState(MCP23017_HandleTypeDef *mcp,MCP23017_PortB *pb,uint8_t light){

	mcp23017_read(mcp, MCP_GPIOB,mcp->outputGPIO);
	pb->raw = mcp->outputGPIO[0];

	if (light == TOWER_GREEN){
		pb->values.towerGreen = !pb->values.towerGreen;
	}
	if (light == TOWER_RED){
		pb->values.towerRed = !pb->values.towerRed;
	}
	if (light == TOWER_AMBER){
		pb->values.towerAmber = !pb->values.towerAmber;
	}
}
