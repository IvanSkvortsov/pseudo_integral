#include"matrix.slm.h"
#include"xyz.pown.integral.h"
#include"omega.integral.h"
#include<iostream>// cerr, endl
#include<iomanip>// setw
#include<cstdio>// snprintf
#include"mpreal.h"
#include<cstdlib>// exit
#include<cmath>// fma (fused multiply-add)
#include"sf.math.h"// kahan

using namespace std;

template<typename T>
T omega_integral( typename matrix_slm<T>::value_type const & v, xyz_pown_integral<T> const & xpn_int )
{
	static unsigned long long int zeros = 0;
	static T value = T(0);
	value = 0;
	T u;
	math::kahan<T> kh_value( value );
	int const * x = 0;
	poly_3<T> const * p = v.data();
	for(int i = 0; i < v.size(); ++i, ++p)
	{
		x = p->x;
		if( p->d == 0 )
		{
			cerr << "Error: [omega_integral] : " << setw(14) << zeros++ << endl;
			continue;
			exit(1);
		}
		// fma(x, y, z) = (x * y) + z
		//value = fma( p->d, xpn_int( x[0], x[1], x[2] ), value );
		u = p->d;
		u *= xpn_int( x[0], x[1], x[2] );
		kh_value += u;
		//value += u;
	}
	return kh_value.x;
	//return value;
}

template<typename T>
int is_zero(T const & value){return value==0;}

//static union{ void * v; unsigned int * z;} store;
//
#define WRONG_BITS   12u
#define IS_ZERO_CFLOAT( d )\
	static union{ void * v; unsigned long long int * z;} store;\
	d += 1;\
	store.v = &d;\
	*store.z >>= WRONG_BITS;\
	*store.z <<= WRONG_BITS;

#define IS_ZERO_CFLOAT_SPEC( type )\
template<> int is_zero(type const & value)\
{\
	type d = value;\
	IS_ZERO_CFLOAT( d );\
	return d==1;\
}

IS_ZERO_CFLOAT_SPEC( float );
IS_ZERO_CFLOAT_SPEC( double );
IS_ZERO_CFLOAT_SPEC( long double );

std::string itoa( int i, int width = 2)
{
	static char format[128], i_char[32];
	snprintf( format, 128, "%%0%dd", width );
	snprintf( i_char, 32, format, i );
	return i_char;
}

template<typename T>
int is_zero_v02(T const & value)
{
	static T tmp = T(0);
	tmp = (value < 0 ? -value : value);
	return tmp < 1.0e-60;
}

template<> int is_zero_v02(double const & value)
{
	static double tmp;
	tmp = (value < 0 ? -value : value);
	return tmp < 1.0e-09;
}

template<> int is_zero_v02(long double const & value)
{
	static long double tmp;
	tmp = (value < 0 ? -value : value);
	return tmp < 1.0e-09;
}

#define RUN_OMEGA_ERR\
	cerr << "Error: [run_omega]" << endl;\
	cerr << "l   : " << l << endl;\
	cerr << "lmb : " << lmb << endl;\
	cerr << "m   : " << m << endl;\
	cerr << "mu  : " << mu << endl;\
	cerr << "value : " << scientific << setprecision(20) << value << (is_zero_v02(value) ? " zero" : " not zero") << endl;

#define __ABS( value ) ((value) < 0 ? -(value) : (value))

