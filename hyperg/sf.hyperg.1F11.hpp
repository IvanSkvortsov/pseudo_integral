#ifndef __SF_HYPERG_1F11_HPP__
#define __SF_HYPERG_1F11_HPP__
#include"sf.hyperg.h"
#include"sf.hyperg.1F11.h"
#include"sf.hyperg.1F1.h"
#include<limits>// numeric_limits
#include"sf.math.h"// add_exp, get_exp

#ifdef  __SF_HYPERG_1F11_DETAIL
  #ifdef __SF_HYPERG_1F11_TIME
    #include"get.work.time.h"
    #include"smart.time.h"
  #endif//_SF_HYPERG_1F11_TIME
  #include"print.hyperg.h"
#endif

#ifdef  __SF_HYPERG_1F11_ERROR_MSG
  #include<iostream>
  #include<iomanip>
template<typename T> inline static void print_1f11_errmsg( const char * _message, const char * name, T const & value, const int ex, const int prec )
{
	std::cerr << "Error: [sf_hyperg_1F11_series] " <<
		std::setw(20) << _message << "; " <<
		std::setw(24) << name << " : " <<
		std::setw( prec + 10 ) << std::setprecision( prec ) << std::scientific << value <<
		std::setw( 8 ) << ex << std::endl;
}
#endif

