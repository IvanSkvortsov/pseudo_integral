#ifndef __QU_HYPERG_1F11_STRUCT_H__
#define __QU_HYPERG_1F11_STRUCT_H__
#include<cmath>// exp, sqrt
#include<cstdlib>// exit
#include"sf.math.h"// fact_n, fact_2n1_n, numeric_constants
#include"sf.hyperg.h"// sf_hyperg::_1F1_SERIES, ..., sf_hyperg::type( int )
#include<cstring>// memset, memcpy

#define __QU_HYPERG_1F11_STRUCT_EXP_X
#define __QU_HYPERG_1F11_STRUCT_ERROR_ALERT

#ifdef  __QU_HYPERG_1F11_STRUCT_ERROR_ALERT
  #include<iostream>// cerr, endl
#endif

#define HYPERG_1F11_ARGS( q )      (q).a, (q).b1, (q).b2 (q).x1, (q).x2, (q).ex, (q).value, (q).hyperg_n, (q).n1, (q).n1_max, (q).n2, (q).n2_max

#define HYPERG_1F11_FLIP_ARGS( q ) (q).a, (q).b2, (q).b1 (q).x2, (q).x1, (q).ex, (q).value, (q).hyperg_n, (q).n2, (q).n2_max, (q).n1, (q).n1_max

// %hyperg_n -- used in %sf_hyperg_1F... functions and modified by that functions
// %hyperg_t -- is set before running %sf_hyperg_1F... function
// P.S. both of them [%hyperg_n, %hyperg_t] (must) have the same value
// %n -- iterator
// %n_max -- maximum iterations
template<typename T>
struct qu_hyperg_1F11_struct
{
	typedef T float_type;
#ifdef  __QU_HYPERG_1F11_STRUCT_ERROR_ALERT
	void error( const char * _method, const char _message[] = "nothing to do here" )const;
#endif
	T a, b1, b2, x1, x2, value;
	int ex;
	int n1, n1_max, n2, n2_max;
	int hyperg_n, hyperg_t;
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
	T exp_x, x;
	int exp_ex;
#endif
	int exp_ex2;
	T sqr_k1, sqr_k2;
	int a_2, lmb1, lmb2;
	T coef_g, coef;
	int _to_flip;

	qu_hyperg_1F11_struct();
	qu_hyperg_1F11_struct( qu_hyperg_1F11_struct<T> const & );
	~qu_hyperg_1F11_struct();
	qu_hyperg_1F11_struct<T> & operator=( qu_hyperg_1F11_struct<T> const & );

	void set_k1( T const & __k1 );
	void set_k2( T const & __k2 );
	void set_alp( T const & __alp );
	void set_a_2( const int __a_2 );
	void set_lmb1( const int __lmb1 );
	void set_lmb2( const int __lmb2 );
	void set_hyperg_t();
	void set_coef( T const & , T const & , T const * , const int );
	void set_coef_g();
	const bool set_flip();
	void set_n1( const int __n = 0 );
	void set_n2( const int __n = 0 );
	void set_n1_max();
	void set_n2_max();
	void set_n1_max( const int __n );
	void set_n2_max( const int __n );
	const bool to_flip()const;
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
	void comp_exp();
#endif
};

template<typename T> inline qu_hyperg_1F11_struct<T>::qu_hyperg_1F11_struct(): a(), b1(), b2(), x1(), x2(), value(), ex(),
	n1(), n1_max(), n2(), n2_max(), hyperg_n(), hyperg_t(),
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
	exp_x(), x(), exp_ex(),
#endif
	exp_ex2(),
	sqr_k1(), sqr_k2(), a_2(), lmb1(), lmb2(),
	coef_g(), coef(), _to_flip()
{}
template<typename T> inline qu_hyperg_1F11_struct<T>::qu_hyperg_1F11_struct( qu_hyperg_1F11_struct<T> const & v ):
	a( v.a ), b1( v.b1 ), b2( v.b2 ), x1( v.x1 ), x2( v.x2 ), value( v.value ), ex( v.ex ),
	n1( v.n1 ), n1_max( v.n1_max ), n2( v.n2 ), n2_max( v.n2_max ), hyperg_n( v.hyperg_n ), hyperg_t( v.hyperg_t ),
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
	exp_x( v.exp_x ), x( v.x ), exp_ex( v.exp_ex ),
