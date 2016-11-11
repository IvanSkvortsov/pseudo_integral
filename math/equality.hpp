#ifndef __EQUALITY_HPP__
#define __EQUALITY_HPP__
#include"equality.h"

template<typename T> static bool is_lt_eps(T const & x);
template<typename T> static bool is_gt_eps(T const & x);

template<typename T> bool is_lt_eps(T const & x){return x < MATH_DBL_EPSILON;}
template<typename T> bool is_gt_eps(T const & x){return x > MATH_DBL_EPSILON;}

template<> bool is_lt_eps(float const & x){return (x < MATH_FLT_EPSILON);}
template<> bool is_gt_eps(float const & x){return (x > MATH_FLT_EPSILON);}

template<> bool is_lt_eps(double const & x){return (x < MATH_DBL_EPSILON);}
template<> bool is_gt_eps(double const & x){return (x > MATH_DBL_EPSILON);}

template<> bool is_lt_eps(long double const & x){return (x < MATH_LDBL_EPSILON);}
template<> bool is_gt_eps(long double const & x){return (x > MATH_LDBL_EPSILON);}

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template bool is_lt_eps(mpfr::mpreal const & x);
template bool is_gt_eps(mpfr::mpreal const & x);
//template<> bool is_lt_eps(mpfr::mpreal const & x){return (x < std::numeric_limits<mpfr::mpreal>::epsilon());}
//template<> bool is_gt_eps(mpfr::mpreal const & x){return (x > std::numeric_limits<mpfr::mpreal>::epsilon());}
#endif

template<typename T> bool math::is_zero(T const & x)
{
	if( x == 0 )
		return true;
	else if( x < 0 )
		return is_lt_eps( -x );
	else
		return is_lt_eps( x );
}

template<typename T> bool math::is_equal(T const & a, T const & b)
{
	if( a == b )
		return true;
	T c = T(a);
	c -= b;
	c /= (b==0 ? ( a==0 ? 1 : a ) : b);
	return math::is_zero<T>(c);
}

template<typename T> bool math::is_equal(T const * a, T const * b, int const & size)
{
	for(int i = 0; i < size; ++i)
		if( math::is_equal<T>(a[i], b[i]) == 0 )
			return false;
	return true;
}

// specialization integer type : int, unsigned int (u_t)
template<> bool math::is_zero(int const & x){return x==0;}
template<> bool math::is_zero(u_t const & x){return x==0u;}

template<> bool math::is_equal(int const & a, int const & b){return a==b;}
template<> bool math::is_equal(u_t const & a, u_t const & b){return a==b;}

template<> bool math::is_equal(int const * a, int const * b, int const & size)
{
	for(int i = 0; i < size; ++i) if( a[i] != b[i] ) return false;
	return true;
}
template<> bool math::is_equal(u_t const * a, u_t const * b, int const & size)
{
	for(int i = 0; i < size; ++i) if( a[i] != b[i] ) return false;
	return true;
}
/*
bool math::is_zero(int const & x){return x==0;}
bool math::is_zero(u_t const & x){return x==0u;}

bool math::is_equal(int const & a, int const & b){return a==b;}
bool math::is_equal(u_t const & a, u_t const & b){return a==b;}

bool math::is_equal(int const * a, int const * b, int const & size)
{
	for(int i = 0; i < size; ++i) if( a[i] != b[i] ) return false;
	return true;
}
bool math::is_equal(u_t const * a, u_t const * b, int const & size)
{
	for(int i = 0; i < size; ++i) if( a[i] != b[i] ) return false;
	return true;
}
*/

// specialization float type : float, double, long double
#define MATH_EQUALITY_SPEC( type )\
template bool math::is_zero(type const & x);\
template bool math::is_equal(type const & a, type const & b);\
template bool math::is_equal(type const * a, type const * b, int const & size);

MATH_EQUALITY_SPEC( float )
MATH_EQUALITY_SPEC( double )
MATH_EQUALITY_SPEC( long double )

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
MATH_EQUALITY_SPEC( mpfr::mpreal )
#endif

#endif//__EQUALITY_HPP__
