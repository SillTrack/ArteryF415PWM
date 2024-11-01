################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../firmware/at32f415_adc.c \
../firmware/at32f415_can.c \
../firmware/at32f415_cmp.c \
../firmware/at32f415_crc.c \
../firmware/at32f415_crm.c \
../firmware/at32f415_debug.c \
../firmware/at32f415_dma.c \
../firmware/at32f415_ertc.c \
../firmware/at32f415_exint.c \
../firmware/at32f415_flash.c \
../firmware/at32f415_gpio.c \
../firmware/at32f415_i2c.c \
../firmware/at32f415_misc.c \
../firmware/at32f415_pwc.c \
../firmware/at32f415_sdio.c \
../firmware/at32f415_spi.c \
../firmware/at32f415_tmr.c \
../firmware/at32f415_usart.c \
../firmware/at32f415_usb.c \
../firmware/at32f415_wdt.c \
../firmware/at32f415_wwdt.c 

OBJS += \
./firmware/at32f415_adc.o \
./firmware/at32f415_can.o \
./firmware/at32f415_cmp.o \
./firmware/at32f415_crc.o \
./firmware/at32f415_crm.o \
./firmware/at32f415_debug.o \
./firmware/at32f415_dma.o \
./firmware/at32f415_ertc.o \
./firmware/at32f415_exint.o \
./firmware/at32f415_flash.o \
./firmware/at32f415_gpio.o \
./firmware/at32f415_i2c.o \
./firmware/at32f415_misc.o \
./firmware/at32f415_pwc.o \
./firmware/at32f415_sdio.o \
./firmware/at32f415_spi.o \
./firmware/at32f415_tmr.o \
./firmware/at32f415_usart.o \
./firmware/at32f415_usb.o \
./firmware/at32f415_wdt.o \
./firmware/at32f415_wwdt.o 

C_DEPS += \
./firmware/at32f415_adc.d \
./firmware/at32f415_can.d \
./firmware/at32f415_cmp.d \
./firmware/at32f415_crc.d \
./firmware/at32f415_crm.d \
./firmware/at32f415_debug.d \
./firmware/at32f415_dma.d \
./firmware/at32f415_ertc.d \
./firmware/at32f415_exint.d \
./firmware/at32f415_flash.d \
./firmware/at32f415_gpio.d \
./firmware/at32f415_i2c.d \
./firmware/at32f415_misc.d \
./firmware/at32f415_pwc.d \
./firmware/at32f415_sdio.d \
./firmware/at32f415_spi.d \
./firmware/at32f415_tmr.d \
./firmware/at32f415_usart.d \
./firmware/at32f415_usb.d \
./firmware/at32f415_wdt.d \
./firmware/at32f415_wwdt.d 


# Each subdirectory must supply rules for building sources it contributes
firmware/%.o: ../firmware/%.c firmware/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: GNU Arm Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -O0 -ffunction-sections  -g -DAT_START_F415_V1 -DAT32F415RCT7 -DUSE_STDPERIPH_DRIVER -I"../include" -I"../include/libraries/drivers/inc" -I"../include/libraries/cmsis/cm4/core_support" -I"../include/libraries/cmsis/cm4/device_support" -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


