################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/fatfs.c \
../Application/User/freertos.c \
/mnt/ds/src/mks_tft/OctoScreen/Src/main.c \
/mnt/ds/src/mks_tft/OctoScreen/Src/stm32f1xx_hal_msp.c \
/mnt/ds/src/mks_tft/OctoScreen/Src/stm32f1xx_it.c \
../Application/User/user_diskio.c 

CPP_SRCS += \
../Application/User/Display.cpp \
../Application/User/Print.cpp \
../Application/User/UTFT.cpp 

OBJS += \
./Application/User/Display.o \
./Application/User/Print.o \
./Application/User/UTFT.o \
./Application/User/fatfs.o \
./Application/User/freertos.o \
./Application/User/main.o \
./Application/User/stm32f1xx_hal_msp.o \
./Application/User/stm32f1xx_it.o \
./Application/User/user_diskio.o 

C_DEPS += \
./Application/User/fatfs.d \
./Application/User/freertos.d \
./Application/User/main.d \
./Application/User/stm32f1xx_hal_msp.d \
./Application/User/stm32f1xx_it.d \
./Application/User/user_diskio.d 

CPP_DEPS += \
./Application/User/Display.d \
./Application/User/Print.d \
./Application/User/UTFT.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/%.o: ../Application/User/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo $(PWD)
	arm-none-eabi-g++ -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/%.o: ../Application/User/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/main.o: /mnt/ds/src/mks_tft/OctoScreen/Src/main.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_hal_msp.o: /mnt/ds/src/mks_tft/OctoScreen/Src/stm32f1xx_hal_msp.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

Application/User/stm32f1xx_it.o: /mnt/ds/src/mks_tft/OctoScreen/Src/stm32f1xx_it.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include"  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


