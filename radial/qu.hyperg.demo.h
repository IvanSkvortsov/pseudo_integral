#include"qu.hyperg.1F11.struct.h"
#include"qu.hyperg.1F1.struct.h"
#include"sf.hyperg.1F11.h"
#include"sf.hyperg.1F1.h"
#include<iostream>
#include<fstream>
#include<iomanip>

#include<limits>// numeric_limts
#include"sf.math.h"// add_exp

#include"get.work.time.h"
#include"smart.time.h"

using namespace std;

template<typename T>
int demo_qu_hyperg_1F1( const char * file )
{
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_qu_1F1] can't open file '" << file << "'" << endl;
		return 1;
	}
	clog.setf( ios::scientific );
	const int prec = 18, w = prec + 10, ws = 24;
	clog.precision( prec );

	T alp, k1, k2;
	int N, lmb1, lmb2;

	inp >> alp >> k1 >> k2;
	inp >> N >> lmb1 >> lmb2;
	inp.close();

	const T sqrt_alp = sqrt( alp );
	const int pown05_max = 13;
	T pown05_alp[ pown05_max + 1 ];
	T u = 1;
	pown05_alp[0] = 1;
	for(int i = 1; i <= pown05_max/2; ++i)
	{
		pown05_alp[2*i - 1] = u * sqrt_alp;
		pown05_alp[2*i] = u;
	}
	if( pown05_max%2 )
		pown05_alp[ pown05_max ] = u * sqrt_alp;

	qu_hyperg_1F1_struct<T> q;
	clog << setw( ws ) << "value : " << setw( w ) << q.value << setw( 10 ) << q.ex << endl;
	
	q.set_k( k1 );
	q.set_alp( alp );
	q.set_hyperg_t();
	q.set_n();
	q.set_n_max();
	
	q.set_lmb( lmb1 );
	q.set_a_2( q.lmb + N + 1 );

	q.set_coef_g();
	q.set_coef( math::pown( k1, q.lmb), pown05_alp, pown05_max );

	get_work_time t1;

	t1.counting_start();
	sf_hyperg_1F1( q );
	t1.counting_end();

	if( q.ex < std::numeric_limits<T>::max_exponent )
	{
		math::add_exp( q.value, q.ex );
		q.ex = 0;
	}
	clog << "Time : " << smart_time( t1 ) << endl;
	if( q.hyperg_n != q.hyperg_t )
	{
		q.error( "hyperg_t", "hyperg_n -neq hyperg_t" );
	}
	clog << setw( ws ) << "hyperg_t : " << setw( w ) << sf_hyperg::type( q.hyperg_t ) << setw(10) << q.hyperg_t << endl;
	clog << setw( ws ) << "hyperg_n : " << setw( w ) << sf_hyperg::type( q.hyperg_n ) << setw(10) << q.hyperg_n << endl;
	clog << setw( ws ) << "value : " << setw( w ) << q.value << setw( 10 ) << q.ex << endl;
	clog << setw( ws ) << "alp : " << setw( w ) << alp << endl;
	clog << setw( ws ) << "k : " << setw( w ) << k1 << endl;
	clog << setw( ws ) << "x : " << setw( w ) << q.x << endl;
	clog << setw( ws ) << "N : " << setw( w ) << q.a_2 - q.lmb - 1 << endl;
	clog << setw( ws ) << "lmb : " << setw( w ) << q.lmb << endl;
	clog << setw( ws ) << "exp_x : " << setw( w ) << q.exp_x << setw( 10) << q.exp_ex << endl;
	clog << setw( ws ) << "coef_g : " << setw( w ) << q.coef_g << endl;
	clog << setw( ws ) << "coef : " << setw( w ) << q.coef << endl;
	clog << setw( ws ) << "coef * coef_g : " << setw( w ) << q.coef * q.coef_g << endl;
	if( q.value < 0 )
		clog << "negative" << endl;
	else
		clog << "positive" << endl;
	return 0;
}

