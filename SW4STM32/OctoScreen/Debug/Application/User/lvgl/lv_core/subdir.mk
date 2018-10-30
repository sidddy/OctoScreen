################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_core/lv_group.c \
../Application/User/lvgl/lv_core/lv_indev.c \
../Application/User/lvgl/lv_core/lv_obj.c \
../Application/User/lvgl/lv_core/lv_refr.c \
../Application/User/lvgl/lv_core/lv_style.c \
../Application/User/lvgl/lv_core/lv_vdb.c 

OBJS += \
./Application/User/lvgl/lv_core/lv_group.o \
./Application/User/lvgl/lv_core/lv_indev.o \
./Application/User/lvgl/lv_core/lv_obj.o \
./Application/User/lvgl/lv_core/lv_refr.o \
./Application/User/lvgl/lv_core/lv_style.o \
./Application/User/lvgl/lv_core/lv_vdb.o 

C_DEPS += \
./Application/User/lvgl/lv_core/lv_group.d \
./Application/User/lvgl/lv_core/lv_indev.d \
./Application/User/lvgl/lv_core/lv_obj.d \
./Application/User/lvgl/lv_core/lv_refr.d \
./Application/User/lvgl/lv_core/lv_style.d \
./Application/User/lvgl/lv_core/lv_vdb.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_core/%.o: ../Application/User/lvgl/lv_core/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


