#ifndef __ZERO_EXP_HPP__
#define __ZERO_EXP_HPP__
#include"zero.exp.h"

void math::zero_exp(float &  val)
{
	unsigned int * p = (unsigned int *)&val;
	*p &= 0x807FFFFFu;
	*p |= 0x3F800000u;
}
#ifdef  __STD_ZEROEXP
#include<math.h>
void math::zero_exp(double & val)
{
	double x = val;
	int n;
	x = frexp(x, &n);
	val = ldexp(x, 1);
}
#else
void math::zero_exp(double & val)
{
	unsigned int * p = (unsigned int *)&val;
	++p;
	*p &= 0x800FFFFFu;
	*p |= 0x3FF00000u;
}
#endif
void math::zero_exp(long double & val)
{
	unsigned int * p = (unsigned int *)&val;
	p += 2u;
	*p &= 0x00008000u;
	*p |= 0x00003FFFu;
}

#if defined( __MPREAL_H__ ) && defined( __MP_ZEROEXP )
void math::zero_exp(mpfr::mpreal & val)
{
	val.set_exp(1);
}
#endif

template<typename T> void math::zero_e(T & val){return math::zero_exp(val);}

template void math::zero_e(float & val);
template void math::zero_e(double & val);
template void math::zero_e(long double & val);
#if defined( __MPREAL_H__ ) && defined( __MP_ZEROEXP )
template void math::zero_e(mpfr::mpreal & val);
#endif

#endif//__ZERO_EXP_HPP__
