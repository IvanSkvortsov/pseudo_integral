#ifndef __MATRIX_VECTOR_H__
#define __MATRIX_VECTOR_H__
#include"size.struct.h"
#include<vector>
#include<iostream>

template<typename T>
struct matrix_vector : public size_struct<2>, public std::vector<T>
{
public:
	typedef typename std::vector<T>::value_type value_type;
	typedef typename std::vector<T>::pointer pointer;
	typedef typename std::vector<T>::const_pointer const_pointer;
	typedef typename std::vector<T>::reference reference;
	typedef typename std::vector<T>::const_reference const_reference;
	//typedef typename std::vector<T>::size_type size_type;
	typedef typename size_struct<2>::size_type size_type;

	inline matrix_vector(): size_struct<2>(), std::vector<T>(){}
	inline matrix_vector(matrix_vector<T> const & v): size_struct<2>( v ), std::vector<T>( v ){}
	inline matrix_vector<T> & operator=( matrix_vector<T> const & v)
	{
		if( this == &v ) return *this;
		this->size_struct<2>::operator=( v );
		this->std::vector<T>::operator=( v );
		return *this;
	}

	inline const size_type & size()const{ return this->size_struct<2>::size();}
	inline size_type & size(){ return this->size_struct<2>::size();}
	//const size_type & size()const{ return this->std::vector<T>::size();}
	//size_type & size(){ return this->size_struct<2>::size();}
	inline const_reference operator()(int __n, int __m)const{ return this->operator[]( this->size_struct<2>::operator()(__n, __m) );}
	inline reference operator()(int __n, int __m){ return this->operator[]( this->size_struct<2>::operator()(__n, __m) );}
	inline void init_size()
	{
		this->size_struct<2>::init_size();
		this->std::vector<T>::resize( this->size_struct<2>::size() );
	}
	inline void resize( const size_type __n, const size_type __m )
	{
		this->size_struct<2>::n() = __n;
		this->size_struct<2>::m() = __m;
		this->size_struct<2>::size() = __n * __m;
		this->std::vector<T>::resize( this->size_struct<2>::size() );
	}
	inline void resize( const size_type __size )
	{
		this->size_struct<2>::size() = __size;
		this->std::vector<T>::resize( __size );
	}
};
#endif//__MATRIX_VECTOR_H__
