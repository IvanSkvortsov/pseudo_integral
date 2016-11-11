CXX=g++ -std=c++11
CXXFLAGS=-I ..  -I ../poly_n -I ../spherical
objects=array.n.o array.scp.o
libraries=-lgmp -lmpfr

all: $(objects)

array.n.o: array.n.h array.n.hpp array.n.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ array.n.cpp $(libraries)

array.scp.o: array.scp.h array.scp.hpp array.scp.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ array.scp.cpp $(libraries)

libarray.a: $(objects)
	ar cr $@ $(objects)

clean:
	rm *.o
