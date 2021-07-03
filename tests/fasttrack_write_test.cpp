/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
// Unit tests for write function of fasttrack.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include "etsan/fasttrack.h"

TEST(FasttrackWriteTestFixture, ftWriteCheckWritesCountIncremented) {
  VS.reads = 0;
  VS.writes = 0;
  constexpr bool no_race_found = false;

  VarState variable_state;
  variable_state.Racy = true;
  ThreadState thread_state;

  EXPECT_EQ(no_race_found, ft_write(variable_state, thread_state));
  EXPECT_EQ(0, VS.reads);
  EXPECT_EQ(1, VS.writes);
}

TEST(FasttrackWriteTestFixture, ftWriteCheckNoRaceSameEpoch) {
  VS.reads = 0;
  VS.writes = 0;
  constexpr bool no_race_found = false;

  VarState variable_state;
  variable_state.Racy = false;
  variable_state.W = 1;

  ThreadState thread_state;
  thread_state.C.reserve(1);
  thread_state.tid = 0;
  thread_state.epoch = variable_state.W;

  EXPECT_EQ(no_race_found, ft_write(variable_state, thread_state));
  EXPECT_EQ(1, VS.writes);
  EXPECT_EQ(0, VS.reads);
}

TEST(FasttrackWriteTestFixture, ftWriteDetectNewRace) {
  VS.reads = 0;
  VS.writes = 0;
  constexpr bool race_found = true;
  constexpr unsigned int num_threads = 5;
  constexpr int tid1 = 3;
  constexpr int tid2 = 1;

  VarState variable_state;
  variable_state.W = (tid2 << 24) + 124;

  ThreadState thread_state;
  thread_state.tid = tid1;
  NumThreads = num_threads;

  thread_state.C.reserve(num_threads);
  thread_state.C[tid2] = 123;

  EXPECT_EQ(race_found, ft_write(variable_state, thread_state));
  EXPECT_EQ(0, VS.reads);
  EXPECT_EQ(1, VS.writes);
  // EXPECT_EQ(num_threads, variable_state.Rvc.size());
}

TEST(FasttrackWriteTestFixture, ftWriteCheckUpdateOnSharedRead) {
  VS.reads = 0;
  constexpr unsigned int num_threads = 5;
  constexpr int tid = 3;

  VarState variable_state;
  variable_state.R = READ_SHARED;
  variable_state.Rvc.reserve(num_threads);

  ThreadState thread_state;
  thread_state.tid = tid;
  thread_state.epoch = 42;

  thread_state.C.reserve(num_threads);

  ft_write(variable_state, thread_state);
  EXPECT_EQ(thread_state.epoch, variable_state.W);
}

TEST(FasttrackWriteTestFixture, ftWriteCheckExclusive) {
  VS.reads = 0;
  constexpr bool race_found = true;
  constexpr unsigned int num_threads = 5;
  constexpr int tid = 3;
  constexpr int tid2 = 1;

  VarState variable_state;
  variable_state.R = (tid2 << 24) + 0;
  variable_state.W = (tid2 << 24) + 1;

  ThreadState thread_state;
  thread_state.tid = tid;
  thread_state.epoch = 42;

  thread_state.C.reserve(num_threads);
  thread_state.C[tid2] = variable_state.W - 1; //(x.W > C[tid2])

  EXPECT_EQ(race_found, ft_write(variable_state, thread_state));
  EXPECT_EQ(thread_state.epoch, variable_state.W);
}
