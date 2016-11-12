#ifndef __BASIS_SET_H__
#define __BASIS_SET_H__
#include<vector>
#include"basis.defs.h"// pointers, stl_vector, ... interface macroses
#include<iostream>// cerr, endl
#include<fstream>// ifstream
#include<cstdlib>// exit

#define __BASIS_SET_DEBUG
#ifdef  __BASIS_SET_DEBUG
  #include<cassert>
#endif

#define __BASIS_SET_PRINT
#ifdef  __BASIS_SET_PRINT
  #include<iomanip>// setw
#endif

#ifndef __BASE_DATA_TYPEDEF
#define __BASE_DATA_TYPEDEF( base_class )\
typedef typename base_class::value_type value_type;\
typedef typename base_class::reference reference;\
typedef typename base_class::const_reference const_reference;\
typedef typename base_class::pointer pointer;\
typedef typename base_class::const_pointer const_pointer;
#endif

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
		_vec_struct _fun;
		_pos_struct _alp;
	} _fun_struct;
};

// basis_fun:
// 1. number of coefficients --> %bas_struct<T>::_fun._size, %bas_struct<T>::_alp._size
template<typename T>
struct basis_fun : protected basis_struct<T>::_fun_struct
{
	typedef typename basis_struct<T>::_fun_struct base_class;
	typedef T float_type;

	inline basis_fun(): base_class(){}
	inline basis_fun( basis_fun<T> const & v ): base_class( v ){}
	inline basis_fun( base_class const & v ): base_class( v ){}
	inline ~basis_fun(){}
	inline basis_fun<T> & operator=( base_class const & v )
	{
		this->base_class::operator=( v );
		return *this;
	}
	inline basis_fun<T> & operator=( basis_fun<T> const & v )
	{
		if( this == &v )
			return *this;
		this->base_class::operator=( v );
		return *this;
	}
	__POINTER_INTERFACE( fun, this->_fun._data, float_type );
	__POINTER_INTERFACE( alp, this->_alp._data, float_type );
	// %set_fun_data( pointer * )
	// %get_fun_data()
	// %get_fun_data( int )
	// %get_fun( int )
	__REFERENCE_INTERFACE( fun_size, this->_fun._size, int );
	__REFERENCE_INTERFACE( alp_pos,  this->_alp._pos,  int );
	// %set_fun_size( int )
	// %get_fun_size()
};

// basis_lx:
// 1. total number of alphas for fixed ang. mom. 'lx' --> %bas_struct<T>::_alp._size
// 2. total number of functions -//- --> %base_class::size()
template<typename T>
struct basis_lx : protected basis_struct<T>::_vec_struct, public std::vector<basis_fun<T> >
{
	typedef std::vector<basis_fun<T> > base_class;
	__BASE_DATA_TYPEDEF( base_class );

	typedef T float_type;
	__POINTER_INTERFACE( alp, this->_data, float_type );
	__REFERENCE_INTERFACE( alp_size, this->_size, int );
	inline const int get_fun_size()const{ return this->size(); }
	inline void set_fun_size(int __size ){ this->resize(__size); }
};

// basis_set:
// 1. number of maximum ang. mom. --> %_lx_max, %base_class::size()-1
// 2. total number of alphas of fixed basis set --> %_alp.size() <==> %get_alp_size()
// 3. total number of coefficients of fixed basis set --> %_fun.size() <==> %get_fun_size()
template<typename T>
struct basis_set : public std::vector<basis_lx<T> >
{
	typedef std::vector<basis_lx<T> > base_class;
	__BASE_DATA_TYPEDEF( base_class );
	typedef value_type bas_lx;
	typedef T float_type;
protected:
	int _lx_max;
	std::vector<T> _alp;
	std::vector<T> _fun;
	std::vector<int> _alp_lx;// _alp_lx[l] - total number of alphas for fixed ang. mom. %l
	std::vector<int> _fun_lx;// _fun_lx[l] - number of contracted functions for fixed ang. mom. %l
#ifdef  __BASIS_SET_PRINT
	inline void info(std::ostream & out = std::cout)const;
#else
	inline void info()const{}
#endif
	inline void error( const char * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] basis_set<T>::" << _method << ", " << _message << std::endl; }
public:
	inline void set_lx_max( const int & __lx_max )
	{
		this->_lx_max = __lx_max;
		this->base_class::resize( __lx_max + 1 );
		this->_alp_lx.resize( __lx_max + 1 );
		this->_fun_lx.resize( __lx_max + 1 );
	}
	inline const int & get_lx_max()const{ return this->_lx_max; }
	__STL_VECTOR_INTERFACE( alp, this->_alp, float_type );
	__STL_VECTOR_INTERFACE( fun, this->_fun, float_type );
	__STL_VECTOR_INTERFACE( alp_lx, this->_alp_lx, int );
	__STL_VECTOR_INTERFACE( fun_lx, this->_fun_lx, int );
	inline const int get_alp_pos( int lx )const{ return (*this)[lx].get_alp_data() - this->get_alp_data(); }
	inline const int get_fun_size( int lx, int fun_num )const{ return (*this)[lx][fun_num].get_fun_size(); }
	inline const int get_alp_pos( int lx, int fun_num )const
	{
#ifdef  __BASIS_SET_DEBUG
		assert( lx < this->size() && fun_num < (*this)[lx].size() );
#endif
		return (*this)[lx].get_alp_data() -  this->get_alp_data() + (*this)[lx][fun_num].get_alp_pos();
	}

