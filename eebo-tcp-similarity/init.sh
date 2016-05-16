#!/bin/sh

cd ../eebo-tcp || exit 1
./list-eebo-tcp > ../eebo-tcp-similarity/eebo-tcp.txt
./list-eebo-tcp 1642 1651 2 24 > ../eebo-tcp-similarity/eebo-tcp-1642-1651.txt
