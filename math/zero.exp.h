#ifndef __ZERO_EXP_H__
#define __ZERO_EXP_H__
#include"expmanip.config.h"

#ifdef  __MP_ZEROEXP
#include"mpreal.h"
#endif

namespace math
{
	void zero_exp(float &  val);
	void zero_exp(double & val);
	void zero_exp(long double & val);
#if defined( __MP_ZEROEXP ) && defined( __MPREAL_H__ )
	void zero_exp(mpfr::mpreal & val);
#endif

	template<typename T> void zero_e(T & val);
}
#endif//__ZERO_EXP_H__
