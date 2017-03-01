#ifndef __QU_RADIAL_MAP_HPP__
#define __QU_RADIAL_MAP_HPP__
#include"qu.radial.map.h"
#include<cstring>// memcpy
#include<cstdlib>// exit

qu_radial_map::qu_radial_map(): mapping_struct(), _map1qu_dat_size(), _M_lmax(), _M_map3qu_pos(), _M_map1qu_dat(),
	__QU_RADIAL_MAP_ITER_INIT_LIST {}

qu_radial_map::qu_radial_map( qu_radial_map const & v): mapping_struct( v ), _map1qu_dat_size( v._map1qu_dat_size ),
	_M_lmax(v._M_lmax), _M_map3qu_pos( v._M_map3qu_pos ), _M_map1qu_dat( v._M_map1qu_dat ),
	__QU_RADIAL_MAP_COPY_ITER_INIT_LIST( v ) {}

qu_radial_map & qu_radial_map::operator=( qu_radial_map const & v)
{
	if( this == &v )
		return *this;
	qu_radial_map tmp( v );
	memcpy( this, &tmp, sizeof(*this) );// danger
	return *this;
}

void qu_radial_map::qu_reset_done_all()
{
	for(int i = 0; i < this->map1qu_size(); ++i)
	{
		this->map1qu_dat_set_i( i );
		this->qu_reset_done();
	}
}
const typename qu_radial_map::size_type qu_radial_map::comp_size( const _lmax_struct & __lmax )const
{
	if( this->is_mapping_min() )
		return 0;
	size_type __size = 0;
	__size += sizeof(_lmax_struct);
	// 3d matrix: n=0..la, m=0..lb, p=0..l
	__size += sizeof(size_struct<3>);
	__size += this->map3qu_pos_SIZE( __lmax ) * sizeof(map3qu_pos_type);// position of N,lmb_a,lmb_a matrix beginning and its' sizes
	// 1d matrix: elements of all N,lmb_a,lmb_b matrices in one big array (that is watching you)
	__size += sizeof( size_struct<1> );
	__size += this->map1qu_dat_SIZE( __lmax ) * sizeof(map1qu_dat_type);
	return __size;
}

