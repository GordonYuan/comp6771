#!/usr/bin/env bash

A1="assignments/wl"
INPUT_PATH=${A1}"/sh_test/input"

# go to /assignment, for using reference_solution_osx which requires the path
cd ../../

# compile my solution
g++ ${A1}/main.cpp ${A1}/lexicon.cpp ${A1}/word_ladder.cpp -o ${A1}/my_solution_osx -O2 -std=c++17

# test with two words
function diff_test() {
    local WORD1=$1
    local WORD2=$2
    local NAME=${WORD1}_${WORD2}

    # make the input file
    echo ${WORD1} > ${INPUT_PATH}/${NAME}
    echo ${WORD2} >> ${INPUT_PATH}/${NAME}

    # calculate IO file path
    local INPUT=${INPUT_PATH}/${NAME}
    local OUTPUT_REF=${A1}"/sh_test/output/reference/"${NAME}
    local OUTPUT_MINE=${A1}"/sh_test/output/mine/"${NAME}

    # run given solution
    ./${A1}/reference_solution_osx < ${INPUT} > ${OUTPUT_REF}

    # run my solution
    ./${A1}/my_solution_osx < ${INPUT} > ${OUTPUT_MINE}

    # show difference
    diff ${OUTPUT_REF} ${OUTPUT_MINE} --ignore-all-space
}


diff_test code word

