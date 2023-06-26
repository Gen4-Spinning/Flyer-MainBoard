################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (10.3-2021.10)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/userButtons/userButtons.c 

OBJS += \
./Drivers/userButtons/userButtons.o 

C_DEPS += \
./Drivers/userButtons/userButtons.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/userButtons/%.o Drivers/userButtons/%.su: ../Drivers/userButtons/%.c Drivers/userButtons/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32G431xx -c -I../Core/Inc -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/SerialLogging" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/MachineSensors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/MachineErrors" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/SysObserver" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/GB_Diagnostics" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/userButtons" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/MotorComms" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/MachineSettings" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/FDCAN/MotherBoard" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/FDCAN" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/Eeprom" -I"/home/harsha/STM32CubeIDE/workspace_1.10.0/MainBoard_Flyer/Drivers/MCP23017" -I../Drivers/STM32G4xx_HAL_Driver/Inc -I../Drivers/STM32G4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32G4xx/Include -I../Drivers/CMSIS/Include -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-userButtons

clean-Drivers-2f-userButtons:
	-$(RM) ./Drivers/userButtons/userButtons.d ./Drivers/userButtons/userButtons.o ./Drivers/userButtons/userButtons.su

.PHONY: clean-Drivers-2f-userButtons

