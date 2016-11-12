#ifndef __BASIS_STRUCT_H__
#define __BASIS_STRUCT_H__

template<typename T>
struct basis_struct
{
	typedef struct
	{
		int _size;
		T * _data;
	} _vec_struct;
	typedef struct
	{
		int _pos;
		T * _data;
	} _pos_struct;
	typedef struct
	{
		_vec_struct _alp;
		_vec_struct _fun;
	} _bas_struct;
	typedef struct
	{
		_vec_struct _fun;
		_pos_struct _alp;
	} _fun_struct;
};
#define __BASIS_STRUCT_INT_INTERFACE( method, variable, source ) \
inline const int  & get_##method##_##variable()const{ return (source)._##variable;}\
inline void set_##method##_##variable(int __value){ (source)._##variable = __value;}

#define __BASIS_STRUCT_DATA_INTERFACE( method, source, float_type ) \
inline float_type & get_##method(int i){ return (source)._data[i];}\
inline float_type * get_##method##_data(){ return (source)._data;}\
inline float_type * get_##method##_data(int i){ return (source)._data + i;}\
inline const float_type & get_##method(int i)const{ return (source)._data[i];}\
inline const float_type * get_##method##_data(int i)const{ return (source)._data + i;}\
inline const float_type * get_##method##_data()const{ return (source)._data;}\
inline void set_##method##_data( float_type * __data ){ (source)._data = __data; }

#define __BASIS_STRUCT_POS_INTERFACE( method, source, float_type ) \
__BASIS_STRUCT_INT_INTERFACE(  method, pos, source ) \
__BASIS_STRUCT_DATA_INTERFACE( method, source, float_type )

#define __BASIS_STRUCT_VEC_INTERFACE( method, source, float_type ) \
__BASIS_STRUCT_INT_INTERFACE(  method, size, source ) \
__BASIS_STRUCT_DATA_INTERFACE( method, source, float_type )

template<typename T>
struct vec_struct : protected basis_struct<T>::_vec_struct
{
public:
	typedef typename basis_struct<T>::_vec_struct base_class;
	typedef T float_type;

	inline vec_struct(): base_class(){}
	inline vec_struct( base_class const & v ): base_class( v ){}
	inline vec_struct( vec_struct<T> const & v ): base_class( v ){}
	inline ~vec_struct(){}
	inline vec_struct<T> & operator=( base_class const & v)
	{
		this->base_class::operator=( v );
		return *this;
	}
	inline vec_struct<T> & operator=( vec_struct<T> const & v )
	{
		if( this == &v )
			return *this;
		this->base_class::operator=( v );
		return *this;
	}
	__BASIS_STRUCT_VEC_INTERFACE( vec, *this, float_type );
};
template<typename T>
struct bas_struct : protected basis_struct<T>::_bas_struct
{
private:
	inline void log(const char * _method, const int __n = 0)const
	{
#ifdef  __BAS_STRUCT_LOG
		static int _count = 0;
		if( __n > 0 ) _count += __n;
		std::clog << "Log: [" << this << "] bas_struct<T>::" << _method << "\t" << _count << std::endl;
		if( __n < 0 ) _count += __n;
#endif
	}
public:
	typedef typename basis_struct<T>::_bas_struct base_class;
	typedef T float_type;

	inline bas_struct(): base_class(){this->log("bas_struct()", 1);}
	inline bas_struct( bas_struct<T> const & v ): base_class( v ){this->log("bas_struct( bas_struct<T> const & )", 1);}
	inline bas_struct( base_class const & v ): base_class( v ){this->log("bas_struct( basis_struct<T>::_bas_struct const & )", 1);}
	inline ~bas_struct(){this->log("~bas_struct()", -1);}
	inline bas_struct<T> & operator=( base_class const & v )
	{
		this->log("operator=( basis_struct<T>::_bas_struct const & )");
		this->base_class::operator=( v );
		return *this;
	}
	inline bas_struct<T> & operator=( bas_struct<T> const & v )
	{
		this->log("operator=( bas_struct<T> const & )");
		if( this == &v )
			return *this;
		this->base_class::operator=( v );
		return *this;
	}
	__BASIS_STRUCT_VEC_INTERFACE( fun, this->_fun, float_type );
	__BASIS_STRUCT_VEC_INTERFACE( alp, this->_alp, float_type );
};

#endif//__BASIS_STRUCT_H__
