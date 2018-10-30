################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/lvgl/lv_misc/lv_anim.c \
../Application/User/lvgl/lv_misc/lv_area.c \
../Application/User/lvgl/lv_misc/lv_circ.c \
../Application/User/lvgl/lv_misc/lv_color.c \
../Application/User/lvgl/lv_misc/lv_font.c \
../Application/User/lvgl/lv_misc/lv_fs.c \
../Application/User/lvgl/lv_misc/lv_ll.c \
../Application/User/lvgl/lv_misc/lv_log.c \
../Application/User/lvgl/lv_misc/lv_math.c \
../Application/User/lvgl/lv_misc/lv_mem.c \
../Application/User/lvgl/lv_misc/lv_task.c \
../Application/User/lvgl/lv_misc/lv_templ.c \
../Application/User/lvgl/lv_misc/lv_txt.c \
../Application/User/lvgl/lv_misc/lv_ufs.c 

OBJS += \
./Application/User/lvgl/lv_misc/lv_anim.o \
./Application/User/lvgl/lv_misc/lv_area.o \
./Application/User/lvgl/lv_misc/lv_circ.o \
./Application/User/lvgl/lv_misc/lv_color.o \
./Application/User/lvgl/lv_misc/lv_font.o \
./Application/User/lvgl/lv_misc/lv_fs.o \
./Application/User/lvgl/lv_misc/lv_ll.o \
./Application/User/lvgl/lv_misc/lv_log.o \
./Application/User/lvgl/lv_misc/lv_math.o \
./Application/User/lvgl/lv_misc/lv_mem.o \
./Application/User/lvgl/lv_misc/lv_task.o \
./Application/User/lvgl/lv_misc/lv_templ.o \
./Application/User/lvgl/lv_misc/lv_txt.o \
./Application/User/lvgl/lv_misc/lv_ufs.o 

C_DEPS += \
./Application/User/lvgl/lv_misc/lv_anim.d \
./Application/User/lvgl/lv_misc/lv_area.d \
./Application/User/lvgl/lv_misc/lv_circ.d \
./Application/User/lvgl/lv_misc/lv_color.d \
./Application/User/lvgl/lv_misc/lv_font.d \
./Application/User/lvgl/lv_misc/lv_fs.d \
./Application/User/lvgl/lv_misc/lv_ll.d \
./Application/User/lvgl/lv_misc/lv_log.d \
./Application/User/lvgl/lv_misc/lv_math.d \
./Application/User/lvgl/lv_misc/lv_mem.d \
./Application/User/lvgl/lv_misc/lv_task.d \
./Application/User/lvgl/lv_misc/lv_templ.d \
./Application/User/lvgl/lv_misc/lv_txt.d \
./Application/User/lvgl/lv_misc/lv_ufs.d 


# Each subdirectory must supply rules for building sources it contributes
Application/User/lvgl/lv_misc/%.o: ../Application/User/lvgl/lv_misc/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -mfloat-abi=soft '-D__weak=__attribute__((weak))' '-D__packed="__attribute__((__packed__))"' -DUSE_HAL_DRIVER -DSTM32F107xC -I"/mnt/ds/src/mks_tft/OctoScreen/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/STM32F1xx_HAL_Driver/Inc/Legacy" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Device/ST/STM32F1xx/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Drivers/CMSIS/Include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM3" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FatFs/src" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/include" -I"/mnt/ds/src/mks_tft/OctoScreen/Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS" -I../../../../../../../../../../../../../Drivers/CMSIS/Include -I../../../../../../../../../../../../../../../Drivers/CMSIS/Include  -O0 -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


