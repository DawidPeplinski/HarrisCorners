################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/cameraMode.cpp \
../src/fileMode.cpp \
../src/harrisCorners.cpp \
../src/main.cpp 

OBJS += \
./src/cameraMode.o \
./src/fileMode.o \
./src/harrisCorners.o \
./src/main.o 

CPP_DEPS += \
./src/cameraMode.d \
./src/fileMode.d \
./src/harrisCorners.d \
./src/main.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: Cross G++ Compiler'
	g++ -I/usr/include/opencv4 -O3 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


