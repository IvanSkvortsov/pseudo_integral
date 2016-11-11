CXX=g++ -std=c++11
CXXFLAGS=-I ../xyz_pown_integral -I .. -I ../mmap -I ../math -I ../memorystream -I ../poly_n -I ../array -I ../matrix -I ../integral -I ../spherical

memory_man_obj=../memorystream/memorystream.o ../mmap/memory.map.o
poly_n_obj=../poly_n/poly.n.o ../array/array.n.o
xpn_obj=../xyz_pown_integral/xyz.pown.integral.o
lib_math=../math/libmath.a

mx_cursor_obj=../matrix/matrix.cursor.o
mx_cursor_h=../matrix/matrix.cursor.h ../matrix/size.struct.h ../matrix/data.struct.h
mx_vector_h=matrix.vector.h
ixs_omega_obj=ixs.omega.o matrix.slm.o omega.integral.o
libraries=-L../mmap -lmmap -L../math/ -lmath -lgmp -lmpfr
libspherical=-L ../spherical -lspherical
.PHONY: clean, all

all: $(ixs_omega_obj)

libomega.a: $(ixs_omega_obj)
	ar cr $@ $(ixs_omega_obj)

omega.integral.o: omega.integral.h omega.integral.hpp omega.integral.cpp $(lib_math) $(xpn_obj) $(poly_n_obj)
	$(CXX) $(CXXFLAGS) -c -o $@ omega.integral.cpp

ixs.omega.exe: ixs.omega.o ixs.omega.demo.h ixs.omega.main.cpp matrix.slm.o omega.integral.o
	$(CXX) $(CXXFLAGS) -o $@ ixs.omega.main.cpp $(memory_man_obj) $(poly_n_obj) $(xpn_obj) $(ixs_omega_obj) $(libraries)

ixs.omega.test.exe: $(ixs_omega_obj) ixs.omega.test.h ixs.omega.test.cpp
	$(CXX) $(CXXFLAGS) -o $@ ixs.omega.test.cpp $(memory_man_obj) $(poly_n_obj) $(xpn_obj) $(ixs_omega_obj) $(libraries)

ixs.omega.o: ixs.omega.h ixs.omega.hpp ixs.omega.cpp matrix.slm.o $(memory_man_obj) $(xpn_obj) $(lib_math)
	$(CXX) $(CXXFLAGS) -c -o $@ ixs.omega.cpp

matrix.slm.exe: matrix.slm.o matrix.slm.write.o matrix.slm.demo.h matrix.slm.main.cpp omega.integral.o
	$(CXX) $(CXXFLAGS) -o $@ matrix.slm.main.cpp matrix.slm.o matrix.slm.write.o omega.integral.o \
		$(poly_n_obj) $(xpn_obj) $(libspherical) $(libraries)

matrix.slm.o: matrix.slm.h matrix.slm.hpp matrix.slm.cpp matrix.vector.h $(mx_cursor_h) $(mx_vector_h) $(poly_n_obj)
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.slm.cpp

matrix.slm.write.o: matrix.slm.h matrix.slm.write.hpp matrix.slm.write.cpp $(mx_cursor_h) $(mx_vector_h) $(poly_n_obj)
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.slm.write.cpp

omega.integral.exe: omega.integral.o omega.integral.demo.h omega.integral.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ omega.integral.main.cpp omega.integral.o $(libraries)

clean:
	rm *.o
