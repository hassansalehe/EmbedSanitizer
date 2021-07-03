#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - 2021 Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the licence file LICENSE.md
#
#
# This script builds and installs the EmbedSanitizer race detection runtime
# for 32-bit ARM
#
#########################################################################

# Variables
DEST=${HOME}/.embedsanitizer/lib/clang/4.0.0/lib/linux/
tSanLib=libclang_rt.tsan_cxx-arm

# Function to Check if a prior command was successful
checkIfActionOK() {
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31m ERROR\033[m"
    exit 1
  fi
}

# Compile and build static library
arm-linux-gnueabi-g++ -c  tsan_interface.cc -o ${tSanLib}.o   \
  -static -std=c++11 -pthread -fpermissive
checkIfActionOK

#arm-linux-gnueabi-g++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static
#../bin/bin/clang++ -c tsan_fasttrack.cpp -o ${tSanLib}.o -static  \
#  -I/usr/arm-linux-gnueabi/include/c++/5/arm-linux-gnueabi  \
#  -I/usr/arm-linux-gnueabi/include
# Generate etsan as static library
arm-linux-gnueabi-ar rcs ${tSanLib}.a ${tSanLib}.o
checkIfActionOK

# Dummy C library
tSanLibc=libclang_rt.tsan-arm
arm-linux-gnueabi-gcc -c  dummy.c -o ${tSanLibc}.o
arm-linux-gnueabi-ar rcs ${tSanLibc}.a ${tSanLibc}.o
checkIfActionOK

# Copy library to destination
mkdir -p $DEST
mv ${tSanLib}.a $DEST
mv ${tSanLibc}.a $DEST
checkIfActionOK

# Finalize: remove temporary files
rm -rf *.o *.a
echo -e "\033[1;32m etsan runtime library successful installed.\033[m"
