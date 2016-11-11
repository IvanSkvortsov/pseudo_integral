#ifndef __LEGENDRE_HPP__
#define __LEGENDRE_HPP__
#include"stdlib.h"// exit
#include"legendre.h"
#include"sf.math.h"

#ifdef  __ERROR_MSG_LEGENDRE
#include<iostream>
#endif

typedef unsigned long long int u64_t;
typedef unsigned int u_t;

template<typename T>
legendre<T>::legendre():orthogonal<poly_n<T,4> >(){}
template<typename T>
legendre<T>::legendre(legendre<T> const & legen):orthogonal<poly_n<T,4> >( legen ){}
template<typename T>
legendre<T>::legendre(int const & __size):orthogonal<poly_n<T,4> >( __size ){}
template<typename T>
legendre<T> & legendre<T>::operator=(legendre<T> const & legen)
{
	if( this == &legen ) return *this;
	this->orthogonal<poly_n<T,4> >::operator=( legen );
	return *this;
}
template<typename T>
void legendre<T>::run( int __n)
{
	this->n(__n);
	T t_i2pown = T(1);
	math::add_exp( t_i2pown, -this->n() );
	if( t_i2pown > 1 )
	{
		std::cerr << "Error: legendre<T>::run(int __n)" << std::endl;
		std::cerr << "t_i2pown : " << t_i2pown << std::endl;
		exit(1);
	}
	this->resize( this->n()/2 + 1 );
	poly_n<T,4> * p_legen = this->data();
	T d;
	for(int k = 0; k < this->size(); ++k)
	{
		d = t_i2pown;
		d *= math::cnk<u64_t>(this->n(), k);
		d *= math::cnk<u64_t>(2*this->n() - 2*k, this->n());
		d *= (k%2?-1:1);
		p_legen->d = d;
		p_legen->set_zero_x();
		int * x = p_legen->x;
		p_legen->x[0] = this->n() - 2 * k;// cos(theta)
		++p_legen;
	}
}
template<typename T>
int legendre<T>::run( int __n, int __m )
{
	this->run( __n );
	return this->run_m( __m );
}
template<typename T>
int legendre<T>::run_m( int __m)
{
	int abs_m = (__m < 0? -__m : __m);
	if( abs_m > this->_n )
	{
#ifdef  __ERROR_MSG_LEGENDRE
		std::cerr << "Error: legendre<T, pT>::run_m(int __m)" << std::endl;
		std::cerr << "__m : " << __m << std::endl;
		std::cerr << "_n  : " << this->n() << std::endl;
#endif
		return 1;
	}
	this->m( __m );
	if( this->m() == 0 ) return 0;
	int ct_pow = 0, iter = 0;//, * zero_k = new int[this->_size];
	poly_n<T,4> * p_legen = this->data();
	T t;
	for(int k = 0; k < this->size(); ++k)
	{
		ct_pow = p_legen->x[0];
		if( ct_pow < abs_m )
		{
			//zero_k[iter] = k;
			p_legen->d  = T(0);
			p_legen->set_zero_x();
			++iter;
		}
		else
		{
			t = math::fact_p<T>(ct_pow-abs_m+1, abs_m);
			//t = math::fact_n<T>(ct_pow);
			//t /= math::fact_n<T>(ct_pow-abs_m);
			p_legen->d *= t;
			p_legen->x[0] = ct_pow - abs_m;// cos(theta)
			p_legen->x[1] = abs_m;// sin(theta)
		}
		++p_legen;
	}
	//if( !iter )
	//{
		//delete [] zero_k;
	//	return 0;
	//}
	// little optimization
	// fast variant
	//this->resize( this->_size - iter );
	this->_size -= iter;
	return 0;
	/*
	legendre<T> tmp( this->_size - iter );
	p_legen = &tmp[0];
	int * p_zero = zero_k;
	for(int k = 0; k < this->_size; ++k)
	{
		if( *p_zero == k )
		{
			p_zero++;
			continue;
		}
	*p_legen++ = this->_data[k];
	}
	this->operator=(tmp);
	*/
	//delete [] zero_k;
	if( abs_m%2==0 )
		return 0;
	poly_n<T,4> * p = this->data();
	for(int i = 0; i < this->size(); ++i)
	{
		p->d = -p->d;
		++p;
	}
	return 0;
}
template<typename T>
void legendre<T>::run_cos_sin( int __m)
{
	this->m( __m );
	if( this->_m == 0 )
	{
		this->resize( 1 );
		this->data()->d = T(1);//T(isqrt_2);
		this->data()->set_zero_x();
		return;
	}
	int abs_m = this->m(), _is_m_neg = (abs_m < 0 ? 1 : 0 ), __new_size = 0;
	abs_m = ( abs_m < 0 ? -abs_m : abs_m );
	for(int i = _is_m_neg; i <= abs_m; i += 2) __new_size++;
	this->resize( (_is_m_neg ? (abs_m+1)/2 : (abs_m/2 + 1) ) );
	if( this->_size != __new_size )
	{
#ifdef  __ERROR_MSG_LEGENDRE
		std::cerr << "Error: legendre<T, pT>::run_cos_sin(int __m)" << std::endl;
		std::cerr << "__m : " << __m << std::endl;
		std::cerr << "this->_size : " << this->_size << std::endl;
		std::cerr << " __new_size : " << __new_size << std::endl;
#endif
		exit(1);
	}
	poly_n<T,4> * p_cs = this->data();
	int __k = 0;
	for(int i = _is_m_neg; i <= abs_m; i += 2)
	{
		//p_cs->d = ( __k%2==0 ? NewtonC<T>(abs_m, i) : -NewtonC<T>(abs_m, i) );
		//p_cs->d = ( (i-_is_m_neg)%4 ? -NewtonC<T>(abs_m, i) : NewtonC<T>(abs_m, i) );
		p_cs->d = math::cnk<u64_t>(abs_m, i);
		if( ((i-_is_m_neg)/2)%2 )
			p_cs->d *= -1;
		//p_cs->d = ( ((i-_is_m_neg)/2)%2 ? -math::cnk(abs_m, i) : math::cnk(abs_m, i) );
		p_cs->set_zero_x();
		p_cs->x[2] = abs_m - i;// cos(phi)
		p_cs->x[3] = i;// cin(phi)
		//p_cs->cp = abs_m - i;
		//p_cs->sp = i;
		++p_cs;
		++__k;
	}
	if( __k != this->size() )
	{
#ifdef  __ERROR_MSG_LEGENDRE
		std::cerr << "Error: legendre<T, pT>::run_cos_sin(int __m)" << std::endl;
		std::cerr << "__m : " << __m << std::endl;
		std::cerr << "this->_size : " << this->size() << std::endl;
		std::cerr << "__k : " << __k << std::endl;
#endif
		exit(1);
	}
}

template struct legendre<float>;
template struct legendre<double>;
template struct legendre<long double>;
#include"mpreal.use.h"
#ifdef __MPREAL_USE
#include"mpreal.h"
template struct legendre<mpfr::mpreal>;
#endif

#endif//__LEGENDRE_HPP__
