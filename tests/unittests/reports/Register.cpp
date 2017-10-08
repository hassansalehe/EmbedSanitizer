#include <iostream>
#include <map>
#include "Bridge.h"

extern "C" {

std::map<int, int> stats;

void addStats(int tid) {
  stats[tid]++;
}


}
