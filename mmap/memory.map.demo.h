#include"memory.map.h"
#include<iostream>
#include<fstream>
#include<iomanip>
#include"/home/Ivan/cpp/projects/bitprint/bitprint.hpp"
#include"/home/Ivan/cpp/projects/bitprint/smart.bits.hpp"
#include<sys/mman.h>// PROT_READ, PROT_WRITE
#include<fcntl.h>// S_IRUSR, S_IWUSR, S_IXUSR, ...
//#include<cstdio>
//#include<sys/stat.h>
//#include<sys/mmap.h>
#include<unistd.h>

using namespace std;

int mmap_create(char * file);
int mmap_open(char * file);

int mmap_demo(char * file)
{
	cout << setw(14) << "PROT_READ" << " " << smart_bits( PROT_READ, 8 ) << endl;
	cout << setw(14) << "PROT_WRITE" << " " << smart_bits( PROT_WRITE, 8 ) << endl;
	cout << setw(14) << "S_IRUSR" << " " << smart_bits( S_IRUSR, 8 ) << endl;
	cout << setw(14) << "S_IWUSR" << " " << smart_bits( S_IWUSR, 8 ) << endl;
	char c;
	cout << "Enter 0/1 (create/open): ";
	cin >> c;
	if( c == '0' )
		return mmap_create( file );
	else if( c == '1' )
		return mmap_open( file );
	else return 1;
}

int mmap_create(char * file)
{
	memory_map pool;
	int cr_z = pool.create( file, 1ull<<14 );// 2016.03.16 19:40
	//int cr_z = pool.create( file, 1ull<<20 );
	if( cr_z )
	{
		cerr << "Error: [mmap_create] from memory_map::create(char const * , const off_t )" << endl;
		return 1;
	}
	const int ws = 14;
	cout << setw(ws) << "size" << " : " << pool.size() << " " << pool.size()/(1ull<<10) << endl;
	cout << setw(ws) << "&data[0]" << " : " << "[" << pool.data() << "]" << endl;
	cout << setw(ws) << "data[0]" << " : " << *((int *)pool.data()) << endl;
	cout << endl;
	union
	{
		void * _void;
		char * _char;
		double * _double;
	} data;
	data._void = pool.data();
	cout << "> ";
	cin >> *(data._double)++;
	*(data._double) = data._double[-1] * 0.25;
	cout.setf( std::ios::scientific );
	int prec = 18, w = prec + 10;
	cout << "[" << data._double << "] : " << setw(w) << setprecision(prec) << data._double[0] << endl;
	cout << "resize :" << endl <<
		"enter positive integral value > ";
	int _psize = 0;
	cin >> _psize;
	cout << endl << setw(ws) << "psize" << " : " << _psize << endl;
	cout << "increasing file size..." << endl;
	lseek( pool.fd(), pool.size() + _psize, SEEK_SET );
	write( pool.fd(), "", 1);
	lseek( pool.fd(), 0, SEEK_SET );
	cout << setw(ws) << "filesize" << " : " << pool.size() + _psize << endl;
	data._void = pool.data();
	data._char += pool.size();
	cout << endl;
	cout << setw(ws) << "end of map" << " : [" << data._void << "] " << data._char - (char *)pool.data() << endl;
	data._char += _psize;
	cout << setw(ws) << "over of map" << " : [" << data._void << "] " << data._char - (char *)pool.data() << endl;
	data._char -= _psize;
	*data._double = 1.25;
	cout << setw(ws) << "over of map" << " : [" << data._void << "] " << *data._double << " " << data._char - (char *)pool.data() << endl;
	//cout << "filename : " << pool.filename() << endl;

	return 0;
}

int mmap_open(char * file)
{
	memory_map pool;
	int op_z = pool.open( file );
	if( op_z )
	{
		cerr << "Error: [mmap_open] from memory_map::open(char const *)" << endl;
		return 1;
	}
	const int ws = 14;
	cout << setw(ws) << "size" << " : " << pool.size() << " " << pool.size()/(1ull<<10) << endl;
	cout << setw(ws) << "&data[0]" << " : " << "[" << pool.data() << "]" << endl;
	cout << setw(ws) << "data[0]" << " : " << *((int *)pool.data()) << endl;
	union
	{
		void * _void;
		char * _char;
		double * _double;
	} data;
	data._void = pool.data();
	cout.setf( std::ios::scientific );
	int prec = 18, w = prec + 10;
	cout << "[" << data._double << "] : " << setw(w) << setprecision(prec) << data._double[0] << endl;
	cout << "[" << data._double+1 << "] : " << setw(w) << setprecision(prec) << data._double[1] << endl;
	data._void = pool.data();
	data._char += pool.size();
	data._char -= 1;
	cout << "[" << data._double << "] : " << setw(w) << setprecision(prec) << data._double[0] << " " << data._char - (char *)pool.data() << endl;
	//cout << "filename : " << pool.filename() << endl;
	return 0;
}


