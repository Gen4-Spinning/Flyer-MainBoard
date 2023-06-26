/*
 * CAN_MotherBoard.h
 *
 *  Created on: Mar 7, 2023
 *      Author: harsha
 */

#ifndef CAN_MOTHERBOARD_H_
#define CAN_MOTHERBOARD_H_
#include "main.h"
#include "stm32g4xx_hal.h"
#include "stdio.h"
#include "Struct.h"
#include "MotorComms.h"
#include "GB_Diag.h"

#define HOMING_DONE 1
#define HOMING_SETUP_SUCCESS 2
#define HOMING_SETUP_FAIL 3

void FDCAN_sendCommand_ToMotor(uint8_t destination,uint8_t command);
void FDCAN_sendSetUp_ToMotor(uint8_t destination, SetupMotor rd);
void FDCAN_sendSetUp_ToLiftMotor(uint8_t destination,SetupMotor rd);
void FDCAN_SendDiagnostics_ToMotor(uint8_t destination,DiagnosticsTypeDef *d);
void FDCAN_SendDiagnostics_ToLiftMotor(uint8_t destination,DiagnosticsTypeDef *d);
void FDCAN_sendNewStroke_ToBothLiftMotors(machineParamsTypeDef *mp);
void FDCAN_sendChangeTarget_ToMotor(uint8_t destination, uint16_t newTarget, uint16_t transitionTime);
void FDCAN_sendNewStroke_ToBothLiftMotors(machineParamsTypeDef *mp);

void FDCAN_Recieve_HomingDataFromLiftMotors(uint8_t source_address);
void FDCAN_Recieve_ACKFromMotors(uint8_t sourceAddress);
void FDCAN_Recieve_RunDataFromMotors(uint8_t functionID,uint8_t source);
void FDCAN_Recieve_RunDataFromLiftMotors(uint8_t functionID,uint8_t source);
void FDCAN_Recieve_LiftChangeDirection(uint8_t source);
void FDCAN_Recieve_ErrorsFromMotors(uint8_t sourceAddress);

void FDCAN_Recieve_GBDataFromLiftMotors(uint8_t source, GB_diag *gbd);
void FDCAN_sendGBCalibCommand_ToLiftMotor(uint8_t destination, uint8_t GBDtype);

void FDCAN_parseForMotherBoard(void);

#endif /* CAN_MOTHERBOARD_H_ */
