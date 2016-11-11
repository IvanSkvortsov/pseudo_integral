CXX=g++ -std=c++11
CXXFLAGS=-I ../xyz_pown_integral -I .. -I ../mmap -I ../math -I ../memorystream -I ../poly_n -I ../array -I ../matrix -I ../integral -I ../spherical

memory_man_obj=../memorystream/memorystream.o ../mmap/memory.map.o
spherical_poly_obj=../poly_n/poly.n.o ../array/array.n.o
xpn_obj=../xyz_pown_integral/xyz.pown.integral.o

ixs_angular_obj=ixs.angular.frame.o ixs.angular.data.o matrix.cursor.o
qu_radial_mx_obj=../qu/qu.radial.mx.o
ixs_omega_obj=ixs.omega.o matrix.cursor.o matrix.slm.o omega.integral.o
#omega.integral.o ixs.test.o
#matrix.slm.o spherical.pown.o
libraries=-L../mmap -lmmap -L../math/ -lmath -lgmp -lmpfr
libspherical=-L ../spherical -lspherical
.PHONY: clean

omega.integral.o: omega.integral.h omega.integral.hpp omega.integral.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ omega.integral.cpp

matrix.cursor.o: matrix.cursor.h matrix.cursor.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.cursor.cpp

matrix.cursor.exe: matrix.cursor.o matrix.cursor.demo.h matrix.cursor.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ matrix.cursor.main.cpp matrix.cursor.o $(ixs_angular_extern_obj) -lgmp -lmpfr

ixs.omega.o: ixs.omega.h ixs.omega.hpp ixs.omega.cpp matrix.cursor.o
	$(CXX) $(CXXFLAGS) -c -o $@ ixs.omega.cpp

ixs.omega.exe: ixs.omega.demo.h ixs.omega.main.cpp $(ixs_omega_obj)
	$(CXX) $(CXXFLAGS) -o $@ ixs.omega.main.cpp $(ixs_omega_obj) $(xpn_obj) $(spherical_poly_obj) $(memory_man_obj) $(libraries)

ixs.angular.data.o: ixs.angular.data.h ixs.angular.data.hpp ixs.angular.data.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ ixs.angular.data.cpp

ixs.angular.frame.o: ixs.angular.frame.h ixs.angular.frame.hpp ixs.angular.frame.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ ixs.angular.frame.cpp

ixs.angular.exe: ixs.angular.demo.h ixs.angular.main.cpp $(ixs_angular_obj) $(ixs_omega_obj) slm.compute.r.o
	$(CXX) $(CXXFLAGS) -o $@ ixs.angular.main.cpp $(ixs_angular_obj) slm.compute.r.o \
		$(ixs_omega_obj) $(ixs_omega_extern_obj) $(qu_radial_mx_obj) $(libraries)

ixs.angular.test.exe: ixs.angular.test.h ixs.angular.test.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ ixs.angular.test.main.cpp slm.compute.r.o matrix.slm.o $(ixs_omega_extern_obj) $(ixs_omega_obj)  $(libraries)

slm.compute.r.o: slm.compute.r.h slm.compute.r.hpp slm.compute.r.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ slm.compute.r.cpp

ixs.test.o: ixs.test.h ixs.test.hpp ixs.test.cpp
	$(CXX) -c -o $@ ixs.test.cpp

matrix.slm.exe: matrix.slm.o matrix.slm.write.o matrix.slm.demo.h matrix.slm.main.cpp omega.integral.o
	$(CXX) $(CXXFLAGS) -o $@ matrix.slm.main.cpp matrix.slm.o matrix.slm.write.o omega.integral.o\
		$(spherical_poly_obj) $(xpn_obj) $(libspherical) $(libraries)

matrix.slm.o: matrix.slm.h matrix.slm.hpp matrix.slm.cpp matrix.vector.h
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.slm.cpp

matrix.slm.write.o: matrix.slm.h matrix.slm.write.hpp matrix.slm.write.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ matrix.slm.write.cpp

spherical.pown.o: spherical.pown.h spherical.pown.hpp spherical.pown.cpp pointer.v.hpp
	$(CXX) $(CXXFLAGS) -c -o $@ spherical.pown.cpp

spherical.pown.exe: spherical.pown.demo.h spherical.pown.main.cpp $(ixs_omega_obj) $(ixs_omega_extern_obj)
	$(CXX) $(CXXFLAGS) -o $@ spherical.pown.main.cpp $(ixs_omega_obj) $(ixs_omega_extern_obj) $(libraries)

spherical.exe: spherical.demo.h spherical.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ spherical.main.cpp $(ixs_omega_extern_obj) -L../spherical -lspherical $(libraries)

omega.integral.exe: omega.integral.o omega.integral.demo.h omega.integral.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ omega.integral.main.cpp omega.integral.o $(libraries)

libomega.a: $(ixs_omega_obj)
	ar cr $@ $(ixs_omega_obj)

clean:
	rm *.o
