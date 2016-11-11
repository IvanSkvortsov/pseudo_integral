#ifndef __SF_HYPERG_1F1_HPP__
#define __SF_HYPERG_1F1_HPP__
#include"sf.hyperg.1F1.h"
#include"sf.hyperg.2F0.h"

#include<limits>// numeric_limits
#include"sf.math.h"// add_exp, get_exp, ...
#include"sf.hyperg.h"// sf_hyperg::_1F1_SERIES, ..., sf_hyperg::_FAIL, sf_hyperg::ERR_MAXITER, ...

#ifdef  __SF_HYPERG_1F1_DETAIL
  #ifdef __SF_HYPERG_1F1_TIME
    #include"get.work.time.h"
    #include"smart.time.h"
  #endif//_SF_HYPERG_1F1_TIME
  #include"print.hyperg.h"// print_hyperg1, print_hyperg2
#endif//__SF_HYPERG_1F1_DETAIL

template<typename T> inline static void sf_hyperg_1F1_series1_code( T & sf_bnn, T & an, T & bn, T & u, T & del, T & val, T const & x, const int n )
{
	//u = x / (bn * n);
	sf_bnn = bn;
	sf_bnn *= n;// bn * n
	u = x;
	u /= sf_bnn;// x / (bn * n)
	u *= an;
	del *= u;
	val += del;
	an += 1;
	bn += 1;
}

#define __SF_HYPERG_1F1_SERIES1 \
if( an == 0 ) return sf_hyperg::SUCCESS;\
if( bn == 0 ) return sf_hyperg::ERR_DIVZERO;\
sf_hyperg_1F1_series1_code<T>( sf_bnn, an, bn, u, del, val, x, n );\
if( val > SER_MAX ) return sf_hyperg::ERR_OVERFLOW;// positive_values

			
//#define __SF_HYPERG_1F1_SERIES1 \
if( an == 0 )\
{\
	_stat = sf_hyperg::SUCCESS;\
	break;\
}\
if( bn == 0 )\
{\
	_stat = sf_hyperg::ERR_DIVZERO;\
	break;\
}\
if( n > MAXITER )\
{\
	_stat = sf_hyperg::ERR_MAXITER;\
	break;\
}\
sf_hyperg_1F1_series1_code<T>( sf_bnn, an, bn, u, del, val, x, n );\
if( val > SER_MAX )\
{\
	_stat = sf_hyperg::ERR_OVERFLOW;/* positive_values */\
	break;\
}

// 1F1(a,b,x)
// sum_i=0^infty a_n x^n / (b_n n!)
// |x| < 100
template<typename T> int sf_hyperg_1F1_series1(T const & a, T const & b, T const & x, T & val, int & n, const int n_iter )
{
#if defined (__SF_HYPERG_1F1_TIME) && defined (__SF_HYPERG_1F1_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() / 128;
	static const int MAXITER   = 300;
	T an = T(a), bn = T(b);
	T u, sf_bnn, del = T(1);
	T err_v = T(1);

	n = 1;
	val = 1;
	if( n_iter <= 0 )
	{
__1F1_SERIES1_LABEL:
		for( ; err_v > SER_EPSILON; ++n )// for positive values
		{
			if( n > MAXITER ) return sf_hyperg::ERR_MAXITER;
			//std::clog << ' ' << n << "  1F1_series1 : " << val << " x= " << x << ' ' << an << ' ' << del << ' ' << err_v << std::endl;
			__SF_HYPERG_1F1_SERIES1;
			err_v = del; err_v /= val;
			//std::clog << ' ' << n << "  1F1_series1 : " << val << " x= " << x << ' ' << an << ' ' << del << ' ' << err_v << std::endl;
		}
	}
	else if( n_iter > MAXITER )
	{
		return sf_hyperg::ERR_MAXITER;
	}
	else
	{
		for( ; n < n_iter; ++n )
		{
			__SF_HYPERG_1F1_SERIES1;
			//std::clog << ' ' << n << "  1F1_series1 : " << val << std::endl;
		}
		err_v = del; err_v /= val;
		goto __1F1_SERIES1_LABEL;
		/*
		if( err_v > SER_EPSILON )
		{
			__SF_HYPERG_1F1_SERIES1;
		}
		*/
	}
#ifdef  __SF_HYPERG_1F1_DETAIL
  #ifdef __SF_HYPERG_1F1_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F1_TIME
	print_hyperg_series1( std::cout, "sf_hyperg_1F1_series1", "1F1", "( a, b, x )", a, b, x, val, 0, err_v, n, SER_MAX, SER_EPSILON, MAXITER );
#endif
	return sf_hyperg::SUCCESS;
}

