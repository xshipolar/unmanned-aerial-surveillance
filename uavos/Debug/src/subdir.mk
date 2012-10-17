################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/comsys.cpp \
../src/uavos.cpp 

OBJS += \
./src/comsys.o \
./src/uavos.o 

CPP_DEPS += \
./src/comsys.d \
./src/uavos.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -I/home/uav-dev/uav-code/uavos/include -I/home/oliver/workspace/unmanned-aerial-surveillance/uavos/include -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


