#ifndef __RACE_REPORT__H_
#define __RACE_REPORT__H_

#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <sstream>
#include <stack>
#include <thread>
#include <unordered_map>
#include <algorithm>
#include <set>
#include "race.h"

using namespace std;

/**
 * Namespace which contains utility functions for manipulating data
 * race reporting metadata.
 */
namespace etsan {

static mutex racePrintLock;

static mutex stackFrameLock;

// Temporary location for Race reporting metadata:
static unordered_map<unsigned int, vector<char*>> callStack;

// Keeps list of races
static set<Race, race_compare> races;

// for printing all races in the list "races"
void printRaces() {

  string msg;

  for(auto & race : races) {
    race.createRaceMessage(msg);
  }

  racePrintLock.lock();
  cout << msg; // print to standard output
  racePrintLock.unlock();
}

/**
 * Pushes a function name to a call stack of a thread
 */
void pushFunction(char* funcName) {

  unsigned int tid = (unsigned int)pthread_self();

  stackFrameLock.lock();
  callStack[tid].push_back(funcName);
  //printf("EmbedSanitizer: function entry: %s\n", funcName);
  stackFrameLock.unlock();
}

void popFunction(char * funcName) {

  unsigned int tid = (unsigned int)pthread_self();

  stackFrameLock.lock();
  vector<char*> & stackFrame = callStack[tid];
  stackFrameLock.unlock();

  if(stackFrame.size() && stackFrame.back() == funcName) {
    stackFrame.pop_back();
  }
  else
    cout << "Something wrong with Function Stack: " << funcName << "\n";
}

vector<char*> getStack(unsigned int tid) {

    stackFrameLock.lock();
    vector<char*> & stackFrame = callStack[tid];
    stackFrameLock.unlock();

    return stackFrame;
}

/**
 * Prints the call stack of a thread when a race is found
 */
string printStack() {

  unsigned int tid = (unsigned int)pthread_self();
  stringstream ss;

  stackFrameLock.lock();
  vector<char*> & stackFrame = callStack[tid];
  stackFrameLock.unlock();

  int depth = 1;
  for( char * func : stackFrame) {
    string msg(depth, ' ');

    depth += 4;
    ss << msg << " '--->" << func << "(...)" << endl;
  }

  return ss.str();
}

/**
 * Report data race on a read operation. */
void reportRaceRead(int lineNo, void * objName, void * fileName) {

  unsigned int tid = (unsigned int)pthread_self();
  Race race(tid, lineNo, "read", (char*)objName, (char *)fileName);
  race.trace = getStack(tid);

  races.insert(race);
}

/**
 * Report data race on a write operation. */
void reportRaceWrite(int lineNo, void * objName, void * fileName) {

  unsigned int tid = (unsigned int)pthread_self();
  Race race(tid, lineNo, "write", (char*)objName, (char *)fileName);
  race.trace = getStack(tid);

  races.insert(race);
}

}  // etsan
#endif