template<typename T> inline static void sf_hyperg_1F1_series2_code( T & sf_bnn, T & an, T & bn, T & u, T & del_0, T & val_0, T & del_1, T & val_1,
		T const & x, const int n )
{
	//u = x / (bn * n);
	sf_bnn = bn;
	sf_bnn *= n;// bn * n
	u = x;
	u /= sf_bnn;// x / (bn * n)
	del_0 *= u;
	del_0 *= an;
	val_0 += del_0;

	an += 1;
	bn += 1;

	del_1 *= u;
	del_1 *= an;
	val_1 += del_1;
}

#define __SF_HYPERG_1F1_SERIES2 \
if( an == 0 ) break; \
if( bn == 0 ) return sf_hyperg::ERR_DIVZERO;\
sf_hyperg_1F1_series2_code<T>( sf_bnn, an, bn, u, del_0, val_0, del_1, val_1, x, n );\
if( val_0 > SER_MAX ) return sf_hyperg::ERR_OVERFLOW;/* positive_values */\
if( val_1 > SER_MAX ) return sf_hyperg::ERR_OVERFLOW;/* positive_values */

// 1F1(a,b,x)
// sum_i=0^infty a_n x^n / (b_n n!)
// |x| < 100
//
// calculates two values : 1F1(a, b, x) and 1F1(a+1, b, x)
// results are written into 'T val_0, val_1' :
//   1F1(a,  b,x) = val_0
//   1F1(a+1,b,x) = val_1
template<typename T> int sf_hyperg_1F1_series2(T const & a, T const & b, T const & x, T * v, int & n, const int n_iter )
{
#if defined (__SF_HYPERG_1F1_TIME) && defined (__SF_HYPERG_1F1_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() / 128;
	static const int MAXITER = 300;
	T & val_0 = v[0], & val_1 = v[1];
	T an = T(a), bn = T(b);
	T u, sf_bnn, del_0 = T(1), del_1 = T(1);
	val_0 = del_0;
	val_1 = del_1;
	T err0 = T(1), err1 = T(1);

	n = 1;
	val_0 = 1;
	val_1 = 1;
	if( n_iter <= 0 )
	{
__1F1_SERIES2_LABEL:
		for( ; err0 > SER_EPSILON || err1 > SER_EPSILON; ++n )// for positive values
		{
			if( n > MAXITER ) return sf_hyperg::ERR_MAXITER;
			__SF_HYPERG_1F1_SERIES2;
			err0 = del_0; err0 /= val_0;
			err1 = del_1; err1 /= val_1;
		}
	}
	else if( n_iter > MAXITER )
	{
		return sf_hyperg::ERR_MAXITER;
	}
	else
	{
		for( ; n < n_iter ; ++n )// for positive values
		{
			__SF_HYPERG_1F1_SERIES2;
		}
		err0 = del_0; err0 /= val_0;
		err1 = del_1; err1 /= val_1;
		goto __1F1_SERIES2_LABEL;
		/*
		if( err0 > SER_EPSILON || err1 > SER_EPSILON )
		{
			__SF_HYPERG_1F1_SERIES2;
		}
		*/
	}
#ifdef  __SF_HYPERG_1F1_DETAIL
  #ifdef __SF_HYPERG_1F1_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F1_TIME
	print_hyperg_series2( std::cout, "sf_hyperg_1F1_series2", "1F1", "( a, b, x )", "( 1+a, b, x )", a, b, x, val_0, val_1, 0, err0, err1, n,
			SER_MAX, SER_EPSILON, MAXITER );
#endif
	return sf_hyperg::SUCCESS;
}

#define __SF_HYPERG_1F1_SERIES1_LARGE_X \
if( an == 0 ) break;\
if( bn == 0 ) return sf_hyperg::ERR_DIVZERO;\
sf_hyperg_1F1_series1_code<T>( sf_bnn, an, bn, u, del, val, x, n );\
if( val > SER_MAX )/* positive_values */\
{\
	ex_s = math::get_exp(val);\
	ex += ex_s;\
	ex_s = -ex_s;\
	math::add_exp( val, ex_s );\
	math::add_exp( del, ex_s );\
}

