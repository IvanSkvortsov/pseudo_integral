#ifndef __MATRIX_CURSOR_H__
#define __MATRIX_CURSOR_H__
#include"size.struct.h"
#include"data.struct.h"

#pragma pack( push, 4 )

// matrix_cursor
template<typename T, int dim>
struct matrix_cursor : public size_struct<dim>, public data_array<T>
{
public:
	typedef size_struct<dim> _size_class;
	typedef data_array<T> _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );// from 'data.struct.h'
protected:
	inline void error(char const * _method, char const _message[] = "nothing to do here")const
	{
		std::cerr << "Error: [" << this << "] matrix_cursor<T," << this->dimension() << ">::" << _method << ", " << _message << std::endl;
	}
public:
	inline matrix_cursor() : _size_class(), _data_class(){}
	inline matrix_cursor(matrix_cursor<T, dim> const & v) : _size_class( v )
	{
		const_pointer pv = v.data();
		pointer p = this->data();
		for(int i = 0; i < this->size(); ++i, ++pv, ++p)
			*p = *pv;
	}
	inline matrix_cursor<T, dim> & operator=(matrix_cursor<T, dim> const & v)
	{
		if( this == &v )
			return *this;
		this->_size_class::operator=( v );
		const_pointer pv = v.data();
		pointer p = this->data();
		for(int i = 0; i < this->size(); ++i, ++pv, ++p)
			*p = *pv;
		return *this;
	}
};

template<int dim> struct matrix_cursor<bool, dim> : public size_struct<dim>, public data_array<bool>
{
public:
	typedef size_struct<dim> _size_class;
	typedef data_array<bool> _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );// from 'data.struct.h'
protected:
	inline void error(char const * _method, char const _message[] = "nothing to do here")const
	{
		std::cerr << "Error: [" << this << "] matrix_cursor<bool," << this->dimension() << ">::" << _method << ", " << _message << std::endl;
	}
public:
	inline matrix_cursor() : _size_class(), _data_class(){}
	inline matrix_cursor(matrix_cursor<bool, dim> const & v) : _size_class( v )
	{
		for(int i = 0; i < this->i_size(); ++i)
			this->i_value(i) = v.i_value(i);
	}
	inline const size_type i_size()const
	{
		size_type __size = this->_size_class::size();
		__size /= _data_class::__INT_BITS;
		__size += 1;
		return __size;
	}
	inline matrix_cursor<bool, dim> & operator=(matrix_cursor<bool, dim> const & v)
	{
		if( this == &v )
			return *this;
		this->_size_class::operator=( v );
		for(int i = 0; i < this->i_size(); ++i)
			this->i_value(i) = v.i_value(i);
		return *this;
	}
};

// pointer
template<typename T, int dim>
struct matrix_pointer : public size_struct<dim>, public data_pointer<T>
{
public:
	typedef size_struct<dim> _size_class;
	typedef data_pointer<T> _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );// 'data.struct.h'
protected:
	inline void error(char const * _method, char const _message[] = "nothing to do here")const
	{
		std::cerr << "Error: [" << this << "] matrix_pointer<T," << this->dimension() << ">::" << _method << ", " << _message << std::endl;
	}
public:
	inline matrix_pointer() : _size_class(), _data_class(){}
	inline matrix_pointer(matrix_pointer<T, dim> const & v) : _size_class( v ), _data_class( v ){}
	inline matrix_pointer<T, dim> & operator=(matrix_pointer<T, dim> const & v)
	{
		if( this == &v )
			return *this;
		this->_size_class::operator=( v );
		this->_data_class::operator=( v );
		return *this;
	}
};

template<int dim>
struct matrix_pointer<bool,dim> : public size_struct<dim>, public data_pointer<bool>
{
public:
	typedef size_struct<dim> _size_class;
	typedef data_pointer<bool> _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );// 'data.struct.h'
protected:
	inline void error(char const * _method, char const _message[] = "nothing to do here")const
	{
		std::cerr << "Error: [" << this << "] matrix_pointer<bool," << this->dimension() << ">::" << _method << ", " << _message << std::endl;
	}
public:
	inline matrix_pointer() : _size_class(), _data_class(){}
	inline matrix_pointer(matrix_pointer<bool, dim> const & v) : _size_class( v ), _data_class( v ){}
	inline matrix_pointer<bool, dim> & operator=(matrix_pointer<bool, dim> const & v)
	{
		if( this == &v )
			return *this;
		this->_size_class::operator=( v );
		this->_data_class::operator=( v );
		return *this;
	}
};

#define __BASE_STRUCT_TYPEDEF( _base_class )\
typedef typename _base_class::_size_class _size_class;\
typedef typename _base_class::_data_class _data_class;

template<typename T>
struct matrix_cursor_4: public matrix_cursor<T,4>
{
public:
	typedef matrix_cursor<T,4> _base_class;
	typedef typename _base_class::_size_class _size_class;
	typedef typename _base_class::_data_class _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_cursor_4(): matrix_cursor<T,4>(){}
	inline matrix_cursor_4(matrix_cursor_4<T> const & v) : matrix_cursor<T,4>(v){}
	inline matrix_cursor_4<T> & operator=(matrix_cursor_4<T> const & v)
	{
		this->matrix_cursor<T,4>::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j, int k, int l)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k, l) );
	}
	inline const_reference operator()(int i, int j, int k, int l)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k, l) );
	}
};

