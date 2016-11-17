
CXXFLAGS=-std=c++11 -I array -I ../math -I ../xyz_pown_integral -I .. -I ../mmap
objects=polynomial.o orthogonal.o legendre.o spherical.o
libraries=-L../poly_n -lpoly -L../array -larray -L../math -lmath -lgmp -lmpfr
.PHONY: all, clean

all: $(objects)

legendre.o: legendre.h legendre.hpp legendre.cpp orthogonal.h poly.n.h polynomial.h
	g++ $(CXXFLAGS) -c -o $@ legendre.cpp $(libraries)

spherical.o: spherical.h spherical.hpp spherical.cpp legendre.h orthogonal.h poly.n.h polynomial.h array.scp.h
	g++ $(CXXFLAGS) -c -o $@ spherical.cpp $(libraries)

orthogonal.o: orthogonal.h orthogonal.hpp orthogonal.cpp polynomial.h array.scp.h
	g++ $(CXXFLAGS) -c -o $@ orthogonal.cpp $(libraries)

polynomial.o: polynomial.h polynomial.hpp polynomial.cpp
	g++ $(CXXFLAGS) -c -o $@ polynomial.cpp $(libraries)

libspherical.a: $(objects)
	ar cr $@ $(objects)

polynomial.exe0: polynomial.demo.h polynomial.main.cpp
	g++ $(CXXFLAGS) -o $@ polynomial.main.cpp $(libraries)

polynomial.exe: polynomial.demo.h polynomial.main.cpp
	g++ $(CXXFLAGS) -o $@ polynomial.main.cpp polynomial.o $(libraries)

legendre.exe: legendre.demo.h legendre.main.cpp $(objects)
	g++ $(CXXFLAGS) -o $@ legendre.main.cpp $(objects) ../mmap/memory.map.o $(libraries)
	#g++ $(CXXFLAGS) -o $@ legendre.main.cpp legendre.o orthogonal.o polynomial.o poly.n.o spherical.o -L. -larray $(libraries)

x.spherical.o: x.spherical.h x.spherical.hpp x.spherical.cpp x.spherical.dbl.data.h x.spherical.ldbl.data.h
	g++ $(CXXFLAGS) -c -o $@ x.spherical.cpp $(libraries)

r.spherical.o: r.spherical.h r.spherical.hpp r.spherical.cpp x.spherical.h
	g++ $(CXXFLAGS) -c -o $@ r.spherical.cpp $(libraries)

sh.vector.o: sh.vector.h sh.vector.hpp sh.vector.cpp x.spherical.h
	g++ $(CXXFLAGS) -c -o $@ sh.vector.cpp $(libraries)


r.spherical.exe: r.spherical.o x.spherical.o r.spherical.demo.h r.spherical.main.cpp
	g++ $(CXXFLAGS) -o $@ r.spherical.main.cpp r.spherical.o x.spherical.o ../mmap/memory.map.o $(libraries)

clean:
	rm *.o

