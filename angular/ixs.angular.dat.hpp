#ifndef __IXS_ANGULAR_DATA_HPP__
#define __IXS_ANGULAR_DATA_HPP__
#include"ixs.angular.dat.h"
#include"n.abc.h"// i2abc, abc2i
#include"sf.math.h"// convert_float
#include<iomanip>
#include"ixs.angular.idx.h"
#include"sf.math.h"

template<typename T, typename U> ixs_angular_dat<T,U>::ixs_angular_dat() : memory_map(), memorystream(), ixs_angular_map(), _M_mx1ang(){}
template<typename T, typename U> ixs_angular_dat<T,U>::ixs_angular_dat(ixs_angular_dat<T,U> const & v) : memory_map(), memorystream(v),
	ixs_angular_map( v ), _M_mx1ang(v._M_mx1ang){}

template<typename T, typename U> ixs_angular_dat<T,U> & ixs_angular_dat<T,U>::operator=(ixs_angular_dat<T,U> const & v)
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	ixs_angular_dat<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::memory_create( const char * file, int __flags, mode_type __mode)
{
	size_type __size = 0;

	__size += sizeof(int);
	__size += sizeof(size_struct<1>);
	__size += this->mx1ang_size() * sizeof(float_type);

	int _st = this->memory_map::create( file, __size, __flags, __mode );
	if( _st )
	{
		this->error("memory_create", "can't create map");
		std::cerr << "file '" << file << '\'' << std::endl;
		std::cerr << " map.size : " << std::setw(8) << this->size() << std::endl;
		std::cerr << " map.data : [" << std::setw(8) << this->data() << "]" << std::endl;
		std::cerr << "comp.size : " << std::setw(8) << __size << std::endl;
		exit(1);
	}
	this->sync_stream();
	this->write_dat();
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::write_dat()
{
	memorystream & ms = *this;
	int szof_T = sizeof(float_type);
	ms.write( &szof_T, sizeof(int) );

	union{
		void * _void;
		matrix_cursor_1<T> * _mx1ang;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_mx1ang = __cnvrt._mx1ang;
	this->_M_mx1ang->size() = this->mx1ang_size();
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1ang->size() * sizeof(float_type), seek_dir::cur );
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::memory_open( const char * file, int __flags, mode_type __mode)
{
	int _st = this->memory_map::open( file, __flags, __mode );
	if( _st )
	{
		this->error("memory_open", "can't create map");
		std::cerr << "file '" << file << '\'' << std::endl;
		std::cerr << "map.size : " << std::setw(8) << this->size() << std::endl;
		std::cerr << "map.data : [" << std::setw(8) << this->data() << "]" << std::endl;
		exit(1);
	}
	this->sync_stream();
	this->read_dat();
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::read_dat()
{
	memorystream & ms = *this;
	int szof_T = 0;
	ms.read( &szof_T, sizeof(int) );
	if( szof_T != sizeof(float_type) )
	{
		this->error("read_dat", "sizeof(this.float_type) != sizeof(file.float_type)");
		std::cerr << "this.float_type : " << sizeof(float_type) << std::endl;
		std::cerr << "file.float_type : " << szof_T << std::endl;
		exit(1);
	}

	union{
		void * _void;
		matrix_cursor_1<T> * _mx1ang;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_mx1ang = __cnvrt._mx1ang;
	if( this->mx1ang_size() != this->_M_mx1ang->size() )
	{
		this->error("read_dat", "sizes of array from 'ixs_ang_map' and 'file' are not equal");
		std::cerr << "file.mx1ang_size : " << this->_M_mx1ang->size() << std::endl;
		std::cerr << " map.mx1ang_size : " << this->mx1ang_size() << std::endl;
		exit(1);
	}
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1ang->size() * sizeof(float_type), seek_dir::cur );
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang( geom_slm<U> & geom_s, alpha_slm<T,U> & alp_s, qu_radial_dat<T,U> & qu_rad, ixs_omega<T,U> const & ixs_omg )
{
	switch( this->ixs_angular_map::get_mapping() )
	{
	case mapping_struct::minimum: return this->comp_ang_min( ixs_omg );
	case mapping_struct::middle : return this->comp_ang_mid( geom_s, qu_rad, ixs_omg );
	case mapping_struct::maximum: return this->comp_ang_max( geom_s, alp_s, qu_rad, ixs_omg );
	default:
		this->error( "comp_ang", "unknown mapping type" );
		std::cerr << "current mapping type : " << this->ixs_angular_map::get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_max( geom_slm<U> & geom_s, alpha_slm<T,U> & alp_s, qu_radial_dat<T,U> & qu_rad, ixs_omega<T,U> const & ixs_omg )
{
	if( !this->ixs_angular_map::is_mapping_max() )
	{
		this->error("comp_ang_max", "ixs_angular_map's mapping type isn't maximum");
		std::cerr << "ixs_ang_map.mapping_type : " << this->ixs_angular_map::get_mapping() << std::endl;
		exit(1);
	}
	T * __p_mx1ang_data = this->_M_mx1ang->data(), * __p_mx1ang;
	_lxyz_struct _lxyz;
	for(int la = 0, ila_size = 1; la <= this->ixs_angular_map::la_max(); ++la, ila_size += (la+1))
	{
		_lxyz.la = la;
		this->ixs_angular_map::map3node_set_la( la );
		alp_s.map2slm_set_la( la );// for local-max part
		alp_s.map1A_set_lx( la );  // for local-max part
		//alp_s.map2AB_set_la( la );
		qu_rad.qu_radial_map::qu_set_la( la );
		for(int i_la = 0; i_la < ila_size; ++i_la)// ax, ay, az
		{
			i2abc( la, ila_size, i_la, _lxyz.ax, _lxyz.ay);
			_lxyz.az = la - _lxyz.ax - _lxyz.ay;
			this->ixs_angular_map::map3node_set_ia( i_la );
			for(int lb = 0, ilb_size = 1; lb <= this->ixs_angular_map::lb_max(); ++lb, ilb_size += (lb+1))
			{
				_lxyz.lb = lb;
				this->ixs_angular_map::map3node_set_lb( lb );
				alp_s.map2slm_set_lb( lb );// local-max
				alp_s.map1B_set_lx( lb );// local-max
				//alp_s.map2AB_set_lb( lb );
				qu_rad.qu_radial_map::qu_set_lb( lb );
				for(int i_lb = 0; i_lb < ilb_size; ++i_lb )// bx, by, bz
				{
					i2abc( lb, ilb_size, i_lb, _lxyz.bx, _lxyz.by);
					_lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					this->ixs_angular_map::map3node_set_ib( i_lb );
					//to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					//to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					//if( !to_compute_value ) continue;
					// semi-local
					for(int l = 0; l < this->ixs_angular_map::l_max(); ++l )
					{
						_lxyz.l = l;
						this->ixs_angular_map::map3node_set_l( l );
						__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
						this->ixs_angular_map::map3nx2_set_l( l );
						this->ixs_angular_map::map2lmbA_set_l( l );
						this->ixs_angular_map::map2lmbB_set_l( l );

						qu_rad.qu_radial_map::qu_set_l( l );
						this->comp_ang_max_SemiLocal( __p_mx1ang, _lxyz, geom_s, qu_rad, ixs_omg );
					}
					// local
					this->ixs_angular_map::map3node_set_lmax();
					__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
					this->ixs_angular_map::map3nx2_set_lmax();

					qu_rad.qu_radial_map::qu_set_lmax();
					//this->comp_ang_max_Local( __p_mx1ang, _lxyz, geom_s, alp_s, ixs_omg );
					for(int ia = 0; ia < alp_s.map1A_size(); ++ia)
					{
						alp_s.map2slm_set_ia( ia );
						for(int ib = 0; ib < alp_s.map1B_size(); ++ib, __p_mx1ang += this->ixs_angular_map::map3node_size() )
						{
							alp_s.map2slm_set_ib( ib );
							this->comp_ang_max_Local( __p_mx1ang, _lxyz, geom_s, alp_s, qu_rad, ixs_omg );
						}
					}
					// TODO: spin-orbit part
				}
			}
		}
	}
}

#define __setw__( w, value ) std::setw( w ) << value
#define __IXS_ANGULAR_DAT_PRINT( _lxyz, _nx, angular_value, lmb_a, lmb_b, qu_rad )\
std::clog << __setw__(3, _lxyz.la) << __setw__(3, _lxyz.lb) << __setw__(3, _lxyz.l) <<\
__setw__(5, _nx.na) << __setw__(3, _nx.nb) << __setw__(3, _nx.na_p_nb) << __setw__(5, lmb_a) << __setw__(3, lmb_b) << ' ' <<\
__setw__(26, angular_value) << __setw__(4, qu_rad.qu_n()) << std::endl

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_max_Local(pointer __p_mx1ang, _lxyz_struct const & _lxyz,
		geom_slm<U> & geom_s, alpha_slm<T, U> & alp_s, qu_radial_dat<T,U> & qu_rad, ixs_omega<T,U> const & ixs_omg)
{
#ifdef  __IXS_ANGULAR_DATA_PRINT
	static const int IT_MAX = 1000;
	static int _it = 0;
#endif
	static U angular_value(0);
	T * __p_mx1ang_beg = __p_mx1ang;

	ixs_omega<T,U> ixs_omg_x( ixs_omg );
	ixs_omg_x.set_lmax();

	_nx_struct _nx;
	int __size = 0;
	ixs_angular_map & ixs_ang = *this;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		ixs_ang.map3nx2_set_na( na );
		_nx.na = na;
		for(int nb = 0, na_p_nb = na; nb <= _lxyz.lb; ++nb, ++na_p_nb )
		{
			qu_rad.qu_radial_map::qu_set_N( na_p_nb );
			ixs_ang.map3nx2_set_nb( nb );
			//__size += ixs_ang.map3nx2() * alp_s.map1A_size() * alp_s.map1B_size();// size += ([(na + nb)/2] + 1) * ...
			__size += ixs_ang.map3nx2();// size += ([(na + nb)/2] + 1) * ...
			_nx.nb = nb;
			_nx.na_p_nb = na_p_nb;
			ixs_omg_x.set_lx( na_p_nb );
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( ixs_ang.map3nx2() == (na + nb)/2 + 1 );
			__ixs_ang_assert__( alp_s.map2slm_size() >= (na_p_nb+1) *(na_p_nb+1) );// map2slm_size = (maximum ang.mom. + 1)^2
			__ixs_ang_assert__( ixs_omg_x.node1_n() == ixs_ang.map3nx2() );
#endif
			for(int i_lmb = 0, lmb = na_p_nb%2; i_lmb < ixs_ang.map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang)
			//for(int i_lmb = 0, lmb = na_p_nb%2; i_lmb < ixs_ang.map3nx2(); ++i_lmb, lmb += 2)
			{
				qu_rad.qu_radial_map::qu_set_lmb_a( lmb );
				qu_rad.qu_radial_map::qu_set_lmb_b( 0 );
				alp_s.map2slm_set_lmb( lmb );
				ixs_omg_x.set_lambda_i( i_lmb );
				_nx.lmbA = lmb;
				// la, (ax,ay,az), lb, (bx,by,bz), l, na, nb
				/*for(int ia = 0; ia < alp_s.map1A_size(); ++ia)
				{
					alp_s.map2slm_set_ia( ia );
					for(int ib = 0; ib < alp_s.map1B_size(); ++ib, ++__p_mx1ang )
					{
						alp_s.map2slm_set_ib( ib );
						this->comp_ang_max_Local( angular_value, _lxyz, _nx, geom_s, alp_s, ixs_omg_x );
						*__p_mx1ang = math::convert_float<T,U>( angular_value );
					}
				}
				*/
				this->comp_ang_max_Local_b( angular_value, _lxyz, _nx, geom_s, alp_s, ixs_omg_x );
				*__p_mx1ang = math::convert_float<T,U>( angular_value );
#ifdef  __IXS_ANGULAR_DATA_DEBUG
				__ixs_ang_assert__( qu_rad.qu_radial_map::qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM );
#endif
				if( angular_value != 0 )
				{
					qu_rad.qu_radial_map::qu_on();
				}
#if defined( __IXS_ANGULAR_DATA_PRINT ) && !defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
				if( (_it < IT_MAX) && to_compute_value )
					std::clog << std::setw(4) << qu_rad.qu_n() << std::endl;
				if( to_compute_value ) ++_it;
#endif
			}
		}
	}
#if defined (__IXS_ANGULAR_DATA_SIZECHECK)
	if( __p_mx1ang - __p_mx1ang_beg != __size )
	{
		this->error("comp_ang_max_Local", "size error");
		std::cerr << "computed size : " << std::setw(8) << __size << std::endl;
		std::cerr << "actual   size : " << std::setw(8) << __p_mx1ang - __p_mx1ang_beg << std::endl;
		exit(1);
	}
#endif
	return;
}

template<typename T, typename U> inline void print_ang_max_Local_mu_sum( const int _it, const int IT_MAX,
		const int mu_x, const int m,
		U const & slm_k, T const & omg_x, U const & tx )
{
	if( _it >= IT_MAX )
		return;
	std::clog << "mu = " << std::setw(2) << mu_x << ", m = " << std::setw(2) << m <<
		std::setw(25) << std::setprecision(15) << std::scientific << slm_k <<
		std::setw(25) << std::setprecision(15) << std::scientific << omg_x <<
		std::setw(25) << std::setprecision(15) << std::scientific << tx << std::endl;
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_max_Local_b(U & angular_value, _lxyz_struct const & _lxyz, _nx_struct const & _nx, 
		geom_slm<U> & geom_s, alpha_slm<T,U> & alp_s, ixs_omega<T,U> & ixs_omg_x )
{
	static U t1(0), t2_a(0), t3_a(0), t2_b(0), t3_b(0);

#if defined (__IXS_ANGULAR_DATA_DEBUG)
	static int it = 0, _a, _b, _c, _i_abc;
#endif
#if defined (__IXS_ANGULAR_DATA_PRINT)
	static int _it = 0, iter = 0, iter_all = 0;
	static const int IT_MAX = 1000;
#endif
	const typename ixs_omega<T,U>::value_struct * p_omg_x, * p_omg_xi;
	int x_size;
	angular_value = 0;
	for(int a = 0, ax_a = _lxyz.ax, na_a = _nx.na, it_a = 0; a <= _lxyz.ax && a <= _nx.na; ++a, --ax_a, --na_a)
	{
		t2_a = geom_s.CA_x( ax_a );
		t2_a *= math::cnk<u64_t>(_lxyz.ax, ax_a);
		for(int b = 0, ay_b = _lxyz.ay, c = na_a, az_c = _lxyz.az - na_a;
			b <= _lxyz.ay && b <= na_a;
			++b, --c, --ay_b, ++az_c, ++it_a)
		{
			if( c > _lxyz.az )
			{
				--it_a;
				continue;
			}
			t3_a = t2_a;
			t3_a *= geom_s.CA_y( ay_b );
			t3_a *= math::cnk<u64_t>(_lxyz.ay, ay_b);
			t3_a *= geom_s.CA_z( az_c );
			t3_a *= math::cnk<u64_t>(_lxyz.az, az_c);
			for(int d = 0, bx_d = _lxyz.bx, nb_d = _nx.nb, it_b = 0; d <= _lxyz.bx && d <= _nx.nb; ++d, --bx_d, --nb_d)
			{
				t2_b = t3_a;
				t2_b *= geom_s.CB_x( bx_d );
				t2_b *= math::cnk<u64_t>(_lxyz.bx, bx_d);
				for(int e = 0, f = nb_d, by_e = _lxyz.by, bz_f = _lxyz.bz - nb_d;
					e <= nb_d && e <= _lxyz.by;
					++e, --f, --by_e, ++bz_f, ++it_b)
				{
					if( f > _lxyz.bz )
					{
						--it_b;
						continue;
					}
					ixs_omg_x.set_abc_i( abc2i( _nx.na_p_nb, a+d, b+e ) );
					x_size = ixs_omg_x.get_omg_size();
					if( !x_size )
						continue;
#ifdef  __IXS_ANGULAR_DATA_DEBUG
					_i_abc = abc2i( _nx.na_p_nb, a+d, b+e );
					i2abc( _nx.na_p_nb, _i_abc, _a, _b );
					_c = _nx.na_p_nb - _a - _b;
#if  0//__IXS_ANGULAR_DATA_PRINT
					std::clog << std::setw(8) << it++ <<
						std::setw(4) << _nx.na_p_nb << " : " <<
						std::setw(3) << a+d << std::setw(3) << b + e << std::setw(3) << c+f << " : " <<
						std::setw(4) << _i_abc << " : " <<
						std::setw(3) << _a << std::setw(3) << _b << std::setw(3) << _c << " : " <<
						"[" << ixs_omg_x.M_node2_data() << "]" <<
						std::setw(8) << ixs_omg_x.node2_pos() << std::setw(8) << ixs_omg_x.node2_size() <<
						std::setw(4) << ixs_omg_x.get_omg_lmb() << " " << std::setw(4) << ixs_omg_x.get_omg_i_abc() <<
						" [0x" << std::hex << ixs_omg_x.get_omg_i_abc() << std::dec << "]" <<
						std::endl;
					if( ixs_omg_x.get_omg_i_abc() != _i_abc || ixs_omg_x.get_omg_lmb() != _nx.lmbA )
					{
						this->error("comp_ang_max_Local", "i_abc not equal");
						std::cerr << "i_abc : " << _i_abc << std::endl;
						std::cerr << "ixs_omg.i_abc : " << ixs_omg_x.get_omg_i_abc() << std::endl;
						std::cerr << "n : " << std::setw(4) << _nx.na_p_nb <<
							std::setw(4) << _nx.na << std::setw(4) << _nx.nb << std::endl;
						std::cerr << "a : " << std::setw(4) << a+d << std::setw(4) << a << std::setw(4) << d << std::endl;
						std::cerr << "b : " << std::setw(4) << b+e << std::setw(4) << b << std::setw(4) << e << std::endl;
						std::cerr << "c : " << std::setw(4) << c+f << std::setw(4) << c << std::setw(4) << f << std::endl;
						std::cerr << "i'n : " << abc2i( _nx.na_p_nb, a+d, b+e ) << std::endl;
						std::cerr << "n_size : " << ((_nx.na_p_nb+1)*(_nx.na_p_nb+2))/2 << std::endl;
					}
#endif
					__ixs_ang_assert__( ixs_omg_x.get_omg_data() != 0 );
					__ixs_ang_assert__( ixs_omg_x.get_omg_l() == 0 );
					__ixs_ang_assert__( ixs_omg_x.get_omg_lx() == _nx.na_p_nb );
					__ixs_ang_assert__( ixs_omg_x.get_omg_lmb() == _nx.lmbA );
					__ixs_ang_assert__( ixs_omg_x.get_omg_i_abc() == _i_abc );

					__ixs_ang_assert__( _i_abc < ((_nx.na_p_nb + 1) * (_nx.na_p_nb + 2))/2 );
					__ixs_ang_assert__( _i_abc >= 0 );
					__ixs_ang_assert__( ixs_omg_x.get_omg_data() !=  0 );
#endif//__IXS_ANGULAR_DATA_DEBUG
					p_omg_x = ixs_omg_x.get_omg_data();
#ifdef  __IXS_ANGULAR_DATA_DEBUG
					__ixs_ang_assert__( p_omg_x != 0 );
#endif

					t3_b = t2_b;
					t3_b *= geom_s.CB_y( by_e );
					t3_b *= math::cnk<u64_t>(_lxyz.by, by_e);
					t3_b *= geom_s.CB_z( bz_f );
					t3_b *= math::cnk<u64_t>(_lxyz.bz, bz_f);
					p_omg_xi = p_omg_x;
					t1 = 0;
					for(int it_ox = 0; p_omg_xi->m == 0 && it_ox < x_size; ++it_ox, ++p_omg_xi)
					{
						t1 += alp_s.mx2slm( p_omg_xi->mu ) * p_omg_xi->d;
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
						print_ang_max_Local_mu_sum( _it, IT_MAX, p_omg_xi->mu, p_omg_xi->m,\
								alp_s.mx2slm( p_omg_xi->mu ), p_omg_xi->d, t1 );
#endif
					}
					t1 *= t3_b;
					angular_value += t1;
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( _it < IT_MAX )
	{
		if( _it%50 == 0 )
			std::clog << std::setw(8) << "iter" <<
				std::setw(4) << "na" << std::setw(3) << "a" << std::setw(3) << "b" << std::setw(3) << "c" << 
				std::setw(4) << "nb" << std::setw(3) << "d" << std::setw(3) << "e" << std::setw(3) << "f" << 
				std::setw(4) << "N" << std::setw(4) << "lmb" <<
				std::setw(25) << "ang" <<
				std::setw(25) << "ca" <<
				std::setw(25) << "cb" <<
				std::setw(25) << "v" <<
				std::endl;
		std::clog << std::setw(8) << _it <<
			std::setw(4) << _nx.na << std::setw(3) << a << std::setw(3) << b << std::setw(3) << c << 
			std::setw(4) << _nx.nb << std::setw(3) << d << std::setw(3) << e << std::setw(3) << f << 
			std::setw(4) << _nx.na + _nx.nb << std::setw(4) << _nx.lmbA <<
			std::setw(25) << std::setprecision(15) << std::scientific << t1 / t3_b <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_b / t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << angular_value <<
			std::endl;
	}
	++_it;
#endif
				}
			}
		}
	}
#if defined( __IXS_ANGULAR_DATA_PRINT ) && !defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	++iter_all;
	if( angular_value != 0 )
		++iter;
#if 0
	if(
		_lxyz.la == this->ixs_angular_map::la_max() && 
		_lxyz.lb == this->ixs_angular_map::lb_max() &&
		_nx.na   == this->ixs_angular_map::la_max() &&
		_nx.nb   == this->ixs_angular_map::lb_max() &&
		_nx.lmbA == _nx.na + _nx.nb )
	{
		std::clog << "i : " << std::setw(8) << iter_all << std::setw(8) << iter;
		fprintf(stderr, "%14.2f\n", double(iter)/iter_all * 100);
	}
#endif
	static int lb = 0, bx = 0, by = 0;
	static const int IT_STEP = 10000;
	if( (_it < IT_MAX || _it%IT_STEP == 0) && to_compute_value )
	{
		if( (_it%50 == 0 && _it < IT_MAX) || ( _it%IT_STEP == 0 && (_it/IT_STEP)%50 == 0) )
			std::clog << std::setw(8) << "iter" <<
				std::setw(4) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" << std::setw(4) << "i" <<
				std::setw(5) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" << std::setw(4) << "j" <<
				std::setw(5) << "na" << std::setw(4) << "nb" << std::setw(4) <<"N"<< std::setw(5) << "lmb" <<
				std::setw(26) << "value" << std::endl;
		if( _it < IT_MAX )
		{
		if( lb != _lxyz.lb )
		{
			for(int _i = 0; _i < 67; ++_i) std::clog << '-';
			std::clog << std::endl;
			lb = _lxyz.lb;
			bx = _lxyz.bx;
			by = _lxyz.by;
		}
		else if( bx != _lxyz.bx || by != _lxyz.by)
		{
			std::clog << std::endl;
			bx = _lxyz.bx;
			by = _lxyz.by;
		}
		}
		std::clog << std::setw(8) << _it <<
			std::setw(4) << _lxyz.la <<
			std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
			std::setw(4) << abc2i(_lxyz.la, _lxyz.ax, _lxyz.ay ) <<
			std::setw(5) << _lxyz.lb <<
			std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
			std::setw(4) << abc2i(_lxyz.lb, _lxyz.bx, _lxyz.by ) <<
			std::setw(5) << _nx.na << std::setw(4) << _nx.nb <<
			std::setw(4) << _nx.na + _nx.nb <<
			std::setw(5) << _nx.lmbA <<
			std::setw(26) << std::setprecision(16) << std::scientific << angular_value;// << std::endl;// << std::flush;
		if( _it > IT_MAX ) std::clog << std::endl;
		//fprintf(stdout, "%20.12e\n", angular_value );
	}
	if( to_compute_value ) ++_it;
#endif//__IXS_ANGULAR_DATA_PRINT
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_max_SemiLocal(pointer __p_mx1ang, _lxyz_struct const & _lxyz, geom_slm<U> & geom_s, qu_radial_dat<T,U> & qu_rad,
		ixs_omega<T,U> const & ixs_omg)
{
#ifdef  __IXS_ANGULAR_DATA_PRINT
	static const int IT_MAX = 1000;
	static int _it = 0;
#endif
	static U angular_value(0);
	T * __p_mx1ang_beg = __p_mx1ang;

	ixs_omega<T,U> ixs_omg_a( ixs_omg ), ixs_omg_b( ixs_omg );
	ixs_omg_a.set_l( _lxyz.l );
	ixs_omg_b.set_l( _lxyz.l );

	_nx_struct _nx;
	int __size = 0;
	ixs_angular_map & ixs_ang = *this;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		ixs_ang.map3nx2_set_na( na );
		ixs_ang.map2lmbA_set_nx( na );
		_nx.na = na;
		ixs_omg_a.set_lx( na );
		for(int nb = 0; nb <= _lxyz.lb; ++nb )
		{
			qu_rad.qu_radial_map::qu_set_N( na + nb );
			ixs_ang.map3nx2_set_nb( nb );
			ixs_ang.map2lmbB_set_nx( nb );
			__size += ixs_ang.map3nx2();// size += lmb_a_size * lmb_b_size
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( ixs_ang.map3nx2() == ixs_ang.map2lmbA_size() * ixs_ang.map2lmbB_size() );
#endif
			_nx.nb = nb;
			ixs_omg_b.set_lx( nb );
			for(int i_lmb_a = 0, lmb_a = ixs_ang.map2lmbA_min(); i_lmb_a < ixs_ang.map2lmbA_size(); ++i_lmb_a, lmb_a += 2)
			{
				_nx.lmbA = lmb_a;
				qu_rad.qu_radial_map::qu_set_lmb_a( lmb_a );
				geom_s.slm_kA_set_lx( lmb_a );
				ixs_omg_a.set_lambda_i( i_lmb_a );
				for(int i_lmb_b = 0, lmb_b = ixs_ang.map2lmbB_min(); i_lmb_b < ixs_ang.map2lmbB_size(); ++i_lmb_b, lmb_b += 2,
						++__p_mx1ang)
				{
					_nx.lmbB = lmb_b;
					qu_rad.qu_radial_map::qu_set_lmb_b( lmb_b );
					geom_s.slm_kB_set_lx( lmb_b );
					ixs_omg_b.set_lambda_i( i_lmb_b );
					// la, (ax,ay,az), lb, (bx,by,bz), l, na, nb
					this->comp_ang_max_SemiLocal_b( angular_value, _lxyz, _nx, geom_s, ixs_omg_a, ixs_omg_b );
					*__p_mx1ang = math::convert_float<T,U>( angular_value );
#ifdef  __IXS_ANGULAR_DATA_DEBUG
					__ixs_ang_assert__( qu_rad.qu_radial_map::qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM );
#endif
					if( angular_value != 0 )
					{
						qu_rad.qu_radial_map::qu_on();
					}
#if defined( __IXS_ANGULAR_DATA_PRINT ) && !defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
					if( _it < IT_MAX )
						std::clog << __setw__( 4, qu_rad.qu_n() ) << std::endl;
					if( to_compute_value ) ++_it;
#endif
					//__IXS_ANGULAR_DAT_PRINT( _lxyz, _nx, angular_value, lmb_a, lmb_b, qu_rad );
				}
			}
		}
	}
#if defined (__IXS_ANGULAR_DATA_SIZECHECK)
	if( __p_mx1ang - __p_mx1ang_beg != __size )
	{
		this->error("comp_ang_max_SemiLocal", "size error");
		std::cerr << "computed size : " << std::setw(8) << __size << std::endl;
		std::cerr << "actual   size : " << std::setw(8) << __p_mx1ang - __p_mx1ang_beg << std::endl;
		exit(1);
	}
#endif
	return;
}

template<typename T> inline static const bool is_same_float( T const & lhs, T const & rhs )
{
	static const int eps_bits = 12;
	static const T eps_T = math::pown<T>( T(2), -eps_bits );
	T err = T(lhs);
	err -= rhs;
	err /= ( lhs != 0 ? lhs : (rhs != 0 ? rhs : T(1) ) );
	err = ( err < 0 ? -err : err );
	err *= eps_T;
	return err < std::numeric_limits<T>::epsilon();
}

template<typename T, typename U> inline void print_ang_max_SemiLocal_m_sum( const int _it, const int IT_MAX, 
		typename ixs_angular_dat<T,U>::_lxyz_struct const & _lxyz,
		typename ixs_angular_dat<T,U>::_nx_struct const & _nx, 
		const int a, const int b, const int c, const int d, const int e, const int f,
		const int m,
		U const & t1, U const & t2, U const & t3 )
{
	if( _it >= IT_MAX )
		return;
	if( _it%50 == 0 )
		std::clog << std::setw(6) << "iter" <<
			std::setw(3) << "na" << std::setw(2) << "a" << std::setw(2) << "b" << std::setw(2) << "c" << 
			std::setw(3) << "nb" << std::setw(2) << "d" << std::setw(2) << "e" << std::setw(2) << "f" << 
			std::setw(3) << "N" << std::setw(3) << "'a" << std::setw(3) << "'b" <<
			std::setw(3) << "l" <<
			std::setw(3) << "m" <<
			std::setw(25) << "ang_a" <<
			std::setw(25) << "ang_b" <<
			std::setw(25) << "t1" <<
			std::endl;
	std::clog << std::setw(6) << _it <<
		std::setw(3) << _nx.na << std::setw(2) << a << std::setw(2) << b << std::setw(2) << c << 
		std::setw(3) << _nx.nb << std::setw(2) << d << std::setw(2) << e << std::setw(2) << f << 
		std::setw(3) << _nx.na + _nx.nb << std::setw(3) << _nx.lmbA << std::setw(3) << _nx.lmbB <<
		std::setw(3) << _lxyz.l << std::setw(3) << m <<
		std::setw(25) << std::setprecision(15) << std::scientific << t2 <<
		std::setw(25) << std::setprecision(15) << std::scientific << t3 <<
		std::setw(25) << std::setprecision(15) << std::scientific << t1 <<
		std::endl;
}

template<typename T, typename U> inline void print_ang_max_SemiLocal_mu_sum( const int _it, const int IT_MAX,
		const char * mu_name, const int mu_x, const int m,
		U const & slm_kX, T const & omg_x, U const & tx )
{
	if( _it >= IT_MAX )
		return;
	std::clog << mu_name << " = " << std::setw(2) << mu_x << ", m = " << std::setw(2) << m <<
		std::setw(25) << std::setprecision(15) << std::scientific << slm_kX <<
		std::setw(25) << std::setprecision(15) << std::scientific << omg_x <<
		std::setw(25) << std::setprecision(15) << std::scientific << tx << std::endl;
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_max_SemiLocal_b(U & angular_value, _lxyz_struct const & _lxyz, _nx_struct const & _nx, 
		geom_slm<U> & geom_s, ixs_omega<T,U> & ixs_omg_a, ixs_omega<T,U> & ixs_omg_b )
{
	static U t1(0), t2(0), t3(0), t2_a(0), t3_a(0), t2_b(0), t3_b(0);

	const typename ixs_omega<T,U>::value_struct * p_omg_a, * p_omg_b, * p_omg_ai, * p_omg_bi;
	int a_size, b_size;
#if defined (__IXS_ANGULAR_DATA_PRINT)
	static int _it = 0;
	static const int IT_MAX = 1000;
	static unsigned long long int iter = 0, iter_all = 0;
#endif
	angular_value = 0;
	for(int a = 0, ax_a = _lxyz.ax, na_a = _nx.na, it_a = 0; a <= _lxyz.ax && a <= _nx.na; ++a, --ax_a, --na_a)
	{
		t2_a = geom_s.CA_x( ax_a );
		__ixs_ang_assert__( ax_a == _lxyz.ax - a );
		__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CA_x( 1 ), ax_a ), geom_s.CA_x( ax_a ) ) );
		t2_a *= math::cnk<u64_t>(_lxyz.ax, ax_a);
		for(
			int b = 0, ay_b = _lxyz.ay, c = na_a, az_c = _lxyz.az - na_a;
			//b <= _lxyz.ay && b <= na_a && c <= _lxyz.az;
			b <= _lxyz.ay && b <= na_a;
			++b, --c, --ay_b, ++az_c, ++it_a
		)
		{
			if( c > _lxyz.az )
			{
				--it_a;
				continue;
			}
			__ixs_ang_assert__( ay_b == _lxyz.ay - b );
			__ixs_ang_assert__( az_c == _lxyz.az - c );
			__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CA_y( 1 ), ay_b ), geom_s.CA_y( ay_b ) ) );
			__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CA_z( 1 ), az_c ), geom_s.CA_z( az_c ) ) );
			ixs_omg_a.set_abc_i( abc2i( _nx.na, a, b) );
			a_size = ixs_omg_a.get_omg_size();
			if( !a_size ) continue;
			p_omg_a = ixs_omg_a.get_omg_data();
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( ixs_omg_a.get_omg_data() != 0 );
			__ixs_ang_assert__( ixs_omg_a.get_omg_l() == _lxyz.l );
			__ixs_ang_assert__( ixs_omg_a.get_omg_lx() == _nx.na );
			__ixs_ang_assert__( ixs_omg_a.get_omg_lmb() == _nx.lmbA );
			__ixs_ang_assert__( ixs_omg_a.get_omg_i_abc() == abc2i(_nx.na, a, b) );
			if( ixs_omg_a.get_omg_i_abc() != abc2i(_nx.na, a, b) && 0 )
			{
				this->error("comp_ang_max_SemiLocal", "abc2i error");
				std::cerr << "ixs_omg_a.i_abc : " << ixs_omg_a.get_omg_i_abc() << std::endl;
				std::cerr << "i_abc : " << abc2i( _nx.na, a, b ) << std::endl;
				std::cerr << "n : " << _nx.na << std::endl;
				std::cerr << "x : " << a << std::endl;
				std::cerr << "y : " << b << std::endl;
				std::cerr << "z : " << c << std::endl;
				exit(1);
			}
#endif

			t3_a = t2_a;
			t3_a *= geom_s.CA_y( ay_b );
			t3_a *= math::cnk<u64_t>(_lxyz.ay, ay_b);
			t3_a *= geom_s.CA_z( az_c );
			t3_a *= math::cnk<u64_t>(_lxyz.az, az_c);
			for(int d = 0, bx_d = _lxyz.bx, nb_d = _nx.nb, it_b = 0; d <= _lxyz.bx && d <= _nx.nb; ++d, --bx_d, --nb_d)
			{
				t2_b = t3_a;
				t2_b *= geom_s.CB_x( bx_d );
				t2_b *= math::cnk<u64_t>(_lxyz.bx, bx_d);
				__ixs_ang_assert__( bx_d == _lxyz.bx - d );
				__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CB_x( 1 ), bx_d ), geom_s.CB_x( bx_d ) ) );
				for(
					int e = 0, f = nb_d, by_e = _lxyz.by, bz_f = _lxyz.bz - nb_d;
					e <= nb_d && e <= _lxyz.by;
					++e, --f, --by_e, ++bz_f, ++it_b)
				{
					if( f > _lxyz.bz )
					{
						--it_b;
						continue;
					}
					__ixs_ang_assert__( by_e == _lxyz.by - e );
					__ixs_ang_assert__( bz_f == _lxyz.bz - f );
					__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CB_y( 1 ), by_e ), geom_s.CB_y( by_e ) ) );
					__ixs_ang_assert__( is_same_float<U>( math::pown<U>( geom_s.CB_z( 1 ), bz_f ), geom_s.CB_z( bz_f ) ) );
					ixs_omg_b.set_abc_i( abc2i( _nx.nb, d, e ) );
					b_size = ixs_omg_b.get_omg_size();
					if( !b_size ) continue;
					p_omg_b = ixs_omg_b.get_omg_data();
