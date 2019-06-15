#include "assignments/wl/word_ladder.h"

unordered_set<string> filterDissimilarWords(const unordered_set<string>& words, const string& word){
    auto wordLength = word.size();
    unordered_set<string> filteredSet;

    for (const string& wordInWords: words) {
        if (wordInWords.size() == wordLength) {
            filteredSet.insert(wordInWords);
        }
    }

    return filteredSet;
}
