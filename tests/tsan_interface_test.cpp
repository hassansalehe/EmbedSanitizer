/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for tsan_interface.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <functional>
#include <memory>
#include <vector>
#include <unordered_map>
#include <thread>

#include "etsan/tsan_interface.h"

using func_t = std::function<void(void*, int, void*, void*)>;

// function-related data definitions
struct data_t
{
  func_t ft_read_func;
  func_t ft_write_func;
  void* addr;
  int line_no;

  char * func_name = "some_function";
  char * file_name = "some_file";
};

void threadFunction(data_t & data) {
  usleep(400);
  data.ft_read_func(data.addr, data.line_no, data.func_name, data.file_name);
  data.ft_write_func(data.addr, data.line_no, data.func_name, data.file_name);
}

class TsanInterfaceTestFixture :public ::testing::TestWithParam< int > {
protected:
  std::unordered_map<int, func_t> read_functions = {
    {1, __tsan_read1},
    {2, __tsan_read2},
    {4, __tsan_read4},
    {8, __tsan_read8},
    {16, __tsan_read16},
  };

  std::unordered_map<int, func_t> write_functions = {
    {1, __tsan_write1},
    {2, __tsan_write2},
    {4, __tsan_write4},
    {8, __tsan_write8},
    {16, __tsan_write16},
  };

  const int NUM_THREADS = 4;
};

TEST(TsanInterfaceTestFixture, CallingTsanInit) {
  __tsan_init();
  ASSERT_TRUE(true);
}

TEST_P(TsanInterfaceTestFixture, CheckTsanRreadWithConcurrencyAndRace) {

  int func_id = GetParam();

  std::vector<std::thread> threads;
  data_t data[NUM_THREADS];

  // create threads
  for (int i = 0; i < NUM_THREADS; i++) {
    data[i].ft_read_func = read_functions[func_id];
    data[i].ft_write_func = write_functions[func_id];
    data[i].addr = (void*)(0x03 + func_id);
    data[i].line_no = 123 + func_id;

    threads.push_back(std::thread(threadFunction, std::ref(data[i])));
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
}

INSTANTIATE_TEST_SUITE_P(AnotherInstantiationName,
                         TsanInterfaceTestFixture,
                         ::testing::ValuesIn({1, 2, 4, 8, 16}),
                         testing::PrintToStringParamName());
