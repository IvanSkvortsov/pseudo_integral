CXX=g++ -std=c++11
CXXFLAGS=-I ../memorystream -I ../matrix -I ../math -I .. -I ../omega -I ../vector_3d_ -I ../poly_n -I ../array -I ../mmap -I ../angular
math_lib=-L../math -lmath
libraries=-lgmp -lmpfr
matrix_slm_obj=../omega/matrix.slm.o ../poly_n/poly.n.o ../array/array.n.o
vector_3d_obj=../vector_3d_/vector.3d.o
memstr_obj=../memorystream/memorystream.o
memmap_obj=../mmap/memory.map.o
objects=alpha.map.o alpha.val.o alpha.pow.o
execute=alpha.map.exe alpha.val.exe alpha.pow.exe alpha.slm.exe alpha.mem.exe

all: $(objects) alpha.slm.o alpha.mem.o

libalpha.a: $(objects) alpha.slm.o alpha.mem.o
	ar cr $@ $(objects) alpha.slm.o alpha.mem.o

alpha.mem.o: alpha.mem.h alpha.mem.hpp alpha.mem.cpp alpha.pow.o
	$(CXX) $(CXXFLAGS) -c -o $@ alpha.mem.cpp 

alpha.mem.exe: alpha.mem.demo.h alpha.mem.main.cpp alpha.mem.o
	$(CXX) $(CXXFLAGS) -o $@ alpha.mem.main.cpp alpha.mem.o $(objects) $(memmap_obj) $(memstr_obj) $(vector_3d_obj) $(math_lib) $(libraries)

alpha.slm.o: alpha.slm.h alpha.slm.hpp alpha.slm.cpp alpha.pow.o
	$(CXX) $(CXXFLAGS) -c -o $@ alpha.slm.cpp

alpha.slm.exe: alpha.slm.demo.h alpha.slm.main.cpp alpha.slm.o
	$(CXX) $(CXXFLAGS) -o $@ alpha.slm.main.cpp $(objects) alpha.slm.o $(memstr_obj) $(matrix_slm_obj) $(vector_3d_obj) $(math_lib) $(libraries)

alpha.pow.o: alpha.pow.h alpha.pow.hpp alpha.pow.cpp alpha.val.o
	$(CXX) $(CXXFLAGS) -c -o $@ alpha.pow.cpp

alpha.pow.exe: alpha.pow.demo.h alpha.pow.main.cpp alpha.pow.o
	$(CXX) $(CXXFLAGS) -o $@ alpha.pow.main.cpp $(objects) $(memstr_obj) $(vector_3d_obj) $(math_lib) $(libraries)

alpha.val.o: alpha.val.h alpha.val.hpp alpha.val.cpp alpha.map.o
	$(CXX) $(CXXFLAGS) -c -o $@ alpha.val.cpp

alpha.val.exe: alpha.val.demo.h alpha.val.main.cpp alpha.val.o
	$(CXX) $(CXXFLAGS) -o $@ alpha.val.main.cpp $(objects) $(memstr_obj) $(vector_3d_obj) $(math_lib) $(libraries)

alpha.map.o: alpha.map.h alpha.map.hpp alpha.map.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ alpha.map.cpp

alpha.map.exe: alpha.map.o alpha.map.demo.h alpha.map.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ alpha.map.main.cpp alpha.map.o $(memstr_obj)

clean:
	rm -f *.o
