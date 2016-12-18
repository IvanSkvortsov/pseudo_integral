#ifndef __ALPHA_VAL_H__
#define __ALPHA_VAL_H__
#include"alpha.map.h"// memorystream, matrix_cursor
//#include"assert.config.h"

#if __ALPHA_VAL_ASSERT// 'assert.config.h'
  #define __ALPHA_VAL_DEBUG
#endif
#ifdef  __ALPHA_VAL_DEBUG
  #include<cassert>
  #define __alp_val_assert__( arg ) assert( (arg) )
#else
  #define __alp_val_assert__( arg )
#endif

#define __ALPHA_VAL_MX1_INTERFACE( mx1X, float_type ) \
inline const size_type & M_##mx1X##_size()const\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->size();\
}\
inline float_type & M_##mx1X(int i)\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->operator[](i);\
}\
inline float_type const & M_##mx1X(int i)const\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->operator[](i);\
}\
inline float_type * M_##mx1X##_data()\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->data();\
}\
inline float_type * M_##mx1X##_data( int i )\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->data( i );\
}\
inline float_type const * M_##mx1X##_data()const\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->data();\
}\
inline float_type const * M_##mx1X##_data( int i )const\
{\
	__alp_val_assert__( this->_M_##mx1X != 0 );\
	return this->_M_##mx1X->data( i );\
}
// mx1A, mx1B, mx1C, mx1C_nk, mx1kA, mx1kB
#define __ALPHA_VAL_MX1X_INTERFACE( mx1X, map1X, float_type )\
__ALPHA_VAL_MX1_INTERFACE( mx1X, float_type ) \
inline void mx1X##_set_idx( int i )\
{\
	__alp_val_assert__( this->M_##mx1X##_data() != 0 );\
	this->_##mx1X##_it = this->M_##mx1X##_data() + i;\
}\
inline void mx1X##_set_idx(){ this->mx1X##_set_idx( this->map1X##_idx() ); }\
inline float_type const * mx1X##_it()const{ return this->_##mx1X##_it; }\
inline float_type const & mx1X()const\
{\
	__alp_val_assert__( this->_##mx1X##_it != 0 );\
	return *(this->_##mx1X##_it);\
}\
inline float_type & mx1X() \
{\
	__alp_val_assert__( this->_##mx1X##_it != 0 );\
	return *(this->_##mx1X##_it);\
}\
inline float_type const & mx1X( int lx, int i )const{ return this->M_##mx1X( this->map1X##_idx(lx, i) ); }\
inline float_type const & mx1X( int i )const{ return this->M_##mx1X( this->map1X##_pos() + i ); }
// mx2k
#define __ALPHA_VAL_MX2X_INTERFACE( mx2X, map2X, float_type )\
__ALPHA_VAL_MX1_INTERFACE( mx2X, float_type ) \
inline void mx2X##_set_idx( int i )\
{\
	__alp_val_assert__( this->M_##mx2X##_data() != 0 );\
	this->_##mx2X##_it = this->M_##mx2X##_data() + i;\
}\
inline void mx2X##_set_idx(){ this->mx2X##_set_idx( this->map2X##_idx() ); }\
inline float_type & mx2X()\
{\
	__alp_val_assert__( this->_##mx2X##_it != 0 );\
	return *(this->_##mx2X##_it);\
}\
inline float_type const & mx2X()const\
{\
	__alp_val_assert__( this->_##mx2X##_it != 0 );\
	return *(this->_##mx2X##_it);\
}\
inline float_type const & mx2X( int i, int j )const{ return this->M_##mx2X( this->map2X##_idx( i, j ) ); }\
inline float_type const & mx2X( int la, int i, int lb, int j )const{ return this->M_##mx2X( this->map2X##_idx( la, i, lb, j ) ); }
// mx1A_norm, mx1B_norm
#define __ALPHA_VAL_MX1X_NORM_INTERFACE( mx1X, map1X, float_type )\
__ALPHA_VAL_MX1_INTERFACE( mx1X, float_type ) \
inline void mx1X##_set_idx( int i )\
{\
	__alp_val_assert__( this->M_##mx1X##_data() != 0 );\
	this->_##mx1X##_it = this->M_##mx1X##_data() + i;\
}\
inline void mx1X##_set_idx(){ this->mx1X##_set_idx( this->map1X##_idx() ); }\
inline float_type const * mx1X##_it()const{ return this->_##mx1X##_it; }\
inline float_type & mx1X()\
{\
	__alp_val_assert__( this->_##mx1X##_it != 0 );\
	return *(this->_##mx1X##_it);\
}\
inline float_type const & mx1X()const\
{\
	__alp_val_assert__( this->_##mx1X##_it != 0 );\
	return *(this->_##mx1X##_it);\
}\
inline float_type const & mx1X( int lx, int ixyz, int i )const{ return this->M_##mx1X( this->map1X##_idx(lx, ixyz, i) ); }\
inline float_type const & mx1X( int i )const{ return this->M_##mx1X( this->map1X##_pos() + i ); }
// mx2AB_norm
#define __ALPHA_VAL_MX2X_NORM_INTERFACE( mx2X, map2X, float_type )\
__ALPHA_VAL_MX1_INTERFACE( mx2X, float_type ) \
inline void mx2X##_set_idx( int i )\
{\
	__alp_val_assert__( this->M_##mx2X##_data() != 0 );\
	this->_##mx2X##_it = this->M_##mx2X##_data() + i;\
}\
inline void mx2X##_set_idx(){ this->mx2X##_set_idx( this->map2X##_idx() ); }\
inline float_type & mx2X()\
{\
	__alp_val_assert__( this->_##mx2X##_it != 0 );\
	return *(this->_##mx2X##_it);\
}\
inline float_type const & mx2X()const\
{\
	__alp_val_assert__( this->_##mx2X##_it != 0 );\
	return *(this->_##mx2X##_it);\
}\
inline float_type const & mx2X( int i, int j )const{ return this->M_##mx2X( this->map2X##_idx( i, j ) ); }

#define __ALPHA_VAL_ITER_INIT_LIST _mx1A_it(), _mx1B_it(), _mx1C_it(), _mx1kA_it(), _mx1kB_it(), _mx1kB3_it(), _mx2k_it(),\
	_mx1A_norm_it(), _mx1B_norm_it(), _mx2AB_norm_it(), _mx2AB_exp_it()

template<typename T, typename U>
struct alpha_val : public alpha_map
{
public:
	typedef typename alpha_map::seek_dir seek_dir;
	typedef typename alpha_map::_pos1_struct _pos1_struct;
	typedef typename alpha_map::_pos2_struct _pos2_struct;
	typedef typename alpha_map::_pos3_struct _pos3_struct;
	typedef typename alpha_map::_lmax_struct _lmax_struct;
	typedef typename alpha_map::_alpsize_struct _alpsize_struct;
	typedef typename alpha_map::size_type size_type;
	__DATA_TYPEDEF( T );
	typedef T float_type;
	typedef struct{
		float_type r[3];
		operator pointer(){ return this->r; }
		operator const_pointer()const{ return this->r; }
		reference operator[](const int i){ return this->r[i]; }
		const_reference operator[](const int i)const{ return this->r[i]; }
		reference x(){ return this->r[0]; }
		reference y(){ return this->r[1]; }
		reference z(){ return this->r[2]; }
		const_reference x()const{ return this->r[0]; }
		const_reference y()const{ return this->r[1]; }
		const_reference z()const{ return this->r[2]; }
	} float_x3_type;
	typedef union
	{
		void * _void;
		matrix_cursor_1<T> * _matrix1_T;// storage of values
		matrix_cursor_1<float_x3_type> * _matrix1_T3;// storage of 3d-vector values
	} _matrixPtr;
protected:
	int _mx1C_nk_max;
	matrix_cursor_1<T> * _M_mx1A, * _M_mx1B, * _M_mx1C;
	matrix_cursor_1<int> * _M_mx1C_nk;
	matrix_cursor_1<T> * _M_mx1kA, * _M_mx1kB;
	matrix_cursor_1<float_x3_type> * _M_mx1kB3;// matrix of kB 3d-vectors (P.S. mapped as same as %_M_mx1kB)
	matrix_cursor_1<T> * _M_mx2k;
	matrix_cursor_1<T> * _M_mx1A_norm, * _M_mx1B_norm, * _M_mx2AB_norm;
	matrix_cursor_1<T> * _M_mx2AB_exp;

	pointer _mx1A_it, _mx1B_it, _mx1C_it, _mx1kA_it, _mx1kB_it, _mx2k_it;
	float_x3_type * _mx1kB3_it;
	pointer _mx1A_norm_it, _mx1B_norm_it, _mx2AB_norm_it;
	pointer _mx2AB_exp_it;

	const size_type comp_size_mx1( const size_type __size, const int szof_type )const;
	const size_type comp_size_mx1T( const size_type __size )const;
	const size_type comp_size_mx1T3( const size_type __size )const;

	void write_mx1T( memorystream & ms, matrix_cursor_1<T> * & __mx1, const size_type __size );
	void write_mx1T3( memorystream & ms, matrix_cursor_1<float_x3_type> * & __mx1, const size_type __size );
	void write_mx1z( memorystream & ms, matrix_cursor_1<int> * & __mx1, const size_type __size );
	void read_mx1T( memorystream & ms, matrix_cursor_1<T> * & __mx1 );
	void read_mx1T3( memorystream & ms, matrix_cursor_1<float_x3_type> * & __mx1 );
	void read_mx1z( memorystream & ms, matrix_cursor_1<int> * & __mx1 );

	void init_mx1X( matrix_cursor_1<T> * __mx1, U const * _X_alp, const size_type __size, U const & _CX );
	void init_mx1X( matrix_cursor_1<T> * __mx1, U const * _X_alp, const size_type __size );
	void init_mx2k( U const * _A_alp, U const * _B_alp, U const & CA, U const & CB, U const & CACB );
	void init_mx1kB3( U const * CX, U const * x_alp);
public:
	alpha_val();
	alpha_val( alpha_val<T,U> const & );
	alpha_val<T,U> & operator=( alpha_val<T,U> const & );
	
	const size_type comp_size( _lmax_struct const & __lmax, _alpsize_struct const & __alpsize, const int * _Asize, const int * _Bsize );

	const size_type write_mxalp( memorystream & ms, _lmax_struct const & __lmax, _alpsize_struct const & __alpsize );
	const size_type read_mxalp( memorystream & ms );

	// nk - powers of r in pseudo potential
	// 1.  %init_mx1C_nk( C_nk )
	void init_mx1C_nk( int const * _C_nk );
	// initialization of matrices %_M_mx1A, %_M_mx1B, %_M_mx1C, %_M_mx1kA, %_M_mx1kB, %_M_mx2k
	// a. %init_mxalp( A_alp, B_alp, C_alp, CA, CB, CACB ) - maximum mapping type <=> A != C != B
	// b. %init_mxalp( A_alp, B_alp, C_alp, CA ) - middle mapping type <=> A != C == B
	// c. %init_mxalp( A_alp, B_alp, C_alp )- minimum mapping type <=> A == C == B
	void init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const & CA, U const & CB, U const & CACB );
	void init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const & CA );
	void init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp );
	void init_mxalp( U const * _A_alp, U const * _B_alp, U const * _C_alp, U const * CA, U const * CB );
	// alpha values initialization: %_M_mx1A, %_M_mx1B, %_M_mx1C
	// 1.  %init_mx1_alp( A_alp, B_alp, C_alp )
	void init_mx1_alp( U const * _A_alp, U const * _B_alp, U const * _C_alp );
	// norm initialization:
	// 1.  %init_norm( A_alp, B_alp )
	// 2.1 %init_mx1X_norm( A_alp, B_alp )
	// 2.2 %init_mx2AB_norm()
	void init_mx1X_norm( U const * _A_alp, U const * _B_alp );
	void init_mx2AB_norm();
	void init_norm( U const * _A_alp, U const * _B_alp );
	void init_exp( U const * _A_alp, U const * _B_alp, U const & sqr_CA, U const & sqr_CB );

	__ALPHA_VAL_MX1X_INTERFACE( mx1A, map1A, float_type );
	__ALPHA_VAL_MX1X_INTERFACE( mx1B, map1B, float_type );
	__ALPHA_VAL_MX1X_INTERFACE( mx1C, map1C, float_type );
	__ALPHA_VAL_MX1X_INTERFACE( mx1kA, map1A, float_type );
	__ALPHA_VAL_MX1X_INTERFACE( mx1kB, map1B, float_type );
	__ALPHA_VAL_MX1X_INTERFACE( mx1kB3, map1B, float_x3_type );
	__ALPHA_VAL_MX1X_NORM_INTERFACE( mx1A_norm, map1A_norm, float_type );
	__ALPHA_VAL_MX1X_NORM_INTERFACE( mx1B_norm, map1B_norm, float_type );
	inline const int mx1C_nk_max()const{ return this->_mx1C_nk_max; }
	inline const int & mx1C_nk()const{ return this->_M_mx1C_nk->operator[]( this->map1C_idx() ); }
	inline int & mx1C_nk(){ return this->_M_mx1C_nk->operator[]( this->map1C_idx() ); }

	__ALPHA_VAL_MX2X_INTERFACE( mx2k, map2AB, float_type );
	__ALPHA_VAL_MX2X_INTERFACE( mx2AB_exp, map2AB, float_type );
	__ALPHA_VAL_MX2X_NORM_INTERFACE( mx2AB_norm, map2AB_norm, float_type );

};

#endif//__ALPHA_VAL_H__
