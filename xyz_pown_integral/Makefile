CXX=g++ -std=c++11
CXXFLAGS=-I ../math  -I ..
libraries=-L../math -lsfmath -lgmp -lmpfr
objects=xyz.pown.integral.o
.PHONY: clean, all

all: $(objects)

xyz.pown.integral.o: xyz.pown.integral.h xyz.pown.integral.hpp xyz.pown.integral.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ xyz.pown.integral.cpp

xyz.pown.integral.exe: xyz.pown.integral.o xyz.pown.integral.demo.h xyz.pown.integral.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ xyz.pown.integral.main.cpp xyz.pown.integral.o $(libraries)

clean:
	rm -f *.o