	inline void pub_info()const{ this->info();}
	inline void read( std::ifstream & inp );
	inline void read_map( std::ifstream & inp );
	inline void init();
};

#ifdef  __BASIS_SET_PRINT
template<typename T> void basis_set<T>::info( std::ostream & out )const
{
	out << std::setw(3) << this->get_lx_max() << std::endl;
	const_pointer p_bl = this->data();
	typename value_type::const_pointer p_bf;
	for(int lx = 0; lx < this->size(); ++lx, ++p_bl )
	{
		p_bf = p_bl->data();
		out << std::setw(4) << p_bl->get_alp_size() << std::setw(4) << p_bl->get_fun_size() << std::endl;
		for(int i = 0; i < p_bl->get_alp_size(); ++i)
		{
			out << std::setw(26) << std::setprecision(16) << std::scientific << p_bl->get_alp( i ) << std::endl;
		}
		for(int i = 0; i < p_bl->size(); ++i, ++p_bf )
		{
			out << std::setw(8 + 4) << p_bf->get_fun_size() << std::setw(4) << p_bf->get_alp_pos() << std::endl;
			for(int j = 0; j < p_bf->get_fun_size(); ++j)
				out << std::setw(26) << std::setprecision(16) << std::scientific << p_bf->get_fun( j ) << std::endl;
		}
		out << std::endl;
	}
}
#endif

template<typename T> void basis_set<T>::read( std::ifstream & inp )
{
	int __lx_max;
	inp >> __lx_max;
	this->set_lx_max( __lx_max );// %_lx_max, %base_class::resize, %_alp_lx.resize, %_fun_lx.resize

	pointer p_bl = this->data();
	typename value_type::pointer p_bf;
	int __alp_size = 0, __fun_size = 0;
	this->_alp.reserve(  40 ); this->_alp.clear();
	this->_fun.reserve( 100 ); this->_fun.clear();
	T alp_, fun_;
	for(int lx = 0, fsize, asize; lx < this->size(); ++lx, ++p_bl )
	{
		inp >> asize >> fsize;
		if( inp == 0 )
		{
			this->error("read( std::ifstream & )", "unexpected end of input" );
			std::cerr << std::setw(10) << "alp_size" << " : " << std::setw(4) << asize << std::endl;
			std::cerr << std::setw(10) << "fun_size" << " : " << std::setw(4) << fsize << std::endl;
			std::cerr << std::setw(10) << "inp_tell" << " : " << std::setw(4) << inp.tellg() << std::endl;
			exit(1);
		}
		this->_alp_lx[lx] = asize;
		this->_fun_lx[lx] = fsize;
		p_bl->set_alp_size( asize );
		p_bl->set_fun_size( fsize );
		for(int i = 0; i < p_bl->get_alp_size(); ++i)
		{
			inp >> alp_;
			this->_alp.push_back( alp_ );
		}
		__alp_size += p_bl->get_alp_size();
		p_bf = p_bl->data();
		for(int i = 0, fsiz, apos; i < p_bl->size(); ++i, ++p_bf )
		{
			inp >> fsiz >> apos;
			p_bf->set_fun_size( fsiz );
			p_bf->set_alp_pos( apos );
#ifdef  __BASIS_SET_DEBUG
			assert( p_bf->get_alp_pos() < p_bl->get_alp_size() );
#endif
			for(int j = 0; j < p_bf->get_fun_size(); ++j)
			{
				inp >> fun_;
				this->_fun.push_back( fun_ );
			}
			__fun_size += p_bf->get_fun_size();
		}
	}
#ifdef  __BASIS_SET_DEBUG
	assert( this->get_alp_size() == __alp_size );
	assert( this->get_fun_size() == __fun_size );
#endif
	this->_alp.shrink_to_fit();
	this->_fun.shrink_to_fit();

	this->init();
}

