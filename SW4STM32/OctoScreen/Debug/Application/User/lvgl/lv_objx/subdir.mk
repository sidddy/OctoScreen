################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_objx/lv_arc.c \
../Application/User/lvgl/lv_objx/lv_bar.c \
../Application/User/lvgl/lv_objx/lv_btn.c \
../Application/User/lvgl/lv_objx/lv_btnm.c \
../Application/User/lvgl/lv_objx/lv_calendar.c \
../Application/User/lvgl/lv_objx/lv_cb.c \
../Application/User/lvgl/lv_objx/lv_chart.c \
../Application/User/lvgl/lv_objx/lv_cont.c \
../Application/User/lvgl/lv_objx/lv_ddlist.c \
../Application/User/lvgl/lv_objx/lv_gauge.c \
../Application/User/lvgl/lv_objx/lv_img.c \
../Application/User/lvgl/lv_objx/lv_imgbtn.c \
../Application/User/lvgl/lv_objx/lv_kb.c \
../Application/User/lvgl/lv_objx/lv_label.c \
../Application/User/lvgl/lv_objx/lv_led.c \
../Application/User/lvgl/lv_objx/lv_line.c \
../Application/User/lvgl/lv_objx/lv_list.c \
../Application/User/lvgl/lv_objx/lv_lmeter.c \
../Application/User/lvgl/lv_objx/lv_mbox.c \
../Application/User/lvgl/lv_objx/lv_objx_templ.c \
../Application/User/lvgl/lv_objx/lv_page.c \
../Application/User/lvgl/lv_objx/lv_preload.c \
../Application/User/lvgl/lv_objx/lv_roller.c \
../Application/User/lvgl/lv_objx/lv_slider.c \
../Application/User/lvgl/lv_objx/lv_sw.c \
../Application/User/lvgl/lv_objx/lv_ta.c \
../Application/User/lvgl/lv_objx/lv_tabview.c \
../Application/User/lvgl/lv_objx/lv_win.c 

OBJS += \
./Application/User/lvgl/lv_objx/lv_arc.o \
./Application/User/lvgl/lv_objx/lv_bar.o \
./Application/User/lvgl/lv_objx/lv_btn.o \
./Application/User/lvgl/lv_objx/lv_btnm.o \
./Application/User/lvgl/lv_objx/lv_calendar.o \
./Application/User/lvgl/lv_objx/lv_cb.o \
./Application/User/lvgl/lv_objx/lv_chart.o \
./Application/User/lvgl/lv_objx/lv_cont.o \
./Application/User/lvgl/lv_objx/lv_ddlist.o \
./Application/User/lvgl/lv_objx/lv_gauge.o \
./Application/User/lvgl/lv_objx/lv_img.o \
./Application/User/lvgl/lv_objx/lv_imgbtn.o \
./Application/User/lvgl/lv_objx/lv_kb.o \
./Application/User/lvgl/lv_objx/lv_label.o \
./Application/User/lvgl/lv_objx/lv_led.o \
./Application/User/lvgl/lv_objx/lv_line.o \
./Application/User/lvgl/lv_objx/lv_list.o \
./Application/User/lvgl/lv_objx/lv_lmeter.o \
./Application/User/lvgl/lv_objx/lv_mbox.o \
./Application/User/lvgl/lv_objx/lv_objx_templ.o \
./Application/User/lvgl/lv_objx/lv_page.o \
./Application/User/lvgl/lv_objx/lv_preload.o \
./Application/User/lvgl/lv_objx/lv_roller.o \
./Application/User/lvgl/lv_objx/lv_slider.o \
./Application/User/lvgl/lv_objx/lv_sw.o \
./Application/User/lvgl/lv_objx/lv_ta.o \
./Application/User/lvgl/lv_objx/lv_tabview.o \
./Application/User/lvgl/lv_objx/lv_win.o 

C_DEPS += \
./Application/User/lvgl/lv_objx/lv_arc.d \
./Application/User/lvgl/lv_objx/lv_bar.d \
./Application/User/lvgl/lv_objx/lv_btn.d \
./Application/User/lvgl/lv_objx/lv_btnm.d \
./Application/User/lvgl/lv_objx/lv_calendar.d \
./Application/User/lvgl/lv_objx/lv_cb.d \
./Application/User/lvgl/lv_objx/lv_chart.d \
./Application/User/lvgl/lv_objx/lv_cont.d \
./Application/User/lvgl/lv_objx/lv_ddlist.d \
./Application/User/lvgl/lv_objx/lv_gauge.d \
./Application/User/lvgl/lv_objx/lv_img.d \
./Application/User/lvgl/lv_objx/lv_imgbtn.d \
./Application/User/lvgl/lv_objx/lv_kb.d \
./Application/User/lvgl/lv_objx/lv_label.d \
./Application/User/lvgl/lv_objx/lv_led.d \
./Application/User/lvgl/lv_objx/lv_line.d \
./Application/User/lvgl/lv_objx/lv_list.d \
./Application/User/lvgl/lv_objx/lv_lmeter.d \
./Application/User/lvgl/lv_objx/lv_mbox.d \
./Application/User/lvgl/lv_objx/lv_objx_templ.d \
./Application/User/lvgl/lv_objx/lv_page.d \
./Application/User/lvgl/lv_objx/lv_preload.d \
./Application/User/lvgl/lv_objx/lv_roller.d \
./Application/User/lvgl/lv_objx/lv_slider.d \
./Application/User/lvgl/lv_objx/lv_sw.d \
./Application/User/lvgl/lv_objx/lv_ta.d \
./Application/User/lvgl/lv_objx/lv_tabview.d \
./Application/User/lvgl/lv_objx/lv_win.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_objx/%.o: ../Application/User/lvgl/lv_objx/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


