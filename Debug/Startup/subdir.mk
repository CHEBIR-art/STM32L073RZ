################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (12.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
S_SRCS += \
../Startup/startup_stm32l073rztx.s 

OBJS += \
./Startup/startup_stm32l073rztx.o 

S_DEPS += \
./Startup/startup_stm32l073rztx.d 


# Each subdirectory must supply rules for building sources it contributes
Startup/%.o: ../Startup/%.s Startup/subdir.mk
	arm-none-eabi-gcc -mcpu=cortex-m0plus -g3 -DDEBUG -c -I"D:/Users/user/Documents/STM32CubeIDE/workspace_1.16.1/stm32l073/Drivers/CMSIS/Device/ST/STM32L0xx/Include" -I"D:/Users/user/Documents/STM32CubeIDE/workspace_1.16.1/stm32l073/Drivers/CMSIS/Include" -x assembler-with-cpp -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@" "$<"

clean: clean-Startup

clean-Startup:
	-$(RM) ./Startup/startup_stm32l073rztx.d ./Startup/startup_stm32l073rztx.o

.PHONY: clean-Startup

