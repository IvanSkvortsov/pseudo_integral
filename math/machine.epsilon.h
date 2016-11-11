#ifndef __MACHINE_EPSILON_H__
#define __MACHINE_EPSILON_H__
#include"numeric.limits.h"// math::numeric_limits

namespace math
{
	template<typename U> inline U machine_epsilon();
	template<typename U> U machine_epsilon(){return math::numeric_limits<U>::epsilon;}

	template float machine_epsilon<float>();
	template double machine_epsilon<double>();
	template long double machine_epsilon<long double>();
}

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"// std::numeric_limits<mpfr::mpreal>
namespace math
{
	template<> inline mpfr::mpreal machine_epsilon<mpfr::mpreal>(){return std::numeric_limits<mpfr::mpreal>::epsilon();}
}
#endif

#endif//__MACHINE_EPSILON_H__

