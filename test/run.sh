#!/bin/bash

i=1
while IFS=, read -r input pattern expect
do
  echo -n "test $i: "
  echo $input | sed 's/\\n/\n/g' > test/input
  echo $expect | sed 's/\\n/\n/g' > test/expected
  ./bisect test/input $pattern > test/got
  res=`diff -u test/expected test/got`
  if [ $? == 0 ]; then
    echo -e "\e[92m[ OK ]\e[0m"
  else
    echo -e "\e[91m[ FAIL ]\e[0m pattern: '$pattern' \ninput:\n$input\ndiff:\n$res"
  fi
  i=$((i+1))
done < test/data.txt

rm -f test/input test/expected test/got