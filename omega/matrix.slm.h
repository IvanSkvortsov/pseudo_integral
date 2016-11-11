#ifndef __MATRIX_SLM_H__
#define __MATRIX_SLM_H__
#include"matrix.vector.h"
#include"matrix.cursor.h"
#include"poly.n.h"
#include<iostream>// cerr, endl
#include<vector>

template<typename T> using poly_3 = poly_n<T,3>;
template<typename T> using slm_type = matrix_pointer_1<poly_3<T> >;
template<typename T> using vector_slm_base = std::vector<slm_type<T> >;
template<typename T> using matrix_slm_base = matrix_vector<slm_type<T> >;// matrix that is made from vector

template<typename T> using source_slm_type = std::vector<poly_3<T> >;

// matrix_slm:
//
// matrix of products of spherical harmonics
// matrix sizes:
//   N = (2*l + 1);
//   M = (lmb_max + 1) * (lmb_max + 1)
//   where l - is angular momentum of lhs spherical harmonic,
//   lmb_max - is maximum angular momentum of rhs spherical harmonics
// P.S. Sum_(i=0)^(i=P) (2*i + 1) = (P + 1) * (P + 1)
//
// e.i. the product S(l,m) * S(lmb,mu) is taken by following: 
//   1) matrix_slm<T>::operator()(l+m, lmb*lmb + lmb + mu); or
//   2) matrix_slm<T>::operator[]( (l+m) * (2*l + 1) + lmb * lmb + lmb + mu )
//
#define __SLM_MX_READ_CHECK_SIZE
template<typename T>
class matrix_slm : public matrix_slm_base<T>
{
public:
	typedef typename matrix_slm_base<T>::size_type size_type;
	typedef typename matrix_slm_base<T>::value_type value_type;
	typedef T float_type;
	typedef typename matrix_slm_base<T>::pointer pointer;
	typedef typename matrix_slm_base<T>::const_pointer const_pointer;
	typedef typename matrix_slm_base<T>::reference reference;
	typedef typename matrix_slm_base<T>::const_reference const_reference;
protected:
	size_type _l, _lmb_max;
	source_slm_type<T> _source;
	inline void error(const char * method_name, const char message[] = "nothing to do here")const;
public:
	matrix_slm();
	matrix_slm(matrix_slm<T> const & v);
	matrix_slm<T> & operator=(matrix_slm<T> const & v);

	int read( char const * file );

	inline source_slm_type<T> & source(){return this->_source;}
	inline source_slm_type<T> const & source()const{return this->_source;}
	inline size_type & l(){return this->_l;}
	inline size_type const & l()const{return this->_l;}
	inline size_type & lmb_max(){return this->_lmb_max;}
	inline size_type const & lmb_max()const{return this->_lmb_max;}
};

template<typename T> void matrix_slm<T>::error(const char * method_name, const char * message)const
{
	std::cerr << "Error: [" << this << "] matrix_slm<T>::" << method_name << ", " << message << std::endl;
}

template<typename T> int matrix_slm_write( const char * file, const int l_max, const int la_max, const int l);

// slm_mx -- is matrix of products of spherical harmonics
//
// std::vector<poly_3<T> > v_src -- data storage
// slm_mx<T> mx -- is array (std::vector<pointer_v<poly_3<T> > >, see 'matrix.h') of pointers (pointer_v<poly_3<T> >) to poly_3<T> which (poly_3)
//   are also pointered (and allocated) by 'v_src'
//   'mx' has pointers to data of 'v_src'
//
//	matrix of product: 
//       (lmb_max + 1)^2
//             |
//             V
// -----------------------
// |                     |
// |                     |
// |                     | <- (2 * l + 1)
// |                     |
// |                     |
// |                     |
// -----------------------
//
// (2 * l + 1) times are 'm' elements : -l, (-l + 1), (-l + 2), ..., 0, 1, ..., l
// (lmb_max + 1) * (lmb_max + 1) times are (lmb,mu) pairs : (0,0); (1,-1), (1,0), (1,1); (2,-2), (2,-1), (2,0), (2,1), (2,2); ...
// (n + 1)^2 := Sum_{i=0}^{i=n} (2 * i + 1)
//
// so that mx(l+m, lmb * lmb + lmb + mu) denotes S(l, m) multiplied by S(lmb, mu)
//
// all such matrices are stored in files (not binary, but symbolic)
// use 'slm_mx_read' to read matrix from file;
// use 'slm_mx_write' to write matrix into file;

#endif//__MATRIX_SLM_H__
