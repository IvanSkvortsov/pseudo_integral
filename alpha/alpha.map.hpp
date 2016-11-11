#ifndef __ALPHA_MAP_HPP__
#define __ALPHA_MAP_HPP__
#include"alpha.map.h"
#include<iostream>// cerr, endl
#include<cstdlib>// exit
#include<cstring>// memcpy

alpha_map::alpha_map(): mapping_struct(maximum),
	_M_mapping_t(), 
#ifdef  __ALPHA_MAP_LMAX
	_M_lmax(),
#endif
	_M_map1A(), _M_map1B(), _M_map1C(), _M_map2AB(), _M_map3ABC(),
	_M_map1A_d(), _M_map1B_d(), _M_map1C_d(), _M_map3ABC_d(),
	_mx1A_norm_size(), _mx1B_norm_size(), _mx2AB_norm_size(),
	_M_map1A_norm(), _M_map1B_norm(), _M_map2AB_norm(),
	_M_map1powkA(), _M_map1powkB(), _M_map2powk(), _M_map2slm(), __ALPHA_MAP_ITER_INIT_LIST
{}
alpha_map::alpha_map(alpha_map const & v):
	mapping_struct( v ), _M_mapping_t(v._M_mapping_t),
#ifdef  __ALPHA_MAP_LMAX
	_M_lmax( v._M_lmax ),
#endif
	_M_map1A( v._M_map1A ), _M_map1B( v._M_map1B ), _M_map1C( v._M_map1C ),
	_M_map1A_d( v._M_map1A_d ), _M_map1B_d( v._M_map1B_d ), _M_map1C_d( v._M_map1C_d ),
	_mx1A_norm_size( v._mx1A_norm_size ), _mx1B_norm_size( v._mx1B_norm_size ), _mx2AB_norm_size( v._mx2AB_norm_size ),
	_M_map1A_norm( v._M_map1A_norm ), _M_map1B_norm( v._M_map1B_norm ), _M_map2AB_norm( v._M_map2AB_norm ),
	_M_map2AB( v._M_map2AB ), _M_map3ABC( v._M_map3ABC ), _M_map3ABC_d( v._M_map3ABC_d ),
	_M_map1powkA( v._M_map1powkA ), _M_map1powkB( v._M_map1powkB ),
	_M_map2powk( v._M_map2powk ), _M_map2slm( v._M_map2slm ), __ALPHA_MAP_ITER_INIT_LIST {}

alpha_map & alpha_map::operator=(alpha_map const & v)
{
	if( this == &v )
		return *this;
	alpha_map tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	memset( &tmp, 0, sizeof(*this) );
	return *this;
}
void alpha_map::error(const char * _method, const char * _message)const
{
	std::cerr << "Error: [" << this << "] alpha_map::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl;
}

#ifdef  __ALPHA_MAP_DEBUG
void alpha_map::test_alp_map()
{
	for(int la = 0, __asize; la <= this->la_max(); ++la)
	{
		this->map1A_set_lx( la );
		__asize = this->map1A_size();
		this->map2AB_set_la( la );
		this->map3ABC_set_la( la );
		for(int lb = 0, __bsize; lb <= this->lb_max(); ++lb)
		{
			this->map1B_set_lx( lb );
			__bsize = this->map1B_size();
			this->map2AB_set_lb( lb );
			this->map3ABC_set_lb( lb );
			__alp_map_assert__( __asize * __bsize == this->map2AB_size() );
			for(int l = 0, __csize; l <= this->l_max(); ++l)
			{
				this->map1C_set_lx( l );
				__csize = this->map1C_size();
				this->map3ABC_set_l( l );
				__alp_map_assert__( __asize * __bsize * __csize == this->map3ABC_size() );
			}
		}
	}
}
#else
void alpha_map::test_alp_map(){}
#endif

