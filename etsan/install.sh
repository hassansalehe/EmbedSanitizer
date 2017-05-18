#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the licence file LICENSE.md
#
#
# This script builds and installs the EmbedSanitizer race detection runtime
# for 32-bit ARM
#
#########################################################################

# Variables
DEST=../arm/lib/clang/4.0.0/lib/linux/
tSanLib=libclang_rt.tsan_cxx-arm

# Compile and build static library
arm-linux-gnueabi-g++ -c  tsan_interface.cc -o ${tSanLib}.o -static -std=c++11 -pthread
#arm-linux-gnueabi-g++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static
#../bin/bin/clang++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static -I/usr/arm-linux-gnueabi/include/c++/5/arm-linux-gnueabi -I/usr/arm-linux-gnueabi/include
arm-linux-gnueabi-ar rcs ${tSanLib}.a ${tSanLib}.o

# Dummy C library
tSanLibc=libclang_rt.tsan-arm
arm-linux-gnueabi-gcc -c  dummy.c -o ${tSanLibc}.o
arm-linux-gnueabi-ar rcs ${tSanLibc}.a ${tSanLibc}.o


# Copy library to destination
mkdir -p $DEST
mv ${tSanLib}.a $DEST
mv ${tSanLibc}.a $DEST

# Check if everything is OK
if [ $? -eq 0 ]; then
    rm -rf *.o *.a
    echo " tsan lib successful installed"
fi
