#ifndef __PSEUDO_TASK_01_WRITE_HPP__
#define __PSEUDO_TASK_01_WRITE_HPP__
#include"pseudo.task.01.write.h"
#include<fstream>
#include<iomanip>

template<typename T> static void write_geom_r3( std::ostream & out, T const * r3, const int w = 25 )
{
	for(int i = 0; i < 3; ++i)
		out << std::setw( w ) << r3[i];
	out << std::endl;
}
template<typename T> void pseudo_task_01_write_geom( const char * file, _3_centers<T> const & geom )
{
	static const int prec = 15, w = prec + 10;
	std::ofstream out( file );
	out.setf( std::ios::scientific );
	out.precision( prec );

	write_geom_r3<T>( out, geom.A, w );
	write_geom_r3<T>( out, geom.B, w );
	write_geom_r3<T>( out, geom.C, w );

	out.close();
}

template<typename T> static void write_basis_set_lx( std::ostream & out, T const & alp_x, const int lx, const int w = 25 )
{
	out << std::setw(4) << 1 << std::setw(4) << 1 << std::endl;// 1 - number of alphas, 1 - number of functions
	out << std::setw(w) << alp_x << std::endl;
	out << std::setw(6) << 1 << std::setw(4) << 0 << std::endl;// 1 - number of primitives in function; 0 - starting index of alpha array
	out << std::setw(w) << T(1) << std::endl;
}
template<typename T> static void write_basis_set( std::ostream & out, T const & alp_x, const int lx_max, const int w = 25 )
{
	out << std::setw(4) << lx_max << std::endl;// maximum angular momentum of basis set
	for( int lx = 0; lx <= lx_max; ++lx )
	{
		write_basis_set_lx( out, alp_x, lx, w );
		out << std::endl;
	}
}
template<typename T> void pseudo_task_01_write_basis( const char * file, _3_alphas<T> const & alps, _3_momenta const & angs )
{
	static const int prec = 15, w = prec + 10;
	std::ofstream out( file );
	out.setf( std::ios::scientific );
	out.precision( prec );

	out << std::setw(4) << 2 << std::endl;// 2 - number of basis sets
	write_basis_set( out, alps.alp_a, angs.la, w );
	write_basis_set( out, alps.alp_b, angs.lb, w );

	out.close();
}
template<typename T> void pseudo_task_01_write_ecp( const char * file, _3_alphas<T> const & alps, _3_momenta const & angs )
{
	static const int prec = 15, w = prec + 10;
	std::ofstream out( file );
	out.setf( std::ios::scientific );
	out.precision( prec );

	static const int l_max = ( angs.l > 5 ? angs.l : 5 ), lso_max = 0;
	out << "ECP 46" << std::setw(4) << l_max << std::setw(4) << lso_max << std::endl;
	for(int l = 0; l <= l_max; ++l )
	{
		out << std::setw(3) << 1 << std::setw(3) << 1 << ' ' << std::setw(3) << angs.nk << ' ' <<
			std::setw( w ) << alps.alp_c << ' ' << std::setw(w) << T(1) << std::endl;
		// 1 - number of functions; 1 - number of primitives in func;
	}

	out.close();
}

#define __PSEUDO_TASK_01_WRITE_SPEC( type )\
template void pseudo_task_01_write_geom<type>( const char * file, _3_centers<type> const & geom );\
template void pseudo_task_01_write_basis<type>( const char * file, _3_alphas<type> const & alps, _3_momenta const & angs );\
template void pseudo_task_01_write_ecp<type>( const char * file, _3_alphas<type> const & alps, _3_momenta const & angs );

#endif//__PSEUDO_TASK_01_WRITE_HPP__
