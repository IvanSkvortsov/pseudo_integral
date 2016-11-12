CXX=g++ --std=c++11
CXXFLAGS=-I ../memorystream -I ../matrix -I ..
libraries=-lgmp -lmpfr
memstr_obj=../memorystream/memorystream.o
objects=
.PHONY: clean

all: $(objects)

basis.set.exe: basis.set.h basis.defs.h basis.set.demo.h basis.set.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ basis.set.main.cpp

basis.map.o: basis.map.h basis.map.hpp basis.map.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ basis.map.cpp

basis.map.exe: basis.map.o basis.map.demo.h basis.map.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ basis.map.main.cpp basis.map.o $(memstr_obj) $(libraries)

ecp.set.exe: ecp.set.h ecp.set.demo.h ecp.set.main.cpp
	$(CXX) -o $@ ecp.set.main.cpp

clean:
	rm -f *.o *.exe
