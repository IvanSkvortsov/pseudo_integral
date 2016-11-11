objects=size.of.int.types.o memory.map.o
executives=size.of.int.types.exe memory.map.exe

all: $(objects)

size.of.int.types.exe: size.of.int.types.o
	g++ -o $@ $(objects)

size.of.int.types.o: size.of.int.types.h size.of.int.types.cpp
	g++ -c -o $@ size.of.int.types.cpp

memory.map.exe: memory.map.o memory.map.demo.h memory.map.main.cpp
	g++ -o $@ memory.map.main.cpp memory.map.o

memory.map.o: memory.map.h memory.map.cpp
	g++ -c -o $@ memory.map.cpp

libmmap.a: memory.map.o
	ar cr $@ memory.map.o

clean:
	rm -f *.o *.exe
