################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_draw/lv_draw.c \
../Application/User/lvgl/lv_draw/lv_draw_arc.c \
../Application/User/lvgl/lv_draw/lv_draw_img.c \
../Application/User/lvgl/lv_draw/lv_draw_label.c \
../Application/User/lvgl/lv_draw/lv_draw_line.c \
../Application/User/lvgl/lv_draw/lv_draw_rbasic.c \
../Application/User/lvgl/lv_draw/lv_draw_rect.c \
../Application/User/lvgl/lv_draw/lv_draw_triangle.c \
../Application/User/lvgl/lv_draw/lv_draw_vbasic.c 

OBJS += \
./Application/User/lvgl/lv_draw/lv_draw.o \
./Application/User/lvgl/lv_draw/lv_draw_arc.o \
./Application/User/lvgl/lv_draw/lv_draw_img.o \
./Application/User/lvgl/lv_draw/lv_draw_label.o \
./Application/User/lvgl/lv_draw/lv_draw_line.o \
./Application/User/lvgl/lv_draw/lv_draw_rbasic.o \
./Application/User/lvgl/lv_draw/lv_draw_rect.o \
./Application/User/lvgl/lv_draw/lv_draw_triangle.o \
./Application/User/lvgl/lv_draw/lv_draw_vbasic.o 

C_DEPS += \
./Application/User/lvgl/lv_draw/lv_draw.d \
./Application/User/lvgl/lv_draw/lv_draw_arc.d \
./Application/User/lvgl/lv_draw/lv_draw_img.d \
./Application/User/lvgl/lv_draw/lv_draw_label.d \
./Application/User/lvgl/lv_draw/lv_draw_line.d \
./Application/User/lvgl/lv_draw/lv_draw_rbasic.d \
./Application/User/lvgl/lv_draw/lv_draw_rect.d \
./Application/User/lvgl/lv_draw/lv_draw_triangle.d \
./Application/User/lvgl/lv_draw/lv_draw_vbasic.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_draw/%.o: ../Application/User/lvgl/lv_draw/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


