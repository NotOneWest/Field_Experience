################################################################################
# Automatically-generated file. Do not edit!
# Toolchain: GNU Tools for STM32 (11.3.rel1)
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Application/User/Components/wm8994/wm8994.c 

C_DEPS += \
./Application/User/Components/wm8994/wm8994.d 

OBJS += \
./Application/User/Components/wm8994/wm8994.o 


# Each subdirectory must supply rules for building sources it contributes
Application/User/Components/wm8994/%.o Application/User/Components/wm8994/%.su Application/User/Components/wm8994/%.cyclo: ../Application/User/Components/wm8994/%.c Application/User/Components/wm8994/subdir.mk
	arm-none-eabi-gcc "$<" -mcpu=cortex-m7 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DUSE_BPP=16 -DSTM32F750xx -DDEBUG -c -I../../Core/Inc -I../../Drivers/CMSIS/Include -I../../Drivers/BSP/STM32F7508-Discovery -I../../TouchGFX/target -I../../Drivers/CMSIS/Device/ST/STM32F7xx/Include -I../../Drivers/STM32F7xx_HAL_Driver/Inc -I../../TouchGFX/App -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I../../TouchGFX/target/generated -I../../Drivers/STM32F7xx_HAL_Driver/Inc/Legacy -I../../Drivers/BSP/Components/Common -I../../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM7/r0p1 -I../../Middlewares/Third_Party/FreeRTOS/Source/include -I../../Middlewares/ST/touchgfx/framework/include -I../../TouchGFX/generated/fonts/include -I../../TouchGFX/generated/gui_generated/include -I../../TouchGFX/generated/images/include -I../../TouchGFX/generated/texts/include -I../../TouchGFX/gui/include -I../../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS_V2 -I../../TouchGFX/generated/videos/include -I../../FATFS/Target -I../../FATFS/App -I../../Middlewares/Third_Party/FatFs/src -Os -ffunction-sections -fdata-sections -Wall -fstack-usage -fcyclomatic-complexity -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" --specs=nano.specs -mfpu=fpv5-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

clean: clean-Application-2f-User-2f-Components-2f-wm8994

clean-Application-2f-User-2f-Components-2f-wm8994:
	-$(RM) ./Application/User/Components/wm8994/wm8994.cyclo ./Application/User/Components/wm8994/wm8994.d ./Application/User/Components/wm8994/wm8994.o ./Application/User/Components/wm8994/wm8994.su

.PHONY: clean-Application-2f-User-2f-Components-2f-wm8994

