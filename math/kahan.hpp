#ifndef __KAHAN_HPP__
#define __KAHAN_HPP__
#include"kahan.h"

template<typename T> math::kahan<T>::kahan() : x(), y(){}
template<typename T> math::kahan<T>::kahan( math::kahan<T> const & k) : x( k.x ), y( k.y ){}
template<typename T> math::kahan<T>::kahan( T const & __x ) : x(__x), y(0){}
template<typename T> math::kahan<T>::kahan( T const & __x, T const & __y ) : x(__x), y(__y){}
template<typename T> math::kahan<T>::~kahan(){}

template<typename T> math::kahan<T> & math::kahan<T>::operator=( math::kahan<T> const & k)
{
	if( this == &k )
		return *this;
	this->x = k.x;
	this->y = k.y;
	return *this;
}
template<typename T> math::kahan<T> & math::kahan<T>::operator=( T const & __x)
{
	this->x = __x;
	this->y = 0;
	return *this;
}

template<typename T> math::kahan<T> & math::kahan<T>::operator+=( T const & __x)
{
	math::kahan_add<T>( this->x, this->y, __x );
	return *this;
}
template<typename T> math::kahan<T> & math::kahan<T>::operator-=( T const & __x)
{
	math::kahan_sub<T>( this->x, this->y, __x );
	return *this;
}

// Kahan summation algorithm:
//
// sum_true = a + a_c 
//
// a1     -- correction
// a0     -- rounded result of summation
// a0+a1  -- true result (with correction)
//
// add
// (a0 + a1) + b
template<typename T>
void
math::kahan_add(T & a0, T & a1, T const & b)
{
	T c = T(b);
	c -= a1;
	T t = T(a0);
	t += c;
	//
	a1 = t;
	a1 -= a0;
	a1 -= c;
	a0 = t;
	return;
	//
	a1 = a0;
	a1 -= t;
	a1 += c;
	a0 = t;
	return;
}

// sub
template<typename T>
void
math::kahan_sub(T & a0, T & a1, T const & b)
{
	T c = T(b);
	c -= a1;
	T t = T(a0);
	t -= c;
	a1 = a0;
	a1 -= t;
	a1 += c;
	a0 = t;
}

#define MATH_KAHAN_SPEC( type )\
	template void math::kahan_add( type & a0, type & a1, type const & b );\
	template void math::kahan_sub( type & a0, type & a1, type const & b );\
	template struct math::kahan<type>;

#endif//__KAHAN_HPP__