const typename alpha_map::size_type
alpha_map::comp_size( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	__size += this->comp_size_map1X( __lmax );// map1A, map1B, map1C, map1A_d, map1B_d, map1C_d, map1A_norm, map1B_norm
	__size += this->comp_size_map2X( __lmax );// map2AB, map2AB_norm
	__size += this->comp_size_map3X( __lmax );// map3ABC, map3ABC_d
	switch( this->_mapping_t )
	{
	case minimum :
		break;
	case middle  :
		__size += this->comp_size_map1powkA( __lmax );
		break;
	case maximum :
		__size += this->comp_size_map1powkA( __lmax );
		__size += this->comp_size_map1powkB( __lmax );
		__size += this->comp_size_map2powk( __lmax ) * 2;// map2powk, map2slm
		break;
	default :
		this->error("comp_size", "undefined mapping type");
		std::cerr << "mapping type : " << this->_mapping_t << std::endl;
		exit(1);
	}
	return __size;
}
const typename alpha_map::size_type
alpha_map::comp_size_map1X( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
#ifdef  __ALPHA_MAP_LMAX
	__size += sizeof(_lmax_struct);
#endif
	__size += sizeof(mapping_struct);
	__size += sizeof(_alpsize_struct);
	const int __la_max = __lmax._la_max;
	const int __lb_max = __lmax._lb_max;
	const int __l_max = __lmax._l_max;
	const int __lso_max = __lmax._lso_max;
	// map1A
	__size += sizeof(size_struct<1>);
	__size += sizeof(_pos1_struct) * (__la_max + 1);
	// map1B
	__size += sizeof(size_struct<1>);
	__size += sizeof(_pos1_struct) * (__lb_max + 1);
	// map1C
	__size += sizeof(size_struct<1>);
	__size += sizeof(_pos1_struct) * (__l_max + __lso_max + 1);
	// map1A_d
	__size += sizeof(size_struct<1>);
	__size += sizeof(map_1d_type) * (__la_max + 1);
	// map1B_d
	__size += sizeof(size_struct<1>);
	__size += sizeof(map_1d_type) * (__lb_max + 1);
	// map1C_d
	__size += sizeof(size_struct<1>);
	__size += sizeof(map_1d_type) * (__l_max + __lso_max + 1);
	// map1A_norm
	__size += sizeof(size_struct<1>);
	__size += sizeof(int) * ((__la_max + 1) * (__la_max + 2) * (__la_max + 3)) / 6;
	// map1B_norm
	__size += sizeof(size_struct<1>);
	__size += sizeof(int) * ((__lb_max + 1) * (__lb_max + 2) * (__lb_max + 3)) / 6;
	return __size;
}
const typename alpha_map::size_type
alpha_map::comp_size_map2X( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	// map2AB
	__size += sizeof(size_struct<2>);
	__size += sizeof(_pos1_struct) * (__lmax._la_max + 1) * (__lmax._lb_max + 1);
	// map2AB_norm
	size_type __2_size = __lmax._la_max + 1;
	__2_size *= (__lmax._la_max + 2);
	__2_size *= (__lmax._la_max + 3);
	__2_size /= 6;
	__2_size *= (__lmax._lb_max + 1);
	__2_size *= (__lmax._lb_max + 2);
	__2_size *= (__lmax._lb_max + 3);
	__2_size /= 6;
	__size += sizeof(size_struct<2>);
	__size += sizeof(int) * __2_size;
	return __size;
}
const typename alpha_map::size_type
alpha_map::comp_size_map3X( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	// map3ABC
	__size += sizeof(size_struct<3>);
	__size += sizeof(_pos1_struct) * (__lmax._la_max + 1) * (__lmax._lb_max + 1) * (__lmax._l_max + __lmax._lso_max + 1);
	// map3ABC_d
	__size += sizeof(size_struct<3>);
	__size += sizeof(map_1d_type) * (__lmax._la_max + 1) * (__lmax._lb_max + 1) * (__lmax._l_max + __lmax._lso_max + 1);
	return __size;
}
// kA, kB, k
const typename alpha_map::size_type
alpha_map::comp_size_map1powkA( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	__size += sizeof(size_struct<1>);
	__size += sizeof(_pos1_struct) * (__lmax._la_max + 1);
	return __size;
}
const typename alpha_map::size_type
alpha_map::comp_size_map1powkB( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	__size += sizeof(size_struct<1>);
	__size += sizeof(_pos1_struct) * (__lmax._lb_max + 1);
	return __size;
}
const typename alpha_map::size_type
alpha_map::comp_size_map2powk( _lmax_struct const & __lmax )const
{
	size_type __size = 0;
	__size += sizeof(size_struct<2>);
	__size += sizeof(_pos1_struct) * (__lmax._la_max + 1) * (__lmax._lb_max + 1);
	return __size;
}

