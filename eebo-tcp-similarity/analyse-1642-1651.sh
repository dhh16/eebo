#!/bin/sh

mkdir html-1642-1651
cp html/style.css html-1642-1651/
./analyse output-1642-1651/results.xml output-1642-1651/raw-64.txt < eebo-tcp-1642-1651.txt
./analyse.py html-1642-1651 output-1642-1651/results.xml
