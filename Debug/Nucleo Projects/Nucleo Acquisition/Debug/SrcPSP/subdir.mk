################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/acquisition.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/common.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/controlLogic.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/monitoring.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/processing.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/storage.c \
C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/transmission.c 

OBJS += \
./SrcPSP/acquisition.o \
./SrcPSP/common.o \
./SrcPSP/controlLogic.o \
./SrcPSP/monitoring.o \
./SrcPSP/processing.o \
./SrcPSP/storage.o \
./SrcPSP/transmission.o 

C_DEPS += \
./SrcPSP/acquisition.d \
./SrcPSP/common.d \
./SrcPSP/controlLogic.d \
./SrcPSP/monitoring.d \
./SrcPSP/processing.d \
./SrcPSP/storage.d \
./SrcPSP/transmission.d 


# Each subdirectory must supply rules for building sources it contributes
SrcPSP/acquisition.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/acquisition.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/acquisition.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/common.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/common.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/common.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/controlLogic.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/controlLogic.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/controlLogic.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/monitoring.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/monitoring.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/monitoring.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/processing.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/processing.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/processing.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/storage.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/storage.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/storage.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcPSP/transmission.o: C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/SrcPSP/transmission.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcPSP/transmission.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

