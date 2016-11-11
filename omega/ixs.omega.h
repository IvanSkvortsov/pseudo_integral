#ifndef __IXS_OMEGA_H__
#define __IXS_OMEGA_H__
#include"memory.map.h"
#include"memorystream.h"
#include"matrix.cursor.h"
#include"xyz.pown.integral.h"
#include"matrix.slm.h"
#include<iomanip>

#define INTEGRAL_COUNT 0x800000u// 2^23 ~ 8*10^6

#define __IXS_OMEGA_CALC_ITERATOR// calculate numbers of integrals
#define __IXS_OMEGA_CALC_PRINT// print some info into stdout when running calculation of integrals
// P.S. if defining %CALC_PRINT it's hardly recomended to define also %CALC_ITERATOR, otherwise it prints all integral values =)

#ifdef  __IXS_OMEGA_CALC_PRINT
  #ifndef __IXS_OMEGA_CALC_ITERATOR
  #define __IXS_OMEGA_CALC_ITERATOR
  #endif
#endif

#define __IXS_OMEGA_DEBUG
#ifdef  __IXS_OMEGA_DEBUG
  #include<cassert>
#endif

//#define __IXS_OMEGA_VALUE_SHORT

#define __IXS_OMEGA_ITER_INIT_LIST _node0_it_l(), _node0_it_lx(), _node1_it(), _node1_it_lambda(), _node1_it_abc(), _node1_sz2()

template<typename T, typename U = T>
class ixs_omega : public memory_map, public memorystream
{
public:
#pragma pack( push, 4 )
	typedef struct
	{
		int _l_max, _lx_max, _node1_size, _node2_size;
	} _lsize_struct;
	typedef struct
	{
		int _pos;
		size_struct<2> _sz2;
	} _pos2_struct;
	typedef struct
	{
		int _pos, _size;
	} _pos1_struct;
	typedef struct
	{
#ifdef  __IXS_OMEGA_VALUE_SHORT
		int m, mu;
#else
		int l, lx, lmb, i_abc, m, mu;
#endif
		T d;
	} _value_struct;
#pragma pack( pop )

	typedef int size_type;
	__DATA_TYPEDEF( T );// 'matrix.cursor.h'

	typedef memory_map::mode_type mode_type;
	typedef memorystream::pos_type pos_type;
	typedef memorystream::off_type off_type;
	typedef memorystream::streamsize streamsize;
	typedef memorystream::seek_dir seek_dir;

	typedef _value_struct value_struct;

protected:
	_lsize_struct _lsize;
	_lsize_struct * _M_lsize;
	matrix_cursor_2<_pos2_struct>  * _M_node0;
	matrix_cursor_1<_pos1_struct>  * _M_node1;
	matrix_cursor_1<_value_struct> * _M_node2;
	// P.S. _M_$name -- '_M' denotes storing variable on binary file

	_pos2_struct * _node0_it_l, * _node0_it_lx;
	_pos1_struct * _node1_it, * _node1_it_lambda, * _node1_it_abc;
	size_struct<2> * _node1_sz2;
protected:
	inline void log(char const * __method, char const * __message, std::ostream & out = std::cout )const
	{
		out << "Log [" << this << "] ixs_omega<T>::" << __method << ", " << __message << std::endl;
	}
public:
	ixs_omega();
	ixs_omega(ixs_omega<T,U> const & );
	ixs_omega<T,U> & operator=(ixs_omega<T,U> const & );

	inline void sync_stream(){ this->setbuf( this->data(), this->size() );}

	// size computations:
	const size_type comp_size()const;

	// %_M_node0 stored as is (2d array)
	// %_M_node1 is actually 1d array (%M_node1_$method - methods of 1d array)
	//   %node1_$method - 2d array methods
	// %_M_node2 - 1d array - storage of all integral values
	// N, M, SIZE - upper case literals denotes computation of returning variables
	const size_type node0_N()const;
	const size_type node0_M()const;
	const size_type M_node1_SIZE();
	const size_type M_node2_SIZE(char const * __file);

