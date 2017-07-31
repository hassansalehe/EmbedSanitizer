################################################
# Copyright (c) 2017, Hassan Salehe Matar
#
# This file contains necessary compiler, linker,
# and include flags for compiling the blackscholes
# benchmark for 32-bit ARM binary.
################################################

#env version=pthreads > /dev/null 2>&1
#rm -f blackscholes blackscholes.m4.cpp
#/usr/bin/m4 ./c.m4.pthreads blackscholes.c > blackscholes.m4.cpp

CXXFLAGS += -g -O3 -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions
CXXFLAGS += -static-libgcc -Wl,--hash-style=both,--as-needed -DNCO=4
CXXFLAGS += -pthread -DENABLE_THREADS

ARMFLAGS  = -static -L/usr/arm-linux-gnueabi/lib

# Add includes for C++ library files: iostream, cstring, cstdlib, bits/config.h, etc
ARMFLAGS += -I$(shell find /usr/arm-linux-gnueabi -name iostream | sed 's/\/iostream//g')
ARMFLAGS += -I$(shell find /usr/arm-linux-gnueabi/include -type d -name arm-linux-gnueabi)
ARMFLAGS += -I/usr/arm-linux-gnueabi/include
TSANFLGS += -fsanitize=thread

ARM_CXX=../../../arm/bin/clang++

# flags for building x86_64 instrumented version of program
LIBFLAGS += -L/usr/lib64 -L/usr/lib -fPIC -shared

all:	arm arm_instr x86_64_instr

arm:
	$(ARM_CXX) blackscholes.m4.cpp -o blackscholes_arm.exe $(CXXFLAGS) $(ARMFLAGS)

arm_instr:
	$(ARM_CXX) blackscholes.m4.cpp -o blackscholes_arm_instrumented.exe $(CXXFLAGS) $(ARMFLAGS) $(TSANFLGS)

x86_64_instr:
	$(CXX) blackscholes.m4.cpp -o blackscholes_x64_instrumented.exe $(CXXFLAGS) $(LIBFLAGS) $(TSANFLGS)

clean:
	rm -rf *.o *.exe
