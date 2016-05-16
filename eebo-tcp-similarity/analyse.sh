#!/bin/sh

./analyse output/results.xml output/raw-1024.txt < eebo-tcp.txt
./analyse.py html output/results.xml
