#ifndef __POLY_N_H__
#define __POLY_N_H__
#include"array.n.h"// ./array/

#define __PRINT_POLY_N
#ifdef  __PRINT_POLY_N
#include<iostream>
#endif

template<class T, int _size = 3>
struct poly_n
{
	typedef T value_type;
	typedef T float_type;
	T d;
	array_n<int,_size> x;

	poly_n();
	poly_n(int const * __x);
	poly_n(T const & __d, int const * __x);
	poly_n(poly_n<T,_size> const & v):d(v.d), x(v.x){}
	poly_n<T, _size> & operator=(poly_n<T,_size> const & v);
	poly_n<T, _size> & operator*=(poly_n<T,_size> const & v);

	poly_n<T,_size> & operator*=(T const & v);
	int & operator[](int i);
	int const & operator[](int i)const;
	static const int size();
	void set_x(int const * __x);
	int sum_x()const;
	void set_zero_x();
	bool is_even_x()const;
	bool is_equal_x(poly_n<T,_size> const & v)const;
	static bool is_equal_x(poly_n<T,_size> const & a, poly_n<T,_size> const & b);
	T calc_r(T const * xval)const;
#ifdef  __PRINT_POLY_N
	void print(std::ostream & out, int prec, int w_x)const;
#endif
};

template<typename T> using poly_3 = poly_n<T,3>;
template<typename T> using poly_4 = poly_n<T,4>;

template<class T, int _size>
poly_n<T,_size> operator*(poly_n<T,_size> const & a, poly_n<T,_size> const & b);

template<class T, int _size>
bool operator==(poly_n<T,_size> const & a, poly_n<T,_size> const & b);

template<class T, int _size>
bool operator!=(poly_n<T,_size> const & a, poly_n<T,_size> const & b);

#endif//__POLY_N_H__
