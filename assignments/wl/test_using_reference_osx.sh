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
    printf ${NAME}
    printf " ref "
    local START=`python -c'import time; print(repr(time.time()))'`
    ./${A1}/reference_solution_osx < ${INPUT} > ${OUTPUT_REF}
    local END=`python -c'import time; print(repr(time.time()))'`
    local REF_RUNTIME=$( echo "${END} - ${START}" | bc -l )
    printf ${REF_RUNTIME}

    # run my solution
    printf " mine "
    local START=`python -c'import time; print(repr(time.time()))'`
    ./bazel-bin/assignments/wl/main < ${INPUT} > ${OUTPUT_MINE}
    local END=`python -c'import time; print(repr(time.time()))'`
    local MINE_RUNTIME=$( echo "${END} - ${START}" | bc -l )
    printf ${MINE_RUNTIME}

    local MULTIPLE=$( echo "${MINE_RUNTIME} / ${REF_RUNTIME}" | bc -l )
    printf ${MULTIPLE}
    local EXCEED=$( echo "${MULTIPLE} > 10" | bc -l )
    if [[ ${EXCEED} == '1' ]]; then
        printf "\tEXCEED"
        echo ${MULTIPLE} " " ${NAME} " my:" ${MINE_RUNTIME} " ref:" ${REF_RUNTIME}  >> ${A1}/sh_test/log/time_exceed.txt
    else
        echo ${MULTIPLE} " " ${NAME} " my:" ${MINE_RUNTIME} " ref:" ${REF_RUNTIME}  >> ${A1}/sh_test/log/time.txt
    fi

    # show difference
    diff ${OUTPUT_REF} ${OUTPUT_MINE} --ignore-all-space

    # print if no diff
    if [[ $? -ne 0 ]]; then
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
    if [[ ${FROM} != ${TO} ]]; then
        diff_test ${FROM} ${TO}
    fi
}

for NUMBER in {1..100}
do
    for LENGTH in {2..22}
    do
        diff_test_length ${LENGTH}
    done
done

# test hardest
#diff_test atlases cabaret