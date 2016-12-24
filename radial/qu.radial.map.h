#ifndef __QU_RADIAL_MAP_H__
#define __QU_RADIAL_MAP_H__
#include"memorystream.h"// memorystream
#include"matrix.cursor.h"// matrix_cursor, size_struct<n>::size_type
#include"mapping.t.h"// mapping_struct
#include"alpha.siz.h"// alpha_siz::_lmax_struct
#include"assert.config.h"

//#define __QU_RADIAL_MAP_INIT_LOG
#define __QU_RADIAL_MAP_MIDDLE_SPEC

#if __QU_RADIAL_MAP_ASSERT
  #define __QU_RADIAL_MAP_DEBUG
#endif
#ifdef  __QU_RADIAL_MAP_DEBUG
  #include<cassert>
  #define __assert__( arg ) assert( (arg) )
#else
  #define __assert__( arg )
#endif

#define __QU_RADILA_MAP_ITER_INIT_LIST \
	_map3qu_pos_it_n(), _map3qu_pos_it_m(), _map3qu_pos_it_p(), _map3qu_pos_it_la(), _map3qu_pos_it_lb(),\
	_map1qu_dat_it_n(), _map1qu_dat_it_m(), _map1qu_dat_it_p(), _map1qu_dat_it_N(), _map1qu_dat_it_lmb_a(), _sz3_it()

#define __QU_RADILA_MAP_COPY_ITER_INIT_LIST( v ) \
	_map3qu_pos_it_n( v._map3qu_pos_it_n ), _map3qu_pos_it_m( v._map3qu_pos_it_m ), _map3qu_pos_it_p( v._map3qu_pos_it_p ),\
	_map3qu_pos_it_la( v._map3qu_pos_it_la ), _map3qu_pos_it_lb( v._map3qu_pos_it_lb ),\
	_map1qu_dat_it_n( v._map1qu_dat_it_n ), _map1qu_dat_it_m( v._map1qu_dat_it_m ), _map1qu_dat_it_p( v._map1qu_dat_it_p ),\
	_map1qu_dat_it_N( v._map1qu_dat_it_N ), _map1qu_dat_it_lmb_a( v._map1qu_dat_it_lmb_a ), _sz3_it( v._sz3_it )

struct qu_radial_map: public mapping_struct
{
public:
	static const int MAP1QU_DAT_INIT_NUM = -1;// any negative number

	typedef typename memorystream::seek_dir seek_dir;
	typedef typename size_struct<1>::size_type size_type;
	typedef typename alpha_siz::_lmax_struct _lmax_struct;

	typedef struct
	{
		int _pos, _size;
		size_struct<3> _sz3;
	} _mx3pos_struct;
	typedef struct
	{
		int _idx, _n_times;
	} _idxn_struct;

	typedef _mx3pos_struct map3qu_pos_type;
	typedef _idxn_struct   map1qu_dat_type;
protected:
	inline void error(const char * _method, const char _message[] = "nothing to do here")const
	{ std::cerr << "Error: [" << this << "] qu_radial_map::" << _method << ", " << _message << std::endl; }

	size_type _map1qu_dat_size;
	_lmax_struct * _M_lmax;
	matrix_cursor_3<map3qu_pos_type> * _M_map3qu_pos;
	matrix_cursor_1<map1qu_dat_type> * _M_map1qu_dat;

