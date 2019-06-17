#include <iostream>
#include <chrono>

#include "lexicon.h"
#include "word_ladder.h"

using namespace std;

int main() {
//    auto start = chrono::steady_clock::now();

    auto lexicon = GetLexicon("assignments/wl/words.txt");

    string from, to;

//    from = "con";
//    to =   "cat";
//    from = "code";
//    to =   "data";
//    from = "great";
//    to =   "lucky";
//    from = "michael";
//    to =   "jackson";
//    from = "prepares";
//    to =   "contorts";
//    from = "dauntlessness";
//    to =   "dastardliness";

    cout << "Enter start word (RETURN to quit): ";
    cin >> from;
    if (from == "RETURN") {
        return 0;
    }
    cout << "Enter destination word: ";
    cin >> to;

    auto ladders = computeLadder(lexicon, from, to);
    sortLadders(ladders);
    printLadders(ladders);


//    auto end = chrono::steady_clock::now();
//    cout << "Elapsed time in milliseconds : "
//         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
//         << " ms" << endl;
//
//    cout << "Elapsed time in seconds : "
//         << chrono::duration_cast<chrono::seconds>(end - start).count()
//         << " sec";
    return 0;
}
