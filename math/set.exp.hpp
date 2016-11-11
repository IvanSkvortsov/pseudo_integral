#ifndef __SET_EXP_HPP__
#define __SET_EXP_HPP__
#include"set.exp.h"

void math::set_exp(float & val, int e)
{
	e += 127;
	unsigned int * p = (unsigned int *)&val;
	// set bits of exponent region to zero value
	*p &= 0x807FFFFF;// &= 1000 0000 0111 1111 ....
	// set exponent value 'e'
	e <<= 23;
	*p |= e;
}
#ifdef  __STD_SETEXP
#include<math.h>
void math::set_exp(double & val, int e)
{
	double x = val;
	int n;
	x = frexp(x, &n);
	x = ldexp(x, e+1);
	val = x;
}
#else
void math::set_exp(double & val, int e)
{
	e += 1023;
	unsigned int * p = (unsigned int *)&val;
	++p;
	*p &= 0x800FFFFFu;// &= 1000 0000 0000 1111 ....
	e <<= 20;
	*p |= e;
}
#endif
void math::set_exp(long double & val, int e)
{
	e += 16383;
	unsigned int * p = (unsigned int *)&val;
	p += 2u;
	*p &= 0x8000u;
	*p |= e;
}

#if defined( __MP_SETEXP ) && defined( __MPREAL_H__ )
void math::set_exp(mpfr::mpreal & val, int e)
{
	val.set_exp( e+1 );
}
#endif

template<typename T> void math::set_e(T & val, int e){return math::set_exp(val, e);}

template void math::set_e(float & val, int e);
template void math::set_e(double & val, int e);
template void math::set_e(long double & val, int e);
#if defined( __MP_SETEXP ) && defined( __MPREAL_H__ )
template void math::set_e(mpfr::mpreal & val, int e);
#endif

#endif//__SET_EXP_HPP__
