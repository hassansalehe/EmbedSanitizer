#!/bin/bash

#########################################################################
#
# Copyright (c) 2017 - Hassan Salehe Matar
#
#  License: Follows License of LLVM/Clang. Read the licence file LICENSE.md
#
#
# This script runs the benchmark applications and reports the results.
# It runs experiments on ARMv7 platform or Qemu emulator should be installed.
#
#########################################################################

home=`pwd`

progress()
{
    local pid=$1
    local delay=0.75
    local spinstr='|/-\'
    while [ "$(ps a | awk '{print $1}' | grep $pid)" ]; do
        local temp=${spinstr#?}
        printf " [%c]  " "$spinstr"
        local spinstr=$temp${spinstr%"$temp"}
        sleep $delay
        printf "\b\b\b\b\b\b"
    done
    printf "    \b\b\b\b"
}

reportRaces() {
  races=`egrep -i "race.*[1-9]" stats.txt`

  if [ -z "$races" ]; then
    echo "$1 races?: no" >> $home/BenchmarkReports.txt
  else
    echo "$1 races?: yes" >> $home/BenchmarkReports.txt
  fi
}


reportBenchStats() {
  addresses=`egrep "Addresses: " stats.txt`
  echo " - Unique $addresses" >> $home/BenchmarkReports.txt

  reads=`egrep "Reads: " stats.txt`
  echo " - Total $reads" >> $home/BenchmarkReports.txt

  writes=`egrep "Writes: " stats.txt`
  echo " - Total $writes" >> $home/BenchmarkReports.txt

  locks=`egrep "Locks: " stats.txt`
  echo " - Unique $locks" >> $home/BenchmarkReports.txt

  threads=`egrep "Threads: " stats.txt`
  echo " - # of $threads" >> $home/BenchmarkReports.txt
}

input_set=""

runBenchmark() {
  benchmark=$1
  cd $home/${benchmark}

  echo "${benchmark}: " >> $home/BenchmarkReports.txt
  echo -n > stats.txt

  platform=""
  emulator=""

  # check if platform is arm
  platform=`uname -a | grep -o -m 1 -i armv7 | head -1`
  if [ -z "$platform" ]; then
    emulator="qemu-arm"
    platform="QEMU"

    make
    ./${benchmark}_x64_instrumented.exe $input_set 2>&1 1>stats.txt | tee -a  stats.txt
    reportRaces " - ThreadSanitizer";
  else
    emulator=""
    platform="ARMv7"

  fi

  echo "" > out.txt
  echo -n "Running ${benchmark} in ${platform}"
  (/usr/bin/time -f "%e" -o time.txt ${emulator} ./${benchmark}_arm.exe $input_set > stats.txt) &
  progress $!
  echo ""
  qemu_orig_time=`head -1 time.txt`
  echo "" > out.txt

  echo -n "Race detection for ${benchmark} in ${platform}"
  (/usr/bin/time -f "%e" -o time.txt ${emulator} ./${benchmark}_arm_instrumented.exe $input_set > stats.txt) &
  progress $!
  echo ""
  qemu_detect_time=`head -1 time.txt`

  reportRaces " - EmbedSanitizer in ${platform}";

  slowdown=`calc $qemu_detect_time/$qemu_orig_time`
  echo " - ${platform} slowdown: ${slowdown}" >> $home/BenchmarkReports.txt

  reportBenchStats;
  echo ""
}

echo -n > $home/BenchmarkReports.txt

# For blackscholes
input_set="2 ./input/simsmall_4K.txt out.txt"
runBenchmark "blackscholes"

# For fluidanimate
input_set="2 2 input/in_5K.fluid  out.txt"
runBenchmark "fluidanimate"


# For streamcluster
input_set="5 10 16 256 256 125 none output.txt 2"
runBenchmark "streamcluster"

# For swaptions
input_set="-ns 16 -sm 400 -nt 2"
runBenchmark "swaptions"

echo "Results saved in BenchmarkReports.txt"
