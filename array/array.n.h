#ifndef __ARRAY_N_H__
#define __ARRAY_N_H__

template<typename T, int _size>
struct array_n
{
	T _data[_size];
	array_n();
	array_n( array_n<T, _size> const & v);
	array_n(T const * v);
	~array_n();
	array_n<T,_size> & operator=(array_n<T,_size> const & v);
	array_n<T,_size> & operator=(T const * v);
	T * data();
	T const * data()const;
	const int size()const;
	T & operator[](int i);
	T const & operator[](int i)const;
	operator T*();
	operator T const *()const;
};
#endif//__ARRAY_N_H__
