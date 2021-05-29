#include <gtest/gtest.h>

#include <memory>

#include "etsan/race.h"

class RaceTestFixture : public ::testing::Test {
 protected:

   std::shared_ptr<Race> race_obj_ptr;

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

  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->accessType));
  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->objName));
  ASSERT_NE(std::string::npos, msg.find(race_obj_ptr->fileName));
}
