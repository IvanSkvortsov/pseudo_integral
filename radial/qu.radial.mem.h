#ifndef __QU_RADIAL_MEM_H__
#define __QU_RADIAL_MEM_H__
#include"memory.map.h"
#include"qu.radial.map.h"
#include<cstring>// memcpy
#include<cstdlib>// exit
#include<iostream>// cerr, endl

#define __QU_RADIAL_MEM_H__LMAX_STRUCT_INTERFACE( method, source )\
inline int const & get_##method()const{ return (source)._##method; }\
inline int & get_##method(){ return (source)._##method; }\
inline void set_##method( const int value){ (source)._##method = value; }

struct qu_radial_mem: public memory_map, public memorystream, public qu_radial_map
{
	typedef typename memory_map::mode_type mode_type;
	typedef typename memorystream::seek_dir seek_dir;
	typedef typename alpha_siz::_lmax_struct _lmax_struct;
	typedef typename qu_radial_map::size_type size_type;
protected:
	inline void error( const char * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] qu_radial_mem::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl; }
	_lmax_struct _lmax;
public:
	qu_radial_mem();
	qu_radial_mem( qu_radial_mem const & );
	qu_radial_mem & operator=( qu_radial_mem const & );

	void memory_create( const char * file, int __flags = O_RDWR| O_TRUNC| O_CREAT, mode_type __mode = memory_map::MODE_644 );
	void memory_open( const char * file, int __flags = O_RDONLY, mode_type __mode = memory_map::MODE_444 );

	inline void sync_stream(){ this->memorystream::setbuf( this->memory_map::data(), this->memory_map::size() ); }

	inline void set_lmax( _lmax_struct const & __lmax ){ this->_lmax = __lmax; }
	inline _lmax_struct const & get_lmax()const{ return this->_lmax; }
	__QU_RADIAL_MEM_H__LMAX_STRUCT_INTERFACE( la_max, this->_lmax );
	__QU_RADIAL_MEM_H__LMAX_STRUCT_INTERFACE( lb_max, this->_lmax );
	__QU_RADIAL_MEM_H__LMAX_STRUCT_INTERFACE( l_max, this->_lmax );
	__QU_RADIAL_MEM_H__LMAX_STRUCT_INTERFACE( lso_max, this->_lmax );
};

inline qu_radial_mem::qu_radial_mem(): memory_map(), memorystream(), qu_radial_map(), _lmax(){}
inline qu_radial_mem::qu_radial_mem( qu_radial_mem const & v ): memory_map(), memorystream( v ), qu_radial_map( v ), _lmax( v._lmax ){}
inline qu_radial_mem & qu_radial_mem::operator=( qu_radial_mem const & v )
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	qu_radial_mem tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;
}

inline void qu_radial_mem::memory_create( const char * file, int __flags, mode_type __mode )
{
	size_type __size = this->qu_radial_map::comp_size( this->get_lmax() );
	if( __size == 0 && this->is_mapping_min() )
		return;
	int _st = this->create( file, __size, __flags, __mode );
	if( _st )
	{
		this->error("memory_create", "memory mapping error");
		std::cerr << "file '" << file << "'" << std::endl;
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		std::cerr << "comp_size : " << __size << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	this->qu_radial_map::write_map( ms, this->get_lmax() );
}
inline void qu_radial_mem::memory_open( const char * file, int __flags, mode_type __mode )
{
	int _st = this->open( file, __flags, __mode );
	if( _st )
	{
		this->error("memory_open", "memory mapping error");
		std::cerr << "file '" << file << "'" << std::endl;
		std::cerr << " map.data : [" << this->data() << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	this->qu_radial_map::read_map( ms );
	this->set_lmax( this->qu_radial_map::M_get_lmax() );
}

#endif//__QU_RADIAL_MEM_H__
