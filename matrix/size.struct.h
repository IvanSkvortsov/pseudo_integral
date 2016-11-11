#ifndef __SIZE_STRUCT_H__
#define __SIZE_STRUCT_H__
#include<iostream>// cerr, endl
#include<cstdlib>// exit

#define __PRAGMA_PACK_4

#ifdef  __PRAGMA_PACK_4
  #pragma pack( push, 4 )
#endif

#define __SIZE_STRUCT_ERROR_METHOD( num ) \
inline void error(char const * _method, char const _message[] = "nothing to do here")const\
{\
	std::cerr << "Error: [" << this << "] size_struct<" << num << ">::" << _method << ", " << _message << std::endl;\
}

#define __SIZE_STRUCT_DIM__SIZEI
#define __SIZE_STRUCT_SAFE

// size
template<int dim> struct size_struct;

template<> struct size_struct<1> 
{
public:
	typedef int size_type;
protected:
	__SIZE_STRUCT_ERROR_METHOD( 1 );
	size_type _M_size;
public:
	inline size_struct() : _M_size(0){};
	inline size_struct(const size_type __size) : _M_size( __size ){}
	inline size_struct(size_struct<1> const & v) : _M_size( v._M_size ){}
	inline size_struct<1> & operator=(size_struct<1> const & v)
	{
		if( this == &v )
			return *this;
		this->_M_size = v.size();
		return *this;
	}

	inline size_type const & size()const{return this->_M_size;}
	inline size_type & size(){return this->_M_size;}
	inline void init_size(){}
#ifdef  __SIZE_STRUCT_DIM__SIZEI
	inline size_type const & size(int i)const{return this->_M_size;}
	inline size_type & size(int i){return this->_M_size;}
#endif

	inline const int dimension()const{return 1;}
	inline const bool operator==( size_struct<1> const & v)const
	{
		return this->_M_size == v.size();
	}
	inline const bool operator!=( size_struct<1> const & v)const{ return this->_M_size != v.size();}
};

#define __SIZE_STRUCT_ASSIGN_OPERATOR( lhs_dim, rhs_dim )\
inline size_struct<lhs_dim> & operator=(size_struct<rhs_dim> const & v)\
{\
	if( this == &v )\
		return *this;\
	this->size_struct<rhs_dim>::operator=( v );\
	return *this;\
}

template<> struct size_struct<2> : public size_struct<1>
{
public:
	typedef typename size_struct<1>::size_type size_type;
protected:
	__SIZE_STRUCT_ERROR_METHOD( 2 );
	size_type _M_n, _M_m;
public:
	inline size_struct() : size_struct<1>(), _M_n(0), _M_m(0){};
	inline size_struct(const size_type __n, const size_type __m) : _M_n(__n), _M_m(__m){this->init_size();}
	inline size_struct(size_struct<2> const & v) : size_struct<1>( v ), _M_n( v._M_n), _M_m( v._M_m ){}
	__SIZE_STRUCT_ASSIGN_OPERATOR( 2, 1 );
	inline size_struct<2> & operator=(size_struct<2> const & v)
	{
		if( this == &v )
			return *this;
		this->size_struct<1>::operator=( v );
		this->_M_n = v.n();
		this->_M_m = v.m();
		return *this;
	}

	inline void init_size()
	{
		this->_M_size = this->_M_n;
		this->_M_size *= this->_M_m;
	}
#ifdef  __SIZE_STRUCT_DIM__SIZEI
	inline size_type const & size()const{return this->size_struct<1>::size();}
	inline size_type & size(){return this->size_struct<1>::size();}
	inline size_type const & size(int i)const{return *(&this->_M_n + i);}
	inline size_type & size(int i){return *(&this->_M_n + i);}
#endif

	inline size_type const & n()const{return this->_M_n;}
	inline size_type & n(){return this->_M_n;}
	inline size_type const & m()const{return this->_M_m;}
	inline size_type & m(){return this->_M_m;}

	inline const int dimension()const{return 2;}
	inline const bool operator==( size_struct<2> const & v)const
	{
		return this->_M_n == v.n() && this->_M_m == v.m() && this->size_struct<1>::operator==( v );// this->_M_size == v.size();
	}
	inline const bool operator!=( size_struct<2> const & v)const{ return !this->operator==( v );}

#ifdef  __SIZE_STRUCT_SAFE
	inline void error_info(int i, int j)const
	{
		std::cerr << "i : " << i << std::endl;
		std::cerr << "n : " << this->n() << std::endl;
		std::cerr << "j : " << j << std::endl;
		std::cerr << "m : " << this->m() << std::endl;
	}
#endif
	inline const int operator()(int i, int j)const
	{
#ifdef  __SIZE_STRUCT_SAFE
		if( i >= this->n() || j >= this->m() )
		{
			this->error("operator()(int i, int j)", "i j ?");
			this->error_info(i, j);
			exit(1);
		}
#endif
		return i * this->m() + j;
	}
};

