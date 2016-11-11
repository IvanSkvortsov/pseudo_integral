#ifndef __POLYNOMIAL_H__
#define __POLYNOMIAL_H__

#include"array.scp.h"
#define __ERROR_MSG_POLINOMIAL

template<class pT>
struct polynomial: public array_scp<pT>
{
	typedef typename pT::value_type float_type;
	typedef pT value_type;
	typedef pT polynomial_type;
	polynomial();
	polynomial(polynomial<pT> const & v);
	polynomial(int const & __size);
	virtual ~polynomial();
	int & size();
	int const & size()const;
	polynomial<pT> & operator=(polynomial<pT> const & v);
	polynomial<pT> & operator+=(polynomial<pT> const & v);
	polynomial<pT> & operator*=(polynomial<pT> const & v);
	polynomial<pT> & operator+=(pT const & pol_b);
	polynomial<pT> & operator*=(pT const & pol_b);
	polynomial<pT> & operator*=(float_type const & val);
	float_type calc_r(float_type const * r)const;
	int is_even()const;
	void optimize_equal();
	void optimize_zero();
	void optimize_ez();// both optimizations: 'ez' = ('e' - equal, 'z' - zero)
	// Warning! Danger!
	void set(int const & __size, int const & __capacity, pT * p);
	// Warning! Danger!
	void zero();
};

// operator+
template<class pT> polynomial<pT> operator+(polynomial<pT> const & a, polynomial<pT> const & b);
template<class pT> polynomial<pT> operator+(polynomial<pT> const & a, pT const & b);
template<class pT> polynomial<pT> operator+(pT const & a, polynomial<pT> const & b);
// operator*
template<class pT> polynomial<pT> operator*(polynomial<pT> const & a, polynomial<pT> const & b);
template<class pT> polynomial<pT> operator*(polynomial<pT> const & a, pT const & b);
template<class pT> polynomial<pT> operator*(pT const & a, polynomial<pT> const & b);

#endif//__POLYNOMIAL_H__
