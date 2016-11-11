#ifndef __SPHERICAL_H__
#define __SPHERICAL_H__
#include"orthogonal.h"// polynomial : array_scp
#include"poly.n.h"// poly_n<T,int>  : array_n ; 'x.spherical.h'; and 'x.polynomial.h' __X_POLYNOMIAL
#include"legendre.h"

#define __ERROR_MSG_SPHERICAL
//#define __LOG_MSG_SPHERICAL

#if defined( __ERROR_MSG_SPHERICAL ) || defined( __LOG_MSG_SPHERICAL )
#include<iostream>
#endif

//template<class T, class pT = poly_n<T, 3> >
// TODO: in method 'optimize_less' decrease memory allocation/deallocation
template<class T>
struct spherical : public orthogonal<poly_n<T,3> >
{
	typedef polynomial<poly_n<T,3> > polynomial_t;
	typedef poly_n<T,3> polynomial_type;
	typedef poly_n<T,3> pT;
	typedef poly_n<T,3> value_type;
	typedef T float_type;
	spherical();
	spherical(spherical<T> const & spher);
	//spherical(polynomial<poly_n<T,3> > const & polyn);
	spherical(int const & __size);
	virtual ~spherical();
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
	spherical(__spherical<T> const & spher);// x_spherical
	spherical<T> & operator=(__spherical<T> const & spher);// x_spherical
	polynomial_t & operator*=(__spherical<T> const & spher);// x_spherical
	polynomial_t & operator+=(__spherical<T> const & spher);// x_spherical
	polynomial_t & operator+=(__polynom<T,3> const & pol_b);
	polynomial_t & operator*=(__polynom<T,3> const & pol_b);
#endif
	spherical<T> & operator=(spherical<T> const & spher);
	polynomial_t & operator+=(polynomial_t const & v);
	polynomial_t & operator*=(polynomial_t const & v);
	polynomial_t & operator+=(pT const & pol_b);
	polynomial_t & operator*=(pT const & pol_b);
	polynomial_t & operator*=(float_type const & val);
	int run_r2();
	int run(int const & __n, int const & __m, int to_norma = 1);
	int run(legendre<T> const & cs__P_nm);
	T norm()const;// norm_4pi() / 4PI
	T norm_4pi()const;
	T normalize();
	T normalize_4pi();
	void optimize_less();
	void optimize();
	T calc(T const * __r)const;
	static void generate(array_scp<spherical<T> > & v_spher, int __n );
private:
#ifdef  __LOG_MSG_SPHERICAL
	void log(std::ostream & out, std::string s)const;
#endif
};

#endif//__SPHERICAL_H__
