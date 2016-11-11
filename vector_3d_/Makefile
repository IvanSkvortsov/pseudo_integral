CXX=g++ -std=c++11
CXXFLAGS=-I ../
objects=vector.3d.o
.PHONY: clean, all

all: $(objects)

vector.3d.o: vector.3d.h vector.3d.hpp vector.3d.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ vector.3d.cpp

vector.3d.exe: vector.3d.o vector.3d.demo.h vector.3d.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ vector.3d.main.cpp vector.3d.o -lgmp -lmpfr

clean:
	rm -f *.o
