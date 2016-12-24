#ifndef __QU_RADIAL_DAT_HPP__
#define __QU_RADIAL_DAT_HPP__
#include"qu.radial.dat.h"
#include<cstring>// memcpy
#include<cstdlib>// exit
#include"sf.hyperg.1F11.h"
#include"sf.hyperg.1F1.h"

#ifdef  __QU_RADIAL_DAT_PRINT
  #include<iomanip>
#endif

template<typename T, typename U> qu_radial_dat<T,U>::qu_radial_dat(): memory_map(), memorystream(), qu_radial_map(), alpha_pow<T,U>(),
	_M_mx1qu(), _M_map3qu(), __QU_RADIAL_DAT_ITER_INIT_LIST {}
template<typename T, typename U> qu_radial_dat<T,U>::qu_radial_dat( qu_radial_dat<T,U> const & v ):
	memory_map(), memorystream( v ), qu_radial_map( v ), alpha_pow<T,U>( v ), _M_mx1qu( v._M_mx1qu ), _M_map3qu( v._M_map3qu ),
	__QU_RADIAL_DAT_COPY_ITER_INIT_LIST( v ) {}
template<typename T, typename U> qu_radial_dat<T,U> & qu_radial_dat<T,U>::operator=( qu_radial_dat<T,U> const & v )
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	qu_radial_dat<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;
}

template<typename T, typename U> void qu_radial_dat<T,U>::memory_create( const char * file, int __flags, mode_type __mode )
{
	size_type __size = this->qu_radial_dat<T,U>::comp_size();
	if( __size == 0 && this->alpha_map::is_mapping_min() )
		return;
	int _st = this->create( file, __size, __flags, __mode );
	if( _st )
	{
		this->error( "memory_create", "memory mapping failed" );
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		std::cerr << "comp_size : " << __size << std::endl;
		exit(1);
	}
	this->sync_stream();
	this->write_dat();
#if 0
	if( __size != this->tell() )
	{
		std::cerr << "memory_stream::data: [" << (void *)(this->memorystream::getbuf()) << "]" << std::endl;
		std::cerr << "memory_stream::size: " << this->memorystream::tell() << std::endl;
		std::cerr << "memory_map::data: [" << this->memory_map::data() << "]" << std::endl;
		std::cerr << "memory_map::size: " << this->memory_map::size() << std::endl;
	}
#endif
	__qu_dat_assert__( __size == this->tell() );
}

template<typename T, typename U> void qu_radial_dat<T,U>::memory_open( const char * file, int __flags, mode_type __mode )
{
	if( this->alpha_map::is_mapping_min() )
		return;
	int _st = this->open( file, __flags, __mode );
	if( _st )
	{
		this->error( "memory_open", "memory mapping failed" );
		std::cerr << " map.data : [" << this->data() << "]" << std::endl;
		std::cerr << " map.size : " << this->size() << std::endl;
		exit(1);
	}
	this->sync_stream();
	this->read_dat();
	__qu_dat_assert__( this->comp_size() == this->tell() );
}

template<typename T, typename U> const typename qu_radial_dat<T,U>::size_type qu_radial_dat<T,U>::comp_size()
{
	if( this->alpha_map::is_mapping_min() )
		return 0;
	size_type __size = 0;
	__size += sizeof( size_struct<3> );
	__size += this->map3qu_SIZE() * sizeof( map3qu_type );

	__size += sizeof( size_struct<1> );
	__size += this->mx1qu_SIZE() * sizeof(T);

	return __size;
}

template<typename T, typename U> const typename qu_radial_dat<T,U>::size_type qu_radial_dat<T,U>::map3qu_SIZE()
{
	return (this->M_get_la_max() + 1) * (this->M_get_lb_max() + 1 ) * ( this->M_get_l_max() + this->M_get_lso_max() + 1 );
}

template<typename T, typename U> const typename qu_radial_dat<T,U>::size_type qu_radial_dat<T,U>::mx1qu_SIZE()
{
	size_type __size = 0;
	for(int la = 0; la <= this->M_get_la_max(); ++la )
	{
		this->qu_set_la( la );
		this->map3ABC_set_la( la );
		for(int lb = 0; lb <= this->M_get_lb_max(); ++lb )
		{
			this->qu_set_lb( lb );
			this->map3ABC_set_lb( lb );
			// semi-local (+) local
			for(int l = 0; l <= this->M_get_l_max(); ++l )
			{
				this->qu_set_l( l );
				this->map3ABC_set_l( l );
				__size += this->qu_size() * this->map3ABC_size();
			}
			// TODO: spin-orbit
		}
	}
	return __size;
}

