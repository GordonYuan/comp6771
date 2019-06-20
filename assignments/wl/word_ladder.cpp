#include "word_ladder.h"
#include <iostream>

void filterDissimilarWords(unordered_set<string> &words, const string &word) {
    auto wordLength = word.size();

    for (auto it = words.cbegin(); it != words.cend(); ) {
        if ((*it).size() != wordLength) {
            it = words.erase(it);
        }
        else {
            ++it;
        }
    }
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
    unordered_map<string, unordered_set<string>> map{};

    for (auto &word: words) {
        map.insert({word, getWordMap(words, word)});
    }

    return map;
}

vector<vector<string>> computeLadder(unordered_set<string> &words, const string &from, const string &to) {
    vector<vector<string>> ladders;
    filterDissimilarWords(words, from);
    auto wordMap = getWordMapAll(words);

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
                return ladders;
            } else {
                hops.push_back(nextHopWords);
            }
        }
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
        if (toHops.empty()) {
            return ladders;
        }
    }

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

    // construct ladders
    stack<string> stackDFS;
    vector<string> ladderStack;
    stackDFS.push(from);
    bool ladderStackNeedFix = false;
    while (!stackDFS.empty()) {
        string next = stackDFS.top();
        if (ladderStackNeedFix) {
            // after a ladder is found, re-construct ladder stack
            while (!ladderStack.empty()) {
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
            ladders.push_back(ladderStack);
            ladderStackNeedFix = true;
        }
        stackDFS.pop();
        for (const string &word: ladderMap[next]) {
            stackDFS.push(word);
        }
    }

    return ladders;
}

void sortLadders(vector<vector<string>> &ladders) {
    auto ladderGreater = [](const vector<string> &ladder1, const vector<string> &ladder2) -> bool {
        string s1 = accumulate(ladder1.cbegin(), ladder1.cend(), string(),
                               [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + ' ' + rhs; }
        );
        string s2 = accumulate(ladder2.cbegin(), ladder2.cend(), string(),
                               [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + ' ' + rhs; }
        );
        return s1 < s2;
    };
    sort(ladders.begin(), ladders.end(), ladderGreater);
}

void printLadders(const vector<vector<string>> &ladders) {
    if (ladders.empty()) {
        cout << "No ladder found." << '\n';
    } else {
        cout << "Found ladder: ";
        auto join = [](const vector<string> &v) -> string {
            return accumulate(v.cbegin(), v.cend(), string(),
                              [](string lhs, const string &rhs) { return lhs.empty() ? rhs : lhs + ' ' + rhs; }
            );
        };
        for (const auto &ladder : ladders) {
            const string ladderString = join(ladder);
            cout << ladderString << '\n';
        }
    }
    cout << flush;
}