template<typename T> void basis_set<T>::read_map( std::ifstream & inp )
{
	inp >> this->_lx_max;
	this->resize( this->_lx_max + 1 );
	pointer p_bl = this->data();
	typename value_type::pointer p_bf;
	int __alp_size = 0, __fun_size = 0;
	for(int lx = 0, fsize, asize; lx < this->size(); ++lx, ++p_bl )
	{
		inp >> asize >> fsize;
		if( inp == 0 )
		{
			this->error("read_map( std::ifstream & )", "unexpected end of input" );
			std::cerr << std::setw(10) << "alp_size" << " : " << std::setw(4) << asize << std::endl;
			std::cerr << std::setw(10) << "fun_size" << " : " << std::setw(4) << fsize << std::endl;
			std::cerr << std::setw(10) << "inp_tell" << " : " << std::setw(4) << inp.tellg() << std::endl;
			exit(1);
		}
		p_bl->set_alp_size( asize );
		p_bl->set_fun_size( fsize );
		__alp_size += p_bl->get_alp_size();
		p_bf = p_bl->data();
		for(int i = 0, fsiz, apos; i < p_bl->size(); ++i, ++p_bf )
		{
			inp >> fsiz >> apos;
			p_bf->set_fun_size( fsiz );
			p_bf->set_alp_pos( apos );
#ifdef  __BASIS_SET_DEBUG
			assert( p_bf->get_alp_pos() < p_bl->get_alp_size() );
#endif
			__fun_size += p_bf->get_fun_size();
		}
	}
	this->set_alp_size( __alp_size );
	this->set_fun_size( __fun_size );

	this->init();
}

template<typename T> void basis_set<T>::init()
{
	if( this->get_alp_data() == 0 || this->get_fun_data() == 0 )
	{
		this->error("init()", "not allocated %_alp, %_fun");
		exit(1);
	}
	int __alp_pos = 0, __fun_pos = 0;
	typename value_type::pointer p_bf;
	pointer p_bl = this->data();
	for(int lx = 0, fsize, asize; lx < this->size(); ++lx, ++p_bl )
	{
		p_bl->set_alp_data( this->get_alp_data() + __alp_pos );
		__alp_pos += p_bl->get_alp_size();
		p_bf = p_bl->data();
		for(int i = 0; i < p_bl->size(); ++i, ++p_bf )
		{
			p_bf->set_fun_data( this->get_fun_data() + __fun_pos );
			__fun_pos += p_bf->get_fun_size();
#ifdef  __BASIS_SET_DEBUG
			assert( p_bf->get_alp_pos() < p_bl->get_alp_size() );
#endif
			p_bf->set_alp_data( p_bl->get_alp_data() + p_bf->get_alp_pos() );
		}
	}
#ifdef  __BASIS_SET_DEBUG
	assert( __alp_pos <= this->get_alp_size() && __fun_pos <= this->get_fun_size() );
#endif
}

template<typename T>
struct basis : public std::vector<basis_set<T> >
{
	typedef std::vector<basis_set<T> > base_class;
	__BASE_DATA_TYPEDEF( base_class );
	typedef T float_type;
protected:
	std::string _file;
	inline void error( const char * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] basis<T>::" << _method << ", " << _message << std::endl; }
public:
	inline void read( char const * __file );
};

template<typename T> void basis<T>::read( char const * __file )
{
	this->_file = __file;
	std::ifstream inp( __file );
	if( !inp.is_open() )
	{
		this->error( "read( char const * )", "can't open file" );
		std::cerr << "file : '" << __file << "'" << std::endl;
		exit(1);
	}
	int _size = 0;
	inp >> _size;
	this->resize( _size );
	pointer p_bs = this->data();
	for(int i = 0; i < this->size(); ++i, ++p_bs)
		p_bs->read( inp );
}

#endif//__BASIS_SET_H__
