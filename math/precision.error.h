#ifndef __PRECISION_ERROR_H__
#define __PRECISION_ERROR_H__

namespace math
{
	template<typename U> inline U precision_error();

	template<> inline float precision_error<float>(){return 1.e02f;}
	template<> inline double precision_error<double>(){return 1.e04;}
	template<> inline long double precision_error<long double>(){return 1.e08L;}
}

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
namespace math
{
	template<> inline mpfr::mpreal precision_error<mpfr::mpreal>(){return 1.e20;}
}
#endif

#endif//__PRECISION_ERROR_H__

