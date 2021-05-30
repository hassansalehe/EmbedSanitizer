/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2018, Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for tsan_interface.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <memory>

#include "etsan/tsan_interface.h"

TEST(TsanInterfaceTest, DummyTest) {
  __tsan_init();
  ASSERT_TRUE(true);
}

TEST(TsanInterfaceTest, CheckTsanRread4Works) {
   __tsan_read4((void*)0x03, 123, nullptr, nullptr);
  ASSERT_TRUE(true);
}

TEST(TsanInterfaceTest, CheckTsanWrite4Works) {
   __tsan_write4((void*)0x03, 123, nullptr, nullptr);
  ASSERT_TRUE(true);
}
