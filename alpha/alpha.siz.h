#ifndef __ALPHA_SIZ_H__
#define __ALPHA_SIZ_H__
#include<cstring>// memcpy

#define __ALPHA_SIZ_INT_INTERFACE( methodname, fieldname )\
inline void put_##methodname(int value){ this->fieldname._##methodname = value; }\
inline int const & get_##methodname()const{ return this->fieldname._##methodname; }

#define __ALPHA_SIZ_SAFE

#ifdef  __ALPHA_SIZ_SAFE
  #include<cassert>
  #define __ALPHA_SIZ_PTR_INTERFACE( method )\
  inline void set_##method(int const * __X_size ){ this->_##method = __X_size; }\
  inline int const * get_##method()const\
  {\
	  assert( this->_##method != 0 );\
	  return this->_##method;\
  }\
  inline int const & get_##method(int i)const\
  {\
	  assert( this->_##method != 0 );\
	  return this->_##method[i];\
  }
#else
  #define __ALPHA_SIZ_PTR_INTERFACE( method )\
  inline void set_##method(int const * __X_size ){ return this->_##method = __X_size; }\
  inline int const * get_##method()const{ return this->_##method; }\
  inline int const & get_##method(int i)const{ return this->_##method[i]; }
#endif

struct alpha_siz
{
public:
#pragma pack( push, 4 )
	typedef struct
	{
		int _l_max, _lso_max, _la_max, _lb_max;
	} _lmax_struct;
	typedef struct
	{
		int _a_size, _b_size, _c_size;
	} _alpsize_struct;
#pragma pack( pop )
protected:
	_lmax_struct _lmax;
	_alpsize_struct _alpsize;
	int const * _A_size, * _B_size, * _C_size;
	int const * _A_dsize, * _B_dsize, * _C_dsize;
public:
	inline alpha_siz(): _lmax(), _alpsize(), _A_size(), _B_size(), _C_size(), _A_dsize(), _B_dsize(), _C_dsize(){}
	inline alpha_siz(alpha_siz const & v): _lmax(v._lmax), _alpsize(v._alpsize), _A_size(v._A_size), _B_size(v._B_size), _C_size(v._C_size),
		_A_dsize(v._A_dsize), _B_dsize(v._B_dsize), _C_dsize(v._C_dsize){}
	inline ~alpha_siz()
	{
		_A_size = 0; _B_size = 0; _C_size = 0;
		_A_dsize = 0; _B_dsize = 0; _C_dsize = 0;
	}
	inline alpha_siz & operator=( alpha_siz const & v )
	{
		if( this == &v )
			return *this;
		alpha_siz tmp( v );
		memcpy( this, &tmp, sizeof(alpha_siz) );
		return *this;
	}
	inline void set_lmax( _lmax_struct const & __lmax ){ this->_lmax = __lmax; }
	inline void set_alpsize( _alpsize_struct const & __alpsize ){ this->_alpsize = __alpsize; }

	inline _lmax_struct const & get_lmax()const{ return this->_lmax; }
	inline _alpsize_struct const & get_alpsize()const{ return this->_alpsize; }

	__ALPHA_SIZ_INT_INTERFACE( l_max, _lmax );
	__ALPHA_SIZ_INT_INTERFACE( lso_max, _lmax );
	__ALPHA_SIZ_INT_INTERFACE( la_max, _lmax );
	__ALPHA_SIZ_INT_INTERFACE( lb_max, _lmax );
	// e.i. l_max:
	// %put_l_max( int __l_max );
	// %get_l_max()

	__ALPHA_SIZ_INT_INTERFACE( a_size, _alpsize );
	__ALPHA_SIZ_INT_INTERFACE( b_size, _alpsize );
	__ALPHA_SIZ_INT_INTERFACE( c_size, _alpsize );
	// %put_a_size( int __a_size );
	// %get_a_size()

	__ALPHA_SIZ_PTR_INTERFACE( A_size );
	__ALPHA_SIZ_PTR_INTERFACE( B_size );
	__ALPHA_SIZ_PTR_INTERFACE( C_size );
	__ALPHA_SIZ_PTR_INTERFACE( A_dsize );
	__ALPHA_SIZ_PTR_INTERFACE( B_dsize );
	__ALPHA_SIZ_PTR_INTERFACE( C_dsize );
	// e.i. A_size:
	// %set_A_size( int const * );
	// %get_A_size(); - return value - pointer to const int
	// %get_A_size(int ); - return value - reference to const int

};

#endif//__ALPHA_SIZ_H__
