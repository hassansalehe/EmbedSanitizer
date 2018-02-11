################################################
# Copyright (c) 2017 - 2018, Hassan Salehe Matar
#
# This file contains necessary compiler, linker,
# and include flags for compiling the fluidanimate
# benchmark for 32-bit ARM binary.
################################################

CXXFLAGS += -Wno-invalid-offsetof -D_GNU_SOURCE -D__XOPEN_SOURCE=600
CXXFLAGS += -pthread -DENABLE_THREADS -static -g

# Add includes for C++ library files: iostream, cstring, cstdlib, bits/config.h, etc
CXXFLAGS += -I$(shell find /usr/arm-linux-gnueabi -name iostream | sed 's/\/iostream//g')
CXXFLAGS += -I$(shell find /usr/arm-linux-gnueabi/include -type d -name arm-linux-gnueabi)
CXXFLAGS += -I/usr/arm-linux-gnueabi/include
LDFLAGS  += -L/usr/arm-linux-gnueabi/lib

ARM_CXX=../../../arm/bin/clang++

CXXFLAGS += -fsanitize=thread

#CXXFLAGS += -L/usr/lib64 -L/usr/lib

OBJS     = pthreads.o cellpool.o parsec_barrier.o

# To enable visualization comment out the following lines (don't do this for benchmarking)
#OBJS     += fluidview.o
#CXXFLAGS += -DENABLE_VISUALIZATION
#LIBS     += -lglut

all: clean pthreads fluidcmp

pthreads: $(OBJS)
	$(ARM_CXX) $(CXXFLAGS) $(OBJS) $(LDFLAGS) $(LIBS) -o fluidanimate_arm_instrumented.exe

%.o : %.cpp
	$(ARM_CXX) $(CXXFLAGS) -c $<

fluidcmp: fluidcmp.cpp
	rm -rf fluidcmp
	$(ARM_CXX) $(CXXFLAGS) fluidcmp.cpp -o fluidcmp

clean:
	rm -rf *.o *.exe fluidcmp
