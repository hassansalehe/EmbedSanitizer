/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for raceReport.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <thread>

#include "etsan/race_report.h"

class RaceReportTestFixture : public ::testing::Test {
protected:
  // arbitrary field values for testing
  const char *func_name1 = "function_1";
  const char *func_name2 = "function_2";
  const char *obj_name = "DummyObj";
  const char *file_name = "some_file.cpp";
  const char *read_access_type = "read";
  const char *write_access_type = "write";
  int line_number = 42;

  RaceReportTestFixture() { etsan::callStack.clear(); }

  ~RaceReportTestFixture() {}
};

TEST_F(RaceReportTestFixture, pushFunction) {
  etsan::pushFunction(func_name1);

  // const auto tid = std::this_thread::get_id();
  const auto tid = static_cast<unsigned int>(pthread_self());
  const auto c_back = etsan::callStack.find(tid);

  ASSERT_NE(etsan::callStack.end(), c_back);
  ASSERT_EQ(1U, c_back->second.size());
  ASSERT_EQ(func_name1, c_back->second.at(0));

  etsan::pushFunction(func_name2);
  ASSERT_EQ(2U, c_back->second.size());
  ASSERT_EQ(func_name2, c_back->second.at(1));
}

TEST_F(RaceReportTestFixture, popFunction) {
  etsan::pushFunction(func_name1);
  etsan::pushFunction(func_name2);

  const auto tid = static_cast<unsigned int>(pthread_self());
  const auto c_back = etsan::callStack.find(tid);
  EXPECT_EQ(2U, c_back->second.size());

  // poping inner function fails
  etsan::popFunction(func_name1);
  EXPECT_EQ(2U, c_back->second.size());

  // popping the top function works
  etsan::popFunction(func_name2);
  EXPECT_EQ(1U, c_back->second.size());
  ASSERT_EQ(func_name1, c_back->second.at(0));
}

TEST_F(RaceReportTestFixture, getStack) {
  // initially no entry for a thread before calling getStack
  const auto tid = static_cast<unsigned int>(pthread_self());
  const auto c_back = etsan::callStack.find(tid);
  ASSERT_EQ(etsan::callStack.end(), c_back);

  // getStack puts an empty entry to callStack
  auto stack = etsan::getStack(tid);
  EXPECT_EQ(0U, stack.size());
  ASSERT_NE(etsan::callStack.end(), etsan::callStack.find(tid));

  // put one function and get stack
  etsan::pushFunction(func_name1);
  stack = etsan::getStack(tid);
  EXPECT_EQ(1U, stack.size());
  ASSERT_EQ(func_name1, stack.at(0));
}

TEST_F(RaceReportTestFixture, printStack) {
  etsan::pushFunction(func_name1);
  etsan::pushFunction(func_name2);

  const auto stack_str = etsan::printStack();

  ASSERT_NE(std::string::npos, stack_str.find(func_name1));
  ASSERT_NE(std::string::npos, stack_str.find(func_name2));
}

TEST_F(RaceReportTestFixture, reportRaceRead) {
  // create a call stack of 2 functions
  etsan::pushFunction(func_name1);
  etsan::pushFunction(func_name2);

  // redirect cout to a stream to capture output string
  std::stringstream input_capture;
  auto cout_read_buffer = std::cout.rdbuf();
  std::cout.rdbuf(input_capture.rdbuf());

  etsan::reportRaceRead(line_number, obj_name, file_name);

  EXPECT_NE(std::string::npos, input_capture.str().find(func_name1));
  EXPECT_NE(std::string::npos, input_capture.str().find(func_name2));
  EXPECT_NE(std::string::npos, input_capture.str().find(obj_name));
  EXPECT_NE(std::string::npos, input_capture.str().find(file_name));
  EXPECT_NE(std::string::npos, input_capture.str().find(read_access_type));

  // return back std::cout buffer
  std::cout.rdbuf(cout_read_buffer);
}

TEST_F(RaceReportTestFixture, reportRaceWrite) {

  // create a call stack of 2 functions
  etsan::pushFunction(func_name1);
  etsan::pushFunction(func_name2);

  // redirect cout to a stream to capture output string
  std::stringstream input_capture;
  auto cout_read_buffer = std::cout.rdbuf();
  std::cout.rdbuf(input_capture.rdbuf());

  etsan::reportRaceWrite(line_number, obj_name, file_name);

  EXPECT_NE(std::string::npos, input_capture.str().find(func_name1));
  EXPECT_NE(std::string::npos, input_capture.str().find(func_name2));
  EXPECT_NE(std::string::npos, input_capture.str().find(obj_name));
  EXPECT_NE(std::string::npos, input_capture.str().find(file_name));
  EXPECT_NE(std::string::npos, input_capture.str().find(write_access_type));

  // return back std::cout buffer
  std::cout.rdbuf(cout_read_buffer);
}
