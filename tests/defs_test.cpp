/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for defs.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <thread>

#include "etsan/defs.h"

class DefsTestFixture : public ::testing::Test {
protected:

  DefsTestFixture() {}

  ~DefsTestFixture() {}
};

TEST_F(DefsTestFixture, checkThreadState) {
  ThreadState st;
  st.tid = 1;
  st.epoch = 0;
  st.C.reserve(st.tid +1);

  st.increment();
  EXPECT_EQ(1, st.epoch);
  EXPECT_EQ(1, st.C[st.tid]);

  st.C[st.tid] = 42;
  st.updateEpoch();
  ASSERT_EQ(st.C[st.tid], st.epoch); 
}

TEST_F(DefsTestFixture, checkUpdateThreadClocks) {
}

// VectorClock related tests
TEST_F(DefsTestFixture, checkCreationOfNewVectorClock) {
  constexpr int size = 5;
  VectorClock VC;
  newVectorClock(VC, size);

  EXPECT_EQ(size, VC.size());
  for (int t = 0; t < size; t++) {
    EXPECT_EQ(0, (VC.at(t) - (t << 24)));
  }
}

TEST_F(DefsTestFixture, checkExtendVectorClock) {
  int size = 5;
  VectorClock VC;
  newVectorClock(VC, size);

  EXPECT_EQ(size, VC.size());

  size *= 2;
  ExtendVectorClock(VC, size);

  for (int t = 0; t < size; t++) {
    EXPECT_EQ(0, (VC.at(t) - (t << 24)));
  }
}

TEST_F(DefsTestFixture, checkExtend2VectorClocks) {
  constexpr int size1 = 5;
  VectorClock VC1;
  newVectorClock(VC1, size1);

  constexpr int size2 = 4;
  VectorClock VC2;
  newVectorClock(VC2, size2);

  ExtendVectorClocks(VC2, VC1);
  auto max_size = std::max(size1, size2);
  EXPECT_EQ(max_size, VC1.size());
  EXPECT_EQ(max_size, VC2.size());

  for (int t = size1; t < max_size; t++) {
    EXPECT_EQ(0, (VC1.at(t) - (t << 24)));
  }

  for (int t = size2; t < max_size; t++) {
    EXPECT_EQ(0, (VC2.at(t) - (t << 24)));
  }
}