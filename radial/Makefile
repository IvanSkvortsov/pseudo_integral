CXX=g++ -std=c++11
CXXFLAGS=-I .. -I ../mmap -I ../memorystream -I ../math -I ../matrix -I ../alpha -I ../hyperg
libraries=-L../hyperg -lhyperg -L../math -lmath -lgmp -lmpfr
memory_obj=../memorystream/memorystream.o ../mmap/memory.map.o
alpha_map_obj=../alpha/alpha.map.o
lib_hyperg=../hyperg/libhyperg.a
qu_radial_obj=qu.radial.map.o qu.radial.dat.o
qu_struct_h=qu.hyperg.1F11.struct.h qu.hyperg.1F1.struct.h
.PHONY: clean, all

all: libradial.a

libradial.a: $(qu_radial_obj)
	ar cr $@ $(qu_radial_obj)

qu.hyperg.exe: qu.hyperg.1F11.struct.h qu.hyperg.demo.h qu.hyperg.main.cpp $(lib_hyperg)
	$(CXX) $(CXXFLAGS) -o $@ qu.hyperg.main.cpp $(libraries)

qu.radial.dat.o: qu.radial.dat.h qu.radial.dat.hpp qu.radial.dat.cpp qu.radial.map.o $(qu_struct_h)
	$(CXX) $(CXXFLAGS) -c -o $@ qu.radial.dat.cpp

qu.radial.dat.exe: $(qu_radial_obj) qu.radial.dat.demo.h qu.radial.dat.main.cpp qu.radial.mem.h $(memory_obj) $(alpha_map_obj)
	$(CXX) $(CXXFLAGS) -o $@ qu.radial.dat.main.cpp $(qu_radial_obj) $(alpha_map_obj) $(memory_obj) $(libraries)

qu.radial.map.o: qu.radial.map.h qu.radial.map.hpp qu.radial.map.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ qu.radial.map.cpp

qu.radial.map.exe: qu.radial.map.o qu.radial.map.demo.h qu.radial.map.main.cpp qu.radial.mem.h
	$(CXX) $(CXXFLAGS) -o $@ qu.radial.map.main.cpp qu.radial.map.o $(memory_obj) $(libraries)

libqu.a: $(qu_radial_obj)
	ar cr $@ $(qu_radial_obj)

clean:
	rm -f *.o
