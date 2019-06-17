#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <forward_list>
#include <vector>
#include <queue>
#include <stack>
#include <deque>
#include <algorithm>
#include <numeric>

using namespace std;

/*
 * return filtered set of words that is not as same length as the given word
 * */
unordered_set<string> filterDissimilarWords(const unordered_set<string> &words, const string &word);

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
 * return the word ladders in vector, size can be 0 when no ladder exists
 * */
vector<vector<string>> computeLadder(const unordered_set<string> &words, const string &from, const string &to);

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
