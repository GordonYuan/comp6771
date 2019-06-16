#include <iostream>
#include <chrono>

#include "assignments/wl/lexicon.h"
#include "assignments/wl/word_ladder.h"

using namespace std;

int main() {
    auto start = chrono::steady_clock::now();

    auto lexicon = GetLexicon("assignments/wl/words.txt");
    cout << "data read" << endl;

    auto from = "dauntlessness";
    auto to =   "dastardliness";
    from = "con";
    to =   "cat";
    from = "code";
    to =   "data";
    from = "great";
    to =   "lucky";
//    from = "michael";
//    to =   "jackson";
//    from = "prepares";
//    to =   "contorts";
//    from = "dauntlessness";
//    to =   "dastardliness";
    auto ladders = computeLadder(lexicon, from, to);
    for (const auto &ladder : ladders) {
        cout << "ladder: ";
        for (const auto &word : ladder) {
            cout << word << " ";
        }
        std::cout << '\n';
    }

    cout << "finish" << endl;


    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in milliseconds : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    cout << "Elapsed time in seconds : "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec";
    return 0;
}