template<typename T>
int demo_qu_hyperg_1F11( const char * file )
{
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_qu_struct] can't open file '" << file << "'" << endl;
		return 1;
	}
	clog.setf( ios::scientific );
	const int prec = 18, w = prec + 10, ws = 24;
	clog.precision( prec );

	T alp, k1, k2;
	int N, lmb1, lmb2;

	inp >> alp >> k1 >> k2;
	inp >> N >> lmb1 >> lmb2;
	inp.close();

	const T sqrt_alp = sqrt( alp );
	const int pown05_max = 13;
	T pown05_alp[ pown05_max + 1 ];
	T u = 1;
	pown05_alp[0] = 1;
	for(int i = 1; i <= pown05_max/2; ++i)
	{
		pown05_alp[2*i - 1] = u * sqrt_alp;
		pown05_alp[2*i] = u;
	}
	if( pown05_max%2 )
		pown05_alp[ pown05_max ] = u * sqrt_alp;

	qu_hyperg_1F11_struct<T> q;
	clog << setw( ws ) << "value : " << setw( w ) << q.value << setw( 10 ) << q.ex << endl;
	
	q.set_k1( k1 );
	q.set_k2( k2 );
	q.set_alp( alp );
	q.set_hyperg_t();
	q.set_flip();
	q.set_n1();
	q.set_n2();
	q.set_n1_max();
	q.set_n2_max();
	
	q.set_lmb1( lmb1 );
	q.set_lmb2( lmb2 );
	q.set_a_2( q.lmb1 + q.lmb2 + N + 1 );

	q.set_coef_g();
	q.set_coef( math::pown( k1, q.lmb1), math::pown( k2, q.lmb2 ), pown05_alp, pown05_max );

	get_work_time t1;
	t1.counting_start();
	if( q.to_flip() )
		sf_hyperg_1F11_series_flip( q );
	else
		sf_hyperg_1F11_series( q );
	t1.counting_end();
	if( q.ex < std::numeric_limits<T>::max_exponent )
	{
		math::add_exp( q.value, q.ex );
		q.ex = 0;
	}
	clog << "Time : " << smart_time( t1 ) << endl;
	if( q.hyperg_n != q.hyperg_t )
	{
		q.error( "hyperg_t", "hyperg_n -neq hyperg_t" );
	}
	clog << setw( ws ) << "hyperg_t : " << setw( w ) << sf_hyperg::type( q.hyperg_t ) << setw(10) << q.hyperg_t << endl;
	clog << setw( ws ) << "hyperg_n : " << setw( w ) << sf_hyperg::type( q.hyperg_n ) << setw(10) << q.hyperg_n << endl;
	clog << setw( ws ) << "value : " << setw( w ) << q.value << setw( 10 ) << q.ex << endl;
	clog << setw( ws ) << "to_flip : " << setw( w ) << boolalpha << q.to_flip() << endl;
	clog << setw( ws ) << "alp : " << setw( w ) << alp << endl;
	clog << setw( ws ) << "k1 : " << setw( w ) << k1 << endl;
	clog << setw( ws ) << "k2 : " << setw( w ) << k2 << endl;
	clog << setw( ws ) << "x1 : " << setw( w ) << q.x1 << endl;
	clog << setw( ws ) << "x2 : " << setw( w ) << q.x2 << endl;
	clog << setw( ws ) << "N : " << setw( w ) << q.a_2 - q.lmb1 - q.lmb2 - 1 << endl;
	clog << setw( ws ) << "lmb1 : " << setw( w ) << q.lmb1 << endl;
	clog << setw( ws ) << "lmb2 : " << setw( w ) << q.lmb2 << endl;
	clog << setw( ws ) << "exp_x : " << setw( w ) << q.exp_x << setw( 10) << q.exp_ex << endl;
	clog << setw( ws ) << "coef_g : " << setw( w ) << q.coef_g << setw( 10 ) << q.exp_ex2 << endl;
	clog << setw( ws ) << "coef : " << setw( w ) << q.coef << endl;
	clog << setw( ws ) << "coef * coef_g : " << setw( w ) << q.coef * q.coef_g << endl;
	if( q.value < 0 )
		clog << "negative" << endl;
	else
		clog << "positive" << endl;
	return 0;
}

template<typename T> int demo_qu_hyperg( const char * file )
{
	int _st = demo_qu_hyperg_1F11<T>( file );
	_st += demo_qu_hyperg_1F1<T>( file );
	return _st;
}

int main( int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	//return demo_qu_hyperg<long double>( argv[1] );
	return demo_qu_hyperg<double>( argv[1] );
}