	inline _value_struct * M_node2_data(){ return this->_M_node2->data();}
	inline  _pos1_struct * M_node1_data(){ return this->_M_node1->data();}
	inline _value_struct const * M_node2_data()const{ return this->_M_node2->data();}
	inline  _pos1_struct const * M_node1_data()const{ return this->_M_node1->data();}
	inline size_type const & M_node2_size()const{ return this->_M_node2->size();}
	inline size_type const & M_node1_size()const{ return this->_M_node1->size();}
	inline size_type & M_node2_size(){ return this->_M_node2->size();}
	inline size_type & M_node1_size(){ return this->_M_node1->size();}

	inline size_type & node0_n(){ return this->_M_node0->n();}
	inline size_type & node0_m(){ return this->_M_node0->m();}
	inline size_type & node0_size(){ return this->_M_node0->size();}
	inline _pos2_struct * node0_data(){ return this->_M_node0->data();}
	inline const size_type & node0_n()const{ return this->_M_node0->n();}
	inline const size_type & node0_m()const{ return this->_M_node0->m();}
	inline const size_type & node0_size()const{ return this->_M_node0->size();}
	inline const _pos2_struct * node0_data()const{ return this->_M_node0->data();}

	void node0_init();

	void memory_create( const char * file, int __flags = O_RDWR| O_CREAT| O_TRUNC, mode_type __mode = memory_map::MODE_644 );
	void memory_open( const char * file, int __flags = O_RDONLY, mode_type __mode = memory_map::MODE_444 );

	inline void pub_info(std::ostream & out = std::cout)const;

	// this->_lsize
	inline void set_l_max( const size_type & __l_max ){ this->_lsize._l_max = __l_max;}
	inline void set_lx_max( const size_type & __lx_max ){ this->_lsize._lx_max = __lx_max;}
	inline void set_node1_size( const size_type & __node1_size ){ this->_lsize._node1_size = __node1_size;}
	inline void set_node2_size( const size_type & __node2_size ){ this->_lsize._node2_size = __node2_size;}

	// this->_M_lsize
	inline size_type & M_lsize_l_max(){ return this->_M_lsize->_l_max;}
	inline size_type & M_lsize_lx_max(){ return this->_M_lsize->_lx_max;}
	inline size_type & M_lsize_node1_size(){ return this->_M_lsize->_node1_size;}
	inline size_type & M_lsize_node2_size(){ return this->_M_lsize->_node2_size;}
	inline size_type const & M_lsize_l_max()const{ return this->_M_lsize->_l_max;}
	inline size_type const & M_lsize_lx_max()const{ return this->_M_lsize->_lx_max;}
	inline size_type const & M_lsize_node1_size()const{ return this->_M_lsize->_node1_size;}
	inline size_type const & M_lsize_node2_size()const{ return this->_M_lsize->_node2_size;}

	//inline size_type & node0_l_max(){ return this->_lsize._l_max;}
	//inline size_type & node0_lx_max(){ return this->_lsize._lx_max;}
	inline size_type const & node0_l_max()const{ return this->_lsize._l_max;}
	inline size_type const & node0_lx_max()const{ return this->_lsize._lx_max;}

	// node0 iterating interface
	// use %node0_set_lmax() - to get access to %l = 0, and double range of %lx = 0..( %_lsize._lx_max * 2 )
	// unless use %node0_set_l( %l ), range of %lx = 0..( %_lsize._lx_max )
	//
	// usage:
	// %node0_set_l( %l );
	// %node0_set_lx( %lx );
	// %node1_set_lambda_i( %i_lmb );
	// %node1_set_abc_i( %i_abc );
	//  and you got position, and size of not zero integrals :
	//    Int( x^a * y^b * z^c * S(%l,m) * S(%lmb, mu), d\Omega);
	//    (a, b, c) <-> %i_abc, a + b + c = %lx
	//    lmb       <-> %i_lmb
	//  %node2_pos()  - position related to %_M_node2->data()
	//  %node2_size() - size of integrals with fixed %l, %lx, %i_lmb, %i_abc
	//  %node2_data() = %_M_node2->data() + %node2_pos();
#ifdef  __IXS_OMEGA_DEBUG
	inline void node0_set_lmax(){ this->node0_set_l( this->node0_l_max() + 1);}
	inline void node0_set_l(const int l){ this->_node0_it_l = this->node0_data() + l * this->node0_m();}
	inline void node0_set_lx( const int lx)
	{
		assert( this->_node0_it_l != 0 );
		this->_node0_it_lx = this->_node0_it_l;
		this->_node0_it_lx += lx;
		this->_node1_sz2 = &(this->_node0_it_lx->_sz2);
		this->_node1_it = this->M_node1_data() + this->node1_pos();
		//this->_node1_it = this->M_node1_data() + this->_node0_it_lx->_pos;
	}
	inline const _pos2_struct * node0_it()const
	{
		assert( this->_node0_it_lx != 0 );
		return this->_node0_it_lx;
	}
	inline _pos2_struct * node0_it()
	{
		assert( this->_node0_it_lx != 0 );
		return this->_node0_it_lx;
	}
	inline const size_type & node1_pos()const
	{
		assert( this->_node0_it_lx != 0 );
		return this->_node0_it_lx->_pos;
	}
	inline void node1_pos(const size_type __node1_pos)
	{
		assert( this->_node0_it_lx != 0 );
		this->_node0_it_lx->_pos = __node1_pos;
	}

