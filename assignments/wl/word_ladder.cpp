#include "assignments/wl/word_ladder.h"
#include <iostream>

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
    vector<unordered_set<string>> hops{unordered_set<string>{from}};
    unordered_set<string> visited;
    bool toFound = false;
    while (!toFound) {
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
                vector<vector<string>> empty;
                return empty;
            } else {
                hops.push_back(nextHopWords);
            }
        }
    }

    int i = 0;
//    cout << "hop size: " << hops.size() << endl;
//    for (auto hop: hops) {
//        cout << i++ << ": ";
//        for (auto word: hop) {
//            cout << word << " ";
//        }
//        cout << endl;
//    }

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

//    i = 0;
//    cout << "hop size: " << hops.size() << endl;
//    for (auto hop: hops) {
//        cout << i++ << ": ";
//        for (auto word: hop) {
//            cout << word << " ";
//        }
//        cout << endl;
//    }

    // convert hops to one-to-many maps
    unordered_map<string, unordered_set<string>> ladderMap;
    for (auto itHops = hops.cbegin(); (itHops + 1) != hops.cend(); ++itHops) {
        const unordered_set<string> &fromSet = *itHops;
        const unordered_set<string> &toSet = *(itHops + 1);
        for (const string &fromWord: fromSet) {
            unordered_set<string> mappedSet;
            for (const string &toWord: toSet) {
                auto fullMap = wordMap[fromWord];
                if (fullMap.find(toWord) != fullMap.end()) {
                    mappedSet.insert(toWord);
                }
            }
            ladderMap.insert({fromWord, mappedSet});
        }
    }
//    for (auto entry: ladderMap) {
//        cout << entry.first << ": ";
//        for (auto word: entry.second) {
//            cout << word << " ";
//        }
//        cout << endl;
//    }

    // construct ladders
    vector<vector<string>> ladders;
    stack<string> stackDFS;
    vector<string> ladderStack;
    stackDFS.push(from);
    bool ladderStackNeedFix = false;
    while (! stackDFS.empty()) {
        string next = stackDFS.top();
        if (ladderStackNeedFix) {
            // after a ladder is found, re-construct ladder stack
            while (! ladderStack.empty()) {
                string back = ladderStack.back();
                auto map = ladderMap[back];
                if (map.find(next) != map.end()) {
                    break;
                } else {
                    ladderStack.pop_back();
                }
            }
        }
        ladderStack.push_back(next);
        if (next == to) {
            // a ladder is found
            vector<string> ladder = ladderStack;
            ladders.push_back(ladder);
            ladderStackNeedFix = true;
//            cout << "ladder: ";
//            for (auto a:ladder) {
//                cout << a << " ";
//            }
//            cout << endl;
        }
        stackDFS.pop();
        for (const string &word: ladderMap[next]) {
            stackDFS.push(word);
        }
    }

    // sort ladders
    // TODO add sort

    return ladders;
}