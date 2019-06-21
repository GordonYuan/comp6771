#include "word_ladder.h"

/*
 * return filtered set of words that is not as same length as the given word
 * */
void filterDissimilarWords(std::unordered_set<std::string> &words, const std::string &word);

/*
 * return the unordered_set of words that is one distance to the given word
 * for performance, it is assumed that all word in words have same size as the given word
 * */
std::unordered_set<std::string> getWordMap(const std::unordered_set<std::string> &words, const std::string &word);

/*
 * return a map of all words that map to the unordered_set of one distance words
 * for performance, it is assumed that all word in words have same size
 * */
std::unordered_map<std::string, std::unordered_set<std::string>>
getWordMapAll(const std::unordered_set<std::string> &words);

/*
 * return true if word1 is exactly one letter different from word2, false otherwise
 */
bool neighbour(const std::string &word1, const std::string &word2);

/*
 * given a vector of hops, reversely filter words that are not achievable
 */
void filterReverseHops(std::vector<std::unordered_set<std::string>> &hops);

/*
 * construct and return a vector of hops using calculeted depth map
 */
std::vector<std::unordered_set<std::string>>
constructHops(const std::string &to, const std::unordered_map<std::string, unsigned short> &depths,
              unsigned short minHops);

/*
 * calculate and return a vector of hops for given word dictionary
 */
std::vector<std::unordered_set<std::string>>
getHops(std::unordered_set<std::string> &words, const std::string &from, const std::string &to);

/*
 * construct and return a ladder map for the small amount of hops
 */
std::unordered_map<std::string, std::unordered_set<std::string>>
constructLadderMap(const std::unordered_set<std::string> &words,
                   const std::vector<std::unordered_set<std::string>> &hops);

/*
 * construct and return all possible ladders using calculated ladder map
 */
std::vector<std::vector<std::string>>
constructLadders(std::unordered_map<std::string, std::unordered_set<std::string>> &ladderMap, const std::string &from,
                 const std::string &to);


bool neighbour(const std::string &word1, const std::string &word2) {
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

void filterDissimilarWords(std::unordered_set<std::string> &words, const std::string &word) {
    auto wordLength = word.size();

    for (auto it = words.cbegin(); it != words.cend();) {
        if ((*it).size() != wordLength) {
            it = words.erase(it);
        } else {
            ++it;
        }
    }
}

std::unordered_set<std::string> getWordMap(const std::unordered_set<std::string> &words, const std::string &word) {
    std::unordered_set<std::string> mappedSet;

    for (const std::string &wordInWords: words) {
        if (neighbour(wordInWords, word)) {
            // this word is ladder, add to set
            mappedSet.emplace(wordInWords);
        }
    }

    return mappedSet;
}

std::unordered_map<std::string, std::unordered_set<std::string>>
getWordMapAll(const std::unordered_set<std::string> &words) {
    std::unordered_map<std::string, std::unordered_set<std::string>> map{};

    for (auto &word: words) {
        map.emplace(make_pair(word, getWordMap(words, word)));
    }

    return map;
}

void filterReverseHops(std::vector<std::unordered_set<std::string>> &hops) {
    // filter words cant be reversely achieved from 'to'
    for (std::reverse_iterator fromHops = hops.rbegin(); (fromHops + 1) != hops.rend(); ++fromHops) {
        std::unordered_set<std::string> &toHops = *(fromHops + 1);
        for (auto itToHops = toHops.begin(); itToHops != toHops.end();) {
            bool canMapFromTo = false;
            for (const std::string &fromWord : (*fromHops)) {
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

std::vector<std::unordered_set<std::string>>
constructHops(const std::string &to, const std::unordered_map<std::string, unsigned short> &depths,
              unsigned short minHops) {
    std::vector<std::unordered_set<std::string>> hops(minHops, std::unordered_set<std::string>{});

    if (!minHops) {
        return hops;
    }

    // convert to hops
    for (auto &entry: depths) {
        const unsigned short depth = entry.second;
        if (depth < minHops) {
            const std::string &word = entry.first;
            hops[depth - 1].insert(word);
        }
    }
    hops[minHops - 1].insert(to);
    filterReverseHops(hops);
    return hops;
}

std::vector<std::unordered_set<std::string>>
getHops(std::unordered_set<std::string> &words, const std::string &from, const std::string &to) {
    std::unordered_map<std::string, unsigned short> depths;
    std::queue<std::string> ladderQueue;

    depths.insert({from, 1});
    ladderQueue.push(from);
    unsigned short minHops = 0;

    while (!ladderQueue.empty()) {
        std::string curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            minHops = depths[curr];
            break;
        } else {
            unsigned short nextDepth = depths[curr] + 1;
            auto it = words.begin();
            while (it != words.end()) {
                std::string word = *it;
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

std::unordered_map<std::string, std::unordered_set<std::string>>
constructLadderMap(const std::unordered_set<std::string> &words,
                   const std::vector<std::unordered_set<std::string>> &hops) {
    std::unordered_map<std::string, std::unordered_set<std::string>> ladderMap;
    auto wordMap = getWordMapAll(words);

    // convert hops to one-to-many maps
    for (auto itHops = hops.cbegin(); (itHops + 1) != hops.cend(); ++itHops) {
        const std::unordered_set<std::string> &fromSet = *itHops;
        const std::unordered_set<std::string> &toSet = *(itHops + 1);
        for (const std::string &fromWord: fromSet) {
            std::unordered_set<std::string> mappedSet;
            for (const std::string &toWord: toSet) {
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

std::vector<std::vector<std::string>>
constructLadders(std::unordered_map<std::string, std::unordered_set<std::string>> &ladderMap, const std::string &from,
                 const std::string &to) {
    std::vector<std::vector<std::string>> ladders;
    std::stack<std::string> stackDFS;
    std::vector<std::string> ladderStack;
    stackDFS.push(from);
    bool ladderStackNeedFix = false;
    while (!stackDFS.empty()) {
        std::string next = stackDFS.top();
        if (ladderStackNeedFix) {
            // after a ladder is found, re-construct ladder stack
            while (!ladderStack.empty()) {
                std::string back = ladderStack.back();
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
        for (const std::string &word: ladderMap[next]) {
            stackDFS.push(word);
        }
    }

    return ladders;
}

std::vector<std::vector<std::string>>
computeLadder(std::unordered_set<std::string> &words, const std::string &from, const std::string &to) {
    filterDissimilarWords(words, from);
    std::vector<std::unordered_set<std::string>> hops = getHops(words, from, to);

    if (hops.empty()) {
        return std::vector<std::vector<std::string>>{};
    }

    words.clear();
    for (auto &hop: hops) {
        for (const std::string &word: hop) {
            words.insert(word);
        }
    }

    std::unordered_map<std::string, std::unordered_set<std::string>> ladderMap = constructLadderMap(words, hops);
    return constructLadders(ladderMap, from, to);
}

void sortLadders(std::vector<std::vector<std::string>> &ladders) {
    sort(ladders.begin(), ladders.end());
}

void printLadders(const std::vector<std::vector<std::string>> &ladders) {
    if (ladders.empty()) {
        std::cout << "No ladder found." << '\n';
    } else {
        std::cout << "Found ladder: ";
        for (const auto &ladder : ladders) {
            for (const auto &word: ladder) {
                std::cout << word << ' ';
            }
            std::cout << '\n';
        }
    }
    std::cout << std::flush;
}