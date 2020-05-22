################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../src/Com_Protocols/BL_protocol/BL_protocol.c \
../src/Com_Protocols/BL_protocol/BL_protocol_cfg.c 

OBJS += \
./src/Com_Protocols/BL_protocol/BL_protocol.o \
./src/Com_Protocols/BL_protocol/BL_protocol_cfg.o 

C_DEPS += \
./src/Com_Protocols/BL_protocol/BL_protocol.d \
./src/Com_Protocols/BL_protocol/BL_protocol_cfg.d 


# Each subdirectory must supply rules for building sources it contributes
src/Com_Protocols/BL_protocol/%.o: ../src/Com_Protocols/BL_protocol/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GNU ARM Cross C Compiler'
	arm-none-eabi-gcc -mcpu=cortex-m3 -mthumb -Og -fmessage-length=0 -fsigned-char -ffunction-sections -fdata-sections -ffreestanding -fno-move-loop-invariants -Wall -Wextra  -g3 -DDEBUG -DUSE_FULL_ASSERT -DTRACE -DOS_USE_TRACE_SEMIHOSTING_DEBUG -DSTM32F10X_MD -DUSE_STDPERIPH_DRIVER -DHSE_VALUE=8000000 -I"../include" -I"../system/include" -I"../system/include/cmsis" -I"../system/include/stm32f1-stdperiph" -std=gnu11 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -c -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


