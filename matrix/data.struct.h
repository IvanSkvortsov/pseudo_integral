#ifndef __DATA_STRUCT_H__
#define __DATA_STRUCT_H__

#define __DATA_TYPEDEF( __type )\
typedef __type value_type;\
typedef __type * pointer;\
typedef __type const * const_pointer;\
typedef __type & reference;\
typedef __type const & const_reference;

#ifndef __BASE_DATA_TYPEDEF
#define __BASE_DATA_TYPEDEF( __base ) \
typedef typename __base::value_type value_type;\
typedef typename __base::pointer pointer;\
typedef typename __base::const_pointer const_pointer;\
typedef typename __base::reference reference;\
typedef typename __base::const_reference const_reference;
#endif

#pragma pack( push, 4 )

template<typename T> struct data_pointer;
template<typename T> struct data_array;

struct _Bit_reference
{
protected:
	friend class data_array<bool>;
	friend class data_pointer<bool>;
	_Bit_reference();
	int * _data;
	int _mask;
public:
	~_Bit_reference();
	operator bool () const;
	_Bit_reference & operator=(const bool x);
	_Bit_reference & operator=(const _Bit_reference & x);
	void flip();
};
	
inline _Bit_reference::_Bit_reference():_data(0), _mask(0){}
inline _Bit_reference::~_Bit_reference()
{
	this->_data = 0;
	this->_mask = 0;
}
inline void _Bit_reference::flip()
{
	*this->_data ^= this->_mask;
}
inline _Bit_reference::operator bool()const
{
	return *this->_data & this->_mask;
}
inline _Bit_reference & _Bit_reference::operator=( const bool x)
{
	if( (bool)*this != x )
		this->flip();
	return *this;
}
inline _Bit_reference & _Bit_reference::operator=( const _Bit_reference & x)
{
	return this->operator=( (bool)x );
}

#define __DATA_INTERFACE \
inline pointer data(){return this->_data;}\
inline const_pointer data()const{return this->_data;}\
inline pointer data(int i){return this->_data + i;}\
inline const_pointer data(int i)const{return this->_data + i;}\
\
inline reference operator[](int i){return this->_data[i];}\
inline const_reference operator[](int i)const{return this->_data[i];}

template<typename T>
struct data_array
{
public:
	__DATA_TYPEDEF( T );
protected:
	T _data[0];
public:
	inline data_array() : _data(){}
	inline data_array(data_array<T> const & v){}
	inline data_array<T> & operator=(data_array<T> const & v){return *this;}

	__DATA_INTERFACE;
};

#define __DATA_BOOL_INTERFACE \
inline pointer data(){}\
inline pointer data(int i){}\
inline const_pointer data()const{}\
inline const_pointer data(int i)const{}\
\
inline int * i_data(){return this->_data;}\
inline int * i_data(int i){return this->_data + i;}\
inline int const * i_data()const{return this->_data;}\
inline int const * i_data(int i)const{return this->_data + i;}\
\
inline int & i_value(int i){return this->_data[i];}\
inline int const & i_value(int i)const{return this->_data[i];}\
\
inline reference operator[](int i)\
{\
	reference ref;\
	ref._data = this->_data + i/__INT_BITS;\
	ref._mask = 1u << (i % __INT_BITS);\
	return ref;\
}\
inline const_reference operator[](int i)const\
{\
	return this->_data[i/__INT_BITS] & (1u << (i%__INT_BITS));\
}

template<> struct data_array<bool>
{
public:
	typedef bool value_type;
	typedef _Bit_reference reference;
	typedef bool const_reference;
	typedef void pointer;
	typedef void const_pointer;
	static const int __INT_BITS = sizeof(int) * 8;
protected:
	int _data[0];
public:
	inline data_array() : _data(){}
	inline data_array(data_array<bool> const & v ){}
	inline data_array<bool> & operator=(data_array<bool> const & v){return *this;}

	__DATA_BOOL_INTERFACE;
};

template<typename T>
struct data_pointer
{
public:
	__DATA_TYPEDEF( T );
protected:
	T * _data;
public:
	inline data_pointer() : _data(){}
	inline data_pointer(data_pointer<T> const & v): _data( v._data ){}
	inline data_pointer<T> & operator=(data_pointer<T> const & v)
	{
		if( this == &v ) return *this;
		this->data( v.data() );
		return *this;
	}

	inline void data(void * __data){ this->_data = (T *)__data;}
	inline void data(void const * __data){ this->_data = (T *)__data;}

	__DATA_INTERFACE;
};

template<> struct data_pointer<bool>
{
public:
	typedef bool value_type;
	typedef _Bit_reference reference;
	typedef bool const_reference;
	typedef void pointer;
	typedef void const_pointer;
	static const int __INT_BITS = sizeof(int) * 8;
protected:
	int * _data;
public:
	inline data_pointer() : _data(){}
	inline data_pointer(data_pointer<bool> const & v ) : _data( v._data ){}
	inline data_pointer<bool> & operator=(data_pointer<bool> const & v)
	{
		if( this == &v )
			return *this;
		data_pointer<bool> tmp( v );
		this->_data = tmp._data;
		return *this;
	}

	inline void data(void * __data){ this->_data = (int *)__data;}
	inline void data(void const * __data){ this->_data = (int *)__data;}

	__DATA_BOOL_INTERFACE;
};

#pragma pack(pop)

#endif//__DATA_STRUCT_H__
