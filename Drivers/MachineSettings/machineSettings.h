/*
 * machineSettings.h
 *
 *  Created on: Apr 11, 2023
 *      Author: harsha
 */

#ifndef MACHINESETTINGS_H_
#define MACHINESETTINGS_H_

#include "stdio.h"
#include "FDCAN.h"

//all Structs with arrays have to follow this Form
#define FLYER 0
#define BOBBIN 1
#define FR 2
#define BR 3
#define LEFT_LIFT 4
#define RIGHT_LIFT 5

#define SPINDLEMOTOR_TO_SPINDLE_RATIO 1.476
#define BOBBINMOTOR_TO_BOBBIN_RATIO 1.476

#define FR_CIRCUMFERENCE 94.248 // 30 *pi
#define FRMOTOR_TO_FR_RATIO 4.61  // taking into account motorGB and pulley Ratio

#define BR_CONSTANT 23.562 // calculated from Excel.
#define LIFT_SCREW_PITCH_MM 4.0
#define LIFT_MOTOR_TO_LIFT_SCREW_RATIO 15.3

//LIMITS
#define MAX_MOTOR_RPM 1450
#define MAX_SPINDLE_SPEED 850
#define MIN_SPINDLE_SPEED 500
#define MAX_CONTENT_DIA_MM 140
#define OFFSET_FROM_MAXLAYERS 5

// lift direction in the motor sw
#define LIFT_UP 1
#define LIFT_DOWN 0

typedef struct machineSettings_Struct
{
    int spindleSpeed;
    float tensionDraft;
    float tpi;
    float rtf;
    int targetLayers ;
    int contentHeight;
    float rovingWidth;
    float deltaBobbinDia;
    int bareBobbinDia;
    int rampUpTime	;
    int rampDownTime;
    int changeLayerTime;
    float coneAngleFactor;// decides the cone Angle.

}machineSettingsTypeDef;

typedef struct machineParamaters_Struct{
    uint16_t flyerMotorRPM;
    uint16_t bobbinMotorRPM;
    uint16_t FR_MotorRPM;
    uint16_t BR_MotorRPM;
    uint16_t liftMotorRPM;

    float FR_RPM;
    float delivery_mtr_min;
    uint16_t maxLayers;
    uint8_t currentLayer;
    uint8_t currentDir;
    float bobbinDia;
    float deltaRpm_Spindle_Bobbin;
    float bobbinRPM;
    float strokeHeight;
    float strokeDistperSec;
    float strokeTime;

    float targetAbsPosition;

    float left_liftStrokeHeight;
    float right_liftStrokeHeight;

    float totalRunTime_Min;
    float currentRunTimeMin;
    float outputLength_mtrs;

}machineParamsTypeDef;


extern machineSettingsTypeDef msp;
extern machineSettingsTypeDef msp_BT;
extern machineParamsTypeDef mcParams;

// Eeprom MachineSettings
void ReadMachineSettingsFromEeprom(machineSettingsTypeDef *m);
uint8_t WriteMachineSettingsIntoEeprom(machineSettingsTypeDef *m);
uint8_t CheckMachineSettings(machineSettingsTypeDef* m);
void LoadDefaultMachineSettings(machineSettingsTypeDef* m);

//otherFns
void InitializeMachineSettings(machineSettingsTypeDef *ms);
void CalculateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *mp);
void SetupMachineParametersForFirstLayer(machineParamsTypeDef *mp);
void UpdateMachineParameters(machineSettingsTypeDef *ms,machineParamsTypeDef *mp,uint8_t layerNo);
void CalculateTotalRunTime(machineSettingsTypeDef *ms ,machineParamsTypeDef *mp);

uint8_t getMotorCANAddress(uint8_t motor);
uint8_t getLiftMotorCANAddress(uint8_t motor);
uint8_t GetMotorID_from_CANAddress(uint8_t canAddress);


#endif /* MACHINESETTINGS_H_ */
