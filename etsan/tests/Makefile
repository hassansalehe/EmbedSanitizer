#########################################################
#
# Copyright (c) 2017 - 2018, Hassan Salehe Matar
#
# See LICENSE file for information about the license.
#
# This make file build the sample test files for
# checking different aspects of our implementation of
# FastTrack race detection algorithm.
#
#########################################################
FLGS=-std=c++11 -pthread -g -fpermissive -I../

all:  LockAcquire
	g++ LockRelease.cpp -o LockRelease.exe $(FLGS)

LockAcquire:
	g++ LockAcquire.cpp -o LockAcquire.exe $(FLGS)

clean:
	rm -rf *.o *.exe
