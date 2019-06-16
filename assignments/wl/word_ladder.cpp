#include "assignments/wl/word_ladder.h"
#include <iostream>

static unordered_set<string> filterDissimilarWords(const unordered_set<string> &words, const string &word) {
    auto wordLength = word.size();
    unordered_set<string> filteredSet;

    for (const string &wordInWords: words) {
        if (wordInWords.size() == wordLength) {
            filteredSet.insert(wordInWords);
        }
    }

    return filteredSet;
}

static unordered_set<string> getWordMap(const unordered_set<string> &words, const string &word) {
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

static unordered_map<string, unordered_set<string>> getWordMapAll(const unordered_set<string> &words) {
    unordered_map<string, unordered_set<string>> map;

    for (auto &word: words) {
        map.insert({word, getWordMap(words, word)});
    }

    return map;
}

vector<vector<string>> computeLadder(const unordered_set<string> &words, const string &from, const string &to) {
    auto similarWords = filterDissimilarWords(words, from);
    auto wordMap = getWordMapAll(similarWords);
//    for (auto map: wordMap) {
//        cout << endl << map.first << ": ";
//        unordered_set<string> mapped;
//        mapped = map.second;
//        for (auto word: mapped) {
//            cout << word << " ";
//        }
//    }

    cout << "pre process done." << endl;


    // compute words appeared until found to
    vector<vector<string>> ladders;
    vector<unordered_set<string>> hops{unordered_set<string>{from}};
    unordered_set<string> visited;
    bool toFound = false;
    while (!toFound) {
        cout << "finding..." << endl;
        unordered_set<string> nextHopWords;
        for (const string &word: hops.back()) {
            for (const string &oneHopWord : wordMap[word]) {
                if (visited.find(oneHopWord) == visited.end()) {
                    nextHopWords.insert(oneHopWord);
                    visited.insert(oneHopWord);
                    toFound |= oneHopWord == to;
                }
            }
        }
        if (toFound) {
            hops.push_back({to});
        } else {
            if (nextHopWords.empty()) {
                // no solution, no more words can be found
                return ladders;
            } else {
                hops.push_back(nextHopWords);
            }
        }
    }

    int i = 0;
    cout << "hop size: " << hops.size() << endl;
    for (auto hop: hops) {
        cout << i++ << ": ";
        for (auto word: hop) {
            cout << word << " ";
        }
        cout << endl;
    }

    // filter words cant be reversely achieved from 'to'
    for (reverse_iterator fromHops = hops.rbegin(); (fromHops + 1) != hops.rend(); ++fromHops) {
        unordered_set<string> &toHops = *(fromHops + 1);
        for (auto itToHops = toHops.begin(); itToHops != toHops.end();) {
            bool canMapFromTo = false;
            for (const string &fromWord : (*fromHops)) {
                auto mappedSet = wordMap[fromWord];
                canMapFromTo = mappedSet.find(*itToHops) != mappedSet.end();
                if (canMapFromTo) {
                    break;
                }
            }
            if (!canMapFromTo) {
                itToHops = toHops.erase(itToHops);
            } else if (itToHops != toHops.end()) {
                ++itToHops;
            }
        }
    }

    i = 0;
    cout << "hop size: " << hops.size() << endl;
    for (auto hop: hops) {
        cout << i++ << ": ";
        for (auto word: hop) {
            cout << word << " ";
        }
        cout << endl;
    }

    return ladders;
}