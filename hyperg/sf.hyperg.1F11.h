#ifndef __SF_HYPERG_1F11_H__
#define __SF_HYPERG_1F11_H__

//#define __SF_HYPERG_1F11_DETAIL
#define __SF_HYPERG_1F11_TIME// don't work without '..._DETAIL' definition

#define __SF_HYPERG_1F11_ERROR_MSG
#define __SF_HYPERG_1F11_LARGE_X1

template<class T> int sf_hyperg_1F11_series(T const & a, T const & b1, T const & b2, T const & x1, T const & x2, int & ex, T & v, int & hyperg_n,
		int & n1, int & n2, const int n1_iter, const int n2_iter );

template<typename T> inline int sf_hyperg_1F11_series( T & q )
{
	return sf_hyperg_1F11_series( q.a, q.b1, q.b2, q.x1, q.x2, q.ex, q.value, q.hyperg_n, q.n1, q.n2, q.n1_max, q.n2_max );
}
template<typename T> inline int sf_hyperg_1F11_series_flip( T & q )
{
	return sf_hyperg_1F11_series( q.a, q.b2, q.b1, q.x2, q.x1, q.ex, q.value, q.hyperg_n, q.n2, q.n1, q.n2_max, q.n1_max );
}

#endif//__SF_HYPERG_1F11_H__
