################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS += \
../code/Camera.c \
../code/Cross.c \
../code/Encoder.c \
../code/Flash.c \
../code/Fussy.c \
../code/Ips.c \
../code/Led.c \
../code/Maincontrol.c \
../code/Motor.c \
../code/Pid.c \
../code/Ring.c \
../code/Tof.c \
../code/Uart.c \
../code/imu.c 

OBJS += \
./code/Camera.o \
./code/Cross.o \
./code/Encoder.o \
./code/Flash.o \
./code/Fussy.o \
./code/Ips.o \
./code/Led.o \
./code/Maincontrol.o \
./code/Motor.o \
./code/Pid.o \
./code/Ring.o \
./code/Tof.o \
./code/Uart.o \
./code/imu.o 

COMPILED_SRCS += \
./code/Camera.src \
./code/Cross.src \
./code/Encoder.src \
./code/Flash.src \
./code/Fussy.src \
./code/Ips.src \
./code/Led.src \
./code/Maincontrol.src \
./code/Motor.src \
./code/Pid.src \
./code/Ring.src \
./code/Tof.src \
./code/Uart.src \
./code/imu.src 

C_DEPS += \
./code/Camera.d \
./code/Cross.d \
./code/Encoder.d \
./code/Flash.d \
./code/Fussy.d \
./code/Ips.d \
./code/Led.d \
./code/Maincontrol.d \
./code/Motor.d \
./code/Pid.d \
./code/Ring.d \
./code/Tof.d \
./code/Uart.d \
./code/imu.d 


# Each subdirectory must supply rules for building sources it contributes
code/%.src: ../code/%.c code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING C/C++ Compiler'
	cctc -D__CPU__=tc26xb "-fF:/AURIXworkspace/2023.3.2--/Debug/TASKING_C_C___Compiler-Include_paths.opt" --iso=99 --c++14 --language=+volatile --exceptions --anachronisms --fp-model=3 -O0 --tradeoff=4 --compact-max-size=200 -g -Wc-w544 -Wc-w557 -Ctc26xb -o "$@"  "$<"  -cs --dep-file="$(@:.src=.d)" --misrac-version=2012 -N0 -Z0 -Y0 2>&1;
	@echo 'Finished building: $<'
	@echo ' '

code/%.o: ./code/%.src code/subdir.mk
	@echo 'Building file: $<'
	@echo 'Invoking: TASKING Assembler'
	astc -Og -Os --no-warnings= --error-limit=42 -o  "$@" "$<" --list-format=L1 --optimize=gs
	@echo 'Finished building: $<'
	@echo ' '


