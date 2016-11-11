#ifndef __CONVERT_FLOAT_H__
#define __CONVERT_FLOAT_H__

namespace math
{
	template<typename T, typename U> inline T convert_float(U const & rhs);

	template<typename T, typename U> T convert_float(U const & rhs){ return rhs; }
	template<typename T> inline T convert_float(T const & rhs){return rhs;}
	// specialization macros
#define CONVERT_FLOAT_SPEC( typeU )\
	template float convert_float<float, typeU>(typeU const & rhs);\
	template double convert_float<double, typeU>(typeU const & rhs);\
	template long double convert_float<long double, typeU>(typeU const & rhs);\

	CONVERT_FLOAT_SPEC(float);
	CONVERT_FLOAT_SPEC(double);
	CONVERT_FLOAT_SPEC(long double);
}

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
namespace math
{
	template<> inline float convert_float(mpfr::mpreal const & rhs){ return rhs.toFloat(); }
	template<> inline double convert_float(mpfr::mpreal const & rhs){ return rhs.toDouble(); }
	template<> inline long double convert_float(mpfr::mpreal const & rhs){ return rhs.toLDouble(); }
	template<> inline mpfr::mpreal convert_float(float const & rhs){ return rhs; }
	template<> inline mpfr::mpreal convert_float(double const & rhs){ return rhs; }
	template<> inline mpfr::mpreal convert_float(long double const & rhs){ return rhs; }
}
#endif

#endif//__CONVERT_FLOAT_H__

