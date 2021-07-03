/////////////////////////////////////////////////////
//
// Copyright (c) 2017 - 2021  Hassan Salehe Matar
//
// See LICENSE file for information about the license.
//
//
// Unit tests for race.h.
//
////////////////////////////////////////////////////

#include <gtest/gtest.h>

#include <memory>

#include "etsan/race.h"

class RaceTestFixture : public ::testing::Test {
 protected:

   std::shared_ptr<Race> race_obj_ptr;
   race_compare functor;

   RaceTestFixture() {
     unsigned int tid = 123;
     int lineNo = 1;
     std::string access_type("read");
     char * obj_name = "DummyClassObj";
     char * file_name = "some_file.cpp";

     race_obj_ptr = std::make_shared<Race>(
       Race(tid, lineNo, access_type, obj_name, file_name));
   }

  ~RaceTestFixture() override {

  }
};

TEST_F(RaceTestFixture, IsMessageCreatedIsSetToFalse) {
  ASSERT_FALSE(race_obj_ptr->isMessageCreated);
}

TEST_F(RaceTestFixture, CheckRaceMessageIsCreated) {
  std::string msg;

  // false --> created message is not printed
  ASSERT_FALSE(race_obj_ptr->createRaceMessage(msg));
  ASSERT_TRUE(race_obj_ptr->isMessageCreated);

  // true --> message was already created before
  ASSERT_TRUE(race_obj_ptr->createRaceMessage(msg));

  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->accessType));
  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->objName));
  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->fileName));
}

TEST_F(RaceTestFixture, CheckPrintStackWhenNoTraces) {
  auto msg = race_obj_ptr->printStack();

  ASSERT_TRUE(msg.empty());
  ASSERT_FALSE(race_obj_ptr->isMessageCreated);
}

TEST_F(RaceTestFixture, CheckPrintStackWithTraces) {
  char * rand_func_signature("void get_current_balance");

  race_obj_ptr->trace.push_back(rand_func_signature);
  auto msg = race_obj_ptr->printStack();

  ASSERT_NE(std::string::npos, msg.find(rand_func_signature));
  ASSERT_FALSE(race_obj_ptr->isMessageCreated);
}

TEST_F(RaceTestFixture, CheckComparisonEqualRaces) {
  const auto lhs = *race_obj_ptr;
  const auto rhs = *race_obj_ptr;

  ASSERT_FALSE(functor.operator()(lhs, rhs));
}

TEST_F(RaceTestFixture, CheckComparisonRacesLHSwithSmallerLineNum) {
  auto lhs = *race_obj_ptr;
  lhs.lineNo -= 1;
  const auto rhs = *race_obj_ptr;

  ASSERT_TRUE(functor.operator()(lhs, rhs));
}


TEST_F(RaceTestFixture, CheckComparisonRacesLHSwithBiggerLineNum) {
  auto lhs = *race_obj_ptr;
  lhs.lineNo += 1;
  const auto rhs = *race_obj_ptr;

  ASSERT_TRUE(functor.operator()(lhs, rhs));
}


TEST_F(RaceTestFixture, CheckComparisonRacesLHSwithDifferentAccessTypes) {
  auto lhs = *race_obj_ptr;
  lhs.accessType = "write";
  const auto rhs = *race_obj_ptr;

  ASSERT_TRUE(functor.operator()(lhs, rhs));
}

TEST_F(RaceTestFixture, CheckComparisonRacesLHSwithDifferentFileNames) {
  auto lhs = *race_obj_ptr;
  lhs.fileName = "some_other_file.cpp";
  const auto rhs = *race_obj_ptr;

  ASSERT_FALSE(functor.operator()(lhs, rhs));
}
