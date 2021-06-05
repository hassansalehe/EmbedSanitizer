/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for read function of fasttrack.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include "etsan/fasttrack.h"

TEST(FasttrackReadTestFixture, ftReadCheckReadsCountIncremented) {
  VS.reads = 0;
  constexpr bool no_race_found = false;

  VarState variable_state;
  variable_state.Racy = true;
  ThreadState thread_state;

  EXPECT_EQ(no_race_found, ft_read(variable_state, thread_state));
  EXPECT_EQ(1, VS.reads);
  EXPECT_EQ(0, VS.writes);
}

TEST(FasttrackReadTestFixture, ftReadCheckNoRaceSameEpoch) {
  VS.reads = 0;
  constexpr bool no_race_found = false;

  VarState variable_state;
  variable_state.Racy = false;
  variable_state.R = 1;

  ThreadState thread_state;
  thread_state.C.reserve(1);
  thread_state.tid = 0;
  thread_state.epoch = variable_state.R;

  EXPECT_EQ(no_race_found, ft_read(variable_state, thread_state));
  EXPECT_EQ(1, VS.reads);
  EXPECT_EQ(0, VS.writes);
}

TEST(FasttrackReadTestFixture, ftReadDetectNewRace) {
  VS.reads = 0;
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

  // set different epochs
  variable_state.R = 0;
  thread_state.epoch = ~variable_state.R ;

  EXPECT_EQ(race_found, ft_read(variable_state, thread_state));
  EXPECT_EQ(1, VS.reads);
  EXPECT_EQ(0, VS.writes);
}

TEST(FasttrackReadTestFixture, ftReadCheckUpdateOnSharedRead) {
  VS.reads = 0;
  constexpr bool race_found = true;
  constexpr unsigned int num_threads = 5;
  constexpr int tid = 3;

  VarState variable_state;
  variable_state.R = READ_SHARED;
  variable_state.Rvc.reserve(num_threads);

  ThreadState thread_state;
  thread_state.tid = tid;
  thread_state.epoch = 42;

  thread_state.C.reserve(num_threads);

  ft_read(variable_state, thread_state);
  EXPECT_EQ(thread_state.epoch, variable_state.Rvc[tid]);
}

TEST(FasttrackReadTestFixture, ftReadCheckExclusive) {
  VS.reads = 0;
  constexpr bool race_found = true;
  constexpr unsigned int num_threads = 5;
  constexpr int tid = 3;
  constexpr int tid2 = 1;

  VarState variable_state;
  variable_state.R = (tid2 << 24) + 0;

  ThreadState thread_state;
  thread_state.tid = tid;
  thread_state.epoch = 42;

  thread_state.C.reserve(num_threads);
  thread_state.C[tid2] = variable_state.R;

  ft_read(variable_state, thread_state);
  EXPECT_EQ(thread_state.epoch, variable_state.R);
}

