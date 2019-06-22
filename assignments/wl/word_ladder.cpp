#include "assignments/wl/word_ladder.h"

/*
 * return filtered set of words that is not as same length as the given word
 * */
void FilterDissimilarWords(std::unordered_set<std::string>& words, const std::string& word);

/*
 * return the unordered_set of words that is one distance to the given word
 * for performance, it is assumed that all word in words have same size as the
 * given word
 * */
std::unordered_set<std::string> GetWordMap(const std::unordered_set<std::string>& words,
                                           const std::string& word);

/*
 * sort ladders in their lexicographic order
 */
void SortLadders(std::vector<std::vector<std::string>>& ladders);

/*
 * return a map of all words that map to the unordered_set of one distance words
 * for performance, it is assumed that all word in words have same size
 * */
std::unordered_map<std::string, std::unordered_set<std::string>>
GetWordMapAll(const std::unordered_set<std::string>& words);

/*
 * return true if word1 is exactly one letter different from word2, false
 * otherwise
 */
bool Neighbour(const std::string& word1, const std::string& word2);

/*
 * given a vector of hops, reversely filter words that are not achievable
 */
void FilterReverseHops(std::vector<std::unordered_set<std::string>>& hops);

/*
 * construct and return a vector of hops using calculeted depth map
 */
std::vector<std::unordered_set<std::string>> ConstructHops(
    const std::string& to, const std::unordered_map<std::string, int>& depths, int min_hops);

/*
 * calculate and return a vector of hops for given word dictionary
 */
std::vector<std::unordered_set<std::string>>
GetHops(std::unordered_set<std::string>& words, const std::string& from, const std::string& to);

/*
 * construct and return a ladder map for the small amount of hops
 */
std::unordered_map<std::string, std::unordered_set<std::string>>
ConstructLadderMap(const std::unordered_set<std::string>& words,
                   const std::vector<std::unordered_set<std::string>>& hops);

/*
 * construct and return all possible ladders using calculated ladder map
 */
std::vector<std::vector<std::string>>
ConstructLadders(std::unordered_map<std::string, std::unordered_set<std::string>>& ladder_map,
                 const std::string& from,
                 const std::string& to);

bool Neighbour(const std::string& word1, const std::string& word2) {
  bool difference_occurred = false;
  // keep comparing until two differences occur or compared all characters
  for (auto it_char1 = word1.cbegin(), it_char2 = word2.cbegin(); it_char1 != word1.cend();
       ++it_char1, ++it_char2) {
    if ((*it_char1) != (*it_char2)) {
      // difference detected
      if (difference_occurred) {
        // already detected a difference, no second difference allowed
        // this word is not ladder
        return false;
      } else {
        // record the first difference
        difference_occurred = true;
      }
    }
  }
  return difference_occurred;
}

void FilterDissimilarWords(std::unordered_set<std::string>& words, const std::string& word) {
  auto word_length = word.size();

  // look up all words and remove words with different length
  for (auto it = words.cbegin(); it != words.cend();) {
    if ((*it).size() != word_length) {
      // size differs, remove it
      it = words.erase(it);
    } else {
      ++it;
    }
  }
}

std::unordered_set<std::string> GetWordMap(const std::unordered_set<std::string>& words,
                                           const std::string& word) {
  std::unordered_set<std::string> mapped_set;

  // for each word, check if it is neighbour
  for (const std::string& word_in_words : words) {
    if (Neighbour(word_in_words, word)) {
      // this word is ladder, add to set
      mapped_set.emplace(word_in_words);
    }
  }

  return mapped_set;
}

std::unordered_map<std::string, std::unordered_set<std::string>>
GetWordMapAll(const std::unordered_set<std::string>& words) {
  std::unordered_map<std::string, std::unordered_set<std::string>> map{};

  // for each word, make a map
  for (auto& word : words) {
    map.emplace(make_pair(word, GetWordMap(words, word)));
  }

  return map;
}

void FilterReverseHops(std::vector<std::unordered_set<std::string>>& hops) {
  // filter words cant be reversely achieved from 'to', start from right side
  for (std::reverse_iterator from_hops = hops.rbegin(); (from_hops + 1) != hops.rend();
       ++from_hops) {
    std::unordered_set<std::string>& to_hops = *(from_hops + 1);
    // for the hop level, check all previous hop level words
    for (auto it_to_hops = to_hops.begin(); it_to_hops != to_hops.end();) {
      bool can_map_fro_tTo = false;
      for (const std::string& from_word : (*from_hops)) {
        // as long as there exist one word that can map to the previous hop
        // word, end for loop
        can_map_fro_tTo = Neighbour(from_word, *it_to_hops);
        if (can_map_fro_tTo) {
          break;
        }
      }
      if (!can_map_fro_tTo) {
        // cannot map to previous hop level word, remove it
        it_to_hops = to_hops.erase(it_to_hops);
      } else if (it_to_hops != to_hops.end()) {
        ++it_to_hops;
      }
    }
    if (to_hops.empty()) {
      // as long as there exist one hop level that contains no words,
      // 'from' word can never map to 'to' word.
      // clear all hops and return immediately
      hops.clear();
      return;
    }
  }
}

std::vector<std::unordered_set<std::string>> ConstructHops(
    const std::string& to, const std::unordered_map<std::string, int>& depths, int min_hops) {
  // hops starts with #minHops of empty sets
  std::vector<std::unordered_set<std::string>> hops(min_hops, std::unordered_set<std::string>{});

  // minHops == 0 means no possible hop exists
  if (!min_hops) {
    return hops;
  }

  // convert to hops by depth map
  for (auto& entry : depths) {
    const int& depth = entry.second;
    if (depth < min_hops) {
      const std::string& word = entry.first;
      hops[depth - 1].insert(word);
    }
  }

  // the last hop will always be 'to'
  hops[min_hops - 1].insert(to);

  // filter unreachable words to improve ladder searching effeciency
  FilterReverseHops(hops);

  return hops;
}

