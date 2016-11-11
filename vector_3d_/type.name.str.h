#ifndef __TYPE_NAME_STR_H__
#define __TYPE_NAME_STR_H__
template<typename T> inline std::string const & type_name_str()
{
	static const std::string str("unknown");
	return str;
}

#define __STRINGIFY( _name ) #_name

#define __TYPE_NAME_STR( _type_ ) \
template<> inline std::string const & type_name_str<_type_>()\
{\
	static const std::string str(#_type_);\
	return str;\
}

__TYPE_NAME_STR( int );
__TYPE_NAME_STR( long int );
__TYPE_NAME_STR( long long int );
__TYPE_NAME_STR( unsigned int );
__TYPE_NAME_STR( unsigned long int );
__TYPE_NAME_STR( unsigned long long int );
__TYPE_NAME_STR( float );
__TYPE_NAME_STR( double );
__TYPE_NAME_STR( long double );
#include"mpreal.use.h"
#ifdef __MPREAL_USE
#include"mpreal.h"
__TYPE_NAME_STR( mpfr::mpreal );
#endif

#endif//__TYPE_NAME_STR_H__

