#ifndef __QU_HYPERG_1F1_STRUCT_H__
#define __QU_HYPERG_1F1_STRUCT_H__
#include<cmath>// exp, sqrt
#include<cstdlib>// exit
#include"sf.math.h"// fact_n, fact_2n1_n, numeric_constants
#include"sf.hyperg.h"// sf_hyperg::_1F1_SERIES, ..., sf_hyperg::type( int )
#include<cstring>// memset, memcpy

#define __QU_HYPERG_1F1_STRUCT_DEBUG
#ifdef  __QU_HYPERG_1F1_STRUCT_DEBUG
  #include<cassert>
  #define __qu_1f1_assert__( arg ) assert( (arg) )
#else
  #define __qu_1f1_assert__( arg )
#endif

#define __QU_HYPERG_1F1_STRUCT_EXP_X
#define __QU_HYPERG_1F1_STRUCT_ERROR_ALERT

#ifdef  __QU_HYPERG_1F1_STRUCT_ERROR_ALERT
  #include<iostream>// cerr, endl
#endif

// %hyperg_n -- used in %sf_hyperg_1F... functions and modified by that functions
// %hyperg_t -- is set before running %sf_hyperg_1F... function
// P.S. both of them [%hyperg_n, %hyperg_t] (must) have the same value
// %n -- iterator
// %n_max -- maximum iterations
template<typename T>
struct qu_hyperg_1F1_struct
{
	typedef T float_type;
#ifdef  __QU_HYPERG_1F1_STRUCT_ERROR_ALERT
	void error( const char * _method, const char _message[] = "nothing to do here" )const;
#endif
	T a, b, x, value;
	int ex;
	int n, n_max;
	int hyperg_n, hyperg_t;
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
	T exp_x;
#endif
	int exp_ex;
	T sqr_k;
	int a_2, lmb;
	T coef_g, coef;

	qu_hyperg_1F1_struct();
	qu_hyperg_1F1_struct( qu_hyperg_1F1_struct<T> const & );
	~qu_hyperg_1F1_struct();
	qu_hyperg_1F1_struct<T> & operator=( qu_hyperg_1F1_struct<T> const & );

	void set_k( T const & __k );// sqr_k = k * k
	void set_alp( T const & __alp );
	void set_a_2( const int __a2 );// ( N + lmb + 1 )
	void set_lmb( const int __lmb );// b = lmb + 1.5
	void set_hyperg_t();// x < 90 ?
	void set_n( const int __n = 0 );
	void set_n_max();// n_max < n ? n : n_max
	void set_n_max( const int __n );
	void set_coef_g();
	void set_coef( T const & pow_k, T const * pown05_alp, const int pown05_max );
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
	void comp_exp();
#endif
};

template<typename T> inline qu_hyperg_1F1_struct<T>::qu_hyperg_1F1_struct(): a(), b(), x(), value(), ex(),
	n(), n_max(), hyperg_n(), hyperg_t(),
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
	exp_x(),
#endif
	exp_ex(),
	sqr_k(), a_2(), lmb(),
	coef_g(), coef()
{}
template<typename T> inline qu_hyperg_1F1_struct<T>::qu_hyperg_1F1_struct( qu_hyperg_1F1_struct<T> const & v ):
	a( v.a ), b( v.b ), x( v.x ), value( v.value ), ex( v.ex ),
	n( v.n ), n_max( v.n_max ), hyperg_n( v.hyperg_n ), hyperg_t( v.hyperg_t ),
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
	exp_x( v.exp_x ),
