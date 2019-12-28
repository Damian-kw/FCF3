################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../Src/user/MedianFilter.c \
../Src/user/battery_monitor.c \
../Src/user/bmi008_accelerometer.c \
../Src/user/bmi088_gyroscope.c \
../Src/user/buzzer.c \
../Src/user/complementary_filter.c \
../Src/user/conversion.c \
../Src/user/eeprom.c \
../Src/user/esc.c \
../Src/user/flight_controler.c \
../Src/user/float_compare.c \
../Src/user/ibus_receiver.c \
../Src/user/imu.c \
../Src/user/interface_implementation.c \
../Src/user/moving_average.c \
../Src/user/mpu.c \
../Src/user/pid_controler.c \
../Src/user/salloc.c \
../Src/user/uart.c 

OBJS += \
./Src/user/MedianFilter.o \
./Src/user/battery_monitor.o \
./Src/user/bmi008_accelerometer.o \
./Src/user/bmi088_gyroscope.o \
./Src/user/buzzer.o \
./Src/user/complementary_filter.o \
./Src/user/conversion.o \
./Src/user/eeprom.o \
./Src/user/esc.o \
./Src/user/flight_controler.o \
./Src/user/float_compare.o \
./Src/user/ibus_receiver.o \
./Src/user/imu.o \
./Src/user/interface_implementation.o \
./Src/user/moving_average.o \
./Src/user/mpu.o \
./Src/user/pid_controler.o \
./Src/user/salloc.o \
./Src/user/uart.o 

C_DEPS += \
./Src/user/MedianFilter.d \
./Src/user/battery_monitor.d \
./Src/user/bmi008_accelerometer.d \
./Src/user/bmi088_gyroscope.d \
./Src/user/buzzer.d \
./Src/user/complementary_filter.d \
./Src/user/conversion.d \
./Src/user/eeprom.d \
./Src/user/esc.d \
./Src/user/flight_controler.d \
./Src/user/float_compare.d \
./Src/user/ibus_receiver.d \
./Src/user/imu.d \
./Src/user/interface_implementation.d \
./Src/user/moving_average.d \
./Src/user/mpu.d \
./Src/user/pid_controler.d \
./Src/user/salloc.d \
./Src/user/uart.d 


# Each subdirectory must supply rules for building sources it contributes
Src/user/%.o: ../Src/user/%.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo $(PWD)
	arm-none-eabi-gcc -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 '-D__weak=__attribute__((weak))' '-D__packed=__attribute__((__packed__))' -DUSE_HAL_DRIVER -DSTM32F303x8 -I"C:/Users/damiank/Desktop/FCF3_FIRMWARE/Inc" -I"C:/Users/damiank/Desktop/FCF3_FIRMWARE/Inc/user" -IC:/Users/damiank/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.0/Drivers/STM32F3xx_HAL_Driver/Inc -IC:/Users/damiank/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.0/Drivers/STM32F3xx_HAL_Driver/Inc/Legacy -IC:/Users/damiank/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Device/ST/STM32F3xx/Include -IC:/Users/damiank/STM32Cube/Repository/STM32Cube_FW_F3_V1.11.0/Drivers/CMSIS/Include  -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