// extended version of [sf_hyperg_1F1_series1]
//
// 1F1(a,b,x)
// sum_i=0^infty a_n x^n / (b_n n!)
template<typename T> int sf_hyperg_1F1_series1_large_x(T const & a, T const & b, T const & x, int & ex, T & val, int & n, const int n_iter )
{
#if defined (__SF_HYPERG_1F1_TIME) && defined (__SF_HYPERG_1F1_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() / 128;
	static const int MAXITER = 100000;
	T an = T(a), bn = T(b);
	T u, sf_bnn, del = T(1);
	T err_v = T(1);
	int ex_s;

	n = 1;
	ex = 0;
	val = 1;
	if( n_iter <= 0 )
	{
__SF_HYPERG_1F1_SERIES1_LARGE_X_LABEL:
		for( ; err_v > SER_EPSILON; ++n )// for positive values
		{
			if( n > MAXITER ) return sf_hyperg::ERR_MAXITER;
			__SF_HYPERG_1F1_SERIES1_LARGE_X;
			err_v = del; err_v /= val;
		}
	}
	else if( n_iter > MAXITER )
	{
		return sf_hyperg::ERR_MAXITER;
	}
	else
	{
		for( ; n < n_iter; ++n )// for positive values
		{
			__SF_HYPERG_1F1_SERIES1_LARGE_X;
		}
		err_v = del; err_v /= val;
		goto __SF_HYPERG_1F1_SERIES1_LARGE_X_LABEL;
		/*
		if( err_v > SER_EPSILON )
		{
			__SF_HYPERG_1F1_SERIES1_LARGE_X;
		}
		*/
	}
	ex_s = math::get_exp(val);
	ex += ex_s;
	math::add_exp(val, -ex_s);
#ifdef  __SF_HYPERG_1F1_DETAIL
  #ifdef __SF_HYPERG_1F1_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F1_TIME
	print_hyperg_series1( std::cout, "sf_hyperg_1F1_series1_large_x", "1F1", "( a, b, x )", a, b, x, val, ex, err_v, n,
			SER_MAX, SER_EPSILON, MAXITER );
#endif
	return sf_hyperg::SUCCESS;
}

#define __SF_HYPERG_1F1_SERIES2_LARGE_X \
if( an == 0 ) break;\
if( bn == 0 ) return sf_hyperg::ERR_DIVZERO;\
sf_hyperg_1F1_series2_code<T>( sf_bnn, an, bn, u, del_0, val_0, del_1, val_1, x, n );\
if( val_1 > SER_MAX )/* positive_values */\
{\
	ex_s = math::get_exp(val_1);\
	ex += ex_s;\
	ex_s = -ex_s;\
	math::add_exp( val_1, ex_s );\
	math::add_exp( del_1, ex_s );\
	math::add_exp( val_0, ex_s );\
	math::add_exp( del_0, ex_s );\
}

// extended version of [sf_hyperg_1F1_series2]
//
// 1F1(a,b,x)
// sum_i=0^infty a_n x^n / (b_n n!)
//
// calculates two values : 1F1(a, b, x) and 1F1(a+1, b, x)
// results are written into 'T val_0, val_1' :
//   1F1(a,  b,x) = val_0 * 2^ex_0
//   1F1(a+1,b,x) = val_1 * 2^ex_1
template<typename T> int sf_hyperg_1F1_series2_large_x(T const & a, T const & b, T const & x, int & ex, T * val, int & n, const int n_iter )
{
#if defined (__SF_HYPERG_1F1_TIME) && defined (__SF_HYPERG_1F1_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() / 128;
	static const int MAXITER = 10000000;

	T & val_0 = val[0], & val_1 = val[1];
	T an = T(a), bn = T(b);
	T u, sf_bnn, del_0 = T(1), del_1 = T(1);
	T err0 = T(1), err1 = T(1);
	int ex_s;

	ex = 0;
	n = 1;
	val_0 = 1;
	val_1 = 1;
	if( n_iter <= 0 )
	{
__SF_HYPERG_1F1_SERIES2_LARGE_X_LABEL:
		for( ; err0 > SER_EPSILON || err1 > SER_EPSILON; ++n )// for positive values
		{
			if( n > MAXITER ) return sf_hyperg::ERR_MAXITER;
			__SF_HYPERG_1F1_SERIES2_LARGE_X;
			err0 = del_0; err0 /= val_0;
			err1 = del_1; err1 /= val_1;
		}
	}
	else if( n_iter > MAXITER )
	{
		return sf_hyperg::ERR_MAXITER;
	}
	else
	{
		for( ; n < n_iter; ++n )
		{
			__SF_HYPERG_1F1_SERIES2_LARGE_X;
		}
		err0 = del_0; err0 /= val_0;
		err1 = del_1; err1 /= val_1;
		goto __SF_HYPERG_1F1_SERIES2_LARGE_X_LABEL;
		/*
		if( err0 > SER_EPSILON || err1 > SER_EPSILON )
		{
			__SF_HYPERG_1F1_SERIES2_LARGE_X;
		}
		*/
	}
	ex_s = math::get_exp(val_1);
	ex += ex_s;
	math::add_exp(val_1, -ex_s);
	math::add_exp(val_0, -ex_s);
#ifdef  __SF_HYPERG_1F1_DETAIL
  #ifdef __SF_HYPERG_1F1_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F1_TIME
	print_hyperg_series2( std::cout,"sf_hyperg_1F1_series2_large_x", "1F1","( a, b, x )","( 1+a, b, x )", a, b, x, val_0, val_1, ex, err0, err1, n,
			SER_MAX, SER_EPSILON, MAXITER );
#endif
	return sf_hyperg::SUCCESS;
}