#ifdef  __IXS_ANGULAR_DATA_DEBUG
					__ixs_ang_assert__( ixs_omg_b.get_omg_data() != 0 );
					__ixs_ang_assert__( ixs_omg_b.get_omg_l() == _lxyz.l );
					__ixs_ang_assert__( ixs_omg_b.get_omg_lx() == _nx.nb );
					__ixs_ang_assert__( ixs_omg_b.get_omg_lmb() == _nx.lmbB );
					__ixs_ang_assert__( ixs_omg_b.get_omg_i_abc() == abc2i(_nx.nb, d, e) );
#endif

					t3_b = t2_b;
					t3_b *= geom_s.CB_y( by_e );
					t3_b *= math::cnk<u64_t>(_lxyz.by, by_e);
					t3_b *= geom_s.CB_z( bz_f );
					t3_b *= math::cnk<u64_t>(_lxyz.bz, bz_f);
					p_omg_ai = p_omg_a;
					p_omg_bi = p_omg_b;
					if( _lxyz.l >= this->ixs_angular_map::l_max() || _lxyz.l < 0 || this->ixs_angular_map::l_max() < 0)
					{
						this->error("comp_ang_max_SemiLocal_b", "last node, l negative ?");
						std::cerr << "l : " << _lxyz.l << std::endl;
						std::cerr << "l_max : " << this->ixs_angular_map::l_max() << std::endl;
						exit(1);
					}
					t1 = 0;
					for(int m = -_lxyz.l, it_oa = 0, it_ob = 0; m <= _lxyz.l; m++)
					{
						t2 = 0;
						for(; p_omg_ai->m == m && it_oa < a_size; ++it_oa, ++p_omg_ai)
						{
							t2 += geom_s.slm_kA( p_omg_ai->mu ) * p_omg_ai->d;
							//print_ang_max_SemiLocal_mu_sum( _it, IT_MAX, "mu_a", p_omg_ai->mu, p_omg_ai->m,\
									geom_s.slm_kA( p_omg_ai->mu ), p_omg_ai->d, t2 );
						}
						t3 = 0;
						for(; p_omg_bi->m == m && it_ob < b_size; ++it_ob, ++p_omg_bi)
						{
							t3 += geom_s.slm_kB( p_omg_bi->mu ) * p_omg_bi->d;
							//print_ang_max_SemiLocal_mu_sum( _it, IT_MAX, "mu_b", p_omg_bi->mu, p_omg_bi->m,\
									geom_s.slm_kB( p_omg_bi->mu ), p_omg_bi->d, t3 );
						}
						t1 += t2 * t3;
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
						print_ang_max_SemiLocal_m_sum<T,U>( _it, IT_MAX, _lxyz, _nx, a, b, c, d, e, f, m, t1, t2, t3 );
						++_it;
