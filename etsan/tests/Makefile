#########################################################
#
# Copyright (c) 2017, Hassan Salehe Matar
#
# See LICENSE file for information about the license.
#
# This make file build the sample test files for
# checking different aspects of our implementation of
# FastTrack race detection algorithm.
#
#########################################################
FLGS=-std=c++11 -pthread -g -fpermissive -I../

all:  test_write test_read MultipleReads LockAcquire
	g++ LockRelease.cpp -o LockRelease.exe $(FLGS)

LockAcquire:
	g++ LockAcquire.cpp -o LockAcquire.exe $(FLGS)

test_write:
	g++ TestWrite4.cpp ../tsan_interface.cc -o test_write4.exe $(FLGS)

MultipleReads:
	g++ MultipleReads.cpp ../tsan_interface.cc -o MultipleReads.exe $(FLGS)
	g++ ReadWrites.cpp ../tsan_interface.cc -o ReadWrites.exe $(FLGS)


test_read:
	g++ TestRead4.cpp ../tsan_interface.cc -o test_read4.exe -I../ $(FLGS)

clean:
	rm -rf *.o *.exe
