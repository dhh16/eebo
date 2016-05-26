LINK.o=$(LINK.cc)
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wno-sign-compare -O3 -fopenmp
CPPFLAGS=-Ipugixml/src -Iutfcpp/source
LDFLAGS=-fopenmp

all: similarity analyse metadata normalised

similarity: similarity.o pugixml.o

analyse: analyse.o pugixml.o

metadata: metadata.o pugixml.o

normalised: normalised.o pugixml.o

similarity.o: similarity.cc char-map.h parser.h timer.h

analyse.o: analyse.cc char-map.h parser.h timer.h

metadata.o: metadata.cc char-map.h parser.h

normalised.o: normalised.cc char-map.h parser.h

clean:
	rm -f analyse similarity *.o
