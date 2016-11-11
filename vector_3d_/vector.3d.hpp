#ifndef __VECTOR_3D_HPP__
#define __VECTOR_3D_HPP__
#include"vector.3d.h"
#include<cmath>// sqrt

#ifdef  __VECTOR_3D_LOG
  #include<iostream>// clog, endl
  #include"type.name.str.h"
#endif

template<typename T>
__VECTOR_3D__INLINE void vector_3d<T>::log(const char * _method)const
{
#ifdef  __VECTOR_3D_LOG
	std::clog << "LOG : [" << this << "] vector_3d<T>::" << _method <<
  #ifdef  __TYPE_NAME_STR_H__
		" [with " <<
		__STRINGIFY(T) << " = " << type_name_str<T>() << "]" <<
  #endif
		std::endl;
#endif
}

template<typename T>
template<typename U> __VECTOR_3D__INLINE void vector_3d<T>::log(const char * _method)const
{
#ifdef  __VECTOR_3D_LOG
	std::clog << "LOG : [" << this << "] template<typename U> vector_3d<T>::" << _method <<
  #ifdef  __TYPE_NAME_STR_H__
		" [with " <<
		__STRINGIFY(T) << " = " << type_name_str<T>() << "; " <<
		__STRINGIFY(U) << " = " << type_name_str<U>() << "]" <<
  #endif
		std::endl;
#endif
}

