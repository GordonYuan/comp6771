#include "word_ladder.h"
#include <iostream>

/*
 * return filtered set of words that is not as same length as the given word
 * */
void filterDissimilarWords(unordered_set<string> &words, const string &word);

/*
 * return the unordered_set of words that is one distance to the given word
 * for performance, it is assumed that all word in words have same size as the given word
 * */
unordered_set<string> getWordMap(const unordered_set<string> &words, const string &word);

/*
 * return a map of all words that map to the unordered_set of one distance words
 * for performance, it is assumed that all word in words have same size
 * */
unordered_map<string, unordered_set<string>> getWordMapAll(const unordered_set<string> &words);

/*
 * return true if word1 is exactly one letter different from word2, false otherwise
 */
bool neighbour(const string &word1, const string &word2);

/*
 * given a vector of hops, reversely filter words that are not achievable
 */
void filterReverseHops(vector<unordered_set<string>> &hops);

/*
 * construct and return a vector of hops using calculeted depth map
 */
vector<unordered_set<string>>
constructHops(const string &to, const unordered_map<string, unsigned short> &depths, unsigned short minHops);

/*
 * calculate and return a vector of hops for given word dictionary
 */
vector<unordered_set<string>> getHops(unordered_set<string> &words, const string &from, const string &to);

/*
 * construct and return a ladder map for the small amount of hops
 */
unordered_map<string, unordered_set<string>>
constructLadderMap(const unordered_set<string> &words, const vector<unordered_set<string>> &hops);

/*
 * construct and return all possible ladders using calculated ladder map
 */
vector<vector<string>>
constructLadders(unordered_map<string, unordered_set<string>> &ladderMap, const string &from, const string &to);


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
            mappedSet.emplace(wordInWords);
        }
    }

    return mappedSet;
}

unordered_map<string, unordered_set<string>> getWordMapAll(const unordered_set<string> &words) {
    unordered_map<string, unordered_set<string>> map{};

    for (auto &word: words) {
        map.emplace(make_pair(word, getWordMap(words, word)));
    }

    return map;
}

void filterReverseHops(vector<unordered_set<string>> &hops) {
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
            hops.clear();
            return;
        }
    }
}

vector<unordered_set<string>>
constructHops(const string &to, const unordered_map<string, unsigned short> &depths, unsigned short minHops) {
    vector<unordered_set<string>> hops(minHops, unordered_set<string>{});

    if (!minHops) {
        return hops;
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
    filterReverseHops(hops);
    return hops;
}

vector<unordered_set<string>> getHops(unordered_set<string> &words, const string &from, const string &to) {
    unordered_map<string, unsigned short> depths;
    queue<string> ladderQueue;

    depths.insert({from, 1});
    ladderQueue.push(from);
    unsigned short minHops = 0;

    while (!ladderQueue.empty()) {
        string curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            minHops = depths[curr];
            break;
        } else {
            unsigned short nextDepth = depths[curr] + 1;
            auto it = words.begin();
            while (it != words.end()) {
                string word = *it;
                if (neighbour(word, curr)) {
                    ladderQueue.push(word);
                    depths.insert({word, nextDepth});
                    it = words.erase(it);
                } else {
                    ++it;
                }
            }
        }
    }

    return constructHops(to, depths, minHops);
}

unordered_map<string, unordered_set<string>>
constructLadderMap(const unordered_set<string> &words, const vector<unordered_set<string>> &hops) {
    unordered_map<string, unordered_set<string>> ladderMap;
    auto wordMap = getWordMapAll(words);

    // convert hops to one-to-many maps
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

    return ladderMap;
}

vector<vector<string>>
constructLadders(unordered_map<string, unordered_set<string>> &ladderMap, const string &from, const string &to) {
    vector<vector<string>> ladders;
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

vector<vector<string>> computeLadder(unordered_set<string> &words, const string &from, const string &to) {
    filterDissimilarWords(words, from);
    vector<unordered_set<string>> hops = getHops(words, from, to);

    if (hops.empty()) {
        return vector<vector<string>>{};
    }

    words.clear();
    for (auto &hop: hops) {
        for (const string &word: hop) {
            words.insert(word);
        }
    }

    unordered_map<string, unordered_set<string>> ladderMap = constructLadderMap(words, hops);
    return constructLadders(ladderMap, from, to);
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