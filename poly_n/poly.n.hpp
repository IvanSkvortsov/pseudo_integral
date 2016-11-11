#ifndef __POLY_N_HPP__
#define __POLY_N_HPP__
#include"poly.n.h"
#include"pown.h"// /home/Ivan/cpp/lib/math/pow/

template<class T, int _size>
poly_n<T, _size>::poly_n():d(1)
{
	int * p = x;
	for(int i = 0; i < _size; ++i)
		*p++ = 0;
}
template<class T, int _size>
poly_n<T, _size>::poly_n(int const * __x):d(1), x( __x){}
template<class T, int _size>
poly_n<T, _size>::poly_n(T const & __d, int const * __x):d(__d), x( __x){}
template<class T, int _size>
poly_n<T, _size> & poly_n<T, _size>::operator=(poly_n<T,_size> const & v)
{
	if( this == &v )
		return *this;
	d = v.d;
	x = v.x;
	return *this;
}
template<class T, int _size>
poly_n<T, _size> & poly_n<T, _size>::operator*=(poly_n<T,_size> const & v)
{
	d *= v.d;
	for(int i = 0; i < _size; ++i)
		x[i] += v[i];
	return *this;
}
template<class T, int _size>
poly_n<T,_size> & poly_n<T, _size>::operator*=(T const & v)
{
	d *= v;
	return *this;
}
template<class T, int _size>
int & poly_n<T, _size>::operator[](int i){return x[i];}
template<class T, int _size>
int const & poly_n<T, _size>::operator[](int i)const{return x[i];}
template<class T, int _size>
const int poly_n<T, _size>::size(){return _size;}
template<class T, int _size>
void poly_n<T, _size>::set_x(int const * __x)
{
	for(int i = 0; i < _size; ++i)
		x[i] = __x[i];
}
template<class T, int _size>
int poly_n<T, _size>::sum_x()const
{
	int sum = 0;
	for(int i = 0; i < _size; ++i)
		sum += x[i];
	return sum;
}
template<class T, int _size>
void poly_n<T, _size>::set_zero_x()
{
	for(int i = 0; i < _size; ++i)
		x[i] = 0;
}
template<class T, int _size>
bool poly_n<T, _size>::is_even_x()const
{
	int sum = 0;
	for(int i = 0; i < _size; ++i)
		sum += ( x[i]%2==0 );
	return sum;
}
template<class T, int _size>
bool poly_n<T, _size>::is_equal_x(poly_n<T,_size> const & v)const
{
	for(int i = 0; i < _size; ++i)
		if(this->x[i] != v.x[i]) return false;
	return true;
}
template<class T, int _size>
bool poly_n<T, _size>::is_equal_x(poly_n<T,_size> const & a, poly_n<T,_size> const & b)
{
	return a.is_equal_x(b);
}
template<class T, int _size>
T poly_n<T, _size>::calc_r(T const * xval)const
{
	T value = T(1);
	for(int i = 0; i < _size; ++i)
		value *= math::pown<T>( xval[i], x[i] );
	value *= d;
	return value;
}

template<class T, int _size>
poly_n<T,_size> operator*(poly_n<T,_size> const & a, poly_n<T,_size> const & b)
{
	poly_n<T,_size> c( a );
	c *= b;
	return c;
}

template<class T, int _size>
bool operator==(poly_n<T,_size> const & a, poly_n<T,_size> const & b)
{
	return a[0] == b[0] && a[1] == b[1] && a[2] == b[2] && a[3] == b[3];
}

template<class T, int _size>
bool operator!=(poly_n<T,_size> const & a, poly_n<T,_size> const & b)
{
	return !(a==b);
}

#ifdef  __PRINT_POLY_N
#include<iostream>
#include<iomanip>
template<class T, int _size>
void poly_n<T, _size>::print(std::ostream & out, int p, int w_x)const
{
	int w = p + 8;
	out.setf( std::ios::scientific );
	out.precision( p );
	out << std::setw( w ) << this->d;
	for(int i = 0; i < _size; ++i)
		out << std::setw( w_x ) << this->x[i];
	out << std::endl;
}
#endif//__PRINT_POLY_N

#define POLY_N_SPEC( type, num )\
template struct poly_n<type, num>;\
template poly_n<type,num> operator*(poly_n<type,num> const & a, poly_n<type,num> const & b);\
template bool operator==(poly_n<type,num> const & a, poly_n<type,num> const & b);\
template bool operator!=(poly_n<type,num> const & a, poly_n<type,num> const & b);

#define POLY_SPEC( type )\
	POLY_N_SPEC( type, 3 );\
	POLY_N_SPEC( type, 4 );

POLY_SPEC( float )
POLY_SPEC( double )
POLY_SPEC( long double )

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
POLY_SPEC( mpfr::mpreal )
#endif

#endif//__POLY_N_HPP__
