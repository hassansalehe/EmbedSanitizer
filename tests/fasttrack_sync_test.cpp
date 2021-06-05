/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for syncronization functions of fasttrack.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include "etsan/fasttrack.h"

TEST(FasttrackSyncTestFixture, ftAcquire) {

  ThreadState thread_state;
  thread_state.C = {0 << 24, (1 << 24) + 1, (2 << 24) + 2};
  thread_state.tid = 3;

  LockState lock_state;
  lock_state.L = {0, 0, 0, (3 << 24) + 3, (4 << 24) + 4};

  const auto max_num_threads = std::max(thread_state.C.size(), lock_state.L.size());

  ft_acquire(thread_state, lock_state);

  EXPECT_EQ(max_num_threads, thread_state.C.size());
  EXPECT_EQ(max_num_threads, lock_state.L.size());
  EXPECT_EQ(thread_state.epoch, thread_state.C[thread_state.tid]);

  for (int t = 0; t < thread_state.C.size(); ++t) {
    EXPECT_EQ((t << 24) + t, thread_state.C[t]);
  }
}

TEST(FasttrackSyncTestFixture, ftRelease) {

  ThreadState thread_state;
  thread_state.C = {0 << 24, (1 << 24) + 1, (2 << 24) + 2};
  thread_state.tid = 3;

  LockState lock_state;
  lock_state.L = {0, 0, 0, (3 << 24) + 3, (4 << 24) + 4};

  const auto max_num_threads = std::max(thread_state.C.size(), lock_state.L.size());

  ft_release(thread_state, lock_state);

  EXPECT_EQ(max_num_threads, thread_state.C.size());
  EXPECT_EQ(max_num_threads, lock_state.L.size());
  EXPECT_EQ(thread_state.epoch, thread_state.C[thread_state.tid]);

  for (int t = 0; t < thread_state.C.size(); ++t) {
    if (t != thread_state.tid)
    {
      EXPECT_EQ(lock_state.L[t], thread_state.C[t]);
    } else {
      // thread's clock is incremented
      EXPECT_EQ((CLOCK(3 << 24) + 1), CLOCK(thread_state.C[t]));
      EXPECT_EQ(lock_state.L[t] + 1, thread_state.C[t]);
    }
  }
}