#ifndef __POLYNOMIAL_HPP__
#define __POLYNOMIAL_HPP__
#include"polynomial.h"
#include<stdlib.h>// exit

#ifdef  __ERROR_MSG_POLINOMIAL
#include<iostream>
#endif

#include"equality.h"// math::is_zero

template<typename pT>
polynomial<pT>::polynomial():array_scp<pT>(){}
template<typename pT>
polynomial<pT>::polynomial(polynomial<pT> const & v):array_scp<pT>( v ){}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT>::polynomial(__spherical<float_type> const & v)
{
	this->resize( v.size() );
	for(int i = 0; i < v.size(); ++i)
		this->array_scp<pT>::operator[](i) = v[i];
}
#endif
*/
template<typename pT>
polynomial<pT>::polynomial(int const & __size):array_scp<pT>( __size ){}
template<typename pT>
polynomial<pT>::~polynomial(){}
template<typename pT>
int & polynomial<pT>::size(){return this->_size;}
template<typename pT>
int const & polynomial<pT>::size()const{return this->_size;}
template<typename pT>
polynomial<pT> & polynomial<pT>::operator=(polynomial<pT> const & v)
{
	if( this == &v ) return *this;
	this->array_scp<pT>::operator=( v );
	return *this;
}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT> & polynomial<pT>::operator=(__spherical<float_type> const & v)
{
	this->resize( v.size() );
	for(int i = 0; i < v.size(); ++i)
		this->array_scp<pT>::operator[](i) = v[i];
	return *this;
}
#endif
*/
template<typename pT>
polynomial<pT> & polynomial<pT>::operator+=(polynomial<pT> const & v)
{
	int v_size = v.size(), old_size = this->size();
	if( !v_size )
		return *this;
	polynomial<pT> tmp( *this );
	this->resize( old_size + v_size );
	*this = tmp;
	this->resize( old_size + v_size );
	pT * ptr_data = this->data()+old_size;
	for(int i = 0; i < v_size; ++i)
		*ptr_data++ = v[i];
	return *this;
}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT> & polynomial<pT>::operator+=(__spherical<float_type> const & v)
{
	polynomial<pT> tmp( v );
	return this->operator+=( tmp );
}
#endif
*/
template<typename pT>
polynomial<pT> & polynomial<pT>::operator*=(polynomial<pT> const & v)
{
	if( !this->size() ) return *this;
	else if( !v.size() )
	{
		this->resize( 0 );
		return *this;
	}
 	int v_size = v.size(), old_size = this->size(), new_size = this->size() * v_size;
	polynomial<pT> tmp( *this );
	pT * pol_a, * pol_c;
	this->resize( new_size );
	pol_c = this->data();
	for(int i = 0; i < old_size; ++i)
	{
		pol_a = &tmp[i];
		for(int j = 0; j < v_size; ++j)
			*pol_c++ = *pol_a * v[j];
	}
	return *this;
}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT> & polynomial<pT>::operator*=(__spherical<float_type> const & v)
{
	polynomial<pT> tmp( v );
	return this->operator*=( tmp );
}
#endif
*/
template<typename pT>
polynomial<pT> & polynomial<pT>::operator+=(pT const & pol_b)
{
	this->append( pol_b );
	return *this;
}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT> & polynomial<pT>::operator+=(__polynom<float_type,pT::__size> const & pol_b)
{
	pT pol = pT( pol_b );
	return this->operator+=( pol );
}
#endif
*/
template<typename pT>
polynomial<pT> & polynomial<pT>::operator*=(pT const & pol_b)
{
	pT * p_pol = this->data();
	for(int i = 0; i < this->size(); ++i)
	{
		*p_pol *= pol_b;
		++p_pol;
	}
	return *this;
}
/*
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename pT>// x_spherical
polynomial<pT> & polynomial<pT>::operator*=(__polynom<float_type, pT::__size> const & pol_b)
{
	pT pol = pT( pol_b );
	return this->operator*=( pol );
}
#endif
*/
template<typename pT>
polynomial<pT> & polynomial<pT>::operator*=(float_type const & val)
{
	pT * p_pol = this->data();
	for(int i = 0; i < this->size(); ++i)
	{
		*p_pol++ *= val;
	}
	return *this;
}
template<typename pT>
typename polynomial<pT>::float_type polynomial<pT>::calc_r(float_type const * r)const
{
	float_type value = float_type(0);
	pT const * p_pol = this->data();
	for(int i = 0; i < this->size(); ++i)
		value += p_pol++->calc_r( r );
	return value;
}
template<typename pT>
int polynomial<pT>::is_even()const
{
	pT const * p_pol = this->data();
	for(int i = 0; i < this->size(); ++i)
		if( p_pol++->is_even_x() )
			return 1;
	return 0;
}
template<typename pT>
void polynomial<pT>::optimize_equal()
{
	pT * p_a = this->data(), * p_b = 0;
	for(int i = 0; i < this->size(); ++i)
	{
		p_b = p_a + 1;
		for(int j = i+1; j < this->size(); ++j)
		{
			//if( math::is_equal<int>( p_a->x, p_b->x, 3) )
			if( pT::is_equal_x( *p_a, *p_b) )
			{
				p_a->d += p_b->d;
				p_b->d = float_type(0);
			}
			++p_b;
		}
		++p_a;
	}
}
template<typename pT>
void polynomial<pT>::optimize_zero()
{
	int iter = 0;
	for( pT * p_a = this->data(), * p_b = p_a + this->size() - 1, tmp_v; p_a <= p_b; ++p_a)
	{
		while( ( p_b->d == 0 || math::is_zero<float_type>( p_b->d ) ) && p_b >= p_a )
		{
			--p_b;
			++iter;
		}
		// p_b->d != 0
		if( (p_a->d == 0 || math::is_zero<float_type>( p_a->d )) && p_a < p_b )
		{
			tmp_v.operator=( *p_a );// tmp_v = *p_a;
			p_a->operator=(*p_b);// *p_a = *p_b;
			p_b->operator=(tmp_v);// *p_b = tmp_v;
			//
			--p_b;
			++iter;
		}
	}
	/*
	pT * p_a = this->data(), * p_b = p_a + this->size() - 1, tmp_v;
	while( p_a < p_b )
	{
		if( p_a->d == 0 || math::is_zero<float_type>( p_a->d ) )
		{
			while( ( p_b->d == 0 || math::is_zero<float_type>( p_b->d ) ) && p_b > p_a )
			{
				--p_b;
				++iter;
			}
			tmp_v.operator=( *p_a );// tmp_v = *p_a;
			p_a->operator=(*p_b);// *p_a = *p_b;
			p_b->operator=(tmp_v);// *p_b = tmp_v;
			//
			--p_b;
			++iter;
		}
		++p_a;
	}
	*/
	if( iter > this->size() )
	{
#ifdef  __ERROR_MSG_POLINOMIAL
		std::cerr << "Error: polynomial<T>::optimize_zero()" << std::endl;
		std::cerr << "size : " << this->size() << std::endl;
		std::cerr << "iter : " << iter << std::endl;
#endif
		exit(1);
	}
	// TODO: make the algo to avoid this extra check of zeroness
	//pT * p = this->data() + this->size() - iter - 1;
	//if( p->d == 0 || math::is_zero<float_type>( p->d ) )
	//	++iter;
	this->_size -= iter;
}
template<typename pT>
void polynomial<pT>::optimize_ez()
{
	this->optimize_equal();
	this->optimize_zero();
}
// Warning! Danger!
template<typename pT>
void polynomial<pT>::set(int const & __size, int const & __capacity, pT * p)
{
	this->_size = __size;
	this->_capacity = __capacity;
	this->_data = p;
}
// Warning! Danger!
template<typename pT>
void polynomial<pT>::zero()
{
	this->_size = 0;
	this->_capacity = 0;
	this->_data = 0;
}

