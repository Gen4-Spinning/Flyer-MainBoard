

#ifndef MCP23017_H_
#define MCP23017_H_

#include "stm32g4xx_hal.h"

//default states of IOCON
/*IOCON register
bit 7 :bank = 0 ;  // port A and portB are next to each other , not seperated
	6 :mirror = 0; // int A and B are not mirrored
	5 :seqOp = 1;  // one means no sequential read or write
	4 :disslw = 0; // no slew rate control on SDA
	3 :haen = 0    // not applicable to this IC
	2 :odr = 0;    // interrupt pin is push pull
	1 :inpol = 0;  // interrupt pin is active LOW
	0 :always 0
IOCON = 00100000 = 0x20 */
#define IOCON_DEFAULT 0x20

//default setup for PortB
/* pins 0,1,2,3 are inputs and 4,5,6,7 are outputs
 * none of the input pins need pullups
 * the default state of the pins is high. and when we get a low signal we want an interrupt.
 * the output pins are low by default. we set them high when we want to turn them on
 *
 * IODIRB register: (1 means input, 0 means output)
 * = 00001111 = 0x0F
 *
 * IPOLB register ( 1 means GPIO register will reflect inverted value of pin)
 * = 0x00000000 = 0x00 - we dont want inveted values
 *
 * GPPUB register (1 means 10K pullup on input pins)
 * = 0x00  ( we have external pullups)
 *
 * To read a value, read GPIOB register, to write a value write to OLAT register.
 * Turn on last all output pins 0xF0, reset it 0x00.
 *
 * interrupt Setup
 * ---------------
 * GPINTENB register : (1 means interrupt on change enabled for those pins)
 * pins have to be inputs.
 * = 0000000 = 0x0F : we dont want interrupt on change for input pins.
 * we only want interrupt when you get a falling edge
 *
 * DEFVALB register : (1 means default value is high, 0 means default value is low)
 * = 0000000 = 0x00 :for us input pins are pulled high by default and made low when triggered.
 * We dont use this - so for now we just continue.
 *
 * INTCONB register : (sets whether we generate interrupt on pinchange or when compared against defVal)
 * 1 means pin is compared with Def Val register, 0 means interrupt triggered when pin changes
 * = 00000000 = 0x00 : for us we want all sensor input pins to be compared with def val register,but
 * some issue with the HAL software in STM32, so we re going to do pinChange , and then read
 * the values with the register.
 *
 * already in IOCON we set mirror = 0, so INTA pin only refers to PortA and INTB refers to PORTB
 *
 * INTF register ( read only) :1 means that pin set the interrupt
 * INTCAP register (readonly) :shows what the value of the pins were when the interrupt was set.
 * we might not need to see this because we know interrupt is only set when value goes low.
 *
 *
 */

#define IODIRB_DEFAULT 0x0F
#define IOPOLB_DEFAULT 0x00
#define GPPUB_DEFAULT 0x00

#define OLATB_ALL_OP_ON  0xF0
#define OLATB_ALL_OP_OFF 0x00

#define IOCON_WHEN_BANK0 0x0A
#define IOCON_WHEN_BANK1 0x05

#define GPINTENB_DEFAULT 0x0F
#define DEFVALB_DEFAULT 0x00
#define INTCONB_DEFAULT 0x00

//register definitions with Bank = 0
#define IODIRB					0x01
#define IPOLB					0x03
#define GPINTENB				0x05
#define DEFVALB					0x07
#define INTCONB					0x09
#define GPPUB					0x0D
#define INTFB					0x0F
#define INTCAPB					0x11
#define MCP_GPIOB				0x13
#define OLATB					0x15

#define IODIRA					0x00
#define IPOLA					0x02
#define GPINTENA				0x04
#define DEFVALA					0x06
#define INTCONA					0x08
#define GPPUA					0x0C
#define INTFA					0x0E
#define INTCAPA					0x10
#define MCP_GPIOA				0x12
#define OLATA					0x14

typedef union {
  uint8_t raw;
  struct __attribute__ ((packed)) {
    uint8_t input0:1;
    uint8_t input1:1;
    uint8_t input2:1;
    uint8_t input3:1;
    uint8_t towerRed:1;
    uint8_t towerAmber:1;
    uint8_t towerGreen:1;
    uint8_t towerBuzzer:1;
  } values;
} MCP23017_PortB;


typedef struct {
	I2C_HandleTypeDef*	hi2c;
	uint16_t		addr;
	uint8_t	 outputGPIO[1];
	uint8_t  intTrigger[1];
	uint8_t  intTriggerCapturedValue[1];
} MCP23017_HandleTypeDef;

extern MCP23017_HandleTypeDef hmcp;
extern MCP23017_PortB mcp_portB;
extern MCP23017_PortB mcp_portB_whichSensor;
extern MCP23017_PortB mcp_portB_sensorVal;



void mcp23017_setupForMainBoard(MCP23017_HandleTypeDef *hdev);
void mcp23017_init(MCP23017_HandleTypeDef *hdev, I2C_HandleTypeDef *hi2c, uint16_t addr);
HAL_StatusTypeDef mcp23017_read(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);
HAL_StatusTypeDef mcp23017_write(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data);


#endif

