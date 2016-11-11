#ifndef __SET_EXP_H__
#define __SET_EXP_H__
#include"expmanip.config.h"

#ifdef  __MP_SETEXP
#include"mpreal.h"
#endif

namespace math
{
	void set_exp(float  & val, int e);
	void set_exp(double & val, int e);
	void set_exp(long double & val, int e);

#if defined( __MP_SETEXP ) && defined( __MPREAL_H__ )
	void set_exp(mpfr::mpreal & val, int e);
#endif
	template<typename T> void set_e(T & val, int e);
}

#endif//__SET_EXP_H__
