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
    // keep comparing until two differences occur or compared all characters
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

    // look up all words and remove words with different length
    for (auto it = words.cbegin(); it != words.cend();) {
        if ((*it).size() != wordLength) {
            // size differs, remove it
            it = words.erase(it);
        } else {
            ++it;
        }
    }
}

std::unordered_set<std::string> getWordMap(const std::unordered_set<std::string> &words, const std::string &word) {
    std::unordered_set<std::string> mappedSet;

    // for each word, check if it is neighbour
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

    // for each word, make a map
    for (auto &word: words) {
        map.emplace(make_pair(word, getWordMap(words, word)));
    }

    return map;
}

void filterReverseHops(std::vector<std::unordered_set<std::string>> &hops) {
    // filter words cant be reversely achieved from 'to', start from right side
    for (std::reverse_iterator fromHops = hops.rbegin(); (fromHops + 1) != hops.rend(); ++fromHops) {
        std::unordered_set<std::string> &toHops = *(fromHops + 1);
        // for the hop level, check all previous hop level words
        for (auto itToHops = toHops.begin(); itToHops != toHops.end();) {
            bool canMapFromTo = false;
            for (const std::string &fromWord : (*fromHops)) {
                // as long as there exist one word that can map to the previous hop word, end for loop
                canMapFromTo = neighbour(fromWord, *itToHops);
                if (canMapFromTo) {
                    break;
                }
            }
            if (!canMapFromTo) {
                // cannot map to previous hop level word, remove it
                itToHops = toHops.erase(itToHops);
            } else if (itToHops != toHops.end()) {
                ++itToHops;
            }
        }
        if (toHops.empty()) {
            // as long as there exist one hop level that contains no words,
            // 'from' word can never map to 'to' word.
            // clear all hops and return immediately
            hops.clear();
            return;
        }
    }
}

std::vector<std::unordered_set<std::string>>
constructHops(const std::string &to, const std::unordered_map<std::string, unsigned short> &depths,
              unsigned short minHops) {
    // hops starts with #minHops of empty sets
    std::vector<std::unordered_set<std::string>> hops(minHops, std::unordered_set<std::string>{});

    // minHops == 0 means no possible hop exists
    if (!minHops) {
        return hops;
    }

    // convert to hops by depth map
    for (auto &entry: depths) {
        const unsigned short &depth = entry.second;
        if (depth < minHops) {
            const std::string &word = entry.first;
            hops[depth - 1].insert(word);
        }
    }

    // the last hop will always be 'to'
    hops[minHops - 1].insert(to);

    // filter unreachable words to improve ladder searching effeciency
    filterReverseHops(hops);

    return hops;
}

std::vector<std::unordered_set<std::string>>
getHops(std::unordered_set<std::string> &words, const std::string &from, const std::string &to) {
    std::unordered_map<std::string, unsigned short> depths;
    std::queue<std::string> ladderQueue;

    // depth map, always starts with 'from'
    depths.insert({from, 1});
    ladderQueue.push(from);
    unsigned short minHops = 0;

    // pure BFS algorithm, search until 'to' appears
    while (!ladderQueue.empty()) {
        const std::string &curr = ladderQueue.front();
        ladderQueue.pop();
        if (curr == to) {
            // destination, 'to' appears. curr is the minimum hops required
            minHops = depths[curr];
            break;
        } else {
            // keep searching and adding depth to next level of hops
            unsigned short nextDepth = depths[curr] + 1;
            auto it = words.begin();

            // search all neighours current word can connect to and enqueue them
            while (it != words.end()) {
                const std::string &word = *it;
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
                const std::unordered_set<std::string> &fullMap = wordMap[fromWord];
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

    // starts from 'from'
    stackDFS.push(from);
    bool ladderStackNeedFix = false;

    // iterative DFS algorithm to construct final ladders
    while (!stackDFS.empty()) {
        const std::string &next = stackDFS.top();
        if (ladderStackNeedFix) {
            // after a ladder is found, re-construct ladder stack
            while (!ladderStack.empty()) {
                const std::string &back = ladderStack.back();
                const std::unordered_set<std::string> &map = ladderMap[back];
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
    // filter words and reserve only those whose size are same with 'from'
    // it is assumed that 'from' and 'to' have same size
    filterDissimilarWords(words, from);

    // compute hops from filtered words
    std::vector<std::unordered_set<std::string>> hops = getHops(words, from, to);

    // empty hops means no ladders, return empty vector
    if (hops.empty()) {
        return std::vector<std::vector<std::string>>{};
    }

    // re-use word vector, to store filtered words in hops
    words.clear();
    for (auto &hop: hops) {
        for (const std::string &word: hop) {
            words.insert(word);
        }
    }

    // compute ladder map which is then used to construct final ladders
    std::unordered_map<std::string, std::unordered_set<std::string>> ladderMap = constructLadderMap(words, hops);

    // compute and reuturn final ladders using ladder map
    return constructLadders(ladderMap, from, to);
}

void sortLadders(std::vector<std::vector<std::string>> &ladders) {
    // simple, just call built in sort, which sorts vectors in order
    std::sort(ladders.begin(), ladders.end());
}

void printLadders(const std::vector<std::vector<std::string>> &ladders) {
    // prints ladders in specified format
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