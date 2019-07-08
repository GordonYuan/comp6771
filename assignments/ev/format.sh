#!/usr/bin/env bash

cd ../../

ASSIGNMENT="assignments/ev/"

for FILE in "euclidean_vector.cpp" "euclidean_vector.h" "euclidean_vector_test.cpp"
do
    FULL_PATH=${ASSIGNMENT}${FILE}
    clang-format -i ${FULL_PATH} && python2 cpplint.py ${FULL_PATH}
done