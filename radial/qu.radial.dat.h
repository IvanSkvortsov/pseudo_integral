#ifndef __QU_RADIAL_DAT_H__
#define __QU_RADIAL_DAT_H__
#include"memory.map.h"
#include"qu.radial.map.h"
#include"alpha.pow.h"
#include"qu.hyperg.1F1.struct.h"
#include"qu.hyperg.1F11.struct.h"
//#include"q.int.h"
//#include"assert.config.h"

//#define __QU_RADIAL_DAT_PRINT
#ifdef  __QU_RADIAL_DAT_PRINT
  #define __ITER_MAX__ 250000
#endif

#define __QU_RADIAL_DAT_R_POWN 2
//#define __QU_RADIAL_DAT_R_POWN 0

#if __QU_RADIAL_DAT_ASSERT
  #define __QU_RADIAL_DAT_DEBUG
#endif
#ifdef  __QU_RADIAL_DAT_DEBUG
  #include<cassert>
  #define __qu_dat_assert__( arg ) assert( (arg) )
#else
  #define __qu_dat_assert__( arg )
#endif

//#define __QU_RADIAL_DAT_TEST_METHOD
#ifdef  __QU_RADIAL_DAT_TEST_METHOD
  #include<iomanip>
#endif

#define __QU_RADIAL_DAT_ITER_INIT_LIST \
  _map3qu_it_la(), _map3qu_it_lb(), _qu_dat_it_ia(), _qu_dat_it_ib(), _map3qu_it_l(), _qu_dat_it_ic(), _qu_dat_it_id()
#define __QU_RADIAL_DAT_COPY_ITER_INIT_LIST( v )\
  _map3qu_it_la( v._map3qu_it_la ), _map3qu_it_lb( v._map3qu_it_lb ), _qu_dat_it_ia( v._qu_dat_it_ia ), _qu_dat_it_ib( v._qu_dat_it_ib ),\
  _map3qu_it_l ( v._map3qu_it_l ),  _qu_dat_it_ic( v._qu_dat_it_ic ), _qu_dat_it_id( v._qu_dat_it_id )

template<typename T, typename U>
struct qu_radial_dat : public memory_map, public memorystream, public qu_radial_map, public alpha_pow<T,U>
{
public:
	__DATA_TYPEDEF( T );// from 'qu.radial.map.h' -> 'matrix.cursor.h' -> 'data.struct.h'
	typedef typename memory_map::mode_type mode_type;
	typedef typename memorystream::seek_dir seek_dir;
	typedef typename qu_radial_map::_lmax_struct _lmax_struct;
	typedef typename size_struct<1>::size_type size_type;

	typedef struct
	{
		int _pos, _size;
	} _pos1_struct;
	typedef _pos1_struct map3qu_type;
	typedef struct
	{
		int la, lb, l;
	} _lx_struct;
protected:
	matrix_cursor_1<T> * _M_mx1qu;
	matrix_cursor_3<map3qu_type> * _M_map3qu;

	int _map3qu_it_la, _map3qu_it_lb, _qu_dat_it_ia, _qu_dat_it_ib;
	map3qu_type * _map3qu_it_l;
	pointer _qu_dat_it_ic, _qu_dat_it_id;

	inline void error( const char * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] qu_radial_dat<T,U>::" << _method << ", " << _message << ' ' << sizeof(*this) << std::endl; }
public:
	qu_radial_dat();
	qu_radial_dat( qu_radial_dat<T,U> const & );
	qu_radial_dat<T,U> & operator=( qu_radial_dat<T,U> const & );

	inline void set_rad_map( qu_radial_map const & rad_map ){ this->qu_radial_map::operator=( rad_map ); }
	inline void set_alp_map( alpha_pow<T,U> const & alp_map ){ this->alpha_pow<T,U>::operator=( alp_map ); }

	const size_type comp_size();
	const size_type map3qu_SIZE();
	const size_type mx1qu_SIZE();

	void memory_create( const char * file, int __flags = O_RDWR| O_TRUNC| O_CREAT, mode_type __mode = memory_map::MODE_644 );
	void memory_open( const char * file, int __flags = O_RDONLY, mode_type __mode = memory_map::MODE_444 );
	inline void sync_stream(){ this->setbuf( this->memory_map::data(), this->memory_map::size() ); }

	void write_dat();
	void read_dat();

