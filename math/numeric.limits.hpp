#ifndef __NUMERIC_LIMITS_HPP__
#define __NUMERIC_LIMITS_HPP__
#include"sf.machine.h"
#include"numeric.limits.h"

#ifdef __MPREAL_USE
#include"mpreal.h"
#endif

namespace math
{
	template<typename T> const T numeric_limits<T>::epsilon = SF_LDBL_EPSILON;
	template<typename T> const T numeric_limits<T>::max     = SF_LDBL_MAX;
	template<typename T> const T numeric_limits<T>::min     = SF_LDBL_MIN;

	const float numeric_limits<float>::epsilon = SF_FLT_EPSILON;
	const float numeric_limits<float>::max     = SF_FLT_MAX;
	const float numeric_limits<float>::min     = SF_FLT_MIN;

	const double numeric_limits<double>::epsilon = SF_DBL_EPSILON;
	const double numeric_limits<double>::max     = SF_DBL_MAX;
	const double numeric_limits<double>::min     = SF_DBL_MIN;

	const long double numeric_limits<long double>::epsilon = SF_LDBL_EPSILON;
	const long double numeric_limits<long double>::max     = SF_LDBL_MAX;
	const long double numeric_limits<long double>::min     = SF_LDBL_MIN;
#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )

	const mpfr::mpreal numeric_limits<mpfr::mpreal>::epsilon = SF_MPF_EPSILON;
	const mpfr::mpreal numeric_limits<mpfr::mpreal>::max     = SF_MPF_MAX;
	const mpfr::mpreal numeric_limits<mpfr::mpreal>::min     = SF_MPF_MIN;
#endif
}
#endif//__NUMERIC_LIMITS_HPP__
