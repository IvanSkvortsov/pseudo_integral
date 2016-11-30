#ifndef __ALPHA_VAL_HPP__
#define __ALPHA_VAL_HPP__
#include"alpha.val.h"
#include"sf.math.h"// convert_float, sqrt
#include"vector.3d.h"
#include<cstring>// memcpy
#include"n.abc.h"

template<typename T, typename U> alpha_val<T,U>::alpha_val(): alpha_map(), _mx1C_nk_max(), _M_mx1A(), _M_mx1B(), _M_mx1C(), _M_mx1C_nk(),
	_M_mx1kA(), _M_mx1kB(), _M_mx1kB3(), _M_mx2k(), _M_mx2AB_exp(), _M_mx1A_norm(), _M_mx1B_norm(), _M_mx2AB_norm(), __ALPHA_VAL_ITER_INIT_LIST {}
template<typename T, typename U> alpha_val<T,U>::alpha_val( alpha_val<T,U> const & v ): alpha_map( v ), _mx1C_nk_max( v._mx1C_nk_max ),
	_M_mx1A( v._M_mx1A ), _M_mx1B( v._M_mx1B ), _M_mx1C( v._M_mx1C ), _M_mx1C_nk( v._M_mx1C_nk ),
	_M_mx1kA( v._M_mx1kA ), _M_mx1kB( v._M_mx1kB ), _M_mx1kB3( v._M_mx1kB3 ), _M_mx2k( v._M_mx2k ), _M_mx2AB_exp( v._M_mx2AB_exp ),
	_M_mx1A_norm( v._M_mx1A_norm ), _M_mx1B_norm( v._M_mx1B_norm), _M_mx2AB_norm( v._M_mx2AB_norm), __ALPHA_VAL_ITER_INIT_LIST {}
template<typename T, typename U> alpha_val<T,U> & alpha_val<T,U>::operator=( alpha_val<T,U> const & v )
{
	if( this == &v )
		return *this;
	alpha_val<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;
}

/*
template<typename T, typename U> const int alpha_val<T,U>::mx1C_nk_max()const
{
	__alp_val_assert__( this->_M_mx1C_nk != 0 );
	__alp_val_assert__( this->_M_mx1C_nk->data() != 0 );
	const int * __p_nk = this->_M_mx1C_nk->data();
	int __nk_max = 0;
	for(int i = 0; i < this->_M_mx1C_nk->size(); ++i, ++__p_nk)
		__nk_max = (*__p_nk > __nk_max ? *__p_nk : __nk_max ); 
	return __nk_max;
}
*/
template<typename T, typename U>
const typename alpha_val<T,U>::size_type alpha_val<T,U>::comp_size(_lmax_struct const & __lmax, _alpsize_struct const & __alpsize,
		const int * _Asize, const int * _Bsize )