#endif
					}
					t1 *= t3_b;
					angular_value += t1;
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( (_it-1) < IT_MAX )
	{
		if( angular_value == 0 )
		std::clog << std::setw(6) << _it <<
			std::setw(3) << _nx.na << std::setw(2) << a << std::setw(2) << b << std::setw(2) << c << 
			std::setw(3) << _nx.nb << std::setw(2) << d << std::setw(2) << e << std::setw(2) << f << 
			std::setw(3) << _nx.na + _nx.nb << std::setw(3) << _nx.lmbA << std::setw(3) << _nx.lmbB <<
			std::setw(3) << _lxyz.l << std::endl;
		std::clog << std::setw(14) << "" <<
			std::setw(25) << std::setprecision(15) << std::scientific << angular_value <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_b / t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << t1 / t3_b <<
			std::endl;
	}
#endif
#if 0
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( _it < IT_MAX )
	{
		if( _it%50 == 0 )
			std::clog << std::setw(8) << "iter" <<
				std::setw(4) << "na" << std::setw(3) << "a" << std::setw(3) << "b" << std::setw(3) << "c" << 
				std::setw(4) << "nb" << std::setw(3) << "d" << std::setw(3) << "e" << std::setw(3) << "f" << 
				std::setw(4) << "N" << std::setw(4) << "la'" << std::setw(4) << "lb'" <<
				std::setw(4) << "l" <<
				std::setw(25) << "ang" <<
				std::setw(25) << "ca" <<
				std::setw(25) << "cb" <<
				std::setw(25) << "v" <<
				std::endl;
		std::clog << std::setw(8) << _it <<
			std::setw(4) << _nx.na << std::setw(3) << a << std::setw(3) << b << std::setw(3) << c << 
			std::setw(4) << _nx.nb << std::setw(3) << d << std::setw(3) << e << std::setw(3) << f << 
			std::setw(4) << _nx.na + _nx.nb << std::setw(4) << _nx.lmbA << std::setw(4) << _nx.lmbB <<
			std::setw(4) << _lxyz.l <<
			std::setw(25) << std::setprecision(15) << std::scientific << t1 / t3_b <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_b / t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << angular_value <<
			std::endl;
	}
	++_it;
