#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <unordered_map>
#include <map>
#include <unordered_set>
#include <string>
#include <forward_list>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <numeric>
#include <utility>

using namespace std;

/*
 * return the word ladders in vector, size can be 0 when no ladder exists
 * */
vector<vector<string>> computeLadder(unordered_set<string> &words, const string &from, const string &to);

/*
 * sort ladders in their lexicographic order
 */
void sortLadders(vector<vector<string>> &ladders);

/*
 * print ladders to stdout if there are ladders found
 * print Each solution appears on a separate line. The very first one is printed after "Found ladder: "
 * if no ladders found, print No ladder found.
 */
void printLadders(const vector<vector<string>> &ladders);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
