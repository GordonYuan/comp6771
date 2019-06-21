#include "word_ladder.h"
#include <iostream>

bool neighbour(const string &word1, const string &word2) {
    bool differenceOccurred = false;
    for (auto itChar1 = word1.cbegin(), itChar2 = word2.cbegin();
         itChar1 != word1.cend(); ++itChar1, ++itChar2) {
        if ((*itChar1) != (*itChar2)) {
            // difference detected
            if (differenceOccurred) {
                // already detected a difference, no second difference allowed
                // this word is not ladder
                return false;
            } else {
                // record the first difference
                differenceOccurred = true;
            }
        }
    }
    return differenceOccurred;
}

void filterDissimilarWords(unordered_set<string> &words, const string &word) {
    auto wordLength = word.size();

    for (auto it = words.cbegin(); it != words.cend();) {
        if ((*it).size() != wordLength) {
            it = words.erase(it);
        } else {
            ++it;
        }
    }
}

unordered_set<string> getWordMap(const unordered_set<string> &words, const string &word) {
    unordered_set<string> mappedSet;

    for (const string &wordInWords: words) {
        if (neighbour(wordInWords, word)) {
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

vector<vector<string>> computeLadderOld(unordered_set<string> &words, const string &from, const string &to) {
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

void filterMore(unordered_set<string> &words, const string &from, const string &to) {
    queue<string> ladderQueue;
    unordered_set<string> newWords;

    ladderQueue.push(from);

    while (!ladderQueue.empty()) {
        string curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            words = newWords;
            return;
        } else {
            auto it = words.begin();
            while (it != words.end()) {
                string word = *it;
                if (neighbour(word, curr)) {
                    ladderQueue.push(word);
                    newWords.insert(word);
                    it = words.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }
}

// TODO swap these two
unsigned short calculateMinHops(unordered_set<string> &words, const string &from, const string &to) {
    unordered_map<string, unsigned short> depths;
    queue<string> ladderQueue;
    unordered_set<string> newWords;

    depths.insert({from, 1});
    ladderQueue.push(from);

    while (!ladderQueue.empty()) {
        string curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            filterMore(newWords, to, from);
            words = newWords;
            return depths[curr];
        } else {
            unsigned short nextDepth = depths[curr] + 1;
            auto it = words.begin();
            while (it != words.end()) {
                string word = *it;
                if (neighbour(word, curr)) {
                    ladderQueue.push(word);
                    depths.insert({word, nextDepth});
                    newWords.insert(word);
                    it = words.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    return 0;
}

vector<unordered_set<string>> getHops(unordered_set<string> &words, const string &from, const string &to) {
    unordered_map<string, unsigned short> depths;
    queue<string> ladderQueue;
//    unordered_set<string> newWords;

    depths.insert({from, 1});
    ladderQueue.push(from);
    unsigned short minHops = 0;

    while (!ladderQueue.empty()) {
        string curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            minHops = depths[curr];
//            break;
        } else {
            unsigned short nextDepth = depths[curr] + 1;
            auto it = words.begin();
            while (it != words.end()) {
                string word = *it;
                if (neighbour(word, curr)) {
                    ladderQueue.push(word);
                    depths.insert({word, nextDepth});
//                    newWords.insert(word);
                    it = words.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    vector<unordered_set<string>> hops;

    if (! minHops) {
        return hops;
    }
    for (unsigned short i = 0; i < minHops; ++i) {
        hops.emplace_back(unordered_set<string>{});
    }

    // convert to hops
    for (auto &entry: depths) {
        const unsigned short depth = entry.second;
        if (depth < minHops) {
            const string &word = entry.first;
            hops[depth - 1].insert(word);
        }
    }
    hops[minHops - 1].insert(to);

    // filter words cant be reversely achieved from 'to'
    for (reverse_iterator fromHops = hops.rbegin(); (fromHops + 1) != hops.rend(); ++fromHops) {
        unordered_set<string> &toHops = *(fromHops + 1);
        for (auto itToHops = toHops.begin(); itToHops != toHops.end();) {
            bool canMapFromTo = false;
            for (const string &fromWord : (*fromHops)) {
                canMapFromTo = neighbour(fromWord, *itToHops);
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
            return vector<unordered_set<string>>{};
        }
    }

    return hops;
}

vector<vector<string>> computeLadder(unordered_set<string> &words, const string &from, const string &to) {
    vector<vector<string>> ladders;
    filterDissimilarWords(words, from);

    auto hops = getHops(words, from, to);
    if (hops.empty()) {
        return ladders;
    }

    unordered_set<string> newWords;
    for (auto &hop: hops) {
        for (const string &word: hop) {
            newWords.insert(word);
        }
    }

    // construct ladders
    auto wordMap = getWordMapAll(newWords);

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
    sort(ladders.begin(), ladders.end());
}

void printLadders(const vector<vector<string>> &ladders) {
    if (ladders.empty()) {
        cout << "No ladder found." << '\n';
    } else {
        cout << "Found ladder: ";
        for (const auto &ladder : ladders) {
            for (const auto &word: ladder) {
                cout << word << ' ';
            }
            cout << '\n';
        }
    }
    cout << flush;
}