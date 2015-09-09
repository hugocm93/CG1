################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../image_Fulano_Beltrano.c \
../iups.c \
../main_Fulano_Beltrano.c 

OBJS += \
./image_Fulano_Beltrano.o \
./iups.o \
./main_Fulano_Beltrano.o 

C_DEPS += \
./image_Fulano_Beltrano.d \
./iups.d \
./main_Fulano_Beltrano.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/iup -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


