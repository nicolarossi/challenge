################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
CPP_SRCS += \
../src/PACKET.cpp \
../src/PACKET_HEADER.cpp \
../src/PAS.cpp \
../src/PAT.cpp \
../src/PES_packet.cpp \
../src/SYSTEM_HEADER.cpp \
../src/challenge.cpp \
../src/common_tool.cpp \
../src/demuxer.cpp 

OBJS += \
./src/PACKET.o \
./src/PACKET_HEADER.o \
./src/PAS.o \
./src/PAT.o \
./src/PES_packet.o \
./src/SYSTEM_HEADER.o \
./src/challenge.o \
./src/common_tool.o \
./src/demuxer.o 

CPP_DEPS += \
./src/PACKET.d \
./src/PACKET_HEADER.d \
./src/PAS.d \
./src/PAT.d \
./src/PES_packet.d \
./src/SYSTEM_HEADER.d \
./src/challenge.d \
./src/common_tool.d \
./src/demuxer.d 


# Each subdirectory must supply rules for building sources it contributes
src/%.o: ../src/%.cpp
	@echo 'Building file: $<'
	@echo 'Invoking: GCC C++ Compiler'
	g++ -O0 -g3 -Wall -c -fmessage-length=0 -MMD -MP -MF"$(@:%.o=%.d)" -MT"$(@)" -o "$@" "$<"
	@echo 'Finished building: $<'
	@echo ' '


