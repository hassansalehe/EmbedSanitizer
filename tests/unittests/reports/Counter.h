#ifndef REPORTS_COUNTER_H_
#define REPORTS_COUNTER_H_

int counter = 0;

namespace reader {

void machine_reader() {
  counter++;
}
};

void incrementCounter() {
  reader::machine_reader();
}

#endif
