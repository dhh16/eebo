#!/bin/sh

if [ "$#" -ne 4 ]; then
  echo "invalid arguments" >&2
  exit 1
fi

t=$1
o=$2
h=$3
s=$4

rm -rf $h
mkdir -p $h
cp style.css $h/
./analyse $o/results.xml $o/raw-$s.txt < $t || exit 1
./analyse.py $h $o/results.xml || exit 1