#endif
	exp_ex2( v.exp_ex2 ),
	sqr_k1( v.sqr_k1 ), sqr_k2( v.sqr_k2 ), a_2( v.a_2 ), lmb1( v.lmb1 ), lmb2( v.lmb2 ),
	coef_g( v.coef_g ), coef( v.coef ), _to_flip( v._to_flip )
{}
template<typename T> inline qu_hyperg_1F11_struct<T>::~qu_hyperg_1F11_struct(){}
template<typename T> inline qu_hyperg_1F11_struct<T> & qu_hyperg_1F11_struct<T>::operator=( qu_hyperg_1F11_struct<T> const & v )
{
	if( this == &v )
		return *this;
	this->~qu_hyperg_1F11_struct<T>();
	memset( this, 0, sizeof(*this) );
	qu_hyperg_1F11_struct<T> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	memset( &tmp, 0, sizeof(*this) );
	return *this;
}
//  ---  qu_hyperg_1F11_struct ---  //
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_coef( T const & pow_k1, T const & pow_k2, T const * pown05_alp, const int pown05_max )
{
	if( this->a_2 > pown05_max )
	{
#ifdef  __QU_HYPERG_1F11_STRUCT_ERROR_ALERT
		this->error( "set_coef", "qu_hyperg_1F11_struct<T>::a_2 -gt pown05_max");
		std::cerr << "a_2 : " << this->a_2 << std::endl;
		std::cerr << "pown05_max : " << pown05_max << std::endl;
#endif
		exit(1);
	}
	this->coef  = pow_k1;
	this->coef *= pow_k2;
	if( pown05_alp[ this->a_2] == 0 )
	{
		this->error( "set_coef", "division by zero" );
		std::cerr << "1 : " << pown05_alp[0] << std::endl;
		std::cerr << "sqrt_alp : " << pown05_alp[1] << std::endl;
		std::cerr << "alp : " << pown05_alp[2] << std::endl;
		std::cerr << "a_2 : " << this->a_2 << std::endl;
		exit(1);
	}
	this->coef /= pown05_alp[ this->a_2 ];
	//this->coef /= (1ull << (this->lmb1 + this->lmb2 + 3) );
	this->coef *= (1ull << (this->lmb1 + this->lmb2 + 1) );
}

