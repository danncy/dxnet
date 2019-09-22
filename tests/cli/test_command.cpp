#include <gtest/gtest.h>

#include "tests/include/gtestbdd.h"
#include "interface/cli/command.h"
#include "mockcpp/mokc.h"

using namespace cli;

SCENARIO("Test command parser") {

  GIVEN("a self-defined git command option") {
    Program prog = Program::New("git")
      .AddCommand(Command("clone")
                    .AddOption("--username", "username")
                    .AddOption("--password", "password")
                    .AddOption("<repo>", "repo path"))
      .AddCommand(Command("checkout").AddOption("<repo>", "repo path"))
      .AddCommand(Command("reset").AddOption("--commit-id", "the commit id", ParseInt));

    WHEN("command is parsed") {

      THEN("ok") {
        EXPECT_EQ("git", prog.name());
      }

    }

  }

}