	map3qu_pos_type * _map3qu_pos_it_n, * _map3qu_pos_it_m, * _map3qu_pos_it_p;
	int _map3qu_pos_it_la, _map3qu_pos_it_lb;
	map1qu_dat_type * _map1qu_dat_it_n, * _map1qu_dat_it_m, * _map1qu_dat_it_p;
	int _map1qu_dat_it_N, _map1qu_dat_it_lmb_a;
	size_struct<3>  * _sz3_it;
public:
	qu_radial_map();
	qu_radial_map(qu_radial_map const & );
	qu_radial_map & operator=(qu_radial_map const & );
	// %comp_size( _lmax_struct const & )const
	const size_type comp_size( _lmax_struct const & __lmax )const;
	const size_type map1qu_dat_SIZE( _lmax_struct const & __lmax )const;
	const size_type map1qu_dat_SIZE_max( _lmax_struct const & __lmax )const;
	const size_type map1qu_dat_SIZE_mid( _lmax_struct const & __lmax )const;
	inline void map1qu_dat_size( _lmax_struct const & __lmax ){ this->_map1qu_dat_size = this->map1qu_dat_SIZE( __lmax ); }
	inline size_type const & map1qu_dat_size()const{ return this->_map1qu_dat_size; }
	inline size_type & map1qu_dat_size(){ return this->_map1qu_dat_size; }
	inline const size_type map3qu_pos_N( const _lmax_struct & __lmax )const{return __lmax._la_max + 1;}
	inline const size_type map3qu_pos_M( const _lmax_struct & __lmax )const{return __lmax._lb_max + 1;}
	inline const size_type map3qu_pos_P( const _lmax_struct & __lmax )const
	{
		if( this->is_mapping_min() )
		{
			this->error("map3qu_pos_P( const _lmax_struct & )");
			exit(1);
		}
		return __lmax._l_max + 1 + __lmax._lso_max;
	}
	inline const size_type map3qu_pos_SIZE( const _lmax_struct & __lmax )const
	{return this->map3qu_pos_N( __lmax ) * this->map3qu_pos_M( __lmax ) * this->map3qu_pos_P( __lmax );}
	// READ, WRITE
	const size_type write_map( memorystream & ms, _lmax_struct const & __lmax );
	const size_type read_map( memorystream & ms );
	// initialization
	inline void init_map()
	{
		switch( this->get_mapping() )
		{
		case minimum:
			//this->error("init_map()", "method launch not needed for minimum mapping type" );
			//std::cerr << "minimum mapping type : " << this->get_mapping() << std::endl;
			//exit(1);
			break;
		case middle :
			this->init_map_mid();
			break;
		case maximum:
			this->init_map_max();
			break;
		default:
			this->error("init_map()", "unknown mapping type" );
			std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
			exit(1);
		}
	}
	void init_map_max();
	void init_map_mid();
	void init_default_map1qu_dat();
	const size_type init_map1qu_dat_max_semilocal( const int la, const int lb, const int l );
	const size_type init_map1qu_dat_max_local( const int la, const int lb );
	const size_type init_map1qu_dat_mid_semilocal( const int la, const int lb, const int l );
	const size_type init_map1qu_dat_mid_local( const int la, const int lb );
	// %_M_map3qu_pos
	inline const size_type & M_map3qu_pos_n()const{ return this->_M_map3qu_pos->n(); }
	inline const size_type & M_map3qu_pos_m()const{ return this->_M_map3qu_pos->m(); }
	inline const size_type & M_map3qu_pos_p()const{ return this->_M_map3qu_pos->p(); }
	inline const size_type & M_map3qu_pos_size()const{ return this->_M_map3qu_pos->size(); }
	inline const map3qu_pos_type * M_map3qu_pos_data()const{ return this->_M_map3qu_pos->data(); }
	inline map3qu_pos_type * M_map3qu_pos_data(){ return this->_M_map3qu_pos->data(); }
	// %_M_map1qu_dat
	inline const size_type & M_map1qu_dat_size()const{ return this->_M_map1qu_dat->size(); }
	inline const map1qu_dat_type * M_map1qu_dat_data()const{ return this->_M_map1qu_dat->data(); }
	inline map1qu_dat_type * M_map1qu_dat_data(){ return this->_M_map1qu_dat->data(); }
	// %_M_lmax
	inline const _lmax_struct & M_get_lmax()const{ __assert__( this->_M_lmax != 0 ); return *this->_M_lmax; }
	inline const int & M_get_l_max()const{ __assert__( this->_M_lmax != 0 ); return this->_M_lmax->_l_max; }
	inline const int & M_get_lso_max()const{ __assert__( this->_M_lmax != 0 ); return this->_M_lmax->_lso_max; }
	inline const int & M_get_la_max()const{ __assert__( this->_M_lmax != 0 ); return this->_M_lmax->_la_max; }
	inline const int & M_get_lb_max()const{ __assert__( this->_M_lmax != 0 ); return this->_M_lmax->_lb_max; }

