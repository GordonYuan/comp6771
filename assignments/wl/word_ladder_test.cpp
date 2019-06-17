/*

  == Explanation and rational of testing ==

  Explain and justify how you approached testing, the degree
   to which you're certain you have covered all possibilities,
   and why you think your tests are that thorough.

*/

#include "lexicon.h"
#include "assignments/wl/word_ladder.h"
#include "catch.h"

SCENARIO("Filter a set") {
    auto words = GetLexicon("assignments/wl/words.txt");
    WHEN("filter words to the same length as word 'abc'") {
        auto filteredWords = filterDissimilarWords(words, "abc");
        THEN("all filtered words should has length 3") {
            for (const string& word : filteredWords) {
                REQUIRE(word.size() == 3);
            }
        }
    }
}

SCENARIO("getWordMap") {
    unordered_set<string> words;
    words.insert("abd");    // only these words are ladder
    words.insert("aec");    // only these words are ladder
    words.insert("fbc");    // only these words are ladder
    words.insert("bec");    // but not this

    string word = "abc";
    WHEN("call getWordMap") {
        auto ladders = getWordMap(words, word);
        THEN("only abd, aec, fbc should be ladder, but not bec") {
            for (const auto& it : ladders) {
                std::cout<<it;
            }
            REQUIRE(ladders.size() == 3);
            REQUIRE(ladders.find("abd") != ladders.end());
            REQUIRE(ladders.find("aec") != ladders.end());
            REQUIRE(ladders.find("fbc") != ladders.end());
            REQUIRE(ladders.find("bec") == ladders.end());
        }
    }
}

SCENARIO("getWordMapAll") {
    unordered_set<string> words;
    words.insert("abd");
    words.insert("aec");
    words.insert("fbc");
    words.insert("bec");

    string word = "abc";
    WHEN("call getWordMapAll") {
        auto map = getWordMapAll(words);

        THEN("get a map of four elements") {
            REQUIRE(map.size() == 4);
        }

        AND_WHEN("get the mapped values of abd") {
            auto abd = map["abd"];

            REQUIRE(abd.empty());
        }

        AND_WHEN("get the mapped values of aec") {
            auto aec = map["aec"];

            REQUIRE(aec.size() == 1);
            REQUIRE(aec.find("bec") != aec.end());
        }

        AND_WHEN("get the mapped values of fbc") {
            auto fbc = map["fbc"];

            REQUIRE(fbc.empty());
        }

        AND_WHEN("get the mapped values of bec") {
            auto bec = map["bec"];

            REQUIRE(bec.size() == 1);
            REQUIRE(bec.find("aec") != bec.end());
        }
    }

}