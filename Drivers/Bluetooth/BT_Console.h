/*
 * BT_Console.h
 *
 *  Created on: 22-Jun-2023
 *      Author: harsha
 */

#ifndef BT_CONSOLE_H_
#define BT_CONSOLE_H_

#include "stdio.h"
#include "string.h"
#include "StateMachine.h"

#define CONNECTION_MADE 1
#define CONNECTION_LOST 2

#define BT_CONNECT_MSG "CONNECT"
#define BT_DISCONNECT_MSG "DISCONN"

#define BT_CMDMODE_STRING "$$$\r\n"
#define CMD_MODE_RESPONSE_STRING "CMD>"
#define CMD_MODE_END_STRING "END"
#define CMD_MODE_REBOOT_STRING "Reboot"
#define CMD_MODE_AOK_STRING "AOK"

#define BTCMD_ENTER 1
#define BTCMD_EXIT 2
#define BTCMD_SG 3
#define BTCMD_RENAME 4
#define BTCMD_REBOOT 5
#define BTCMD_GETNAME 6

#define MACHINENAME "FLYER"
#define NAME_STR_LEN 5

typedef struct BTconsole{
	uint8_t manual_setup;
	int manual_setup_result;
	uint8_t manual_setup_rebootOK;

	uint8_t inCMDMode;
	uint8_t cmdModeMsgSent;
	uint8_t cmdMSG;

	uint8_t btRebooted;
	uint8_t btSetCmdApplied;
	uint8_t btPaired;
	uint8_t btNameSameAsMachineName;

	uint8_t btTickOn;
	uint16_t btTick;
}BTConsole;

extern BTConsole BTCmd;

extern char BufferRec [150];
extern char BufferTransmit [150];

uint8_t ParseCmdModeMsg(BTConsole *btc);
uint8_t BTConsole_WriteCMD(uint8_t command);
int BT_SetupDevice(void);

#endif /* BT_CONSOLE_H_ */
