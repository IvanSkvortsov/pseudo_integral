#ifndef __SF_EXP_H__
#define __SF_EXP_H__
#include"exponent.2n.h"
#include<limits>// numeric_limits

typedef unsigned int u_t;

template<class T> int sf_exponent(T const & x, T & v, int & n);
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
template<> inline int sf_exponent(mpfr::mpreal const & x, mpfr::mpreal & v, int & n)
{
	v = exp( x ); n = 0;
}
#endif

template<class T> inline T sf_exp(T const & x, int & n )
{
	T value;
	sf_exponent( x, value, n );
	return value;
}

void exponent_pint(u_t x, long double & v, int & n);
void exponent_mint(u_t x, long double & v, int & n);
void exponent_int( int x, long double & v, int & n);
template<class T> int exponent_series(T const & x, T & v);

// exp( x ) = v * 2^n
// x > 0
// |x| < 2^30
inline void exponent_pint(u_t x, long double & v, int & n)
{
	v = 1;
	n = 0;
	for(int i = 0; i < 30; ++i, x >>= 1)
	{
		if( (x & 1u) == 0u )
			continue;
		v *= exponent_2n_plus[i].x;
		n += exponent_2n_plus[i].n;
	}
}

// exp( x ) = v * 2^n
// x < 0
// |x| < 2^30
inline void exponent_mint(u_t x, long double & v, int & n)
{
	x = ~x;
	x += 1u;
	v = 1;
	n = 0;
	for(int i = 0; i < 30; ++i, x >>= 1)
	{
		if( (x & 1u) == 0u )
			continue;
		v *= exponent_2n_minus[i].x;
		n += exponent_2n_minus[i].n;
	}
}

inline void exponent_int(int x, long double & v, int & n)
{
	if( x < 0 )
		exponent_mint(x, v, n);
	else
		exponent_pint(x, v, n);
}

// exp(x) := 1 + x * (1 + x/2 * (1 + x/3 * (1 + x/4 * (1 + ...
// series expansion
// |x| < 2^127
template<class T> inline int exponent_series(T const & x, T & v)
{
	static const float EPSILON = float(std::numeric_limits<T>::epsilon());
	static const T   MAXVAL = T(std::numeric_limits<T>::max() * 0.25 );
	static const int MAXITER = 1000;
	v = 1;
	v += x;
	//T c = T(0);
	//math::kahan_add<T>(v, c, x);
	float cur_eps = x, abs_cur_eps;
	cur_eps /= float(v);
	abs_cur_eps = (cur_eps < 0 ? -cur_eps : cur_eps);
	T u = T(x);
	for(int n = 2; abs_cur_eps > EPSILON; ++n )
	{
		if( n > MAXITER ) return 1;
		if( v > MAXVAL ) return 2;
		u *= x;
		u /= n;
		//math::kahan_add<T>(v, c, u);
		v += u;
		cur_eps = u;
		cur_eps /= float(v);
		abs_cur_eps = (cur_eps < 0 ? -cur_eps : cur_eps);
	}
	return 0;
}

#endif//__U_EXP_H__
