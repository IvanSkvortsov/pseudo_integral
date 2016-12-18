#ifndef __ALPHA_POW_H__
#define __ALPHA_POW_H__
#include"alpha.val.h"
//#include"assert.config.h"

#if __ALPHA_POW_ASSERT
  #define __ALPHA_POW_DEBUG// 'assert.config.h'
#endif
#ifdef  __ALPHA_POW_DEBUG
  #include<cassert>
  #define __alp_pow_assert__( arg ) assert( (arg) )
#else
  #define __alp_pow_assert__( arg )
#endif

#define __ALPHA_POW_INTERFACE_MX1POWKX( powkX )\
inline reference M_mx1##powkX( int i )\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	return this->_M_mx1##powkX->operator[]( i );\
}\
inline const_reference M_mx1##powkX( int i )const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	return this->_M_mx1##powkX->operator[]( i );\
}\
inline pointer M_mx1##powkX##_data()\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	return this->_M_mx1##powkX->data();\
}\
inline pointer M_mx1##powkX##_data( int i )\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	__alp_pow_assert__( this->_M_mx1##powkX->data() != 0 );\
	return this->_M_mx1##powkX->data() + i;\
}\
inline const_pointer M_mx1##powkX##_data()const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	return this->_M_mx1##powkX->data();\
}\
inline const_pointer M_mx1##powkX##_data( int i )const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	__alp_pow_assert__( this->_M_mx1##powkX->data() != 0 );\
	return this->_M_mx1##powkX->data() + i;\
}\
inline const size_type & M_mx1##powkX##_size()const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	return this->_M_mx1##powkX->size();\
}\
\
inline void mx1##powkX##_set_idx( int i ){ this->_mx1##powkX##_it = this->M_mx1##powkX##_data() + i; }\
inline void mx1##powkX##_set_idx()\
{\
	__alp_pow_assert__( this->map1##powkX##_idx() >= 0 );\
	/*__alp_pow_assert__( this->map1##powkX##_idx() < this->M_mx1##powkX##_size() );*/\
	this->_mx1##powkX##_it = this->M_mx1##powkX##_data() + this->map1##powkX##_idx();\
}\
inline const_reference mx1##powkX( int _n )const\
{\
	__alp_pow_assert__( this->_mx1##powkX##_it != 0 );\
	return this->_mx1##powkX##_it[_n];\
}\
inline const_pointer mx1##powkX()const\
{\
	__alp_pow_assert__( this->_mx1##powkX##_it != 0 );\
	return this->_mx1##powkX##_it;\
}\
inline const_pointer mx1##powkX( int lx, int i )const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	__alp_pow_assert__( this->_M_mx1##powkX->data() != 0 );\
	return this->_M_mx1##powkX->data() + this->map1##powkX##_idx( lx, i );\
}\
inline const_reference mx1##powkX( int lx, int i, int _n )const\
{\
	__alp_pow_assert__( this->_M_mx1##powkX != 0 );\
	__alp_pow_assert__( this->_M_mx1##powkX->data() != 0 );\
	return this->_M_mx1##powkX->operator[](this->map1##powkX##_idx( lx, i ) + _n);\
}

#define __ALPHA_POW_ITER_INIT_LIST _mx1powkA_it(), _mx1powkB_it(), _mx2powk_it()

template<typename T, typename U>
struct alpha_pow : public alpha_val<T,U>
{
public:
	typedef typename alpha_val<T,U>::size_type size_type;
	typedef typename alpha_val<T,U>::_pos1_struct _pos1_struct;
	typedef typename alpha_val<T,U>::_pos2_struct _pos2_struct;
	typedef typename alpha_val<T,U>::_pos3_struct _pos3_struct;
	typedef typename alpha_val<T,U>::_lmax_struct _lmax_struct;
	typedef typename alpha_val<T,U>::_alpsize_struct _alpsize_struct;
	typedef typename alpha_val<T,U>::seek_dir seek_dir;
	__DATA_TYPEDEF( T );
	typedef T float_type;
	typedef union
	{
		void * _void;
		matrix_cursor_1<T> * _matrix1_T;
	} _matrixPtr;
protected:
	matrix_cursor_1<T> * _M_mx1powkA, * _M_mx1powkB;
	matrix_cursor_1<T> * _M_mx2powk;

	pointer _mx1powkA_it, _mx1powkB_it, _mx2powk_it;
	//pointer _mx2powk_it_lx, _mx2powk_it_lb;//, _mx2powk_it_ia, _mx2powk_it_ib;
	//size_type _map2powk_B_size;

	const size_type mx1powkX_size( const size_type __l0_size, const size_type & __lx_max, const int * _Xsize );
	const size_type mx2powk_size( _lmax_struct const & __lmax, const int * _Asize, const int * _Bsize );
	const size_type comp_size_mx2powk( _lmax_struct const & __lmax, const int * _Asize, const int * _Bsize );
	const size_type comp_size_mx1powkX( const size_type __l0_size, const size_type & __lx_max, const int * _Xsize );

	void write_mx1powkX( memorystream & ms, matrix_cursor_1<T> *& __mx1,const size_type __l0_size, const size_type & __lx_max, const int * _Xsize);
	void write_mx2powk( memorystream & ms, _lmax_struct const & __lmax, const int * _Asize, const int * _Bsize );

	void init_mx1powkA( U const & CA_len, U const * A_alp);
	void init_mx1powkB( U const & CB_len, U const * B_alp);
	void init_mx2powk( U const * CA, U const * A_alp, U const * CB, U const * B_alp);
public:
	alpha_pow();
	alpha_pow( alpha_pow<T,U> const & );
	alpha_pow<T,U> & operator=( alpha_pow<T,U> const & );

	const size_type comp_size( _lmax_struct const & __lmax, _alpsize_struct const & __alpsize, const int * _Asize, const int * _Bsize );
	const size_type write_mxpow( memorystream & ms,
			_lmax_struct const & __lmax, _alpsize_struct const & __alpsize, const int * _Asize, const int * _Bsize);
	const size_type read_mxpow( memorystream & ms );
	void init_mxpow( U const * CA, U const * A_alp, U const * CB, U const * B_alp);

	// Interface
	// usage (e.i. powers of kA) by 2 ways:
	//   1.a   %mx1powkA(la, i, n)  - n'th power of kA = 2 * alp[la][i] * CA
	//   1.b   %mx1powkA(la, i)[n]  - -//-
	//
	//   2.1   %map1powkA_set_lx( la );
	//   2.2   %map1powkA_set_ix( ia );
	//   2.3.a %mx1powkA( n );
	//   2.3.b %mx1powkA()[n];
	__ALPHA_POW_INTERFACE_MX1POWKX( powkA );
	__ALPHA_POW_INTERFACE_MX1POWKX( powkB );

	// usage ( powers of k);
	// 1.a   %mx2powk( la, ia, lb, ib, n ) - n'th power of k = |kA + kB|
	// 1.b   %mx2powk( la, ia, lb, ib )[n]
	//
	// 2.1   %map2powk_set_la( la );
	// 2.2   %map2powk_set_lb( lb );
	// 2.3   %map1B_set_lx( lb );
	// 2.4   %mx2powk( ia, ib )[n];
	//
	// 3.1   %map2powk_set_la( la );
	// 3.2   %map2powk_set_lb( lb );
	// 3.3   %map1B_set_lx( lb );
	// 3.4   %map2powk_set_ia( ia );
	// 3.5   %map2powk_set_ib( ib );
	// 3.6.a %mx2powk( n );
	// 3.6.b %mx2powk()[n];
	inline const_pointer M_mx2powk_data()const{ return this->_M_mx2powk->data(); }
	inline pointer M_mx2powk_data(){ return this->_M_mx2powk->data(); }
	inline const size_type & M_mx2powk_size()const{ return this->_M_mx2powk->size(); }
	inline void mx2powk_set_idx()
	{
		this->_mx2powk_it = this->M_mx2powk_data() + this->map2powk_idx();
	}
	inline const_pointer mx2powk(int ia, int ib)const
	{
		return this->M_mx2powk_data() + this->map2powk_idx(ia, ib );
	}
	inline const_reference mx2powk( int _n )const
	{
		__alp_pow_assert__( this->_mx2powk_it != 0 );
		return this->_mx2powk_it[_n];
		//return this->_M_mx2powk->operator[]( this->map2powk_idx() + _n );
	}
	inline const_pointer mx2powk()const
	{
		__alp_pow_assert__( this->_mx2powk_it != 0 );
		return this->_mx2powk_it;
		//return this->M_mx2powk_data() + this->map2powk_idx();
	}
	inline pointer mx2powk()
	{
		__alp_pow_assert__( this->_mx2powk_it != 0 );
		return this->_mx2powk_it;
		//return this->M_mx2powk_data() + this->map2powk_idx();
	}
	inline const_reference mx2powk( int la, int ia, int lb, int ib, int n )const
	{
		return this->mx2powk( la, ia, lb, ib )[n];
	}
	inline const_pointer mx2powk( int la, int ia, int lb, int ib )const
	{
		return this->M_mx2powk_data() + this->map2powk_idx( la, ia, lb, ib);
	}
	void test_alp_pow();
	void test_alp_pow_powkA();
	void test_alp_pow_powkB();
	void test_alp_pow_powk();
};

#endif//__ALPHA_POW_H__