	// qu_radial_map has 2 main charactristics:
	//   1st - position of the beginning of 3d matrix (all of them (3d matrices) are stored in one big array) : position method %map1qu_pos()
	//   2nd - index of unique triple {N,lmb_a,lmb_b} for fixed (la,lb,l) in array that is made by reduction of above 3d matrix : method %qu_idx()
	//   So  why there are 2 main objects: %_M_map3qu_pos, %_M_map1qu_dat
	// some word about the array:
	//   array is made according to 2 conditions:
	//     1st - unique triple {N,lmb_a,lmb_b};
	//     2nd - loop restriction (for more details see %init_map1qu_dat);
	//   array size is gotten by method %qu_size()
	//   P.S. array is virtual (in the imagination =))
	//
	// *********************** INSTRUCTION ***********************
	//
	// ---------------- Initialization of objects ----------------
	//
	// 1. First of all one should set mapping type:
	//   %set_mapping_min() <--> [ A == C == B ]
	//   %set_mapping_mid() <--> [ A != C == B ]
	//   %set_mapping_max() <--> [ A != C != B ]
	// 2. Then allocate memory ( in heap, in stack, whatever you want) with size %comp_size( _lmax_struct const & ) [ in bytes ]
	// 3. Third initialize pointers : method %write_map( memorystream & , _lmax_struct const & )
	//   P.S. %memorystream object should be initialized with allocated memory : method %memorystream::setbuf( void * data, const int size )
	// 4. Forth initialize objects  : method %init_map()
	//
	// -------------------- Usage of objects ---------------------
	//
	// There are 4 situations:
	//   I.a) semi-local part of pseudo-potential (+) maximum mapping type <==> [ A!=C!=B ]
	//   I.b) local      part of pseudo-potential (+) maximum mapping type <==> [ A!=C!=B ]
	//  II.a) semi-local part of pseudo-potential (+) middle  mapping type <==> [ A!=C==B ]
	//  II.b) local      part of pseudo-potential (+) middle  mapping type <==> [ A!=C==B ]
	//
	// I.a.1 - %qu_size()
	// In order to get size of array of unique {N,lmb_a,lmb_b}s for fixed (la, lb, l) there are 4 ways:
	//   1.1 %map3qu_pos_set( %la, %lb, %l )
	//   1.2 %qu_size()
	// 
	//   2.1 %map3qu_pos_set_la( %la )
	//   2.2 %map3qu_pos_set_lb( %lb )
	//   2.3 %map3qu_pos_set_l ( %l )
	//   2.4 %qu_size()
	//
	//   3.1 %qu_set_lx( %la, %lb, %l )
	//   3.2 %qu_size()
	//
	//   4.1 %qu_set_la( %la )
	//   4.2 %qu_set_lb( %lb )
	//   4.3 %qu_set_l ( %l )
	//   4.4 %qu_size()
	//
	// I.b.1 - %qu_size()
	// For local part of potential ( unique {N,lmb} ) use:
	//   1.1.a  %map3qu_pos_set( %la, %lb, this->M_get_l_max() )
	//   1.1.b  %map3qu_pos_set( %la, %lb )
	//   1.2    %qu_size()
	//
	//   ... etc.
	//       
	// P.S. The principle is to replace value of %l (from instruction I.a.1) to %l_max; variant 1.1.b makes it by default
	//
	// I.a.2 - %qu_idx()
	// In order to get index of unique {N,lmb_a,lmb_b} in (virtual) array, one should continue (4 ways):
	//   1.1 %map1qu_dat_set( %N, %lmb_a, %lmb_b )
	//   1.2 %qu_idx()
	//
	//   2.1 %map1qu_dat_set_N( %N )
	//   2.2 %map1qu_dat_set_lmb_a( %lmb_a )
	//   2.3 %map1qu_dat_set_lmb_b( %lmb_b )
	//   2.4 %qu_idx()
	//
	//   3.1 %qu_set_lmbx( %N, %lmb_a, %lmb_b )
	//   3.2 %qu_idx()
	//
	//   4.1 %qu_set_N( %N )
	//   4.2 %qu_set_lmb_a( %lmb_a )
	//   4.3 %qu_set_lmb_b( %lmb_b )
	//   4.4 %qu_idx()
	//
	// I.b.2 - %qu_idx()
	// For local ( index of unique {N,lmb} ):
	//   1.1.a  %map1qu_dat_set( %N, %lmb, 0 )
	//   1.1.b  %map1qu_dat_set( %N, %lmb )
	//   1.2    %qu_idx()
	//
	//   ... etc.
	//
	// P.S. method 1.1.a uses : ( %N * matrix_m() + %lmb ) * matrix_p() + 0
	//      method 1.1.b uses : %N * matrix_m() + %lmb
	// P.P.S matrix_p() value is equal to 1 for this case (I.b)
	//
	// II.a.1 - Usage is same as for I.a.1
	// II.b.1 - Usage is same as for I.b.1
	// II.a.2 - Use methods of I.b.2 <-- not a typo
	// II.b.2 - Use methods of I.b.2
	//
	// In conclusion: usage of local part for both maximum and middle mapping type is same
	//  semi-local part usage:
	//  I. A != C != B
	//    I.a.1
	//    I.a.2
	// II. A != C == B
	//    II.a.1 <--> I.a.1
	//    II.a.2 <--> I.b.2
	//  local part usage:
	//  I. A != C != B
	//    I.b.1
	//    I.b.2
	// II. A != C == B
	//    II.b.1 <--> I.b.1
	//    II.b.2 <--> I.b.2
	inline void map3qu_pos_set_la(const size_type la)
	{
		__assert__( this->_M_map3qu_pos != 0 );
		this->_map3qu_pos_it_la = la * this->_M_map3qu_pos->m();
	}
	inline void map3qu_pos_set_lb(const size_type lb)
	{
		__assert__( this->_M_map3qu_pos != 0 );
		this->_map3qu_pos_it_lb = this->_map3qu_pos_it_la + lb;
		this->_map3qu_pos_it_lb *= this->_M_map3qu_pos->p();

		this->_map3qu_pos_it_m = this->_M_map3qu_pos->data() + this->_map3qu_pos_it_lb;
	}
	inline void map3qu_pos_set_lmax(){ this->map3qu_pos_set_l( this->M_get_l_max() ); }
	inline void map3qu_pos_set_l(const size_type l )
	{
		__assert__( this->_M_map3qu_pos != 0 );
		__assert__( this->_map3qu_pos_it_m != 0 );
		this->_map3qu_pos_it_p = this->_map3qu_pos_it_m + l;
		this->_sz3_it = &this->_map3qu_pos_it_p->_sz3;
	}
	inline void map3qu_pos_set( const size_type la, const size_type lb, const size_type l)
	{
		__assert__( this->_M_map3qu_pos != 0 );
		this->_map3qu_pos_it_p = this->_M_map3qu_pos->data() + (la * this->_M_map3qu_pos->m() + lb) * this->_M_map3qu_pos->p() + l;
		_sz3_it = &this->_map3qu_pos_it_p->_sz3;
	}
	inline void map3qu_pos_set( const size_type la, const size_type lb)
	{
		__assert__( this->_M_map3qu_pos != 0 );
		this->_map3qu_pos_it_p = this->_M_map3qu_pos->data();
		this->_map3qu_pos_it_p += (la * this->_M_map3qu_pos->m() + lb) * this->_M_map3qu_pos->p() + this->M_get_l_max();
		_sz3_it = &this->_map3qu_pos_it_p->_sz3;
	}

