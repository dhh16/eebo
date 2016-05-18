#!/bin/sh

rm -rf html-same
mkdir -p html-same
cp style.css html-same/
./analyse output/results-same.xml output/raw-256.txt --same-text < eebo-tcp.txt
./analyse.py html-same output/results-same.xml
rm -f output/results-same.xml
