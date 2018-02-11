/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2018, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// This sample code executes the race detection
// callbacks "__tsan_write4" and "__tsan_read4"
// by different threads to check if they work correctly.
//
////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include "tsan_interface.h"

void * threadFunction(void * data) {
  __tsan_write4((void *) 0x03);
  return NULL;
}

void * threadFunction2(void * data) {
  __tsan_read4((void *) 0x03);
  return NULL;
}

int main() {

  pthread_t threads[4];
  int data[4];
  int i = 0;

  data[i] = i+1;
  pthread_create( &threads[i], NULL, threadFunction, &data[i] );
  pthread_join( threads[i], NULL );

  __tsan_write4((void *) 0x03);

  i++;
  data[i] = i+1;
  pthread_create( &threads[i], NULL, threadFunction2, &data[i]);
  pthread_join( threads[i], NULL );

  return 0;
}
