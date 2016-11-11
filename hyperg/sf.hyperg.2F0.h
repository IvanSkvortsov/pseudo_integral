#ifndef __SF_HYPERG_2F0_H__
#define __SF_HYPERG_2F0_H__

//#define __SF_HYPERG_2F0_DETAIL
#define __SF_HYPERG_2F0_TIME

template<typename T> int sf_hyperg_2F0_series1(T const & a, T const & b, T const & x, T & v, const int n_iter = 0 );
template<typename T> int sf_hyperg_2F0_series2(T const & a, T const & b, T const & x, T * v);// not good as expected
template<typename T> int sf_hyperg_2F0_poly_a(int a, T const & b, T const & x, T & v);
template<typename T> int sf_hyperg_2F0_poly_b(T const & a, int b, T const & x, T & v);
template<typename T> int sf_hyperg_2F0(T const & a, T const & b, T const & x, T & v);
template<typename T> T   sf_hyperg_2F0(T const & a, T const & b, T const & x);

#endif//__SF_HYPERG_2F0_H__