#endif
#endif
				}
			}
		}
	}
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( (_it-1) < IT_MAX )
		std::clog << std::setw(14) << "" <<
			std::setw(25) << std::setprecision(15) << std::scientific << angular_value << std::endl;
	//std::clog << std::setw(25) << std::setprecision(15) << std::scientific << angular_value << std::endl;
#endif
#if defined( __IXS_ANGULAR_DATA_PRINT ) && !defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	++iter_all;
	if( angular_value != 0 )
		++iter;
#if 0
	if(
		_lxyz.l  == this->ixs_angular_map::l_max()-1 &&
		_lxyz.la == this->ixs_angular_map::la_max() &&
		_lxyz.lb == this->ixs_angular_map::lb_max() &&
		_nx.na   == this->ixs_angular_map::la_max() &&
		_nx.nb   == this->ixs_angular_map::lb_max() &&
		_nx.lmbA == _nx.na + _lxyz.l &&
		_nx.lmbB == _nx.nb + _lxyz.l )
	{
		std::clog << "i : " << std::setw(8) << iter_all << std::setw(8) << iter;
		fprintf(stderr, "%14.2f\n", double(iter)/iter_all * 100);
	}
#endif
	static int lb = 0, bx = 0, by = 0;
	static const int IT_STEP = 10000;
	if( (_it < IT_MAX || _it%IT_STEP == 0) && to_compute_value )
	{
		if( (_it%50 == 0 && _it < IT_MAX) || ( _it%IT_STEP == 0 && (_it/IT_STEP)%50 == 0) )
			std::clog << std::setw(8) << "iter" <<
				std::setw(4) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" << std::setw(4) << "i" <<
				std::setw(5) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" << std::setw(4) << "j" <<
				std::setw(5) << "l" <<
				std::setw(5) << "na" << std::setw(4) << "nb" << std::setw(4) <<"N"<< std::setw(5) << "la'" << std::setw(4) << "lb'" <<
				std::setw(26) << "value" << std::endl;
		if( _it < IT_MAX )
		{
		if( lb != _lxyz.lb )
		{
			for(int _i = 0; _i < 72; ++_i) std::clog << '-';
			std::clog << std::endl;
			lb = _lxyz.lb;
			bx = _lxyz.bx;
			by = _lxyz.by;
		}
		else if( bx != _lxyz.bx || by != _lxyz.by)
		{
			std::clog << std::endl;
			bx = _lxyz.bx;
			by = _lxyz.by;
		}
		}
		std::clog << std::setw(8) << _it <<
			std::setw(4) << _lxyz.la <<
			std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
			std::setw(4) << abc2i(_lxyz.la, _lxyz.ax, _lxyz.ay ) <<
			std::setw(5) << _lxyz.lb <<
			std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
			std::setw(4) << abc2i(_lxyz.lb, _lxyz.bx, _lxyz.by ) <<
			std::setw(5) << _lxyz.l << std::setw(5) << _nx.na << std::setw(4) << _nx.nb <<
			std::setw(4) << _nx.na + _nx.nb <<
			std::setw(5) << _nx.lmbA << std::setw(4) << _nx.lmbB <<
			std::setw(26) << std::setprecision(16) << std::scientific << angular_value;// << std::endl;// << std::flush;
		if( _it > IT_MAX ) std::clog << std::endl;
		//fprintf(stdout, "%20.12e\n", angular_value );
	}
	if( to_compute_value ) ++_it;