// Ivan Skvortsov
// 2016.01.26  13:22
//
// 1F11(a, b1, b2, x1, x2) = 
//   Sum_i=0^infty a_i x1^i / (b1_i i!) Sum_j=0^infty (a+i)_j x2^j / (b2_j j!)
//   -eq
//   Sum_i=0^infty a_i x1^i / (b1_i i!) 1F1(a+i, b2, x2)
template<typename T>
int sf_hyperg_1F11_series(T const & a, T const & b1, T const & b2, T const & x1, T const & x2, int & ex, T & val, int & hyperg_1f11_n,
		int & n1, int & n2, const int n1_iter, const int n2_iter )
{
#if defined (__SF_HYPERG_1F11_TIME) && defined (__SF_HYPERG_1F11_DETAIL)
	get_work_time t1;
	t1.counting_start();
#endif
	static const T SER_EPSILON = std::numeric_limits<T>::epsilon();
	static const T SER_MAX     = std::numeric_limits<T>::max() * 0.06125;// 2^(-4)
	static const T SER_MIN     = std::numeric_limits<T>::min() * 8192;// 2^13
	static const int MAXITER   = 100000;
	T abs_x1 = T( (x1 < 0 ? -x1 : x1) ), abs_x2 = T((x2 < 0 ? -x2 : x2));
	ex = 0;
	T M_a[2] = {T(0), T(0)};
	hyperg_1f11_n = 0;
	n2 = 0;
	int stat_1f1 = sf_hyperg_1F1_ap1<T>(a, b2, x2, ex, M_a, hyperg_1f11_n, n2, n2_iter );
	if( stat_1f1 )
		return stat_1f1;
	T Mam1 = M_a[0], Ma = M_a[1], Map1;
	// Mam1 = 1F1(a  ,b2,x2)
	// Ma   = 1F1(a+1,b2,x2)
	if( hyperg_1f11_n == sf_hyperg::_2F0_SERIES )
	{
		// x2 -> +Infinity
		// M(a,   b2, x2) =          Gamma(a)/Gamma(b) * exp(x2) * (x2)^(a-b) * 2F0(b2-a, 1-a, 1/x2)
		// M(a+1, b2, x2) = (a*x2) * Gamma(a)/Gamma(b) * exp(x2) * (x2)^(a-b) * 2F0(b2-a-1,-a, 1/x2)
		// P.S. =)
		// a * Gamma(a) = Gamma(a+1)
		// x2 * x2^(a-b) = x2^(a+1-b)
		Ma *= a;
		Ma *= x2;// M(a+1, b2, x2)
	}
	else if( hyperg_1f11_n == sf_hyperg::_1F1_SERIES_LARGE )
	{
		int ex_s = math::get_exp( Ma );
		ex += ex_s;
		math::add_exp( Ma, -ex_s );
		math::add_exp( Mam1, -ex_s );
	}
#ifdef  __SF_HYPERG_1F11_DETAIL
	print_hyperg_1f11_series( std::cout, "sf_hyperg_1F11_series", "1F1", "(a-1,b2,x2)", "(a,  b2,x2)", a, b1, b2, x1, x2,
			Mam1, Ma, ex, MAXITER, SER_EPSILON, SER_MAX, SER_MIN, hyperg_1f11_n );
#endif
	//------------------------- Recursion -------------------------//
	// recurrence relation : Map1 = ( (2*a+x-b)*Ma - (a-b)*Mam1 )/a;
	// Mam1 = 1F1(a-1,b,x)
	// Ma   = 1F1(a,b,x)
	// Map1 = 1F1(a+1,b,x)
	T an = T(a);
	T bn = T(b1);
	n1 = 1;
	//
	T u = T(0), del = T(1);
	T el_v = T(0);
	T bnn = T(0);
	//
	val = Mam1;// M(a,b2,x2)
	u = x1;
	u *= an;
	u /= bn;
	del = u;
	el_v = u;
	el_v *= Ma;// a * x1 / b1 * M(a+1,b2,x2)
	val += el_v;
	T err_v = T(el_v);
	err_v /= val;
	//
	an += 1;
	bn += 1;
	++n1;
	T amb = an;
	amb -= b2;
	//
	T _2a_x_b = an;
	_2a_x_b *= 2;
	_2a_x_b += x2;
	_2a_x_b -= b2;
	//
	T Ma_2axb;
	T Mam1_amb;
#ifdef  __SF_HYPERG_1F11_DETAIL
	print_hyperg_1f11_title( std::cout );
#endif
	//main loop
	int stat = sf_hyperg::SUCCESS, ex_s = 0, ex_d = 0, exv = 0;
	for( ; err_v > SER_EPSILON; ++n1 )// an = a+1, bn = b1+1, n1 = 2
	{
		if( n1 > MAXITER )
		{
			stat = sf_hyperg::ERR_MAXITER;
			break;
		}
		// Mam1 * (a-b)
		Mam1_amb = amb;
		Mam1_amb *= Mam1;
		// Ma * (2*a + x - b)
		Ma_2axb = _2a_x_b;
		Ma_2axb *= Ma;
		if( Ma_2axb >= SER_MAX )
		{
#ifdef  __SF_HYPERG_1F11_LARGE_X1
			//ex_s = math::get_exp( Ma_2axb );
			ex_s = math::get_exp( Ma );
			ex += ex_s;
			ex_s = -ex_s;
			math::add_exp( Ma_2axb, ex_s );
			math::add_exp( Mam1_amb,ex_s );
			math::add_exp( val, ex_s );
			math::add_exp( Ma,  ex_s );
#else
			stat = sf_hyperg::ERR_OVERFLOW;
  #ifdef  __SF_HYPERG_1F11_ERROR_MSG
			print_1f11_errmsg( "OVERFLOW", "Ma_2axb", Ma_2axb, ex_s, __PRINT_FLT_PREC );
  #endif
			break;
#endif//__SF_HYPERG_1F11_LARGE_X1
		}
		// Map1 = (Ma * (2*a+x-b) - Mam1 * (a-b)) / a
		Map1 = Ma_2axb;
		Map1 -= Mam1_amb;
		// u = x1 / ((b+n-1) * n)
		u = x1;
		bnn = bn;
		bnn *= n1;
		u /= bnn;
		u *= an;
		//
		del *= u;
		Map1 /= an;
		if( del <= SER_MIN )
		{
#ifdef  __SF_HYPERG_1F11_LARGE_X1
			ex_d = math::get_exp( del );
			//exv += ex_d;
			math::add_exp( del, -ex_d );
			math::add_exp( Ma, ex_d );
			math::add_exp( Map1, ex_d );
#else
			stat = sf_hyperg::ERR_UNDERFLOW;
  #ifdef  __SF_HYPERG_1F11_ERROR_MSG
			print_1f11_errmsg( "UNDERFLOW", "del", del, ex_d, __PRINT_FLT_PREC );
  #endif
			break;
#endif//__SF_HYPERG_1F11_LARGE_X1
		}
		el_v = del;
		el_v *= Map1;// u * (a+n-1) * M(a+n, b, x)
		//if( exv != 0 )
		//	math::add_exp( el_v, exv);
		val += el_v;
		if( val > SER_MAX )
		{
#ifdef  __SF_HYPERG_1F11_LARGE_X1
			ex_s = math::get_exp( val );
			ex += ex_s;
			ex_s = -ex_s;
			math::add_exp( val, ex_s );
			math::add_exp( Ma,  ex_s );
			math::add_exp( Map1,ex_s );
#else
			stat = sf_hyperg::ERR_OVERFLOW;
  #ifdef  __SF_HYPERG_1F11_ERROR_MSG
			print_1f11_errmsg( "OVERFLOW", "1F11(a, b1, b2, x1, x2)", val, ex_s, __PRINT_FLT_PREC );
  #endif
			break;
#endif//__SF_HYPERG_1F11_LARGE_X1
		}
		amb += 1;
		_2a_x_b += 2;
		err_v = el_v;
		err_v /= val;
		err_v = (err_v < 0 ? -err_v : err_v);
#ifdef  __SF_HYPERG_1F11_DETAIL
		print_hyperg_1f11_series_idx( std::cout, Map1, del, el_v, val, ex, err_v, n1 );
#endif
		Mam1 = Ma;
		Ma   = Map1;
		//
		an += 1;
		bn += 1;
	}
	if( ex != 0 )
	{
		ex_s = math::get_exp(val);
		ex += ex_s;
		math::add_exp( val, -ex_s );
		math::add_exp( el_v, -ex_s );
		math::add_exp( Map1, -ex_s );
	}
#ifdef  __SF_HYPERG_1F11_DETAIL
	std::cout << std::endl;
  #ifdef __SF_HYPERG_1F11_TIME
	t1.counting_end();
	std::cout << "Time: " << smart_time( t1 ) << std::endl;
  #endif//_SF_HYPERG_1F11_TIME
	print_hyperg_1f11_series_idx( std::cout, Map1, del, el_v, val, ex, err_v, n1-1 );
	for(int k = 0; k < 4*( __PRINT_FLT_PREC + 10 ) + 26; ++k) std::cout << '-'; std::cout << std::endl;
	std::cout.unsetf( std::ios::scientific );
#endif
	return stat;
}

#define SF_HYPERG_1F11_SER_SPEC( type )\
template int sf_hyperg_1F11_series(type const & a, type const & b1, type const & b2, type const & x1, type const & x2, \
		int & ex, type & val, int & hyperg_1f11_n, int & , int & , const int , const int );

#endif//__SF_HYPERG_1F11_HPP__