// use after initialization: %init_map
const typename alpha_map::size_type
alpha_map::M_mx2slm_size()const
{
	const size_type __la_max = this->la_max(), __lb_max = this->lb_max();
	size_type __size_ = this->map2slm_pos( __la_max, __lb_max );
	__size_ += this->map2slm_size( __la_max, __lb_max ) * this->map2AB_size(__la_max, __lb_max);
	//return __size_
	size_type __size = 0;
	_pos1_struct const * p_pos1 = this->_M_map2slm->data();
	alpha_map t1( *this );
	for(int la = 0; la < this->la_size(); ++la)
	{
		t1.map2slm_set_la( la );
		t1.map2AB_set_la( la );
		for(int lb = 0; lb < this->lb_size(); ++lb, ++p_pos1)
		{
			int sqr_lp1 = (la + lb + 1) * (la + lb + 1);
			t1.map2slm_set_lb( lb );
			t1.map2AB_set_lb( lb );
			__alp_map_assert__( t1.map2AB_size()  == this->map2AB_size( la, lb ) );
			__alp_map_assert__( t1.map2slm_size(la, lb) == sqr_lp1 );
			__alp_map_assert__( sqr_lp1 == t1.map2slm_size() );
			__alp_map_assert__( sqr_lp1 == p_pos1->_size );
			__alp_map_assert__( t1.map2slm_size() == p_pos1->_size );
			__size += (p_pos1->_size * this->map2AB_size(la, lb) );
		}
	}
	if( __size_ != __size )
	{
		this->error("M_mx2slm_size()", "size calculation failed");
		std::cerr << "exact calculated size : " << __size << std::endl;
		std::cerr << "revealed         size : " << __size_ << std::endl;
		exit(1);
	}
	return __size;
}
#define __ALPHA_MAP_WRITE_MAP1( __map1, __size1, __value_type )\
__cnvrt._void = ms.getcur();\
__map1 = __cnvrt._mx1;\
__map1->size() = (__size1);\
ms.seek( sizeof(size_struct<1>) + sizeof(__value_type) * __map1->size(), seek_dir::cur );

void alpha_map::write_map1X( memorystream & ms, _lmax_struct const & __lmax )
{
#ifdef  __ALPHA_MAP_LMAX
	this->_M_lmax = (_lmax_struct *)ms.getcur();
	*this->_M_lmax = __lmax;
	ms.seek( sizeof(_lmax_struct), seek_dir::cur );
#endif
	this->_M_alpsize = (_alpsize_struct *)ms.getcur();
	ms.seek( sizeof(_alpsize_struct), seek_dir::cur );
	this->_M_mapping_t = (mapping_enum *)ms.getcur();
	ms.seek( sizeof(mapping_enum), seek_dir::cur );

	{
	union{
		void * _void;
		matrix_cursor_1<_pos1_struct> * _mx1;
	} __cnvrt;
	// A, B, C
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1A, __lmax._la_max + 1, _pos1_struct );
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1B, __lmax._lb_max + 1, _pos1_struct );
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1C, __lmax._l_max + __lmax._lso_max + 1, _pos1_struct );
	}
	{
	union{
		void * _void;
		matrix_cursor_1<map_1d_type> * _mx1;
	} __cnvrt;
	// A_d, B_d, C_d
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1A_d, __lmax._la_max + 1, map_1d_type );
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1B_d, __lmax._lb_max + 1, map_1d_type );
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1C_d, __lmax._l_max + __lmax._lso_max + 1, map_1d_type );
	}
	{
	union{
		void * _void;
		matrix_cursor_1<int> * _mx1;
	} __cnvrt;
	// A_norm, B_norm
	const int __A_norm_size = ((__lmax._la_max + 1) * (__lmax._la_max + 2) * (__lmax._la_max + 3)) / 6;
	const int __B_norm_size = ((__lmax._lb_max + 1) * (__lmax._lb_max + 2) * (__lmax._lb_max + 3)) / 6;
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1A_norm, __A_norm_size, int );
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1B_norm, __B_norm_size, int );
	}
}

