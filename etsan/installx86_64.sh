#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the licence file LICENSE.md
#
#
# This script builds and installs the EmbedSanitizer race detection runtime
# for x86_64 for testing purposes.
#
#########################################################################

# Variables
DEST=../x86_64/lib/clang/5.0.0/lib/linux/
tSanLib=libclang_rt.tsan_cxx-x86_64

# Compile and build static library
g++ -c  tsan_interface.cc -o ${tSanLib}.o -static -std=c++11 -pthread -fpermissive
#arm-linux-gnueabi-g++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static
#../bin/bin/clang++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static -I/usr/arm-linux-gnueabi/include/c++/5/arm-linux-gnueabi -I/usr/arm-linux-gnueabi/include
ar rcs ${tSanLib}.a ${tSanLib}.o

# Dummy C library
tSanLibc=libclang_rt.tsan-x86_64
gcc -c  dummy.c -o ${tSanLibc}.o
ar rcs ${tSanLibc}.a ${tSanLibc}.o


# Copy library to destination
mkdir -p $DEST
mv ${tSanLib}.a $DEST
mv ${tSanLibc}.a $DEST

# Check if everything is OK
if [ $? -eq 0 ]; then
    rm -rf *.o *.a
    echo "Dummy tsan lib successful installed"
fi
