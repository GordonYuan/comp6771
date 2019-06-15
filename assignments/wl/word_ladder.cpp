#include "assignments/wl/word_ladder.h"

unordered_set<string> filterDissimilarWords(const unordered_set<string> &words, const string &word) {
    auto wordLength = word.size();
    unordered_set<string> filteredSet;

    for (const string &wordInWords: words) {
        if (wordInWords.size() == wordLength) {
            filteredSet.insert(wordInWords);
        }
    }

    return filteredSet;
}

unordered_set<string> getWordMap(const unordered_set<string> &words, const string &word) {
    unordered_set<string> mappedSet;

    for (const string &wordInWords: words) {
        bool differenceOccurred = false;
        bool isLadder = true;
        for (auto itChar1 = wordInWords.cbegin(), itChar2 = word.cbegin();
             itChar1 != wordInWords.cend(); ++itChar1, ++itChar2) {
            if ((*itChar1) != (*itChar2)) {
                // difference detected
                if (differenceOccurred) {
                    // already detected a difference, no second difference allowed
                    // this word is not ladder
                    isLadder = false;
                    continue;
                } else {
                    // record the first difference
                    differenceOccurred = true;
                }
            }
        }
        // ladder should have difference
        isLadder &= differenceOccurred;
        if (isLadder) {
            // this word is ladder, add to set
            mappedSet.insert(wordInWords);
        }
    }

    return mappedSet;
}

unordered_map<string, unordered_set<string>> getWordMapAll(const unordered_set<string> &words) {
    unordered_map<string, unordered_set<string>> map;

    for (auto &word: words) {
        map.insert({word, getWordMap(words, word)});
    }

    return map;
}