template<typename T, typename float_type>
void print(std::ostream & out, T const & v, xyz_pown_integral<float_type> const & xpn_int )
{
	const int * x;
	float_type value = float_type(0);
	out << setw(8) << v.size() << endl;
	for(int i = 0; i < v.size(); ++i)
	{
		x = v[i].x;
		value += v[i].d * xpn_int( x[0], x[1], x[2] );
		out  << setw(40) << setprecision(30) << v[i].d <<
			setw(4) << x[0] <<
			setw(4) << x[1] <<
			setw(4) << x[2] << ' ' <<
			setw(40)<< xpn_int(x[0], x[1], x[2]) << ' ' <<
			setw(40)<< value << endl;
	}
}
template<typename T>
int run_omega( int l, int la_max, matrix_slm<T> const & mx, xyz_pown_integral<T> const & xpn_int )
{
	T value = T(0), min_value = T(1000), max_value = T(0);
	for(int m = -l; m <= l; ++m)
	{
		cout << setw(4) << l << setw(4) << m << flush;
		for( int lmb = 0; lmb <= l+la_max; ++lmb )
		{
			for(int mu = -lmb; mu <= lmb; ++mu)
			{
				value = omega_integral<T>( mx(l+m, lmb*lmb + lmb + mu ), xpn_int );
				if( (l != lmb || m != mu) && !is_zero_v02( value ) )
				{
					RUN_OMEGA_ERR;
					print( cerr, mx(l+m, lmb*lmb + lmb + mu), xpn_int );
					return 1;
				}
				else if( l == lmb && m == mu && !is_zero_v02( value-1 ) )
				//else if( l == lmb && m == mu && value != 1 )
				{
					RUN_OMEGA_ERR;
					return 2;
				}
				if( min_value > __ABS( value ) && value != 0 )
					min_value = value;
				if( max_value < __ABS( value ) && (l != lmb || m != mu) && value != 0 )
					max_value = value;
			}
		}
		cout << setw(20) << min_value <<
			setw(20) << max_value <<
			endl;
	}
	return 0;
}
template<typename T>
int demo_mxslm()
{
	int lmax = 16;
	vector<string> vs( lmax + 1 );

	xyz_pown_integral<T> xpn_int;
	int xpn_imax = lmax*4;
	xpn_int.read();
	if( xpn_int.i_max() < lmax*4 || xpn_int.j_max() < lmax*4 || xpn_int.k_max() < lmax*4 )
	{
		cerr << "Error: [demo_slmmx]" << endl;
		cerr << "lmax : " << lmax << endl;
		cerr << "lmax * 4 : " << lmax * 4 << endl;
		cerr << "xpn.i_max : " << xpn_int.i_max() << endl;
		cerr << "xpn.j_max : " << xpn_int.j_max() << endl;
		cerr << "xpn.k_max : " << xpn_int.k_max() << endl;
		return 1;
	}
	//xpn_int.run( xpn_imax, xpn_imax, xpn_imax );

	matrix_slm<T> mx;
	mx.source().reserve( 1u<<21 );
	mx.resize( 5000 );

	int _st = 0;
	//for(int i = 0; i < vs.size(); ++i)
	for(int i = 0; i <= 9; ++i)
	{
		vs[i] = "matrix.slm." + itoa(i, 2) + ".src";
		cout << vs[i] << endl;
		_st = mx.read( vs[i].c_str() );
		if( _st )
		{
			cerr << "Error: [demo_slmmx] " << _st << endl;
			cerr << "l : " << i << endl;
			cerr << "file '" << vs[i] << "'" << endl;
			return 1;
		}
		_st = run_omega<T>( i, lmax, mx, xpn_int );
		if( _st )
		{
			cerr << "Error: [demo_slmmx] " << _st << endl;
			return 2;
		}
	}
	return 0;
}

template<typename T>
int demo_vecslm()
{
	int lmax = 32;

	std::string file = "vector.slm.";
	file += itoa(lmax, 2);
	file += ".src";
	int _st = matrix_slm_write<T>(file.c_str(), 0, lmax, 0);// 0 - is %l_max, %lmax - is %la_max, 0 - is %l
	if( _st )
	{
		cerr << "Error: from [demo_slmvec] " << _st << endl;
		return _st;
	}
	return 0;
}

int demo_mxslm_d()
{
	//return demo_mxslm<double>();
	return demo_mxslm<long double>();
	//mpfr::mpreal::set_default_prec( mpfr::digits2bits(105) );
	//return demo_mxslm<mpfr::mpreal>();
	//return demo_vecslm<mpfr::mpreal>();
}
