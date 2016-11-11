#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
using namespace std;

template<typename T>
int alpha_gen(char const * file, const char * file_out )
{
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [alpha_gen] can't open file '" << file << "'" << endl;
		return 1;
	}

	int num;
	T min, max;
	inp >> num >> min >> max;
	if( min < 0 ) min = -min;
	if( max < 0 ) max = -max;
	T tmp = min;
	min = (min < max ? min : max );
	max = (tmp < max ? max : tmp );
	vector<T> v( num );
	const T dx = (max - min) / (num - 1);
	for(int i = 0; i < v.size(); ++i)
		v[i] = min + dx * i;
	ofstream out( file_out );
	out << setw(10) << num << endl;
	for(int i = 0; i < v.size(); ++i)
		out << setw(26) << setprecision(16) << scientific << v[i] << endl;
	return 0;
}

int main(int argc, char ** argv)
{
	if( argc != 3 )
	{
		cerr << "Error: [main] usage './main.exe file.inp file.out'" << endl;
		return 1;
	}
	return alpha_gen<double>( argv[1], argv[2] );
}
