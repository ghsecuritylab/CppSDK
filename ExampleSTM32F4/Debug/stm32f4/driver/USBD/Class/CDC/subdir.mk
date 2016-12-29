################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBD/Class/CDC/usbd_cdc_desc.c 

CPP_SRCS += \
C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBD/Class/CDC/usbd_cdc_interface.cpp 

OBJS += \
./stm32f4/driver/USBD/Class/CDC/usbd_cdc_desc.o \
./stm32f4/driver/USBD/Class/CDC/usbd_cdc_interface.o 

C_DEPS += \
./stm32f4/driver/USBD/Class/CDC/usbd_cdc_desc.d 

CPP_DEPS += \
./stm32f4/driver/USBD/Class/CDC/usbd_cdc_interface.d 


# Each subdirectory must supply rules for building sources it contributes
stm32f4/driver/USBD/Class/CDC/usbd_cdc_desc.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBD/Class/CDC/usbd_cdc_desc.c
	@echo 'Building file: $<'
	@echo 'Invoking: MCU GCC Compiler'
	@echo %cd%
	arm-none-eabi-gcc -x c++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '

stm32f4/driver/USBD/Class/CDC/usbd_cdc_interface.o: C:/GitHub/CppSDK/SDK/platform/stm32f4/driver/USBD/Class/CDC/usbd_cdc_interface.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: MCU G++ Compiler'
	@echo %cd%
	arm-none-eabi-g++ -mcpu=cortex-m4 -mthumb -mfloat-abi=hard -mfpu=fpv4-sp-d16 -DSTM32F469I_DISCO -DSTM32F4 -DSTM32F469NIHx -DSTM32 -DDEBUG -I"C:/GitHub/CppSDK/SDK/lib" -I"C:/GitHub/CppSDK/ExampleSTM32F4" -I"C:/GitHub/CppSDK/SDK/platform/stm32f4" -Og -g3 -Wall -fmessage-length=0 -ffunction-sections -c -fno-exceptions -fno-rtti -MMD -MP -MF"$(@:%.o=%.d)" -MT"$@" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


