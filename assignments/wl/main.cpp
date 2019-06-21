#include <iostream>
#include <string>
#include <unordered_set>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

int main() {
  std::unordered_set<std::string> lexicon = GetLexicon("assignments/wl/words.txt");

  // read inputs
  std::string from, to;
  std::cout << "Enter start word (RETURN to quit): ";
  getline(std::cin, from);
  if (from.empty()) {
    return 0;
  }
  std::cout << "Enter destination word: ";
  getline(std::cin, to);

  // calculate and output result
  auto ladders = computeLadder(lexicon, from, to);
  printLadders(ladders);

  return 0;
}
