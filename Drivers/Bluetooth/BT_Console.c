/*
 * BT_Console.c
 *
 *  Created on: 22-Jun-2023
 *      Author: harsha
 */



#include "BT_Console.h"
extern UART_HandleTypeDef huart1;

char BTCmdString[8];

uint8_t ParseCmdModeMsg(BTConsole *btc){
	if (btc->btPaired == 0){
		if (btc->inCMDMode == 0){
			if (btc->cmdModeMsgSent == BTCMD_ENTER){
				strncpy(BTCmdString,BufferRec,4);
				BTCmdString[4] = '\0';
				if (strcmp(BTCmdString,CMD_MODE_RESPONSE_STRING) == 0){
					btc->inCMDMode = 1;
					return 1;
				}
			}
			else{
				strncpy(BTCmdString,BufferRec+1,8);
				BTCmdString[7] = '\0';
				if (strcmp(BTCmdString,BT_CONNECT_MSG) == 0){
					btc->btPaired = 1;
					return 1;
				}
			}
		}else{
			if (btc->cmdModeMsgSent == BTCMD_EXIT){
				strncpy(BTCmdString,BufferRec,3);
				BTCmdString[3] = '\0';
				if (strcmp(BTCmdString,CMD_MODE_END_STRING) == 0){
					btc->inCMDMode = 0;
					return 1;
				}
			}else if (btc->cmdModeMsgSent == BTCMD_REBOOT){
				strncpy(BTCmdString,BufferRec,6);
				BTCmdString[6] = '\0';
				if (strcmp(BTCmdString,CMD_MODE_REBOOT_STRING) == 0){
					btc->btRebooted = 1;
					btc->inCMDMode = 0;
					//make all other vars also 0
					btc->cmdModeMsgSent = 0;
					btc->btNameSameAsMachineName = 0;
					btc->btSetCmdApplied = 0;
					return 1;
				}
			}else if (btc->cmdModeMsgSent == BTCMD_GETNAME){
				strncpy(BTCmdString,BufferRec,NAME_STR_LEN);
				BTCmdString[NAME_STR_LEN] = '\0';
				if (strcmp(BTCmdString,MACHINENAME) == 0){
					btc->btNameSameAsMachineName = 1;
					return 1;
				}
			}else{
				strncpy(BTCmdString,BufferRec,3);
				BTCmdString[3] = '\0';
				if (strcmp(BTCmdString,CMD_MODE_AOK_STRING) == 0){
					btc->btSetCmdApplied = 1;
					return 1;
				}
			}
		}
	}else{ // closes if bt paired == 0
		//TAKES SOME TIME FOR THE MSG TO COME
		strncpy(BTCmdString,BufferRec+1,8);
		BTCmdString[7] = '\0';
		if (strcmp(BTCmdString,BT_DISCONNECT_MSG) == 0){
			btc->btPaired = 0;
			return 1;
		}
	}
	return 0;
} // closes Fn


uint8_t BTConsole_WriteCMD(uint8_t command){
	uint8_t result = 0;
	BTCmd.btTickOn = 1;
	if (command==BTCMD_ENTER){
		BTCmd.cmdModeMsgSent = BTCMD_ENTER;
		sprintf(BufferTransmit,"$$$\r\n");
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,5);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.inCMDMode){
			result = 1;
		}
	}else if (command == BTCMD_EXIT){
		BTCmd.cmdModeMsgSent = BTCMD_EXIT;
		sprintf(BufferTransmit,"---\r\n");
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,5);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.inCMDMode==0){
			result = 1;
		}
	}else if (command == BTCMD_SG){
		BTCmd.cmdModeMsgSent = BTCMD_SG;
		BTCmd.btSetCmdApplied = 0;
		sprintf(BufferTransmit,"SG,2\r\n");//bluetooth classic only
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,6);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.btSetCmdApplied==1){
			result = 1;
		}
	}else if (command == BTCMD_RENAME){
		BTCmd.cmdModeMsgSent = BTCMD_RENAME;
		BTCmd.btSetCmdApplied = 0;
		sprintf(BufferTransmit,"S-,%s\r\n",MACHINENAME);// rename, only 11 chars for the name, then appends "-ABCD" to get unique name
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,NAME_STR_LEN + 5);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.btSetCmdApplied==1){
			result = 1;
		}
	}else if (command == BTCMD_REBOOT){
		BTCmd.cmdModeMsgSent = BTCMD_REBOOT;
		BTCmd.btRebooted = 0;
		sprintf(BufferTransmit,"R,1\r\n");// rebooth and apply the Set commands
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,5);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.btRebooted==1){
			result = 1;
		}
	}else if (command == BTCMD_GETNAME){
		BTCmd.cmdModeMsgSent = BTCMD_GETNAME;
		BTCmd.btNameSameAsMachineName = 0;
		sprintf(BufferTransmit,"GN\r\n");// GetName
		HAL_UART_Transmit_IT(&huart1,(uint8_t*)BufferTransmit,4);
		BTCmd.btTick = 0;
		while(BTCmd.btTick<500){}
		if (BTCmd.btNameSameAsMachineName==1){
			result = 1;
		}
	}else{}

	BTCmd.btTickOn = 0;

	return result;
}



int BT_SetupDevice(void){
	uint8_t btResult;
	btResult = BTConsole_WriteCMD(BTCMD_ENTER);
	if (btResult == 0){
		return -1;}
	btResult = BTConsole_WriteCMD(BTCMD_GETNAME);
	if (btResult == 0){	// if 1 theres nothing to do, we can exit the cmd mode
		// means name is not same as machine name.
		btResult = BTConsole_WriteCMD(BTCMD_SG);
		if (btResult == 0){
			return -2;}
		btResult = BTConsole_WriteCMD(BTCMD_RENAME);
		if (btResult == 0){
			return -3;}
		btResult = BTConsole_WriteCMD(BTCMD_REBOOT);
		if (btResult == 0){
			return -4;}
	}else{
		btResult = BTConsole_WriteCMD(BTCMD_EXIT);
		if (btResult == 0){
			return -5;}
	}

	return 1;

}
