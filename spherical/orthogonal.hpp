#ifndef __ORTHOGONAL_HPP__
#define __ORTHOGONAL_HPP__
#include"orthogonal.h"

#if defined( __ERROR_MSG_ORTHOGONAL ) || defined( __LOG_MSG_ORTHOGONAL ) || defined( __PRINT_ORTHOGONAL )
#include<iostream>
#include<iomanip>
#endif

template<class pT>
orthogonal<pT>::orthogonal():polynomial<pT>(), _n(0), _m(0){}
template<class pT>
orthogonal<pT>::orthogonal(orthogonal<pT> const & orth):polynomial<pT>( orth ), _n(orth._n), _m(orth._m){}
template<class pT>
orthogonal<pT>::orthogonal(polynomial<pT> const & v):polynomial<pT>( v ), _n(0), _m(0){}
template<class pT>
orthogonal<pT>::orthogonal(int const & __size):polynomial<pT>( __size ), _n(0), _m(0){}
template<class pT>
orthogonal<pT> & orthogonal<pT>::operator=(orthogonal<pT> const & orth)
{
	if( this == &orth ) return *this;
	this->polynomial<pT>::operator=( orth );
	_n = orth.n();
	_m = orth.m();
	return *this;
}
template<class pT> int orthogonal<pT>::n(int const & __n){return _n = __n;}
template<class pT> int orthogonal<pT>::m(int const & __m){return _m = __m;}
template<class pT> int & orthogonal<pT>::n(){return _n;}
template<class pT> int & orthogonal<pT>::m(){return _m;}
template<class pT> int const & orthogonal<pT>::n()const{return _n;}
template<class pT> int const & orthogonal<pT>::m()const{return _m;}
template<class pT> int orthogonal<pT>::poly_size()const{return pT::size();}
#ifdef  __PRINT_ORTHOGONAL
template<class pT>
void orthogonal<pT>::print(std::ostream & out, std::string title)const
{
	int p = 4, w = p + 8, w_x = 8;
	int sz = 8 + w + pT::size() * w_x;

	//out << "--------------------------" << std::endl;
	//out << "---------'title'----------" << std::endl;
	for(int i = 0; i < sz; ++i) out << '-'; out << std::endl;
	int lh_sz = (sz-title.size()-2)/2;
	for(int i = 0; i < lh_sz; ++i) out << '-';
	out << '\'' << title << '\'';
	int rh_sz = sz - title.size() - lh_sz - 2;
	for(int i = 0; i < rh_sz; ++i) out << '-'; out << std::endl;

	out.setf( std::ios::scientific );
	out.precision( p );
	out <<  std::setw( 4 ) << "n" << std::setw( 4 ) << "m" <<
		std::setw( w ) << "d";
	for(int i = 0; i < pT::size(); ++i)
		out << std::setw( w_x-2 ) << "x[" << i << "]";
	out << std::endl << std::endl;
	out << std::setw( 4 ) << this->n() << std::setw( 4 ) << this->m();
	pT const * ptr = this->data();
	for(int i = 0; i < this->size(); ++i, ++ptr)
	{
		if( i ) out << std::setw(8) << "";
#ifdef  __PRINT_POLY_N
		ptr->print( out, p, w_x );
#else
		out << std::setw(w) << std::setprecision(p) << ptr->d;
		for(int j = 0; j < pT::size(); ++j)
			out << std::setw(w_x) << ptr->x[j];
		out << std::endl;
#endif
	}
	out << std::endl;
	out.unsetf( std::ios::scientific );
}
#endif//__PRINT_ORTHOGONAL
#ifdef  __LOG_MSG_ORTHOGONAL
template<class pT>
void orthogonal<pT>::log(std::ostream & out, std::string s)const
{
	out << "[" << this << "] orthogonal<pT>::" << s << std::endl;
}
#endif


#include"poly.n.h"

template struct orthogonal<poly_n<float, 3> >;
template struct orthogonal<poly_n<float, 4> >;
template struct orthogonal<poly_n<double, 3> >;
template struct orthogonal<poly_n<double, 4> >;
template struct orthogonal<poly_n<long double, 3> >;
template struct orthogonal<poly_n<long double, 4> >;
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template struct orthogonal<poly_n<mpfr::mpreal, 3> >;
template struct orthogonal<poly_n<mpfr::mpreal, 4> >;
#endif

#endif//__ORTHOGONAL_HPP__
