################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_themes/lv_theme.c \
../Application/User/lvgl/lv_themes/lv_theme_alien.c \
../Application/User/lvgl/lv_themes/lv_theme_default.c \
../Application/User/lvgl/lv_themes/lv_theme_material.c \
../Application/User/lvgl/lv_themes/lv_theme_mono.c \
../Application/User/lvgl/lv_themes/lv_theme_nemo.c \
../Application/User/lvgl/lv_themes/lv_theme_night.c \
../Application/User/lvgl/lv_themes/lv_theme_templ.c \
../Application/User/lvgl/lv_themes/lv_theme_zen.c 

OBJS += \
./Application/User/lvgl/lv_themes/lv_theme.o \
./Application/User/lvgl/lv_themes/lv_theme_alien.o \
./Application/User/lvgl/lv_themes/lv_theme_default.o \
./Application/User/lvgl/lv_themes/lv_theme_material.o \
./Application/User/lvgl/lv_themes/lv_theme_mono.o \
./Application/User/lvgl/lv_themes/lv_theme_nemo.o \
./Application/User/lvgl/lv_themes/lv_theme_night.o \
./Application/User/lvgl/lv_themes/lv_theme_templ.o \
./Application/User/lvgl/lv_themes/lv_theme_zen.o 

C_DEPS += \
./Application/User/lvgl/lv_themes/lv_theme.d \
./Application/User/lvgl/lv_themes/lv_theme_alien.d \
./Application/User/lvgl/lv_themes/lv_theme_default.d \
./Application/User/lvgl/lv_themes/lv_theme_material.d \
./Application/User/lvgl/lv_themes/lv_theme_mono.d \
./Application/User/lvgl/lv_themes/lv_theme_nemo.d \
./Application/User/lvgl/lv_themes/lv_theme_night.d \
./Application/User/lvgl/lv_themes/lv_theme_templ.d \
./Application/User/lvgl/lv_themes/lv_theme_zen.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_themes/%.o: ../Application/User/lvgl/lv_themes/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


