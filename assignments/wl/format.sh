#!/usr/bin/env bash

cd ../../

ASSIGNMENT="assignments/wl/"

for FILE in "word_ladder_test.cpp" "main.cpp" "word_ladder.h" "word_ladder_test.cpp" "word_ladder.cpp"
do
    FULL_PATH=${ASSIGNMENT}${FILE}
    clang-format -i ${FULL_PATH} && python2 cpplint.py ${FULL_PATH}
done