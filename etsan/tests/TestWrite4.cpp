/////////////////////////////////////////////////////
//
// Copyright (c) 2017, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// This sample code executes the race detection
// callback "__tsan_write4" to check if it works correctly.
//
////////////////////////////////////////////////////


#include <iostream>
#include "tsan_interface.h"

using namespace std;

int main() {
  __tsan_write4((void*)0x03);
  return 0;
}
