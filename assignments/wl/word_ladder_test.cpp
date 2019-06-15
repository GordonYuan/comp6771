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
