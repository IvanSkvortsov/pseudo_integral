#ifndef __ALPHA_MAP_H__
#define __ALPHA_MAP_H__
#include"memorystream.h"
#include"matrix.cursor.h"
#include"mapping.t.h"
#include"alpha.siz.h"
#include"assert.config.h"

#if  __ALPHA_MAP_ASSERT
  #define __ALPHA_MAP_DEBUG
#endif
#ifdef  __ALPHA_MAP_DEBUG
  #include<cassert>
  #define __alp_map_assert__( arg ) assert( (arg) )
#else
  #define __alp_map_assert__( arg )
#endif

// map1A, map1B, map1C
#define __ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1X ) \
inline const size_type & M_##map1X##_size()const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	return this->_M_##map1X->size();\
}\
inline const size_type & map1X##_size(int lx)const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	return this->_M_##map1X->operator[]( lx )._size;\
}\
inline const size_type & map1X##_pos(int lx)const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	__alp_map_assert__( lx >= 0 && lx < this->_M_##map1X->size() );\
	return this->_M_##map1X->operator[]( lx )._pos;\
}\
inline void map1X##_set_lx( int lx )\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	__alp_map_assert__( this->_M_##map1X->data() != 0 );\
	__alp_map_assert__( lx >= 0 && lx < this->_M_##map1X->size() );\
	this->_##map1X##_it = this->_M_##map1X->data() + lx;\
}\
inline void map1X##_set_ix( int ix )\
{\
	__alp_map_assert__( ix >= 0 && ix < this->map1X##_size() );\
	this->_##map1X##_it_ix = this->map1X##_pos() + ix;\
}\
inline const size_type map1X##_idx ( int i )const\
{\
	return this->map1X##_pos() + i;\
}\
inline const size_type map1X##_idx ( int lx, int i )const{ return this->map1X##_pos( lx ) + i;}\
inline const size_type & map1X##_idx ()const{ return this->_##map1X##_it_ix;}\
inline const size_type & map1X##_pos ()const\
{\
	__alp_map_assert__( this->_##map1X##_it != 0 );\
	return this->_##map1X##_it->_pos;\
}\
inline const size_type & map1X##_size()const\
{\
	__alp_map_assert__( this->_##map1X##_it != 0 );\
	return this->_##map1X##_it->_size;\
}
// map1powkA, map1powkB
#define __ALPHA_MAP_INTERFACE_MAP1_POS1( map1X ) \
inline const size_type & M_##map1X##_size()const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	return this->_M_##map1X->size();\
}\
inline const _pos1_struct * M_##map1X##_data()const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	return this->_M_##map1X->data();\
}\
inline _pos1_struct * M_##map1X##_data()\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	return this->_M_##map1X->data();\
}\
inline const _pos1_struct & M_##map1X(int i)const\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	__alp_map_assert__( i >= 0 && i < this->_M_##map1X->size() );\
	return this->_M_##map1X->operator[]( i );\
}\
inline _pos1_struct & M_##map1X(int i)\
{\
	__alp_map_assert__( this->_M_##map1X != 0 );\
	__alp_map_assert__( i >= 0 && i < this->_M_##map1X->size() );\
	return this->_M_##map1X->operator[]( i );\
}\
\
inline const size_type & map1X##_size(int lx)const{ return this->M_##map1X( lx )._size;}\
inline const size_type & map1X##_pos (int lx)const{ return this->M_##map1X( lx )._pos;}\
inline void map1X##_set_lx( int lx )\
{\
	__alp_map_assert__( this->_M_##map1X->data() != 0 );\
	__alp_map_assert__( lx >= 0 && lx < this->_M_##map1X->size() );\
	this->_##map1X##_it = this->M_##map1X##_data() + lx;\
}\
inline void map1X##_set_ix( int i  )\
{\
	this->_##map1X##_it_ix = this->map1X##_pos() + i * this->map1X##_size();\
}\
inline const size_type map1X##_idx ( int i )const{ return this->map1X##_pos() + i * this->map1X##_size(); }\
inline const size_type map1X##_idx ( int lx, int i )const{ return this->map1X##_pos( lx ) + i * this->map1X##_size( lx ); }\
inline const size_type & map1X##_idx ()const{ return this->_##map1X##_it_ix; }\
inline const size_type & map1X##_pos ()const\
{\
	__alp_map_assert__( this->_##map1X##_it != 0 );\
	return this->_##map1X##_it->_pos;\
}\
inline const size_type & map1X##_size()const\
{\
	__alp_map_assert__( this->_##map1X##_it != 0 );\
	return this->_##map1X##_it->_size;/* maximum power of kA/kB -eq %map1powkX_size() - 1 */\
}
// map2AB
#define __ALPHA_MAP_INTERFACE_MAP2_POS1_AB( map2X ) \
inline const size_type & M_##map2X##_n()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->n();\
}\
inline const size_type & M_##map2X##_m()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->m();\
}\
inline const size_type & M_##map2X##_size()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->size();\
}\
\
inline const size_type & map2X##_size(int la, int lb)const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	return this->_M_##map2X->operator()(la, lb)._size;\
}\
inline const size_type & map2X##_pos (int la, int lb)const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	return this->_M_##map2X->operator()(la, lb)._pos;\
}\
inline void map2X##_set_la( int la)\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( this->_M_##map2X->data() != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	this->_##map2X##_it_la = this->_M_##map2X->data() + la * this->M_##map2X##_m();\
}\
inline void map2X##_set_lb( int lb)\
{\
	__alp_map_assert__( this->_##map2X##_it_la != 0 );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	this->_##map2X##_it_lb = this->_##map2X##_it_la + lb;\
}\
inline void map2X##_set_ia( int i ){ this->_##map2X##_it_ia = this->map2X##_pos() + this->map1B_size() * i; }\
inline void map2X##_set_ib( int i ){ this->_##map2X##_it_ib = this->_##map2X##_it_ia + i; }\
inline const size_type map2X##_idx ( int i, int j )const{ return this->map2X##_pos() + i * this->map1B_size() + j; }\
inline const size_type map2X##_idx ( int la, int i, int lb, int j )const{ return this->map2X##_pos(la, lb) + i * this->map1B_size() + j; }\
inline const size_type & map2X##_idx ()const{ return this->_##map2X##_it_ib; }\
inline const size_type & map2X##_pos ()const\
{\
	__alp_map_assert__( this->_##map2X##_it_lb != 0 );\
	return this->_##map2X##_it_lb->_pos;\
}\
inline const size_type & map2X##_size()const\
{\
	__alp_map_assert__( this->_##map2X##_it_lb != 0 );\
	return this->_##map2X##_it_lb->_size;\
}
// map2powk, map2slm
#define __ALPHA_MAP_INTERFACE_MAP2_POS1( map2X ) \
inline const size_type & M_##map2X##_n()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->n();\
}\
inline const size_type & M_##map2X##_m()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->m();\
}\
inline const size_type & M_##map2X##_size()const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	return this->_M_##map2X->size();\
}\
\
inline const size_type & map2X##_size(int la, int lb)const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	return this->_M_##map2X->operator()(la, lb)._size;\
}\
inline const size_type & map2X##_pos (int la, int lb)const\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	return this->_M_##map2X->operator()(la, lb)._pos;\
}\
inline void map2X##_set_la( int la)\
{\
	__alp_map_assert__( this->_M_##map2X != 0 );\
	__alp_map_assert__( this->_M_##map2X->data() != 0 );\
	__alp_map_assert__( la >= 0 && la < this->_M_##map2X->n() );\
	this->_##map2X##_it_la = this->_M_##map2X->data() + la * this->M_##map2X##_m();\
}\
inline void map2X##_set_lb( int lb)/* 2. suppose %map1B_set_l( lb ) to be set */\
{\
	__alp_map_assert__( this->_##map2X##_it_la != 0 );\
	__alp_map_assert__( lb >= 0 && lb < this->_M_##map2X->m() );\
	this->_##map2X##_it_lb = this->_##map2X##_it_la + lb;\
	/*1.1 this->_##map2X##_B_size = this->map1B_size(lb);*/\
	/*1.2 this->_##map2X##_BC_size = this->map2X##_size() * this->_##map2X##_B_size;*/\
	/*2.  this->_##map2X##_bcsize = this->map2X##_size() * this->map1B_size();*/\
}\
inline void map2X##_set_ia( int ia)\
{\
	/*2. this->_##map2X##_it_ia = this->map2X##_pos() + ia * this->_##map2X##_bcsize;*/\
	this->_##map2X##_it_ia = ia * this->map1B_size();\
}\
inline void map2X##_set_ib( int ib)\
{\
	/*2. this->_##map2X##_it_ib = this->_##map2X##_it_ia + ib * this->map2X##_size();*/\
	this->_##map2X##_it_ib = (this->_##map2X##_it_ia + ib) * this->map2X##_size();\
	this->_##map2X##_it_ib += this->map2X##_pos();\
}\
inline const size_type & map2X##_idx ()const{ return this->_##map2X##_it_ib;}\
inline const size_type & map2X##_pos ()const\
{\
	__alp_map_assert__( this->_##map2X##_it_lb != 0 );\
	return this->_##map2X##_it_lb->_pos;\
}\
inline const size_type & map2X##_size()const\
{\
	__alp_map_assert__( this->_##map2X##_it_lb != 0 );\
	return this->_##map2X##_it_lb->_size;\
}\
inline const size_type map2X##_idx(int ia, int ib)const/* supppose %map1B_set_l( lb ) to be set */\
{\
	return this->map2X##_pos() + (ia * this->map1B_size() + ib) * this->map2X##_size();\
}\
inline const size_type map2X##_idx(int la, int ia, int lb, int ib)const\
{\
	return this->map2X##_pos(la, lb) + (ia * this->map1B_size(lb) + ib) * this->map2X##_size(la, lb);\
}
// map3ABC, map3ABC_d
#define __ALPHA_MAP_INTERFACE_MAP3_POS1_ABC( map3X ) \
inline const size_type & M_##map3X##_n()const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->n();\
}\
inline const size_type & M_##map3X##_m()const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->m();\
}\
inline const size_type & M_##map3X##_p()const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->p();\
}\
inline const size_type & M_##map3X##_size()const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->size();\
}\
\
inline const size_type & map3X##_size(int la, int lb, int l)const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->operator()(la, lb, l)._size;\
}\
inline const size_type & map3X##_pos (int la, int lb, int l)const\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	return this->_M_##map3X->operator()(la, lb, l)._pos;\
}\
inline void map3X##_set_la( const size_type la)\
{\
	__alp_map_assert__( this->_M_##map3X != 0 );\
	__alp_map_assert__( this->_M_##map3X->data() != 0 );\
	this->_##map3X##_it_la = this->_M_##map3X->data() + la * this->M_##map3X##_m() * this->M_##map3X##_p();\
}\
inline void map3X##_set_lb( const size_type lb)\
{\
	__alp_map_assert__( this->_##map3X##_it_la != 0 );\
	this->_##map3X##_it_lb = this->_##map3X##_it_la + lb * this->M_##map3X##_p();\
}\
inline void map3X##_set_l( const size_type l)\
{\
	__alp_map_assert__( this->_##map3X##_it_lb != 0 );\
	this->_##map3X##_it_l = this->_##map3X##_it_lb + l;\
}\
inline const size_type & map3X##_pos ()const\
{\
	__alp_map_assert__( this->_##map3X##_it_l != 0 );\
	return this->_##map3X##_it_l->_pos;\
}\
inline const size_type & map3X##_size()const\
{\
	__alp_map_assert__( this->_##map3X##_it_l != 0 );\
	return this->_##map3X##_it_l->_size;\
}

// A_norm, B_norm : Interface
#define __ALPHA_MAP_INTERFACE_MAP1X_NORM( X_norm )\
inline const size_type M_map1##X_norm##_size()const{ __alp_map_assert__( this->_M_map1##X_norm != 0 ); return this->_M_map1##X_norm->size(); }\
inline const int * M_map1##X_norm##_data()const\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->data();\
}\
inline int const * M_map1##X_norm##_data( int i )const\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->data() + i;\
}\
inline int * M_map1##X_norm##_data( int i )\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->data() + i;\
}\
inline int * M_map1##X_norm##_data()\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->data();\
}\
inline int const & M_map1##X_norm(int i)const\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->operator[]( i );\
}\
inline int & M_map1##X_norm(int i)\
{\
	__alp_map_assert__( this->_M_map1##X_norm != 0 );\
	return this->_M_map1##X_norm->operator[]( i );\
}\
\
inline void map1##X_norm##_set_lx( int lx )\
{\
	this->_map1##X_norm##_it_lx = lx * (lx + 1) * (lx + 2);\
	this->_map1##X_norm##_it_lx /= 6;\
}\
inline void map1##X_norm##_set_xyz( int ixyz ){ this->_map1##X_norm##_it_xyz = this->M_map1##X_norm##_data() + this->_map1##X_norm##_it_lx + ixyz; }\
inline int & map1##X_norm##_pos( int lx, int ixyz ){ return this->M_map1##X_norm( (lx*(lx+1)*(lx+2))/6 + ixyz ); }\
inline const int & map1##X_norm##_pos( int lx, int ixyz )const{ return this->M_map1##X_norm( (lx*(lx+1)*(lx+2))/6 + ixyz ); }\
inline int & map1##X_norm##_pos(){ __alp_map_assert__( this->_map1##X_norm##_it_xyz != 0 ); return *(this->_map1##X_norm##_it_xyz); }\
inline const int & map1##X_norm##_pos()const{ __alp_map_assert__( this->_map1##X_norm##_it_xyz != 0 ); return *(this->_map1##X_norm##_it_xyz); }\
inline void map1##X_norm##_set_ix( int ix )\
{\
	this->_map1##X_norm##_it_ix = this->map1##X_norm##_pos() + ix;\
}\
inline const size_type map1##X_norm##_idx( int i )const\
{\
	return this->map1##X_norm##_pos() + i;\
}\
inline const size_type map1##X_norm##_idx( int lx, int ixyz, int ix )const{ return this->map1##X_norm##_pos( lx, ixyz ) + ix;}\
inline const size_type & map1##X_norm##_idx()const{ return this->_map1##X_norm##_it_ix;}

#define __ALPHA_MAP_LMAX

#define __ALPHA_MAP_ITER_INIT_LIST \
_map1A_it(), _map1B_it(), _map1C_it(),\
_map2AB_it_la(), _map2AB_it_lb(),\
_map3ABC_it_la(),  _map3ABC_it_lb(), _map3ABC_it_l(),\
_map1A_d_it(), _map1B_d_it(), _map1C_d_it(),\
_map1A_d_it_ix(), _map1B_d_it_ix(), _map1C_d_it_ix(),\
_map3ABC_d_it_la(),  _map3ABC_d_it_lb(), _map3ABC_d_it_l(),\
_map1A_norm_it_lx(), _map1A_norm_it_xyz(), _map1B_norm_it_lx(), _map1B_norm_it_xyz(),\
_map2AB_norm_it_la(), _map2AB_norm_it_axyz(), _map2AB_norm_it_lb(), _map2AB_norm_it_bxyz(),\
_map1powkA_it(), _map1powkB_it(), _map2powk_it_la(), _map2powk_it_lb(), _map2slm_it_la(), _map2slm_it_lb(),\
_map1powkA_it_ix(), _map1powkB_it_ix(), _map1A_it_ix(), _map1B_it_ix(), _map1C_it_ix(),\
_map1A_norm_it_ix(), _map1B_norm_it_ix(), _map2AB_norm_it_ia(), _map2AB_norm_it_ib(),\
_map2powk_it_ia(), _map2powk_it_ib(), _map2AB_it_ia(), _map2AB_it_ib(), _map2slm_it_ia(), _map2slm_it_ib()

struct alpha_map : public mapping_struct
{
public:
	typedef typename memorystream::seek_dir seek_dir;
	typedef typename size_struct<1>::size_type size_type;
#pragma pack( push, 4 )
	typedef struct
	{
		int _pos, _size;
	} _pos1_struct;
	typedef struct
	{
		int _pos;
		size_struct<2> _size2;
	} _pos2_struct;
	typedef struct
	{
		int _pos;
		size_struct<3> _size3;
	} _pos3_struct;
	typedef _pos1_struct map_1d_type;
#pragma pack( pop )
	typedef typename alpha_siz::_lmax_struct _lmax_struct;
	typedef typename alpha_siz::_alpsize_struct _alpsize_struct;
	typedef union
	{
		void * _void;
		matrix_cursor_1<_pos1_struct> * _matrix1_pos1;
		matrix_cursor_1<map_1d_type> * _matrix1_dpos1;
		matrix_cursor_2<_pos1_struct> * _matrix2_pos1;
		matrix_cursor_3<_pos1_struct> * _matrix3_pos1;
		matrix_cursor_3<map_1d_type> * _matrix3_dpos1;
	} _matrixPtr;
protected:
	// mapping fields :
	//   _M_lmax, _M_map1A, _M_map1B, _M_map1C, _M_map2AB, _M_map3ABC;
	//   1. minimum - ;
	//   2. middle -- _M_map1powkA;
	//   3. maximum - _M_map1powkA, _M_map1powkB, _M_map2powk, _M_map2slm;
#ifdef  __ALPHA_MAP_LMAX
	_lmax_struct * _M_lmax;
#endif
	_alpsize_struct * _M_alpsize;

	mapping_enum * _M_mapping_t;

	matrix_cursor_1<_pos1_struct> * _M_map1A, * _M_map1B, * _M_map1C;
	matrix_cursor_2<_pos1_struct> * _M_map2AB;
	matrix_cursor_3<_pos1_struct> * _M_map3ABC;

	matrix_cursor_1<map_1d_type> * _M_map1A_d, * _M_map1B_d, * _M_map1C_d;
	matrix_cursor_3<map_1d_type> * _M_map3ABC_d;

	size_type _mx1A_norm_size, _mx1B_norm_size, _mx2AB_norm_size;
	matrix_cursor_1<int> * _M_map1A_norm, * _M_map1B_norm;
	matrix_cursor_2<int> * _M_map2AB_norm;

	matrix_cursor_1<_pos1_struct> * _M_map1powkA, * _M_map1powkB;
	matrix_cursor_2<_pos1_struct> * _M_map2powk;
	matrix_cursor_2<_pos1_struct> * _M_map2slm;

	void error(char const * _method, const char _message[] = "nothing to do here!" )const;
//private:
	_pos1_struct * _map1A_it, * _map1B_it, * _map1C_it;
	_pos1_struct * _map2AB_it_la, * _map2AB_it_lb;
	_pos1_struct * _map3ABC_it_la, * _map3ABC_it_lb, * _map3ABC_it_l;

	_pos1_struct * _map1A_d_it, * _map1B_d_it, * _map1C_d_it;
	size_type _map1A_d_it_ix, _map1B_d_it_ix, _map1C_d_it_ix;
	_pos1_struct * _map3ABC_d_it_la, * _map3ABC_d_it_lb, * _map3ABC_d_it_l;

	int _map1A_norm_it_lx, * _map1A_norm_it_xyz, _map1B_norm_it_lx, * _map1B_norm_it_xyz;
	int _map2AB_norm_it_la, _map2AB_norm_it_axyz, _map2AB_norm_it_lb, * _map2AB_norm_it_bxyz;

	_pos1_struct * _map1powkA_it, * _map1powkB_it;
	_pos1_struct * _map2powk_it_la, * _map2powk_it_lb;
	_pos1_struct * _map2slm_it_la, * _map2slm_it_lb;
	size_type _map1powkA_it_ix, _map1powkB_it_ix, _map1A_it_ix, _map1B_it_ix, _map1C_it_ix;
	size_type _map1A_norm_it_ix, _map1B_norm_it_ix, _map2AB_norm_it_ia, _map2AB_norm_it_ib;
	size_type _map2powk_it_ia, _map2powk_it_ib;
	size_type _map2AB_it_ia, _map2AB_it_ib;
	size_type _map2slm_it_ia, _map2slm_it_ib;

	inline void init_it_min()
	{
		this->map1A_set_lx( 0 );
		this->map1B_set_lx( 0 );
		this->map1C_set_lx( 0 );
		this->map2AB_set_la( 0 );
		this->map2AB_set_lb( 0 );
		this->map3ABC_set_la( 0 );
		this->map3ABC_set_lb( 0 );
		this->map3ABC_set_l( 0 );
	}
	inline void init_it_mid()
	{
		this->init_it_min();
		this->map1powkA_set_lx( 0 );
	}
	inline void init_it_max()
	{
		this->init_it_mid();
		this->map1powkB_set_lx( 0 );
		this->map2powk_set_la( 0 );
		this->map2powk_set_lb( 0 );
		this->map2slm_set_la( 0 );
		this->map2slm_set_lb( 0 );
	}
public:
	// Usage:
	//
	// writing to storage
	// 1. set mapping type: methods %set_mapping_min/mid/max()
	// 2. initialize pointers: method %write_map( memorystream & , _lmax_struct const & )
	// 3. initialize matrices: method %init_map( const int * , const int * , const int * )
	// 4. use it
	//
	// reading from storage
	// 1. read it (initialize pointers): method %read_map( memorystream & );
	alpha_map();
	alpha_map(alpha_map const & v);
	alpha_map & operator=( alpha_map const & v);
	// Interface for READ, WRITE operations
	inline void init_mapping(){ *this->_M_mapping_t = this->_mapping_t;}

#ifdef  __ALPHA_MAP_LMAX
	inline void init_lmax( _lmax_struct const & __lmax ){ *this->_M_lmax = __lmax; }
#endif
	inline const size_type & mx1A_norm_size()const{ return this->_mx1A_norm_size; }
	inline size_type & mx1A_norm_size(){ return this->_mx1A_norm_size; }
	inline const size_type & mx1B_norm_size()const{ return this->_mx1B_norm_size; }
	inline size_type & mx1B_norm_size(){ return this->_mx1B_norm_size; }
	inline const size_type & mx2AB_norm_size()const{ return this->_mx2AB_norm_size; }
	inline size_type & mx2AB_norm_size(){ return this->_mx2AB_norm_size; }

	const size_type comp_mx1X_norm_size( const int __la_max, const int * _A_size);
	const size_type comp_mx2AB_norm_size( const int __la_max, const int __lb_max, const int * _A_size, const int * _B_size );

	const size_type comp_size_map1X(_lmax_struct const & __lmax )const;
	const size_type comp_size_map2X(_lmax_struct const & __lmax )const;
	const size_type comp_size_map3X(_lmax_struct const & __lmax )const;
	const size_type comp_size_map1powkA(_lmax_struct const & __lmax )const;
	const size_type comp_size_map1powkB(_lmax_struct const & __lmax )const;
	const size_type comp_size_map2powk(_lmax_struct const & __lmax )const;
	const size_type comp_size(_lmax_struct const & __lmax )const;

	const size_type M_mx2slm_size()const;

	void write_map1X( memorystream & ms, _lmax_struct const & __lmax );
	void write_map2X( memorystream & ms );
	void write_map3X( memorystream & ms );
	void write_map1powkA( memorystream & ms );
	void write_map1powkB( memorystream & ms );
	void write_map2powk( memorystream & ms );
	void write_map2slm( memorystream & ms );
	const size_type write_map( memorystream & ms, _lmax_struct const & __lmax );

	void init_map1X_norm();
	void init_map2AB_norm();
	void init_map_norm();

	void init_map1X_d( const int * _A_dsize, const int * _B_dsize, const int * _C_dsize );// 0
	void init_map3X_d();// 1
	void init_mapd( const int * _A_dsize, const int * _B_dsize, const int * _C_dsize );// all: 0, 1

	void init_map1X( const int * _Asize, const int * _Bsize, const int * _Csize);// 0
	void init_map2X();// 1
	void init_map3X();// 2
	void init_map1powkA(const size_type __l0_max);// 3
	void init_map1powkB();// 4
	void init_map2powk();// 5
	void init_map2slm();// 6
	void init_map( const int * _Asize, const int * _Bsize, const int * _Csize);// all: 0, 1, 2, 3, 4, 5

	void read_map1X( memorystream & ms );
	void read_map2X( memorystream & ms );
	void read_map3X( memorystream & ms );
	void read_map1powkA( memorystream & ms );
	void read_map1powkB( memorystream & ms );
	void read_map2powk( memorystream & ms );
	void read_map2slm( memorystream & ms );
	const size_type read_map( memorystream & ms );

	// Interface for USING
#ifdef  __ALPHA_MAP_LMAX
	inline const size_type & la_max()const{ __alp_map_assert__( this->_M_lmax != 0 ); return this->_M_lmax->_la_max;}
	inline const size_type & lb_max()const{ __alp_map_assert__( this->_M_lmax != 0 ); return this->_M_lmax->_lb_max;}
	inline const size_type & l_max()const{ __alp_map_assert__( this->_M_lmax != 0 ); return this->_M_lmax->_l_max;}
	inline const size_type & lso_max()const{ __alp_map_assert__( this->_M_lmax != 0 ); return this->_M_lmax->_lso_max;}
#endif
	// lx_size
	inline const size_type & la_size()const{ __alp_map_assert__( this->_M_map1A != 0 ); return this->_M_map1A->size();}
	inline const size_type & lb_size()const{ __alp_map_assert__( this->_M_map1B != 0 ); return this->_M_map1B->size();}
	inline const size_type & lc_size()const{ __alp_map_assert__( this->_M_map1C != 0 ); return this->_M_map1C->size();}
	// alpsize
	inline const size_type & a_size()const{ __alp_map_assert__( this->_M_alpsize != 0 ); return this->_M_alpsize->_a_size;}// number of A alphas
	inline const size_type & b_size()const{ __alp_map_assert__( this->_M_alpsize != 0 ); return this->_M_alpsize->_b_size;}// --//-- B alphas
	inline const size_type & c_size()const{ __alp_map_assert__( this->_M_alpsize != 0 ); return this->_M_alpsize->_c_size;}// --//-- C alphas (ecp)
	// A, B, C, powkA, powkB
	// usage:
	// 1.
	// -> %map1A_size( la );
	// -> %map1A_pos( la );
	// 2.
	//    %map1A_set_lx( la );
	// -> %map1A_size();
	// -> %map1A_pos();
	//    %map1A_set_ix( ia );
	// -> %map1A_idx();
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1A );
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1B );
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1C );
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1A_d );
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1B_d );
	__ALPHA_MAP_INTERFACE_MAP1_POS1_X( map1C_d );
	__ALPHA_MAP_INTERFACE_MAP1_POS1( map1powkA );
	__ALPHA_MAP_INTERFACE_MAP1_POS1( map1powkB );
	__ALPHA_MAP_INTERFACE_MAP1X_NORM( A_norm );
	__ALPHA_MAP_INTERFACE_MAP1X_NORM( B_norm );

	// AB
	// usage:
	//   %map2AB_set_la( la );
	//   %map2AB_set_lb( lb );
	//   %map2AB_size(); - return value - product of %map1A_size(la), and %map1B_size(lb)
	//   %map2AB_pos(); - return value - position of the beginning
	//
	// powk, slm
	// usage:
	// same result by 3 ways
	// 1.
	// -> %map2powk_idx( la, ia, lb, ib); - return value - position of %_M_mx2powk's element // ia = 0..%map1A_size(la); ib = 0..%map1B_size(lb)
	// -> %map2powk_size(la, lb); - return value - maximum power of |k| (P.S. max_power = (size - 1) )
	// 2.
	//    %map2powk_set_la( la );
	//    %map2powk_set_lb( lb );
	// -> %map2powk_idx( ia, ib ); - return value - the same as above
	// -> %map2powk_size(); - return value - see above
	// 3.
	//    %map2powk_set_la( la );
	//    %map2powk_set_lb( lb );
	//    %map2powk_set_ia( ia );
	//    %map2powk_set_ib( ib );
	// -> %map2powk_idx(); - return value - see above
	// -> %map2powk_size(); - return value - see above
	//
	// P.S. %map1B_set_lx( lb ) has to be set for map2powk and map2slm interface to work
	//
	// the same for 'slm' methods : e.i.
	//    %map2slm_idx( la, ia, lb, ib ); - return value - position
	//    %map2powk_size(la, lb); - return value - (la + lb + 1)^2 ( P.S. (L+1)^2 = Sum_(i=0)^(i=L) (2*i + 1) )
	__ALPHA_MAP_INTERFACE_MAP2_POS1_AB( map2AB );
	__ALPHA_MAP_INTERFACE_MAP2_POS1( map2powk );
	__ALPHA_MAP_INTERFACE_MAP2_POS1( map2slm );// %map2slm_size() -- maximum angular momentum + 1
	inline const size_type & M_map2AB_norm_size()const{ __alp_map_assert__( this->_M_map2AB_norm != 0 ); return this->_M_map2AB_norm->size(); }
	inline const size_type & M_map2AB_norm_n()const{ __alp_map_assert__( this->_M_map2AB_norm != 0 ); return this->_M_map2AB_norm->n(); }
	inline const size_type & M_map2AB_norm_m()const{ __alp_map_assert__( this->_M_map2AB_norm != 0 ); return this->_M_map2AB_norm->m(); }
	inline const int * M_map2AB_norm_data()const{ __alp_map_assert__( this->_M_map2AB_norm != 0 ); return this->_M_map2AB_norm->data(); }
	inline int * M_map2AB_norm_data(){ __alp_map_assert__( this->_M_map2AB_norm != 0 ); return this->_M_map2AB_norm->data(); }
	inline void map2AB_norm_set_la( int la )
	{
		this->_map2AB_norm_it_la = la * (la + 1) * (la + 2);
		this->_map2AB_norm_it_la /= 6;
	}
	inline void map2AB_norm_set_axyz( int i )
	{
		this->_map2AB_norm_it_axyz = this->_map2AB_norm_it_la + i;
		this->_map2AB_norm_it_axyz *= this->M_map2AB_norm_m();
	}
	inline void map2AB_norm_set_lb( int lb )
	{
		this->_map2AB_norm_it_lb = lb * (lb + 1) * (lb + 2);
		this->_map2AB_norm_it_lb /= 6;
		this->_map2AB_norm_it_lb += this->_map2AB_norm_it_axyz;
	}
	inline void map2AB_norm_set_bxyz( int i )
	{
		this->_map2AB_norm_it_bxyz = this->M_map2AB_norm_data() + i;
	}
	inline const int & map2AB_norm_pos()const{ __alp_map_assert__( this->_map2AB_norm_it_bxyz != 0 ); return *(this->_map2AB_norm_it_bxyz); }
	inline int & map2AB_norm_pos(){ __alp_map_assert__( this->_map2AB_norm_it_bxyz != 0 ); return *(this->_map2AB_norm_it_bxyz); }
	inline void map2AB_norm_set_ia( int i ){ this->_map2AB_norm_it_ia = this->map2AB_norm_pos() + i * this->map1B_size(); }
	inline void map2AB_norm_set_ib( int i ){ this->_map2AB_norm_it_ib = this->_map2AB_norm_it_ia + i; }
	inline const size_type map2AB_norm_idx()const{ return this->_map2AB_norm_it_ib; }
	inline const size_type map2AB_norm_idx(int ia, int ib)const{ return this->map2AB_norm_pos() + ia * this->map1B_size() + ib; }

	// ABC
	__ALPHA_MAP_INTERFACE_MAP3_POS1_ABC( map3ABC );
	__ALPHA_MAP_INTERFACE_MAP3_POS1_ABC( map3ABC_d );
	void test_alp_map();
};

#endif//__ALPHA_MAP_H__
