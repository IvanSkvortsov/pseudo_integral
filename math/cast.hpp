#ifndef __CAST_HPP__
#define __CAST_HPP__
#include"cast.h"

#ifdef __MPREAL_USE
#include"mpreal.h"
#endif

namespace math
{
	template<> int cast<int, float>(float const & v){return v;}
	template<> int cast<int, double>(double const & v){return v;}
	template<> int cast<int, long double>(long double const & v){return v;}
#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )
	template<> int cast<int, mpfr::mpreal>(mpfr::mpreal const & v){return v.toLong();}
#endif
}

#endif//__CAST_HPP__
