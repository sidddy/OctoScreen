################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_fonts/lv_font_builtin.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_10.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_10_cyrillic.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_10_latin_sup.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_20.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_20_cyrillic.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_20_latin_sup.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_30.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_30_cyrillic.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_30_latin_sup.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_40.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_40_cyrillic.c \
../Application/User/lvgl/lv_fonts/lv_font_dejavu_40_latin_sup.c \
../Application/User/lvgl/lv_fonts/lv_font_monospace_8.c \
../Application/User/lvgl/lv_fonts/lv_font_symbol_10.c \
../Application/User/lvgl/lv_fonts/lv_font_symbol_20.c \
../Application/User/lvgl/lv_fonts/lv_font_symbol_30.c \
../Application/User/lvgl/lv_fonts/lv_font_symbol_40.c 

OBJS += \
./Application/User/lvgl/lv_fonts/lv_font_builtin.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10_cyrillic.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10_latin_sup.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20_cyrillic.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20_latin_sup.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30_cyrillic.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30_latin_sup.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40_cyrillic.o \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40_latin_sup.o \
./Application/User/lvgl/lv_fonts/lv_font_monospace_8.o \
./Application/User/lvgl/lv_fonts/lv_font_symbol_10.o \
./Application/User/lvgl/lv_fonts/lv_font_symbol_20.o \
./Application/User/lvgl/lv_fonts/lv_font_symbol_30.o \
./Application/User/lvgl/lv_fonts/lv_font_symbol_40.o 

C_DEPS += \
./Application/User/lvgl/lv_fonts/lv_font_builtin.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10_cyrillic.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_10_latin_sup.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20_cyrillic.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_20_latin_sup.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30_cyrillic.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_30_latin_sup.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40_cyrillic.d \
./Application/User/lvgl/lv_fonts/lv_font_dejavu_40_latin_sup.d \
./Application/User/lvgl/lv_fonts/lv_font_monospace_8.d \
./Application/User/lvgl/lv_fonts/lv_font_symbol_10.d \
./Application/User/lvgl/lv_fonts/lv_font_symbol_20.d \
./Application/User/lvgl/lv_fonts/lv_font_symbol_30.d \
./Application/User/lvgl/lv_fonts/lv_font_symbol_40.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_fonts/%.o: ../Application/User/lvgl/lv_fonts/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


