/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
// Test program for instrumenting pthread synchronization
// functions.
//
////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>

pthread_mutex_t lock;
int balance = 0;
void * threadFunction(void * data) {
  pthread_mutex_lock(&lock);
  printf("Child:  %u\n", (unsigned int)pthread_self());
  balance++;
  pthread_mutex_unlock(&lock);
  return NULL;
}

int main() {
#define N 4
  pthread_t threads[N];
  int data[N];

  threads[0] = pthread_t(42);
  pthread_mutex_init(&lock, NULL);
  printf("P:%u\n", (unsigned int)pthread_self());
  printf("The lock :%p\n", &lock);


  printf("== Hassan creating threads in linked library ==\n");
  for(int i = 0; i < N; i++) {
     data[i] = i+1;
     pthread_create( &threads[i], NULL, threadFunction, &data[i]);
  }

   for(int i = 0; i < N; i++) {
     printf("Parent: %u\n", (unsigned int)threads[i]);
  }

  // wait for threads to terminate
  for(int i=0; i < N; i++) {
    pthread_join( threads[i], NULL );
    printf("Parent: %p %u\n", (void*)threads[i], (unsigned int)threads[i]);
  }

  printf( "New balance %d\n", balance );

  return 0;
}
