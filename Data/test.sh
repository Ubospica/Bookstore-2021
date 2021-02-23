#!/usr/bin/env bash

./clean.sh

for j in $(seq 1 5)
do
  ./clean.sh
  echo case$j:
  for i in $(seq 1 10)
  do
    echo testing $i.out
  #  ../cmake-build-debug/Bookstore_2021 < AdvancedDataSet/testcase1/$i.in > a.out 2> a.err
  #  diff a.out AdvancedDataSet/testcase1/$i.out
    ../cmake-build-debug/Bookstore_2021 < AdvancedDataSet/testcase$j/$i.in > a.out 2> a.err
    diff a.out AdvancedDataSet/testcase$j/$i.out -s
  #  read -n 1
  done
done
