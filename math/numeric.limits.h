#ifndef __NUMERIC_LIMITS_H__
#define __NUMERIC_LIMITS_H__

#include"mpreal.use.h"

#if defined( __MPREAL_USE )
#include"mpreal.h"
#endif

namespace math
{
	template<typename T>
	struct numeric_limits
	{
		static const T epsilon;
		static const T max;
		static const T min;
	};
	template<> struct numeric_limits<float>
	{
		static const float epsilon;
		static const float max;
		static const float min;
	};
	template<> struct numeric_limits<double>
	{
		static const double epsilon;// = SF_DBL_EPSILON;
		static const double max;//     = SF_DBL_MAX;
		static const double min;//     = SF_DBL_MIN;
	};
	template<> struct numeric_limits<long double>
	{
		static const long double epsilon;// = SF_LDBL_EPSILON;
		static const long double max;//     = SF_LDBL_MAX;
		static const long double min;//     = SF_LDBL_MIN;
	};
#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )
	template<> struct numeric_limits<mpfr::mpreal>
	{
		static const mpfr::mpreal epsilon;
		static const mpfr::mpreal max;
		static const mpfr::mpreal min;
	};
#endif
}

#endif//__NUMERIC_LIMITS_H__