	void init_dat();
	void comp_qu();
	void comp_qu_max();
	void comp_qu_max_SemiLocal( _lx_struct const & _lx, qu_hyperg_1F11_struct<T> & qu_1f11, T & _alp, pointer _pown05_alp, const int _arr_maxsize );
	void comp_qu_max_SemiLocal_b(_lx_struct const& _lx, qu_hyperg_1F11_struct<T> & qu_1f11, pointer _pown05_alp, const int _alp_pown05_max );
	void comp_qu_max_Local( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu_1f1, T & _alp, pointer _pown05_alp, const int _arr_maxsize );
	void comp_qu_max_Local_b(_lx_struct const& _lx, qu_hyperg_1F1_struct<T> & qu_1f1, pointer _pown05_alp, const int _alp_pown05_max );
	void comp_qu_max_SpinOrbit( _lx_struct const & _lx, qu_hyperg_1F11_struct<T> & qu_1f11, T & _alp, pointer _pown05_alp, const int _arr_maxsize )
	{
		this->comp_qu_max_SemiLocal( _lx, qu_1f11, _alp, _pown05_alp, _arr_maxsize );
	}
	void comp_qu_mid();
	void comp_qu_mid_SemiLocal( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu_1f1, T & _alp, pointer _pown05_alp, const int _arr_midsize );
	void comp_qu_mid_SemiLocal_b(_lx_struct const& _lx, qu_hyperg_1F1_struct<T> & qu_1f1, pointer _pown05_alp, const int _alp_pown05_mid );
	void comp_qu_mid_Local( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu_1f1, T & _alp, pointer _pown05_alp, const int _arr_midsize );
	void comp_qu_mid_Local_b(_lx_struct const& _lx, qu_hyperg_1F1_struct<T> & qu_1f1, pointer _pown05_alp, const int _alp_pown05_mid );
	void comp_qu_mid_SpinOrbit( _lx_struct const & _lx, qu_hyperg_1F1_struct<T> & qu_1f1, T & _alp, pointer _pown05_alp, const int _arr_midsize )
	{
		this->comp_qu_mid_SemiLocal( _ls, qu_1f1, _alp, _pown05_alp, _arr_maxsize );
	}

	// Usage:
	// %qu_dat_set_la( la )
	// %qu_dat_set_lb( lb )
	// %qu_dat_set_l( l )

