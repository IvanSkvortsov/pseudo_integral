#ifndef __PSEUDO_INTEGRAL_MEM_HPP__
#define __PSEUDO_INTEGRAL_MEM_HPP__
#include"pseudo.integral.mem.h"
#include<cstdlib>// exit
#include<iostream>// cerr, endl

template<typename T, typename U> pseudo_integral_mem<T,U>::pseudo_integral_mem(): memory_map(), memorystream(), pseudo_integral<T,U>(){}
template<typename T, typename U> pseudo_integral_mem<T,U>::pseudo_integral_mem( pseudo_integral_mem<T,U> const & v):
	memory_map(), memorystream( v ), pseudo_integral<T,U>( v ){}
template<typename T, typename U> pseudo_integral_mem<T,U> & pseudo_integral_mem<T,U>::operator=( pseudo_integral_mem<T,U> const & v)
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	this->memorystream::operator=( v );
	this->pseudo_integral<T,U>::operator=( v );
	return *this;
}
template<typename T, typename U> void pseudo_integral_mem<T,U>::error( const char * _method, const char * _message )const
{
	std::cerr << "Error: [" << this << "] pseudo_integral_mem<T,U>::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl;
}

template<typename T, typename U> void pseudo_integral_mem<T,U>::memory_create( const char * file, int __flags, mode_type __mode )
{
	size_type __size = this->pseudo_integral<T,U>::comp_size();
	__pseudo_assert__( __size >= 0 );
	int _st = this->memory_map::create( file, __size, __flags, __mode );
	if( _st )
	{
		this->error("memory_create", "memory mapping failed" );
		std::cerr << "file '" << file << "'" << std::endl;
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		std::cerr << "comp_size : " << __size << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	this->pseudo_integral<T,U>::write_map( ms );
}
template<typename T, typename U> void pseudo_integral_mem<T,U>::memory_open( const char * file, int __flags, mode_type __mode )
{
	int _st = this->memory_map::open( file, __flags, __mode );
	if( _st )
	{
		this->error("memory_create", "memory mapping failed" );
		std::cerr << "file '" << file << "'" << std::endl;
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		std::cerr << "comp_size : " << this->comp_size() << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	this->pseudo_integral<T,U>::read_map( ms );
}

#define __PSEUDO_INTEGRAL_MEM_SPEC( typeU )\
template class pseudo_integral_mem<float, typeU>;\
template class pseudo_integral_mem<double, typeU>;\
template class pseudo_integral_mem<long double, typeU>;

#endif//__PSEUDO_INTEGRAL_MEM_HPP__