template<typename T>
struct matrix_cursor_3: public matrix_cursor<T,3>
{
public:
	typedef matrix_cursor<T,3> _base_class;
	__BASE_STRUCT_TYPEDEF( _base_class );
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_cursor_3(): matrix_cursor<T,3>(){}
	inline matrix_cursor_3(matrix_cursor_3<T> const & v) : matrix_cursor<T,3>(v){}
	inline matrix_cursor_3<T> & operator=(matrix_cursor_3<T> const & v)
	{
		this->matrix_cursor<T,3>::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j, int k)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k) );
	}
	inline const_reference operator()(int i, int j, int k)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k) );
	}
};
template<typename T>
struct matrix_cursor_2: public matrix_cursor<T,2>
{
public:
	typedef matrix_cursor<T,2> _base_class;
	__BASE_STRUCT_TYPEDEF( _base_class );
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_cursor_2(): _base_class(){}
	inline matrix_cursor_2(matrix_cursor_2<T> const & v) : _base_class(v){}
	inline matrix_cursor_2<T> & operator=(matrix_cursor_2<T> const & v)
	{
		this->_base_class::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j) );
	}
	inline const_reference operator()(int i, int j)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j) );
	}
};

template<typename T>
struct matrix_pointer_4: public matrix_pointer<T,4>
{
public:
	typedef matrix_pointer<T,4> _base_class;
	typedef typename _base_class::_size_class _size_class;
	typedef typename _base_class::_data_class _data_class;
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_pointer_4(): matrix_pointer<T,4>(){}
	inline matrix_pointer_4(matrix_pointer_4<T> const & v) : matrix_pointer<T,4>(v){}
	inline matrix_pointer_4<T> & operator=(matrix_pointer_4<T> const & v)
	{
		this->matrix_pointer<T,4>::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j, int k, int l)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k, l) );
	}
	inline const_reference operator()(int i, int j, int k, int l)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k, l) );
	}
};

template<typename T>
struct matrix_pointer_3: public matrix_pointer<T,3>
{
public:
	typedef matrix_pointer<T,3> _base_class;
	__BASE_STRUCT_TYPEDEF( _base_class );
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_pointer_3(): matrix_pointer<T,3>(){}
	inline matrix_pointer_3(matrix_pointer_3<T> const & v) : matrix_pointer<T,3>(v){}
	inline matrix_pointer_3<T> & operator=(matrix_pointer_3<T> const & v)
	{
		this->matrix_pointer<T,3>::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j, int k)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k) );
	}
	inline const_reference operator()(int i, int j, int k)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j, k) );
	}
};
template<typename T>
struct matrix_pointer_2: public matrix_pointer<T,2>
{
public:
	typedef matrix_pointer<T,2> _base_class;
	__BASE_STRUCT_TYPEDEF( _base_class );
	typedef typename _size_class::size_type size_type;
	__BASE_DATA_TYPEDEF( _data_class );

	inline matrix_pointer_2(): _base_class(){}
	inline matrix_pointer_2(matrix_pointer_2<T> const & v) : _base_class(v){}
	inline matrix_pointer_2<T> & operator=(matrix_pointer_2<T> const & v)
	{
		this->_base_class::operator=( v );
		return *this;
	}

	inline reference operator()(int i, int j)
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j) );
	}
	inline const_reference operator()(int i, int j)const
	{
		return this->_data_class::operator[]( this->_size_class::operator()(i, j) );
	}
};

template<typename T> using matrix_cursor_1 = matrix_cursor<T,1>;

template<typename T> using matrix_pointer_1 = matrix_pointer<T,1>;
/*
template<typename T>
struct matrix_pointer_1
{
	__DATA_TYPEDEF( T );
	typedef int size_type;
protected:
	size_type _size;
	T * _data;
public:
	inline matrix_pointer_1():_size(0), _data(0){}
	inline matrix_pointer_1(matrix_pointer_1<T> const & v): _size(v._size), _data(v._data){}
	inline matrix_pointer_1<T> & operator=( matrix_pointer_1<T> const & v)
	{
		if( this == &v )
			return *this;
		this->_size = v.size();
		this->_data = (T *)v.data();
		//for(int i = 0; i < this->size(); ++i)
		//	this->_data[i] = v[i];
		return *this;
	}
	inline size_type & size(){ return this->_size;}
	inline const size_type & size()const{ return this->_size;}
	inline const_reference operator[](int i)const{ return this->_data[i];}
	inline reference operator[](int i){ return this->_data[i];}
	inline void data(pointer __data){this->_data = __data;}
	inline pointer data(){ return this->_data;}
	inline const_pointer data()const{ return this->_data;}
};
*/

#define MATRIX_POINTER_SPEC( type )\
	template struct matrix_pointer<type,4>;\
	template struct matrix_pointer<type,3>;\
	template struct matrix_pointer<type,2>;\
	template struct matrix_pointer<type,1>;\
	template struct matrix_pointer_4<type>;\
	template struct matrix_pointer_3<type>;\
	template struct matrix_pointer_2<type>;
	//template struct matrix_pointer_1<type>;

#define MATRIX_CURSOR_SPEC( type )\
	template struct matrix_cursor<type,4>;\
	template struct matrix_cursor<type,3>;\
	template struct matrix_cursor<type,2>;\
	template struct matrix_cursor<type,1>;\
	template struct matrix_cursor_4<type>;\
	template struct matrix_cursor_3<type>;\
	template struct matrix_cursor_2<type>;

#pragma pack(pop)

#endif//__MATRIX_CURSOR_H__
