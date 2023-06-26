/*
 * FDCAN.h
 *
 *  Created on: Mar 5, 2023
 *      Author: Jonathan
 */

#ifndef FDCAN_FDCAN_H_
#define FDCAN_FDCAN_H_

#include "stm32g4xx_hal.h"

#define MOTHERBOARD_ADDRESS		0x01
#define FLYER_ADDRESS			0x02
#define BOBBIN_ADDRESS			0x03
#define LEFTLIFT_ADDRESS		0x04
#define RIGHTLIFT_ADDRESS		0x05
#define FRONTROLLER_ADDRESS 	0x06
#define BACKROLLER_ADDRESS	 	0x07

#define MOTORSTATE_FUNCTIONID			0x01
#define ERROR_FUNCTIONID				0x02
#define DRIVECHECK_FUNCTIONID			0x03
#define DRIVECHECK_RESPONSE_FUNCTIONID	0x04
#define SETUP_CALLBACK_FUNCTIONID		0x05
#define TUNINGDATA_FUNCTIONID			0x06
#define RUNSETUPDATA_FUNCTIONID			0x07
#define ANALYSISDATA_FUNCTIONID			0x08
#define RUNTIMEDATA_FUNCTIONID			0x09
#define DIAGNOSTICSDATA_FUNCTIONID 		0x0A
#define LIFTRUNTIMEDATA_FUNCTIONID		0x0C
#define CHANGETARGET_FUNCTIONID			0x0D
#define LIFTSTROKEOVER_FUNCTIONID		0x0E
#define ACKFRAME_FUNCTIONID				0x0F
#define LIFTANALYSIS_FUCTIONID			0x11
#define HOMINGDONEFRAME_FUNCTIONID		0x13
#define DIAGNOSTICSDONEFRAME_FUNCTIONID	0X14
#define LIFTNEWSTROKEFRAME_FUNCTIONID	0x15
#define LIFTSTARTSENDINGGBD_FUNCTIONID	0x16
#define LIFTGBD_FUNCTIONID				0x17//GBD means gearbox data

#define PRIORITY1 0x06//priority1 has the highest priority
#define PRIORITY2 0x0A
#define PRIORITY3 0x0E


#define MOTHERBOARD_RECIEVE_MASK 0x00000100 //this is the value you want in the mask
#define MOTHERBOARD_RECIEVE_FILTER 0x0000FF00 //this is the position you want to mask


void FDCAN_TxInit(void);
void FDCAN_RxFilterInit(void);
uint32_t FDCAN_generateIdentifier(uint16_t source, uint16_t destination, uint16_t functionID, uint8_t priority);

//variables that you want else where define as extern here
extern FDCAN_HandleTypeDef hfdcan1;
extern FDCAN_RxHeaderTypeDef   RxHeader;
extern FDCAN_TxHeaderTypeDef   TxHeader;

extern uint8_t TxData[16];
extern uint8_t RxData[16];
extern uint32_t functionID;
extern uint32_t source_address;


#endif /* FDCAN_FDCAN_H_ */
