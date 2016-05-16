#!/bin/sh

mkdir -p html-1599
cp html/style.css html-1599/
./analyse output-1599/results.xml output-1599/raw-256.txt < eebo-tcp-1599.txt
./analyse.py html-1599 output-1599/results.xml
