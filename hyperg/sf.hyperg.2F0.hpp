#ifndef __SF_HYPERG_2F0_HPP__
#define __SF_HYPERG_2F0_HPP__
#include"sf.hyperg.2F0.h"
#include"sf.hyperg.h"// sf_hyperg::ERR_...,
#include<cmath>// floor
#include<limits>// numeric_limits

#ifdef  __SF_HYPERG_2F0_DETAIL
  #ifdef __SF_HYPERG_2F0_TIME
    #include"get.work.time.h"// get_work_time
    #include"smart.time.h"// const char * smart_time( get_work_time const & )
  #endif//_SF_HYPERG_1F1_TIME
  #include"print.hyperg.h"// print_hyperg_series1( ... ), ...
#endif

template<class T> inline static void sf_hyperg_2F0_series1_code( T & an, T & bn, T & u, T & del, T & v, T const & x, const int n )
{
	//u = an * (bn/n * x);
	u  = an;
	u *= bn;
	u /= n;
	u *= x;
	// value
	del *= u;
	v += del;
	an += 1;
	bn += 1;
}

template<class T> int sf_hyperg_2F0_series1(T const & a, T const & b, T const & x, T & v, const int n_iter )
{
#if defined (__SF_HYPERG_2F0_TIME) && defined (__SF_HYPERG_2F0_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() / 128;
	static const int MAXITER = 100;
	T u, err_v = T(1);
	T an = T(a), bn = T(b), del = T(1);
	int n;
	v = 1;
	if( n_iter <= 0 )
	{
		for( n = 1; err_v > SER_EPSILON; ++n )
		{
			if( n > MAXITER ) return sf_hyperg::ERR_MAXITER;
			if( an == 0 || bn == 0 ) break;
			sf_hyperg_2F0_series1_code( an, bn, u, del, v, x, n );
			err_v = del;
			err_v /= v;
			err_v = (err_v < 0 ? -err_v : err_v );
		}
	}
	else if( n_iter > MAXITER ) return sf_hyperg::ERR_MAXITER;
	else
	{
		for( n = 0; n < n_iter; ++n )
		{
			if( an == 0 || bn == 0 ) break;
			sf_hyperg_2F0_series1_code( an, bn, u, del, v, x, n );
		}
		err_v = del;
		err_v /= v;
		err_v = (err_v < 0 ? -err_v : err_v );
	}
#ifdef  __SF_HYPERG_2F0_DETAIL
  #ifdef __SF_HYPERG_2F0_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F1_TIME
	print_hyperg_series1( std::cout, "sf_hyperg_2F0_series1", "2F0", "( a, b, x )", a, b, x, v, 0, err_v, n, SER_MAX, SER_EPSILON, MAXITER );
#endif
	return sf_hyperg::SUCCESS;
}

template<class T>
int sf_hyperg_2F0_series2(T const & a, T const & b, T const & x, T * v)
{
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const int MAXITER = 100;
	T & v0 = v[0], & v1 = v[1];
	v0 = 0;
	v1 = 0;
	T u, u0, u1, err_v0 = T(1), err_v1 = T(1);
	T an = T(a), bn = T(b), del_0 = T(1), del_1 = T(1);
	int n = 1;
	for(; n < MAXITER && (err_v0 > SER_EPSILON || err_v1 > SER_EPSILON); ++n)
	{
		if( an == 0 || bn == 0 ) break;
		//u = an * (bn/n * x);
		u = x;
		u /= n;
		u0  = an;
		u0 *= bn;
		u0 *= u;
		an += 1;
		bn += 1;
		u1  = an;
		u1 *= bn;
		u1 *= u;
		// value
		del_0 *= u0;
		v0 += del_0;
		del_1 *= u1;
		v1 += del_1;
		// error
		err_v0 = del_0;
		err_v0 /= v0;
		err_v0 = (err_v0 < 0 ? -err_v0 : err_v0 );
		err_v1 = del_1;
		err_v1 /= v1;
		err_v1 = (err_v1 < 0 ? -err_v1 : err_v1 );
	}
	v0 += 1;
	v1 += 1;
	if( n == MAXITER )
		return sf_hyperg::ERR_MAXITER;
	return sf_hyperg::SUCCESS;
}

template<typename T> int sf_hyperg_2F0_poly_a(int a, T const & b, T const & x, T & v)
{
	if( a > 0 ) return sf_hyperg::ERR_UNKNOWN;
	a = -a;
	v = 1;
	T u;
	T corr = T(0);
	for(int k = a-1; k >= 0; --k)
	{
		//u = -x * (a-k) * (b-k) / (k+1);
		//v = 1 + u * v;
		u = -x;
		u *= (a-k);
		u *= (b-k);
		u /= (k+1);
		v *= u;
		v += 1;
	}
	return sf_hyperg::SUCCESS;
}

template<typename T>
int
sf_hyperg_2F0_poly_b(T const & a, int b, T const & x, T & v)
{
	if( b > 0 ) return sf_hyperg::ERR_UNKNOWN;
	b = -b;
	v = 1;
	T u;
	for(int k = b-1; k >= 0; --k)
	{
		//u = -x * (b-k) * (b-k) / (k+1);
		//v = 1 + u * v;
		u = -x;
		u *= (b-k);
		u *= (a-k);
		u /= (k+1);
		v *= u;
		v += 1;
	}
	return sf_hyperg::SUCCESS;
}

template<typename T> int sf_hyperg_2F0(T const & a, T const & b, T const & x, T & v)
{
	if( a == floor(a) && a <= 0 )
		return sf_hyperg_2F0_poly_a<T>( (int)(a), b, x, v);
	if( b == floor(b) && b <= 0 )
		return sf_hyperg_2F0_poly_b<T>(a, (int)(b), x, v);
	return sf_hyperg_2F0_series1<T>(a, b, x, v);
}

template<class T> T sf_hyperg_2F0(T const & a, T const & b, T const & x)
{
	T v;
	int stat_s = sf_hyperg_2F0<T>(a, b, x, v);
	return v;
}

#define SF_HYPERG_2F0_SPEC( type )\
template int sf_hyperg_2F0_series1(type const & a, type const & b, type const & x, type & v, const int );\
template int sf_hyperg_2F0_series2(type const & a, type const & b, type const & x, type * v);\
template int sf_hyperg_2F0_poly_a(int a, type const & b, type const & x, type & v);\
template int sf_hyperg_2F0_poly_b(type const & a, int b, type const & x, type & v);\
template int sf_hyperg_2F0(type const & a, type const & b, type const & x, type & v);\
template type   sf_hyperg_2F0(type const & a, type const & b, type const & x);

#endif//__SF_HYPERG_2F0_HPP__
