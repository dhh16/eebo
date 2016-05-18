#!/bin/sh

rm -rf html-shakespeare
mkdir -p html-shakespeare
cp style.css html-shakespeare/
./analyse output/results-shakespeare.xml output/raw-128.txt Shakespeare < eebo-tcp.txt
./analyse.py html-shakespeare output/results-shakespeare.xml
rm -f output/results-shakespeare.xml
