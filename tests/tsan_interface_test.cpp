/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
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

constexpr int NUM_THREADS = 4;

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

  std::unordered_map<int, func_t> unaligned_read_functions = {
    {2, __tsan_unaligned_read2},
    {4, __tsan_unaligned_read4},
    {8, __tsan_unaligned_read8},
    {16, __tsan_unaligned_read16},
  };

  std::unordered_map<int, func_t> unaligned_write_functions = {
    {2, __tsan_unaligned_write2},
    {4, __tsan_unaligned_write4},
    {8, __tsan_unaligned_write8},
    {16, __tsan_unaligned_write16},
  };

  void simulate_parallel_threads_exections(func_t& read_func, func_t& write_func, void* addr, int line_num) {
    std::vector<std::thread> threads;
    std::array<data_t, NUM_THREADS> data;

    // redirect cout to a stream to capture output string
    std::stringstream input_capture;
    auto cout_read_buffer = std::cout.rdbuf();
    std::cout.rdbuf(input_capture.rdbuf());

    // create threads
    for (int i = 0; i < NUM_THREADS; i++) {
      data[i].ft_read_func = read_func;
      data[i].ft_write_func = write_func;
      data[i].addr = addr;
      data[i].line_no = line_num;

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

    __tsan_main_func_exit();

    // assertions
    std::string file_report = std::string("A race detected at: ") + data.front().file_name;
    std::string line_no_report = std::string("At line number: ") + std::to_string(line_num);
    std::string read_func_report = std::string("read \"") + data.front().func_name + "\"";
    std::string write_func_report = std::string("write \"") + data.front().func_name + "\"";

    EXPECT_NE(std::string::npos, input_capture.str().find(file_report));
    EXPECT_NE(std::string::npos, input_capture.str().find(line_no_report));
    EXPECT_TRUE((std::string::npos != input_capture.str().find(read_func_report)) ||
                (std::string::npos != input_capture.str().find(write_func_report)));

    // return back std::cout buffer
    std::cout.rdbuf(cout_read_buffer);
    std::cout << input_capture.str() << std::endl;
  }
};

TEST(TsanInterfaceTestFixture, CallingTsanInit) {
  __tsan_init();
  ASSERT_TRUE(true);
}

TEST_P(TsanInterfaceTestFixture, CheckTsanRreadWithConcurrencyAndRace) {
  int func_id = GetParam();
  void* addr = (void*)(0x03 + func_id);
  int line_num = 123 + func_id;
  simulate_parallel_threads_exections(read_functions[func_id],
                                      write_functions[func_id],
                                      addr,
                                      line_num);
}

TEST_P(TsanInterfaceTestFixture, CheckTsanUNALIGNEDreadWithConcurrencyAndRace) {
  int func_id = GetParam();
  void* addr = (void*)(0x014 + func_id);
  int line_num = 163 + func_id;
  if (unaligned_read_functions.count(func_id) > 0) {
    simulate_parallel_threads_exections(unaligned_read_functions[func_id],
                                        unaligned_write_functions[func_id],
                                        addr,
                                        line_num);
  }
}

INSTANTIATE_TEST_SUITE_P(ParamTest,
                         TsanInterfaceTestFixture,
                         ::testing::ValuesIn({1, 2, 4, 8, 16}),
                         testing::PrintToStringParamName());
