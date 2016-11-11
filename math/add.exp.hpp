#ifndef __ADD_EXP_HPP__
#define __ADD_EXP_HPP__
#include"add.exp.h"

void math::add_exp(float & val, int e)
{
	if( e==0 ) return;
	unsigned int * p = (unsigned int *)&val;
	// add exponent 'e'
	e <<= 23;
	*p += e;
}
#ifdef  __STD_ADDEXP
#include<math.h>
void math::add_exp(double & val, int e)
{
	if( e==0 ) return;
	double x = val;
	val = ldexp(x, e);
}
#else
void math::add_exp(double & val, int e)
{
	if( e==0 ) return;
	unsigned int * p = (unsigned int *)&val;
	++p;
	e <<= 20;// 52 - 32
	*p += e;
}
#endif
void math::add_exp(long double & val, int e)
{
	if( e==0 ) return;
	unsigned int * p = (unsigned int *)&val;
	p += 2u;
	*p += e; 
}

#if defined( __MP_ADDEXP ) && defined( __MPREAL_H__ )
void math::add_exp(mpfr::mpreal & val, int e)
{
	if( e==0 ) return;
	//mpfr_mul_2si(val.mp, val.mp, e, mpfr::mpreal::get_default_rnd() );
	mpfr::mpreal x(val);
	val = ldexp(x, e);
}
#endif

template<typename T>
void math::add_e(T & val, int e){return math::add_exp(val, e);}

template void math::add_e(float & val, int e);
template void math::add_e(double & val, int e);
template void math::add_e(long double & val, int e);
#if defined( __MP_ADDEXP ) && defined( __MPREAL_H__ )
template void math::add_e(mpfr::mpreal & val, int e);
#endif

#endif//__ADD_EXP_HPP__
