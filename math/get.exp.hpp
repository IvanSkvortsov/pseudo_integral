#ifndef __GETEXP_HPP__
#define __GETEXP_HPP__
#include"get.exp.h"

typedef unsigned long long int u64_t;
typedef unsigned int u32_t;

u32_t math::get_exp_u(float const & val)
{
	//return ((*((u32_t *)&val)<<1)>>24);
	//return ((*((u32_t *)&val)>>23)&0xFFu);
	u32_t ex = *( (u32_t *)&val );
	ex <<= 1;
	ex >>= 24;
	return ex;
}
int math::get_exp(float const & val){return math::get_exp_u(val)-127;}

#ifdef  __STD_GETEXP
#include<math.h>// frexp
u32_t math::get_exp_u(double const & val)
{
	return 1023u+math::get_exp(val);
}
int math::get_exp(double const & val)
{
	int n;
	double x = val, res = frexp(x, &n);
	return n-1;
}
#else
u32_t math::get_exp_u(double const & val)
{
	//u64_t * data64 = (u64_t *)&val;
	//return (*data64>>52)&0x7FF;
	//return (*((u64_t *)&val)>>52)&0x7FF;
	//return ((*((u32_t *)&val+1u))>>52)&0x7FF;
	u32_t ex = *((u32_t *)&val + 1u);
	//u32_t * data32 = (u32_t *)&val;
	//data32 += 1u;
	//u32_t ex = *data32;
	ex <<= 1;
	ex >>= 21;
	//ex >>= 20;
	//ex &= 0x7FFu;
	return ex;
}
int math::get_exp(double const & val){return math::get_exp_u(val)-1023;}
#endif

u32_t math::get_exp_u(long double const & val)
{
	//u64_t * p = (u64_t *)&val;
	//++p;
	//return ((*p)&0x7FFF);
	//return ((*(((u64_t *)&val)+1))&0x7FFFull);
	u32_t ex = *((u32_t *)&val + 2u);
	ex &= 0x7FFFu;
	return ex;
}
int math::get_exp(long double const & val){return math::get_exp_u(val)-16383;}

#if defined( __MP_GETEXP ) && defined( __MPREAL_H__ )
u32_t math::get_exp_u(mpfr::mpreal const & val)
{
	return math::get_exp( val ) + 1073741823u;
}
int math::get_exp(mpfr::mpreal const & val)
{
	return val.get_exp()-1;
}
#endif

template<typename T>
int math::get_e(T const & val)
{
	return math::get_exp(val);
}
template int math::get_e(float const & val);
template int math::get_e(double const & val);
template int math::get_e(long double const & val);
#if defined( __MP_GETEXP ) && defined( __MPREAL_H__ )
template int math::get_e(mpfr::mpreal const & val);
#endif

#endif//__GETEXP_HPP__
