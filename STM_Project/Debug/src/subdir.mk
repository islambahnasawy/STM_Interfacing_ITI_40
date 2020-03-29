################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/C_LCD_program.c \
../src/DELAY_program.c \
../src/DIO_program.c \
../src/LED_cfg.c \
../src/LED_program.c \
../src/NVIC_program.c \
../src/RCC_program.c \
../src/RTOS_config.c \
../src/RTOS_program.c \
../src/STK_program.c \
../src/Seven_Segment_program.c \
../src/Switch_cfg.c \
../src/Switch_program.c \
../src/USART_program.c \
../src/USRTH_program.c \
../src/_write.c \
../src/keypad_program.c \
../src/main.c \
../src/queue.c 

OBJS += \
./src/C_LCD_program.o \
./src/DELAY_program.o \
./src/DIO_program.o \
./src/LED_cfg.o \
./src/LED_program.o \
./src/NVIC_program.o \
./src/RCC_program.o \
./src/RTOS_config.o \
./src/RTOS_program.o \
./src/STK_program.o \
./src/Seven_Segment_program.o \
./src/Switch_cfg.o \
./src/Switch_program.o \
./src/USART_program.o \
./src/USRTH_program.o \
./src/_write.o \
./src/keypad_program.o \
./src/main.o \
./src/queue.o 

C_DEPS += \
./src/C_LCD_program.d \
./src/DELAY_program.d \
./src/DIO_program.d \
./src/LED_cfg.d \
./src/LED_program.d \
./src/NVIC_program.d \
./src/RCC_program.d \
./src/RTOS_config.d \
./src/RTOS_program.d \
./src/STK_program.d \
./src/Seven_Segment_program.d \
./src/Switch_cfg.d \
./src/Switch_program.d \
./src/USART_program.d \
./src/USRTH_program.d \
./src/_write.d \
./src/keypad_program.d \
./src/main.d \
./src/queue.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


