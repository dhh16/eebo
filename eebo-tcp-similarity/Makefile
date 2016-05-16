LINK.o=$(LINK.cc)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wno-sign-compare -O3 -fopenmp
CPPFLAGS=-Ipugixml/src -Iutfcpp/source
LDFLAGS=-fopenmp

all: similarity analyse

similarity: similarity.o pugixml.o

analyse: analyse.o pugixml.o

clean:
	rm -f analyse similarity *.o
