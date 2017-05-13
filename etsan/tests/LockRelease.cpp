/////////////////////////////////////////////////////
//
// Copyright (c) 2017, Hassan Salehe Matar
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
#include "fasttrack.h"

using namespace std;

int main() {

  time_t tm;
  srand((unsigned) time(&tm));

  ThreadState t;
  t.tid = 5;
  for(int i = 0; i < 6; i++) {
    t.C.push_back((i << 24) + (rand() % 10));
  }

  t.epoch = t.tid << 24 + 1;
  t.C[t.tid] = t.epoch;

  LockState Lm;
  for(int i = 0; i < 10; i++) {
    Lm.L.push_back((i << 24) + (rand() % 11));
  }

  cout << "BEFORE:\nLm.L: <";
  for(int i = 0; i < 10; i++) {
    cout <<  (Lm.L[i] & 0xFFFFFF)<< "@" <<  (Lm.L[i] >> 24)  << ",";
  }
  cout << ">" << endl;

  cout << "t.C:  <";
  for(int i = 0; i < 6; i++) {
    cout << (t.C[i] & 0xFFFFFF) << "@" << (t.C[i] >> 24) << ",";
  }
  cout << ">" << endl;

  ft_release(t, Lm);

  cout << "AFTER:\nLm.L: <";
  for(int i = 0; i < 10; i++) {
    cout <<  (Lm.L[i] & 0xFFFFFF)<< "@" <<  (Lm.L[i] >> 24)  << ",";
  }
  cout << ">" << endl;

  cout << "t.C:  <";
  for(int i = 0; i < 10; i++) {
    cout << (t.C[i] & 0xFFFFFF) << "@" << (t.C[i] >> 24) << ",";
  }
  cout << ">" << endl;

  return 0;
}
