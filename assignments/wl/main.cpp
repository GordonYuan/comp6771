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
    auto ladders = computeLadder(lexicon, from, to);
    for (const auto &ladder : ladders) {
        cout  << endl << "size: " << ladder.size() << endl;
        for (const auto &word : ladder) {
            cout << word << " ";
        }
        std::cout << '\n';
    }

    cout << "finish" << endl;


    auto end = chrono::steady_clock::now();
    cout << "Elapsed time in nanoseconds : "
          << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
          << " ns" << endl;

    cout << "Elapsed time in microseconds : "
         << chrono::duration_cast<chrono::microseconds>(end - start).count()
         << " µs" << endl;

    cout << "Elapsed time in milliseconds : "
         << chrono::duration_cast<chrono::milliseconds>(end - start).count()
         << " ms" << endl;

    cout << "Elapsed time in seconds : "
         << chrono::duration_cast<chrono::seconds>(end - start).count()
         << " sec";
    return 0;
}
