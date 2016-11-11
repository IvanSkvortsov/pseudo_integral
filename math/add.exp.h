#ifndef __ADD_EXP_H__
#define __ADD_EXP_H__
#include"expmanip.config.h"

#ifdef  __MP_ADDEXP
#include"mpreal.h"
#endif

namespace math
{
	void add_exp(float  & val, int e);
	void add_exp(double & val, int e);
	void add_exp(long double & val, int e);
#if defined( __MP_ADDEXP ) && defined( __MPREAL_H__ )
	void add_exp(mpfr::mpreal & val, int e);
#endif
	template<typename T> void add_e(T  & val, int e);
}
#endif//__ADD_EXP_H__
