/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
// This sample code create 4 threads and each thread
// increments as shared varable "balance". A functinal
// race detection tool should not report any race from
// this example.
//
////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
int balance = 0;
void * threadFunction(void * data) {
  pthread_mutex_lock(&lock);
  balance++;
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {

  pthread_t threads[4];
  int data[4];

  printf("== main creating threads in linked library ==\n");
  for(int i = 0; i < 4; i++) {
     data[i] = i+1;
     pthread_create( &threads[i], NULL, threadFunction, &data[i]);
  }

  // wait for threads to terminate
  for(int i=0; i < 4; i++)
    pthread_join( threads[i], NULL );

  printf( "New balance %d\n", balance );

  return 0;
}
