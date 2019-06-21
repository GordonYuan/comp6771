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

  GIVEN("3 words which does not forms a ladder") {
    std::unordered_set<std::string> lexicon{"con", "abc", "cat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("no solution") {
        std::vector<std::vector<std::string>> solution{};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("4 words which forms 2 ladders") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat", "cot"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("2 solutions") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"},
                                                       {"con", "cot", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("4 words which forms 1 ladder") {
    std::unordered_set<std::string> lexicon{"con", "can", "abc", "cat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("1 solution") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("many words which forms 2 ladders") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat", "cot", "abc",
                                            "aba", "aaa", "bab", "cda"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("2 solutions") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"},
                                                       {"con", "cot", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("4 words which forms a ladder, including one with different length") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat", "concancat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("one solution") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("many words which forms 2 ladders, including some with different length") {
    std::unordered_set<std::string> lexicon{
        "con", "can", "cat", "cot", "abc", "concancat", "canconcat",
        "bab", "cda", "c",   "a",   "b",   "ba",        "asdgausgboasduighsaipghasi"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("2 solutions") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"},
                                                       {"con", "cot", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }

  GIVEN("words that contains paths including short path and long paths") {
    std::unordered_set<std::string> lexicon{"con", "can", "cat", "don", "dan", "dat"};
    WHEN("search for ladders") {
      std::vector<std::vector<std::string>> ladders = computeLadder(lexicon, "con", "cat");
      THEN("only shortest path is returned") {
        std::vector<std::vector<std::string>> solution{{"con", "can", "cat"}};
        REQUIRE(ladders == solution);
      }
    }
  }
}
