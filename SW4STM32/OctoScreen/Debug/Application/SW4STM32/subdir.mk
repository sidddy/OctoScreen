################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
/mnt/ds/src/mks_tft/OctoScreen/SW4STM32/startup_stm32f107xc.s 

OBJS += \
./Application/SW4STM32/startup_stm32f107xc.o 


# Each subdirectory must supply rules for building sources it contributes
Application/SW4STM32/startup_stm32f107xc.o: /mnt/ds/src/mks_tft/OctoScreen/SW4STM32/startup_stm32f107xc.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m3 -mthumb -mfloat-abi=soft -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


