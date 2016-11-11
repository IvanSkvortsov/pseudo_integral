#ifndef __XYZ_POWN_INTEGRAL_HPP__
#define __XYZ_POWN_INTEGRAL_HPP__
#include"xyz.pown.integral.h"
#include<cstdlib>// exit
#include<iostream>// cout, cerr, endl
#include<iomanip>// setw, setprecision
#include<fstream>// ifstream, ofstream
#include"fact.h"// fact_2n1_n(i) := (2*i+1)!!, i = 0, 1, ...

#ifndef _100x1024x1024_// 100 Mb
#define _100x1024x1024_ 104857600
#endif

#ifndef _1024x1024_// 1 Mb
#define _1024x1024_ 1048576
#endif

#ifndef _512x1024_// 512 Kb
#define _512x1024_  524288
#endif

#ifndef _256x1024_// 256 Kb
#define _256x1024_  262144
#endif

#ifndef _128x1024_// 128 Kb
#define _128x1024_  131072
#endif

// error
#ifdef  __ERROR_xyz_pown_integral__
#include<iostream>
#endif


// info
#ifdef  __INFO_xyz_pown_integral__
#include<iostream>
#include<iomanip>
#include"logn.h"// log(x, n); n -- base (integer), x -- integer, return value : integer ~= log(x, n)
#endif

// log
#ifdef  __LOG_xyz_pown_integral__
#include<iostream>
#endif

// xyz_pown_integral
//
// calculate integrals like this
// T xyz_pown_integral<T>::operator()(i, j, k) := int( x^i * y^j * z^k, theta=0..Pi, phi=0..2*Pi);
// integral(i,j,k) value := 4*Pi*(i-1)!!*(j-1)!!*(k-1)!!/(i+j+k+1)!!
// P.S. warning, one should multiply the result of xyz_pown_integral<T>::operator(int , int, int ) by (4*Pi) in order to get mentioned integral value