	inline size_type & map1qu_n(){ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->n();}
	inline size_type & map1qu_m(){ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->m();}
	inline size_type & map1qu_p(){ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->p();}
	inline size_type & map1qu_size(){ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->size();}

	inline size_type const & map1qu_n()const{ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->n();}
	inline size_type const & map1qu_m()const{ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->m();}
	inline size_type const & map1qu_p()const{ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->p();}
	inline size_type const & map1qu_size()const{ __assert__( this->_sz3_it != 0 ); return this->_sz3_it->size();}

	inline int & map1qu_pos()
	{
		__assert__( this->_map3qu_pos_it_p != 0 );
		return this->_map3qu_pos_it_p->_pos; // position of the beginning of mx3 data
	}
	inline int const & map1qu_pos()const
	{
		__assert__( this->_map3qu_pos_it_p != 0 );
		return this->_map3qu_pos_it_p->_pos; // position of the beginning of mx3 data
	}
	inline map1qu_dat_type const * map1qu_data()const
	{
		__assert__( this->_M_map1qu_dat != 0 );
		return this->_M_map1qu_dat->data() + this->map1qu_pos();// data of mx3
	}
	inline map1qu_dat_type * map1qu_data()
	{
		__assert__( this->_M_map1qu_dat != 0 );
		return this->_M_map1qu_dat->data() + this->map1qu_pos(); // data of mx3
	}

