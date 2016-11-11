#ifndef __LEGENDRE_H__
#define __LEGENDRE_H__
#include"poly.n.h"
#include"orthogonal.h"

#define __ERROR_MSG_LEGENDRE

template<class T>
struct legendre: public orthogonal<poly_n<T,4> >
{
	typedef poly_n<T,4> polynomial_type;
	typedef poly_n<T,4> pT;
	typedef poly_n<T,4> value_type;
	typedef T float_type;
	legendre();
	legendre(legendre<T> const & legen);
	legendre(int const & __size);
	legendre<T> & operator=(legendre<T> const & legen);
	void run( int __n);
	int run( int __n, int __m );
	int run_m( int __m);
	void run_cos_sin( int __m);
};

#endif//__LEGENDRE_H__
