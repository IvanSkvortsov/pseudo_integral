#include"xyz.pown.integral.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include"mpreal.h"

using namespace std;
using mpfr::mpreal;

template<typename T>
int demo_xpn(char const * file)
{
	xyz_pown_integral<T> xpn_int;
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_xpn] can't open file '" << file << "'" << endl;
		return 1;
	}
	int i_max, j_max, k_max;
	inp >> i_max >> j_max >> k_max;
	xpn_int.run( i_max, j_max, k_max );
	static const char file_src[] = "xyz.pown.integral.src";
	xpn_int.write( file_src, 105 );
	xyz_pown_integral<T> xpn_int2;
	xpn_int2.read( file_src );
	cout << "size : " << xpn_int2.size() << endl;
	cout << "max_size : " << xpn_int2.max_size() << endl;
	return 0;
}

int demo_xpn_d(const char * file)
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(105) );
	return demo_xpn<mpfr::mpreal>( file );
}

int main(int argc, char ** argv)
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_xpn_d( argv[1] );
}