	inline int & qu_size()
	{
		__assert__( this->_map3qu_pos_it_p != 0 );
		return this->_map3qu_pos_it_p->_size; // size for qu computation
	}
	inline int const & qu_size()const
	{
		__assert__( this->_map3qu_pos_it_p != 0 );
		return this->_map3qu_pos_it_p->_size; // size for qu computation
	}

	inline void map1qu_dat_set_N(const size_type N){ this->_map1qu_dat_it_N = N * this->map1qu_m(); }
	inline void map1qu_dat_set_lmb_a(const size_type lmb_a)
	{
		this->_map1qu_dat_it_lmb_a = this->_map1qu_dat_it_N + lmb_a;
		this->_map1qu_dat_it_lmb_a *= this->map1qu_p();

		this->_map1qu_dat_it_m = this->map1qu_data() + this->_map1qu_dat_it_lmb_a;
	}
	inline void map1qu_dat_set_lmb_b(const size_type lmb_b)
	{
		__assert__( this->_map1qu_dat_it_m != 0 );
		this->_map1qu_dat_it_p = this->_map1qu_dat_it_m + lmb_b;
	}
	inline void map1qu_dat_set( const size_type N, const size_type lmb_a, const size_type lmb_b)
	{
		this->_map1qu_dat_it_p = this->map1qu_data() + (N * this->map1qu_m() + lmb_a) * this->map1qu_p() + lmb_b;
	}
	inline void map1qu_dat_set( const size_type N, const size_type lmb){ this->_map1qu_dat_it_p = this->map1qu_data() + N * this->map1qu_m() + lmb;}
	inline void map1qu_dat_set_i( const int i){ this->_map1qu_dat_it_p = this->map1qu_data() + i; }
	inline map1qu_dat_type const & qu_it()const
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		return *this->_map1qu_dat_it_p;
	}
	inline int & qu_idx()
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		return this->_map1qu_dat_it_p->_idx;
	}
	inline int const & qu_idx()const
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		return this->_map1qu_dat_it_p->_idx;
	}
	inline const bool qu_idx_is_good()const
	{
		return this->qu_idx() != MAP1QU_DAT_INIT_NUM;
	}
	inline int const & qu_n()const
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		return this->_map1qu_dat_it_p->_n_times;
	}
	inline const bool qu_is_on()  const{ return (this->qu_n() & 0x1u); }
	inline const bool qu_is_done()const{ return (this->qu_n() & 0x2u); }
	inline void qu_set_done()
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		if( this->qu_is_on() )
			this->_map1qu_dat_it_p->_n_times |= (0x2u);// | this->qu_n()) ;
	}
	inline void qu_on()
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		this->_map1qu_dat_it_p->_n_times = 0x1u;
	}
	inline void qu_off()
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		this->_map1qu_dat_it_p->_n_times = 0x0u;
	}
	inline void qu_reset_done()
	{
		__assert__( this->_map1qu_dat_it_p != 0 );
		if( this->qu_is_on() )
			this->_map1qu_dat_it_p->_n_times &= (~(0x2u));
	}
	void qu_reset_done_all();
	// Easy interface -> merge 'map3qu_pos' and 'map1qu_dat' into 'qu' : %map3qu_pos_set_la => %qu_set_la, etc.
	inline void qu_set_la( const int la ){ this->map3qu_pos_set_la( la ); }
	inline void qu_set_lb( const int lb ){ this->map3qu_pos_set_lb( lb ); }
	inline void qu_set_l( const int l ){ this->map3qu_pos_set_l( l ); }
	inline void qu_set_lmax(){ this->map3qu_pos_set_lmax(); }
	inline void qu_set_lx( const int la, const int lb, const int l ){ this->map3qu_pos_set( la, lb, l ); }
	inline void qu_set_lx( const int la, const int lb ){ this->map3qu_pos_set( la, lb ); }

	inline void qu_set_N( const int N ){ this->map1qu_dat_set_N( N ); }
	inline void qu_set_lmb_a( const int lmb_a ){ this->map1qu_dat_set_lmb_a( lmb_a ); }
	inline void qu_set_lmb_b( const int lmb_b ){ this->map1qu_dat_set_lmb_b( lmb_b ); }
	inline void qu_set_lmbx( const int N, const int lmb_a, const int lmb_b ){ this->map1qu_dat_set( N, lmb_a, lmb_b ); }
	inline void qu_set_lmbx( const int N, const int lmb ){ this->map1qu_dat_set( N, lmb ); }
};

#endif//__QU_RADIAL_MAP_H__