	// %_M_mx1qu
	inline const size_type & M_mx1qu_size()const
	{
		__qu_dat_assert__( this->_M_mx1qu != 0 );
		return this->_M_mx1qu->size();
	}
	inline const_pointer M_mx1qu_data()const
	{
		__qu_dat_assert__( this->_M_mx1qu != 0 );
		return this->_M_mx1qu->data();
	}
	inline pointer M_mx1qu_data()
	{
		__qu_dat_assert__( this->_M_mx1qu != 0 );
		return this->_M_mx1qu->data();
	}
	// %_M_map3qu
	inline const size_type & M_map3qu_size()const
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->size();
	}
	inline const size_type & M_map3qu_n()const
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->n();
	}
	inline const size_type & M_map3qu_m()const
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->m();
	}
	inline const size_type & M_map3qu_p()const
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->p();
	}
	inline const map3qu_type * M_map3qu_data()const
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->data();
	}
	inline map3qu_type * M_map3qu_data()
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		return this->_M_map3qu->data();
	}
	// map3qu Interface
	inline void map3qu_set_la( int la )
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		__qu_dat_assert__( this->M_map3qu_data() != 0 );
		this->_map3qu_it_la = this->M_map3qu_m() * la;
	}
	inline void map3qu_set_lb( int lb )
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		__qu_dat_assert__( this->M_map3qu_data() != 0 );
		this->_map3qu_it_lb = this->_map3qu_it_la + lb;
		this->_map3qu_it_lb *= this->M_map3qu_p();
	}
	inline void map3qu_set_l( int l )
	{
		__qu_dat_assert__( this->_M_map3qu != 0 );
		__qu_dat_assert__( this->M_map3qu_data() != 0 );
		this->_map3qu_it_l = this->M_map3qu_data() + this->_map3qu_it_lb + l;
	}
	// qu_dat Interface
	inline void qu_dat_set_la( int la )
	{
		this->qu_radial_map::qu_set_la( la );
		//this->alpha_map::map3ABC_set_la( la );
		this->alpha_map::map1A_set_lx( la );
		this->map3qu_set_la( la );
	}
	inline void qu_dat_set_lb( int lb )
	{
		this->qu_radial_map::qu_set_lb( lb );
		//this->alpha_map::map3ABC_set_lb( lb );
		this->alpha_map::map1B_set_lx( lb );
		this->map3qu_set_lb( lb );
	}
	inline void qu_dat_set_l( int l )
	{
		this->qu_radial_map::qu_set_l( l );
		//this->alpha_map::map3ABC_set_l( l );
		this->alpha_map::map1C_set_lx( l );
		this->map3qu_set_l( l );
	}
	inline void qu_dat_set_lmax(){ this->qu_dat_set_l( this->M_get_l_max() ); }
	inline int & qu_dat_pos()
	{
		__qu_dat_assert__( this->_map3qu_it_l != 0 );
		return this->_map3qu_it_l->_pos;
	}
	inline const int & qu_dat_pos()const
	{
		__qu_dat_assert__( this->_map3qu_it_l != 0 );
		return this->_map3qu_it_l->_pos;
	}
	inline int & qu_dat_size()
	{
		__qu_dat_assert__( this->_map3qu_it_l != 0 );
		return this->_map3qu_it_l->_size;
	}
	inline const int & qu_dat_size()const
	{
		__qu_dat_assert__( this->_map3qu_it_l != 0 );
		return this->_map3qu_it_l->_size;
	}
	inline const int & qu_dat_n()const{ return this->alpha_map::map1A_size(); }
	inline const int & qu_dat_m()const{ return this->alpha_map::map1B_size(); }
	inline const int & qu_dat_p()const{ return this->alpha_map::map1C_size(); }
	inline const int & qu_dat_q()const{ return this->qu_radial_map::qu_size(); }

	inline void qu_dat_set_ia( int i ){ this->_qu_dat_it_ia = i * this->qu_dat_m(); }
	inline void qu_dat_set_ib( int i )
	{
		this->_qu_dat_it_ib = this->_qu_dat_it_ia + i;
		this->_qu_dat_it_ib *= this->qu_dat_p();
	}
	inline void qu_dat_set_ic( int i )
	{
		__qu_dat_assert__( this->_M_mx1qu != 0 );
		__qu_dat_assert__( this->M_mx1qu_data() != 0 );
		this->_qu_dat_it_ic = this->M_mx1qu_data() + this->qu_dat_pos() + (this->_qu_dat_it_ib + i) * this->qu_dat_q();
	}
	inline const_pointer qu_dat()const
	{
		__qu_dat_assert__( this->_qu_dat_it_ic != 0 );
		return this->_qu_dat_it_ic;
	}
	inline pointer qu_dat()
	{
		__qu_dat_assert__( this->_qu_dat_it_ic != 0 );
		return this->_qu_dat_it_ic;
	}
	inline void qu_dat_set_id( int i )// int i -- is this->qu_radial_map::qu_idx()
	{
		__qu_dat_assert__( this->_qu_dat_it_ic != 0 );
		this->_qu_dat_it_id = this->_qu_dat_it_ic + i;
	}
	inline const_reference qu_dat_value()const
	{
		__qu_dat_assert__( this->_qu_dat_it_id != 0 );
		return *this->_qu_dat_it_id;
	}
	inline reference qu_dat_value()
	{
		__qu_dat_assert__( this->_qu_dat_it_id != 0 );
		return *this->_qu_dat_it_id;
	}
	inline const_reference qu_dat_value( int i )const
	{
		__qu_dat_assert__( this->_qu_dat_it_ic != 0 );
		return this->_qu_dat_it_ic[i];
	}
	inline reference qu_dat_value( int i )
	{
		__qu_dat_assert__( this->_qu_dat_it_ic != 0 );
		return this->_qu_dat_it_ic[i];
	}
	//
#ifdef  __QU_RADIAL_DAT_TEST_METHOD
	void test_print(std::ostream & out);
	void test_print_alp(std::ostream & out, _lx_struct const & _lx);
	void test_print_n(std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix);
	void test_print_semi_local(std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix);
	void test_print_local(std::ostream & out, _lx_struct const & _lx, _lx_struct const & _ix);
#else
	void test_print(std::ostream & out){}
#endif
};

#endif//__QU_RADIAL_DAT_H__
