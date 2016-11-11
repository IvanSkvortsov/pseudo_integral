#include"sf.hyperg.1F1.h"
#include"sf.hyperg.h"
#include"mpreal.h"
#include"sf.hyperg.h"
#include<cmath>//exp
#include<iostream>
#include<iomanip>
#include<fstream>

using namespace std;

template<typename T>
int hyperg_demo(char * file)
{
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [hyperg_demo] can't open file \"" << file << "\"" << endl;
		return 1;
	}
	T a, b, x, value = T(0);
	int ex = 0, iter = 0, hyperg_n = 0;
	int prec = 18, w = prec + 10, wz = 10;
	/*
	cout.setf( ios::scientific );
	cout.precision( prec );
	cout << setw(w) << "a" <<
		setw(w) << "b" <<
		setw(w) << "x" <<
		setw(w) << "1F1(a,b,x)" <<
		setw(wz) << "e" <<
		setw(w) << "exp(x)" << endl << endl;
	*/
	int size = 0, n;
	inp >> size;
	if( size < 0 )
		return 1;
	for(int i = 0; i < size; ++i)
	{
		inp >> a >> b >> x;
		if( inp == 0 || iter++ > 10 )
			break;
		int st = sf_hyperg_1F1<T>(a, b, x, ex, value, hyperg_n, n, 20 );
		//int st = sf_hyperg_1F1_series1<T>(a, b, x, value );
		if( st )
		{
			cerr << "Error: [hyperg_demo] from sf_hyperg_1F1 st = " << st << ' ' << sf_hyperg::error( st ) << endl;
			cerr << "iter : " << iter << endl;
			return st;
		}
		/*
		cout << setprecision(4) << setw(w) << a << setw(w) << b <<
			setprecision(prec) <<
			setw(w) << x << setw(w) << value << setw(wz) << ex <<
			setw(w) << exp(x) << endl;
		*/
	}
	return 0;
}

int hyperg_demo_d(char * file)
{
	//mpfr::mpreal::set_default_prec( mpfr::digits2bits( 30 ) );
	//return hyperg_demo<mpfr::mpreal>( file );
	//return hyperg_demo<long double>( file );
	return hyperg_demo<double>( file );
}

int main(int argc, char ** argv)
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return hyperg_demo_d(argv[1]);
}
