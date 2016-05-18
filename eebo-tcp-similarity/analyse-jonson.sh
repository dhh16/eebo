#!/bin/sh

mkdir -p html-jonson
cp html/style.css html-jonson/
./analyse output/results-jonson.xml output/raw-128.txt "Jonson, Ben" < eebo-tcp.txt
./analyse.py html-jonson output/results-jonson.xml
