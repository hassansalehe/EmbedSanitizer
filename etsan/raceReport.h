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

using namespace std;

namespace etsan {

static mutex racePrintLock;

static mutex stackFrameLock;

// Temporary location for Race reporting metadata:
static unordered_map<unsigned int, vector<char*>> callStack;

/**
 * Pushes a function name to a call stack of a thread
 */
void pushFunction(char* funcName) {

  unsigned int tid = (unsigned int)pthread_self();

  stackFrameLock.lock();
  callStack[tid].push_back(funcName);
  //printf("EmbedSanitizer: function entry at %s\n", funcName);
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
 * Constructs a nicely reading message about the race
 * and stores the result in "msg" string. */
void createRaceMessage(string& msg,
                       string type,
                       int lineNo,
                       void * objName,
                       void * fileName
) {

  unsigned int tid = (unsigned int)pthread_self();
  stringstream ss;

  ss << "=============================================\n"  ;
  ss << "\033[1;32mEMBEDSANITIZER Race report\033[m\n"     ;
  ss << "\033[1;31m A race detected at: " << (char*)fileName << "\033[m\n";
  ss << "  At line number: "     << lineNo          << "\n";
  ss << "  Thread (tid=" << tid << ") " << type << " \""   ;
  ss << (char*)objName  << "\"\n"                          ;
  ss << "                                             \n"  ;
  ss << "\033[1;33m Call stack:   \033[m              \n"  ;
  ss << printStack();
  ss << "=============================================\n"  ;

  msg.append( ss.str() );
}

/**
 * Report data race on a read operation. */
void reportRaceRead(int lineNo, void * objName, void * fileName) {

  string msg;
  createRaceMessage(msg, "read", lineNo, objName, fileName);

  racePrintLock.lock();
  cout << msg; // print to standard output
  racePrintLock.unlock();
}

/**
 * Report data race on a write operation. */
void reportRaceWrite(int lineNo, void * objName, void * fileName) {

  string msg;
  createRaceMessage(msg, "write", lineNo, objName, fileName);

  racePrintLock.lock();
  cout << msg; // print to standard output
  racePrintLock.unlock();
}

}  // etsan
#endif