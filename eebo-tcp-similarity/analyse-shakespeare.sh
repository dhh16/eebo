#!/bin/sh

mkdir html-shakespeare
cp html/style.css html-shakespeare/
./analyse output/results-shakespeare.xml output/raw-128.txt Shakespeare < eebo-tcp.txt
./analyse.py html-shakespeare output/results-shakespeare.xml
