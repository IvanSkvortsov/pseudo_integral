CXX=g++ -std=c++11
CXXFLAGS=-I .. -I ../alpha -I ../angular -I ../array -I ../basis -I ../math -I ../matrix -I ../memorystream -I ../mmap -I ../omega -I ../poly_n \
	 -I ../vector_3d_ -I ../xyz_pown_integral -I ../radial -I ../hyperg
libraries=-L../hyperg -lhyperg -L ../math -lmath -lgmp -lmpfr

alpha_obj=../alpha/alpha.map.o ../alpha/alpha.val.o ../alpha/alpha.pow.o ../alpha/alpha.slm.o ../alpha/alpha.mem.o
angular_obj=../angular/ixs.angular.map.o ../angular/ixs.angular.dat.o ../angular/geom.slm.o ../angular/ixs.angular.idx.o
array_obj=../array/array.n.o
basis_obj=
matrix_obj=../matrix/matrix.cursor.o
mstream_obj=../memorystream/memorystream.o
mmap_obj=../mmap/memory.map.o
omega_obj=../omega/ixs.omega.o ../omega/matrix.slm.o ../omega/omega.integral.o
poly_obj=../poly_n/poly.n.o
vector3d_obj=../vector_3d_/vector.3d.o
xyzpi_obj=../xyz_pown_integral/xyz.pown.integral.o
qu_rad_obj=../radial/qu.radial.map.o ../radial/qu.radial.dat.o
objects=$(alpha_obj) $(angular_obj) $(array_obj) $(basis_obj) $(matrix_obj) $(mstream_obj) $(mmap_obj) $(omega_obj) $(poly_obj) $(vector3d_obj)\
	$(xyzpi_obj) $(qu_rad_obj)
matrix_h=../matrix/matrix.cursor.h ../matrix/size.struct.h ../matrix/data.struct.h
math_lib=../math/libmath.a
hyperg_lib=../hyperg/libhyperg.a
pseudo_obj=pseudo.integral.o pseudo.integral.mem.o
.PHONY: clean, all_glob, vec3d, math, xpni, poly, mems, omgi, alp, hypg, radi, angi

vec3: 
	cd ../vector_3d_; make vector.3d.o;

math:
	cd ../math; make libmath.a;

xpni: $(math_lib)
	cd ../xyz_pown_integral; make clean; make all;

poly: $(math_lib)
	cd ../array;  make clean; make all;
	cd ../poly_n; make clean; make all;

mems:
	cd ../memorystream; make memorystream.o;
	cd ../mmap;         make memory.map.o;

omgi: $(math_lib) $(xyzpi_obj) $(poly_obj) $(array_obj) $(mstream_obj) $(mmap_obj) $(matrix_h)
	cd ../omega; make clean; make all;

alp:  $(mstream_obj) $(mmap_obj) $(math_lib) $(vector3d_obj) $(matrix_h) $(math_lib)
	cd ../alpha; make clean; make all;

hypg:
	cd ../hyperg; make all;

radi: $(hyperg_lib) $(alpha_obj)
	cd ../radial; make clean; make all;

angi: $(vector3d_obj) $(omgega_obj) $(alpha_obj) $(qu_rad_obj)
	cd ../angular; make clean; make all;

all_glob: vec3 math xpni poly mems omgi alp hypg radi angi pseudo.struct.exe


all_:
	make vec3
	make math
	make xpni
	make poly
	make mems
	make omgi
	make alp
	make hypg
	make radi
	make angi
	make pseudo.struct.exe

remake_all:
	cd ../vector_3d_;         make clean; make all;
	cd ../math;               make clean; make all;
	cd ../xyz_pown_integral;  make clean; make all;
	cd ../array;              make clean; make all;
	cd ../poly_n;             make clean; make all;
	cd ../memorystream;       make clean; make all;
	cd ../mmap;               make clean; make all;
	cd ../omega;              make clean; make all;
	cd ../alpha;              make clean; make all;
	cd ../hyperg;             make clean; make all;
	cd ../radial;             make clean; make all;
	cd ../angular;            make clean; make all;
	make pseudo.struct.exe

pseudo.struct.exe: pseudo.struct.h pseudo.struct.demo.h pseudo.struct.main.cpp $(objects) $(pseudo_obj)
	$(CXX) $(CXXFLAGS) -o $@ pseudo.struct.main.cpp $(objects) $(pseudo_obj) $(libraries)

pseudo.integral.o: pseudo.integral.h pseudo.integral.hpp pseudo.integral.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ pseudo.integral.cpp

pseudo.integral.mem.o: pseudo.integral.mem.h pseudo.integral.mem.hpp pseudo.integral.mem.cpp pseudo.integral.o
	$(CXX) $(CXXFLAGS) -c -o $@ pseudo.integral.mem.cpp

pseudo.task.01.write.o: pseudo.task.01.write.h pseudo.task.01.write.hpp pseudo.task.01.write.cpp
	$(CXX) -I .. -std=c++11 -c -o $@ pseudo.task.01.write.cpp

pseudo.task.01.read.o: pseudo.task.01.read.h pseudo.task.01.read.hpp pseudo.task.01.read.cpp
	$(CXX) -I .. -std=c++11 -c -o $@ pseudo.task.01.read.cpp

pseudo.task.01.exe: pseudo.task.01.demo.h pseudo.task.01.read.o pseudo.task.01.write.o
	$(CXX) -I .. -std=c++11 -o $@ pseudo.task.01.main.cpp pseudo.task.01.read.o pseudo.task.01.write.o -lgmp -lmpfr

libpseudo.a: $(pseudo_obj)
	ar cr $@ $(pseudo_obj)

clean:
	rm -f *.o *.exe
