#include <gtest/gtest.h>

#include "tests/include/gtestbdd.h"
#include "interface/cli/command.h"
#include "mockcpp/mokc.h"

SCENARIO("Test command parser") {

  GIVEN("a command option") {

    WHEN("command is parsed") {

      THEN("ok") {
        EXPECT_EQ(1, 1);
      }

    }

  }

}