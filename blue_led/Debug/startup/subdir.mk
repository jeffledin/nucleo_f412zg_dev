################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../startup/startup_stm32f412xg.s 

OBJS += \
./startup/startup_stm32f412xg.o 


# Each subdirectory must supply rules for building sources it contributes
startup/%.o: ../startup/%.s
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Assembler'
	@echo $(PWD)
	arm-none-eabi-as -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -I"C:/Users/ledin/workspace/embedded/nucleo_f412zg/blue_led/inc" -I"C:/Users/ledin/workspace/embedded/nucleo_f412zg/blue_led/CMSIS/core" -I"C:/Users/ledin/workspace/embedded/nucleo_f412zg/blue_led/CMSIS/device" -I"C:/Users/ledin/workspace/embedded/nucleo_f412zg/blue_led/StdPeriph_Driver/inc" -g -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


