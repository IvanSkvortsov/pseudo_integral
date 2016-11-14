#include"pseudo.task.01.read.h"
#include"pseudo.task.01.write.h"
#include<iostream>
#include<fstream>

using namespace std;

template<typename T>
int read_task_01( const char * file, _3_centers<T> & geoms, _3_alphas<T> & alps, _3_momenta & angs )
{
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [read_task_01] can't open file '" << file << "'" << endl;
		return 1;
	}
	geoms.read_a( inp );
	geoms.read_b( inp );
	geoms.read_c( inp );

	alps.read_a( inp );
	alps.read_b( inp );
	alps.read_c( inp );

	angs.read( inp );
	inp.close();
	return 0;
}

template<typename T>
int pseudo_task_01_demo( const char * file )
{
	_3_centers<T> geoms;
	_3_alphas<T> alps;
	_3_momenta angs;
	int _st = read_task_01( file, geoms, alps, angs );
	if( _st != 0 )
	{
		cerr << "Error: from [pseudo_task_01_demo]" << endl;
		return _st;
	}
	const char file_geom[] = "u.geom.txt";
	const char file_basis[] = "u.basis.txt";
	const char file_ecp[] = "u.ecp.txt";
	pseudo_task_01_write_geom<T>( file_geom, geoms );
	pseudo_task_01_write_basis<T>( file_basis, alps, angs );
	pseudo_task_01_write_ecp<T>( file_ecp, alps, angs );
	return 0;
}

int pseudo_task_01_demo_d( const char * file )
{
	return pseudo_task_01_demo<double>( file );
}

int main( int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return pseudo_task_01_demo_d( argv[1] );
}