template<class pT>
polynomial<pT> operator+(polynomial<pT> const & a, polynomial<pT> const & b)
{
	polynomial<pT> c( a );
	c += b;
	return c;
}

template<class pT>
polynomial<pT> operator+(polynomial<pT> const & a, pT const & b)
{
	polynomial<pT> c( a );
	c += b;
	return c;
}

template<class pT>
polynomial<pT> operator+(pT const & a, polynomial<pT> const & b)
{
	polynomial<pT> c( b );
	c += a;
	return c;
}

template<class pT>
polynomial<pT> operator*(polynomial<pT> const & a, polynomial<pT> const & b)
{
	polynomial<pT> c( a );
	c *= b;
	return c;
}

template<class pT>
polynomial<pT> operator*(polynomial<pT> const & a, pT const & b)
{
	polynomial<pT> c( a );
	c *= b;
	return c;
}

template<class pT>
polynomial<pT> operator*(pT const & a, polynomial<pT> const & b)
{
	polynomial<pT> c( b );
	c *= a;
	return c;
}

#include"poly.n.h"

template struct polynomial<poly_n<float, 3> >;
template struct polynomial<poly_n<float, 4> >;
template struct polynomial<poly_n<double, 3> >;
template struct polynomial<poly_n<double, 4> >;
template struct polynomial<poly_n<long double, 3> >;
template struct polynomial<poly_n<long double, 4> >;
//template struct polynomial<poly_n<double, 3> > : array_scp<poly_n<double,3> >;
//template struct polynomial<poly_n<double, 4> > : array_scp<poly_n<double,4> >;
//template struct polynomial<poly_n<long double, 3> > : array_scp<poly_n<long double,3> >;
//template struct polynomial<poly_n<long double, 4> > : array_scp<poly_n<long double,4> >;
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template struct polynomial<poly_n<mpfr::mpreal, 3> >;
template struct polynomial<poly_n<mpfr::mpreal, 4> >;
#endif

#endif//__POLYNOMIAL_HPP__
