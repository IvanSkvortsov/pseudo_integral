#ifndef __MATRIX_SLM_HPP__
#define __MATRIX_SLM_HPP__
#include"matrix.slm.h"// matrix.h: vector; pointer.v.h, poly.n.h

#include<iostream>
#include<iomanip>
#include<fstream>

template<typename T> matrix_slm<T>::matrix_slm() : matrix_slm_base<T>(), _l(0), _lmb_max(0), _source(){}
template<typename T> matrix_slm<T>::matrix_slm(matrix_slm<T> const & v) : _l(v._l), _lmb_max(v._lmb_max)
									  // : matrix_slm_base<T>( v ), _source( v._source )
{
	source_slm_type<T> const & v_src = v.source();
	this->_source = v_src;

	matrix_slm_base<T> const & v_mx = v;
	matrix_slm_base<T> & mx = *this;
	mx.resize( v_mx.n(), v_mx.m() );

	slm_type<T> const * ptr_vmx = v_mx.data();
	slm_type<T> * ptr_mx = mx.data();
	poly_3<T> * p = this->_source.data();
	for(int i = 0; i < v_mx.size(); ++i, ++ptr_mx, ++ptr_vmx)
	{
		ptr_mx->data( p );
		ptr_mx->size() = ptr_vmx->size();
		p += ptr_vmx->size();
	}
}
template<typename T> matrix_slm<T> & matrix_slm<T>::operator=( matrix_slm<T> const & v)
{
	if( this == &v )
		return *this;
	this->_source = v.source();
	this->resize( v.n(), v.m() );
	this->_l = v.l();
	this->_lmb_max = v.lmb_max();
	poly_3<T> * p = this->_source.data();
	for(int i = 0; i < v.size(); ++i)
	{
		(*this)[i].data( p );
		(*this)[i].size() = v[i].size();
		p += v[i].size();
	}
	return *this;
}

static int matrix_slm_check( const int l_max, const int la_max, const int __l, const int lm_size, const int l_size, const int size )
{
	if( (__l + la_max + 1) * (__l + la_max + 1) == lm_size && (2*__l + 1) * lm_size == l_size )
		return 0;
	std::cerr << "Error: [matrix_slm_check]" << std::endl;
	std::cerr << std::setw(14) << "l_max : " << std::setw(8) << l_max << std::endl;
	std::cerr << std::setw(14) << "la_max : " << std::setw(8) << la_max << std::endl;
	std::cerr << std::setw(14) << "lm_size : " << std::setw(8) << lm_size << std::endl;
	std::cerr << std::setw(14) << "l_size : " << std::setw(8) << l_size << std::endl;
	std::cerr << std::setw(14) << "l : " << std::setw(8) << __l << std::endl;
	std::cerr << std::setw(14) << "lm_size = (l + la_max + 1)^2 : " << std::setw(8) << (__l + la_max + 1) * (__l + la_max + 1) << std::endl;
	std::cerr << std::setw(14) << "l_size = (2*l+1) * lm_size : " << std::setw(8) << (2*__l+1) * lm_size << std::endl;
	std::cerr << std::setw(14) << "size : " << std::setw(8) << size << std::endl;
	return 1;
}

// claculate %size of spherical harmonics' product
static unsigned int matrix_slm_size( const char * file )
{
	// file structre:
	//  1: %l_max    %la_max
	//  2: %l
	//  3: %lm_size  %l_size   (%size)
	//  4: %polynom_0_size
	//  5: %x  %y  %z   %d
	//  6: %x  %y  %z   %d
	//....................
	//  n: %polynom_(%l_size)_size
	//1+n: %x  %y  %z   %d
	//2+n: %x  %y  %z   %d
	//....................
	//
	// P.S. variable %size may not be there, so why %matrix_slm_size method is needed - to calculate this %size variable
	// P.P.S. %size = Sum_(i=0)^(i=%l_size) polynom_(%i)_size
	// %lm_size = (%l + %la_max + 1)^2 - is number of spherical harmonics with ang.mom = 0..(%l+%la_max)
	// %l_size  = (2 * %l + 1) * %lm_size - is number of spherical harmonics' product : 
	//   (lhs * rhs)_i, where i = 0..%l_size, %lhs has size eq (2 * %l + 1) and %rhs has size eq %lm_size
	// %polynom_n_size - is size of array with elements are : {int x, y, z; float_type d}
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
		std::cerr << "Error: [matrix_slm_size] can't open file '" << file << "'" << std::endl;
		return -1;
	}
	std::string s;
	for(int i = 0; i < 3; ++i)// 1st 3 lines are sizes (avoid them)
		getline( inp, s );
	unsigned int size = 0u, size_i;
	// %size_i - is exactly %polynom_size
	// %size   - is sum of %size_i's
	while(true)
	{
		inp >> size_i;
		if( inp == 0 )
			break;
		size += size_i;
		getline(inp, s);
		for(int i = 0; i < size_i; ++i)
			getline(inp, s);
	}
	return size;
}

template<typename T>
int matrix_slm<T>::read( const char * file )
{
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
		this->error("read", "can't open file");
		std::cerr << file << std::endl;
		return 1;
	}
	int l_max, la_max, __l, lm_size, l_size, size, size_summed;
	inp >> l_max >> la_max >> __l >> lm_size >> l_size >> size;// suppose %file has %size variable (see above, method %matrix_slm_size)
#ifdef  __SLM_MX_READ_CHECK_SIZE
	size_summed = matrix_slm_size( file );
#else
	size_summed = size;
#endif
	if( matrix_slm_check( l_max, la_max, __l, lm_size, l_size, size ) || size != size_summed || size_summed == -1)
	{
		this->error("read", "");
		std::cerr << "size : " << size_summed << std::endl;
		std::cerr << "size : " << size << std::endl;
		return 2;
	}
	this->_l = __l;
	this->_lmb_max = __l + la_max;

	this->_source.resize( size );
	this->resize( 2*__l+1, lm_size );

	typename matrix_slm<T>::pointer ptr_mx = this->data();// pointer_v<poly_n<T,3> > *
	poly_n<T,3> * p = this->_source.data();// typename slm_mx<T>::value_type::pointer
	int sz;
	for(int i = 0; i < l_size; ++i, ++ptr_mx)
	{
		inp >> sz;
		ptr_mx->data( p );
		ptr_mx->size() = sz;
		for(int j = 0; j < sz; ++j, ++p)
			inp >> p->x[0] >> p->x[1] >> p->x[2] >> p->d;
	}
	return 0;
}


//#define MATRIX_SLM_SPEC( type )\
	template class matrix_pointer_1<poly_3<type> >;\
	template class matrix_slm<type>;

#define MATRIX_SLM_SPEC( type )\
	template class matrix_slm<type>;

MATRIX_SLM_SPEC( float );
MATRIX_SLM_SPEC( double );
MATRIX_SLM_SPEC( long double );

//MATRIX_SPEC( pointer_v<poly_3<float> > );
//MATRIX_SPEC( pointer_v<poly_3<double> > );
//MATRIX_SPEC( pointer_v<poly_3<long double> > );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
//MATRIX_SPEC( pointer_v<poly_3<mpfr::mpreal> > );
MATRIX_SLM_SPEC( mpfr::mpreal );
#endif

#endif//__MATRIX_SLM_HPP__