#define __ALPHA_MAP_WRITE_MAP2( __map2, __a_size, __b_size, __value_type )\
__cnvrt._void = ms.getcur();\
__map2 = __cnvrt._mx2;\
__map2->n() = (__a_size);\
__map2->m() = (__b_size);\
__map2->init_size();\
ms.seek( sizeof(size_struct<2>) + sizeof(__value_type) * __map2->size(), seek_dir::cur );

void alpha_map::write_map2X( memorystream & ms )
{
	// AB
	{
	union{
		void * _void;
		matrix_cursor_2<_pos1_struct> * _mx2;
	} __cnvrt;
	__ALPHA_MAP_WRITE_MAP2( this->_M_map2AB, this->_M_map1A->size(), this->_M_map1B->size(), _pos1_struct );
	}
	// AB_norm
	{
	union{
		void * _void;
		matrix_cursor_2<int> * _mx2;
	} __cnvrt;
	__ALPHA_MAP_WRITE_MAP2( this->_M_map2AB_norm, this->_M_map1A_norm->size(), this->_M_map1B_norm->size(), int );
	}
}
void alpha_map::write_map3X( memorystream & ms )
{
	union{
		void * _void;
		matrix_cursor_3<_pos1_struct> * _mx3pos1;
		matrix_cursor_3<map_1d_type> * _mx3dpos1;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map3ABC = __cnvrt._mx3pos1;
	this->_M_map3ABC->n() = this->_M_map1A->size();
	this->_M_map3ABC->m() = this->_M_map1B->size();
	this->_M_map3ABC->p() = this->_M_map1C->size();
	this->_M_map3ABC->init_size();
	ms.seek( sizeof(size_struct<3>) + sizeof(_pos1_struct) * this->_M_map3ABC->size(), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_map3ABC_d = __cnvrt._mx3dpos1;
	this->_M_map3ABC_d->n() = this->_M_map1A->size();
	this->_M_map3ABC_d->m() = this->_M_map1B->size();
	this->_M_map3ABC_d->p() = this->_M_map1C->size();
	this->_M_map3ABC_d->init_size();
	ms.seek( sizeof(size_struct<3>) + sizeof(map_1d_type) * this->_M_map3ABC_d->size(), seek_dir::cur );
}
void alpha_map::write_map1powkA( memorystream & ms )
{
	union{ void * _void; matrix_cursor_1<_pos1_struct> * _mx1; } __cnvrt;
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1powkA, this->_M_map1A->size(), _pos1_struct );
}
void alpha_map::write_map1powkB( memorystream & ms )
{
	union{ void * _void; matrix_cursor_1<_pos1_struct> * _mx1; } __cnvrt;
	__ALPHA_MAP_WRITE_MAP1( this->_M_map1powkB, this->_M_map1B->size(), _pos1_struct );
}
void alpha_map::write_map2powk( memorystream & ms )
{
	union{ void * _void; matrix_cursor_2<_pos1_struct> * _mx2; } __cnvrt;
	__ALPHA_MAP_WRITE_MAP2( this->_M_map2powk, this->_M_map1A->size(), this->_M_map1B->size(), _pos1_struct );
}
void alpha_map::write_map2slm( memorystream & ms )
{
	union{ void * _void; matrix_cursor_2<_pos1_struct> * _mx2; } __cnvrt;
	__ALPHA_MAP_WRITE_MAP2( this->_M_map2slm, this->_M_map1A->size(), this->_M_map1B->size(), _pos1_struct );
}
const typename alpha_map::size_type alpha_map::write_map( memorystream & ms, _lmax_struct const & __lmax )
{
	const size_type _seek_start = ms.tell();
	this->write_map1X( ms, __lmax );
	this->write_map2X( ms );
	this->write_map3X( ms );
	*this->_M_mapping_t = this->_mapping_t;
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		this->write_map1powkA( ms );
		break;
	case maximum :
		this->write_map1powkA( ms );
		this->write_map1powkB( ms );
		this->write_map2powk( ms );
		this->write_map2slm( ms );
		break;
	default :
		this->error("write_map", "undefinded mapping type");
		std::cerr << "current mapping type : " << this->_mapping_t << std::endl;
		std::cerr << "file    mapping type : " << *this->_M_mapping_t << std::endl;
		exit(1);
	}
#ifdef  __ALPHA_MAP_WRITESIZECHECK
	if( ms.tell() - _seek_start != this->comp_size( __lmax ) )
	{
		this->error("write_alp", "size error" );
		std::cerr << "written  size : " << ms.tell() - _seek_start << std::endl;
		std::cerr << "computed size : " << this->comp_size( __lmax ) << std::endl;
		exit(1);
	}
#endif
	return ms.tell() - _seek_start;
}

