#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <unordered_map>
#include <set>
#include <string>
#include <vector>

using namespace std;

/*
 * return filtered set of words that is not as same length as the given word
 * */
set<string> filterDissimilarWords(set<string> words, string word);

/*
 * return the set of words that is one distance to the given word
 * */
set<string> getWordMap(set<string> words, string word);

/*
 * return a map of all words that map to the set of one distance words
 * */
unordered_map<string, set<string>> getWordMapAll();

/*
 * return the word ladder if any, null otherwise
 * */
vector<string> computeLadder(set<string> words, string from, string to);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
