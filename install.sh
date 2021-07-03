#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - 2021  Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the license file Licence.md
#
#########################################################################

# clone release version 4.0 of LLVM/Clang
version=4.0.1
EsanHomeDir=`pwd`

BuildDir=$EsanHomeDir/build
SrcDir=$EsanHomeDir/build/llvm

# Installation directory is created under Linux user's home directory.
# In Ubuntu by default and if user name is Umut then EmbedSanitizer 
# binary will be inside /home/Umut/.embedsanitizer 
InstallDir=${HOME}/.embedsanitizer

procNo=`cat /proc/cpuinfo | grep processor | wc -l`

# The help function
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
  echo " (c) 2017 - 2021, Hassan Salehe Matar"
  echo " See LICENSE.md for license information."
}

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

# Checks if necessary packages are installed in the machine
# Installs the packages if user approves.
CheckPrerequisites() {
  which wget > /dev/null
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mERROR! wget missing\033[m"
    read -p "Do you want to install wget (root password is needed) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then sudo apt-get install wget; fi
  fi

  which tar > /dev/null
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mERROR! tar missing\033[m"
    read -p "Do you want to install tar (root password is needed) " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then sudo apt-get install tar; fi
  fi

  # check if cmake is installed
  which cmake > /dev/null
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mERROR! cmake missing\033[m"
    read -p "Do you want to install cmake (root password is needed)? " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
      cd /usr/src
      sudo wget https://cmake.org/files/v3.10/cmake-3.10.3.tar.gz
      sudo tar -xvzf cmake-3.10.3.tar.gz
      cd cmake-3.10.3
      sudo ./bootstrap
      sudo make -j $procNo
      sudo make install
      cd $EsanHomeDir
    fi
  fi

  # check if cross-compiler utilities are installed
  which arm-linux-gnueabi-g++ > /dev/null
  if [ $? -ne 0 ]; then
    echo -e "\033[1;31mERROR! cross-compiler utilities missing\033[m"
    read -p "Do you want to install them (root password is needed)? " -n 1 -r
    echo
    if [[ $REPLY =~ ^[Yy]$ ]]; then
      sudo apt-get install -y gcc-multilib g++-multilib
      sudo apt-get install -y libc6-armel-cross libc6-dev-armel-cross binutils-arm-linux-gnueabi
      sudo apt-get install -y libncurses5-dev gcc-arm-linux-gnueabi g++-arm-linux-gnueabi
    fi
  fi
}

# Check command arguments
if [ $# -gt 0 ]; then
  if [ $1 == "--help" ]; then
    Usage
    exit 0
  elif [ $1 == "--clean" ]; then
    rm -rf $BuildDir
    rm -rf $InstallDir
    rm -rf $EsanHomeDir/.status
    exit 0
  elif [ $1 == "--new" ]; then
    rm -rf $BuildDir
    rm -rf $InstallDir
    rm -rf $EsanHomeDir/.status
  fi
fi

# Check if relevant downloads are available
if [ ! -f "./.status" ]; then

  mkdir -p $BuildDir
  mkdir -p $SrcDir
  mkdir -p $InstallDir


# Step 0:
#   Check for prerequisites and install necessary packages
  CheckPrerequisites

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
  reportIfSuccessful
  touch "$EsanHomeDir/.status" # Log success status

fi # end of downloads

# Step 2:
#   Copy EmbedSanitizer files to ./build/llvm
cp -r $EsanHomeDir/llvm/* $SrcDir/

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
cd $EsanHomeDir/etsan
./install.sh

reportIfSuccessful;
echo 
echo -e "\033[1;32m EmbedSanitizer is installed in ${InstallDir}\033[m"
