/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for tsan_interface virtual pointer related functions.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>

#include "etsan/tsan_interface.h"

void** addr = (void**)(0x03);
void *new_val = (void*)(0x001);
int line_num = 123;
char * func_name = "some_function";
char * file_name = "some_file";

void threadFunction() {

  usleep(400);
  __tsan_vptr_read(addr, line_num, func_name, file_name);
  __tsan_vptr_update(addr, new_val, line_num, func_name, file_name);
}

constexpr int NUM_THREADS = 4;

TEST(TsanInterfaceTestFixture, CheckTsanRreadWriteVptrWithConcurrencyAndRace) {
  std::vector<std::thread> threads;

  // redirect cout to a stream to capture output string
  std::stringstream input_capture;
  auto cout_read_buffer = std::cout.rdbuf();
  std::cout.rdbuf(input_capture.rdbuf());

  // create threads
  for (int i = 0; i < NUM_THREADS; i++) {
    threads.push_back(std::thread(threadFunction));
    usleep(200);

    auto child_id = threads[i].get_id();
    __tsan_thread_create((void*)(&child_id));
  }

  // wait for threads to terminate
  for (auto& thread : threads) {
    auto child_id = thread.get_id();
    thread.join();
    __tsan_thread_join((void*)(&child_id));
  }

  __tsan_main_func_exit();

  // assertions
  std::string file_report = std::string("A race detected at: ") + file_name;
  std::string line_no_report = std::string("At line number: ") + std::to_string(line_num);
  std::string read_func_report = std::string("read \"") + func_name + "\"";
  std::string write_func_report = std::string("write \"") + func_name + "\"";

  EXPECT_NE(std::string::npos, input_capture.str().find(file_report));
  EXPECT_NE(std::string::npos, input_capture.str().find(line_no_report));
  EXPECT_TRUE((std::string::npos != input_capture.str().find(read_func_report)) ||
              (std::string::npos != input_capture.str().find(write_func_report)));

  // return back std::cout buffer
  std::cout.rdbuf(cout_read_buffer);
  std::cout << input_capture.str() << std::endl;
}
