#ifndef __ARRAY_N_HPP__
#define __ARRAY_N_HPP__
#include"array.n.h"

template<typename T, int _size> array_n<T,_size>::array_n(){}
template<typename T, int _size> array_n<T,_size>::array_n( array_n<T, _size> const & v)
{
	for(int i = 0; i < _size; ++i)
		this->_data[i] = v[i];
}
template<typename T, int _size> array_n<T,_size>::array_n(T const * v)
{
	for(int i = 0; i < this->size(); ++i)
		this->_data[i] = v[i];
}
template<typename T, int _size> array_n<T,_size>::~array_n(){}
template<typename T, int _size> array_n<T,_size> & array_n<T,_size>::operator=(array_n<T,_size> const & v)
{
	if( this == &v )
		return *this;
	for(int i = 0; i < this->size(); ++i)
		this->_data[i] = v[i];
	return *this;
}
template<typename T, int _size>
array_n<T,_size> & array_n<T,_size>::operator=(T const * v)
{
	if( this == (void *)v )
		return *this;
	for(int i = 0; i < this->size(); ++i)
		this->_data[i] = v[i];
	return *this;
}
template<typename T, int _size> T * array_n<T,_size>::data(){return this->_data;}
template<typename T, int _size> T const * array_n<T,_size>::data()const{return this->_data;}
template<typename T, int _size> const int array_n<T,_size>::size()const{return _size;}
template<typename T, int _size> T & array_n<T,_size>::operator[](int i){return this->_data[i];}
template<typename T, int _size> T const & array_n<T,_size>::operator[](int i)const{return this->_data[i];}
template<typename T, int _size> array_n<T,_size>::operator T*(){return this->_data;}
template<typename T, int _size> array_n<T,_size>::operator T const *()const{return this->_data;}

#define __ARRAY_N_SPEC( type )\
template struct array_n<type, 0>;\
template struct array_n<type, 1>;\
template struct array_n<type, 2>;\
template struct array_n<type, 3>;\
template struct array_n<type, 4>;\
template struct array_n<type, 5>;

#endif//__ARRAY_N_HPP__
