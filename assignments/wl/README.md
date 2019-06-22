# wl (Word Ladder)

This folder is the assignment 1 of COMP6771 at UNSW, trimester 2.  


# Specification
Specification is at [COMP6771-Advanced-C++Programming-19T2.pdf](COMP6771-Advanced-C++Programming-19T2.pdf).

# Files
- [main.cpp](main.cpp) is the driver file, [lexicon.cpp](lexicon.cpp) is provided and read all the words from [words.txt](words.txt), [word_ladder.cpp](word_ladder.cpp) is my actual solution, [word_ladder_test.cpp](word_ladder_test.cpp) contains some test cases using catch2.
- [format.sh](format.sh) runs `clang-format` and `cpplint` for all non-provided sourse and header files. 
- [hard.sh](hard.sh) times the solution which has the longest output ladders (atlases -> cabaret) I found so far.
- [length_count.sh](length_count.sh) outputs the number of words for each of length between 2 - 25.
- [test_using_reference_osx.sh](test_using_reference_osx.sh) times and tests my codes in comparison to the given reference solution. Results will be under [sh_test](sh_test) folder, [sh_test/input](sh_test/input) stores random input files generated during the script, [sh_test/log](sh_test/log) records the log ([difference.txt](sh_test/log/difference.txt) logs incorrect test cases, [time_exceed.txt](sh_test/log/time_exceed.txt) logs the test cases that ran 10x longer than reference solution, [time.txt](sh_test/log/time.txt) logs the test cases that ran within 10x bound of reference solution) [sh_test/output](sh_test/output) logs the output of both reference and my program. [sh_test/temp](sh_test/temp) is just rubbish temp file.


# Efficiency 
My solution is a little bit verbose (300+ lines), but it works. Running [test_using_reference_osx.sh](test_using_reference_osx.sh) which examines correctness and time efficiency in comparison to [reference_solution](reference_solution) which runs on Linux or [reference_solution_osx](reference_solution_osx) which runs on macOS, most of the random cases are faster than the reference solution. And 99% of the cases are done within 10x times of reference solution.
 
 
# Related Work
- [Word Ladder 2](https://leetcode.com/problems/word-ladder-ii/) is a LeetCode question that is exactly same to the assignment question.
- [88ms Word Ladder 2 solution C++](https://leetcode.com/problems/word-ladder-ii/discuss/40540/88ms-accepted-c-solution-with-two-end-bfs-68ms-for-word-ladder-and-88ms-for-word-ladder-ii) is a fast solution of the LeetCode solution in C++.