// vector_3d()
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::vector_3d(): _data{0, 0, 0}
{
	this->log("vector_3d()");
}
// ~vector_3d()
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::~vector_3d()
{
	this->log("~vector_3d()");
}
// vector_3d( Type const & , Type const & , Type const & )
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( U const & _x, U const & _y, U const & _z ): _data{ T(_x), T(_y), T(_z) }
{
	this->log<U>("vector_3d(U const & , U const & , U const & )");
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( T const & _x, T const & _y, T const & _z ): _data{ _x, _y, _z }
{
	this->log("vector_3d(T const & , T const & , T const & )");
}
// vector_3d( Type const * )
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( U const * v ) : _data{ T(v[0]), T(v[1]), T(v[2]) }
{
	this->log<U>("vector_3d(U const * )");
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( T const * v ) : _data{ v[0], v[1], v[2] }
{
	this->log("vector_3d(T const * )");
}
// vector_3d( vector_3d<Type> const & )
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( vector_3d<U> const & v ) : _data{ T(v[0]), T(v[1]), T(v[2]) }
{
	this->log<U>("vector_3d( vector_3d<U> const & )");
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::vector_3d( vector_3d<T> const & v ) : _data{ v[0], v[1], v[2] }
{
	this->log("vector_3d( vector_3d<T> const & )");
}

// operator=( vector_3d<Type> const & )
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator=( vector_3d<U> const & v )
{
	this->log<U>("operator=( vector_3d<U> const & )");
	return this->operator=( v.data() );
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator=( vector_3d<T> const & v )
{
	this->log("operator=( vector_3d<T> const & )");
	if( this == &v )
		return *this;
	return this->operator=( v.data() );
}
// operator=( Type const * )
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator=( U const * v )
{
	for(int i = 0; i < 3; ++i) this->_data[i] = v[i];
	return *this;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator=( T const * v )
{
	for(int i = 0; i < 3; ++i) this->_data[i] = v[i];
	return *this;
}

template<typename T> __VECTOR_3D__INLINE vector_3d<T>::operator T*(){ return this->_data;}
template<typename T> __VECTOR_3D__INLINE vector_3d<T>::operator T const *()const{ return this->_data;}

template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::const_pointer vector_3d<T>::data()const{ return this->_data;}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::pointer vector_3d<T>::data(){ return this->_data;}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::const_reference vector_3d<T>::operator[](int i)const{ return this->_data[i];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::reference vector_3d<T>::operator[](int i){ return this->_data[i];}
// x, y, z
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::const_reference vector_3d<T>::x()const{ return this->_data[0];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::const_reference vector_3d<T>::y()const{ return this->_data[1];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::const_reference vector_3d<T>::z()const{ return this->_data[2];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::reference vector_3d<T>::x(){ return this->_data[0];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::reference vector_3d<T>::y(){ return this->_data[1];}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::reference vector_3d<T>::z(){ return this->_data[2];}
// sqr
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::value_type vector_3d<T>::sqr()const
{
	value_type __sqr;
	this->sqr( __sqr );
	return __sqr;
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::sqr(reference __sqr)const
{
	__sqr = 0;
	const_pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		__sqr += (*ptr * *ptr);
}
// len
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::value_type vector_3d<T>::len()const
{
	value_type __len;
	this->len( __len );
	return __len;
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::len(reference __len)const
{
	this->sqr( __len );
	__len = sqrt( __len );
}
// neg
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::neg()
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr = -*ptr;
	return *this;
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::neg( vector_3d<T> & v)const
{
	v = *this;
	v.neg();
}
// norm
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::norm()
{
	value_type __len;
	this->len( __len );
	for(int i = 0; i < 3; ++i)
		this->_data[i] /= __len;
	return *this;
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::norm( vector_3d<T> & v )const
{
	v = *this;
	v.norm();
}
// operator+=
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator+=( vector_3d<U> const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr += v[i];
	return *this;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator+=( vector_3d<T> const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr += v[i];
	return *this;
}
// operator-=
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator-=( vector_3d<U> const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr -= v[i];
	return *this;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator-=( vector_3d<T> const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr -= v[i];
	return *this;
}
// operator-()
template<typename T> __VECTOR_3D__INLINE vector_3d<T> vector_3d<T>::operator-()const
{
	vector_3d<T> tmp( *this );
	tmp.neg();
	return tmp;
}
// operator*=
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator*=( U const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr *= v;
	return *this;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator*=( T const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr *= v;
	return *this;
}
// operator/=
template<typename T>
template<typename U> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator/=( U const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr /= v;
	return *this;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> & vector_3d<T>::operator/=( T const & v )
{
	pointer ptr = this->data();
	for(int i = 0; i < 3; ++i, ++ptr)
		*ptr /= v;
	return *this;
}
// scalar
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::value_type vector_3d<T>::scalar( vector_3d<T> const & v)const
{
	value_type __result;
	this->scalar( __result, v );
	return __result;
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::scalar( T & __result, vector_3d<T> const & v)const
{
	if( this == &v )
	{
		this->sqr( __result );
		return;
	}
	const_pointer ptr = this->data();
	__result = 0;
	for(int i = 0; i < 3; ++i, ++ptr)
		__result += *ptr * v[i];
}
template<typename T> __VECTOR_3D__INLINE void vector_3d<T>::scalar(T & __result, vector_3d<T> const & lhs, vector_3d<T> const & rhs)
{
	lhs.scalar( __result, rhs );
}
template<typename T> __VECTOR_3D__INLINE typename vector_3d<T>::value_type vector_3d<T>::scalar(vector_3d<T> const & lhs, vector_3d<T> const & rhs)
{
	return lhs.scalar( rhs );
}

// operator+
template<typename T, typename U> __VECTOR_3D__INLINE vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<U> const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp += rhs;
	return tmp;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<T> const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp += rhs;
	return tmp;
}
// operator-
template<typename T, typename U> __VECTOR_3D__INLINE vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<U> const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp -= rhs;
	return tmp;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<T> const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp -= rhs;
	return tmp;
}
// operator*
template<typename T, typename U> __VECTOR_3D__INLINE vector_3d<T> operator*( vector_3d<T> const & lhs, U const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp *= rhs;
	return tmp;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> operator*( vector_3d<T> const & lhs, T const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp *= rhs;
	return tmp;
}
// operator/
template<typename T, typename U> __VECTOR_3D__INLINE vector_3d<T> operator/( vector_3d<T> const & lhs, U const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp /= rhs;
	return tmp;
}
template<typename T> __VECTOR_3D__INLINE vector_3d<T> operator/( vector_3d<T> const & lhs, T const & rhs)
{
	vector_3d<T> tmp( lhs );
	tmp /= rhs;
	return tmp;
}

#define VECTOR_3D_SPEC2( T, U )\
template vector_3d<T>::vector_3d( U const * );\
template vector_3d<T>::vector_3d( U const & , U const & , U const & );\
template vector_3d<T>::vector_3d( vector_3d<U> const & );\
template vector_3d<T> & vector_3d<T>::operator=( vector_3d<U> const & );\
template vector_3d<T> & vector_3d<T>::operator=( U const * );\
template vector_3d<T> & vector_3d<T>::operator+=( vector_3d<U> const & );\
template vector_3d<T> & vector_3d<T>::operator-=( vector_3d<U> const & );\
template vector_3d<T> & vector_3d<T>::operator*=( U const & );\
template vector_3d<T> & vector_3d<T>::operator/=( U const & );\
template vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<U> const & rhs);\
template vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<U> const & rhs);\
template vector_3d<T> operator*( vector_3d<T> const & lhs, U const & rhs);\
template vector_3d<T> operator/( vector_3d<T> const & lhs, U const & rhs);

#define VECTOR_3D_SPEC_REAL_INT( float_type )\
VECTOR_3D_SPEC2( float_type, int );\
VECTOR_3D_SPEC2( float_type, long int );\
VECTOR_3D_SPEC2( float_type, long long int );\
VECTOR_3D_SPEC2( float_type, unsigned int );\
VECTOR_3D_SPEC2( float_type, unsigned long int );\
VECTOR_3D_SPEC2( float_type, unsigned long long int );

#define VECTOR_3D_SPEC_REAL( T )\
template class vector_3d<T>;\
VECTOR_3D_SPEC_REAL_INT( T )\
template vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<T> const & rhs);\
template vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<T> const & rhs);\
template vector_3d<T> operator*( vector_3d<T> const & lhs, T const & rhs);\
template vector_3d<T> operator/( vector_3d<T> const & lhs, T const & rhs);


#define VECTOR_3D_SPEC_INT_REAL( int_type )\
VECTOR_3D_SPEC2( int_type, float );\
VECTOR_3D_SPEC2( int_type, double );\
VECTOR_3D_SPEC2( int_type, long double );

#define VECTOR_3D_SPEC_INT( T )\
template class vector_3d<T>;\
VECTOR_3D_SPEC_INT_REAL( T )\
template vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<T> const & rhs);\
template vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<T> const & rhs);\
template vector_3d<T> operator*( vector_3d<T> const & lhs, T const & rhs);\
template vector_3d<T> operator/( vector_3d<T> const & lhs, T const & rhs);

#endif//__VECTOR_3D_HPP__
