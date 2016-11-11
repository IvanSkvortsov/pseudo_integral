#ifndef __ARRAY_SCP_HPP__
#define __ARRAY_SCP_HPP__
#include"array.scp.h"
#include<cstdlib>// exit

#ifdef  __ERROR_MSG_ARRAY_SCP
#include<iostream>
#endif

template<typename T> array_scp<T>::array_scp(): _data(0), _size(0), _capacity(0){}
template<typename T> array_scp<T>::array_scp(array_scp<T> const & v): _data(new T[v._size]), _size(v._size), _capacity(_size)
{
	for(int i = 0; i < this->_size; ++i)
		this->_data[i] = v[i];
}
template<typename T> array_scp<T>::array_scp(int const & __size): _data(new T[__size]), _size(__size), _capacity(__size){}
template<typename T> array_scp<T>::array_scp(T const * __data, int const & __size): _data(new T[__size]), _size(__size), _capacity(__size)
{
	for(int i = 0; i < this->_size; ++i)
		this->_data[i] = __data[i];
}
template<typename T> array_scp<T>::~array_scp()
{
	if( this->_data )
		delete [] this->_data;
	this->_size = 0;
	this->_capacity = 0;
	this->_data = 0;
}
template<typename T> array_scp<T> & array_scp<T>::operator=(array_scp<T> const & v)
{
	if( this == &v ) return *this;
	this->resize( v.size() );
	for(int i = 0; i < this->_size; ++i)
		this->_data[i] = v[i];
	return *this;
}
template<typename T> T * array_scp<T>::data(){return this->_data;}
template<typename T> T const * array_scp<T>::data()const{return this->_data;}
template<typename T> T & array_scp<T>::operator[](int i){return this->_data[i];}
template<typename T> T const & array_scp<T>::operator[](int i)const{return this->_data[i];}
template<typename T> int const & array_scp<T>::size()const{return this->_size;}
template<typename T> int const & array_scp<T>::capacity()const{return this->_capacity;}
template<typename T> void array_scp<T>::append(T const & v)
{
	if( this->_capacity > this->_size )
	{
		this->_data[this->_size] = v;
		++(this->_size);
	}
	else if( this->_capacity == this->_size )
	{
		array_scp<T> tmp( *this );
		this->resize( this->_size + 1 );
		for(int i = 0; i < tmp.size(); ++i)
			this->_data[i] = tmp[i];
		this->_data[this->_size-1] = v;
	}
	else// this->_capacity < this->_size
	{
#ifdef  __ERROR_MSG_ARRAY_SCP
		std::cerr << "Error: array_scp<T>::append(T const & v)" << std::endl;
		std::cerr << "this->_size     : " << this->_size << std::endl;
		std::cerr << "this->_capacity : " << this->_capacity << std::endl;
#endif
		exit(1);
	}
}
template<typename T> void array_scp<T>::resize(int __size)
{
	if( __size < 0 )
	{
#ifdef  __ERROR_MSG_ARRAY_SCP
		std::cerr << "Error: array_scp<T>::resize(int __size)" << std::endl;
		std::cerr << "__size : " << __size << std::endl;
#endif
		exit(1);
	}
	if( this->_capacity < __size )
	{
		if( this->_data ) delete [] this->_data;
		this->_size = __size;
		if( this->_capacity == 0 )
			this->_capacity = __size;
		else
			while( this->_capacity < __size ) this->_capacity *= 2;
		if( this->_capacity ) this->_data = new T[this->_capacity];
		else
		{
#ifdef  __ERROR_MSG_ARRAY_SCP
			std::cerr << "Error: array_scp<T>::resize(int __size)" << std::endl;
			std::cerr << "this->_capacity : " << this->_capacity << std::endl;
#endif
			exit(1);
		}
	}
	else this->_size = __size;
}
template<typename T> void array_scp<T>::reserve(int __capacity)
{
	if( __capacity < 0 )
	{
#ifdef  __ERROR_MSG_ARRAY_SCP
		std::cerr << "Error: array_scp<T>::reserve(int __capacity)" << std::endl;
		std::cerr << "__capacity : " << __capacity << std::endl;
#endif
		exit(1);
	}
	if( this->_capacity < __capacity )
	{
		if( this->_data ) delete [] this->_data;
		if( this->_capacity == 0 )
			this->_capacity = __capacity;
		else
			while( this->_capacity < __capacity ) this->_capacity *= 2;
		if( this->_capacity ) this->_data = new T[this->_capacity];
	}
}

#endif//__ARRAY_SCP_HPP__