template<> struct size_struct<3> : public size_struct<2>
{
public:
	typedef typename size_struct<2>::size_type size_type;
protected:
	__SIZE_STRUCT_ERROR_METHOD( 3 );
	size_type _M_p;
public:
	inline size_struct() : size_struct<2>(), _M_p(0){};
	inline size_struct(const size_type __n, const size_type __m, const size_type __p) : size_struct<2>( __n, __m), _M_p( __p )
	{
		//this->init_size();
		this->size() *= this->_M_p;
	}
	inline size_struct(size_struct<3> const & v) : size_struct<2>( v ), _M_p( v._M_p ){}
	__SIZE_STRUCT_ASSIGN_OPERATOR( 3, 1 );
	__SIZE_STRUCT_ASSIGN_OPERATOR( 3, 2 );
	inline size_struct<3> & operator=(size_struct<3> const & v)
	{
		if( this == &v )
			return *this;
		this->size_struct<2>::operator=( v );
		this->_M_p = v.p();
		return *this;
	}

	inline void init_size()
	{
		this->size_struct<2>::init_size();
		this->_M_size *= this->_M_p;
	}
	inline size_type const & p()const{return this->_M_p;}
	inline size_type & p(){return this->_M_p;}

	inline const int dimension()const{return 3;}

	inline const bool operator==( size_struct<3> const & v)const
	{
		return this->size_struct<2>::operator==( v ) && this->_M_p == v.p();
	}
	inline const bool operator!=( size_struct<3> const & v)const{ return !this->operator==( v );}

#ifdef  __SIZE_STRUCT_SAFE
	inline void error_info(int i, int j, int k)const
	{
		this->size_struct<2>::error_info(i, j);
		std::cerr << "k : " << k << std::endl;
		std::cerr << "p : " << this->p() << std::endl;
	}
#endif
	inline const int operator()(int i, int j)const{return this->size_struct<2>::operator()(i, j);}
	inline const int operator()(int i, int j, int k)const
	{
#ifdef  __SIZE_STRUCT_SAFE
		if( k >= this->p() )
		{
			this->error("operator()(int i, int j, int k)", "i j k?");
			this->error_info(i, j, k);
			exit(1);
		}
#endif
		return this->size_struct<2>::operator()(i, j) * this->p() + k;
	}
};

template<> struct size_struct<4> : public size_struct<3>
{
public:
	typedef typename size_struct<3>::size_type size_type;
protected:
	__SIZE_STRUCT_ERROR_METHOD( 4 );
	size_type _M_q;
public:
	inline size_struct() : size_struct<3>(), _M_q(0){};
	inline size_struct(const size_type __n, const size_type __m, const size_type __p, const size_type __q) :
		size_struct<3>( __n, __m, __p), _M_q( __q )
	{
		//this->init_size();
		this->size() *= this->_M_q;
	}
	inline size_struct(size_struct<4> const & v) : size_struct<3>( v ), _M_q( v._M_q ){}
	__SIZE_STRUCT_ASSIGN_OPERATOR( 4, 1 );
	__SIZE_STRUCT_ASSIGN_OPERATOR( 4, 2 );
	__SIZE_STRUCT_ASSIGN_OPERATOR( 4, 3 );
	inline size_struct<4> & operator=(size_struct<4> const & v)
	{
		if( this == &v )
			return *this;
		this->size_struct<3>::operator=( v );
		this->_M_q = v.q();
		return *this;
	}

	inline void init_size()
	{
		this->size_struct<3>::init_size();
		this->_M_size *= this->_M_q;
	}
	inline size_type const & q()const{return this->_M_q;}
	inline size_type & q(){return this->_M_q;}

	inline const int dimension()const{return 4;}

	inline const bool operator==( size_struct<4> const & v)const
	{
		return this->size_struct<3>::operator==( v ) && this->_M_q == v.q();
	}
	inline const bool operator!=( size_struct<4> const & v)const{ return !this->operator==( v );}

#ifdef  __SIZE_STRUCT_SAFE
	inline void error_info(int i, int j, int k, int l)const
	{
		this->size_struct<3>::error_info(i, j, k);
		std::cerr << "l : " << l << std::endl;
		std::cerr << "q : " << this->q() << std::endl;
	}
#endif
	inline const int operator()(int i, int j)const{return this->size_struct<2>::operator()(i, j);}
	inline const int operator()(int i, int j, int k)const{return this->size_struct<3>::operator()(i, j, k);}
	inline const int operator()(int i, int j, int k, int l)const
	{
#ifdef  __SIZE_STRUCT_SAFE
		if( l >= this->q() )
		{
			this->error("operator()(int i, int j, int k, int l)", "i j k l?");
			this->error_info( i, j, k, l );
			exit(1);
		}
#endif
		return this->size_struct<3>::operator()(i, j, k) * this->q() + l;
	}
};

#ifdef  __PRAGMA_PACK_4
  #pragma pack(pop)
#endif

#endif//__SIZE_STRUCT_H__
