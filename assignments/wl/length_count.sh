#!/usr/bin/env bash
for i in {1..25}; do
    printf "%2d" $i;
    cat words.txt | grep -x '.\{'${i}'\}' | wc -l;
done