const typename alpha_map::size_type alpha_map::comp_mx1X_norm_size( const int __la_max, const int * _A_size)
{
	size_type __size = 0;
	for(int la = 0, ila_size = 1; la <= __la_max; ++la, ila_size += (la + 1) )
	{
		__size += _A_size[la] * ila_size;
	}
	return __size;
}

const typename alpha_map::size_type alpha_map::comp_mx2AB_norm_size( const int __la_max, const int __lb_max, const int * _A_size, const int * _B_size )
{
	size_type __size = 0;
	for(int la = 0, ila_size = 1, __b_size; la <= __la_max; ++la, ila_size += (la + 1) )
	{
		__b_size = 0;
		for(int lb = 0, ilb_size = 1; lb <= __lb_max; ++lb, ilb_size += (lb + 1) )
		{
			__b_size += _B_size[lb] * ilb_size;
		}
		__size += (_A_size[la] * ila_size * __b_size);
	}
	return __size;
}

void alpha_map::init_map1X_norm()
{
	size_type __pos;
	// A_norm
	__pos = 0;
	for(int la = 0, ila_size = 1; la <= this->la_max(); ++la, ila_size += (la + 1) )
	{
		this->map1A_norm_set_lx( la );
		this->map1A_set_lx( la );
		for(int ila = 0; ila < ila_size; ++ila )
		{
			this->map1A_norm_set_xyz( ila );
			this->map1A_norm_pos() = __pos;
			__pos += this->map1A_size();
		}
	}
	this->_mx1A_norm_size = __pos;
	//__alp_map_assert__( __pos == this->comp_mx1X_norm_size( this->la_max(), this->M_map1A_data() ) );
	// B_norm
	__pos = 0;
	for(int lb = 0, ilb_size = 1; lb <= this->lb_max(); ++lb, ilb_size += (lb + 1) )
	{
		this->map1B_norm_set_lx( lb );
		this->map1B_set_lx( lb );
		for(int ilb = 0; ilb < ilb_size; ++ilb )
		{
			this->map1B_norm_set_xyz( ilb );
			this->map1B_norm_pos() = __pos;
			__pos += this->map1B_size();
		}
	}
	this->_mx1B_norm_size = __pos;
	//__alp_map_assert__( __pos == this->comp_mx1X_norm_size( this->lb_max(), this->M_map1B_data() ) );
}

void alpha_map::init_map2AB_norm()
{
	size_type __pos = 0;
	for(int la = 0, ila_size = 1; la <= this->la_max(); ++la, ila_size += (la + 1) )
	{
		this->map2AB_norm_set_la( la );
		this->map2AB_set_la( la );
		for(int ila = 0; ila < ila_size; ++ila )
		{
			this->map2AB_norm_set_axyz( ila );
			for(int lb = 0, ilb_size = 1; lb <= this->lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map2AB_norm_set_lb( lb );
				this->map2AB_set_lb( lb );
				for(int ilb = 0; ilb < ilb_size; ++ilb )
				{
					this->map2AB_norm_set_bxyz( ilb );
					this->map2AB_norm_pos() = __pos;
					__pos += this->map2AB_size();
				}
			}
		}
	}
	this->_mx2AB_norm_size = __pos;
}

