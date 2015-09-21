################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../image_HugoMachado.c \
../iups_HugoMachado.c \
../main_HugoMachado.c 

OBJS += \
./image_HugoMachado.o \
./iups_HugoMachado.o \
./main_HugoMachado.o 

C_DEPS += \
./image_HugoMachado.d \
./iups_HugoMachado.d \
./main_HugoMachado.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/iup -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