template<typename T, typename U> void qu_radial_dat<T,U>::write_dat()
{
	union{
		void * _void;
		matrix_cursor_1<T> * _mx1T;
		matrix_cursor_3<map3qu_type> * _map3q;
	} __cnvrt;
	memorystream & ms = *this;
	__cnvrt._void = ms.getcur();
	this->_M_map3qu = __cnvrt._map3q;
	this->_M_map3qu->n() = this->M_get_la_max() + 1;
	this->_M_map3qu->m() = this->M_get_lb_max() + 1;
	this->_M_map3qu->p() = this->M_get_l_max() + this->M_get_lso_max() + 1;
	this->_M_map3qu->init_size();
	ms.seek( sizeof(size_struct<3>) + this->_M_map3qu->size() * sizeof(map3qu_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1qu = __cnvrt._mx1T;
	this->_M_mx1qu->size() = this->mx1qu_SIZE();
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1qu->size() * sizeof(T), seek_dir::cur );
}

template<typename T, typename U> void qu_radial_dat<T,U>::read_dat()
{
	union{
		void * _void;
		matrix_cursor_1<T> * _mx1T;
		matrix_cursor_3<map3qu_type> * _map3q;
	} __cnvrt;
	memorystream & ms = *this;
	__cnvrt._void = ms.getcur();
	this->_M_map3qu = __cnvrt._map3q;
	ms.seek( sizeof(size_struct<3>) + this->_M_map3qu->size() * sizeof(map3qu_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1qu = __cnvrt._mx1T;
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1qu->size() * sizeof(T), seek_dir::cur );
}

template<typename T, typename U> void qu_radial_dat<T,U>::init_dat()
{
	if( this->alpha_map::is_mapping_min() )
		return;
	size_type __pos = 0;
	for(int la = 0; la <= this->M_get_la_max(); ++la )
	{
		this->qu_dat_set_la( la );
		for(int lb = 0; lb <= this->M_get_lb_max(); ++lb )
		{
			this->qu_dat_set_lb( lb );
			// semi-local (+) local
			for(int l = 0; l <= this->M_get_l_max(); ++l )
			{
				this->qu_dat_set_l( l );

				this->qu_dat_pos() = __pos;
				this->qu_dat_size() = this->qu_size() * this->map3ABC_size();
				__pos += this->qu_dat_size();
			}
			// TODO: spin-orbit
		}
	}
}

template<typename T, typename U> void qu_radial_dat<T,U>::comp_qu()
{
	switch( this->qu_radial_map::get_mapping() )
	{
	case mapping_struct::minimum: return;
	case mapping_struct::middle : return this->comp_qu_mid();
	case mapping_struct::maximum: return this->comp_qu_max();
	default:
		this->error("comp_qu", "unknown mapping type");
		std::cerr << "current mapping type : " << this->qu_radial_map::get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U> void qu_radial_dat<T,U>::comp_qu_max()
{
	__qu_dat_assert__( this->qu_radial_map::is_mapping_max() );
	_lx_struct _lx;
	qu_hyperg_1F11_struct<T> qu_1f11;
	qu_hyperg_1F1_struct<T>  qu_1f1;
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _arr_maxsize = 2 * ( this->M_get_la_max() + this->M_get_lb_max() + this->M_get_l_max() ) + __nk_max + __QU_RADIAL_DAT_R_POWN + 1;
	T _pown05_alp[_arr_maxsize] = {0}, _alp = 0;
	__qu_dat_assert__( this->qu_radial_map::M_get_la_max() == this->alpha_map::la_max() );
	__qu_dat_assert__( this->qu_radial_map::M_get_lb_max() == this->alpha_map::lb_max() );
	for(int la = 0; la <= this->qu_radial_map::M_get_la_max(); ++la)
	{
		_lx.la = la;
		this->qu_dat_set_la( la );// <- map1A_set_lx( la )
		this->map2AB_set_la( la );// map of A-B exponent powers(alphas)/values(exp): set A angular momentum (set_la)
		this->map1powkA_set_lx( la );// powers map of |kA| values - for semi-local part
		this->map2powk_set_la( la );//  powers map of |k| = |kA + kB| - for local part
		for(int lb = 0; lb <= this->qu_radial_map::M_get_lb_max(); ++lb)
		{
			_lx.lb = lb;
			this->qu_dat_set_lb( lb );// <- map1B_set_lx( lb )
			this->map2AB_set_lb( lb );// A-B map: set B angular momentum (set_lb)
			this->map1powkB_set_lx( lb );// powers map of |kB| values - for semi-local part
			// semi-local
			for(int l = 0; l < this->qu_radial_map::M_get_l_max(); ++l)
			{
				_lx.l = l;
				this->qu_dat_set_l( l );// <- map1C_set_lx( l )
				this->comp_qu_max_SemiLocal( _lx, qu_1f11, _alp, _pown05_alp, _arr_maxsize );
			}
			this->map2powk_set_lb( lb );
			// local
			this->qu_dat_set_lmax();
			this->comp_qu_max_Local( _lx, qu_1f1, _alp, _pown05_alp, _arr_maxsize );
			// TODO: spin-orbit
			this->comp_qu_max_SpinOrbit();
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_max_SemiLocal( _lx_struct const & _lx, qu_hyperg_1F11_struct<T> & qu, T & _alp,
		pointer _pown05_alp, const int _arr_maxsize )
{
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _alp_pown05_max = 2 * (_lx.la + _lx.lb + _lx.l ) + __nk_max + __QU_RADIAL_DAT_R_POWN + 1;
	__qu_dat_assert__( _alp_pown05_max + 1 <= _arr_maxsize );
	__qu_dat_assert__( this->alpha_map::map2AB_size() == this->alpha_map::map1A_size() * this->alpha_map::map1B_size() );
	// P.S. it needs to be set alpha_map::map1B_set_lx( int ) to correctly usage of alpha_map::map2AB_set_ia( int ) method
	for(int ia = 0, iter = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->qu_dat_set_ia( ia );

		this->alpha_map::map1A_set_ix( ia );// index ( map1A_idx() ) evaluated for mx1A, and mx1kA usage
		this->alpha_val<T,U>::mx1A_set_idx(); // mx1A_set_idx( map1A_idx() );
		this->alpha_val<T,U>::mx1kA_set_idx();// mx1kA_set_idx( map1A_idx() );
		qu.set_k1( this->alpha_val<T,U>::mx1kA() );

		this->alpha_map::map1powkA_set_ix( ia );// set index for mx1powkA usage
		this->alpha_pow<T,U>::mx1powkA_set_idx();// mx1powkA_set_idx( map1powkA_idx() )

		this->alpha_map::map2AB_set_ia( ia );// mx2AB_exp: a index for exp( -alpA * |CA|^2 ) * exp( -alpB * |CB|^2 )
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->qu_dat_set_ib( ib );

			this->alpha_map::map1B_set_ix( ib );// set index map1B_idx()
			this->alpha_val<T,U>::mx1B_set_idx();// mx1B_set_idx( map1B_idx() )
			this->alpha_val<T,U>::mx1kB_set_idx();// mx1kB_set_idx( map1B_idx() )
			qu.set_k2( this->alpha_val<T,U>::mx1kB() );

			this->alpha_map::map1powkB_set_ix( ib );
			this->alpha_pow<T,U>::mx1powkB_set_idx();// mx1powkB_set_idx( map1powkB_idx() )

			this->alpha_map::map2AB_set_ib( ib );// b index for exp( -alpA * |CA|^2 ) * exp( -alpB * |CB|^2 )
			this->alpha_val<T,U>::mx2AB_exp_set_idx();// mx2AB_exp_set_idx( map2AB_idx() )
			for(int ic = 0, nk, _st = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->qu_dat_set_ic( ic );
				this->alpha_map::map1C_set_ix( ic );
				this->alpha_val<T,U>::mx1C_set_idx();// mx1C_set_idx( map1C_idx() )

				_alp = this->alpha_val<T,U>::mx1A();
				_alp += this->alpha_val<T,U>::mx1B();
				_alp += this->alpha_val<T,U>::mx1C();
				__qu_dat_assert__( _alp != 0 );
				//nk = this->alpha_val<T,U>::mx1C_nk();

				qu.set_alp( _alp );
				qu.set_hyperg_t();
				qu.set_flip();
				qu.set_n1( 0 );
				qu.set_n2( 0 );
				qu.set_n1_max( 0 );
				qu.set_n2_max( 0 );

				_st = math::pown05<T>( _alp, _alp_pown05_max, _pown05_alp, _arr_maxsize );
				__qu_dat_assert__( _st == 0 );
				__qu_dat_assert__( _alp_pown05_max <= _arr_maxsize );
				//__qu_dat_assert__( this->mx1powkA()[0] == 1 );
				//__qu_dat_assert__( this->mx1powkB()[0] == 1 );
				this->comp_qu_max_SemiLocal_b( _lx, qu, _pown05_alp, _alp_pown05_max );
				this->qu_reset_done_all();
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_max_SemiLocal_b( _lx_struct const& _lx, qu_hyperg_1F11_struct<T>& qu, pointer _pown05_alp, const int _alp_pown05_max )
{
	// lmbA_max = l + na
	// lmbB_max = l + nb
#ifdef  __QU_RADIAL_DAT_PRINT
	static int _it = 0, IT_MAX = __ITER_MAX__;
	static int _it_err = 0;
#endif
	const int nk = this->mx1C_nk() + __QU_RADIAL_DAT_R_POWN;
	for(int na = 0, lmbA_min, lmbA_max = _lx.l + na; na <= _lx.la; ++na, ++lmbA_max)
	{
		lmbA_min = ( _lx.l < na ? (_lx.l + na)%2 : _lx.l - na );
		__qu_dat_assert__( lmbA_max == _lx.l + na );
		for(int nb = 0, N = na, Nk = N + nk, lmbB_min, lmbB_max = _lx.l + nb; nb <= _lx.lb; ++nb, ++N, ++Nk, ++lmbB_max)
		{
			this->qu_radial_map::qu_set_N( N );
			__qu_dat_assert__( lmbB_max == _lx.l + nb );
			lmbB_min = ( _lx.l < nb ? (_lx.l + nb)%2 : _lx.l - nb );
			for(int lmb_A = lmbA_min; lmb_A <= lmbA_max; lmb_A += 2 )
			{
				// powers of |kA| - values of pown( kA, lmb_A ) - this->alpha_pow<T,U>::mx1powkA( lmb_A );
				this->qu_radial_map::qu_set_lmb_a( lmb_A );

				qu.set_lmb1( lmb_A );
				for(int lmb_B = lmbB_min, pow_sum = Nk + lmb_A + lmbB_min + 1; lmb_B <= lmbB_max; lmb_B += 2, pow_sum += 2 )
				{
					this->qu_radial_map::qu_set_lmb_b( lmb_B );
					if( this->qu_is_done() || !this->qu_idx_is_good() || !this->qu_is_on() )
						continue;
					// P.S. pow_sum = na + nb + nk + lmb_A + lmb_B + 1
					__qu_dat_assert__( pow_sum <= _alp_pown05_max );
					__qu_dat_assert__( pow_sum == (na + nb + nk + lmb_A + lmb_B + 1) );
					// - values of pown( kB, lmb_B ) - this->alpha_pow<T,U>::mx1powkB( lmb_B );
					_pown05_alp[ pow_sum ];

					qu.set_lmb2( lmb_B );
					qu.set_a_2( pow_sum );
					__qu_dat_assert__( qu.a_2 == (Nk + lmb_A + lmb_B + 1) );
					this->qu_dat_set_id( this->qu_radial_map::qu_idx() );
					qu.value = 0;
					qu.coef_g = 0;
					qu.coef = 0;
					int _hyperg_stat = 1;
					if( qu.to_flip() )
					{
						_hyperg_stat = sf_hyperg_1F11_series_flip( qu );
					} else {
						_hyperg_stat = sf_hyperg_1F11_series( qu );
					}
					if( _hyperg_stat != sf_hyperg::SUCCESS )
					{
						this->error( "comp_qu_max", "hyperg_1F11 computation failed" );
						std::cerr << "errno : " << sf_hyperg::type( _hyperg_stat ) << std::endl;
						exit(1);
					}
					qu.set_coef_g();
					qu.set_coef( this->alpha_pow<T,U>::mx1powkA( lmb_A ), this->alpha_pow<T,U>::mx1powkB( lmb_B ), _pown05_alp,\
							_alp_pown05_max );
					//__qu_dat_assert__( this->mx1powkA( lmb_A ) > 1.0e-100 );
					//__qu_dat_assert__( this->mx1powkB( lmb_B ) > 1.0e-100 );
					this->qu_dat_value() = qu.value;
					this->qu_dat_value() *= qu.coef;
					this->qu_dat_value() *= qu.coef_g;
					this->qu_dat_value() *= this->alpha_val<T,U>::mx2AB_exp();// exp( -alpA * |CA|^2 - alpB * |CB|^2 )
					this->qu_dat_value() *= 4;
					this->qu_dat_value() *= math::numeric_constants<T>::pi;
					this->qu_set_done();// set 2nd bit (done mask) to 1 (true value): is_done() <==> is value calculated?
#if 0
					T _alp = this->mx1A();
					_alp  += this->mx1B();
					_alp  += this->mx1C();
					T q_int = q_int_1f11<T>( Nk, lmb_A, lmb_B, this->mx1kA(), this->mx1kB(), _alp );
#endif
#ifdef  __QU_RADIAL_DAT_PRINT
					if( _it++ < IT_MAX )
					{
					if( _it % 50 == 1 )
					{
					std::clog << std::setw(5) << "iter" <<
						std::setw(4) << "idx" << std::setw(3) << "sz" <<
						std::setw(4) << "la" <<
						std::setw(3) << "lb" <<
						std::setw(3) << "l" <<
						std::setw(4) << "Nk" << std::setw(3) << "N" << std::setw(3) << "nk" <<
						std::setw(3) << "'a" <<
						std::setw(3) << "'b" <<
						std::setw(20) << "1F11(a,b1,b2,x1,x2)" <<
						std::setw(20) << "Gamma" <<
						std::setw(20) << "Power" <<
						std::setw(25) << "Value" <<
						//std::setw(25) << "q_int" <<
						std::endl;
					}
					std::clog << std::setw(5) << _it <<
						std::setw(4) << this->qu_idx() << std::setw(3) << this->qu_size() <<
						std::setw(4) << _lx.la <<
						std::setw(3) << _lx.lb <<
						std::setw(3) << _lx.l <<
						std::setw(4) << Nk << std::setw(3) << N << std::setw(3) << (Nk-N) <<
						std::setw(3) << lmb_A <<
						std::setw(3) << lmb_B <<
						std::setw(20) << std::setprecision(10) << std::scientific << qu.value <<
						std::setw(20) << std::setprecision(10) << std::scientific << qu.coef_g <<
						std::setw(20) << std::setprecision(10) << std::scientific << qu.coef <<
						std::setw(25) << std::setprecision(15) << std::scientific << this->qu_dat_value() <<
						" [" << &this->qu_dat_value() << "]" <<
						//std::setw(25) << std::setprecision(15) << std::scientific << q_int <<
						std::setw(4) << ( qu.hyperg_t == sf_hyperg::_2F0_SERIES ? "2f0" : "" ) <<
						std::setw(4) << qu.n1 << std::setw(4) << qu.n1_max <<
						std::endl;
#if 0
					if( qu.coef < 1.e-100 )
					{
						if( !_it_err )
							std::cerr << std::endl;
						std::cerr << "------------------------------------" << std::endl;
						this->error("comp_qu_max_SemiLocal_b", "overflow ?");
						std::cerr << "mx1powkA( lmb_a ) : " << this->alpha_pow<T,U>::mx1powkA( lmb_A ) <<
							" [" << this->mx1powkA() << "]" << std::endl;
						for(int i = 0; i < 5; ++i)
							std::cerr << "mx1powkA[" << i << "] : " << this->mx1powkA()[i] << std::endl;
						std::cerr << "mx1powkB( lmb_b ) : " << this->alpha_pow<T,U>::mx1powkB( lmb_B ) <<
							" [" << this->mx1powkB() << "]" << std::endl;
						for(int i = 0; i < 5; ++i)
							std::cerr << "mx1powkB[" << i << "] : " << this->mx1powkB()[i] << std::endl;
						std::cerr << "map1powkA_pos  : " << this->map1powkA_pos() << std::endl;
						std::cerr << "M_mx1powkA( pos ) : " << this->M_mx1powkA( this->map1powkA_pos() ) << std::endl;
						std::cerr << "map1powkA_idx  : " << this->map1powkA_idx() << std::endl;
						std::cerr << "M_mx1powkA( idx ) : " << this->M_mx1powkA( this->map1powkA_idx() ) << std::endl;
						std::cerr << "map1powkA_size : " << this->map1powkA_size() << std::endl;
						//
						std::cerr << "map1powkB_pos  : " << this->map1powkB_pos() << std::endl;
						std::cerr << "M_mx1powkB( pos ) : " << this->M_mx1powkB( this->map1powkB_pos() ) << std::endl;
						std::cerr << "map1powkB_idx  : " << this->map1powkB_idx() << std::endl;
						std::cerr << "M_mx1powkB( idx ) : " << this->M_mx1powkB( this->map1powkB_idx() ) << std::endl;
						std::cerr << "map1powkB_size : " << this->map1powkB_size() << std::endl;
						for(int i = 0; i < this->M_mx1powkB_size(); ++i)
							std::cerr << std::setw(6) << i << std::setw(26) << this->M_mx1powkB(i) << std::endl;
						//
						std::cerr << "lmb_a : " << lmb_A << std::endl;
						std::cerr << "lmb_b : " << lmb_B << std::endl;
						std::cerr << "mx1kA : " << this->alpha_val<T,U>::mx1kA() << ' ' <<
							math::pown<T>( this->mx1kA(), lmb_A ) << std::endl;
						std::cerr << "mx1kB : " << this->alpha_val<T,U>::mx1kB() << ' ' <<
							math::pown<T>( this->mx1kB(), lmb_B ) << std::endl;
						T __alp = this->alpha_val<T,U>::mx1A();
						__alp  += this->alpha_val<T,U>::mx1B();
						__alp  += this->alpha_val<T,U>::mx1C();
						std::cerr << "alp : " << __alp << std::endl;
						std::cerr << "pown05_alp_max : " << _alp_pown05_max << std::endl;
						std::cerr << "pown05_alp : [" << _pown05_alp << "]" << std::endl;
						for(int i = 0; i <= _alp_pown05_max; ++i)
							std::cerr << "pown05_alp[" << i << "] : " << _pown05_alp[i] <<
								' ' << math::pown<T>( __alp, i/2) << std::endl;
						if( ++_it_err >= 1 )
							exit(1);
					}
#endif

					}
#endif//__QU_RADIAL_DAT_PRINT
					qu.set_n1_max();
					qu.set_n2_max();
					__qu_dat_assert__( qu.hyperg_t == qu.hyperg_n );
					__qu_dat_assert__( qu.ex == 0 );
					__qu_dat_assert__( qu.exp_ex2 == 0 );
					if( qu.exp_ex != 0 )
					{
						std::clog << "qu.x : " << qu.x << std::endl;
						std::clog << "qu.exp_x : " << qu.exp_x << ' ' << qu.exp_ex << std::endl;
					}
					__qu_dat_assert__( qu.exp_ex == 0 );
				}
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_max_Local( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu, T & _alp,
		pointer _pown05_alp, const int _arr_maxsize )
{
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _alp_pown05_max = 2 * (_lx.la + _lx.lb ) + __nk_max + __QU_RADIAL_DAT_R_POWN + 1;
	__qu_dat_assert__( _alp_pown05_max + 1 <= _arr_maxsize );
	for(int ia = 0, iter = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->qu_dat_set_ia( ia );

		this->alpha_map::map1A_set_ix( ia );// set index map1A_idx() -> mx1A, mx1kA
		this->alpha_val<T,U>::mx1A_set_idx();// mx1A_set_idx( map1A_idx() )
		this->alpha_map::map2AB_set_ia( ia );

		this->alpha_map::map2powk_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->qu_dat_set_ib( ib );
			this->alpha_map::map1B_set_ix( ib );
			this->alpha_val<T,U>::mx1B_set_idx();// mx1B_set_idx( map1B_idx() )
			this->alpha_map::map2AB_set_ib( ib );
			this->alpha_val<T,U>::mx2k_set_idx();// mx2k_set_idx( map2AB_idx() )
			qu.set_k( this->alpha_val<T,U>::mx2k() );
			this->alpha_val<T,U>::mx2AB_exp_set_idx();// mx2AB_exp_set_idx( map2AB_idx() )

			this->alpha_map::map2powk_set_ib( ib );
			this->alpha_pow<T,U>::mx2powk_set_idx();// mx2powk_set_idx( map2powk_idx() )
			for(int ic = 0, nk, _st = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->qu_dat_set_ic( ic );
				this->alpha_map::map1C_set_ix( ic );
				this->alpha_val<T,U>::mx1C_set_idx();

				_alp = this->alpha_val<T,U>::mx1A();
				_alp += this->alpha_val<T,U>::mx1B();
				_alp += this->alpha_val<T,U>::mx1C();
				__qu_dat_assert__( _alp != 0 );
				//nk = this->alpha_val<T,U>::mx1C_nk();

				qu.set_alp( _alp );
				qu.set_hyperg_t();
				qu.set_n( 0 );
				qu.set_n_max( 0 );

				_st = math::pown05<T>( _alp, _alp_pown05_max, _pown05_alp, _arr_maxsize );
				__qu_dat_assert__( _st == 0 );
				this->comp_qu_max_Local_b( _lx, qu, _pown05_alp, _alp_pown05_max );
				this->qu_reset_done_all();
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_max_Local_b( _lx_struct const& _lx, qu_hyperg_1F1_struct<T>& qu, pointer _pown05_alp, const int _alp_pown05_max )
{
	// lmbA_max = l + na
	// lmbB_max = l + nb
#ifdef  __QU_RADIAL_DAT_PRINT
	static int _it = 0, IT_MAX = __ITER_MAX__;
#endif
	const int nk = this->mx1C_nk() + __QU_RADIAL_DAT_R_POWN;
	for(int na = 0; na <= _lx.la; ++na )
	{
		for(int nb = 0, N = na + nb, Nk = N + nk, lmb_min; nb <= _lx.lb; ++nb, ++N, ++Nk )
		{
			this->qu_radial_map::qu_set_N( N );
			for(int lmb = N%2, pow_sum = Nk + lmb + 1; lmb <= N; lmb += 2, pow_sum += 2 )
			{
				// powers of |k| - values of pown( k, lmb ) - this->alpha_pow<T,U>::mx2powk( lmb );
				// powers of alp - value of alp^( pow_sum/2 ) - _pown05_alp[ pow_sum ];
				this->qu_radial_map::qu_set_lmb_a( lmb );
				this->qu_radial_map::qu_set_lmb_b( 0 );// set qu_radial_map::qu_idx()

				qu.set_lmb( lmb );
				if( this->qu_is_done() || !this->qu_idx_is_good() || !this->qu_is_on() )
					continue;
				// P.S. pow_sum = na + nb + nk + lmb + 1
				__qu_dat_assert__( _alp_pown05_max >= pow_sum );
				qu.set_a_2( pow_sum );
				__qu_dat_assert__( qu.a_2 == (Nk + lmb + 1) );
				this->qu_dat_set_id( this->qu_radial_map::qu_idx() );
				qu.value = 0;
				qu.coef_g = 0;
				qu.coef = 0;
				int _hyperg_stat = 1;
				_hyperg_stat = sf_hyperg_1F1( qu );
				if( _hyperg_stat != sf_hyperg::SUCCESS )
				{
					this->error( "comp_qu_max_Local", "hyperg_1F1 computation failed" );
					std::cerr << "errno : " << sf_hyperg::type( _hyperg_stat ) << std::endl;
					exit(1);
				}
				qu.set_coef_g();
				qu.set_coef( this->alpha_pow<T,U>::mx2powk( lmb ), _pown05_alp,	_alp_pown05_max );
				__qu_dat_assert__( this->mx2powk( lmb ) > 1.0e-100 );
				this->qu_dat_value() = qu.value;
				this->qu_dat_value() *= qu.coef;
				this->qu_dat_value() *= qu.coef_g;
				this->qu_dat_value() *= this->alpha_val<T,U>::mx2AB_exp();// exp( -alpA * |CA|^2 - alpB * |CB|^2 )
				this->qu_dat_value() *= 4;
				this->qu_dat_value() *= math::numeric_constants<T>::pi;
				this->qu_set_done();
#ifdef  __QU_RADIAL_DAT_PRINT
				if( _it++ < IT_MAX )
				{
				if( _it % 50 == 1 )
				{
				std::clog << std::setw(8) << "iter" <<
					std::setw(5) << "idx" << std::setw(4) << "siz" <<
					std::setw(5) << "la" <<
					std::setw(4) << "lb" <<
					std::setw(4) << "l" <<
					std::setw(5) << "Nk" << std::setw(4) << "N" << std::setw(3) << "nk" <<
					std::setw(4) << "lmb" <<
					std::setw(22) << "1F1(a, b, x )" <<
					std::setw(22) << "Gamma" <<
					std::setw(22) << "Power" <<
					std::setw(26) << "Value" <<
					std::endl;
				}
				std::clog << std::setw(8) << _it <<
					std::setw(5) << this->qu_idx() << std::setw(4) << this->qu_size() <<
					std::setw(5) << _lx.la <<
					std::setw(4) << _lx.lb <<
					std::setw(4) << _lx.l <<
					std::setw(5) << Nk << std::setw(4) << N << std::setw(3) << (Nk-N) <<
					std::setw(4) << lmb <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.value <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.coef_g <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.coef <<
					std::setw(26) << std::setprecision(16) << std::scientific << this->qu_dat_value() <<
					std::setw(4) << ( qu.hyperg_t == sf_hyperg::_2F0_SERIES ? "2f0" : "" ) <<
					std::setw(4) << qu.n << std::setw(4) << qu.n_max <<
					std::endl;
				}
#endif//__QU_RADIAL_DAT_PRINT
				qu.set_n_max();
				__qu_dat_assert__( qu.ex == 0 );
				__qu_dat_assert__( qu.hyperg_t == qu.hyperg_n );
				if( qu.exp_ex != 0 )
				{
					std::clog << "qu.x : " << qu.x << std::endl;
					std::clog << "qu.exp_x : " << qu.exp_x << ' ' << qu.exp_ex << std::endl;
				}
				__qu_dat_assert__( qu.exp_ex == 0 );
			}
		}
	}
}

template<typename T, typename U> void qu_radial_dat<T,U>::comp_qu_mid()
{
	__qu_dat_assert__( this->qu_radial_map::is_mapping_mid() );
	_lx_struct _lx;
	qu_hyperg_1F1_struct<T>  qu_1f1;
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _arr_maxsize = 2 * ( this->M_get_la_max() + this->M_get_lb_max() + this->M_get_l_max() ) + __nk_max + __QU_RADIAL_DAT_R_POWN + 1 ;
	T _pown05_alp[_arr_maxsize], _alp = 0;
	for(int la = 0; la <= this->qu_radial_map::M_get_la_max(); ++la)
	{
		_lx.la = la;
		this->qu_dat_set_la( la );// <- map1A_set_lx( la )
		this->map2AB_set_la( la );
		this->map1powkA_set_lx( la );// both for local, ang semi-local parts
		for(int lb = 0; lb <= this->qu_radial_map::M_get_lb_max(); ++lb)
		{
			_lx.lb = lb;
			this->qu_dat_set_lb( lb );// <- map1B_set_lx( lb )
			this->map2AB_set_lb( lb );
			// semi-local
			for(int l = lb%2; l < this->qu_radial_map::M_get_l_max() && l <= lb; l += 2)
			{
				_lx.l = l;
				this->qu_dat_set_l( l );// <- map1C_set_lx( l )
				this->comp_qu_mid_SemiLocal( _lx, qu_1f1, _alp, _pown05_alp, _arr_maxsize );
			}
			// local
			this->qu_dat_set_lmax();
			this->comp_qu_mid_Local( _lx, qu_1f1, _alp, _pown05_alp, _arr_maxsize );
			// TODO: spin-orbit
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_mid_Local( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu, T & _alp,
		pointer _pown05_alp, const int _arr_maxsize )
{
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _alp_pown05_max = 2 * (_lx.la + _lx.lb ) + __nk_max + __QU_RADIAL_DAT_R_POWN + 1;
	__qu_dat_assert__( _alp_pown05_max + 1 <= _arr_maxsize );
	for(int ia = 0, iter = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->qu_dat_set_ia( ia );

		this->alpha_map::map1A_set_ix( ia );
		this->alpha_val<T,U>::mx1A_set_idx();
		this->alpha_val<T,U>::mx1kA_set_idx();
		qu.set_k( this->alpha_val<T,U>::mx1kA() );

		this->alpha_map::map1powkA_set_ix( ia );
		this->alpha_pow<T,U>::mx1powkA_set_idx();

		this->alpha_map::map2AB_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->qu_dat_set_ib( ib );
			this->alpha_map::map1B_set_ix( ib );
			this->alpha_val<T,U>::mx1B_set_idx();

			this->alpha_map::map2AB_set_ib( ib );
			this->alpha_val<T,U>::mx2AB_exp_set_idx();
			for(int ic = 0, nk, _st = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->qu_dat_set_ic( ic );
				this->alpha_map::map1C_set_ix( ic );
				this->alpha_val<T,U>::mx1C_set_idx();

				_alp = this->alpha_val<T,U>::mx1A();
				_alp += this->alpha_val<T,U>::mx1B();
				_alp += this->alpha_val<T,U>::mx1C();
				__qu_dat_assert__( _alp != 0 );
				//nk = this->alpha_val<T,U>::mx1C_nk();
				//__qu_dat_assert__( nk <= __nk_max && nk >= 0 );

				qu.set_alp( _alp );
				qu.set_hyperg_t();
				qu.set_n( 0 );
				qu.set_n_max( 0 );

				_st = math::pown05<T>( _alp, _alp_pown05_max, _pown05_alp, _arr_maxsize );
				__qu_dat_assert__( _st == 0 );
				this->comp_qu_mid_Local_b( _lx, qu, _pown05_alp, _alp_pown05_max );
				this->qu_reset_done_all();
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_mid_Local_b( _lx_struct const& _lx, qu_hyperg_1F1_struct<T>& qu, pointer _pown05_alp, const int _alp_pown05_max )
{
	// lmbA_max = l + na
	// lmbB_max = l + nb
#ifdef  __QU_RADIAL_DAT_PRINT
	static int _it = 0, IT_MAX = __ITER_MAX__;
#endif
	const int nk = this->mx1C_nk() + __QU_RADIAL_DAT_R_POWN;
	for(int na = 0; na <= _lx.la; ++na )
	{
		for(int nb = _lx.lb, N = na + nb, Nk = N + nk, lmb_min; nb <= _lx.lb; ++nb, ++N, ++Nk )
		{
			this->qu_radial_map::qu_set_N( N );
			for(int lmb = N%2, pow_sum = Nk + lmb + 1; lmb <= N; lmb += 2, pow_sum += 2 )
			{
				this->qu_radial_map::qu_set_lmb_a( lmb );
				this->qu_radial_map::qu_set_lmb_b( 0 );

				qu.set_lmb( lmb );
				if( this->qu_is_done() || !this->qu_idx_is_good() || !this->qu_is_on() )
					continue;
				// P.S. pow_sum = na + nb + nk + lmb + 1
				__qu_dat_assert__( _alp_pown05_max >= pow_sum );

				qu.set_a_2( pow_sum );
				__qu_dat_assert__( pow_sum == qu.a_2 );
				this->qu_dat_set_id( this->qu_radial_map::qu_idx() );
				qu.value = 0;
				qu.coef_g = 0;
				qu.coef = 0;
				int _hyperg_stat = 1;
				_hyperg_stat = sf_hyperg_1F1( qu );
				if( _hyperg_stat != sf_hyperg::SUCCESS )
				{
					this->error( "comp_qu_mid_Local", "hyperg_1F1 computation failed" );
					std::cerr << "errno : " << sf_hyperg::type( _hyperg_stat ) << std::endl;
					exit(1);
				}
				qu.set_coef_g();
				// powers of |kA| - values of pown( kA, lmb ) - this->alpha_pow<T,U>::mx1powkA( lmb );
				// powers of  alp - value of alp^( pow_sum/2 ) - _pown05_alp[ pow_sum ];
				qu.set_coef( this->alpha_pow<T,U>::mx1powkA( lmb ), _pown05_alp, _alp_pown05_max );
				this->qu_dat_value() = qu.value;
				this->qu_dat_value() *= qu.coef;
				this->qu_dat_value() *= qu.coef_g;
				T _alp = this->mx1A();
				_alp  += this->mx1B();
				_alp  += this->mx1C();
				//T q_int = q_int_1f1<T>( Nk, lmb, this->mx1kA(), _alp );
				this->qu_dat_value() *= this->alpha_val<T,U>::mx2AB_exp();// exp( -alpA * |CA|^2 - alpB * |CB|^2 )
				this->qu_dat_value() *= 4;
				this->qu_dat_value() *= math::numeric_constants<T>::pi;
				this->qu_set_done();
#ifdef  __QU_RADIAL_DAT_PRINT
				if( _it++ < IT_MAX )
				{
				if( _it % 50 == 1 )
				{
				std::clog << std::setw(8) << "iter" <<
					std::setw(5) << "idx" << std::setw(4) << "siz" <<
					std::setw(5) << "la" <<
					std::setw(4) << "lb" <<
					std::setw(4) << "l" <<
					std::setw(5) << "Nk" << std::setw(4) << "N" << std::setw(3) << "nk" <<
					std::setw(4) << "lmb" <<
					std::setw(22) << "1F1(a, b, x )" <<
					//std::setw(22) << "Gamma" <<
					//std::setw(22) << "Power" <<
					std::setw(26) << "Value" <<
					//std::setw(26) << "q_int" <<
					std::endl;
				}
				std::clog << std::setw(8) << _it <<
					std::setw(5) << this->qu_idx() << std::setw(4) << this->qu_size() <<
					std::setw(5) << _lx.la <<
					std::setw(4) << _lx.lb <<
					std::setw(4) << _lx.l <<
					std::setw(5) << Nk << std::setw(4) << N << std::setw(3) << (Nk-N) <<
					std::setw(4) << lmb <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.value <<
					//std::setw(22) << std::setprecision(12) << std::scientific << qu.coef_g <<
					//std::setw(22) << std::setprecision(12) << std::scientific << qu.coef <<
					std::setw(26) << std::setprecision(16) << std::scientific << this->qu_dat_value() <<
					//std::setw(26) << std::setprecision(16) << std::scientific << q_int <<
					std::setw(4) << ( qu.hyperg_t == sf_hyperg::_2F0_SERIES ? "2f0" : "" ) <<
					std::setw(4) << qu.n << std::setw(4) << qu.n_max <<
					std::endl;
				}
#endif//__QU_RADIAL_DAT_PRINT
				qu.set_n_max();
				__qu_dat_assert__( qu.ex == 0 );
				__qu_dat_assert__( qu.hyperg_t == qu.hyperg_n );
				if( qu.exp_ex != 0 )
				{
					std::clog << "qu.x : " << qu.x << std::endl;
					std::clog << "qu.exp_x : " << qu.exp_x << ' ' << qu.exp_ex << std::endl;
				}
				__qu_dat_assert__( qu.exp_ex == 0 );
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_mid_SemiLocal( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu, T & _alp,
		pointer _pown05_alp, const int _arr_maxsize )
{
	const int __nk_max = this->alpha_val<T,U>::mx1C_nk_max();
	const int _alp_pown05_max = 2 * _lx.la + _lx.lb + _lx.l + __nk_max + __QU_RADIAL_DAT_R_POWN + 1;// N + lmb + nk + (2) + 1
	// P.S. N = la + lb; lmb = l + la; nk = __nk_max; (2) = __QU_RADIAL_DAT_R_POWN
	__qu_dat_assert__( _alp_pown05_max + 1 <= _arr_maxsize );
	for(int ia = 0, iter = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->qu_dat_set_ia( ia );

		this->alpha_map::map1A_set_ix( ia );
		this->alpha_val<T,U>::mx1A_set_idx();
		this->alpha_val<T,U>::mx1kA_set_idx();
		qu.set_k( this->alpha_val<T,U>::mx1kA() );

		this->alpha_map::map1powkA_set_ix( ia );
		this->alpha_pow<T,U>::mx1powkA_set_idx();

		this->alpha_map::map2AB_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->qu_dat_set_ib( ib );
			this->alpha_map::map1B_set_ix( ib );
			this->alpha_val<T,U>::mx1B_set_idx();

			this->alpha_map::map2AB_set_ib( ib );
			this->alpha_val<T,U>::mx2AB_exp_set_idx();
			for(int ic = 0, nk, _st = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->qu_dat_set_ic( ic );
				this->alpha_map::map1C_set_ix( ic );
				this->alpha_val<T,U>::mx1C_set_idx();

				_alp = this->alpha_val<T,U>::mx1A();
				_alp += this->alpha_val<T,U>::mx1B();
				_alp += this->alpha_val<T,U>::mx1C();
				__qu_dat_assert__( _alp != 0 );
				//nk = this->alpha_val<T,U>::mx1C_nk();
				//__qu_dat_assert__( nk <= __nk_max && nk >= 0 );

				qu.set_alp( _alp );
				qu.set_hyperg_t();
				qu.set_n( 0 );
				qu.set_n_max( 0 );

				_st = math::pown05<T>( _alp, _alp_pown05_max, _pown05_alp, _arr_maxsize );
				__qu_dat_assert__( _st == 0 );
				this->comp_qu_mid_SemiLocal_b( _lx, qu, _pown05_alp, _alp_pown05_max );
				this->qu_reset_done_all();
			}
		}
	}
}

template<typename T, typename U>
void qu_radial_dat<T,U>::comp_qu_mid_SemiLocal_b( _lx_struct const& _lx, qu_hyperg_1F1_struct<T>& qu, pointer _pown05_alp, const int _alp_pown05_max )
{
	// lmbA_max = l + na
	// lmbB_max = l + nb
#ifdef  __QU_RADIAL_DAT_PRINT
	static int _it = 0, IT_MAX = __ITER_MAX__;
#endif
	const int nk = this->mx1C_nk() + __QU_RADIAL_DAT_R_POWN;
	for(int na = 0, lmb_max = _lx.l; na <= _lx.la; ++na, ++lmb_max )
	{
		for(int nb = _lx.lb, N = na + nb, Nk = N + nk, lmb_min; nb <= _lx.lb; ++nb, ++N, ++Nk )
		{
			this->qu_radial_map::qu_set_N( N );
			for(int lmb = na%2, pow_sum = Nk + lmb + 1; lmb <= lmb_max; lmb += 2, pow_sum += 2 )
			{
				// - values of pown( k, lmb ) - this->alpha_pow<T,U>::mx2powk( lmb );
				this->qu_radial_map::qu_set_lmb_a( lmb );
				this->qu_radial_map::qu_set_lmb_b( 0 );
				this->qu_dat_set_id( this->qu_radial_map::qu_idx() );

				qu.set_lmb( lmb );
				if( this->qu_is_done() || !this->qu_idx_is_good() || !this->qu_is_on() )
					continue;
				// P.S. pow_sum = na + nb + nk + lmb + 1
				__qu_dat_assert__( _alp_pown05_max >= pow_sum );
				// - values of pown( kB, lmb_B ) - this->alpha_pow<T,U>::mx1powkB( lmb_B );
				// - value of alp^( pow_sum/2 ) - _pown05_alp[ pow_sum ];

				qu.set_a_2( pow_sum );
				__qu_dat_assert__( qu.a_2 == (Nk + lmb + 1) );
				qu.value = 0;
				qu.coef_g = 0;
				qu.coef = 0;
				int _hyperg_stat = 1;
				_hyperg_stat = sf_hyperg_1F1( qu );
				if( _hyperg_stat != sf_hyperg::SUCCESS )
				{
					this->error( "comp_qu_mid_SemiLocal", "hyperg_1F1 computation failed" );
					std::cerr << "errno : " << sf_hyperg::type( _hyperg_stat ) << std::endl;
					exit(1);
				}
				qu.set_coef_g();
				qu.set_coef( this->alpha_pow<T,U>::mx1powkA( lmb ), _pown05_alp, _alp_pown05_max );
				this->qu_dat_value() = qu.value;
				this->qu_dat_value() *= qu.coef;
				this->qu_dat_value() *= qu.coef_g;
				this->qu_dat_value() *= this->alpha_val<T,U>::mx2AB_exp();// exp( -alpA * |CA|^2 - alpB * |CB|^2 )
				this->qu_dat_value() *= 4;
				this->qu_dat_value() *= math::numeric_constants<T>::pi;
				this->qu_set_done();
#ifdef  __QU_RADIAL_DAT_PRINT
				if( _it++ < IT_MAX )
				{
				if( _it % 50 == 1 )
				{
				std::clog << std::setw(8) << "iter" <<
					std::setw(5) << "idx" << std::setw(4) << "siz" <<
					std::setw(5) << "la" <<
					std::setw(4) << "lb" <<
					std::setw(4) << "l" <<
					std::setw(5) << "Nk" << std::setw(4) << "N" << std::setw(3) << "nk" <<
					std::setw(4) << "lmb" <<
					std::setw(22) << "1F1(a, b, x )" <<
					std::setw(22) << "Gamma" <<
					std::setw(22) << "Power" <<
					std::setw(26) << "Value" <<
					std::endl;
				}
				std::clog << std::setw(8) << _it <<
					std::setw(5) << this->qu_idx() << std::setw(4) << this->qu_size() <<
					std::setw(5) << _lx.la <<
					std::setw(4) << _lx.lb <<
					std::setw(4) << _lx.l <<
					std::setw(5) << Nk << std::setw(4) << N << std::setw(3) << (Nk-N) <<
					std::setw(4) << lmb <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.value <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.coef_g <<
					std::setw(22) << std::setprecision(12) << std::scientific << qu.coef <<
					std::setw(26) << std::setprecision(16) << std::scientific << this->qu_dat_value() <<
					std::setw(4) << ( qu.hyperg_t == sf_hyperg::_2F0_SERIES ? "2f0" : "" ) <<
					std::setw(4) << qu.n << std::setw(4) << qu.n_max <<
					std::endl;
				}
#endif//__QU_RADIAL_DAT_PRINT
				qu.set_n_max();
				__qu_dat_assert__( qu.ex == 0 );
				__qu_dat_assert__( qu.hyperg_t == qu.hyperg_n );
				if( qu.exp_ex != 0 )
				{
					std::clog << "qu.x : " << qu.x << std::endl;
					std::clog << "qu.exp_x : " << qu.exp_x << ' ' << qu.exp_ex << std::endl;
				}
				__qu_dat_assert__( qu.exp_ex == 0 );
			}
		}
	}
}

#ifdef  __QU_RADIAL_DAT_TEST_METHOD
template<typename T, typename U> void qu_radial_dat<T,U>::test_print( std::ostream & out )
{
	_lx_struct _lx;
	for(int la = 0; la <= this->qu_radial_map::M_get_la_max(); ++la)
	{
		this->qu_dat_set_la( la );
		_lx.la = la;
		for(int lb = 0; lb <= this->qu_radial_map::M_get_lb_max(); ++lb)
		{
			this->qu_dat_set_lb( lb );
			_lx.lb = lb;
			for(int l = 0; l <= this->qu_radial_map::M_get_l_max(); ++l)
			{
				this->qu_dat_set_l( l );
				_lx.l = l;
				this->test_print_alp( out, _lx );
			}
		}
	}
}

template<typename T, typename U> void qu_radial_dat<T,U>::test_print_alp( std::ostream & out, _lx_struct const & _lx )
{
	_lx_struct _ix;
	for(int ia = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->qu_dat_set_ia( ia );
		_ix.la = ia;
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib)
		{
			this->qu_dat_set_ib( ib );
			_ix.lb = ib;
			for(int ic = 0; ic < this->alpha_map::map1C_size(); ++ic)
			{
				this->qu_dat_set_ic( ic );
				_ix.l = ic;
				this->test_print_n( out, _lx, _ix );
				out << std::endl;
			}
		}
	}
}
template<typename T, typename U> void qu_radial_dat<T,U>::test_print_n( std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix )
{
	if( _lx.l == this->qu_radial_map::M_get_l_max() )
	{
		return this->test_print_local( out, _lx, _ix );
	} else {
		return this->test_print_semi_local( out, _lx, _ix );
	}
}

template<typename T, typename U> void qu_radial_dat<T,U>::test_print_semi_local( std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix )
{
	static int it = 0;
	for(int na = 0, lmb_a_min, lmb_a_max = _lx.l + na; na <= _lx.la; ++na, ++lmb_a_max )
	{
		lmb_a_min = ( _lx.l < na ? (_lx.l + na)%2 : _lx.l - na );
		for(int nb = 0, lmb_b_min, lmb_b_max = _lx.l + nb, N = na + nb; nb <= _lx.lb; ++nb, ++lmb_b_max, ++N )
		{
			this->qu_radial_map::qu_set_N( N );
			lmb_b_min = ( _lx.l < nb ? (_lx.l + nb)%2 : _lx.l - nb );
			for( int lmb_a = lmb_a_min; lmb_a <= lmb_a_max; lmb_a += 2 )
			{
				this->qu_radial_map::qu_set_lmb_a( lmb_a );
				for( int lmb_b = lmb_b_min; lmb_b <= lmb_b_max; lmb_b += 2 )
				{
					this->qu_radial_map::qu_set_lmb_b( lmb_b );
					this->qu_dat_set_id( this->qu_radial_map::qu_idx() );
					if( it%50 == 0 )
					out <<  std::setw(4) << "la" << std::setw(3) << "lb" << std::setw(3) << "l" <<
						std::setw(4) << "ia" << std::setw(3) << "ib" << std::setw(3) << "ic" <<
						std::setw(4) << "N" << std::setw(3) << "na" << std::setw(3) << "nb" <<
						std::setw(4) << "l'a" << std::setw(4) << "l'b" <<
						std::setw(25) << "radial value" <<
						std::setw(4) << "idx" <<
						std::endl;
					out <<  std::setw(4) << _lx.la << std::setw(3) << _lx.lb << std::setw(3) << _lx.l <<
						std::setw(4) << _ix.la << std::setw(3) << _ix.lb << std::setw(3) << _ix.l <<
						std::setw(4) << na + nb << std::setw(3) << na << std::setw(3) << nb <<
						std::setw(4) << lmb_a << std::setw(4) << lmb_b <<
						std::setw(25) << std::setprecision(15) << std::scientific << this->qu_dat_value() <<
						std::setw(4) << this->qu_radial_map::qu_idx() <<
						" [" << &this->qu_dat_value() << "]" <<
						" [" << this->qu_dat() << "]" <<
						std::endl;
					++it;
				}
			}
		}
	}
}

template<typename T, typename U> void qu_radial_dat<T,U>::test_print_local( std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix )
{
	static int it = 0;
	for(int na = 0; na <= _lx.la; ++na )
	{
		for(int nb = 0, lmb_min, lmb_max = na + nb, N = na + nb; nb <= _lx.lb; ++nb, ++lmb_max, ++N )
		{
			this->qu_radial_map::qu_set_N( N );
			lmb_min = N%2;
			for( int lmb = lmb_min; lmb <= lmb_max; lmb += 2 )
			{
				this->qu_radial_map::qu_set_lmb_a( lmb );
				this->qu_radial_map::qu_set_lmb_b( 0 );
				this->qu_dat_set_id( this->qu_radial_map::qu_idx() );
				if( it%50 == 0 )
				out <<  std::setw(4) << "la" << std::setw(3) << "lb" << std::setw(3) << "l" <<
					std::setw(4) << "ia" << std::setw(3) << "ib" << std::setw(3) << "ic" <<
					std::setw(4) << "N" << std::setw(3) << "na" << std::setw(3) << "nb" <<
					std::setw(4) << "lmb" << std::setw(4) << "" <<
					std::setw(25) << "radial value" <<
					std::setw(4) << "idx" <<
					std::endl;
				out <<  std::setw(4) << _lx.la << std::setw(3) << _lx.lb << std::setw(3) << _lx.l <<
					std::setw(4) << _ix.la << std::setw(3) << _ix.lb << std::setw(3) << _ix.l <<
					std::setw(4) << na + nb << std::setw(3) << na << std::setw(3) << nb <<
					std::setw(4) << lmb << std::setw(4) << "" <<
					std::setw(25) << std::setprecision(15) << std::scientific << this->qu_dat_value() <<
					std::setw(4) << this->qu_radial_map::qu_idx() <<
					" [" << &this->qu_dat_value() << "]" <<
					" [" << this->qu_dat() << "]" <<
					std::endl;
				++it;
			}
		}
	}
}
#endif//__QU_RADIAL_DAT_TEST_METHOD

#define __QU_RADIAL_DAT_SPEC( typeU )\
template class qu_radial_dat<float, typeU>;\
template class qu_radial_dat<double, typeU>;\
template class qu_radial_dat<long double, typeU>;

#endif//__QU_RADIAL_DAT_HPP__
