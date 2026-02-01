################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (13.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../app/src/app.c \
../app/src/logger.c \
../app/src/systick.c \
../app/src/task_HC05.c \
../app/src/task_mic.c \
../app/src/task_sensor.c 

OBJS += \
./app/src/app.o \
./app/src/logger.o \
./app/src/systick.o \
./app/src/task_HC05.o \
./app/src/task_mic.o \
./app/src/task_sensor.o 

C_DEPS += \
./app/src/app.d \
./app/src/logger.d \
./app/src/systick.d \
./app/src/task_HC05.d \
./app/src/task_mic.d \
./app/src/task_sensor.d 


# Each subdirectory must supply rules for building sources it contributes
app/src/%.o app/src/%.su app/src/%.cyclo: ../app/src/%.c app/src/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m3 -std=gnu11 -g3 -DDEBUG -DUSE_HAL_DRIVER -DSTM32F103xB -c -I../Core/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc -I../Drivers/STM32F1xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F1xx/Include -I../Drivers/CMSIS/Include -I"C:/Users/ramaz/STM32CubeIDE/workspace_1.19.0/Traductor Morse Sonoro/app" -I"C:/Users/ramaz/STM32CubeIDE/workspace_1.19.0/Traductor Morse Sonoro/app/inc" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfloat-abi=soft -mthumb -o "$@"

clean: clean-app-2f-src

clean-app-2f-src:
	-$(RM) ./app/src/app.cyclo ./app/src/app.d ./app/src/app.o ./app/src/app.su ./app/src/logger.cyclo ./app/src/logger.d ./app/src/logger.o ./app/src/logger.su ./app/src/systick.cyclo ./app/src/systick.d ./app/src/systick.o ./app/src/systick.su ./app/src/task_HC05.cyclo ./app/src/task_HC05.d ./app/src/task_HC05.o ./app/src/task_HC05.su ./app/src/task_mic.cyclo ./app/src/task_mic.d ./app/src/task_mic.o ./app/src/task_mic.su ./app/src/task_sensor.cyclo ./app/src/task_sensor.d ./app/src/task_sensor.o ./app/src/task_sensor.su

.PHONY: clean-app-2f-src