#endif
	exp_ex( v.exp_ex ),
	sqr_k( v.sqr_k ), a_2( v.a_2 ), lmb( v.lmb ),
	coef_g( v.coef_g ), coef( v.coef )
{}
template<typename T> inline qu_hyperg_1F1_struct<T>::~qu_hyperg_1F1_struct(){}
template<typename T> inline qu_hyperg_1F1_struct<T> & qu_hyperg_1F1_struct<T>::operator=( qu_hyperg_1F1_struct<T> const & v )
{
	if( this == &v )
		return *this;
	this->~qu_hyperg_1F1_struct<T>();
	memset( this, 0, sizeof(*this) );
	qu_hyperg_1F1_struct<T> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	memset( &tmp, 0, sizeof(*this) );
	return *this;
}
//  ---  qu_hyperg_1F1_struct ---  //
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_coef( T const & pow_k, T const * pown05_alp, const int pown05_max )
{
	if( this->a_2 > pown05_max )
	{
#ifdef  __QU_HYPERG_1F1_STRUCT_ERROR_ALERT
		this->error("set_coef", "qu_hyperg_1F1_struct<T>::a_2 -gt pown05_max");
		std::cerr << "a_2 : " << this->a_2 << std::endl;
		std::cerr << "pown05_max : " << pown05_max << std::endl;
#endif
		exit(1);
	}
	this->coef = pow_k;
	if( pown05_alp[this->a_2] == 0 )
	{
		this->error( "set_coef", "division by zero" );
		exit(1);
	}
	this->coef /= pown05_alp[ this->a_2 ];
	if( this->lmb )
		this->coef *= (1ull << (this->lmb));// + 2));
}
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_hyperg_t()
{
	static const T XARG_REF = T(90);
	if( this->x < XARG_REF )
		this->hyperg_t = sf_hyperg::_1F1_SERIES;
	else
		this->hyperg_t = sf_hyperg::_2F0_SERIES;
}
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_coef_g()
{
	if( this->hyperg_t == sf_hyperg::_1F1_SERIES )
	{// pi * Gamma( a ) / Gamma( b )
		const int __az = this->a_2 / 2;
		if( this->a_2 % 2 == 0 )
		{
			this->coef_g = math::fact_n<u64_t>( __az - 1 );// ( __az - 1 )!
		} else {
			this->coef_g  = math::fact_2n1_n( __az - 1 );// ( 2 * __az - 1 )!!
			this->coef_g *= math::numeric_constants<T>::sqrt_pi;
			this->coef_g /= (1ull << __az );
		}
		u64_t gl = 1ull;
		gl <<= (this->lmb + 1);
		gl *= math::fact_2n1_n( this->lmb );
		if( gl == 0 )
		{
			this->error( "set_coef_g", "division by zero" );
			exit(1);
		}
		this->coef_g /= gl;
	} else if( this->hyperg_t == sf_hyperg::_2F0_SERIES )
	{// exp( x ) * x^( (N-lmb)/2 - 1 ) * sqrt_pi
		const int __ax = this->a_2 - 2 * this->lmb - 1;// __ax = N - lmb; P.S. a_2 = N + lmb + 1
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
		this->coef_g = this->exp_x;
#else
		this->coef_g = sf_exp( this->x, this->exp_ex );
#endif
		this->coef_g *= math::pown( this->x, __ax/2 - 1 );
		if( __ax % 2 )
			this->coef_g *= sqrt( this->x );
		this->coef_g *= math::numeric_constants<T>::sqrt_pi;
	} else {
#ifdef  __QU_HYPERG_1F1_STRUCT_ERROR_ALERT
		this->error( "set_coef_g", "unknown hyperg type" );
		std::cerr << "hyperg type : " << sf_hyperg::type( this->hyperg_t ) << std::endl;
#endif
		exit(1);
	}
}
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_n( const int __n ){ this->n = __n; }
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_n_max(){ this->n_max = ( this->n > this->n_max ? this->n : this->n_max ); }
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_n_max( const int __n_max ){ this->n_max = __n_max; }
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
template<typename T> inline void qu_hyperg_1F1_struct<T>::comp_exp()
{
	this->exp_x = sf_exp( this->x, this->exp_ex );
	/*
	if( this->exp_ex < std::numeric_limits<T>::max_exponent )
	{
		math::add_exp( this->exp_x, this->exp_ex );
		this->exp_ex = 0;
	}
	*/
}
#endif
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_k( T const & __k )
{
	this->sqr_k = __k;
	this->sqr_k *= __k;
	this->sqr_k /= 4;
	__qu_1f1_assert__( this->sqr_k != 0 );
}
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_alp( T const & __alp )
{
	this->x = this->sqr_k;
	this->x /= __alp;
#ifdef  __QU_HYPERG_1F1_STRUCT_EXP_X
	this->comp_exp();
#endif
}

template<typename T> inline void qu_hyperg_1F1_struct<T>::set_a_2( const int __a_2 )
{
	this->a_2 = __a_2;
	this->a = __a_2;
	this->a /= 2;
}
template<typename T> inline void qu_hyperg_1F1_struct<T>::set_lmb( const int __lmb )
{
	this->lmb = __lmb;
	this->b = __lmb;
	this->b += 1.5;
}
#ifdef  __QU_HYPERG_1F1_STRUCT_ERROR_ALERT
template<typename T> inline void qu_hyperg_1F1_struct<T>::error( char const * _method, char const * _message )const
{
	std::cerr << "Error: [" << this << "] qu_hyperg_1F1_struct<T>::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl;
}
#endif

#endif//__QU_HYPERG_1F1_STRUCT_H__
