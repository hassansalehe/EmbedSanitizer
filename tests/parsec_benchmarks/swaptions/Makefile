################################################
# Copyright (c) 2017, Hassan Salehe Matar
#
# This file contains necessary compiler, linker,
# and include flags for compiling the swaptions
# benchmark for 32-bit ARM binary.
################################################

CXXFLAGS += -pthread -DENABLE_THREADS -static -g
CLDFLAGS += -L/usr/arm-linux-gnueabi/lib

# Add includes for C++ library files: iostream, cstring, cstdlib, bits/config.h, etc
CXXFLAGS += -I$(shell find /usr/arm-linux-gnueabi -name iostream | sed 's/\/iostream//g')
CXXFLAGS += -I$(shell find /usr/arm-linux-gnueabi/include -type d -name arm-linux-gnueabi)
CXXFLAGS += -I/usr/arm-linux-gnueabi/include

ARM_CXX=../../../arm/bin/clang++

TSANFLGS = -fsanitize=thread

#CXXFLAGS += -L/usr/lib64 -L/usr/lib

EXEC = swaptions_arm_instrumented.exe

OBJS= CumNormalInv.o MaxFunction.o RanUnif.o nr_routines.o icdf.o \
	HJM_SimPath_Forward_Blocking.o HJM.o HJM_Swaption_Blocking.o  \
	HJM_Securities.o

all: $(EXEC)

$(EXEC): $(OBJS)
	$(ARM_CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJS) $(TSANFLGS) $(LIBS) -o $(EXEC)

.cpp.o:
	$(ARM_CXX) $(CXXFLAGS) $(TSANFLGS) -c $*.cpp -o $*.o

.c.o:
	$(ARM_CXX) $(CXXFLAGS) $(TSANFLGS) -c $*.c -o $*.o

clean:
	rm -f *.o *.exe

