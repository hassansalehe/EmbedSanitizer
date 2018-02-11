/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2018, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// This sample code executes the race detection
// callback "__tsan_read4" to check if it works correctly.
//
////////////////////////////////////////////////////

#include <iostream>
#include "tsan_interface.h"

int main() {
  __tsan_read4((void*)0x03);
  return 0;
}
