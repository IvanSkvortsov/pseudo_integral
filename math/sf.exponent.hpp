#ifndef __SF_EXP_HPP__
#define __SF_EXP_HPP__
#include"sf.exponent.h"
#include<cmath>// modf, log, exp
#include<limits>// numeric_limits

template<typename T> inline static T sf_modf( T const & num, T * i )
{
	return modf( num, i );
}
template<> inline float sf_modf( float const & num, float * i ){ return modff( num, i ); }
template<> inline double sf_modf( double const & num, double * i ){ return modf( num, i ); }
template<> inline long double sf_modf( long double const & num, long double * i )
{
	double x_int;
	modf( (double)num, &x_int );
	*i = x_int;
	long double xl_eps = num;
	xl_eps -= *i;
	return xl_eps;
}

template<typename T> int sf_exponent(T const & x, T & v, int & n)
{
	static const T LOG_MAX = log( std::numeric_limits<T>::max() ) / 4;
	const T abs_x = (x < 0 ? -x : x);
	if( abs_x < LOG_MAX )
	{
		v = exp( x );
		n = 0;
		return 0;
	}
	const int is_xint = (x == floor(x));
	if( x == 0 )
	{
		v = 1;
		n = 0;
		return 0;
	}
	else if( x == 1 )
	{
		v = exponent_2n_plus[0].x;
		n = 1;
		return 0;
	}
	else if( is_xint )
	{
		long double v_ld;
		exponent_int(x, v_ld, n);
		v = v_ld;
		return 0;
	}
	else if( abs_x < 4)
	{
		n = 0;
		return exponent_series<T>(x, v);
	}
	else
	{
		T x_int;
		T x_eps = sf_modf(x, &x_int);// double
		//T x_eps = modf(x, &x_int);// double
		long double v_ld;
		exponent_int(x_int, v_ld, n);
		int stat_ser = exponent_series<T>( x_eps, v );
		v *= v_ld;
		return stat_ser;
	}
}

#define __SF_EXP_SPEC( type ) template int sf_exponent(type const & x, type & v, int & n)

#endif//__SF_EXP_HPP__
