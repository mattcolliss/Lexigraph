################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../connectedComponnetLabelling.c \
../convolution.c \
../sizeFilter.c \
../sobelFilter.c \
../test.c 

OBJS += \
./connectedComponnetLabelling.o \
./convolution.o \
./sizeFilter.o \
./sobelFilter.o \
./test.o 

C_DEPS += \
./connectedComponnetLabelling.d \
./convolution.d \
./sizeFilter.d \
./sobelFilter.d \
./test.d 


# Each subdirectory must supply rules for building sources it contributes
%.o: ../%.c
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C Compiler'
	gcc -I/usr/include/opencv -O2 -g3 -pedantic -Wall -c -fmessage-length=0 -std=c99 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o"$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


