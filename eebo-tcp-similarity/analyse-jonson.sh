#!/bin/sh

rm -rf html-jonson
mkdir -p html-jonson
cp style.css html-jonson/
./analyse output/results-jonson.xml output/raw-128.txt "Jonson, Ben" < eebo-tcp.txt
./analyse.py html-jonson output/results-jonson.xml
rm -f output/results-jonson.xml