#endif
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_mid( geom_slm<U> & geom_s, qu_radial_map & qu_rad, ixs_omega<T,U> const & ixs_omg )
{
	if( !this->ixs_angular_map::is_mapping_mid() )
	{
		this->error("comp_ang_mid", "ixs_angular_map's mapping type isn't middle");
		std::cerr << "ixs_ang_map.mapping_type : " << this->ixs_angular_map::get_mapping() << std::endl;
		exit(1);
	}
	T * __p_mx1ang_data = this->_M_mx1ang->data(), * __p_mx1ang;
	_lxyz_struct _lxyz;
	for(int la = 0, ila_size = 1; la <= this->ixs_angular_map::la_max(); ++la, ila_size += (la+1))
	{
		_lxyz.la = la;
		this->ixs_angular_map::map3node_set_la( la );
		qu_rad.qu_radial_map::qu_set_la( la );
		for(int i_la = 0; i_la < ila_size; ++i_la)// ax, ay, az
		{
			i2abc( la, ila_size, i_la, _lxyz.ax, _lxyz.ay);
			_lxyz.az = la - _lxyz.ax - _lxyz.ay;
			this->ixs_angular_map::map3node_set_ia( i_la );
			for(int lb = 0, ilb_size = 1; lb <= this->ixs_angular_map::lb_max(); ++lb, ilb_size += (lb+1))
			{
				_lxyz.lb = lb;
				this->ixs_angular_map::map3node_set_lb( lb );
				qu_rad.qu_radial_map::qu_set_lb( lb );
				for(int i_lb = 0; i_lb < ilb_size; ++i_lb )// bx, by, bz
				{
					i2abc( lb, ilb_size, i_lb, _lxyz.bx, _lxyz.by);
					_lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					this->ixs_angular_map::map3node_set_ib( i_lb );
					to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					// semi-local
					// when lmb_min = 0 ? -> if l<=lb && l%2 == lb%2
					for(int l = lb%2; l < this->ixs_angular_map::l_max() && l <= lb; l += 2 )
					{
						this->ixs_angular_map::map3node_set_l( l );
						__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
						this->ixs_angular_map::map3nx2_set_l( l );
						this->ixs_angular_map::map2lmbA_set_l( l );
						_lxyz.l = l;
						qu_rad.qu_radial_map::qu_set_l( l );
						this->comp_ang_mid_SemiLocal( __p_mx1ang, _lxyz, geom_s, qu_rad, ixs_omg );
					}
					// local
					this->ixs_angular_map::map3node_set_lmax();
					__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
					this->ixs_angular_map::map3nx2_set_lmax();
					qu_rad.qu_radial_map::qu_set_lmax();
					this->comp_ang_mid_Local( __p_mx1ang, _lxyz, geom_s, qu_rad, ixs_omg );
					// TODO: spin-orbit part
				}
			}
		}
	}
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_mid_Local(pointer __p_mx1ang, _lxyz_struct const & _lxyz, geom_slm<U> & geom_s, qu_radial_map & qu_rad,
		ixs_omega<T,U> const & ixs_omg)
{
	static U angular_value(0);
	T * __p_mx1ang_beg = __p_mx1ang;

	ixs_omega<T,U> ixs_omg_x( ixs_omg );
	ixs_omg_x.set_lmax();

	_nx_struct _nx;
	ixs_angular_map & ixs_ang = *this;
	int __size = 0;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		ixs_ang.map3nx2_set_na( na );
		_nx.na = na;
		_nx.na_p_nb = _lxyz.lb + na;
		ixs_ang.map3nx2_set_nb( _lxyz.lb );
		__size += ixs_ang.map3nx2();// size += [(na + nb)/2] + 1;
#ifdef  __IXS_ANGULAR_DATA_DEBUG
		__ixs_ang_assert__( ixs_ang.map3nx2() == (_nx.na_p_nb)/2 + 1 );
#endif
		ixs_omg_x.set_lx( _nx.na_p_nb );
		qu_rad.qu_radial_map::qu_set_N( _nx.na_p_nb );
		for(int i_lmb = 0, lmb = _nx.na_p_nb%2; i_lmb < ixs_ang.map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang)
		{
			_nx.lmbA = lmb;
			geom_s.slm_kA_set_lx( lmb );
			ixs_omg_x.set_lambda_i( i_lmb );
			qu_rad.qu_radial_map::qu_set_lmb_a( lmb );
			qu_rad.qu_radial_map::qu_set_lmb_b( 0 );
			// la, (ax,ay,az), lb, (bx,by,bz), l, na, nb
			this->comp_ang_mid_Local_b( angular_value, _lxyz, _nx, geom_s, ixs_omg_x );
			*__p_mx1ang = math::convert_float<T,U>( angular_value );
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( qu_rad.qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM );
#endif
			if( angular_value != 0 )
			{
				qu_rad.qu_on();
			}
		}
	}
#if defined (__IXS_ANGULAR_DATA_SIZECHECK)
	if( __p_mx1ang - __p_mx1ang_beg != __size )
	{
		this->error("comp_ang_mid_Local", "size error");
		std::cerr << "computed size : " << std::setw(8) << __size << std::endl;
		std::cerr << "actual   size : " << std::setw(8) << __p_mx1ang - __p_mx1ang_beg << std::endl;
		exit(1);
	}
#endif
	return;
}

