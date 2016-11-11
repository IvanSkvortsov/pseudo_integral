CXX=g++
CXXFLAGS=-std=c++11 -I ..
objects=equality.o get.exp.o set.exp.o add.exp.o zero.exp.o fact.2n1.o pown.o fact.n.o logn.o binom.o fact.p.o numeric.constants.o fma.o \
	numeric.limits.o cast.o kahan.o sf.exponent.o
libraries=-lgmp -lmpfr
.PHONY: clean, all

all: libmath.a

libsfmath.a: $(objects) sf.math.h sf.machine.h
	ar cr $@ $(objects)

sf.exponent.o: sf.exponent.h sf.exponent.hpp sf.exponent.cpp exponent.2n.h
	$(CXX) $(CXXFLAGS) -c -o $@ sf.exponent.cpp

equality.o: equality.h equality.hpp equality.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ equality.cpp $(libraries)

get.exp.o: get.exp.h get.exp.hpp get.exp.cpp expmanip.config.h
	$(CXX) $(CXXFLAGS) -c -o $@ get.exp.cpp $(libraries)

set.exp.o: set.exp.h set.exp.hpp set.exp.cpp expmanip.config.h
	$(CXX) $(CXXFLAGS) -c -o $@ set.exp.cpp $(libraries)

add.exp.o: add.exp.h add.exp.hpp add.exp.cpp expmanip.config.h
	$(CXX) $(CXXFLAGS) -c -o $@ add.exp.cpp $(libraries)

zero.exp.o: zero.exp.h zero.exp.hpp zero.exp.cpp expmanip.config.h
	$(CXX) $(CXXFLAGS) -c -o $@ zero.exp.cpp $(libraries)

libexpmanip.a: get.exp.o set.exp.o add.exp.o zero.exp.o
	ar cr $@ get.exp.o set.exp.o add.exp.o zero.exp.o

fact.p.o: fact.h fact.p.hpp fact.p.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ fact.p.cpp $(libraries)

fact.n.o: fact.h fact.n.hpp fact.n.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ fact.n.cpp $(libraries)

fact.2n1.o: fact.h fact.2n1.hpp fact.2n1.cpp
	$(CXX) -c -o $@ fact.2n1.cpp

libfact.a: fact.n.o fact.2n1.o
	ar cr $@ fact.n.o fact.2n1.o

logn.o: logn.h logn.hpp logn.cpp
	$(CXX) -c -o $@ logn.cpp

pown.o: pown.h pown.hpp pown.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ pown.cpp $(libraries)

binom.o: binom.h binom.hpp binom.cpp \
	binom.uint32.0.hpp binom.uint32.1.hpp binom.uint32.2.hpp \
	binom.uint64.0.hpp binom.uint64.1.hpp binom.uint64.2.hpp
	$(CXX) -c -o $@ binom.cpp

numeric.constants.o: numeric.constants.h numeric.constants.hpp numeric.constants.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ numeric.constants.cpp $(libraries)

numeric.limits.o: numeric.limits.h numeric.limits.hpp numeric.limits.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ numeric.limits.cpp $(libraries)

fma.o: fma.h fma.hpp fma.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ fma.cpp $(libraries)

cast.o: cast.h cast.hpp cast.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ cast.cpp $(libraries)

kahan.o: kahan.h kahan.hpp kahan.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ kahan.cpp

numeric.limits.exe: numeric.limits.o numeric.limits.demo.h numeric.limits.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ numeric.limits.main.cpp numeric.limits.o $(libraries)

numeric.constants.exe: numeric.constants.o numeric.constants.demo.h numeric.constants.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ numeric.constants.main.cpp numeric.constants.o $(libraries)

libmath.a: libsfmath.a
	cp libsfmath.a $@


clean:
	rm *.o *.a
