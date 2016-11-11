#ifndef __SPHERICAL_HPP__
#define __SPHERICAL_HPP__
#include"stdlib.h"// exit
#include"spherical.h"
#include<math.h>// sqrt
#include"sf.math.h"// I4PI, fact_n, pown

/*
template<class T>
T Sigma_sph(xyz_pown_integral<T> const & sigma_ijk, spherical<T> const & spher)
{
	if( !spher.is_even() )
		return T(0);
	T value = T(0), t;
	typename spherical<T>::polynomial_type const * p_pol = &spher[0];
	int const * x;
	for(int i = 0; i < spher.size(); ++i)
	{
		x = p_pol->x;
		t = sigma_ijk(x[0], x[1], x[2]);
		t *= p_pol->d;
		value += t;
		++p_pol;
	}
	return value;
}
*/

#if defined( __ERROR_MSG_SPHERICAL ) || defined( __LOG_MSG_SPHERICAL )
#include<iostream>
#endif

//template<class T, class pT = poly_n<T, 3> >
// TODO: in method 'optimize_less' decrease memory allocation/deallocation
// TODO: int _n, _m -- make some class that's parent for 'spherical' and 'legendre' (reason is that both classes have _n, _m member-elements)
template<class T>
spherical<T>::spherical():orthogonal<pT>(){}
template<typename T>
spherical<T>::spherical(spherical<T> const & spher):orthogonal<pT>( spher ){}
//template<typename T>
//spherical<T>::spherical(polynomial<poly_n<T,3> > const & v):orthogonal<pT>( v ){}
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename T>
spherical<T>::spherical(__spherical<T> const & spher)
{
	this->operator=(spher);
}
#endif
template<typename T>
spherical<T>::spherical(int const & __size):orthogonal<pT>( __size ){}
template<typename T>
spherical<T>::~spherical(){}
template<typename T>
spherical<T> & spherical<T>::operator=(spherical<T> const & spher)
{
	if( this == &spher ) return *this;
	this->orthogonal<pT>::operator=( spher );
	return *this;
}
#if defined( __X_SPHERICAL_H__ ) && defined( __X_POLYNOMIAL )
template<typename T>
spherical<T> & spherical<T>::operator=(__spherical<T> const & spher)
{
	this->resize( spher.size() );
	for(int i = 0; i < spher.size(); ++i)
		this->array_scp<pT>::operator[](i) = spher[i];
	return *this;
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator*=(__spherical<T> const & spher)
{
	spherical<T> tmp( spher );
	return this->polynomial<poly_n<T,3> >::operator*=( tmp );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator+=(__spherical<T> const & spher)
{
	spherical<T> tmp( spher );
	return this->polynomial<poly_n<T,3> >::operator+=( tmp );
}
#endif
template<typename T>// operator*=
polynomial<poly_n<T,3> > & spherical<T>::operator*=(polynomial<poly_n<T,3> > const & v)
{
	return this->polynomial<poly_n<T,3> >::operator*=( v );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator*=(poly_n<T,3> const & v)
{
	return this->polynomial<poly_n<T,3> >::operator*=( v );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator*=(__polynom<T,3> const & v)
{
	poly_n<T,3> pol( v );
	return this->polynomial<poly_n<T,3> >::operator*=( pol );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator*=(T const & v)
{
	return this->polynomial<poly_n<T,3> >::operator*=( v );
}
template<typename T>// operator+=
polynomial<poly_n<T,3> > & spherical<T>::operator+=(polynomial<poly_n<T,3> > const & v)
{
	return this->polynomial<poly_n<T,3> >::operator+=( v );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator+=(poly_n<T,3> const & v)
{
	return this->polynomial<poly_n<T,3> >::operator+=( v );
}
template<typename T>
polynomial<poly_n<T,3> > & spherical<T>::operator+=(__polynom<T,3> const & v)
{
	poly_n<T,3> pol( v );
	return this->polynomial<poly_n<T,3> >::operator+=( pol );
}
template<typename T>
int spherical<T>::run_r2()
{
	if( this->size() != 3 )
		this->resize( 3 );
	pT * p = this->data();
	for(int i = 0; i < 3; ++i)
	{
		p->set_zero_x();
		p->d = T(1);
		p->x[i] = 2;
		++p;
	}
	return 0;
}
template<typename T>
int spherical<T>::run(int const & __n, int const & __m, int to_norma )
{
#ifdef  __LOG_MSG_SPHERICAL
	this->log(std::cout, "run(int const & , int const & , int )");
#endif
	legendre<T> P_nm, cos_sin;
	if( P_nm.run( __n, __m ) )
	{
#ifdef  __ERROR_MSG_SPHERICAL
		std::cerr << "Error: from spherical<T>::run(int const & __n, int const & __m)" << std::endl;
		std::cerr << "__n : " << __n << std::endl;
		std::cerr << "__m : " << __m << std::endl;
#endif
		exit(1);
	}
	cos_sin.run_cos_sin( __m );
	P_nm *= cos_sin;
	this->run(P_nm);
	if( to_norma == 1 ) this->normalize();
	else if( to_norma >= 2 ) this->normalize_4pi();
	//if( this->m()%2 ) *this *= T(-1);
	return 0;
}
template<typename T>
int spherical<T>::run(legendre<T> const & cs__P_nm)
{
	this->resize( cs__P_nm.size() );
	this->n( cs__P_nm.n() );
	this->m( cs__P_nm.m() );
	poly_n<T,3> * p_xyz = this->data();
	poly_n<T,4> const * p_4 = &cs__P_nm[0];
	//typename spherical<T>::polynomial_type * p_xyz = this->data();
	//typename legendre<T>::polynomial_type const * p_4 = &cs__P_nm[0];
	for(int i = 0; i < this->size(); ++i)
	{
		p_xyz->d = p_4->d;
		p_xyz->x[0] = p_4->x[2];// cos(phi)
		p_xyz->x[1] = p_4->x[3];// sin(phi)
		p_xyz->x[2] = p_4->x[0];// cos(theta)
		++p_xyz;
		++p_4;
	}
	return 0;
}
template<typename T>
T spherical<T>::norm()const// norm_4pi() / 4PI
{
	T _norm = this->norm_4pi();
	_norm *= math::numeric_constants<T>::i_pi;// 1/(4PI)
	_norm /= 4;
	return _norm;
}
template<typename T>
T spherical<T>::norm_4pi()const
	// m == 0 : (2n+1)
	// m != 0 : (2n+1) * (n-|m|)!/(n+|m|)! * 2
{
	if( this->n() == 0 ) return T(1);
	T _norm_4pi = T(this->n() * 2 + 1);
	if( this->m() == 0 )
		return _norm_4pi;
	int abs_m = this->m();
	abs_m = (abs_m < 0 ? -abs_m : abs_m);
	_norm_4pi *= math::fact_n<T>(this->n() - abs_m);
	_norm_4pi /= math::fact_n<T>(this->n() + abs_m);
	//_norm_4pi *= math::fact_n<long double>(this->n() - abs_m);
	//_norm_4pi /= math::fact_n<long double>(this->n() + abs_m);
	_norm_4pi *= 2;
	return _norm_4pi;
}
template<typename T>
T spherical<T>::normalize()
{
	T const _norm = T( sqrt(this->norm()) );
	*this *= _norm;
	return _norm;
}
template<typename T>
T spherical<T>::normalize_4pi()
{
	T const _norm_4pi = T( sqrt(this->norm_4pi()) );
	*this *= _norm_4pi;
	return _norm_4pi;
}
template<typename T>
T spherical<T>::calc(T const * __r)const
{
	T value = T(0);
	typename spherical<T>::polynomial_type const * ppol = this->data();
	//poly_n<T,3> const * ppol = this->data();
	for(int i = 0; i < this->size(); ++i, ++ppol)
		value += ppol->calc_r( __r );
	return value;
}
template<typename T>
void spherical<T>::optimize_less()
{
	int sz = this->size(), * lack_n = new int[sz], * lack_pow = new int[sz], * lack_sz = new int[sz], new_sz = 0, tmp_si = 0;
	poly_n<T,3> * p_xyz = this->data(), * p_b = 0, * tmp_p = 0;
	for(int i = 0; i < sz; ++i)
	{
		if( p_xyz->sum_x() > this->n() )
		{
#ifdef  __ERROR_MSG_SPHERICAL
			std::cerr << "Error: from spherical<T>::optimize_less()" << std::endl;
			std::cerr << "n : " << this->n() << std::endl;
			std::cerr << "m : " << this->m() << std::endl;
			std::cerr << "sum_x : " << p_xyz->sum_x() << std::endl;
			std::cerr << "x : " << p_xyz->x[0] << std::endl;
			std::cerr << "y : " << p_xyz->x[1] << std::endl;
			std::cerr << "z : " << p_xyz->x[2] << std::endl;
#endif
			exit(1);
		}
		lack_n[i] = this->n() - p_xyz++->sum_x();
	}
	for(int i = 0; i < sz; ++i)
	{
		tmp_si = lack_n[i];
		if( tmp_si%2 )
		{
#ifdef  __ERROR_MSG_SPHERICAL
			std::cerr << "Error: from spherical<T>::optimize_less()" << std::endl;
			std::cerr << "n : " << this->n() << std::endl;
			std::cerr << "m : " << this->m() << std::endl;
			std::cerr << "tmp_si : " << tmp_si << std::endl;
#endif
			exit(1);
		}
		tmp_si /= 2;
		lack_pow[i] = tmp_si;
		new_sz += ( lack_sz[i] = math::pown<int>(3, tmp_si ) );
		//new_sz += ( lack_sz[i] = pow(3, tmp_si ) );
	}
	if( new_sz == sz )
	{
		delete [] lack_n;
		delete [] lack_pow;
		delete [] lack_sz;
		return;
	}
	spherical<T> spher_new( new_sz ), spher_buf, spher_r2(3);
	spher_r2.run_r2();
	p_b = &spher_new[0];
	int _lack_sz = 0;
	p_xyz = this->data();
	for(int i = 0; i < sz; ++i)
	{
		*p_b = *p_xyz;
		_lack_sz = lack_sz[i];
		spher_buf.set( 1, _lack_sz, p_b);// dangerous magic: explicit set of member elements (do not repeat!)
		for(int j = 0; j < lack_pow[i]; ++j)
			spher_buf *= spher_r2;
		p_b += _lack_sz;
		spher_buf.zero();                // dangerous magic: zeroize all member elements (do not repeat!)
		++p_xyz;
	}
	this->orthogonal<pT>::operator=(spher_new);
	delete [] lack_n;
	delete [] lack_pow;
	delete [] lack_sz;
}
template<typename T>
void spherical<T>::optimize()
{
	this->optimize_equal();
	this->optimize_zero();
	this->optimize_less();
	this->optimize_equal();
	this->optimize_zero();
}
template<typename T>
void spherical<T>::generate(array_scp<spherical<T> > & v_spher, int __n )
{
	legendre<T> P_n, cs__P_nm, cos_sin;
	P_n.run( __n );
	v_spher.resize(2 * __n + 1);
	spherical<T> * p_spher = &v_spher[0];
	for(int __m = -__n; __m <= __n; ++__m)
	{
		cs__P_nm = P_n;
		cs__P_nm.run_m( __m );
		cos_sin.run_cos_sin( __m );
		cs__P_nm *= cos_sin;
		p_spher->n( __n );
		p_spher->m( __m );
		p_spher->run( cs__P_nm );
		p_spher++->optimize_ez();
	}
}
#ifdef  __LOG_MSG_SPHERICAL
template<typename T>
void spherical<T>::log(std::ostream & out, std::string s)const
{
	out << "[" << this << "] spherical<T>::" << s << std::endl;
}
#endif

template struct spherical<float>;
template struct spherical<double>;
template struct spherical<long double>;
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template struct spherical<mpfr::mpreal>;
#endif

#endif//__SPHERICAL_HPP__