template<typename T, typename U> inline void 
print_ang_mid_Local_sum( const int _it, const int IT_MAX,
		typename ixs_angular_dat<T,U>::_lxyz_struct const & _lxyz, typename ixs_angular_dat<T,U>::_nx_struct const & _nx,
		const int a, const int b, const int c, U const & t1, U const &t3_a, U const & angular_value)
{
	if( _it >= IT_MAX ) return;
	if( _it == 0 ) std::clog << std::endl;
	if( _it%50 == 0 )
		std::clog << std::setw(4) << "la" << std::setw(4) << "lb" << std::setw(4) << "l" <<
			std::setw(4) << "N"  << std::setw(4) << "na" << std::setw(4) << "lmb" <<
			std::setw(4) << "a" << std::setw(3) << "b" << std::setw(4) << "c" <<
			std::setw(25) << "t1" << std::setw(25) << "ca" << std::setw(25) << "ang" << std::endl;
	std::clog << std::setw(4) << _lxyz.la << std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.l <<
		std::setw(4) << _nx.na_p_nb  << std::setw(4) << _nx.na << std::setw(4) << _nx.lmbA <<
		std::setw(4) << a << std::setw(3) << b << std::setw(4) << c <<
		std::setw(25) << std::setprecision(15) << std::scientific << t1 <<
		std::setw(25) << std::setprecision(15) << std::scientific << t3_a <<
		std::setw(25) << std::setprecision(15) << std::scientific << angular_value <<
		std::endl;
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_mid_Local_b(U & angular_value, _lxyz_struct const & _lxyz, _nx_struct const & _nx, 
		geom_slm<U> & geom_s, ixs_omega<T,U> & ixs_omg_x )
{
	static U t1(0), t2_a(0), t3_a(0);

	const typename ixs_omega<T,U>::value_struct * p_omg_x;
	int x_size;
#if defined( __IXS_ANGULAR_DATA_PRINT )
	static int _it = 0;
	static const int IT_MAX = 1000;
	U tmp_t1;
#endif
	angular_value = 0;
	for(int a = 0, ax_a = _lxyz.ax, na_a = _nx.na, it_a = 0; a <= _lxyz.ax && a <= _nx.na; ++a, --ax_a, --na_a)
	{
		t2_a = geom_s.CA_x( ax_a );
		t2_a *= math::cnk<u64_t>(_lxyz.ax, ax_a);
		for(int b = 0, ay_b = _lxyz.ay, c = na_a, az_c = _lxyz.az - na_a; b <= _lxyz.ay && b <= na_a && c <= _lxyz.az;
				++b, --c, --ay_b, ++az_c, ++it_a)
		{
			ixs_omg_x.set_abc_i( abc2i( _nx.na_p_nb, a+_lxyz.bx, b+_lxyz.by ) );
			x_size = ixs_omg_x.get_omg_size();
			if( !x_size )
				continue;
			p_omg_x = ixs_omg_x.get_omg_data();
			t3_a = t2_a;
			t3_a *= geom_s.CA_y( ay_b );
			t3_a *= math::cnk<u64_t>(_lxyz.ay, ay_b);
			t3_a *= geom_s.CA_z( az_c );
			t3_a *= math::cnk<u64_t>(_lxyz.az, az_c);

			t1 = 0;
			for(int it_ox = 0; it_ox < x_size; ++it_ox, ++p_omg_x)
			{
				t1 += geom_s.slm_kA( p_omg_x->mu ) * p_omg_x->d;
#if defined( __IXS_ANGULAR_DATA_PRINT )
				if( to_compute_value )
				print_ang_max_SemiLocal_mu_sum( _it, IT_MAX, "mu", p_omg_x->mu, p_omg_x->m,
						geom_s.slm_kA( p_omg_x->mu ), p_omg_x->d, t1 );
				tmp_t1 = t1;
#endif
#ifdef  __IXS_ANGULAR_DATA_DEBUG
				__ixs_ang_assert__( p_omg_x->m == 0 );
				__ixs_ang_assert__( abs(p_omg_x->mu) <= _nx.lmbA );
#endif
			}
			t1 *= t3_a;
			angular_value += t1;
#if defined( __IXS_ANGULAR_DATA_PRINT )
			if( to_compute_value )
			{
			print_ang_mid_Local_sum<T,U>( _it, IT_MAX, _lxyz, _nx, a, b, c, tmp_t1, t3_a, angular_value );
			++_it;
			}
#endif
		}
	}
#if defined( __IXS_ANGULAR_DATA_PRINT )
	if( to_compute_value )
	if( _it < IT_MAX )
	{
		//if( angular_value == 0 )
		//	print_ang_mid_Local_sum<T,U>( _it, IT_MAX, _lxyz, _nx, -1, -1, -1, t1, t3_a, angular_value );
		std::clog << "-----------------------" << std::endl;
		std::clog << std::setw(25) << std::setprecision(15) << std::scientific << angular_value << std::endl;
		std::clog << "-----------------------" << std::endl;
	}
#endif
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_mid_SemiLocal(pointer __p_mx1ang, _lxyz_struct const & _lxyz, geom_slm<U> & geom_s, qu_radial_map & qu_rad, 
		ixs_omega<T,U> const & ixs_omg)
{
	static U angular_value(0);
	int __size = 0;
	T * __p_mx1ang_beg = __p_mx1ang;

	ixs_omega<T,U> ixs_omg_a( ixs_omg ), ixs_omg_b( ixs_omg );

	ixs_omg_b.set_l( _lxyz.l );
	ixs_omg_b.set_lx( _lxyz.lb );
	ixs_omg_b.set_lambda_i( 0 );
	ixs_omg_b.set_abc_i( abc2i( _lxyz.lb, _lxyz.bx, _lxyz.by ) );

	ixs_angular_map & ixs_ang = *this;
	if( ixs_omg_b.get_omg_size() == 0 )
	{
		for(int i = 0; i < ixs_ang.map3node_size(); ++i, ++__p_mx1ang )
			*__p_mx1ang = 0;
		__size = ixs_ang.map3node_size();
	}
	else
	{
	ixs_omg_a.set_l( _lxyz.l );
	_nx_struct _nx;
	_nx.nb = _lxyz.lb;
	_nx.lmbB = 0;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		_nx.na = na;
		_nx.na_p_nb = _nx.na + _lxyz.lb;

		ixs_ang.map2lmbA_set_nx( na );

		ixs_ang.map3nx2_set_na( na );
		ixs_ang.map3nx2_set_nb( 0 );
		ixs_omg_a.set_lx( na );

		__size += ixs_ang.map3nx2();// size += lmb_a_size * lmb_b_size
#ifdef  __IXS_ANGULAR_DATA_DEBUG
		__ixs_ang_assert__( ixs_ang.map3nx2() == ixs_ang.map2lmbA_size() );
#endif
		qu_rad.qu_radial_map::qu_set_N( _nx.na_p_nb );
		for(int i_lmb_a = 0, lmb_a = ixs_ang.map2lmbA_min(); i_lmb_a < ixs_ang.map2lmbA_size(); ++i_lmb_a, lmb_a += 2, ++__p_mx1ang)
		{
			geom_s.slm_kA_set_lx( lmb_a );
			ixs_omg_a.set_lambda_i( i_lmb_a );
			qu_rad.qu_radial_map::qu_set_lmb_a( lmb_a );
			qu_rad.qu_radial_map::qu_set_lmb_b( 0 );
			_nx.lmbA = lmb_a;
			// la, (ax,ay,az), lb, (bx,by,bz), l, na, nb
			if( false )//ixs_omg_b.get_omg_size() == 0 )
			{
				*__p_mx1ang = 0;
				continue;
			}
			this->comp_ang_mid_SemiLocal_b( angular_value, _lxyz, _nx, geom_s, ixs_omg_a, ixs_omg_b );
			*__p_mx1ang = math::convert_float<T,U>( angular_value );
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( qu_rad.qu_idx() != qu_radial_map::MAP1QU_DAT_INIT_NUM );
#endif
			if( angular_value != 0 )
			{
				qu_rad.qu_on();
			}
		}
	}
	}
#if defined (__IXS_ANGULAR_DATA_SIZECHECK)
	if( __p_mx1ang - __p_mx1ang_beg != __size )
	{
		this->error("comp_ang_mid_SemiLocal", "size error");
		std::cerr << "computed size : " << std::setw(8) << __size << std::endl;
		std::cerr << "actual   size : " << std::setw(8) << __p_mx1ang - __p_mx1ang_beg << std::endl;
		std::cerr << "la   : " << std::setw(4) << _lxyz.la << std::endl;
		std::cerr << "lb   : " << std::setw(4) << _lxyz.lb << std::endl;
		std::cerr << "a    : " << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az << std::endl;
		std::cerr << "b    : " << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz << std::endl;
		std::cerr << "map3node_size : " << std::setw(8) << ixs_ang.map3node_size() << std::endl;
		std::cerr << "ixs_omg_bsize : " << std::setw(8) << ixs_omg_b.get_omg_size() << std::endl;
		exit(1);
	}
#endif
	return;
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_mid_SemiLocal_b(U & angular_value, _lxyz_struct const & _lxyz, _nx_struct const & _nx, 
		geom_slm<U> & geom_s, ixs_omega<T,U> & ixs_omg_a, ixs_omega<T,U> & ixs_omg_b )
{
	static U t1(0), t2(0), t3(0), t2_a(0), t3_a(0);

	const typename ixs_omega<T,U>::value_struct * p_omg_a, * p_omg_b, * p_omg_ai, * p_omg_bi;
	p_omg_b = ixs_omg_b.get_omg_data();
	int a_size, b_size = ixs_omg_b.get_omg_size();

#if defined( __IXS_ANGULAR_DATA_PRINT )
	static int _it = 0;
	static const int IT_MAX = 1000;
#endif
	angular_value = 0;
	for(int a = 0, ax_a = _lxyz.ax, na_a = _nx.na, it_a = 0; a <= _lxyz.ax && a <= _nx.na; ++a, --ax_a, --na_a)
	{
		t2_a = geom_s.CA_x( ax_a );
		t2_a *= math::cnk<u64_t>(_lxyz.ax, ax_a);
		for(int b = 0, ay_b = _lxyz.ay, c = na_a, az_c = _lxyz.az - na_a; b <= _lxyz.ay && b <= na_a && c <= _lxyz.az;
				++b, --c, --ay_b, ++az_c, ++it_a)
		{
			ixs_omg_a.set_abc_i( abc2i( _nx.na, a, b) );
			a_size = ixs_omg_a.get_omg_size();
			if( !a_size )
				continue;
			p_omg_a = ixs_omg_a.get_omg_data();

			t3_a = t2_a;
			t3_a *= geom_s.CA_y( ay_b );
			t3_a *= math::cnk<u64_t>(_lxyz.ay, ay_b);
			t3_a *= geom_s.CA_z( az_c );
			t3_a *= math::cnk<u64_t>(_lxyz.az, az_c);

			p_omg_ai = p_omg_a;
			p_omg_bi = p_omg_b;
			if( _lxyz.l >= this->ixs_angular_map::l_max() || _lxyz.l < 0 || this->ixs_angular_map::l_max() < 0)
			{
				this->error("comp_ang_mid_SemiLocal_b", "last node, l negative ?");
				std::cerr << "l : " << _lxyz.l << std::endl;
				std::cerr << "l_max : " << this->ixs_angular_map::l_max() << std::endl;
				exit(1);
			}
			t1 = 0;
			for(int m = -_lxyz.l, it_oa = 0, it_ob = 0; m <= _lxyz.l; ++m)
			{
				t2 = 0;
				for(; p_omg_ai->m == m && it_oa < a_size; ++it_oa, ++p_omg_ai)
				{
					t2 += geom_s.slm_kA( p_omg_ai->mu ) * p_omg_ai->d;
#if defined( __IXS_ANGULAR_DATA_PRINT )
					if( to_compute_value )
					print_ang_max_SemiLocal_mu_sum( _it, IT_MAX, "mu_a", p_omg_ai->mu, p_omg_ai->m,
						geom_s.slm_kA( p_omg_ai->mu ), p_omg_ai->d, t2 );
#endif
				}
				t3 = 0;
				for(; p_omg_bi->m == m && it_ob < b_size; ++it_ob, ++p_omg_bi)
				{
#ifdef  __IXS_ANGULAR_DATA_DEBUG
					__ixs_ang_assert__( p_omg_bi->mu == 0 );
#endif
					t3 += p_omg_bi->d;
#if defined( __IXS_ANGULAR_DATA_PRINT )
					if( to_compute_value )
					print_ang_max_SemiLocal_mu_sum( _it, IT_MAX, "mu_b", p_omg_bi->mu, p_omg_bi->m,
						U(1), p_omg_bi->d, t3 );
#endif
				}
				t1 += t2 * t3;
#if defined( __IXS_ANGULAR_DATA_PRINT )
				if( to_compute_value )
				{
				print_ang_max_SemiLocal_m_sum<T,U>( _it, IT_MAX, _lxyz, _nx, a, b, c, _lxyz.bx, _lxyz.by, _lxyz.bz, m, t1, t2, t3 );
				++_it;
				}
#endif
			}
			t1 *= t3_a;
			angular_value += t1;
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( to_compute_value )
	if( (_it-1) < IT_MAX )
	{
		/*
		if( angular_value == 0 )
		std::clog << std::setw(6) << _it <<
			std::setw(3) << _nx.na << std::setw(2) << a << std::setw(2) << b << std::setw(2) << c << 
			std::setw(3) << _nx.nb << std::setw(2) << _lxyz.bx << std::setw(2) << _lxyz.by << std::setw(2) << _lxyz.bz << 
			std::setw(3) << _nx.na + _nx.nb << std::setw(3) << _nx.lmbA <<
			std::setw(3) << _lxyz.l << std::endl;
		*/
		std::clog << "------------------------------" << std::endl;
		std::clog << std::setw(14) << "" <<
			std::setw(25) << std::setprecision(15) << std::scientific << angular_value <<
			std::setw(25) << std::setprecision(15) << std::scientific << t3_a <<
			std::setw(25) << std::setprecision(15) << std::scientific << t1 / t3_a <<
			std::endl;
		std::clog << "------------------------------" << std::endl;
	}
#endif
		}
	}
#if defined( __IXS_ANGULAR_DATA_PRINT ) && defined(__IXS_ANGULAR_DATA_PRINT_DETAIL)
	if( to_compute_value )
	if( (_it-1) < IT_MAX )
	{
		std::clog << "------------------------------" << std::endl;
		std::clog << std::setw(25) << std::setprecision(15) << std::scientific << angular_value << std::endl;
		std::clog << "------------------------------" << std::endl;
	}
#endif
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_min( ixs_omega<T,U> const & ixs_omg )
{
	if( !this->ixs_angular_map::is_mapping_min() )
	{
		this->error("comp_ang_min", "ixs_angular_map's mapping type isn't minimum");
		std::cerr << "ixs_ang_map.mapping_type : " << this->ixs_angular_map::get_mapping() << std::endl;
		exit(1);
	}
	T * __p_mx1ang_data = this->_M_mx1ang->data(), * __p_mx1ang;
	_lxyz_struct _lxyz;
	for(int la = 0, ila_size = 1; la <= this->ixs_angular_map::la_max(); ++la, ila_size += (la+1))
	{
		_lxyz.la = la;
		this->ixs_angular_map::map3node_set_la( la );
		for(int i_la = 0; i_la < ila_size; ++i_la)// ax, ay, az
		{
			i2abc( la, ila_size, i_la, _lxyz.ax, _lxyz.ay);
			_lxyz.az = la - _lxyz.ax - _lxyz.ay;
			this->ixs_angular_map::map3node_set_ia( i_la );
			for(int lb = la%2, ilb_size = (la%2 ? 3 : 1); lb <= this->ixs_angular_map::lb_max(); lb += 2, ilb_size += (2*lb+1))
			{
				_lxyz.lb = lb;
				this->ixs_angular_map::map3node_set_lb( lb );
				for(int i_lb = 0; i_lb < ilb_size; ++i_lb )// bx, by, bz
				{
					i2abc( lb, ilb_size, i_lb, _lxyz.bx, _lxyz.by);
					_lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					this->ixs_angular_map::map3node_set_ib( i_lb );
					// semi-local
					// when lmbA_min = 0 ? -> if l<=la && l%2 == la%2
					// when lmbB_min = 0 ? -> if l<=lb && l%2 == lb%2
					for(int l = lb%2; l < this->ixs_angular_map::l_max() && l <= lb && l <= la; l += 2 )
					{
						this->ixs_angular_map::map3node_set_l( l );
						__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
#ifdef  __IXS_ANGULAR_DATA_DEBUG
						__ixs_ang_assert__( this->ixs_angular_map::map3node_size() == 1 );
#endif
						this->ixs_angular_map::map3nx2_set_l( l );
						this->ixs_angular_map::map2lmbA_set_l( l );
						this->ixs_angular_map::map2lmbB_set_l( l );
						_lxyz.l = l;
						this->comp_ang_min_SemiLocal( __p_mx1ang, _lxyz, ixs_omg );
					}
				}
			}
		}
	}
}

template<typename T, typename U>
void ixs_angular_dat<T,U>::comp_ang_min_SemiLocal(pointer __p_mx1ang, _lxyz_struct const & _lxyz, ixs_omega<T,U> const & ixs_omg)
{
	static U t1(0), t2(0), t3(0);

	ixs_omega<T,U> ixs_omg_a( ixs_omg ), ixs_omg_b( ixs_omg );

	ixs_omg_a.set_l( _lxyz.l );
	ixs_omg_a.set_lx( _lxyz.la );
	ixs_omg_a.set_lambda_i( 0 );
	ixs_omg_a.set_abc_i( abc2i( _lxyz.la, _lxyz.ax, _lxyz.ay ) );
	if( ixs_omg_a.get_omg_size() == 0 )
	{
		*__p_mx1ang = 0;
		return;
	}

	ixs_omg_b.set_l( _lxyz.l );
	ixs_omg_b.set_lx( _lxyz.lb );
	ixs_omg_b.set_lambda_i( 0 );
	ixs_omg_b.set_abc_i( abc2i( _lxyz.lb, _lxyz.bx, _lxyz.by ) );
	if( ixs_omg_b.get_omg_size() == 0 )
	{
		*__p_mx1ang = 0;
		return;
	}
	typename ixs_omega<T,U>::_value_struct const * p_omg_ai, * p_omg_bi;
	p_omg_ai = ixs_omg_a.get_omg_data();
	p_omg_bi = ixs_omg_b.get_omg_data();
	const int a_size = ixs_omg_a.get_omg_size();
	const int b_size = ixs_omg_b.get_omg_size();
	t1 = 0;
	for(int m = -_lxyz.l, it_oa = 0, it_ob = 0; m <= _lxyz.l; ++m)
	{
		t2 = 0;
		for(; p_omg_ai->m == m && it_oa < a_size; ++it_oa, ++p_omg_ai)
		{
			t2 += p_omg_ai->d;
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( p_omg_ai->mu == 0 );
			__ixs_ang_assert__( p_omg_ai->lmb == 0 );
#endif
		}
		t3 = 0;
		for(; p_omg_bi->m == m && it_ob < b_size; ++it_ob, ++p_omg_bi)
		{
#ifdef  __IXS_ANGULAR_DATA_DEBUG
			__ixs_ang_assert__( p_omg_bi->mu == 0 );
			__ixs_ang_assert__( p_omg_bi->lmb == 0 );
#endif
			t3 += p_omg_bi->d;
		}
		t1 += t2 * t3;
	}
	*__p_mx1ang = math::convert_float<T,U>( t1 );

	return;
}

#ifdef  __IXS_ANGULAR_DATA_TEST
template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_max( alpha_map & alp_s )
{
	T const * __p_mx1ang_data = this->_M_mx1ang->data(), * __p_mx1ang;
	_lxyz_struct _lxyz;
	for(int la = 0, ila_size = 1; la <= this->ixs_angular_map::la_max(); ++la, ila_size += (la+1))
	{
		_lxyz.la = la;
		this->ixs_angular_map::map3node_set_la( la );
		alp_s.alpha_map::map1A_set_lx( la );
		for(int i_la = 0; i_la < ila_size; ++i_la)// ax, ay, az
		{
			i2abc( la, ila_size, i_la, _lxyz.ax, _lxyz.ay);
			_lxyz.az = la - _lxyz.ax - _lxyz.ay;
			this->ixs_angular_map::map3node_set_ia( i_la );
			for(int lb = 0, ilb_size = 1; lb <= this->ixs_angular_map::lb_max(); ++lb, ilb_size += (lb+1))
			{
				_lxyz.lb = lb;
				this->ixs_angular_map::map3node_set_lb( lb );
				alp_s.alpha_map::map1B_set_lx( lb );
				for(int i_lb = 0; i_lb < ilb_size; ++i_lb )// bx, by, bz
				{
					i2abc( lb, ilb_size, i_lb, _lxyz.bx, _lxyz.by);
					_lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					this->ixs_angular_map::map3node_set_ib( i_lb );
					to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					if( !to_compute_value ) continue;
					// semi-local
					for(int l = 0; l < this->ixs_angular_map::l_max(); ++l )
					{
						_lxyz.l = l;
						this->ixs_angular_map::map3node_set_l( l );
						__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
						alp_s.map1C_set_lx( l );
						this->ixs_angular_map::map3nx2_set_l( l );
						this->ixs_angular_map::map2lmbA_set_l( l );
						this->ixs_angular_map::map2lmbB_set_l( l );

						this->test_ang_max_SemiLocal( __p_mx1ang, _lxyz );
					}
					// local
					_lxyz.l = this->ixs_angular_map::l_max();
					this->ixs_angular_map::map3node_set_lmax();
					__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
					this->ixs_angular_map::map3nx2_set_lmax();
					for(int ia = 0; ia < alp_s.map1A_size(); ++ia)
					{
						for(int ib = 0; ib < alp_s.map1B_size(); ++ib, __p_mx1ang += this->ixs_angular_map::map3node_size() )
						{
							this->test_ang_max_Local( __p_mx1ang, _lxyz );
						}
					}
					// TODO: spin-orbit part
				}
			}
		}
	}
}

template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_max_SemiLocal(T const * __p_mx1ang, _lxyz_struct const & _lxyz )
{
	_nx_struct _nx;
	for(int na = 0; na <= _lxyz.la; ++na)
	{
		this->ixs_angular_map::map3nx2_set_na( na );
		this->ixs_angular_map::map2lmbA_set_nx( na );
		_nx.na = na;
		for(int nb = 0; nb <= _lxyz.lb; ++nb)
		{
			this->ixs_angular_map::map3nx2_set_nb( nb );
			this->ixs_angular_map::map2lmbB_set_nx( nb );
			_nx.nb = nb;
			_nx.na_p_nb = na + nb;
			__ixs_ang_assert__( this->ixs_angular_map::map3nx2() == this->map2lmbA_size() * this->map2lmbB_size() );
			for(int i_lmb_a = 0, lmb_a = this->map2lmbA_min(); i_lmb_a < this->map2lmbA_size(); ++i_lmb_a, lmb_a += 2 )
			{
				_nx.lmbA = lmb_a;
				for(int i_lmb_b=0, lmb_b=this->map2lmbB_min(); i_lmb_b < this->map2lmbB_size(); ++i_lmb_b, lmb_b += 2, ++__p_mx1ang)
				{
					_nx.lmbB = lmb_b;
					this->test_print( *__p_mx1ang, _lxyz, _nx );
				}
			}
		}
	}
}

template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_max_Local(T const * __p_mx1ang, _lxyz_struct const & _lxyz )
{
	_nx_struct _nx;
	for(int na = 0; na <= _lxyz.la; ++na)
	{
		this->ixs_angular_map::map3nx2_set_na( na );
		_nx.na = na;
		for(int nb = 0; nb <= _lxyz.lb; ++nb)
		{
			this->ixs_angular_map::map3nx2_set_nb( nb );
			_nx.nb = nb;
			_nx.na_p_nb = na + nb;
			__ixs_ang_assert__( this->ixs_angular_map::map3nx2() == (_nx.na_p_nb/2 + 1) );
			for(int i_lmb = 0, lmb = _nx.na_p_nb%2; i_lmb < this->map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang )
			{
				_nx.lmbA = lmb;
				_nx.lmbB = 0;
				this->test_print( *__p_mx1ang, _lxyz, _nx );
			}
		}
	}
}

template<typename T, typename U> void ixs_angular_dat<T,U>::test_print( T const & value, _lxyz_struct const & _lxyz, _nx_struct const & _nx )const
{
	static int iter = 0;
	static const int IT_MAX = 1000;
	static const int prec = 15, w = prec + 10;
	if( iter > IT_MAX )
		return;
	std::ostream & out = std::clog;
	if( iter%50 == 0 )
	{
		out <<  std::setw(8) << "iter" <<
		std::setw(4) << "la" << std::setw(3) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" << 
		std::setw(4) << "lb" << std::setw(3) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" << 
		std::setw(4) << "l" <<
		std::setw(4) << "na" << std::setw(4) << "nb" <<
		std::setw(4) << "l'a" << std::setw(4) << "l'b" <<
		std::setw(w) << "value" <<
		std::endl;
	}
	out <<  std::setw(8) << iter <<
		std::setw(4) << _lxyz.la << std::setw(3) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az << 
		std::setw(4) << _lxyz.lb << std::setw(3) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz << 
		std::setw(4) << _lxyz.l <<
		std::setw(4) << _nx.na << std::setw(4) << _nx.nb <<
		std::setw(4) << _nx.lmbA << std::setw(4) << _nx.lmbB <<
		std::setw(w) << std::setprecision(prec) << std::scientific << value <<
		std::endl;
	++iter;
}

template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_mid()
{
	T const * __p_mx1ang_data = this->_M_mx1ang->data(), * __p_mx1ang;
	_lxyz_struct _lxyz;
	for(int la = 0, ila_size = 1; la <= this->ixs_angular_map::la_max(); ++la, ila_size += (la+1))
	{
		_lxyz.la = la;
		this->ixs_angular_map::map3node_set_la( la );
		for(int i_la = 0; i_la < ila_size; ++i_la)// ax, ay, az
		{
			i2abc( la, ila_size, i_la, _lxyz.ax, _lxyz.ay);
			_lxyz.az = la - _lxyz.ax - _lxyz.ay;
			this->ixs_angular_map::map3node_set_ia( i_la );
			for(int lb = 0, ilb_size = 1; lb <= this->ixs_angular_map::lb_max(); ++lb, ilb_size += (lb+1))
			{
				_lxyz.lb = lb;
				this->ixs_angular_map::map3node_set_lb( lb );
				for(int i_lb = 0; i_lb < ilb_size; ++i_lb )// bx, by, bz
				{
					i2abc( lb, ilb_size, i_lb, _lxyz.bx, _lxyz.by);
					_lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					this->ixs_angular_map::map3node_set_ib( i_lb );
					to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					if( !to_compute_value )
						continue;
					// semi-local
					// when lmb_min = 0 ? -> if l<=lb && l%2 == lb%2
					for(int l = lb%2; l < this->ixs_angular_map::l_max() && l <= lb; l += 2 )
					{
						this->ixs_angular_map::map3node_set_l( l );
						__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
						this->ixs_angular_map::map3nx2_set_l( l );
						this->ixs_angular_map::map2lmbA_set_l( l );
						_lxyz.l = l;
						this->test_ang_mid_SemiLocal( __p_mx1ang, _lxyz );
					}
					// local
					this->ixs_angular_map::map3node_set_lmax();
					_lxyz.l = this->ixs_angular_map::l_max();
					__p_mx1ang = __p_mx1ang_data + this->ixs_angular_map::map3node_pos();// position
					this->ixs_angular_map::map3nx2_set_lmax();
					this->test_ang_mid_Local( __p_mx1ang, _lxyz );
					// TODO: spin-orbit part
				}
			}
		}
	}
}
template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_mid_SemiLocal(T const * __p_mx1ang, _lxyz_struct const & _lxyz )
{
	_nx_struct _nx;
	for(int na = 0; na <= _lxyz.la; ++na)
	{
		this->ixs_angular_map::map3nx2_set_na( na );
		this->ixs_angular_map::map2lmbA_set_nx( na );
		_nx.na = na;
		for(int nb = _lxyz.lb; nb <= _lxyz.lb; ++nb)
		{
			this->ixs_angular_map::map3nx2_set_nb( 0 );
			_nx.nb = nb;
			_nx.na_p_nb = na + nb;
			__ixs_ang_assert__( this->ixs_angular_map::map3nx2() == this->map2lmbA_size() );
			for(int i_lmb_a = 0, lmb_a = this->map2lmbA_min(); i_lmb_a < this->map2lmbA_size(); ++i_lmb_a, lmb_a += 2, ++__p_mx1ang )
			{
				_nx.lmbA = lmb_a;
				_nx.lmbB = 0;
				this->test_print( *__p_mx1ang, _lxyz, _nx );
			}
		}
	}
}

template<typename T, typename U> void ixs_angular_dat<T,U>::test_ang_mid_Local(T const * __p_mx1ang, _lxyz_struct const & _lxyz )
{
	_nx_struct _nx;
	for(int na = 0; na <= _lxyz.la; ++na)
	{
		this->ixs_angular_map::map3nx2_set_na( na );
		_nx.na = na;
		for(int nb = _lxyz.lb; nb <= _lxyz.lb; ++nb)
		{
			this->ixs_angular_map::map3nx2_set_nb( nb );
			_nx.nb = nb;
			_nx.na_p_nb = na + nb;
			__ixs_ang_assert__( this->ixs_angular_map::map3nx2() == _nx.na_p_nb/2 + 1 );
			for(int i_lmb = 0, lmb = _nx.na_p_nb%2; i_lmb < this->map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang )
			{
				_nx.lmbA = lmb;
				_nx.lmbB = 0;
				this->test_print( *__p_mx1ang, _lxyz, _nx );
			}
		}
	}
}
#endif

#define IXS_ANGULAR_DATA_SPEC( typeU )\
	template class ixs_angular_dat<float, typeU>;\
	template class ixs_angular_dat<double, typeU>;\
	template class ixs_angular_dat<long double, typeU>;

#endif//__IXS_ANGULAR_DATA_HPP__
