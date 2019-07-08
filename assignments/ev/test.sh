#!/usr/bin/env bash

cd ../../

bazel build //assignments/ev:client
bazel run //assignments/ev:client > assignments/ev/client.myout

diff assignments/ev/client.myout assignments/ev/client.sampleout --side

if [[ $? == 0 ]]
then
    echo pass
else
    echo fail
fi
