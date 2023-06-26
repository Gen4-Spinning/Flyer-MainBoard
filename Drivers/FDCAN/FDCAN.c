/*
 * FDCAN.c
 *
 *  Created on: Mar 5, 2023
 *      Author: Jonathan
 */

#include "FDCAN.h"
#include "CommonConstants.h"
#include "Struct.h"
#include "main.h"


//CAN variables here
uint32_t functionID,source_address, destination_address,data_size;
uint8_t TxData[16];

FDCAN_TxHeaderTypeDef   TxHeader;
FDCAN_RxHeaderTypeDef   RxHeader;
uint8_t RxData[16];


void FDCAN_TxInit(void)
{
	if(HAL_FDCAN_Start(&hfdcan1)!= HAL_OK){
	  Error_Handler();}
	if (HAL_FDCAN_ActivateNotification(&hfdcan1, FDCAN_IT_RX_FIFO0_NEW_MESSAGE, 0) != HAL_OK){
	 Error_Handler();
	}
    TxHeader.Identifier = 0x0E090102;
    TxHeader.IdType = FDCAN_EXTENDED_ID;
    TxHeader.TxFrameType = FDCAN_DATA_FRAME;
    TxHeader.DataLength = FDCAN_DLC_BYTES_8;
    TxHeader.ErrorStateIndicator = FDCAN_ESI_ACTIVE;
    TxHeader.BitRateSwitch = FDCAN_BRS_ON;
    TxHeader.FDFormat = FDCAN_FD_CAN;
    TxHeader.TxEventFifoControl = FDCAN_NO_TX_EVENTS;
    TxHeader.MessageMarker = 0;
}

void FDCAN_RxFilterInit(void)
{
	  FDCAN_FilterTypeDef sFilterConfig;

	  sFilterConfig.IdType = FDCAN_EXTENDED_ID;
	  sFilterConfig.FilterIndex = 0;
	  sFilterConfig.FilterType = FDCAN_FILTER_MASK; //FilterID1 = filter, FilterID2 = mask
	  sFilterConfig.FilterConfig = FDCAN_FILTER_TO_RXFIFO0;
	  sFilterConfig.FilterID1 = MOTHERBOARD_RECIEVE_MASK;
	  sFilterConfig.FilterID2 = MOTHERBOARD_RECIEVE_FILTER;
	  if (HAL_FDCAN_ConfigFilter(&hfdcan1, &sFilterConfig) != HAL_OK){
	    /* Filter configuration Error */
	    Error_Handler();
	  }
	  HAL_FDCAN_ConfigGlobalFilter(&hfdcan1,FDCAN_REJECT,FDCAN_REJECT,FDCAN_REJECT_REMOTE,FDCAN_REJECT_REMOTE);
}

uint32_t FDCAN_generateIdentifier(uint16_t source, uint16_t destination, uint16_t functionID, uint8_t priority)
{
	uint32_t temp_identifier;
	temp_identifier=(priority<<24)|(functionID<<16)|(destination<<8)|(source);
	return(temp_identifier);

}





