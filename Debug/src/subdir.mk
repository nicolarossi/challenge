################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/Demuxer.cpp \
../src/PES_packet.cpp \
../src/TS_Packet.cpp \
../src/challenge.cpp \
../src/common_tool.cpp 

OBJS += \
./src/Demuxer.o \
./src/PES_packet.o \
./src/TS_Packet.o \
./src/challenge.o \
./src/common_tool.o 

CPP_DEPS += \
./src/Demuxer.d \
./src/PES_packet.d \
./src/TS_Packet.d \
./src/challenge.d \
./src/common_tool.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g -pg -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


