#ifndef __MATRIX_SLM_WRITE_HPP__
#define __MATRIX_SLM_WRITE_HPP__
#include"matrix.slm.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include"spherical.h"// spherical, orthogonal, polynomial, poly_n

#define __MATRIX_SLM_WRITE_COUT

template<typename T>
static void slm_write( std::ostream & out, spherical<T> const & buf, int prec = 105 )
{
	out << std::setw(4) << buf.size() << std::endl;
	poly_n<T,3> const * p = buf.data();
	int const * x, w = prec + 4;
	for(int i = 0; i < buf.size(); ++i, ++p)
	{
		x = p->x;
		for(int j = 0; j < 3; ++j)
			out << std::setw(4) << x[j];
		out << ' ' << std::setw(w) << std::setprecision(prec-1) << p->d << std::endl;
	}
}
template<typename T> int matrix_slm_write( const char * file, const int l_max, const int la_max, const int l)
{
	if( l_max < 0 || la_max < 0 || l > l_max || l < 0 )
	{
		std::cerr << "Error: matrix_slm<T>::write" << std::endl;
		std::cerr << "l_max : " << l_max << std::endl;
		std::cerr << "la_max : " << la_max << std::endl;
		std::cerr << "l : " << l << std::endl;
		return 1;
	}

	const int lmb_max = l + la_max, lm_size = (l + la_max + 1) * (l + la_max + 1), l_size = (2 * l + 1) * lm_size;
	// lm_size - number of spherical harmonics with ang.mom. = 0..%(l + la_max) aka 0..%lmb_max
	// l_size  - number of elements of %slm_mx matrix
	// P.S. (2*l + 1) - number of spherical harmonics with ang.mom. eq %l
	std::ofstream out( file );
	out <<  std::setw(4) << l_max << std::setw(4) << la_max << std::endl <<
		std::setw(4) << l << std::endl <<
		std::setw(8) << lm_size << std::setw(8) << l_size << std::endl;
	
	static std::vector<spherical<T> > vs;
	if( vs.size() == 0 )
	{
		vs.resize( (l_max + la_max + 1) * (l_max + la_max + 1) );// because %vs is static it's better to allocate maximum needed memory
		// and generate all of spherical harmonics ever needed (in case of %l_max and %la_max)
		spherical<T> * p = vs.data();
		int i = 0;
		for(; i <= (l_max + la_max); ++i)
			for(int m = -i; m <= i; ++m, ++p)
				p->run( i, m, 2 );// 2 --> Int( S(l,m) * S(l,m) ) := 4Pi; 1 --> Int( S(l,m) * S(l,m) ) := 1
	}
	else if( (l_max + la_max + 1) * (l_max + la_max + 1) > vs.size() )// if it needs more spherical harmonics
	{
		std::vector<spherical<T> > tmp( vs );// copy previous array
		vs.resize( (l_max + la_max + 1) * (l_max + la_max + 1) );// reallocate memory

		spherical<T> * p = vs.data();
		for(int i = 0; i < tmp.size(); ++i, ++p)
			*p = tmp[i];

		int i = 0;
		for(int it = 0; it < tmp.size(); it += (2 * i + 1), ++i);

		for(; i < (l_max + la_max); ++i)// generate remaining spherical harmonics
			for(int m = -i; m <= i; ++m, ++p)
				p->run( i, m, 2);
	}
	spherical<T> buf, * slm_l = vs.data() + l * l/* get spherical with ang.mom. eq %l */, * slm_lmb = 0;
	unsigned int __size = 0u, prec = 105;
	static unsigned long long int it = 0;
	for(int m = -l; m <= l; ++m, ++slm_l)
	{
#ifdef  __MATRIX_SLM_WRITE_COUT
		std::cout << std::setw(4) << l << std::setw(4) << m << std::setw(4) << slm_l->size() << std::endl << std::flush;
#endif
		for(int lmb = 0; lmb <= lmb_max; ++lmb)
		{
			slm_lmb = vs.data() + lmb * lmb;// get spherical with ang.mom. eq %lmb
			for(int mu = -lmb; mu <= lmb; ++mu, ++slm_lmb)
			{
				buf = *slm_l;
				buf *= *slm_lmb;
				buf.optimize_ez();
				slm_write<T>( out, buf, prec );// write into %out stream
				if( l == 5 && it == 0 )// extra code wrote for %optimize_zero (from struct %spherical) bug checking
				{
					for(int i = 0; i < buf.size(); ++i)
						it += (buf[i].d == 0);
					if( it )
					{
						std::cout << "l : " << l << std::endl;
						std::cout << "lmb : " << lmb << std::endl;
						std::cout << "m : " << m << std::endl;
						std::cout << "mu : " << mu << std::endl;
						std::cout << "==> zero <==" << std::endl;
					}
				}
				__size += buf.size();
			}
		}
	}
	// %__size - number of spherical's elements : {int x, y, z; float_type d;}
#ifdef  __MATRIX_SLM_WRITE_COUT
	std::cout << std::setw(8) << __size << std::endl << std::flush;
#else
	out << std::setw(8) << __size << std::endl;
#endif
	return 0;
}

#define MATRIX_SLM_WRITE_SPEC( type )\
	template int matrix_slm_write<type>( const char * file, const int l_max, const int la_max, const int l);\
	template void slm_write( std::ostream & out, spherical<type> const & buf, int prec );


#endif//__MATRIX_SLM_WRITE_HPP__
