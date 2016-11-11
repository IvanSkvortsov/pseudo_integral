#ifndef __GET_EXP_H__
#define __GET_EXP_H__
#include"expmanip.config.h"

#ifdef  __MP_GETEXP
#include"mpreal.h"
#endif

namespace math
{
	unsigned int get_exp_u(float const &  val);
	unsigned int get_exp_u(double const & val);
	unsigned int get_exp_u(long double const & val);

	int get_exp(float const & val);
	int get_exp(double const & val);
	int get_exp(long double const & val);

#if defined( __MP_GETEXP ) && defined( __MPREAL_H__ )
	unsigned int get_exp_u(mpfr::mpreal const & val);
	int get_exp(mpfr::mpreal const & val);
#endif

	template<typename T> int get_e(T const & val);
}

#endif//__GET_EXP_H__
