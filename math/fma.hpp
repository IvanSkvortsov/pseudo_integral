#ifndef __FMA_HPP__
#define __FMA_HPP__
#include"fma.h"
#include<math.h>
#include<float.h>
#include<fenv.h>

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
#endif

template<typename T>
T math::fma(T const & v1, T const & v2, T const & v3)
{
	T a = T(v1);
	a *= v2;
	a += v3;
	return a;
}

namespace math
{
	template<> float fma<float>(float const & v1, float const & v2, float const & v3){return fmaf(v1, v2, v3);}
	template<> double fma<double>(double const & v1, double const & v2, double const & v3){return fma(v1, v2, v3);}
	//template<> long double fma<long double>(long double const & v1, long double const & v2, long double const & v3){return fmal(v1, v2, v3);}
#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )
	template<> mpfr::mpreal fma(mpfr::mpreal const & v1, mpfr::mpreal const & v2, mpfr::mpreal const & v3){return mpfr::fma(v1, v2, v3);}
#endif
}

template long double math::fma(long double const & v1, long double const & v2, long double const & v3);

#endif//__FMA_HPP__
