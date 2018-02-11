################################################
# Copyright (c) 2017 - 2018, Hassan Salehe Matar
#
# This file contains necessary compiler, linker,
# and include flags for compiling the streamcluster
# benchmark for 32-bit ARM binary.
################################################


# Makefile for streamcluster

CXXFLAGS += -g -O3 -pthread -DENABLE_THREADS

ARMFLAGS  = -static -L/usr/arm-linux-gnueabi/lib

# Add includes for C++ library files: iostream, cstring, cstdlib, bits/config.h, etc
ARMFLAGS += -I$(shell find /usr/arm-linux-gnueabi -name iostream | sed 's/\/iostream//g')
ARMFLAGS += -I$(shell find /usr/arm-linux-gnueabi/include -type d -name arm-linux-gnueabi)
ARMFLAGS += -I/usr/arm-linux-gnueabi/include
TSANFLGS += -fsanitize=thread

ARM_CXX=../../../arm/bin/clang++

# Library linkage for producing x86_64 instrumented binary
LIBFLAGS += -L/usr/lib64 -L/usr/lib -fPIC -shared

all:    arm arm_instr x86_64_instr

arm:
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) 		\
        streamcluster.cpp parsec_barrier.cpp 		\
        -o streamcluster_arm.exe

arm_instr:
	$(ARM_CXX) $(CXXFLAGS) $(ARMFLAGS) $(TSANFLGS) 	\
        streamcluster.cpp parsec_barrier.cpp -o 	\
        streamcluster_arm_instrumented.exe

x86_64_instr:
	$(CXX) $(CXXFLAGS) $(LIBFLAGS) $(TSANFLGS) 	\
        streamcluster.cpp parsec_barrier.cpp -o 	\
        streamcluster_x64_instrumented.exe

clean:
	rm -rf *.o *.exe
