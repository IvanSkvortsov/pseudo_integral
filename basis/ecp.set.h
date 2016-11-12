#ifndef __ECP_SET_H__
#define __ECP_SET_H__
#include<vector>
#include"basis.defs.h"
#include<iostream>// cerr, endl
#include<iomanip>// setw, setprecision, scientific
#include<fstream>// ifstream
#include<cstdlib>// exit

template<typename T>
struct ecp_struct
{
	typedef struct
	{
		int _size, _pos;
		T * _fun, * _alp;
		int * _n;
	} _fun_struct;
};

template<typename T>
struct ecp_fun : protected ecp_struct<T>::_fun_struct
{
	typedef typename ecp_struct<T>::_fun_struct base_class;
	typedef T float_type;
public:

	inline ecp_fun(): base_class(){}
	inline ecp_fun( ecp_fun<T> const & v ): base_class( v ){}
	inline ecp_fun( base_class const & v ): base_class( v ){}
	inline ~ecp_fun(){}
	inline ecp_fun<T> & operator=( base_class const & v )
	{
		this->base_class::operator=( v );
		return *this;
	}
	inline ecp_fun<T> & operator=( ecp_fun<T> const & v )
	{
		if( this == &v )
			return *this;
		this->base_class::operator=( v );
		return *this;
	}
	__REFERENCE_INTERFACE( fun_size, this->_size, int );
	__REFERENCE_INTERFACE( alp_pos, this->_pos, int );
	__POINTER_INTERFACE( fun, this->_fun, float_type );
	__POINTER_INTERFACE( alp, this->_alp, float_type );
	__POINTER_INTERFACE( n, this->_n, int );
};

template<typename T> using ecp_lx = std::vector<ecp_fun<T> >;

#define __ECP_SET_ASSIGN_CODE( rhs, float_type )\
ecp_set<float_type> tmp1( rhs ), tmp2;\
memcpy( &tmp2, this, sizeof( *this ) );\
memcpy( this, &tmp1, sizeof( *this ) );\
memset( &tmp1, 0, sizeof( *this ) );

template<typename T>
//struct ecp_set : public std::vector<ecp_fun<T> >
struct ecp_set : public std::vector<ecp_lx<T> >
{
	//typedef std::vector<ecp_fun<T> > base_class;
	typedef std::vector<ecp_lx<T> > base_class;
	typedef typename base_class::value_type value_type;
	typedef typename base_class::pointer pointer;
	typedef typename base_class::const_pointer const_pointer;
	typedef typename base_class::reference reference;
	typedef typename base_class::const_reference const_reference;
	typedef T float_type;
protected:
	int _l_max, _lso_max, _n_core;
	std::vector<T> _alp;
	std::vector<T> _fun;
	std::vector<int> _n;
	std::vector<int> _alp_lx;
	std::vector<int> _fun_lx;
	inline void info( std::ostream & out = std::cout )const;
	inline void error( const char * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] ecp_set<T>::" << _method << ", " << _message << std::endl; }
public:
	inline ecp_set(): base_class(), _alp(), _fun(), _n(){}
	inline ecp_set( ecp_set<T> const & v ): base_class( v ), _alp(v._alp), _fun(v._fun), _n(v._n){}
	inline ecp_set( base_class const & v ): base_class( v ), _alp(), _fun(), _n(){}
	inline ~ecp_set(){}
	inline ecp_set<T> & operator=( ecp_set<T> const & v )
	{
		if( this == &v )
			return *this;
		// Warning! Danger!
		__ECP_SET_ASSIGN_CODE( v, float_type );
		return *this;
	}
	inline ecp_set<T> & operator=( base_class const & v )
	{
		// Warning! Danger!
		__ECP_SET_ASSIGN_CODE( v, float_type );
		return *this;
	}
	__REFERENCE_INTERFACE( l_max, _l_max, int );
	__REFERENCE_INTERFACE( lso_max, _lso_max, int );
	__REFERENCE_INTERFACE( n_core, _n_core, int );
	inline void resize(){ this->base_class::resize( this->get_l_max() + this->get_lso_max() + 1 ); }
	inline void resize(int __size){ this->base_class::resize( __size ); }
	inline void set_size( int __size )
	{
		this->set_fun_size( __size );
		this->set_alp_size( __size );
		this->set_n_size( __size );
	}
	__STL_VECTOR_INTERFACE( fun, this->_fun, float_type );
	__STL_VECTOR_INTERFACE( alp, this->_alp, float_type );
	__STL_VECTOR_INTERFACE( n, this->_n, int );
	__STL_VECTOR_INTERFACE( alp_lx, this->_alp_lx, int );
	__STL_VECTOR_INTERFACE( fun_lx, this->_fun_lx, int );

