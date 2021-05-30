#include <gtest/gtest.h>

#include <memory>

#include "etsan/tsan_interface.h"

TEST(TsanInterfaceTest, DummyTest) {
  __tsan_init();
  ASSERT_TRUE(true);
}
