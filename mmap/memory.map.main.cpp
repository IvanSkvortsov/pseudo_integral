#include"memory.map.demo.h"

int main(int argc, char ** argv)
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file'" << endl;
		return 1;
	}
	return mmap_demo(argv[1]);
}
