#ifndef __SF_HYPERG_1F1_H__
#define __SF_HYPERG_1F1_H__

//#define __SF_HYPERG_1F1_DETAIL
#define __SF_HYPERG_1F1_TIME

//#define __SF_HYPERG_1F1_LARGE_X

template<typename T> int sf_hyperg_1F1_series1(T const & a, T const & b, T const & x, T & v, int & n, const int n_iter = 0);
template<typename T> int sf_hyperg_1F1_series1_large_x(T const & a, T const & b, T const & x, int & v_ex, T & v, int & n, const int n_iter = 0);
template<typename T> int sf_hyperg_1F1_series2(T const & a, T const & b, T const & x, T * v, int & n, const int n_iter = 0);
template<typename T> int sf_hyperg_1F1_series2_large_x(T const & a, T const & b, T const & x, int & v_ex, T * v, int & n, const int n_iter = 0);
//
template<typename T> int sf_hyperg_1F1(T const & a, T const & b, T const & x, int & ex, T & v, int & hyperg_1f1_n1, int & n, const int n_iter = 0);
template<typename T> int sf_hyperg_1F1_ap1(T const & a, T const & b, T const & x, int & ex, T * v, int & hyperg_1f1_n2, int & n, const int n_iter = 0);

template<typename T> inline int sf_hyperg_1F1( T & inst )
{
	return sf_hyperg_1F1( inst.a, inst.b, inst.x, inst.ex, inst.value, inst.hyperg_n, inst.n, inst.n_max );
}

#endif//__SF_HYPERG_1F1_H__
