/*
 * BT_Fns.h
 *
 *  Created on: Mar 8, 2023
 *      Author: harsha
 */

#ifndef INC_BT_FNS_H_
#define INC_BT_FNS_H_

#include <string.h>
#include <stdio.h>
#include <ctype.h>
#include "Struct.h"
#include "machineSettings.h"
#include "CommonConstants.h"


#define TLC_C 6
#define TLV_INT 8
#define TLV_FLOAT 12

// Defines General for all machines
#define FRAME_DELIMITER 0x7E
#define PAIRED_FROM_PHONE 0x99
#define PAIRED_FROM_TAB 0x98

// FOR INFO
#define SETTINGS_FROM_APP 0x01
#define SETTINGS_FROM_MC 0x02
#define REQ_SETTINGS_FROM_APP 0x03
#define DIAGNOSTICS_FROM_APP 0x04
#define DIAG_RUN_DATA_TO_APP 0x05
#define MC_STATE 0x06
#define CAROUSAL_INFO 0x07
#define GBCALIB_CMD_FROM_APP 0x08
#define GBCALIB_DATA_FROM_MB 0x09

//subStates
#define SUBSTATE_NA 0x99
#define BT_IDLE 0x00
#define BT_RUN 0x01
#define BT_PAUSE 0x02
#define BT_STOP 0x03
#define BT_HOMING 0x04
#define BT_INCHING 0x05
#define BT_FINISH 0x06
//vars for pause reasons
#define BT_PAUSE_USER_PRESSED 0x01
#define BT_PAUSE_CREEL_SLIVER_CUT 0x02

//Types for MC States
#define HOMING_LEFT_LIFT_DIST 0x01
#define HOMING_RIGHT_LIFT_DIST 0x02

#define BT_PAUSE_REASON 0x01 // for the TLV

//vars to identify what data is going to be sent in run mode
//data to be recieved from app carousel to define whats going to be sent back
#define RUN_PRODUCTION_DATA 0x0A

#define RUN_LEFT_LIFT_DIST 0x01
#define RUN_RIGHT_LIFT_DIST 0x02
#define RUN_LAYERNO 0x03
#define RUN_TLV_MOTOR_TEMP 0x04
#define RUN_TLV_MOSFET_TEMP 0x05
#define RUN_TLV_MOTOR_CURRENT 0x06
#define RUN_TLV_MOTOR_RPM 0x07
#define RUN_OUTPUT_MTRS_PER_SPINDLE 0x08
#define RUN_TLV_WHAT_INFO  0x09// what data is going to be sent
#define RUN_TOTAL_POWER 0x0A
#define RUN_DUCT_SENSOR 0x0B
#define RUN_COILER_SENSOR 0x0C
#define RUN_CARDING_DLVRY_MTR_MIN 0x0D
#define RUN_DF_OUTPUT_LENGTH 0x0E


#define ERR_REASON 0x01
#define ERR_SOURCE 0x02
#define ERR_CODE 0x03

//for GB_calib
#define BT_GB_CALIB_START 1
#define BT_GB_CALIB_STOP 2
#define BT_GB_CALIB_SAVE_LEFT 3
#define BT_GB_CALIB_SAVE_RIGHT 4

//diagnostic SETTINGS
#define MOTOR_ID 0x40
#define CONTROL_TYPE 0x41
#define TARGET_PERCENT 0x42
#define TARGET_RUNTIME 0x43
#define MOTOR_DIR 0x44
#define BED_DIST 0x45

#define DIAG_SPEED_DATA 0x01
#define DIAG_PWM_DATA 0x02
#define DIAG_CURRENT_DATA 0x03
#define DIAG_POWER_DATA 0x04
#define DIAG_LIFT_DATA 0x05
#define LIFT_OL_DEFAULT_DUTY 300
#define LIFT_CL_SLOW_SPEED 1
#define LIFT_CL_MEDIUM_SPEED 2.5

#define BT_DEFAULT_DIR 0
#define BT_REVERSE_DIR 1



#define SAVINGFAILURE "7E007E"
#define SAVINGSUCCESS "7E017E"

typedef struct BT_Recieve_MsgsStruct
{
	char sof;
	char length;
	char eof;
	char information;
	char subState;
	char attributeCnt;
	char TLVsLength;
	char BTmsgGood;

}BTRecvMsg_TypeDef;


typedef struct TLVStruct
{
	char type;
	char length;
	uint16_t value_int;
	float value_f;
}TLVStruct_TypeDef;

extern BTRecvMsg_TypeDef BT;

//Functions Common to all Machines
char charToHexDigit(char c);
char StringToHex(char c[2]);
uint16_t stringToINT(char c[4]);
uint8_t DecodeStringAsUint8(char c[2]);
uint16_t DecodeStringAsInteger(char c[4]);
unsigned long stringToFLOAT(char c[8]);

void generateTLV_I(char *buffer, uint8_t ID,uint16_t value);
void generateTLV_F(char *buffer, uint8_t ID,float valueF);
uint8_t Init_TXBuf_Frame(uint8_t information,uint8_t substate,uint8_t TLV_count);
void add_TLVBuf_To_TxBuf(char *TLVbuf,uint8_t copySize,uint8_t TXBuf_location);
uint8_t addEOF(uint8_t location);
void correctLengthInFrame(uint8_t initLength,uint8_t TLV_length,uint8_t eof_length);

uint8_t parseTLV(TLVStruct_TypeDef *T, uint8_t startLocation);
uint8_t parseSettings(void);
uint8_t SaveSettings(void);

void InitializeBTMsg_TypeDef(BTRecvMsg_TypeDef *b);
void ParseBTMsg(void);
uint8_t ParseDiagnostics(void);
uint8_t BT_MC_generateDiagnosticMsg(uint8_t BT_MotorID);
uint8_t BT_MC_generateDiagnosticMsg_DoubleMotors(uint8_t diagCommand);
uint8_t BT_MC_generateStatusMsg(uint8_t state);


extern char BufferRec [150];
extern char BufferTransmit [150];

#endif /* INC_BT_FNS_H_ */
