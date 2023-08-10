/*
 * EepromSettings.h
 *
 *  Created on: 06-Mar-2023
 *      Author: harsha
 */

#ifndef INC_EEPROMSETTINGS_H_
#define INC_EEPROMSETTINGS_H_

//Addresses
//Dont let a address go across address 32 and its multiples. Thats one page.
//Addresses
//Dont let a address go across address 32 and its multiples. Thats one page.
#define FF_SPINDLE_SPEED_ADDR 0X02
#define FF_TENSION_DRAFT_ADDR 0X04 // float
#define FF_TPI_ADDR 0X08 //float
#define FF_RTF_ADDR 0X0C //float
#define FF_TARGET_LAYERS_ADDR 0X10 //int
#define FF_BOBBIN_HEIGHT_ADDR 0X12 //int
#define FF_ROVING_WIDTH_ADDR 0X14 //float
#define FF_DELTA_BOBBIN_DIA_ADDR 0X18 // float
#define FF_BARE_BOBBIN_DIA_ADDR 0X22 // int
#define RAMPUP_TIME_ADDR 0X24 //int
#define RAMPDOWN_TIME_ADDR 0X26 // int
#define CHANGE_LAYER_RAMP_TIME_ADDR 0x28 // int
#define FF_CONE_ANGLE_FACTOR 0x32 //float

//DEFAULTS
#define DEFAULT_SPINDLESPEED 650
#define DEFAULT_DRAFT 8
#define DEFAULT_TPI 1.2
#define DEFAULT_RTF 1.0
#define DEFAULT_ROVINGLAYERS 10
#define DEFAULT_MAXCONTENT_HEIGHT 250
#define DEFAULT_ROVINGWIDTH 3
#define DEFAULT_DELTA_BOBBIN_DIA 0.6
#define DEFAULT_BARE_BOBBIN_DIA 48
#define DEFAULT_RAMPUP_TIME 12		// in sec
#define DEFAULT_RAMPDOWN_TIME 10	// in sec
#define DEFAULT_CHANGE_LAYER_RAMP_TIME 800
#define DEFAULT_CONE_ANGLE_FACTOR 0.5


#endif /* INC_EEPROMSETTINGS_H_ */