template<typename T> inline void qu_hyperg_1F11_struct<T>::set_coef_g()
{
	//    a = ( N + lmb1 + lmb2 + 1 ) / 2
	//  a_2 = ( N + lmb1 + lmb2 + 1 )
	// __az = [ a_2 / 2 ]
	// __ax = ( N + lmb1 - lmb2 )
	if( this->hyperg_t == sf_hyperg::_1F1_SERIES )
	{// Pi * Gamma( a ) / Gamma( b1 ) / Gamma( b2 )
		const int __az = this->a_2 / 2;
		if( this->a_2%2 == 0 )
		{
			this->coef_g = math::fact_n<u64_t>( __az - 1 );// ( __az - 1 )!
		} else {
			this->coef_g  = math::fact_2n1_n( __az - 1 );// ( 2 * __az - 1 )!!
			this->coef_g *= math::numeric_constants<T>::sqrt_pi;
			this->coef_g /= (1ull << __az);// 2^(__az)
		}
		u64_t __2pown = 1ull;
		__2pown <<= (this->lmb1 + this->lmb2 + 2);// 2^(lmb1 + lmb2 + 2)
		T gl = __2pown;
		gl *= math::fact_2n1_n( this->lmb1 );// ( 2 * lmb1 + 1 )!!
		gl *= math::fact_2n1_n( this->lmb2 );// ( 2 * lmb2 + 1 )!!
		if( gl == 0 )
		{
			this->error( "set_coef_g", "division by zero (1F1_SERIES)" );
			std::cerr << "lmb1 : " << this->lmb1 << std::endl;
			std::cerr << "lmb2 : " << this->lmb2 << std::endl;
			std::cerr << "(2 * lmb1 + 1)!! : " << math::fact_2n1_n( this->lmb1 ) << std::endl;
			std::cerr << "(2 * lmb2 + 1)!! : " << math::fact_2n1_n( this->lmb2 ) << std::endl;
			std::cerr << "2^(lmb1 + lmb2 + 2) : " << (1ull <<( this->lmb1 + this->lmb2 + 2 ) ) << std::endl;
			std::cerr << "gl : " << gl << std::endl;
			exit(1);
		}
		this->coef_g /= gl;
	} else if( this->hyperg_t == sf_hyperg::_2F0_SERIES )
	{// exp( x2 ) * x2^( (N + lmb1 - lmb2) / 2 - 1 ) * Pi / Gamma( lmb1 + 1.5 )
		T const * x2_ = &(this->x2);
		int const * lmb2_ = &(this->lmb2), * lmb1_ = &(this->lmb1);
		if( this->to_flip() )
		{
			x2_ = &(this->x1);
			lmb2_ = &(this->lmb1);
			lmb1_ = &(this->lmb2);
		}

		const int __ax = this->a_2 - 2 * *lmb2_ - 1;// a_2 - 2 * lmb2 - 1
		this->coef_g = sf_exp( *x2_, this->exp_ex2 );
		this->coef_g *= math::pown( *x2_, __ax / 2 - 1 );
		if( __ax % 2 )
			this->coef_g *= sqrt( *x2_ );
		u64_t __2pown = 1ull;
		__2pown <<= (*lmb1_ + 1);
		T gl = __2pown;
		gl *= math::fact_2n1_n( *lmb1_ );
		if( gl == 0 )
		{
			this->error( "set_coef_g", "division by zero (2F0_SERIES)" );
			std::cerr << "lmb1 : " << *lmb1_ << std::endl;
			std::cerr << "lmb2 : " << *lmb2_ << std::endl;
			int __ex2 = 0;
			T __exp_x2 = sf_exp( *x2_, __ex2 );
			std::cerr << "exp( x2 ) : " << __exp_x2 << ' ' << __ex2 << std::endl;
			std::cerr << "x2 : " << *x2_ << std::endl;
			std::cerr << "x2^(__n/2 - 1) : " << math::pown( *x2_, __ax/2 - 1) * (__ax%2 ? sqrt( *x2_ ) : T(1) ) << std::endl;
			std::cerr << "(2 * lmb1 + 1)!! : " << math::fact_2n1_n( *lmb1_ ) << std::endl;
			std::cerr << "2^(lmb1 + 1) : " << (1ull << (*lmb1_ + 1) ) << std::endl;
			std::cerr << "gl : " << gl << std::endl;
			exit(1);
		}
		this->coef_g /= gl;
		this->coef_g *= math::numeric_constants<T>::sqrt_pi;
	} else {
#ifdef  __QU_HYPERG_1F11_STRUCT_ERROR_ALERT
		this->error( "set_coef_g", "unknown hyperg type" );
		std::cerr << "hyperg type : " << sf_hyperg::type( this->hyperg_t ) << std::endl;
#endif
		exit(1);
	}
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_hyperg_t()
{
	static const T XARG_REF = T(90);
	T const * x2_ = ( this->to_flip() ? &(this->x1) : &(this->x2) );
	if( *x2_ < XARG_REF )
		this->hyperg_t = sf_hyperg::_1F1_SERIES;
	else
		this->hyperg_t = sf_hyperg::_2F0_SERIES;
}
template<typename T> inline const bool qu_hyperg_1F11_struct<T>::set_flip()
{
	this->_to_flip = ( this->x1 > this->x2 );
	return this->to_flip();
}
template<typename T> inline const bool qu_hyperg_1F11_struct<T>::to_flip()const
{
	return this->_to_flip;
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n1( const int __n ) { this->n1 = __n; }
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n2( const int __n ) { this->n2 = __n; }
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n1_max(){ this->n1_max = ( this->n1 > this->n1_max ? this->n1 : this->n1_max ); }
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n2_max(){ this->n2_max = ( this->n2 > this->n2_max ? this->n2 : this->n2_max ); }
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n1_max( const int __n){ this->n1_max = __n; }
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_n2_max( const int __n){ this->n2_max = __n; }
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
template<typename T> inline void qu_hyperg_1F11_struct<T>::comp_exp()
{
	this->x  = sqrt( this->x1 );
	this->x += sqrt( this->x2 );
	this->x *= this->x;
	this->exp_x = sf_exp( this->x, this->exp_ex );
}
#endif
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_k1( T const & __k )
{
	this->sqr_k1 = __k;
	this->sqr_k1 *= __k;
	this->sqr_k1 /= 4;
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_k2( T const & __k )
{
	this->sqr_k2 = __k;
	this->sqr_k2 *= __k;
	this->sqr_k2 /= 4;
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_alp( T const & __alp )
{
	this->x1 = this->sqr_k1;
	this->x1 /= __alp;
	this->x2 = this->sqr_k2;
	this->x2 /= __alp;
#ifdef  __QU_HYPERG_1F11_STRUCT_EXP_X
	this->comp_exp();
#endif
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_a_2( const int __a_2 )
{
	this->a_2 = __a_2;
	this->a = __a_2;
	this->a /= 2;
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_lmb1( const int __lmb )
{
	this->lmb1 = __lmb;
	this->b1 = __lmb;
	this->b1 += 1.5;
}
template<typename T> inline void qu_hyperg_1F11_struct<T>::set_lmb2( const int __lmb )
{
	this->lmb2 = __lmb;
	this->b2 = __lmb;
	this->b2 += 1.5;
}
#ifdef  __QU_HYPERG_1F11_STRUCT_ERROR_ALERT
template<typename T> inline void qu_hyperg_1F11_struct<T>::error( const char * _method, const char * _message )const
{
	std::cerr << "Error: [" << this << "] qu_hyperg_1F11_struct<T>::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl;
}
#endif

#endif//__QU_HYPERG_1F11_STRUCT_H__
