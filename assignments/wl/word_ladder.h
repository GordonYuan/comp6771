#ifndef ASSIGNMENTS_WL_WORD_LADDER_H_
#define ASSIGNMENTS_WL_WORD_LADDER_H_

#include <unordered_map>
#include <unordered_set>
#include <string>
#include <vector>

using namespace std;

/*
 * return filtered set of words that is not as same length as the given word
 * */
unordered_set<string> filterDissimilarWords(const unordered_set<string>& words, const string& word);

/*
 * return the unordered_set of words that is one distance to the given word
 * */
unordered_set<string> getWordMap(const unordered_set<string>& words, const string& word);

/*
 * return a map of all words that map to the unordered_set of one distance words
 * */
unordered_map<string, unordered_set<string>> getWordMapAll();

/*
 * return the word ladder if any, null otherwise
 * */
vector<string> computeLadder(unordered_set<string> words, string from, string to);

#endif  // ASSIGNMENTS_WL_WORD_LADDER_H_
