#!/usr/bin/env bash

cd ../../

ASSIGNMENT="assignments/dg/"

for FILE in "graph.h" "graph.tpp" "graph_test.cpp"
do
    FULL_PATH=${ASSIGNMENT}${FILE}
    clang-format -i ${FULL_PATH} && python2 cpplint.py ${FULL_PATH}
done