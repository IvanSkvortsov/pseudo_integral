#ifndef __Q_INT_H__
#define __Q_INT_H__
#include"sf.hyperg.1F11.h"
#include"sf.hyperg.1F1.h"
#include"sf.hyperg.h"
#include"qu.hyperg.1F1.struct.h"
#include"qu.hyperg.1F11.struct.h"
#include"sf.math.h"

template<typename T> T q_int_1f11( const int N, const int lmb_a, const int lmb_b, T const & kA, T const & kB, T const & alp )
{
	qu_hyperg_1F11_struct<T> q;
	q.set_k1( kA );
	q.set_k2( kB );
	q.set_alp( alp );// x1 = k1^2/(4 * alp); x2 = k2^2/(4 * alp)
	q.set_a_2( N + lmb_a + lmb_b + 1 );
	q.set_lmb1( lmb_a );
	q.set_lmb2( lmb_b );
	q.set_hyperg_t();
	q.set_flip();
	T pown_alp[ q.a_2 + 1 ];
	pown_alp[ q.a_2 ] = math::pown<T>( alp, q.a_2/2 ) * ( q.a_2%2 ? sqrt( alp ) : T(1) );
	q.set_coef( math::pown<T>( kA, lmb_a ), math::pown<T>( kB, lmb_b ), pown_alp, q.a_2 );
	q.set_coef_g();
	if( q.to_flip() )
	{
		sf_hyperg_1F11_series_flip( q );
	} else {
		sf_hyperg_1F11_series( q );
	}
	T value = q.value;
	value *= q.coef;
	value *= q.coef_g;
	return value;
}

template<typename T> T q_int_1f1( const int N, const int lmb, T const & k, T const & alp )
{
	qu_hyperg_1F1_struct<T> q;
	q.set_k( k );
	q.set_alp( alp );// x1 = k1^2/(4 * alp); x2 = k2^2/(4 * alp)
	q.set_a_2( N + lmb + 1 );
	q.set_lmb( lmb );
	q.set_hyperg_t();
	T pown_alp[ q.a_2 + 1 ];
	pown_alp[ q.a_2 ] = math::pown<T>( alp, q.a_2/2 ) * ( q.a_2%2 ? sqrt( alp ) : T(1) );
	q.set_coef( math::pown<T>( k, lmb ), pown_alp, q.a_2 );
	q.set_coef_g();
	sf_hyperg_1F1( q );
	T value = q.value;
	value *= q.coef;
	value *= q.coef_g;
	return value;
}
#endif//__Q_INT_H__
