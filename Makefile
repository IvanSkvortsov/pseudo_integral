CXX=g++
CXXFLAGS=-std=c++11 -I .
#LIBRARIES=-L./spherical -lspherical -L./math -lmath -lgmp -lmpfr -L./xyz_spherical -lxyzsph
#LIBRARIES=-L./integral -lintegral -L./qu -lqu -L./hyperg -lhyperg -L./args -largs -L./xyz_spherical -lxyzsph -L./spherical -lspherical -L./env -lenv\
	  -L./math -lsfmath -lgmp -lmpfr
#LIBRARIES=-L./env -lenv -L./qu -lqu -L./hyperg -lhyperg -L./args -largs -L./xyz_spherical -lxyzsph -L./spherical -lspherical \
	  -L./math -lsfmath -lgmp -lmpfr
LIBRARIES=-L./spherical -lspherical -L./math -lmath -lgmp -lmpfr
EXTERN_OBJS=args/ecp.args.alpha.o args/ecp.args.center.o \
	    env/ecp.env.o \
	    integral/ecp.integral.o integral/ecp.integral.type.o \
	    hyperg/sf.hyperg.1F1.o hyperg/sf.hyperg.2F0.o hyperg/sf.hyperg.1F11.o hyperg/sf.errno.tostring.o hyperg/sf.hyperg.n.tostring.o \
	    mmap/memory.map.o\
	    qu/qu.o \
	    xyz_spherical/i.xyz.spherical.o xyz_spherical/i.xyz.spherical.calculator.o xyz_spherical/r.xyz.spherical.o \
	    xyz_pown_integral/xyz.pown.integral.o \
	    vec/sh.vector.o vec/ecp.sh.vector.o
OBJECTS=main.o
.PHONY: clean, start

start:
	make clean;
	make main.exe
	./main.exe file.inp file.out > file.err;
	cat file.out;

main.o: main.cpp main.h
	$(CXX) $(CXXFLAGS) -c -o $@ main.cpp $(LIBRARIES)

main.exe: $(OBJECTS)
	cd spherical; make libspherical.a
	cd omega; make ixs.omega.exe
	$(CXX) $(CXXFLAGS) -o $@ $(OBJECTS) $(EXTERN_OBJS) $(LIBRARIES)

clean:
	rm *.o
