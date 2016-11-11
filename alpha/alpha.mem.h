#ifndef __ALPHA_MEM_H__
#define __ALPHA_MEM_H__
#include"memory.map.h"
#include"alpha.siz.h"
#include"alpha.pow.h"

template<typename T, typename U>
struct alpha_mem : public memory_map, public memorystream, public alpha_siz, public alpha_pow<T,U>
{
public:
	typedef typename memory_map::mode_type mode_type;
	typedef typename memorystream::seek_dir seek_dir;

	typedef typename alpha_siz::_lmax_struct _lmax_struct;
	typedef typename alpha_siz::_alpsize_struct _alpsize_struct;

	typedef typename alpha_pow<T,U>::size_type size_type;
	typedef typename alpha_pow<T,U>::_pos1_struct _pos1_struct;
	typedef typename alpha_pow<T,U>::_pos2_struct _pos2_struct;
	typedef typename alpha_pow<T,U>::_pos3_struct _pos3_struct;

	__DATA_TYPEDEF( T );
	typedef T float_type;
protected:
	inline void error(const char * _method, const char _message[] = "nothing to do here" )const
	{
		std::cerr << "Error: [" << this << "] alpha_mem<T,U>::" << _method << ", " << _message << std::endl;
	}
public:
	alpha_mem();
	alpha_mem( alpha_mem<T,U> const & v );
	alpha_mem<T,U> & operator=( alpha_mem<T,U> const & v );

	inline void sync_stream(){ this->memorystream::setbuf( this->memory_map::data(), this->memory_map::size() ); }

	void memory_create( const char * file, int __flags = O_RDWR| O_TRUNC| O_CREAT, mode_type __mode = memory_map::MODE_644 );
	void memory_open( const char * file, int __flags = O_RDONLY, mode_type __mode = memory_map::MODE_444 );

	inline void init_map()
	{
		this->alpha_map::init_map( this->get_A_size(), this->get_B_size(), this->get_C_size() );
		this->alpha_map::init_map_norm();
		this->alpha_map::init_mapd( this->get_A_dsize(), this->get_B_dsize(), this->get_C_dsize() );
	}
};

#endif//__ALPHA_MEM_H__
