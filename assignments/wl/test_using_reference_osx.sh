#!/usr/bin/env bash

A1="assignments/wl"
INPUT_PATH=${A1}"/sh_test/input"

# go to /assignment, for using reference_solution_osx which requires the path
cd ../../

# compile my solution
bazel build //assignments/wl:main

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
    echo "Running " ${NAME} " with reference_solution_osx"
    ./${A1}/reference_solution_osx < ${INPUT} > ${OUTPUT_REF}

    # run my solution
    echo "Running " ${NAME} " with my_solution_osx"
    ./bazel-bin/assignments/wl/main < ${INPUT} > ${OUTPUT_MINE}

    # show difference
    diff ${OUTPUT_REF} ${OUTPUT_MINE} --ignore-all-space

    # print if no diff
    if [[ $? -eq 0 ]]; then
        echo "No difference";
    else
        echo ${NAME} >> ${A1}/sh_test/log/difference.txt
    fi
    echo
}


# run diff_test with given word length
# words will be picked randomly
function diff_test_length() {
    # generate words with exactly same length as given
    local LENGTH=$1
    cat ${A1}/words.txt | grep -x '.\{'${LENGTH}'\}' > ${A1}/sh_test/temp/temp.txt

    # generate from and to randomly
    local FROM=$(shuf -n 1 ${A1}/sh_test/temp/temp.txt)
    local TO=$(shuf -n 1 ${A1}/sh_test/temp/temp.txt)

    # call diff_test
    diff_test ${FROM} ${TO}
}

for NUMBER in {1..100}
do
    for LENGTH in {2..22}
    do
        diff_test_length ${LENGTH}
    done
done