################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/core/ClientSocket.cpp \
../src/core/Condition.cpp \
../src/core/DataInterface.cpp \
../src/core/DataPacket.cpp \
../src/core/FloatValue.cpp \
../src/core/InPort.cpp \
../src/core/IntValue.cpp \
../src/core/Mutex.cpp \
../src/core/OutPort.cpp \
../src/core/SerialDevice.cpp \
../src/core/Socket.cpp \
../src/core/StreamTask.cpp \
../src/core/TBObject.cpp \
../src/core/Thread.cpp \
../src/core/Timer.cpp \
../src/core/Value.cpp 

OBJS += \
./src/core/ClientSocket.o \
./src/core/Condition.o \
./src/core/DataInterface.o \
./src/core/DataPacket.o \
./src/core/FloatValue.o \
./src/core/InPort.o \
./src/core/IntValue.o \
./src/core/Mutex.o \
./src/core/OutPort.o \
./src/core/SerialDevice.o \
./src/core/Socket.o \
./src/core/StreamTask.o \
./src/core/TBObject.o \
./src/core/Thread.o \
./src/core/Timer.o \
./src/core/Value.o 

CPP_DEPS += \
./src/core/ClientSocket.d \
./src/core/Condition.d \
./src/core/DataInterface.d \
./src/core/DataPacket.d \
./src/core/FloatValue.d \
./src/core/InPort.d \
./src/core/IntValue.d \
./src/core/Mutex.d \
./src/core/OutPort.d \
./src/core/SerialDevice.d \
./src/core/Socket.d \
./src/core/StreamTask.d \
./src/core/TBObject.d \
./src/core/Thread.d \
./src/core/Timer.d \
./src/core/Value.d 


# Each subdirectory must supply rules for building sources it contributes
src/core/%.o: ../src/core/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


