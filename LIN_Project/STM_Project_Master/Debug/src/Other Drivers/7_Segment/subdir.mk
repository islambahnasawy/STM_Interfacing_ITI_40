################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Other\ Drivers/7_Segment/Seven_Segment_program.c 

OBJS += \
./src/Other\ Drivers/7_Segment/Seven_Segment_program.o 

C_DEPS += \
./src/Other\ Drivers/7_Segment/Seven_Segment_program.d 


# Each subdirectory must supply rules for building sources it contributes
src/Other\ Drivers/7_Segment/Seven_Segment_program.o: ../src/Other\ Drivers/7_Segment/Seven_Segment_program.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"src/Other Drivers/7_Segment/Seven_Segment_program.d" -MT"src/Other\ Drivers/7_Segment/Seven_Segment_program.d" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


