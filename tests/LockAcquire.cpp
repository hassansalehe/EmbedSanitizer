/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2018, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// This code checks of FastTrack's vector clocks are
// updated appropriately.
//
////////////////////////////////////////////////////

#include <stdio.h>
#include <pthread.h>
#include <iostream>
#include <stdlib.h>
#include "etsan/fasttrack.h"

int main() {

  time_t tm;
  srand((unsigned) time(&tm));

  ThreadState t;
  t.tid = 5;
  for (int i = 0; i < 6; i++) {
    t.C.push_back((i << 24) + (rand() % 10));
  }

  LockState Lm;
  for (int i = 0; i < 10; i++) {
    Lm.L.push_back((i << 24) + (rand() % 11));
  }

  std::cout << "BEFORE:\nLm.L: <";
  for (int i = 0; i < 10; i++) {
    std::cout <<  (Lm.L[i] & 0xFFFFFF)<< "@" <<  (Lm.L[i] >> 24)  << ",";
  }
  std::cout << ">" << std::endl;

  std::cout << "t.C:  <";
  for (int i = 0; i < 6; i++) {
    std::cout << (t.C[i] & 0xFFFFFF) << "@" << (t.C[i] >> 24) << ",";
  }
  std::cout << ">" << std::endl;

  ft_acquire(t, Lm);

  std::cout << "AFTER:\nLm.L: <";
  for (int i = 0; i < 10; i++) {
    std::cout <<  (Lm.L[i] & 0xFFFFFF)<< "@" <<  (Lm.L[i] >> 24)  << ",";
  }
  std::cout << ">" << std::endl;

  std::cout << "t.C:  <";
  for (int i = 0; i < 10; i++) {
    std::cout << (t.C[i] & 0xFFFFFF) << "@" << (t.C[i] >> 24) << ",";
  }
  std::cout << ">" << std::endl;

  return 0;
}
