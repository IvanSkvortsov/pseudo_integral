#ifndef __POWN_HPP__
#define __POWN_HPP__
#include"pown.h"
#include<cmath>// sqrt

// n > 0
template<typename T>
inline static void pown_npos(T const & x, int n, T & value)
{
	T t = T(x);
	value = 1;
	for(int i = 0; i < 31; ++i, n >>= 1u)
	{
		if(n & 1u)
			value *= t;
		t *= t;
	}
}

template<typename T>
void math::pown(T const & x, int n, T & value)
{
	if( n == 0 )
	{
		value = 1;
		return;
	}
	else if( n > 0 )
	{
		pown_npos<T>(x, n, value);
		return;
	}
	else
	{
		T t;
		pown_npos<T>(x, -n, t);
		value = 1;
		value /= t;
		return;
	}
}
template<typename T>
T math::pown(T const & x, int n)
{
	T value;
	math::pown<T>(x, n, value);
	return value;
}

template<typename T> int math::pown05( T const & x, const int _pown05_max, T * _arr, const int _arr_size )
{
	if( _pown05_max + 1 > _arr_size )
		return 1;
	_arr[0] = 1;
	if( _pown05_max == 0 )
		return 0;
	const T sqrt_x = sqrt( x );
	T u = T(1);
	const int _pown_max = _pown05_max / 2;
	for(int i = 1; i <= _pown_max; ++i)
	{
		_arr[ 2 * i - 1 ] = u;
		_arr[ 2 * i - 1 ] *= sqrt_x;
		u *= x;
		_arr[ 2 * i ] = u;
	}
	if( _pown05_max % 2 )
	{
		_arr[ _pown05_max ] = u;
		_arr[ _pown05_max ] *= sqrt_x;
	}
	return 0;
}

#define MATH_POWN_SPEC( type )\
template void math::pown(type const & x, int n, type & value);\
template type math::pown(type const & x, int n);\
template int  math::pown05(type const & x, const int _pown05_max, type * _arr, const int _arr_size );

MATH_POWN_SPEC( int )
MATH_POWN_SPEC( unsigned int )
MATH_POWN_SPEC( long int )
MATH_POWN_SPEC( unsigned long int )
MATH_POWN_SPEC( long long int )
MATH_POWN_SPEC( unsigned long long int )
MATH_POWN_SPEC( float )
MATH_POWN_SPEC( double )
MATH_POWN_SPEC( long double )

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
MATH_POWN_SPEC( mpfr::mpreal )
#endif

#endif//__POWN_HPP__
