################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../SrcInterface/baroInterface.c \
../SrcInterface/genericInterface.c \
../SrcInterface/gpsInterface.c \
../SrcInterface/nandInterface.c 

OBJS += \
./SrcInterface/baroInterface.o \
./SrcInterface/genericInterface.o \
./SrcInterface/gpsInterface.o \
./SrcInterface/nandInterface.o 

C_DEPS += \
./SrcInterface/baroInterface.d \
./SrcInterface/genericInterface.d \
./SrcInterface/gpsInterface.d \
./SrcInterface/nandInterface.d 


# Each subdirectory must supply rules for building sources it contributes
SrcInterface/baroInterface.o: ../SrcInterface/baroInterface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcInterface/baroInterface.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcInterface/genericInterface.o: ../SrcInterface/genericInterface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcInterface/genericInterface.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcInterface/gpsInterface.o: ../SrcInterface/gpsInterface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcInterface/gpsInterface.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"
SrcInterface/nandInterface.o: ../SrcInterface/nandInterface.c
	arm-none-eabi-gcc "$<" -mcpu=cortex-m4 -std=gnu11 -g3 -DUSE_HAL_DRIVER -DSTM32F411xE -DDEBUG -c -I../Core/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc -I../Drivers/STM32F4xx_HAL_Driver/Inc/Legacy -I../Drivers/CMSIS/Device/ST/STM32F4xx/Include -I../Drivers/CMSIS/Include -I../Middlewares/Third_Party/FreeRTOS/Source/include -I../Middlewares/Third_Party/FreeRTOS/Source/portable/GCC/ARM_CM4F -I../Middlewares/Third_Party/FreeRTOS/Source/CMSIS_RTOS -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncPSP" -I"C:/Users/Jeff/Documents/GitHub/AnR/MITCH_2021/IncInterface" -O0 -ffunction-sections -fdata-sections -Wall -fstack-usage -MMD -MP -MF"SrcInterface/nandInterface.d" -MT"$@" --specs=nano.specs -mfpu=fpv4-sp-d16 -mfloat-abi=hard -mthumb -o "$@"

