/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include <string>
#include <unordered_set>
#include <vector>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("Find ladders", "[computeLadder]") {
  GIVEN("3 words which forms a ladder") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("one solution") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("3 words which forms a ladder") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("one solution") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }
}
