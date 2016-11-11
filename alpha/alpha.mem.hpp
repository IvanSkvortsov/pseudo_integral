#ifndef __ALPHA_MEM_HPP__
#define __ALPHA_MEM_HPP__
#include"alpha.mem.h"

template<typename T, typename U> alpha_mem<T,U>::alpha_mem(): memory_map(), memorystream(), alpha_pow<T,U>(){}
template<typename T, typename U> alpha_mem<T,U>::alpha_mem( alpha_mem<T,U> const & v ) : memory_map(), memorystream( v ), alpha_pow<T,U>( v ){}

template<typename T, typename U> alpha_mem<T,U> & alpha_mem<T,U>::operator=( alpha_mem<T,U> const & v )
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	alpha_mem<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;

}

template<typename T, typename U> void alpha_mem<T,U>::memory_create( const char * file, int __flags, mode_type __mode )
{
	size_type __size = this->alpha_pow<T,U>::comp_size( this->get_lmax(), this->get_alpsize(), this->get_A_size(), this->get_B_size() );
	int _st = this->memory_map::create( file, __size, __flags, __mode );
	if( _st )
	{
		this->error("memory_create", "memory mappig failed");
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		std::cerr << "comp.size : " << __size << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	size_type _w_size = this->alpha_pow<T,U>::write_mxpow( ms, this->get_lmax(), this->get_alpsize(), this->get_A_size(), this->get_B_size() );
#ifdef  __ALPHA_MEM_DEBUG
	assert( _w_size == __size );
#endif
}

template<typename T, typename U> void alpha_mem<T,U>::memory_open( const char * file, int __flags, mode_type __mode )
{
	int _st = this->memory_map::open( file, __flags, __mode );
	if( _st )
	{
		this->error("memory_open", "memory mappig failed");
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	size_type _r_size = this->alpha_pow<T,U>::read_mxpow( ms );
#ifdef  __ALPHA_MEM_DEBUG
	assert( _r_size == this->alpha_pow<T,U>::comp_size( this->get_lmax(), this->get_alpsize(), this->get_A_size(), this->get_B_size() ) );
#endif
}

#define __ALPHA_MEM_SPEC( typeU )\
template class alpha_mem<float, typeU>;\
template class alpha_mem<double, typeU>;\
template class alpha_mem<long double, typeU>;

#endif//__ALPHA_MEM_HPP__