template<typename T>
xyz_pown_integral<T>::xyz_pown_integral():
		_i_max(0), _j_max(0), _k_max(0), 
		_half_imax__p1(0), _half_jmax__p1(0), _half_kmax__p1(0),
		v(size_type(_1024x1024_)/sizeof(T))
{
#ifdef  __LOG_xyz_pown_integral__
	log("xyz_pown_integral()");
#endif
}
template<typename T>
xyz_pown_integral<T>::~xyz_pown_integral()
{
#ifdef  __LOG_xyz_pown_integral__
	log("~xyz_pown_integral()");
#endif
}
template<typename T> typename xyz_pown_integral<T>::size_type const & xyz_pown_integral<T>::i_max()const{return _i_max;}
template<typename T> typename xyz_pown_integral<T>::size_type const & xyz_pown_integral<T>::j_max()const{return _j_max;}
template<typename T> typename xyz_pown_integral<T>::size_type const & xyz_pown_integral<T>::k_max()const{return _k_max;}
template<typename T> typename xyz_pown_integral<T>::size_type & xyz_pown_integral<T>::i_max(){return _i_max;}
template<typename T> typename xyz_pown_integral<T>::size_type & xyz_pown_integral<T>::j_max(){return _j_max;}
template<typename T> typename xyz_pown_integral<T>::size_type & xyz_pown_integral<T>::k_max(){return _k_max;}
template<typename T> typename xyz_pown_integral<T>::size_type xyz_pown_integral<T>::i_max(size_type const & __i_max)
{_half_imax__p1 = __i_max/2 + 1;return _i_max = __i_max;}
template<typename T> typename xyz_pown_integral<T>::size_type xyz_pown_integral<T>::j_max(size_type const & __j_max)
{_half_jmax__p1 = __j_max/2 + 1;return _j_max = __j_max;}
template<typename T> typename xyz_pown_integral<T>::size_type xyz_pown_integral<T>::k_max(size_type const & __k_max)
{_half_kmax__p1 = __k_max/2 + 1;return _k_max = __k_max;}
template<typename T> void xyz_pown_integral<T>::half_max__p1()
{
	_half_imax__p1 = _i_max/2+1;
	_half_jmax__p1 = _j_max/2+1;
	_half_kmax__p1 = _k_max/2+1;
}
template<typename T> typename xyz_pown_integral<T>::size_type const xyz_pown_integral<T>::max_size()const{return v.size();}
template<typename T> typename xyz_pown_integral<T>::size_type const xyz_pown_integral<T>::size()const
{return _half_imax__p1 * _half_jmax__p1 * _half_kmax__p1;} 
template<typename T> T * xyz_pown_integral<T>::begin(){return &v[0];}
template<typename T> T * xyz_pown_integral<T>::end(){return this->begin()+v.size();}
template<typename T> T const * xyz_pown_integral<T>::begin()const{return &v[0];}
template<typename T> T const * xyz_pown_integral<T>::end()const{return this->begin()+v.size();}
template<typename T> void xyz_pown_integral<T>::set_zero()
{
	T * p = this->begin();
	for(size_type i = 0; i < this->size(); ++i)
		*p++ = T(0);
}
template<typename T> void xyz_pown_integral<T>::run(size_type const & __i_max, size_type const & __j_max, size_type const & __k_max)
{
	this->i_max(__i_max);
	this->j_max(__j_max);
	this->k_max(__k_max);
	this->run();
}
template<typename T> void xyz_pown_integral<T>::run()
{
#ifdef  __LOG_xyz_pown_integral__
	log("run()");
	std::cout << "    size : " << this->size() << std::endl;
	std::cout << "max_size : " << this->max_size() << std::endl;
#endif
	if( this->size() > this->max_size() )
	{
#ifdef  __ERROR_xyz_pown_integral__
		std::cerr << "Error: xyz_pown_integral<T>::run() : maximum size reached" << std::endl;
		std::cerr << "size     : " << this->size() << std::endl;
		std::cerr << "max_size : " << this->max_size() << std::endl;
#endif
		exit(1);
	}
	T * p = &v[0];
	// TODO: 1) realize less calculation and less memory usage for integral storage because of equivalence of (i,j,k), (j,i,k), etc.
	size_type __ijk_max = _i_max + _j_max + _k_max, __i_p_j, _half_ijkmax__p2 = __ijk_max/2 + 2;
	T * fact_2n1_src = new T[_half_ijkmax__p2];// fact_2n1_src[i] := (2*i-1)!!
	fact_2n1_src[0] = 1;
	for(int i = 1; i < _half_ijkmax__p2; ++i)
		fact_2n1_src[i] = fact_2n1_src[i-1] * (2*i-1);
		//fact_2n1_src[i] = math::fact_2n1_n(i-1);// fact_2n1_n(i-1) := (2*i-1)!!
	T const * p_im1 = fact_2n1_src, * p_jm1 = fact_2n1_src, * p_km1 = fact_2n1_src;
	T f_im1_x_jm1;
	for(size_type i = 0; i <= _i_max; i+=2, ++p_im1)
	{
		p_jm1 = fact_2n1_src;
		for(size_type j = 0; j <= _j_max; j+=2, ++p_jm1)
		{
			f_im1_x_jm1 = *p_im1;
			f_im1_x_jm1 *= *p_jm1;
			__i_p_j = i + j;
			p_km1 = fact_2n1_src;
			for(size_type k = 0; k <= _k_max; k+=2, ++p_km1)
			{
				//*p++ = (f_im1_x_jm1 * *p_km1) / fact_2n1_src[(__i_p_j + k)/2]+1;
				*p = f_im1_x_jm1;
				*p *= *p_km1;
				*p++ /= fact_2n1_src[(__i_p_j + k)/2+1];// (i+j+k+1)!! := fact_2n1_src[(i+j+k)/2+1]
			}
		}
	}
	delete [] fact_2n1_src;
}
template<typename T> T const & xyz_pown_integral<T>::operator()(size_type i, size_type j, size_type k)const
{
	static T zero_value = T(0);
	if( i%2 == 1 || j%2 == 1 || k%2 == 1 ) return zero_value;
	if( i > _i_max || j > _j_max || k > _k_max )
	{
#ifdef  __ERROR_xyz_pown_integral__
		std::cerr << "Error: xyz_pown_integral<T>::operator()(size_type i, size_type j, size_type k)" << std::endl;
		std::cerr << "i_max    : " << this->i_max() << std::endl;
		std::cerr << "j_max    : " << this->j_max() << std::endl;
		std::cerr << "k_max    : " << this->k_max() << std::endl;
		std::cerr << "i        : " << i << std::endl;
		std::cerr << "j        : " << j << std::endl;
		std::cerr << "k        : " << k << std::endl;
		std::cerr << "size     : " << this->size() << std::endl;
		std::cerr << "max_size : " << this->max_size() << std::endl;
#endif
		exit(1);
	}
	return v[ ((i * _half_jmax__p1 + j) * _half_kmax__p1 + k)/2 ];
}
template<typename T> void xyz_pown_integral<T>::write(char const * file, const int prec)const
{
#ifdef  __LOG_xyz_pown_integral__
	this->log("write(char const * file, const int prec )");
#endif
	std::ofstream out( file );
	out <<  std::setw(4) << this->i_max() <<
		std::setw(4) << this->j_max() <<
		std::setw(4) << this->k_max() <<
		std::setw(8) << this->size() <<
		std::setw(8) << this->v.size() << std::endl;
	T const * p = this->v.data();
	const int w = prec + 8;
	out.setf( std::ios::scientific );
	out.precision( prec );
	for(size_type i = 0; i <= this->i_max(); i+=2)
	{
		for(size_type j = 0; j <= this->j_max(); j+=2)
		{
			for(size_type k = 0; k <= this->k_max(); k+=2, ++p)
			{
				out << std::setw(4) << i << std::setw(4) << j << std::setw(4) << k <<
					std::setw(w) << std::setprecision( prec ) << *p << std::endl;
			}
		}
	}
}
template<typename T> void xyz_pown_integral<T>::read(char const * file)
{
#ifdef  __LOG_xyz_pown_integral__
	this->log("read(char const * )");
#endif
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
#ifdef  __ERROR_xyz_pown_integral__
		std::cerr << "Error: xyz_pown_integral<T>::read(const char * file) can't open file'" << file << "'" << std::endl;
#endif
		exit(1);
	}
	size_type __i_max, __j_max, __k_max, __size, __v_size;
	inp >> __i_max >> __j_max >> __k_max >> __size >> __v_size;
	this->i_max( __i_max );
	this->j_max( __j_max );
	this->k_max( __k_max );
	if( this->size() > __v_size || __size != this->size())
	{
#ifdef  __ERROR_xyz_pown_integral__
		std::cerr << "Error: xyz_pown_integral<T>::read(const char * file)" << std::endl;
		std::cerr << "this.size : " << this->size() << std::endl;
		std::cerr << "     size : " << __size << std::endl;
		std::cerr << " max_size : " << __v_size << std::endl;
#endif
		exit(1);
	}
	this->v.resize( __v_size );
	T * p = this->v.data();
	size_type __i, __j, __k;
	for(int i = 0; i < this->size(); ++i, ++p)
		inp >> __i >> __j >> __k >> *p;
}
template<typename T> void xyz_pown_integral<T>::pub_info()const
{
#ifdef  __INFO_xyz_pown_integral__
	this->info();
#endif
}
#ifdef  __INFO_xyz_pown_integral__
template<typename T> void xyz_pown_integral<T>::info()const
{
	std::string s = "-----";
	int s_size = s.size();
	char * ptr_zero = 0x0;
	int addr = (char *)this - ptr_zero;
	int _pow = math::logn_z(addr, 16), leng = (s_size + 2) * 2 + (_pow + 3);
	int idx_w = 12, w = leng - idx_w;
	std::cout << s << " [" << this << "] " << s << std::endl;
	std::cout << std::setw(idx_w) << "max_size : " << std::setw(w) << this->max_size() << std::endl;
	std::cout << std::setw(idx_w) << "size : " << std::setw(w) << this->size() << std::endl;
	std::cout << std::setw(idx_w) << "i_max : " << std::setw(w) << this->_i_max << std::endl;
	std::cout << std::setw(idx_w) << "j_max : " << std::setw(w) << this->_j_max << std::endl;
	std::cout << std::setw(idx_w) << "k_max : " << std::setw(w) << this->_k_max << std::endl;
	for(int i = 0; i < leng; ++i) std::cout << '-'; std::cout << std::endl;
}
#endif
#ifdef  __LOG_xyz_pown_integral__
template<typename T> void xyz_pown_integral<T>::log(std::string const & s)const
{
	std::cout << "[" << this << "] xyz_pown_integral<T>::" << s << ' ' << sizeof( T ) << std::endl;
}
#endif

template struct xyz_pown_integral<float>;
template struct xyz_pown_integral<double>;
template struct xyz_pown_integral<long double>;
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template struct xyz_pown_integral<mpfr::mpreal>;
#endif

#endif//__XYZ_POWN_INTEGRAL_HPP__
