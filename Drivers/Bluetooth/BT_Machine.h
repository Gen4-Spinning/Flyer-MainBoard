/*
 * BT_Constants.h
 *
 *  Created on: Mar 9, 2023
 *      Author: harsha
 */

#ifndef BT_MC_H_
#define BT_MC_H_

#include "BT_Fns.h"
#include "GB_Diag.h"
#include "machineSettings.h"

//settings TLVs
#define SPINDLE_SPEED 0x50
#define DRAFT 0x51
#define TPI_FF 0x52
#define RTF 0x53
#define TARGET_LAYERS 0x54
#define CONTENTHEIGHT 0x55
#define ROVING_WIDTH 0x56
#define DELTA_BOBBIN_DIA 0x57
#define BARE_BOBBIN_DIA 0x58
#define RAMPUP_TIME 0x59
#define RAMPDOWN_TIME 0x60
#define CHANGELAYER_TIME 0x61
#define CONEANGLE_FACTOR 0x62

#define BT_FLYER 0x01
#define BT_BOBBIN 0x02
#define BT_FR 0x03
#define BT_BR 0x04
#define BT_DRAFTING 0x05
#define BT_WINDING 0x06
#define BT_LIFT_BOTH 0x07
#define BT_LIFT_LEFT 0x08
#define BT_LIFT_RIGHT 0x09

#define BT_DIAGNOSTIC_RUN 0x01
#define BT_DIAGNOSTIC_STOP 0x06

//TLVs for GB calib
#define GB_CALIB_LEFT_ABSPOS 0x01
#define GB_CALIB_RIGHT_ABSPOS 0x02

uint8_t BT_MC_generateSettingsMsg(machineSettingsTypeDef *m);
uint8_t BT_MC_parse_Settings(machineSettingsTypeDef *mspBT);
uint8_t BT_MC_Save_Settings(void);
uint8_t GetMotorID_from_BTMotor_ID(uint8_t BT_motorID);
uint8_t GetMotorId_from_CarousalID(uint8_t carousalID);
uint8_t BT_generate_GbCalibMsg(GB_diag *gbd);
uint8_t Get_BTMotorID_from_Motor_ID(uint8_t motorID);

#endif /* BT_MC_H_ */
