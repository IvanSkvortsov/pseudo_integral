CXX=g++ -std=c++11
CXXFLAGS=-I .. -I ../mmap -I ../memorystream
objects=../mmap/memory.map.o ../memorystream/memorystream.o
libraries=-lgmp -lmpfr
.PHONY: clean

matrix.cursor.o: matrix.cursor.h matrix.cursor.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.cursor.cpp

matrix.cursor.exe: matrix.cursor.o matrix.cursor.demo.h matrix.cursor.main.cpp $(objects)
	$(CXX) $(CXXFLAGS) -o $@ matrix.cursor.main.cpp matrix.cursor.o $(objects) $(libraries)

libomega.a: $(ixs_omega_obj)
	ar cr $@ $(ixs_omega_obj)

clean:
	rm *.o
