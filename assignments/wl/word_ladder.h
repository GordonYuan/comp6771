#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <forward_list>
#include <vector>
#include <queue>
#include <algorithm>

using namespace std;

/*
 * return filtered set of words that is not as same length as the given word
 * */
static unordered_set<string> filterDissimilarWords(const unordered_set<string> &words, const string &word);

/*
 * return the unordered_set of words that is one distance to the given word
 * for performance, it is assumed that all word in words have same size as the given word
 * */
static unordered_set<string> getWordMap(const unordered_set<string> &words, const string &word);

/*
 * return a map of all words that map to the unordered_set of one distance words
 * for performance, it is assumed that all word in words have same size
 * */
static unordered_map<string, unordered_set<string>> getWordMapAll(const unordered_set<string> &words);

/*
 * return the word ladders in sorted vector, size can be 0 when no ladder exists
 * */
vector<vector<string>> computeLadder(const unordered_set<string> &words, const string &from, const string &to);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
