// Copyright (c) 2018 Rudá Moura <ruda.moura@gmail.com>
// License: The MIT License (MIT)

#include "mcp23017.h"

// Register  definitions with BANK = 1
#define REGISTER_IODIRA		0x00
#define REGISTER_IODIRB		0x01
#define REGISTER_IPOLA		0x02
#define REGISTER_IPOLB		0x03
#define REGISTER_GPINTENA	0x04
#define REGISTER_GPINTENB	0x05
#define REGISTER_DEFVALA	0x06
#define REGISTER_DEFVALB	0x07
#define REGISTER_INTCONA	0x08
#define REGISTER_INTCONB	0x09

//	IOCON			0x0B
#define REGISTER_GPPUA		0x0C
#define REGISTER_GPPUB		0x0D
#define REGISTER_INTFA		0x0E
#define REGISTER_INTFB		0x0F
#define REGISTER_INTCAPA	0x10
#define REGISTER_INTCAPB	0x11
#define REGISTER_GPIOA		0x12
#define REGISTER_GPIOB		0x13
#define REGISTER_OLATA		0x14
#define REGISTER_OLATB		0x15

#define I2C_TIMEOUT		10


void mcp23017_setupForMainBoard(MCP23017_HandleTypeDef *hdev){
	 HAL_StatusTypeDef status0;
	 uint8_t data[1];

	 data[0] = IOCON_DEFAULT;
	 status0 = mcp23017_write(hdev, IOCON_WHEN_BANK0, data);
	 HAL_Delay(2);

	 data[0] = IODIRB_DEFAULT;
	 status0 = mcp23017_write(hdev, IODIRB, data);
	 HAL_Delay(2);

	 data[0] = IOPOLB_DEFAULT;
	 status0 = mcp23017_write(hdev, IPOLB, data);
	 HAL_Delay(2);

	 data[0] = GPPUB_DEFAULT;
	 status0 = mcp23017_write(hdev, GPPUB, data);
	 HAL_Delay(2);

	 //INTERRUPT SETUP
	 data[0] = GPINTENB_DEFAULT;
	 status0 = mcp23017_write(hdev, GPINTENB, data);
	 HAL_Delay(2);

	 data[0] = DEFVALB_DEFAULT;
	 status0 = mcp23017_write(hdev, DEFVALB, data);
	 HAL_Delay(2);

	 data[0] = INTCONB_DEFAULT;
	 status0 = mcp23017_write(hdev, INTCONB, data);
	 HAL_Delay(2);

	 status0 = mcp23017_read(hdev, INTCAPB,hdev->intTriggerCapturedValue ); // after youve set up , read once
	 HAL_Delay(2);

}

void mcp23017_init(MCP23017_HandleTypeDef *hdev, I2C_HandleTypeDef *hi2c, uint16_t addr)
{
	hdev->hi2c = hi2c;
	hdev->addr = addr << 1;
}

HAL_StatusTypeDef mcp23017_read(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data)
{
	/*HAL_I2C_Mem_Read performs a I2C write operation to select the memory address to read and then reads N bytes (start, I2C address + Write, Memory address, repeated start, I2C address + Read, N bytes, stop) */
	return HAL_I2C_Mem_Read(hdev->hi2c, hdev->addr, reg, 1, data, 1, I2C_TIMEOUT);
	//return HAL_I2C_Mem_Read(hdev->hi2c, hdev->addr, reg, 1, data, 1);
}

HAL_StatusTypeDef mcp23017_write(MCP23017_HandleTypeDef *hdev, uint16_t reg, uint8_t *data)
{
	/*HAL_I2C_Mem_Write performs a I2C write operation to select the memory address to read and then writes N bytes (start, I2C address + Write, Memory address, repeated start, I2C address + Write, N bytes, stop)*/
	
	return HAL_I2C_Mem_Write(hdev->hi2c, hdev->addr, reg, 1, data, 1, I2C_TIMEOUT);
}
