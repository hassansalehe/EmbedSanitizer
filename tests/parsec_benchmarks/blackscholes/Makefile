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
ARMFLAGS += -I/usr/arm-linux-gnueabi/include
ARMFLAGS += -I/usr/arm-linux-gnueabi/include/c++/5/arm-linux-gnueabi
TSANFLGS += -fsanitize=thread

ARM_CXX=../../../arm/bin/clang++

LIBFLAGS += -L/usr/lib64 -L/usr/lib

all:	arm arm_instr x86_64_instr

arm:
	$(ARM_CXX) blackscholes.m4.cpp -o blackscholes_arm.exe $(CXXFLAGS) $(ARMFLAGS)

arm_instr:
	$(ARM_CXX) blackscholes.m4.cpp -o blackscholes_arm_instrumented.exe $(CXXFLAGS) $(ARMFLAGS) $(TSANFLGS)

x86_64_instr:
	$(CXX) blackscholes.m4.cpp -o blackscholes_x64_instrumented.exe $(CXXFLAGS) $(LIBFLAGS) $(TSANFLGS)

clean:
	rm -rf *.o *.exe
