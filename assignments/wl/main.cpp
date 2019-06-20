#include <iostream>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

using namespace std;

int main() {
    auto lexicon = GetLexicon("assignments/wl/words.txt");

    // read inputs
    string from, to;
    cout << "Enter start word (RETURN to quit): ";
    getline(cin, from);
    if (from.empty()) {
        return 0;
    }
    cout << "Enter destination word: ";
    getline(cin, to);

    // calculate and output result
    auto ladders = computeLadder(lexicon, from, to);
    sortLadders(ladders);
    printLadders(ladders);

    return 0;
}