	inline _pos1_struct * node1_data()
	{
		assert( this->_node1_it != 0 );
		return this->_node1_it;
	}
	inline _pos1_struct const * node1_data()const
	{
		assert( this->_node1_it != 0 );
		return this->_node1_it;
	}
	inline size_type & node1_n()
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->n();
	}
	inline size_type & node1_m()
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->m();
	}
	inline size_type & node1_size()
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->size();
	}
	inline const size_type & node1_n()const
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->n();
	}
	inline const size_type & node1_m()const
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->m();
	}
	inline const size_type & node1_size()const
	{
		assert( this->_node1_sz2 !=  0 );
		return this->_node1_sz2->size();
	}
	inline void node1_set_lambda_i( const int _i)
	{
		assert( this->node1_data() != 0 );
		assert( this->node1_n() > _i );
		this->_node1_it_lambda = this->node1_data() + this->node1_m() * _i;
	}
	inline void node1_set_abc_i( const int _i)
	{
		assert( this->_node1_it_lambda != 0 );
		assert( this->node1_m() > _i );
		this->_node1_it_abc = this->_node1_it_lambda + _i;
	}

	inline size_type & node2_pos()
	{
		assert( this->_node1_it_abc != 0 );
		return this->_node1_it_abc->_pos;
	}
	inline size_type & node2_size()
	{
		assert( this->_node1_it_abc != 0 );
		return this->_node1_it_abc->_size;
	}
	inline const size_type & node2_pos()const
	{
		assert( this->_node1_it_abc != 0 );
		return this->_node1_it_abc->_pos;
	}
	inline const size_type & node2_size()const
	{
		assert( this->_node1_it_abc != 0 );
		return this->_node1_it_abc->_size;
	}
	inline const _value_struct * node2_data()const{ return this->M_node2_data() + this->node2_pos();}
	inline _value_struct * node2_data(){ return this->_M_node2->data( this->node2_pos() );}
	inline _value_struct & node2_value(const int i){ return *(this->node2_data() + i);}
	inline const _value_struct & node2_value(const int i)const{ return *(this->node2_data() + i);}
