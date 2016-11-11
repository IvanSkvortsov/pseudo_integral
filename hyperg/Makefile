CXX=g++ -std=c++11
CXXFLAGS=-I .. -I ../math
objects=sf.hyperg.1F1.o sf.hyperg.1F11.o sf.hyperg.2F0.o
files=sf.hyperg.h print.hyperg.h
libraries=-L../math -lsfmath -lgmp -lmpfr
.PHONY: clean, all

all: libhyperg.a

sf.hyperg.n.exe: sf.hyperg.n.h sf.hyperg.n.demo.h sf.hyperg.n.main.cpp
	$(CXX) -o $@ sf.hyperg.n.main.cpp

sf.hyperg.1F1.o: sf.hyperg.1F1.h sf.hyperg.1F1.hpp sf.hyperg.1F1.cpp sf.hyperg.2F0.hpp $(files)
	$(CXX) $(CXXFLAGS) -c -o $@ sf.hyperg.1F1.cpp $(libraries)

sf.hyperg.1F11.o: sf.hyperg.1F11.h sf.hyperg.1F11.hpp sf.hyperg.1F11.cpp sf.hyperg.1F1.hpp sf.hyperg.2F0.hpp $(files)
	$(CXX) $(CXXFLAGS) -c -o $@ sf.hyperg.1F11.cpp $(libraries)

sf.hyperg.2F0.o: sf.hyperg.2F0.h sf.hyperg.2F0.hpp sf.hyperg.2F0.cpp $(files)
	$(CXX) $(CXXFLAGS) -c -o $@ sf.hyperg.2F0.cpp $(libraries)

libhyperg.a: $(objects)
	ar cr $@ $(objects)

hyperg.exe: $(objects) hyperg.demo.h hyperg.main.cpp
	$(CXX) $(CXXFLAGS) -o $@ hyperg.main.cpp $(objects) $(libraries)

clean:
	rm -f *.o *.exe
