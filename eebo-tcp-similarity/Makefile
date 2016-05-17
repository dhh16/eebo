LINK.o=$(LINK.cc)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wno-sign-compare -O3 -fopenmp
CPPFLAGS=-Ipugixml/src -Iutfcpp/source
LDFLAGS=-fopenmp

all: similarity analyse metadata

similarity: similarity.o pugixml.o

analyse: analyse.o pugixml.o

metadata: metadata.o pugixml.o

similarity.o: similarity.cc char-map.h timer.h

analyse.o: analyse.cc char-map.h timer.h

metadata.o: metadata.cc

clean:
	rm -f analyse similarity *.o