void alpha_map::init_map_norm()
{
	this->init_map1X_norm();
	this->init_map2AB_norm();
}
#define __ALPHA_MAP_INIT_MAP1X( __map1, _Xsize ) \
p_pos1 = __map1->data();\
__pos = 0;\
for(int i = 0; i < __map1->size(); ++i, ++p_pos1, ++_Xsize)\
{\
	p_pos1->_pos = __pos;\
	p_pos1->_size = *_Xsize;\
	__pos += *_Xsize;\
}

void alpha_map::init_map1X_d( const int * _A_dsize, const int * _B_dsize, const int * _C_dsize )
{
	size_type __pos;
	map_1d_type * p_pos1;
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1A_d, _A_dsize );
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1B_d, _B_dsize );
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1C_d, _C_dsize );
}
void alpha_map::init_map3X_d()
{
	map_1d_type * p_pos1 = this->_M_map3ABC_d->data();
	for(int la = 0, __pos = 0; la < this->_M_map3ABC_d->n(); ++la)
		for(int lb = 0; lb < this->_M_map3ABC_d->m(); ++lb)
			for(int l = 0; l < this->_M_map3ABC_d->p(); ++l, ++p_pos1)
			{
				p_pos1->_pos = __pos;
				p_pos1->_size = this->map1A_d_size( la );
				p_pos1->_size *= this->map1B_d_size( lb );
				p_pos1->_size *= this->map1C_d_size( l );
				__pos += p_pos1->_size;
			}
}
void alpha_map::init_mapd( const int * _A_dsize, const int * _B_dsize, const int * _C_dsize )
{
	this->init_map1X_d( _A_dsize, _B_dsize, _C_dsize );
	this->init_map3X_d();
}

void alpha_map::init_map1X( const int * _Asize, const int * _Bsize, const int * _Csize )
{
	size_type __pos;
	_pos1_struct * p_pos1;
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1A, _Asize );
	this->_M_alpsize->_a_size = __pos;
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1B, _Bsize );
	this->_M_alpsize->_b_size = __pos;
	__ALPHA_MAP_INIT_MAP1X( this->_M_map1C, _Csize );
	this->_M_alpsize->_c_size = __pos;
}
void alpha_map::init_map2X()
{
	_pos1_struct * p_pos1 = this->_M_map2AB->data();
	for(int la = 0, __pos = 0; la < this->_M_map2AB->n(); ++la)
		for(int lb = 0; lb < this->_M_map2AB->m(); ++lb, ++p_pos1)
		{
			p_pos1->_pos = __pos;
			p_pos1->_size = this->map1A_size(la) * this->map1B_size( lb );
			__pos += p_pos1->_size;
		}
}
void alpha_map::init_map3X()
{
	_pos1_struct * p_pos1 = this->_M_map3ABC->data();
	for(int la = 0, __pos = 0; la < this->_M_map3ABC->n(); ++la)
		for(int lb = 0; lb < this->_M_map3ABC->m(); ++lb)
			for(int l = 0; l < this->_M_map3ABC->p(); ++l, ++p_pos1)
			{
				p_pos1->_pos = __pos;
				p_pos1->_size = this->map1A_size( la );
				p_pos1->_size *= this->map1B_size( lb );
				p_pos1->_size *= this->map1C_size( l );
				__pos += p_pos1->_size;
			}
}
void alpha_map::init_map1powkA(const size_type __l0_max)
{
	_pos1_struct * p_pos1;
	__alp_map_assert__( this->_M_map1powkA != 0 );
	__alp_map_assert__( this->_M_map1powkA->data() != 0 );
	p_pos1 = this->_M_map1powkA->data();
	for(int la = 0, __pos = 0; la < this->la_size(); ++la, ++p_pos1)
	{
		p_pos1->_pos = __pos;
		p_pos1->_size = __l0_max + la;// _l_max + la, or _lb_max + 1 + la;
		__pos += (p_pos1->_size * this->map1A_size(la));
	}
}
void alpha_map::init_map1powkB()
{
	_pos1_struct * p_pos1;
	__alp_map_assert__( this->_M_map1powkB != 0 );
	__alp_map_assert__( this->_M_map1powkB->data() != 0 );
	__alp_map_assert__( this->_M_lmax != 0 );
	p_pos1 = this->_M_map1powkB->data();
	for(int lb = 0, __pos = 0; lb < this->lb_size(); ++lb, ++p_pos1)
	{
		p_pos1->_pos = __pos;
		p_pos1->_size = this->_M_lmax->_l_max + lb;
		__pos += (p_pos1->_size * this->map1B_size(lb));
	}
}
void alpha_map::init_map2powk()
{
	_pos1_struct * p_pos1;
	p_pos1 = this->_M_map2powk->data();
	for(int la = 0, __pos = 0; la < this->la_size(); ++la)
	{
		for(int lb = 0; lb < this->lb_size(); ++lb, ++p_pos1)
		{
			p_pos1->_pos = __pos;
			p_pos1->_size = la + lb + 1;
			__pos += (p_pos1->_size * this->map2AB_size(la, lb) );
		}
	}
}

