#ifndef __RACE_REPORT__H_
#define __RACE_REPORT__H_

#include <thread>
#include <mutex>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

namespace etsan {


  static mutex printLock;
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
    ss << "EMBEDSANITIZER Race report\n"                     ;
    ss << "  A race detected at: " << (char*)fileName << "\n";
    ss << "  At line number: "     << lineNo          << "\n";
    ss << "  Thread (tid=" << tid << ") " << type << " \""   ;
    ss << (char*)objName  << "\"\n"                          ;
    ss << "=============================================\n"  ;

    msg.append( ss.str() );
  }

  /**
   * Report data race on a read operation. */
   void reportRaceRead(int lineNo, void * objName, void * fileName) {

     string msg;
     createRaceMessage(msg, "read", lineNo, objName, fileName);

     printLock.lock();
     cout << msg; // print to standard output
     printLock.unlock();
  }

  /**
   * Report data race on a write operation. */
  void reportRaceWrite(int lineNo, void * objName, void * fileName) {

     string msg;
     createRaceMessage(msg, "write", lineNo, objName, fileName);

     printLock.lock();
     cout << msg; // print to standard output
     printLock.unlock();
  }



}
#endif