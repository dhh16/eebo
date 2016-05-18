#!/bin/sh

if [ "$#" -ne 2 ]; then
  echo "invalid arguments" >&2
  exit 1
fi

t=$1
o=$2

rm -rf $o
mkdir -p $o
cat $t | ./similarity $o
