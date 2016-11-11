
CXXFLAGS=-std=c++11 -I array -I ../math -I ../xyz_pown_integral -I .. -I ../mmap
OBJECTS=poly.n.o polynomial.o orthogonal.o legendre.o spherical.o array.n.o array.scp.o x.spherical.o r.spherical.o
LIBRARIES=-L../math -lsfmath -lgmp -lmpfr
.PHONY: all, clean

all: $(OBJECTS)

poly.n.o: poly.n.h poly.n.hpp poly.n.cpp ./array/array.n.h
	g++ $(CXXFLAGS) -c -o $@ poly.n.cpp

legendre.o: legendre.h legendre.hpp legendre.cpp orthogonal.h poly.n.h polynomial.h ./array/array.scp.h
	g++ $(CXXFLAGS) -c -o $@ legendre.cpp $(LIBRARIES)

spherical.o: spherical.h spherical.hpp spherical.cpp legendre.h orthogonal.h poly.n.h polynomial.h ./array/array.scp.h
	g++ $(CXXFLAGS) -c -o $@ spherical.cpp $(LIBRARIES)

orthogonal.o: orthogonal.h orthogonal.hpp orthogonal.cpp polynomial.h ./array/array.scp.h
	g++ $(CXXFLAGS) -c -o $@ orthogonal.cpp $(LIBRARIES)

polynomial.o: polynomial.h polynomial.hpp polynomial.cpp poly.n.h ./array/array.scp.h ./array/array.n.h
	g++ $(CXXFLAGS) -c -o $@ polynomial.cpp $(LIBRARIES)

array.n.o:
	cp ./array/$@ ./

array.scp.o:
	cp ./array/$@ ./

libarray.a:
	cp ./array/$@ ./

libspherical.a: $(OBJECTS)
	ar cr $@ $(OBJECTS)

polynomial.exe0: polynomial.demo.h polynomial.main.cpp poly.n.o array.n.o
	g++ $(CXXFLAGS) -o $@ polynomial.main.cpp array.n.o poly.n.o $(LIBRARIES)

polynomial.exe: polynomial.demo.h polynomial.main.cpp polynomial.o poly.n.o libarray.a
	g++ $(CXXFLAGS) -o $@ polynomial.main.cpp polynomial.o poly.n.o -L. -larray $(LIBRARIES)

legendre.exe: legendre.demo.h legendre.main.cpp $(OBJECTS) libarray.a
	g++ $(CXXFLAGS) -o $@ legendre.main.cpp $(OBJECTS) ../mmap/memory.map.o $(LIBRARIES)
	#g++ $(CXXFLAGS) -o $@ legendre.main.cpp legendre.o orthogonal.o polynomial.o poly.n.o spherical.o -L. -larray $(LIBRARIES)

x.spherical.o: x.spherical.h x.spherical.hpp x.spherical.cpp x.spherical.dbl.data.h x.spherical.ldbl.data.h
	g++ $(CXXFLAGS) -c -o $@ x.spherical.cpp $(LIBRARIES)

r.spherical.o: r.spherical.h r.spherical.hpp r.spherical.cpp x.spherical.h
	g++ $(CXXFLAGS) -c -o $@ r.spherical.cpp $(LIBRARIES)

sh.vector.o: sh.vector.h sh.vector.hpp sh.vector.cpp x.spherical.h
	g++ $(CXXFLAGS) -c -o $@ sh.vector.cpp $(LIBRARIES)


r.spherical.exe: r.spherical.o x.spherical.o r.spherical.demo.h r.spherical.main.cpp
	g++ $(CXXFLAGS) -o $@ r.spherical.main.cpp r.spherical.o x.spherical.o ../mmap/memory.map.o $(LIBRARIES)

clean:
	rm *.o

