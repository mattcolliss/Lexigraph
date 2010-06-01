################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../analysis.c \
../connectedComponnetLabelling.c \
../implementationMain.c 

OBJS += \
./analysis.o \
./connectedComponnetLabelling.o \
./implementationMain.o 

C_DEPS += \
./analysis.d \
./connectedComponnetLabelling.d \
./implementationMain.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/opencv -O2 -g3 -pedantic -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


