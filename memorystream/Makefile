CXX=g++
CXXFLAGS=-std=c++11
objects=memorystream.o
.PHONY: clean, all

all: $(objects)

memorystream.o: memorystream.h memorystream.hpp memorystream.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ memorystream.cpp

memorystream.exe: memorystream.o memorystream.demo.h memorystream.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ memorystream.main.cpp memorystream.o

libmemorystream.a: memorystream.o
	ar cr $@ memorystream.o

clean:
	-rm -f *.o
