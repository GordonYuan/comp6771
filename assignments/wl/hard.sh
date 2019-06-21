#!/usr/bin/env bash
cd ../../
bazel build //assignments/wl:main
time bazel run //assignments/wl:main < assignments/wl/sh_test/input/atlases_cabaret