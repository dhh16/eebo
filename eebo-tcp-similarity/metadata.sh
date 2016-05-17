#!/bin/sh

./metadata < eebo-tcp.txt > output-metadata/metadata.tsv || exit 1
./metadata.py || exit 1
