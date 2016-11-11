CXX=g++
CXXFLAGS=-std=c++11 -I ../array -I ../math -I ../
objects=poly.n.o
libraries=-L../math -lsfmath -lgmp -lmpfr
cxxfiles=../array/array.n.h ../mpreal.use.h ../mpreal.h
.PHONY: all, clean

all: $(objects)

poly.n.o: poly.n.h poly.n.hpp poly.n.cpp $(cxxfiles)
	$(CXX) $(CXXFLAGS) -c -o $@ poly.n.cpp

poly.n.exe: poly.n.o poly.n.demo.h poly.n.main.cpp $(cxxfiles)
	$(CXX) $(CXXFLAGS) -o $@ poly.n.main.cpp poly.n.o ../array/array.n.o $(libraries)

clean:
	rm *.o