	inline void read( std::ifstream & inp );
	inline void read( const char * file )
	{
		std::ifstream inp( file );
		if( !inp.is_open() )
		{
			this->error( "read( const char * )", "can't open file" );
			std::cerr << "file : '" << file << "'" << std::endl;
			exit(1);
		}
		this->read( inp );
	}
	inline void init();
	inline void pub_info()const{ this->info();}
};

template<typename T> void ecp_set<T>::read( std::ifstream & inp )
{
	std::string str;
	inp >> str >> this->_n_core >> this->_l_max >> this->_lso_max;
	this->resize();
	this->set_alp_lx_size( this->size() );
	this->set_fun_lx_size( this->size() );

	this->_fun.reserve( 40 ); this->_fun.clear();
	this->_alp.reserve( 40 ); this->_alp.clear();
	this->_n.reserve( 40 ); this->_n.clear();

	pointer p_el = this->data();// ecp_lx *
	typename value_type::pointer p_ef = 0;// ecp_fun *
	int __ef_size, __fun_size;
	T alp_, fun_;
	int n_;
	for( int l = 0; l < this->size(); ++l, ++p_el )
	{
		inp >> __fun_size;
		this->_fun_lx[l] = __fun_size;
		p_el->resize( __fun_size );
		p_ef = p_el->data();

		this->_alp_lx[l] = 0;
		for(int i = 0; i < __fun_size; ++i, ++p_ef )
		{
			inp >> __ef_size;
			p_ef->set_fun_size( __ef_size );
			p_ef->set_alp_pos( this->_alp_lx[l] );
			this->_alp_lx[l] += __ef_size;
			for(int j = 0; j < p_ef->get_fun_size(); ++j )
			{
				inp >> n_ >> alp_ >> fun_;
				this->_n.push_back( n_ );
				this->_fun.push_back( fun_ );
				this->_alp.push_back( alp_ );
			}
		}
	}
	this->_fun.shrink_to_fit();
	this->_alp.shrink_to_fit();
	this->_n.shrink_to_fit();

	this->init();
}

template<typename T> void ecp_set<T>::init()
{
	pointer p_el = this->data();
	typename value_type::pointer p_ef = 0;
	for(int l = 0, __pos = 0; l < this->size(); ++l, ++p_el )
	{
		p_ef = p_el->data();
		for(int i = 0; i < p_el->size(); ++i, ++p_ef )
		{
			p_ef->set_fun_data( this->get_fun_data() + __pos );
			p_ef->set_alp_data( this->get_alp_data() + __pos );
			p_ef->set_n_data( this->get_n_data() + __pos );
			__pos += p_ef->get_fun_size();
		}
	}
}

template<typename T> void ecp_set<T>::info( std::ostream & out )const
{
	const_pointer p_el = this->data();
	typename value_type::const_pointer p_ef = 0;
	for(int l = 0; l < this->size(); ++l, ++p_el )
	{
		out << std::setw(4) << l << std::setw(4) << p_el->size() << std::endl;
		p_ef = p_el->data();
		for(int i = 0; i < p_el->size(); ++i, ++p_ef )
		{
			out << std::setw(4) << p_ef->get_fun_size() << std::endl;
			for(int j = 0; j < p_ef->get_fun_size(); ++j)
			{
				out << std::setw(4) << p_ef->get_n( j ) <<
					std::setw(26) << std::setprecision(16) << std::scientific << p_ef->get_alp( j ) <<
					std::setw(26) << std::setprecision(16) << std::scientific << p_ef->get_fun( j ) << std::endl;
			}
			out << std::endl;
		}
	}
}

#endif//__ECP_SET_H__