std::vector<std::unordered_set<std::string>>
GetHops(std::unordered_set<std::string>& words, const std::string& from, const std::string& to) {
  std::unordered_map<std::string, int> depths;
  std::queue<std::string> ladder_queue;

  // depth map, always starts with 'from'
  depths.insert({from, 1});
  ladder_queue.push(from);
  int min_hops = 0;

  // pure BFS algorithm, search until 'to' appears
  while (!ladder_queue.empty()) {
    const std::string& curr = ladder_queue.front();
    ladder_queue.pop();
    if (curr == to) {
      // destination, 'to' appears. curr is the minimum hops required
      min_hops = depths[curr];
      break;
    } else {
      // keep searching and adding depth to next level of hops
      int next_depth = depths[curr] + 1;
      auto it = words.begin();

      // search all neighours current word can connect to and enqueue them
      while (it != words.end()) {
        const std::string& word = *it;
        if (Neighbour(word, curr)) {
          ladder_queue.push(word);
          depths.insert({word, next_depth});
          it = words.erase(it);
        } else {
          ++it;
        }
      }
    }
  }

  return ConstructHops(to, depths, min_hops);
}

std::unordered_map<std::string, std::unordered_set<std::string>>
ConstructLadderMap(const std::unordered_set<std::string>& words,
                   const std::vector<std::unordered_set<std::string>>& hops) {
  std::unordered_map<std::string, std::unordered_set<std::string>> ladder_map;
  auto word_map = GetWordMapAll(words);

  // convert hops to one-to-many maps
  for (auto it_hops = hops.cbegin(); (it_hops + 1) != hops.cend(); ++it_hops) {
    const std::unordered_set<std::string>& from_set = *it_hops;
    const std::unordered_set<std::string>& to_set = *(it_hops + 1);
    for (const std::string& from_word : from_set) {
      std::unordered_set<std::string> mapped_set;
      for (const std::string& to_word : to_set) {
        const std::unordered_set<std::string>& full_map = word_map[from_word];
        if (full_map.find(to_word) != full_map.end()) {
          mapped_set.insert(to_word);
        }
      }
      ladder_map.insert({from_word, mapped_set});
    }
  }

  return ladder_map;
}

std::vector<std::vector<std::string>>
ConstructLadders(std::unordered_map<std::string, std::unordered_set<std::string>>& ladder_map,
                 const std::string& from,
                 const std::string& to) {
  std::vector<std::vector<std::string>> ladders;
  std::stack<std::string> stackDFS;
  std::vector<std::string> ladder_stack;

  // starts from 'from'
  stackDFS.push(from);
  bool ladder_stack_need_fix = false;

  // iterative DFS algorithm to construct final ladders
  while (!stackDFS.empty()) {
    const std::string& next = stackDFS.top();
    if (ladder_stack_need_fix) {
      // after a ladder is found, re-construct ladder stack
      while (!ladder_stack.empty()) {
        const std::string& back = ladder_stack.back();
        const std::unordered_set<std::string>& map = ladder_map[back];
        if (map.find(next) != map.end()) {
          break;
        } else {
          ladder_stack.pop_back();
        }
      }
    }
    ladder_stack.push_back(next);
    if (next == to) {
      // a ladder is found
      ladders.push_back(ladder_stack);
      ladder_stack_need_fix = true;
    }
    stackDFS.pop();
    for (const std::string& word : ladder_map[next]) {
      stackDFS.push(word);
    }
  }

  return ladders;
}

std::vector<std::vector<std::string>> ComputeLadder(std::unordered_set<std::string>& words,
                                                    const std::string& from,
                                                    const std::string& to) {
  // filter words and reserve only those whose size are same with 'from'
  // it is assumed that 'from' and 'to' have same size
  FilterDissimilarWords(words, from);

  // compute hops from filtered words
  std::vector<std::unordered_set<std::string>> hops = GetHops(words, from, to);

  // empty hops means no ladders, return empty vector
  if (hops.empty()) {
    return std::vector<std::vector<std::string>>{};
  }

  // re-use word vector, to store filtered words in hops
  words.clear();
  for (auto& hop : hops) {
    for (const std::string& word : hop) {
      words.insert(word);
    }
  }

  // compute ladder map which is then used to construct final ladders
  std::unordered_map<std::string, std::unordered_set<std::string>> ladder_map =
      ConstructLadderMap(words, hops);

  // compute  final ladders using ladder map
  std::vector<std::vector<std::string>> ladders = ConstructLadders(ladder_map, from, to);

  // sort ladders
  SortLadders(ladders);

  return ladders;
}

void SortLadders(std::vector<std::vector<std::string>>& ladders) {
  // simple, just call built in sort, which sorts vectors in order
  std::sort(ladders.begin(), ladders.end());
}

void PrintLadders(const std::vector<std::vector<std::string>>& ladders) {
  // prints ladders in specified format
  if (ladders.empty()) {
    std::cout << "No ladder found." << '\n';
  } else {
    std::cout << "Found ladder: ";
    for (const auto& ladder : ladders) {
      for (const auto& word : ladder) {
        std::cout << word << ' ';
      }
      std::cout << '\n';
    }
  }
  std::cout << std::flush;
}
