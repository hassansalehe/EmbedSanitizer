#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the license file Licence.md
#
#########################################################################

# clone release version 4.0 of LLVM/Clang
version=4.0.0
HOME=`pwd`

BuildDir=$HOME/build
SrcDir=$HOME/build/llvm
InstallDir=$HOME/arm

Usage() {
  echo ""
  echo -e "\033[1;36m EmbedSanitizer: A Runtime Race Detection Tool for 32-bit Embedded ARM\033[m"
  echo ""
  echo -e "\033[1;32m ./install.sh <option>\033[m"
  echo "  e.g.   ./install.sh --new"
  echo ""
  echo "  <option> can be empty or one of the following:"
  echo "    --help   : prints this help message"
  echo "    --clean  : cleans the whole installation"
  echo "    --new    : For a complete new installation"
  echo ""
  echo " (c) 2017, Hassan Salehe Matar"
  echo " See LICENSE.md for license information."
}

# Check command arguments
if [ $# -gt 0 ]; then
  if [ $1 == "--help" ]; then
    Usage
    exit 0
  elif [ $1 == "--clean" ]; then
    rm -rf $BuildDir
    rm -rf $InstallDir
    exit 0
  elif [ $1 == "--new" ]; then
    rm -rf $BuildDir
    rm -rf $InstallDir
  fi
fi

# Checks if previous command was successful.
# Exit script otherwise.
reportIfSuccessful() {
if [ $? -eq 0 ]; then
    echo -e "\033[1;32m Done.\033[m"
    return 0
else
    echo -e "\033[1;31m Fail.\033[m"
    exit 1
fi
}


# Check if relevant downloads are available
if [ ! -f "./.status" ]; then

mkdir -p $BuildDir
mkdir -p $SrcDir
mkdir -p $InstallDir

# Step 1:
#   Download original(unmodified) LLVM/Clang files from a remote serer.
#   The code will be stored under ./build/llvm
#   Note: the individual, tracked files under ./llvm remain untouched and
#   will later be copied to .build/llvm
echo -e "\033[1;95m Downloading LLVM/Clang files from remote server\033[m"

cd $SrcDir
wget -c http://releases.llvm.org/${version}/llvm-${version}.src.tar.xz
tar xf llvm-${version}.src.tar.xz --strip-components 1
rm llvm-${version}.src.tar.xz

cd $SrcDir
mkdir -p tools/clang
cd tools/clang
wget -c http://releases.llvm.org/${version}/cfe-${version}.src.tar.xz
tar xf cfe-${version}.src.tar.xz --strip-components 1
rm cfe-${version}.src.tar.xz

mkdir -p tools/extra
cd tools/extra
wget -c http://releases.llvm.org/${version}/clang-tools-extra-${version}.src.tar.xz
tar xf clang-tools-extra-${version}.src.tar.xz --strip-components 1
rm clang-tools-extra-${version}.src.tar.xz

cd $SrcDir
mkdir -p projects/compiler-rt
cd projects/compiler-rt
wget -c http://releases.llvm.org/${version}/compiler-rt-${version}.src.tar.xz
tar xf compiler-rt-${version}.src.tar.xz --strip-components 1
rm compiler-rt-${version}.src.tar.xz

cd $SrcDir
mkdir -p projects/libcxx
cd projects/libcxx
wget -c http://releases.llvm.org/${version}/libcxx-${version}.src.tar.xz
tar xf libcxx-${version}.src.tar.xz --strip-components 1
rm libcxx-${version}.src.tar.xz

cd $SrcDir
mkdir -p projects/libcxxabi
cd projects/libcxxabi
wget -c http://releases.llvm.org/${version}/libcxxabi-${version}.src.tar.xz
tar xf libcxxabi-${version}.src.tar.xz --strip-components 1
rm libcxxabi-${version}.src.tar.xz

cd $SrcDir
mkdir -p projects/libunwind
cd projects/libunwind
wget -c http://releases.llvm.org/${version}/libunwind-${version}.src.tar.xz
tar xf libunwind-${version}.src.tar.xz --strip-components 1
rm libunwind-${version}.src.tar.xz

cd $SrcDir
mkdir -p projects/test-suite
cd projects/test-suite
wget -c http://releases.llvm.org/${version}/test-suite-${version}.src.tar.xz
tar xf test-suite-${version}.src.tar.xz --strip-components 1
rm test-suite-${version}.src.tar.xz

# Report if everything is OK so far
reportIfSuccessful;
if [ $? -eq 0 ]; then touch "$home/.status"; fi

fi # end of downloads

# Step 2:
#   Copy EmbedSanitizer files to ./build/llvm
cp -r $HOME/llvm/* $SrcDir/

# Step 3:
#   Go to "build" and configure cmake for compiling LLVM/Clang.
echo -e "\033[1;95m Configuring build for LLVM/Clang\033[m"
mkdir -p $BuildDir/build
cd $BuildDir/build
cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release -DCMAKE_CROSSCOMPILING=True -DLLVM_DEFAULT_TARGET_TRIPLE=arm-none-linux-gnueabi -DLLVM_TARGET_ARCH=armv7-a -DLLVM_TARGETS_TO_BUILD=all -DCMAKE_INSTALL_PREFIX=$InstallDir $SrcDir

# Report if everything is OK so far
reportIfSuccessful;

# Step 4:
#   Build the LLVM/Clang with EmbedSanitizer in it
echo -e "\033[1;95m Building the LLVM/Clang embedded compiler\033[m"
procNo=`cat /proc/cpuinfo | grep processor | wc -l`
make -j $procNo
make install

# Report if everything is OK so far
reportIfSuccessful;

# Step 5: build and install the embedded tsan runtime
echo -e "\033[1;95m Building the embedded static race detection runtime\033[m"
cd $HOME/etsan
./install.sh

reportIfSuccessful;