const typename qu_radial_map::size_type qu_radial_map::map1qu_dat_SIZE( _lmax_struct const & __lmax )const
{
	switch( this->get_mapping() )
	{
	case minimum:
		//this->error( "map1qu_dat_SIZE( _lmax_struct const & )", "not need in minimum mapping type" );
		//std::cerr << "minimum mapping type : " << this->get_mapping() << std::endl;
		//exit(1);
		return 0;
	case middle :
		return this->map1qu_dat_SIZE_mid( __lmax );
	case maximum:
		return this->map1qu_dat_SIZE_max( __lmax );
	default :
		this->error( "map1qu_dat_SIZE( _lmax_struct const & )", "unknown mapping type" );
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}
const typename qu_radial_map::size_type qu_radial_map::map1qu_dat_SIZE_max( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	for(size_type la = 0; la <= __lmax._la_max; ++la)
	{
		for(size_type lb = 0, __dat_N = la + 1, __dat_SIZE; lb <= __lmax._lb_max; ++lb, ++__dat_N)
		{
			// semi-local part
			for(size_type l = 0, __dat_M = la + 1, __dat_P = lb + 1; l < __lmax._l_max; ++l, ++__dat_M, ++__dat_P)
			{
				__dat_SIZE = __dat_N;//  la + lb + 1
				__dat_SIZE *= __dat_M;// la + l  + 1
				__dat_SIZE *= __dat_P;// lb + l  + 1
				__size += __dat_SIZE;
			}
			// local part
			__dat_SIZE = __dat_N;
			__dat_SIZE *= __dat_N;
			__size += __dat_SIZE;// (la + lb + 1)^2
			// spin-orbit part
			for(size_type lso = 1, __dat_M = la + 2, __dat_P = lb + 2; lso <= __lmax._lso_max; ++lso, ++__dat_M, ++__dat_P)
			{
				__dat_SIZE = __dat_N;//  la + lb  + 1
				__dat_SIZE *= __dat_M;// la + lso + 1
				__dat_SIZE *= __dat_P;// lb + lso + 1
				__size += __dat_SIZE;
			}
		}
	}
	return __size;
}
const typename qu_radial_map::size_type qu_radial_map::map1qu_dat_SIZE_mid( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	for(size_type la = 0; la <= __lmax._la_max; ++la)
	{
		for(size_type lb = 0, __dat_N = la + 1, __dat_SIZE; lb <= __lmax._lb_max; ++lb, ++__dat_N)
		{
			// semi-local part
			for(size_type l = 0, __dat_M = la + 1; l < __lmax._l_max; ++l, ++__dat_M )
			{
				__dat_SIZE = __dat_N;//  la + lb + 1
				__dat_SIZE *= __dat_M;// la + l  + 1
				__size += __dat_SIZE;
			}
			// local part
			__dat_SIZE = __dat_N;
			__dat_SIZE *= __dat_N;
			__size += __dat_SIZE;// (la + lb + 1)^2
			// spin-orbit part
			for(size_type lso = 1, __dat_M = la + 2; lso <= __lmax._lso_max; ++lso, ++__dat_M )
			{
				__dat_SIZE = __dat_N;//  la + lb + 1
				__dat_SIZE *= __dat_M;// la + lso  + 1
				__size += __dat_SIZE;
			}
		}
	}
	return __size;
}

const typename qu_radial_map::size_type qu_radial_map::write_map( memorystream & ms, _lmax_struct const & __lmax )
{
	size_type _seek_start = ms.tell();

	this->_M_lmax = (_lmax_struct *)ms.getcur();// lmax
	*this->_M_lmax = __lmax;// put %_M_lmax
	ms.seek( sizeof(_lmax_struct), seek_dir::cur );

	union{
		void * _void;
		matrix_cursor_3<map3qu_pos_type> * _map3qu;
		matrix_cursor_1<map1qu_dat_type> * _map1qu;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map3qu_pos = __cnvrt._map3qu;
	this->_M_map3qu_pos->n() = this->map3qu_pos_N( __lmax );// (la_max + 1)
	this->_M_map3qu_pos->m() = this->map3qu_pos_M( __lmax );// (lb_max + 1)
	this->_M_map3qu_pos->p() = this->map3qu_pos_P( __lmax );// (l_max + lso_max + 1)
	this->_M_map3qu_pos->init_size();
	ms.seek( sizeof(size_struct<3>) + this->_M_map3qu_pos->size() * sizeof(map3qu_pos_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_map1qu_dat = __cnvrt._map1qu;
	this->map1qu_dat_size( __lmax );// run %map1qu_dat_SIZE( _lmax_struct const & ), and store size value into %_map1qu_dat_size variable
	this->_M_map1qu_dat->size() = this->map1qu_dat_size();// get size from %_map1qu_dat_size variable
	ms.seek( sizeof(size_struct<1>) + this->_M_map1qu_dat->size() * sizeof(map1qu_dat_type), seek_dir::cur );

	__assert__( this->comp_size(__lmax) == ms.tell() - _seek_start );
	return ms.tell() - _seek_start;
}

const typename qu_radial_map::size_type qu_radial_map::read_map( memorystream & ms )
{
	size_type _seek_start = ms.tell();

	this->_M_lmax = (_lmax_struct *)ms.getcur();
	ms.seek( sizeof(_lmax_struct), seek_dir::cur );

	union{
		void * _void;
		matrix_cursor_3<map3qu_pos_type> * _map3qu;
		matrix_cursor_1<map1qu_dat_type> * _map1qu;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map3qu_pos = __cnvrt._map3qu;
	ms.seek( sizeof(size_struct<3>) + this->_M_map3qu_pos->size() * sizeof(map3qu_pos_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_map1qu_dat = __cnvrt._map1qu;
	ms.seek( sizeof(size_struct<1>) + this->_M_map1qu_dat->size() * sizeof(map1qu_dat_type), seek_dir::cur );

	__assert__( this->comp_size( *this->_M_lmax ) == ms.tell() - _seek_start );
	return ms.tell() - _seek_start;
}

void qu_radial_map::init_map_max()
{
	size_type __map1qu_pos = 0;// positions of %_M_map1qu_dat array elements

	__assert__( this->is_mapping_max() );
	this->init_default_map1qu_dat();
	for(size_type la = 0; la <= this->M_get_la_max(); ++la)
	{
		this->map3qu_pos_set_la( la );
		for(size_type lb = 0, __dat_N = la + 1; lb <= this->M_get_lb_max(); ++lb, ++__dat_N)
		{
			this->map3qu_pos_set_lb( lb );
			// semi-local
			for(size_type l = 0, __dat_M = la + 1, __dat_P = lb + 1; l < this->M_get_l_max(); ++l, ++__dat_M, ++__dat_P)
			{
				this->map3qu_pos_set_l( l );

				this->map1qu_n() = __dat_N;
				this->map1qu_m() = __dat_M;
				this->map1qu_p() = __dat_P;

				this->map1qu_size() = (__dat_N * __dat_M * __dat_P);
				this->map1qu_pos() = __map1qu_pos;

				this->qu_size() = this->init_map1qu_dat_max_semilocal( la, lb, l );
				__map1qu_pos += this->map1qu_size();
			}
			// local
			this->map3qu_pos_set_lmax();

			this->map1qu_n() = __dat_N;
			this->map1qu_m() = __dat_N;
			this->map1qu_p() = 1;

			this->map1qu_size() = (__dat_N * __dat_N);
			this->map1qu_pos() = __map1qu_pos;

			this->qu_size() = this->init_map1qu_dat_max_local( la, lb );
			__map1qu_pos += this->map1qu_size();
			// spin-orbit
			for(size_type lso = 1, __dat_M = la + 2, __dat_P = lb + 2; lso <= this->M_get_lso_max(); ++lso, ++__dat_M, ++__dat_P)
			{
				this->map3qu_pos_set_l( lso + this->M_get_l_max() );

				this->map1qu_n() = __dat_N;
				this->map1qu_m() = __dat_M;
				this->map1qu_p() = __dat_P;

				this->map1qu_size() = (__dat_N * __dat_M * __dat_P);
				this->map1qu_pos() = __map1qu_pos;

				this->qu_size() = this->init_map1qu_dat_max_semilocal( la, lb, lso );// same as for semi-local and spin-orbit
				__map1qu_pos += this->map1qu_size();
			}
		}
	}
}

void qu_radial_map::init_map_mid()
{
	size_type __map1qu_pos = 0;// positions of %_M_map1qu_dat array elements

	__assert__( this->is_mapping_mid() );
	this->init_default_map1qu_dat();
	for(size_type la = 0; la <= this->M_get_la_max(); ++la)
	{
		this->map3qu_pos_set_la( la );
		for(size_type lb = 0, __dat_N = la + 1; lb <= this->M_get_lb_max(); ++lb, ++__dat_N)
		{
			this->map3qu_pos_set_lb( lb );
			// semi-local
			for(size_type l = 0, __dat_M = la + 1; l < this->M_get_l_max(); ++l, ++__dat_M )
			{
				this->map3qu_pos_set_l( l );

				this->map1qu_n() = __dat_N;// la + lb + 1
				this->map1qu_m() = __dat_M;// la + l  + 1
				this->map1qu_p() = 1;

				this->map1qu_size() = (__dat_N * __dat_M);
				this->map1qu_pos() = __map1qu_pos;

				this->qu_size() = this->init_map1qu_dat_mid_semilocal( la, lb, l );
				__map1qu_pos += this->map1qu_size();
			}
			// local
			this->map3qu_pos_set_lmax();

			this->map1qu_n() = __dat_N;
			this->map1qu_m() = __dat_N;
			this->map1qu_p() = 1;

			this->map1qu_size() = (__dat_N * __dat_N);
			this->map1qu_pos() = __map1qu_pos;

			this->qu_size() = this->init_map1qu_dat_mid_local( la, lb );
			__map1qu_pos += this->map1qu_size();
			// semi-local
			for(size_type lso = 1, __dat_M = la + 2; lso <= this->M_get_lso_max(); ++lso, ++__dat_M )
			{
				this->map3qu_pos_set_l( lso + this->M_get_l_max() );

				this->map1qu_n() = __dat_N;// la + lb  + 1
				this->map1qu_m() = __dat_M;// la + lso + 1
				this->map1qu_p() = 1;

				this->map1qu_size() = (__dat_N * __dat_M);
				this->map1qu_pos() = __map1qu_pos;

				this->qu_size() = this->init_map1qu_dat_mid_semilocal( la, lb, lso );// same as for semi-local and spin-orbit
				__map1qu_pos += this->map1qu_size();
			}
		}
	}
}

void qu_radial_map::init_default_map1qu_dat()
{
	__assert__( qu_radial_map::MAP1QU_DAT_INIT_NUM < 0 );
	map1qu_dat_type * __p_dat = this->M_map1qu_dat_data();
	for(int i = 0; i < this->M_map1qu_dat_size(); ++i, ++__p_dat )
	{
		__p_dat->_idx = qu_radial_map::MAP1QU_DAT_INIT_NUM;
		__p_dat->_n_times = 0;
	}
}

#ifdef  __QU_RADIAL_MAP_INIT_LOG
  #include<fstream>
  #include<iomanip>
inline static void open_file( std::ofstream & out, const qu_radial_map & qu_rad, const int la, const int lb, const int l )
{
	static const char file[] = "qu.radial.map.log";
	if( la == 0 && lb == 0 && l == 0 )
		out.open( file, std::ofstream::out );
	else
		out.open( file , std::ofstream::out | std::ofstream::app );
	for(int i = 0; i < 28; ++i) out << '-'; out << std::endl;
	out <<  std::setw(4) << "la" << std::setw(4) << "lb" << std::setw(4) << "l" <<
		std::setw(4) << "n" << std::setw(4) << "m" << std::setw(4) << "p" << std::setw(6) << "size" << std::endl <<
		std::setw(4) << la << std::setw(4) << lb << std::setw(4) << l << std::setw(4) <<
		qu_rad.map1qu_n() << std::setw(4) << qu_rad.map1qu_m() << std::setw(4) << qu_rad.map1qu_p() <<
		std::setw(6) << qu_rad.map1qu_size() << std::endl << std::endl;

	out <<  std::setw(4) << "na" << std::setw(4) << "nb" <<
		std::setw(4) << "N" << std::setw(4) << "l'a" << std::setw(4) << "l'b" <<
		std::setw(8) << "it" << std::setw(8) << "idx" <<
		" [" << qu_rad.map1qu_data() << "] [" << qu_rad.map1qu_data() + qu_rad.map1qu_size() << "]" <<
		std::endl << std::endl;
}
#endif

const typename qu_radial_map::size_type qu_radial_map::init_map1qu_dat_max_semilocal( const int la, const int lb, const int l)
{
#ifdef  __QU_RADIAL_MAP_INIT_LOG
	std::ofstream out;
	open_file( out, *this, la, lb, l );
#endif
	size_type __size = 0;
	// %__size - is number of unique triples {N,lmb_a,lmb_b} for loop below
	for(int na = 0; na <= la; ++na)
	{
		for(int nb = 0, N = na; nb <= lb; ++nb, ++N)// N = na + nb;
		{
			for(int lmb_a = (l < na ? (l + na)%2 : (l - na)); lmb_a <= (l + na); lmb_a += 2)
			{
				for(int lmb_b = (l < nb ? (l + nb)%2 : (l - nb)); lmb_b <= (l + nb); lmb_b += 2)
				{
					this->map1qu_dat_set( N, lmb_a, lmb_b );
					if( this->qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM )
						continue;
					this->qu_idx() = __size;
#ifdef  __QU_RADIAL_MAP_INIT_LOG
					int it = ( N * this->map1qu_m() + lmb_a ) * this->map1qu_p() + lmb_b;
					out <<  std::setw(4) << na << std::setw(4) << nb <<
						std::setw(4) << N  << std::setw(4) << lmb_a << std::setw(4) << lmb_b <<
						std::setw(8) << __size << std::setw(8) << it <<
						" [" << this->map1qu_data() + it << "]" <<
						std::endl;
#endif
					++__size;
				}
			}
		}
	}
	return __size;
}

const typename qu_radial_map::size_type qu_radial_map::init_map1qu_dat_max_local( const int la, const int lb )
{
#ifdef  __QU_RADIAL_MAP_INIT_LOG
	std::ofstream out;
	open_file( out, *this, la, lb, this->M_get_l_max() );
#endif
	size_type __size = 0;
	// %__size - is number of unique doubles {N,lmb} for loop below
	for(int N = 0; N <= la+lb; ++N)
	{
		for(int lmb = N%2; lmb <= N; lmb += 2)
		{
			this->map1qu_dat_set( N, lmb );
			if( this->qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM )
				continue;
			this->qu_idx() = __size;
#ifdef  __QU_RADIAL_MAP_INIT_LOG
			int it = N * this->map1qu_m() + lmb;
			out << std::setw(4) << '-' << std::setw(4) << '-' << std::setw(4) << N << std::setw(4) << lmb << std::setw(4) << '-' <<
				std::setw(8) << __size << std::setw(8) << it <<
				" [" << this->map1qu_data() + it << "]" <<
				std::endl;
#endif
			++__size;
		}
	}
	return __size;
}

const typename qu_radial_map::size_type qu_radial_map::init_map1qu_dat_mid_semilocal( const int la, const int lb, const int l)
{
#ifdef  __QU_RADIAL_MAP_MIDDLE_SPEC
	if( l%2 != lb%2 || l > lb )// zero condition for angular integral when middle mapping
		return 0;
#endif
#ifdef  __QU_RADIAL_MAP_INIT_LOG
	std::ofstream out;
	open_file( out, *this, la, lb, l );
#endif
	size_type __size = 0;
	// %__size - is number of unique doubles {N,lmb} for loop below
	for(int na = 0, N = lb; na <= la; ++na, ++N)// N = na + lb
	{
		for(int lmb_a = (l < na ? (l + na)%2 : (l - na)); lmb_a <= (l + na); lmb_a += 2)
		{
			this->map1qu_dat_set( N, lmb_a );
			if( this->qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM )
				continue;
			this->qu_idx() = __size;
#ifdef  __QU_RADIAL_MAP_INIT_LOG
			int it = N * this->map1qu_m() + lmb_a;
			out <<  std::setw(4) << na << std::setw(4) << '-' <<
				std::setw(4) << N  << std::setw(4) << lmb_a << std::setw(4) << '-' <<
				std::setw(8) << __size << std::setw(8) << it <<
				" [" << this->map1qu_data() + it << "]" <<
				std::endl;
#endif
			++__size;
		}
	}
	return __size;
}

const typename qu_radial_map::size_type qu_radial_map::init_map1qu_dat_mid_local( const int la, const int lb )
{
#ifdef  __QU_RADIAL_MAP_INIT_LOG
	std::ofstream out;
	open_file( out, *this, la, lb, this->M_get_l_max() );
#endif
	size_type __size = 0;
	// %__size - is number of unique doubles {N,lmb} for loop below
	for(int N = lb; N <= la+lb; ++N)
	{
		for(int lmb = N%2; lmb <= N; lmb += 2)
		{
			this->map1qu_dat_set( N, lmb );
			if( this->qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM )
				continue;
			this->qu_idx() = __size;
#ifdef  __QU_RADIAL_MAP_INIT_LOG
			int it = N * this->map1qu_m() + lmb;
			out << std::setw(4) << '-' << std::setw(4) << '-' << std::setw(4) << N << std::setw(4) << lmb << std::setw(4) << '-' <<
				std::setw(8) << __size << std::setw(8) << it <<
				" [" << this->map1qu_data() + it << "]" <<
				std::endl;
#endif
			++__size;
		}
	}
	return __size;
}

#endif//__QU_RADIAL_MAP_HPP__
