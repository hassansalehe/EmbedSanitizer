################################################
# Copyright (c) 2017, Hassan Salehe Matar
#
# This file contains necessary compiler, linker,
# and include flags for compiling the Parsec
# benchmarks for 32-bit ARM binary.
################################################

CXXFLAGS += -pthread -DENABLE_THREADS -static -g
CXXFLAGS += -L/usr/arm-linux-gnueabi/lib
CXXFLAGS += -I/usr/arm-linux-gnueabi/include -I/usr/arm-linux-gnueabi/include/c++/5/arm-linux-gnueabi

CXX=/home/hmatar/Desktop/EmbedSanitizer/arm/bin/clang++
#platform=armNormal

### For ARM instrumentation ###
CXXFLAGS += -fsanitize=thread
platform=armInstr

### For x86_64 instrumentation ###
#CXX=/usr/bin/clang++
#CXX=/home/hmatar/Desktop/EmbedSanitizer/x86_64/bin/clang++
#CXXFLAGS += -L/usr/lib64 -L/usr/lib
#platform=x86_64Normal


# Known threadSantizer
# platform=ThreadSanitizerInstr
