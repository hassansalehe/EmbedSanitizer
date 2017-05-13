#env version=pthreads > /dev/null 2>&1
#rm -f blackscholes blackscholes.m4.cpp
#/usr/bin/m4 ./c.m4.pthreads blackscholes.c > blackscholes.m4.cpp

include ../Makefile.inc

CXXFLAGS += -O3 -funroll-loops -fprefetch-loop-arrays -fpermissive -fno-exceptions
CXXFLAGS += -static-libgcc -Wl,--hash-style=both,--as-needed -DNCO=4

all:
	$(CXX) blackscholes.m4.cpp -o $(platform)blackscholes $(CXXFLAGS)

clean:
	rm $(platform)blackscholes