//const typename alpha_val<T,U>::size_type alpha_val<T,U>::comp_size( _alpsize_struct const & __alpsize )const
{
	size_type __size = 0;

	__size += this->alpha_map::comp_size( __lmax );
	__size += this->comp_size_mx1T( __alpsize._a_size );// mx1A
	__size += this->comp_size_mx1T( __alpsize._b_size );// mx1B
	__size += this->comp_size_mx1T( __alpsize._c_size );// mx1C
	__size += this->comp_size_mx1( __alpsize._c_size, sizeof(int) );// mx1C_nk
	this->mx1A_norm_size() = this->alpha_map::comp_mx1X_norm_size( __lmax._la_max, _Asize );// mx1A_norm
	this->mx1B_norm_size() = this->alpha_map::comp_mx1X_norm_size( __lmax._lb_max, _Bsize );// mx1B_norm
	this->mx2AB_norm_size() = this->alpha_map::comp_mx2AB_norm_size( __lmax._la_max, __lmax._lb_max, _Asize, _Bsize );// mx2AB_norm
	__size += this->comp_size_mx1T( this->mx1A_norm_size() );// mx1A_norm
	__size += this->comp_size_mx1T( this->mx1B_norm_size() );// mx1B_norm
	__size += this->comp_size_mx1T( this->mx2AB_norm_size() );// mx2AB_norm
	__size += this->comp_size_mx1T( __alpsize._a_size * __alpsize._b_size );// mx2AB_exp
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		__size += this->comp_size_mx1T( __alpsize._a_size );// mx1kA
		break;
	case maximum :
		__size += this->comp_size_mx1T( __alpsize._a_size );// mx1kA
		__size += this->comp_size_mx1T( __alpsize._b_size );// mx1kB
		__size += this->comp_size_mx1T3( __alpsize._b_size );// mx1kB3
		__size += this->comp_size_mx1T( __alpsize._a_size * __alpsize._b_size );// mx2k
		break;
	default :
		this->error("comp_size", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return __size;
}

template<typename T, typename U>
const typename alpha_val<T,U>::size_type alpha_val<T,U>::comp_size_mx1( const size_type __mx_size, const int szof_type ) const
{
	size_type __size = 0;
	__size += sizeof(size_struct<1>);
	__size += szof_type * __mx_size;
	return __size;
}

template<typename T, typename U>
const typename alpha_val<T,U>::size_type alpha_val<T,U>::comp_size_mx1T( const size_type __mx_size) const
{
	return this->comp_size_mx1( __mx_size, sizeof(T) );
}

template<typename T, typename U>
const typename alpha_val<T,U>::size_type alpha_val<T,U>::comp_size_mx1T3( const size_type __mx_size) const
{
	return this->comp_size_mx1( __mx_size, sizeof(float_x3_type) );
}

template<typename T, typename U> const typename alpha_val<T,U>::size_type
alpha_val<T,U>::write_mxalp( memorystream & ms, _lmax_struct const & __lmax, _alpsize_struct const & __alpsize )
//alpha_val<T,U>::write_mxalp( memorystream & ms, _alpsize_struct const & __alpsize )
{
	const size_type _seek_start = ms.tell();

	this->alpha_map::write_map( ms, __lmax );
	this->write_mx1T( ms, this->_M_mx1A, __alpsize._a_size );
	this->write_mx1T( ms, this->_M_mx1B, __alpsize._b_size );
	this->write_mx1T( ms, this->_M_mx1C, __alpsize._c_size );
	this->write_mx1z( ms, this->_M_mx1C_nk, __alpsize._c_size );
	this->write_mx1T( ms, this->_M_mx1A_norm, this->mx1A_norm_size() );
	this->write_mx1T( ms, this->_M_mx1B_norm, this->mx1B_norm_size() );
	this->write_mx1T( ms, this->_M_mx2AB_norm, this->mx2AB_norm_size() );
	this->write_mx1T( ms, this->_M_mx2AB_exp, __alpsize._a_size * __alpsize._b_size );
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		this->write_mx1T( ms, this->_M_mx1kA, __alpsize._a_size );
		break;
	case maximum :
		this->write_mx1T( ms, this->_M_mx1kA, __alpsize._a_size );
		this->write_mx1T( ms, this->_M_mx1kB, __alpsize._b_size );
		this->write_mx1T3( ms, this->_M_mx1kB3, __alpsize._b_size );
		this->write_mx1T( ms, this->_M_mx2k, __alpsize._a_size * __alpsize._b_size );
		break;
	default :
		this->error("write_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return ms.tell() - _seek_start;
}

template<typename T, typename U>
void alpha_val<T,U>::write_mx1z( memorystream & ms, matrix_cursor_1<int> * & __mx1, const size_type __size1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	union{
		void * _void;
		matrix_cursor_1<int> * _matrix1_z;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_z;
	__mx1->size() = __size1;
	ms.seek( sizeof(size_struct<1>) + sizeof(int) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
void alpha_val<T,U>::write_mx1T( memorystream & ms, matrix_cursor_1<T> * & __mx1, const size_type __size1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	_matrixPtr __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_T;
	__mx1->size() = __size1;
	ms.seek( sizeof(size_struct<1>) + sizeof(T) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
void alpha_val<T,U>::write_mx1T3( memorystream & ms, matrix_cursor_1<float_x3_type> * & __mx1, const size_type __size1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	_matrixPtr __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_T3;
	__mx1->size() = __size1;
	ms.seek( sizeof(size_struct<1>) + sizeof(float_x3_type) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
const typename alpha_val<T,U>::size_type alpha_val<T,U>::read_mxalp( memorystream & ms )
{
	size_type _seek_start = ms.tell();
	
	this->alpha_map::read_map( ms );
	this->read_mx1T( ms, this->_M_mx1A );
	this->read_mx1T( ms, this->_M_mx1B );
	this->read_mx1T( ms, this->_M_mx1C );
	this->read_mx1z( ms, this->_M_mx1C_nk );
	this->read_mx1T( ms, this->_M_mx1A_norm );
	this->read_mx1T( ms, this->_M_mx1B_norm );
	this->read_mx1T( ms, this->_M_mx2AB_norm );
	this->read_mx1T( ms, this->_M_mx2AB_exp );
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		this->read_mx1T( ms, this->_M_mx1kA );
		break;
	case maximum :
		this->read_mx1T( ms, this->_M_mx1kA );
		this->read_mx1T( ms, this->_M_mx1kB );
		this->read_mx1T3( ms, this->_M_mx1kB3 );
		this->read_mx1T( ms, this->_M_mx2k );
		break;
	default :
		this->error("read_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return ms.tell() - _seek_start;
}

template<typename T, typename U>
void alpha_val<T,U>::read_mx1z( memorystream & ms, matrix_cursor_1<int> * & __mx1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	union{
		void * _void;
		matrix_cursor_1<int> * _matrix1_z;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_z;
	ms.seek( sizeof(size_struct<1>) + sizeof(int) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
void alpha_val<T,U>::read_mx1T( memorystream & ms, matrix_cursor_1<T> * & __mx1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	_matrixPtr __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_T;
	ms.seek( sizeof(size_struct<1>) + sizeof(T) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
void alpha_val<T,U>::read_mx1T3( memorystream & ms, matrix_cursor_1<float_x3_type> * & __mx1)
{
	//_matrixPtr __cnvrt = {ms.getcur()};
	_matrixPtr __cnvrt;
	__cnvrt._void = ms.getcur();
	__mx1 = __cnvrt._matrix1_T3;
	ms.seek( sizeof(size_struct<1>) + sizeof(float_x3_type) * __mx1->size(), seek_dir::cur );
}

template<typename T, typename U>
void alpha_val<T,U>::init_mx1C_nk( int const * _C_nk )
{
	if( this->c_size() != this->_M_mx1C_nk->size() )
	{
		this->error("init_mx1C_nk", "size error" );
		std::cerr << "mx1.size : " << this->_M_mx1C_nk->size() << std::endl;
		std::cerr << "  __size : " << this->c_size() << std::endl;
		exit(1);
	}
	int * p = this->_M_mx1C_nk->data();
	this->_mx1C_nk_max = 0;
	for(int i = 0; i < this->_M_mx1C_nk->size(); ++i, ++p)
	{
		*p = _C_nk[i];
		this->_mx1C_nk_max = ( *p > this->_mx1C_nk_max ? *p : this->_mx1C_nk_max );
	}
}

template<typename T> inline static void xyz_exp_aR2_norm( const int lx, const int x, const int y, const int z, T const & alp, T & __norm )
{// 2^(lx) * alp^(lx/2) * (2 * alp)^(3/4) * (Pi)^(-1/4) * ((2x - 1)!! (2y - 1)!! (2z - 1)!!)^(-1/2)
	static const T i_sqrt_sqrt_pi = sqrt( math::numeric_constants<T>::i_sqrt_pi );
	__norm = 1;
	if( lx%2 )
		__norm *= sqrt( alp );
	__norm *= math::pown<T>( alp, lx/2 );

	T __c_alp = T(alp);// P.S. (2 * alp)^(3/4)
	__c_alp *= 2;// 2 * alp
	__c_alp *= __c_alp;// 4 * alp^2
	__c_alp *= alp;// 4 * alp^3
	__c_alp *= 2;// 8 * alp^3
	__c_alp = sqrt( __c_alp );// (2 * alp)^(3/2)
	__c_alp = sqrt( __c_alp );// (2 * alp)^(3/4)

	__norm *= __c_alp;
	__norm *= (1u << lx);// 2^(lx)

	__norm *= 2;
	__norm *= i_sqrt_sqrt_pi;// Pi^(-0.25)

	T __c_xyz = math::fact_2n1_n( x-1 );
	__c_xyz  *= math::fact_2n1_n( y-1 );
	__c_xyz  *= math::fact_2n1_n( z-1 );
	__c_xyz = sqrt( __c_xyz );

	__norm /= __c_xyz;
	return;
}

template<typename T, typename U> void alpha_val<T,U>::init_norm( U const * _A_alp, U const * _B_alp )
{
	this->init_mx1X_norm( _A_alp, _B_alp );
	this->init_mx2AB_norm();
}
template<typename T, typename U> void alpha_val<T,U>::init_mx1X_norm( U const * _A_alp, U const * _B_alp )
{
	U __norm;
	for(int la = 0, ila_size = 1; la <= this->la_max(); ++la, ila_size += (la + 1) )
	{
		this->map1A_norm_set_lx( la );
		this->map1A_set_lx( la );
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila)
		{
			i2abc( la, ila, ax, ay ); az = la - ax - ay;
			__alp_val_assert__( (ax + ay + az) == la );
			__alp_val_assert__( ax >= 0 );
			__alp_val_assert__( ay >= 0 );
			__alp_val_assert__( az >= 0 );
			this->map1A_norm_set_xyz( ila );
			for(int ia = 0, it = this->map1A_pos(); ia < this->map1A_size(); ++ia, ++it)
			{
				this->map1A_norm_set_ix( ia );
				this->mx1A_norm_set_idx();
				xyz_exp_aR2_norm( la, ax, ay, az, _A_alp[it], __norm );
				this->mx1A_norm() = math::convert_float<T,U>( __norm );
			}
		}
	}
	for(int lb = 0, ilb_size = 1; lb <= this->lb_max(); ++lb, ilb_size += (lb + 1) )
	{
		this->map1B_norm_set_lx( lb );
		this->map1B_set_lx( lb );
		for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb)
		{
			i2abc( lb, ilb, bx, by ); bz = lb - bx - by;
			__alp_val_assert__( (bx + by + bz) == lb );
			__alp_val_assert__( bx >= 0 );
			__alp_val_assert__( by >= 0 );
			__alp_val_assert__( bz >= 0 );
			this->map1B_norm_set_xyz( ilb );
			for(int ia = 0, it = this->map1B_pos(); ia < this->map1B_size(); ++ia, ++it)
			{
				this->map1B_norm_set_ix( ia );
				this->mx1B_norm_set_idx();
				xyz_exp_aR2_norm( lb, bx, by, bz, _B_alp[it], __norm );
				this->mx1B_norm() = math::convert_float<T,U>( __norm );
			}
		}
	}
}
template<typename T, typename U> void alpha_val<T,U>::init_mx2AB_norm()
{
	for(int la = 0, ila_size = 1; la <= this->la_max(); ++la, ila_size += (la + 1) )
	{
		this->map1A_norm_set_lx( la );
		this->map1A_set_lx( la );
		this->map2AB_norm_set_la( la );
		for(int ila = 0; ila < ila_size; ++ila)
		{
			this->map1A_norm_set_xyz( ila );
			this->map2AB_norm_set_axyz( ila );
			for(int lb = 0, ilb_size = 1; lb <= this->lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map1B_norm_set_lx( lb );
				this->map1B_set_lx( lb );
				this->map2AB_norm_set_lb( lb );
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb)
				{
					this->map1B_norm_set_xyz( ilb );
					this->map2AB_norm_set_bxyz( ilb );
					for(int ia = 0; ia < this->map1A_size(); ++ia )
					{
						this->map1A_norm_set_ix( ia );
						this->mx1A_norm_set_idx();
						this->map2AB_norm_set_ia( ia );
						for(int ib = 0; ib < this->map1B_size(); ++ib )
						{
							this->map1B_norm_set_ix( ib );
							this->mx1B_norm_set_idx();
							this->map2AB_norm_set_ib( ib );
							this->mx2AB_norm_set_idx();
							this->mx2AB_norm()  = this->mx1A_norm();
							this->mx2AB_norm() *= this->mx1B_norm();
						}
					}
				}
			}
		}
	}
}

template<typename T, typename U>
void alpha_val<T,U>::init_exp( U const * _A_alp, U const * _B_alp, U const & sqr_CA, U const & sqr_CB )
{
	const int __a_size = this->M_mx1A_size();
	const int __b_size = this->M_mx1B_size();
	U __mx1A_exp[ __a_size ], __mx1B_exp[ __b_size ], __xA[ __a_size ], __xB[ __b_size ], * ptr;
	for(int i = 0; i < __a_size; ++i )
	{
		ptr = __xA + i;
		*ptr = _A_alp[i]; *ptr *= sqr_CA; *ptr = -(*ptr);
		__mx1A_exp[i] = exp( *ptr );
	}
	for(int i = 0; i < __b_size; ++i )
	{
		ptr = __xB + i;
		*ptr = _B_alp[i]; *ptr *= sqr_CB; *ptr = -(*ptr);
		__mx1B_exp[i] = exp( *ptr );
	}
	U exp_x, x;
	static T const LOG_MAX_T = log( std::numeric_limits<T>::max() );
	//int iter = 0, IT_MAX = 100;
	for(int la = 0; la <= this->alpha_map::la_max(); ++la)
	{
		this->alpha_map::map1A_set_lx( la );
		this->alpha_map::map2AB_set_la( la );
		for(int lb = 0; lb <= this->alpha_map::lb_max(); ++lb)
		{
			this->alpha_map::map1B_set_lx( lb );
			this->alpha_map::map2AB_set_lb( lb );
			for(int ia = 0, it_a = this->alpha_map::map1A_pos(); ia < this->alpha_map::map1A_size(); ++ia, ++it_a)
			{
				this->alpha_map::map2AB_set_ia( ia );
				for(int ib = 0, it_b = this->alpha_map::map1B_pos(); ib < this->alpha_map::map1B_size(); ++ib, ++it_b)
				{
					this->alpha_map::map2AB_set_ib( ib );
					this->mx2AB_exp_set_idx();
					//tmp  = __mx1A_exp[it_a];
					//tmp *= __mx1B_exp[it_b];
					x  = __xA[it_a];
					x += __xB[it_b];
					if( -x > LOG_MAX_T )
						this->mx2AB_exp() = 0;
					else
						this->mx2AB_exp() = exp( math::convert_float<T,U>( x ) );
#if 0
					if( iter++ >= IT_MAX )
						continue;
					if( iter%50 == 1 )
					std::clog << std::setw(4) << "la" << std::setw(4) << "lb" <<
						std::setw(4) << "ia" << std::setw(4) << "ib" <<
						std::setw(24) << "xa" <<
						std::setw(24) << "xb" <<
						std::setw(24) << "x" <<
						std::setw(24) << "exp( x )" <<
						std::setw(6) << "idx" <<
						std::endl;
					std::clog << std::setw(4) << la << std::setw(4) << lb <<
						std::setw(4) << ia << std::setw(4) << ib <<
						std::setw(24) << std::setprecision(15) << std::scientific << __xA[it_a] <<
						std::setw(24) << std::setprecision(15) << std::scientific << __xB[it_b] <<
						std::setw(24) << std::setprecision(15) << std::scientific << x <<
						std::setw(24) << std::setprecision(15) << std::scientific << this->mx2AB_exp() <<
						std::setw(6) << &(this->mx2AB_exp())-this->M_mx2AB_exp_data() <<
						std::endl;
#endif
				}
			}
		}
	}
}

template<typename T, typename U>
void alpha_val<T,U>::init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const * CA, U const * CB )
{
	this->init_mx1_alp( _A_alp, _B_alp, _C_alp );
	vector_3d<U> _CA( CA ), _CB( CB );
	U CA_len, CB_len, CAxCB;
	CA_len = _CA.len();
	switch( this->get_mapping() )
	{
	case minimum :
	case middle  :
		this->init_mx1X( this->_M_mx1kA, _A_alp, this->a_size(), CA_len );
		break;
	case maximum :
		CB_len = _CB.len();
		CAxCB = _CA.scalar( _CB );
		this->init_mx1X( this->_M_mx1kA, _A_alp, this->a_size(), CA_len );
		this->init_mx1X( this->_M_mx1kB, _B_alp, this->b_size(), CB_len );
		this->init_mx1kB3( CB, _B_alp );
		this->init_mx2k( _A_alp, _B_alp, CA_len, CB_len, CAxCB );
		break;
	default :
		this->error("init_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const & CA, U const & CB, U const & CACB )
{
	this->init_mx1_alp( _A_alp, _B_alp, _C_alp );
	switch( this->get_mapping() )
	{
	case minimum :
	case middle  :
		this->error("init_mxalp( U const * , U const * , U const * , U const & , U const & , U const & )",\
				"too many arguments for minimum/middle mapping");
		exit(1);
	case maximum :
		this->init_mx1X( this->_M_mx1kA, _A_alp, this->a_size(), CA );
		this->init_mx1X( this->_M_mx1kB, _B_alp, this->b_size(), CB );
		this->init_mx2k( _A_alp, _B_alp, CA, CB, CACB );
		break;
	default :
		this->error("init_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const & CA )
{
	this->init_mx1_alp( _A_alp, _B_alp, _C_alp );
	switch( this->get_mapping() )
	{
	case minimum :
		this->error("init_mxalp( U const * , U const * , U const * , U const & )", "too many arguments for minimum mapping");
		exit(1);
	case middle  :
		this->init_mx1X( this->_M_mx1kA, _A_alp, this->a_size(), CA );
		break;
	case maximum :
		this->error("init_mxalp( U const * , U const * , U const * , U const & )", "too few arguments for maximum mapping");
		exit(1);
	default :
		this->error("init_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mx1_alp( U const * _A_alp, U const * _B_alp, U const * _C_alp )
{
	this->init_mx1X( this->_M_mx1A, _A_alp, this->a_size() );
	this->init_mx1X( this->_M_mx1B, _B_alp, this->b_size() );
	this->init_mx1X( this->_M_mx1C, _C_alp, this->c_size() );
}
template<typename T, typename U>
void alpha_val<T,U>::init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp )
{
	this->init_mx1_alp( _A_alp, _B_alp, _C_alp );
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		this->error("init_mxalp( U const * , U const * , U const * , U const & )", "too few arguments for middle mapping");
		exit(1);
	case maximum :
		this->error("init_mxalp( U const * , U const * , U const * , U const & )", "too few arguments for maximum mapping");
		exit(1);
	default :
		this->error("init_mxalp", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mx2k( U const * _A_alp, U const * _B_alp, U const & CA, U const & CB, U const & CACB )
{
	pointer p = this->_M_mx2k->data();
	U _t(0), _ta(0), _tb(0), _tc(0);
	U const * _A_alp_i, * _B_alp_i;
	// k = sqrt( kA * kA + 2 * (kA, kB) + kB * kB )
	for(int la = 0; la < this->la_size(); ++la)
	{
		this->map1A_set_lx( la );
		_A_alp_i = _A_alp + this->map1A_pos();
		for(int lb = 0; lb < this->lb_size(); ++lb)
		{
			this->map1B_set_lx( lb );
			_B_alp_i = _B_alp + this->map1B_pos();
			for(int i = 0; i < this->map1A_size(); ++i)
			{
				_ta = CA;
				_ta *= _A_alp_i[i];
				_ta *= 2;
				_ta *= _ta;// kA * kA

				for(int j = 0; j < this->map1B_size(); ++j, ++p)
				{
					_tc = CACB;
					_tc *= _A_alp_i[i];
					_tc *= _B_alp_i[j];
					_tc *= 8;// 2 * (kA, kB) - scalar product of 3d vector

					_tb = CB;
					_tb *= _B_alp_i[j];
					_tb *= 2;
					_tb *= _tb;// kB * kB

					_t = _ta;
					_t += _tb;
					_t += _tc;
					_t = sqrt( _t );
					*p = math::convert_float<T,U>( _t );
				}
			}
		}
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mx1kB3( U const * CX, U const * X_alp )
{
	// mx1kB3 - array of 3d-vectors
	const U m2_CX[3] = {U(-2*CX[0]), U(-2*CX[1]), U(-2*CX[2])};
	U kx;
	for(int lx = 0; lx <= this->lb_max(); ++lx)
	{
		this->map1B_set_lx( lx );
		for(int i = 0; i < this->map1B_size(); ++i )
		{
			this->map1B_set_ix( i );
			__alp_val_assert__( this->map1B_idx() < this->M_mx1B_size() );
			__alp_val_assert__( this->map1B_idx() == this->map1B_pos() + i );
			this->mx1kB3_set_idx();
			for(int k = 0; k < 3; ++k)
			{
				kx = m2_CX[k];
				kx *= X_alp[this->map1B_idx()];
				this->mx1kB3()[k] = math::convert_float<T,U>( kx );
			}
		}
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mx1X( matrix_cursor_1<T> * __mx1, U const * __X_alp, const size_type __size, U const & CX )
{
	if( __size != __mx1->size() )
	{
		this->error("init_mx1", "size error" );
		std::cerr << "mx1.size : " << __mx1->size() << std::endl;
		std::cerr << "  __size : " << __size << std::endl;
		exit(1);
	}
	pointer p = __mx1->data();
	U tmp(0);
	for(int i = 0; i < __mx1->size(); ++i, ++p, ++__X_alp)
	{
		tmp = CX;
		tmp *= *__X_alp;
		*p = math::convert_float<T,U>( tmp );
	}
}
template<typename T, typename U>
void alpha_val<T,U>::init_mx1X( matrix_cursor_1<T> * __mx1, U const * __X_alp, const size_type __size )
{
	if( __size != __mx1->size() )
	{
		this->error("init_mx1", "size error" );
		std::cerr << "mx1.size : " << __mx1->size() << std::endl;
		std::cerr << "  __size : " << __size << std::endl;
		exit(1);
	}
	pointer p = __mx1->data();
	for(int i = 0; i < __mx1->size(); ++i, ++p, ++__X_alp)
		*p = math::convert_float<T,U>( *__X_alp );
}

#define __ALPHA_VAL_SPEC( typeU )\
template class alpha_val<float, typeU>;\
template class alpha_val<double, typeU>;\
template class alpha_val<long double, typeU>;

#endif//__ALPHA_VAL_HPP__
