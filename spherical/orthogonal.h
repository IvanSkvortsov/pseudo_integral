#ifndef __ORTHOGONAL_H__
#define __ORTHOGONAL_H__
#include"polynomial.h"

#define __PRINT_ORTHOGONAL
#define __ERROR_MSG_ORTHOGONAL
//#define __LOG_MSG_ORTHOGONAL

#if defined( __ERROR_MSG_ORTHOGONAL ) || defined( __LOG_MSG_ORTHOGONAL ) || defined( __PRINT_ORTHOGONAL )
#include<iostream>
#endif

//template<class T, class pT = poly_n<T, poly_size> >
// TODO: in method 'optimize_less' decrease memory allocation/deallocation
// TODO: int _n, _m -- make some class that's parent for 'orthogonal' and 'legendre' (reason is that both classes have _n, _m member-elements)
template<class pT>
struct orthogonal : public polynomial<pT>
{
	typedef pT polynomial_type;
	typedef pT value_type;
	typedef typename pT::value_type float_type;
	int _n, _m;
	orthogonal();
	orthogonal(orthogonal<pT> const & orth);
	orthogonal(polynomial<pT> const & v);
	orthogonal(int const & __size);
	orthogonal<pT> & operator=(orthogonal<pT> const & orth);
	int n(int const & __n);
	int m(int const & __m);
	int & n();
	int & m();
	int const & n()const;
	int const & m()const;
	int poly_size()const;
#ifdef  __PRINT_ORTHOGONAL
	void print(std::ostream & out, std::string title)const;
#endif//__PRINT_ORTHOGONAL
private:
#ifdef  __LOG_MSG_ORTHOGONAL
	void log(std::ostream & out, std::string s)const;
#endif
};

#endif//__ORTHOGONAL_H__