#else
	inline void node0_set_lmax(){ this->node0_set_l( this->node0_l_max() + 1);}
	inline void node0_set_l(const int l){ this->_node0_it_l = this->node0_data() + l * this->node0_m();}
	inline void node0_set_lx( const int lx)
	{
		this->_node0_it_lx = this->_node0_it_l;
		this->_node0_it_lx += lx;
		this->_node1_sz2 = &(this->_node0_it_lx->_sz2);
		//this->_node1_it = this->M_node1_data() + this->node1_pos();
		this->_node1_it = this->M_node1_data() + this->_node0_it_lx->_pos;
	}
	inline const size_type & node1_pos()const{ return this->_node0_it_lx->_pos;}
	inline void node1_pos(const size_type __node1_pos){ this->_node0_it_lx->_pos = __node1_pos;}

	inline _pos1_struct * node1_data(){ return this->_node1_it;}
	inline _pos1_struct const * node1_data()const{ return this->_node1_it;}
	inline size_type & node1_n(){ return this->_node1_sz2->n();}
	inline size_type & node1_m(){ return this->_node1_sz2->m();}
	inline size_type & node1_size(){ return this->_node1_sz2->size();}
	inline const size_type & node1_n()const{ return this->_node1_sz2->n();}
	inline const size_type & node1_m()const{ return this->_node1_sz2->m();}
	inline const size_type & node1_size()const{ return this->_node1_sz2->size();}

	inline void node1_set_lambda_i( const int _i){ this->_node1_it_lambda = this->node1_data() + this->node1_m() * _i;}
	inline void node1_set_abc_i( const int _i){ this->_node1_it_abc = this->_node1_it_lambda + _i;}

	inline size_type & node2_pos(){ return this->_node1_it_abc->_pos;}
	inline size_type & node2_size(){ return this->_node1_it_abc->_size;}
	inline const size_type & node2_pos()const{ return this->_node1_it_abc->_pos;}
	inline const size_type & node2_size()const{ return this->_node1_it_abc->_size;}
	inline const _value_struct * node2_data()const{ return this->M_node2_data() + this->node2_pos();}
	inline _value_struct * node2_data(){ return this->_M_node2->data( this->node2_pos() );}
	inline _value_struct & node2_value(const int i){ return *(this->node2_data() + i);}
	inline const _value_struct & node2_value(const int i)const{ return *(this->node2_data() + i);}
#endif//__IXS_OMEGA_DEBUG

	// simplified User Interface (recommended)
	inline void set_lmax(){ this->node0_set_lmax();}
	inline void set_l( const int l){ this->node0_set_l( l );}
	inline void set_lx( const int lx){ this->node0_set_lx( lx );}
	inline void set_lambda_i( const int _i){ this->node1_set_lambda_i( _i );}
	inline void set_abc_i( const int _i){ this->node1_set_abc_i( _i );}
	inline const _value_struct * get_omg_data()const{ return this->node2_data();}
	inline const _value_struct * get_omg_data(int i)const{ return this->node2_data() + i;}
	inline const size_type & get_omg_size()const{ return this->node2_size();}

#ifndef __IXS_OMEGA_VALUE_SHORT
	inline const int get_omg_l()const{ return this->get_omg_data()->l; }
	inline const int get_omg_lx()const{ return this->get_omg_data()->lx; }
	inline const int get_omg_lmb()const{ return this->get_omg_data()->lmb; }
	inline const int get_omg_i_abc()const{ return this->get_omg_data()->i_abc; }
#endif
	inline const int get_omg_m()const{ return this->get_omg_data()->m; }
	inline const int get_omg_mu()const{ return this->get_omg_data()->mu; }

	const size_type llx_size()const;
	// integral calculation
	const size_type run_calc(std::vector<std::string> const & vs_filename, char const * file, xyz_pown_integral<U> const & xpn_int);
	const size_type calc_omega( size_type __pos, size_type l, size_type na,\
			matrix_slm<U> const & mx_slm, xyz_pown_integral<U> const & xpn_int);
	// test
	//void test_set_matrices();
	//void test_set_matrix( arr_pointer_t & __data, mx_t * mx_lmb_abc, size_type l, size_type na );
	//
	int shrink_to_fit(char const * file);
protected:
	inline void error( const char * method_name, const char message[] = "nothing to do here" )const;
};

template<typename T, typename U> void ixs_omega<T,U>::error( const char * method_name, const char * message )const
{
	std::cerr << "Error: [" << this << "] ixs_omega<T,U>::" << method_name << ", " << message << std::endl;
}
template<typename T, typename U> void ixs_omega<T,U>::pub_info(std::ostream & out)const
{
	out << "[" << this << "] INFO : ixs_omega " << sizeof(T) << std::endl;
	out << "map.data : [" << this->memory_map::data() << "]" << std::endl;
	out << "map.size : " << this->memory_map::size() << std::endl;
	if( this->data() && this->size() )
		out << "map.end  : [" << (void *)( (char *)(this->data()) + this->size()) << "]" << std::endl;
	out << "lsize.l_max  : " << this->_lsize._l_max << std::endl;
	out << "lsize.lx_max : " << this->_lsize._lx_max << std::endl;
	out << "lsize.node1_size : " << this->_lsize._node1_size << std::endl;
	out << "lsize.node2_size : " << this->_lsize._node2_size << std::endl;
}
#endif//__IXS_OMEGA_H__
