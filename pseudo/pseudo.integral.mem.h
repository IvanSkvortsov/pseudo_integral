#ifndef __PSEUDO_INTEGRAL_MEM_H__
#define __PSEUDO_INTEGRAL_MEM_H__
#include"memory.map.h"
#include"memorystream.h"
#include"pseudo.integral.h"

template<typename T, typename U>
class pseudo_integral_mem: public memory_map, public memorystream, public pseudo_integral<T,U>
{
public:
	typedef typename memory_map::mode_type mode_type;
	typedef typename size_struct<1>::size_type size_type;
protected:
	void error( char const * _method, const char _message[] = "nothing to do here" )const;
public:
	pseudo_integral_mem();
	pseudo_integral_mem( pseudo_integral_mem<T,U> const & );
	pseudo_integral_mem<T,U> & operator=( pseudo_integral_mem<T,U> const & );

	void memory_create( const char * file, int __flags = O_RDWR| O_TRUNC| O_CREAT, mode_type __mode = memory_map::MODE_644 );
	void memory_open(   const char * file, int __flags = O_RDONLY, mode_type __mode = memory_map::MODE_444 );
	inline void sync_stream(){ this->memorystream::setbuf( this->memory_map::data(), this->memory_map::size() ); }
};

#endif//__PSEUDO_INTEGRAL_MEM_H__
