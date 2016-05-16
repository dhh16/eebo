#!/bin/sh

mkdir html-shakespeare
cp html/style.css html-shakespeare/
./analyse output-shakespeare/results.xml output/raw-256.txt Shakespeare < eebo-tcp.txt
./analyse.py html-shakespeare output-shakespeare/results.xml
