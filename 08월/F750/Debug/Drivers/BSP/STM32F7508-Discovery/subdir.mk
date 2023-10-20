################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.c \
../Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.c 

OBJS += \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.o \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.o 

C_DEPS += \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.d \
./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.d 


# Each subdirectory must supply rules for building sources it contributes
Drivers/BSP/STM32F7508-Discovery/%.o Drivers/BSP/STM32F7508-Discovery/%.su Drivers/BSP/STM32F7508-Discovery/%.cyclo: ../Drivers/BSP/STM32F7508-Discovery/%.c Drivers/BSP/STM32F7508-Discovery/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DDEBUG -DARM_MATH_CM7 -DUSE_HAL_DRIVER -DSTM32F750xx -c -I../Drivers/CMSIS/DSP/Include -I"C:/ST/F750/Drivers/BSP/STM32F7508-Discovery" -I../Core/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc -I../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../Drivers/CMSIS/Include -I../FATFS/Target -I../FATFS/App -I../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Drivers-2f-BSP-2f-STM32F7508-2d-Discovery

clean-Drivers-2f-BSP-2f-STM32F7508-2d-Discovery:
	-$(RM) ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_audio.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_camera.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_eeprom.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_lcd.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_qspi.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sd.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_sdram.su ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.cyclo ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.d ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.o ./Drivers/BSP/STM32F7508-Discovery/stm32f7508_discovery_ts.su

.PHONY: clean-Drivers-2f-BSP-2f-STM32F7508-2d-Discovery