template<class T> int sf_hyperg_1F1(T const & a, T const & b, T const & x, int & ex, T & v, int & hyperg_1f1_n1, int & n, const int n_iter )
{
	T abs_x = T( (x < 0 ? -x : x) );
	ex = 0;
	if( abs_x < 90 )
	{
		ex = 0;
		hyperg_1f1_n1 = sf_hyperg::_1F1_SERIES;
		return sf_hyperg_1F1_series1<T>(a, b, x, v, n, n_iter );
	}
	else
	{
		T bma = T(b), _1ma = T(1), x_recip = T(1);
		bma -= a;
		_1ma -= a;
		x_recip /= x;
		int stat_2f0 = sf_hyperg_2F0_series1<T>(bma, _1ma, x_recip, v );
		//int stat_2f0 = sf_hyperg_2F0_series1<T>(b-a, 1-a, 1/x, v, n_iter );
		if( stat_2f0 == sf_hyperg::SUCCESS )
		{
			ex = 0;
			hyperg_1f1_n1 = sf_hyperg::_2F0_SERIES;
			return sf_hyperg::SUCCESS;
		}
#ifdef  __SF_HYPERG_1F1_LARGE_X
		else if( stat_2f0 == sf_hyperg::ERR_MAXITER )
		{
			hyperg_1f1_n1 = sf_hyperg::_1F1_SERIES_LARGE;
			return sf_hyperg_1F1_series1_large_x<T>(a, b, x, ex, v, n, n_iter );
		}
#endif
		else
		{
			ex = 0;
			hyperg_1f1_n1 = sf_hyperg::_FAIL;
			return sf_hyperg::ERR_UNKNOWN;
		}
	}
}

template<class T> int sf_hyperg_1F1_ap1(T const & a, T const & b, T const & x, int & ex, T * v, int & hyperg_1f1_n2, int & n, const int n_iter )
{
	T abs_x = T( (x < 0 ? -x : x) );
	ex = 0;
	if( abs_x < 90 )
	{
		hyperg_1f1_n2 = sf_hyperg::_1F1_SERIES;
		return sf_hyperg_1F1_series2<T>(a, b, x, v, n, n_iter );
	}
	else
	{
		T x_recip = T(1);
		x_recip /= x;
		T bma = T(b), _1ma = T(1);
		bma  -= a;
		_1ma -= a;
		int stat_2f0_0 = sf_hyperg_2F0_series1<T>(bma, _1ma, x_recip, v[0] );
		bma  -= 1;
		_1ma -= 1;
		int stat_2f0_1 = sf_hyperg_2F0_series1<T>(bma, _1ma, x_recip, v[1] );
		//int stat_2f0_0 = sf_hyperg_2F0_series1<T>(b-a,  1-a, x_recip, v[0]);
		//int stat_2f0_1 = sf_hyperg_2F0_series1<T>(b-a-1, -a, x_recip, v[1]);
		if( stat_2f0_0 == sf_hyperg::SUCCESS && stat_2f0_1 == sf_hyperg::SUCCESS )
		{
			hyperg_1f1_n2 = sf_hyperg::_2F0_SERIES;
			return sf_hyperg::SUCCESS;
		}
#ifdef  __SF_HYPERG_1F1_LARGE_X
		else if( stat_2f0_0 == sf_hyperg::ERR_MAXITER || stat_2f0_1 == sf_hyperg::ERR_MAXITER)
		{
			hyperg_1f1_n2 = sf_hyperg::_1F1_SERIES_LARGE;
			return sf_hyperg_1F1_series2_large_x<T>(a, b, x, ex, v, n, n_iter );
		}
#endif
		else
		{
			hyperg_1f1_n2 = sf_hyperg::_FAIL;
			return sf_hyperg::ERR_UNKNOWN;
		}
	}
}


#define SF_HYPERG_1F1_SPEC( type )\
template int sf_hyperg_1F1_series1<type>(type const & a, type const & b, type const & x, type & v, int &, const int );\
template int sf_hyperg_1F1_series1_large_x(type const & a, type const & b, type const & x, int & v_ex, type & v, int & , const int );\
template int sf_hyperg_1F1_series2(type const & a, type const & b, type const & x, type * v, int & , const int );\
template int sf_hyperg_1F1_series2_large_x(type const & a, type const & b, type const & x, int & v_ex, type * v, int & , const int );\
template int sf_hyperg_1F1(type const & a, type const & b, type const & x, int & ex, type & v, int & hyperg_1f1_n1, int & , const int );\
template int sf_hyperg_1F1_ap1(type const & a, type const & b, type const & x, int & ex, type * v, int & hyperg_1f1_n2, int & , const int );

#endif//__SF_HYPERG_1F1_HPP__
