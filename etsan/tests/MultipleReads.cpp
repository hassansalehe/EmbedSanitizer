/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2018, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// This sample code four threads execute the race detection
// callback "__tsan_read4" to check if it works correctly.
//
////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include "tsan_interface.h"

void * threadFunction(void * data) {
  __tsan_read4((void *) 0x03);
  return NULL;
}

int main() {

  pthread_t threads[4];
  int data[4];

  for (int i = 0; i < 4; i++) {
     data[i] = i+1;
     pthread_create( &threads[i], NULL, threadFunction, &data[i]);
  }

  // wait for threads to terminate
  for (int i = 0; i < 4; i++)
    pthread_join( threads[i], NULL );

  return 0;
}