void alpha_map::init_map2slm()
{
	_pos1_struct * p_pos1;
	p_pos1 = this->_M_map2slm->data();
	for(int la = 0, __pos = 0; la < this->la_size(); ++la)
	{
		for(int lb = 0; lb < this->lb_size(); ++lb, ++p_pos1)
		{
			p_pos1->_pos = __pos;
			p_pos1->_size = ( la + lb + 1);
			p_pos1->_size *= (la + lb + 1);// p_pos1->_size;
			__pos += (p_pos1->_size * this->map2AB_size(la, lb) );
		}
	}
}

void alpha_map::init_map( const int * _Asize, const int * _Bsize, const int * _Csize )
{
	this->init_map1X( _Asize, _Bsize, _Csize );
	this->init_map2X();
	this->init_map3X();
	switch( this->get_mapping() )
	{
	case minimum :
		this->init_it_min();
		break;
	case middle  :
		this->init_it_mid();
		this->init_map1powkA( (this->lb_max() + 1 > this->l_max() ? this->lb_max() + 1 : this->l_max()) );
		break;
	case maximum :
		this->init_it_max();
		this->init_map1powkA( this->l_max() );
		this->init_map1powkB();
		this->init_map2powk();
		this->init_map2slm();
		break;
	default :
		this->error("init_map", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}

#define __ALPHA_MAP_READ_MAP1( __map1, __value_type ) \
__cnvrt._void = ms.getcur();\
__map1 = __cnvrt._mx1;\
ms.seek( sizeof(size_struct<1>) + sizeof(__value_type) * __map1->size(), seek_dir::cur );

void alpha_map::read_map1X( memorystream & ms )
{
#ifdef  __ALPHA_MAP_LMAX
	this->_M_lmax = (_lmax_struct *)ms.getcur();
	ms.seek( sizeof(_lmax_struct), seek_dir::cur );
#endif
	this->_M_alpsize = (_alpsize_struct *)ms.getcur();
	ms.seek( sizeof(_alpsize_struct), seek_dir::cur );
	this->_M_mapping_t = (mapping_enum *)ms.getcur();
	ms.seek( sizeof(mapping_enum), seek_dir::cur );

	{
	union{
		void * _void;
		matrix_cursor_1<_pos1_struct> * _mx1;
	} __cnvrt;
	__ALPHA_MAP_READ_MAP1( this->_M_map1A, _pos1_struct );
	__ALPHA_MAP_READ_MAP1( this->_M_map1B, _pos1_struct );
	__ALPHA_MAP_READ_MAP1( this->_M_map1C, _pos1_struct );
	}

	{
	union{
		void * _void;
		matrix_cursor_1<map_1d_type> * _mx1;
	} __cnvrt;
	__ALPHA_MAP_READ_MAP1( this->_M_map1A_d, map_1d_type );
	__ALPHA_MAP_READ_MAP1( this->_M_map1B_d, map_1d_type );
	__ALPHA_MAP_READ_MAP1( this->_M_map1C_d, map_1d_type );
	}
	{
	union{
		void * _void;
		matrix_cursor_1<int> * _mx1;
	} __cnvrt;
	__ALPHA_MAP_READ_MAP1( this->_M_map1A_norm, int );
	__ALPHA_MAP_READ_MAP1( this->_M_map1B_norm, int );
	}
}
void alpha_map::read_map2X( memorystream & ms )
{
	{
	union{
		void * _void;
		matrix_cursor_2<_pos1_struct> * _mx2;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map2AB = __cnvrt._mx2;
	ms.seek( sizeof(size_struct<2>) + sizeof(_pos1_struct) * this->_M_map2AB->size(), seek_dir::cur );
	}
	{
	union{
		void * _void;
		matrix_cursor_2<int> * _mx2;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map2AB_norm = __cnvrt._mx2;
	ms.seek( sizeof(size_struct<2>) + sizeof(int) * this->_M_map2AB_norm->size(), seek_dir::cur );
	}
}
void alpha_map::read_map3X( memorystream & ms )
{
	{
	union{
		void * _void;
		matrix_cursor_3<_pos1_struct> * _mx3;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map3ABC = __cnvrt._mx3;
	ms.seek( sizeof(size_struct<3>) + sizeof(_pos1_struct) * this->_M_map3ABC->size(), seek_dir::cur );
	}
	{
	union{
		void * _void;
		matrix_cursor_3<map_1d_type> * _mx3;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map3ABC_d = __cnvrt._mx3;
	ms.seek( sizeof(size_struct<3>) + sizeof(map_1d_type) * this->_M_map3ABC_d->size(), seek_dir::cur );
	}
}
void alpha_map::read_map1powkA( memorystream & ms )
{
	union{ void * _void; matrix_cursor_1<_pos1_struct> * _mx1; } __cnvrt;
	__ALPHA_MAP_READ_MAP1( this->_M_map1powkA, _pos1_struct );
}
void alpha_map::read_map1powkB( memorystream & ms )
{
	union{ void * _void; matrix_cursor_1<_pos1_struct> * _mx1; } __cnvrt;
	__ALPHA_MAP_READ_MAP1( this->_M_map1powkB, _pos1_struct );
}
void alpha_map::read_map2powk( memorystream & ms )
{
	union{ void * _void; matrix_cursor_2<_pos1_struct> * _mx2; } __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map2powk = __cnvrt._mx2;
	ms.seek( sizeof(size_struct<2>) + sizeof(_pos1_struct) * this->_M_map2powk->size(), seek_dir::cur );
}
void alpha_map::read_map2slm( memorystream & ms )
{
	union{ void * _void; matrix_cursor_2<_pos1_struct> * _mx2; } __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_map2slm = __cnvrt._mx2;
	ms.seek( sizeof(size_struct<2>) + sizeof(_pos1_struct) * this->_M_map2slm->size(), seek_dir::cur );
}
const typename alpha_map::size_type alpha_map::read_map( memorystream & ms )
{
	const size_type _seek_start = ms.tell();
	this->read_map1X( ms );
	this->read_map2X( ms );
	this->read_map3X( ms );
	switch( this->get_mapping() )
	{
	case minimum :
		break;
	case middle  :
		this->read_map1powkA( ms );
		break;
	case maximum :
		this->read_map1powkA( ms );
		this->read_map1powkB( ms );
		this->read_map2powk( ms );
		this->read_map2slm( ms );
		break;
	default :
		this->error("read_map", "undefined mapping type");
		std::cerr << "current mapping type : " << this->_mapping_t << std::endl;
		std::cerr << "file    mapping type : " << *this->_M_mapping_t << std::endl;
		exit(1);
	}
	return ms.tell() - _seek_start;
}

#endif//__ALPHA_MAP_HPP__
