#ifndef __PSEUSO_INTEGRAL_H__
#define __PSEUSO_INTEGRAL_H__

#include"basis.set.h"
#include"ecp.set.h"

#include"pseudo.index.h"

//#include"ixs.omega.h"

//#include"alpha.mem.h"
//#include"alpha.slm.h"
//#include"geom.slm.h"

//#include"ixs.angular.mem.h"
#include"ixs.angular.dat.h"

//#include"qu.radial.mem.h"
#include"qu.radial.dat.h"

//#include"get.work.time.h"



//#include"memorystream.h"
//#include"matrix.cursor.h"
//#include"mapping.t.h"
//#include"alpha.map.h"

#define __PSEUDO_INTEGRAL_DEBUG
#ifdef  __PSEUDO_INTEGRAL_DEBUG
  #include<cassert>
  #define __pseudo_assert__( arg ) assert( (arg) )
#else
  #define __pseudo_assert__( arg )
#endif

#define __PSEUDO_INTEGRAL_MX3( mx3, mx3_value_type )\
inline const size_type & M_##mx3##_n()const{ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->n();}\
inline const size_type & M_##mx3##_m()const{ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->m();}\
inline const size_type & M_##mx3##_p()const{ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->p();}\
inline const size_type & M_##mx3##_size()const{ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->size();}\
inline const mx3_value_type * M_##mx3##_data()const{ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->data();}\
inline mx3_value_type * M_##mx3##_data(){ __pseudo_assert__( this->_M_##mx3 != 0 ); return this->_M_##mx3->data();}

#define __PSEUDO_INTEGRAL_PRIM_FUNC_INTERFACE( X, map1A, map1B, map1C, map3ABC )\
/* M_mx1prim_data(), ...; M_mx1func_data(), ... etc.*/\
inline const size_type & M_mx1##X##_size()const\
{\
	__pseudo_assert__( this->_M_mx1##X != 0 );\
	return this->_M_mx1##X->size();\
}\
inline const_pointer M_mx1##X##_data()const\
{\
	__pseudo_assert__( this->_M_mx1##X != 0 );\
	return this->_M_mx1##X->data();\
}\
inline pointer M_mx1##X##_data()\
{\
	__pseudo_assert__( this->_M_mx1##X != 0 );\
	return this->_M_mx1##X->data();\
}\
/* mx1prim_set_it(), map3node_prim_pos(), ...*/\
inline void mx1##X##_set_it()\
{\
	__pseudo_assert__( this->_map3node_it_l != 0 );\
	this->_mx1##X##_it  = this->M_mx1##X##_data() + this->map3node_##X##_pos();\
}\
inline pointer mx1##X##_it()\
{\
	__pseudo_assert__( this->_mx1##X##_it != 0 );\
	return this->_mx1##X##_it;\
}\
inline const_pointer mx1##X##_it()const\
{\
	__pseudo_assert__( this->_mx1##X##_it != 0 );\
	return this->_mx1##X##_it;\
}\
inline int & map3node_##X##_pos()\
{\
	__pseudo_assert__( this->_map3node_it_l != 0 );\
	return this->_map3node_it_l->_##X##_pos;\
}\
inline int & map3node_##X##_size()\
{\
	__pseudo_assert__( this->_map3node_it_l != 0 );\
	return this->_map3node_it_l->_##X##_size;\
}\
inline int const & map3node_##X##_pos()const\
{\
	__pseudo_assert__( this->_map3node_it_l != 0 );\
	return this->_map3node_it_l->_##X##_pos;\
}\
inline int const & map3node_##X##_size()const\
{\
	__pseudo_assert__( this->_map3node_it_l != 0 );\
	return this->_map3node_it_l->_##X##_size;\
}\
/* la, lb, l iteration interface */\
inline void X##_set_la( const int la )\
{\
	this->alpha_map::map1A##_set_lx( la );\
	/*this->alpha_map::map3ABC##_set_la( la );*/\
}\
inline void X##_set_axyz( const int i ){ this->map3node_set_axyz( i ); }\
inline void X##_set_lb( const int lb )\
{\
	this->alpha_map::map1B##_set_lx( lb );\
	/*this->alpha_map::map3ABC##_set_lb( lb );*/\
}\
inline void X##_set_bxyz( const int i ){ this->map3node_set_bxyz( i ); }\
inline void X##_set_l( const int l )/* need map3node_set_l( int ) to be set*/\
{\
	this->alpha_map::map1C##_set_lx(  l );\
	/*this->alpha_map::map3ABC##_set_l( l );*/\
	this->mx1##X##_set_it();\
}\
inline void X##_set_lmax(){ this->X##_set_l( this->alpha_map::l_max() ); }\
/*alpha iteration*/\
inline void X##_set_ia( const int i )\
{\
	this->alpha_map::map1A##_set_ix( i );\
	this->_mx1##X##_it_ia = i * this->alpha_map::map1B##_size();\
}\
inline const int X##_a_idx()const{ return this->alpha_map::map1A##_idx(); }\
inline void X##_set_ib( const int i )\
{\
	this->alpha_map::map1B##_set_ix( i );\
	this->_mx1##X##_it_ib = (this->_mx1##X##_it_ia + i) * this->alpha_map::map1C##_size();\
}\
inline const int X##_b_idx()const{ return this->alpha_map::map1B##_idx(); }\
inline void X##_set_ic( const int i )\
{\
	this->alpha_map::map1C##_set_ix( i );\
	this->_mx1##X##_it_ic = this->_mx1##X##_it_ib + i;\
}\
inline const int X##_c_idx()const{ return this->alpha_map::map1C##_idx(); }\
inline const int X##_idx()const{ return this->_mx1##X##_it_ic; }\
inline void X##_set_idx()\
{\
	__pseudo_assert__( this->X##_idx() < this->map3node_##X##_size() && this->X##_idx() >= 0 );\
	/* P.S. %map3node_##X##_size() - values inited with %map3ABC_size() ( so that they are equal to each other, check it out)*/\
	/*__pseudo_assert__( this->map3node_##X##_size() == this->alpha_map::map3ABC##_size() );*/\
	this->_mx1##X##_it_idx = this->mx1##X##_it() + this->X##_idx();\
}\
inline pointer X##_ptr()\
{\
	__pseudo_assert__( this->_mx1##X##_it_idx != 0 );\
	return this->_mx1##X##_it_idx;\
}\
inline const_pointer X##_ptr()const\
{\
	__pseudo_assert__( this->_mx1##X##_it_idx != 0 );\
	return this->_mx1##X##_it_idx;\
}\
inline reference X##_value()\
{\
	__pseudo_assert__( this->_mx1##X##_it_idx != 0 );\
	return *this->_mx1##X##_it_idx;\
}\
inline const_reference X##_value()const\
{\
	__pseudo_assert__( this->_mx1##X##_it_idx != 0 );\
	return *this->_mx1##X##_it_idx;\
}

#define __PSEUDO_INTEGRAL_ITER_INIT_LIST \
_map3node_it_la(), _map3node_it_axyz(), _map3node_it_lb(), _map3node_it_bxyz(), _map3node_it_l(),\
_mx1prim_it_ia(), _mx1prim_it_ib(), _mx1prim_it_ic(), _mx1prim_it_idx(), _mx1prim_it(),\
_mx1func_it_ia(), _mx1func_it_ib(), _mx1func_it_ic(), _mx1func_it_idx(), _mx1func_it()

#define __PSEUDO_INTEGRAL_COUNT
#define __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
//#define __PSEUDO_INTEGRAL_COMP_FUNC_PRINT

struct pseudo_norm
{
protected:
	bool _to_norm;
public:
	inline pseudo_norm(): _to_norm(){}
	inline pseudo_norm( pseudo_norm const & v ): _to_norm( v._to_norm ){}
	inline void set_to_norm(){ this->_to_norm = true; }
	inline void set_not_to_norm(){ this->_to_norm = false; }
	inline const bool to_norm()const{ return this->_to_norm; }
};

template<typename T, typename U>
struct pseudo_integral : public pseudo_norm, public alpha_val<T,U>
{
public:
	typedef typename memorystream::pos_type pos_type;
	typedef typename memorystream::off_type off_type;
	typedef typename memorystream::seek_dir seek_dir;

	typedef typename size_struct<1>::size_type size_type;
	typedef typename alpha_siz::_lmax_struct _lmax_struct;
	typedef typename alpha_map::_pos1_struct _pos1_struct;
#pragma pack( push, 4 )
	typedef struct
	{
		int _prim_pos, _prim_size;
		int _func_pos, _func_size;
	} _pseudo_node_struct;
#pragma pack( pop )
	typedef _pseudo_node_struct mx3node_type;
	__DATA_TYPEDEF( T );
	typedef struct
	{
		int la, lb, l, ax, ay, az, bx, by, bz;
	} _lxyz_struct;
#ifdef  __PSEUDO_INTEGRAL_COUNT
	static int semi_local_count;
	inline void set_semi_local_count( const int __count )const{ pseudo_integral<T,U>::semi_local_count = __count; }
	inline const int get_semi_local_count()const{ return pseudo_integral<T,U>::semi_local_count; }
	inline void incr_semi_local_count( const int __plus )const{ pseudo_integral<T,U>::semi_local_count += __plus; }
	static int local_count;
	inline void set_local_count( const int __count )const{ pseudo_integral<T,U>::local_count = __count; }
	inline const int get_local_count()const{ return pseudo_integral<T,U>::local_count; }
	inline void incr_local_count( const int __plus )const{ pseudo_integral<T,U>::local_count += __plus; }
#else
	inline void set_semi_local_count( const int __count )const{}
	inline const int get_semi_local_count()const{ return -1; }
	inline void incr_semi_local_count( const int __plus )const{}
	inline void set_local_count( const int __count )const{}
	inline const int get_local_count()const{ return -2; }
	inline void incr_local_count( const int __plus )const{}
#endif
protected:
	size_type _mx1prim_size, _mx1func_size;
	matrix_cursor_3<mx3node_type> * _M_node;
	matrix_cursor_1<T> * _M_mx1prim;
	matrix_cursor_1<T> * _M_mx1func;

	const size_type comp_mx1prim_size();
	const size_type comp_mx1prim_size_max();
	const size_type comp_mx1prim_size_mid();
	const size_type comp_mx1prim_size_min();
	const size_type comp_mx1func_size();
	const size_type comp_mx1func_size_max();
	const size_type comp_mx1func_size_mid();
	const size_type comp_mx1func_size_min();
	const size_type comp_mx1T_2x_size_max();
	const size_type comp_mx1T_2x_size_mid();
	const size_type comp_mx1T_2x_size_min();

	void init_map_max();
	void init_map_mid();
	void init_map_min();

	void comp_prim_min( ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_min( const int , T const & );

	void comp_prim_mid( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_mid_SemiLocal( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_mid_SemiLocal_b( T & v, _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_mid_Local( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_mid_Local_b( T & v, _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );

	void comp_prim_max( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_max_SemiLocal( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_max_SemiLocal_b( T & v, _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_max_Local( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_prim_max_Local_b( T & v, _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang, const int __it );

	void comp_func_min( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB );
	void comp_func_mid( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB );
	void comp_func_max( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB );

#ifdef  __PSEUDO_INTEGRAL_COMP_FUNC_PRINT
	void comp_func( _lxyz_struct const & _lxyz, basis_lx<U> const & basA, ecp_lx<U> const & ecp, basis_lx<U> const & basB );
	void comp_func_b( _lxyz_struct const & _lxyz, const int ia_f, const int ib_f, const int ic_f,
			T & value, basis_fun<U> const & basA, ecp_fun<U> const & ecp, basis_fun<U> const & basB );
#endif
	void comp_func( basis_lx<U> const & basA, ecp_lx<U> const & ecp, basis_lx<U> const & basB );
	void comp_func_b( T & value, basis_fun<U> const & basA, ecp_fun<U> const & ecp, basis_fun<U> const & basB );

	inline void error(char const * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] pseudo_integral<T,U>::" << _method << ", " << _message << std::endl;}
private:
	int _map3node_it_la, _map3node_it_axyz, _map3node_it_lb, _map3node_it_bxyz;
	mx3node_type * _map3node_it_l;
	int _mx1prim_it_ia, _mx1prim_it_ib, _mx1prim_it_ic;
	T * _mx1prim_it_idx, * _mx1prim_it;

	int _mx1func_it_ia, _mx1func_it_ib, _mx1func_it_ic;
	T * _mx1func_it_idx, * _mx1func_it;
public:
	pseudo_integral();
	pseudo_integral( pseudo_integral<T,U> const & v);
	pseudo_integral<T,U> & operator=(pseudo_integral<T,U> const & v);
	// alpha_map
	inline void set_alpha( alpha_val<T,U> const & v ){ this->alpha_val<T,U>::operator=( v ); }
	// comp_size
	const size_type comp_size();
	const size_type comp_node_size()const;
	const size_type comp_mx1T_2x_size();
	// read, write
	const size_type write_map( memorystream & ms );
	const size_type read_map( memorystream & ms );
	// init
	void init_map();
	void comp_pseudo( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang,
			basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB );
	void comp_prim( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang );
	void comp_func( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB );
	// mx1prim_size, mx1func_size
	inline const size_type & mx1prim_size()const{ return this->_mx1prim_size;}
	inline size_type & mx1prim_size(){ return this->_mx1prim_size;}
	inline const size_type & mx1func_size()const{ return this->_mx1func_size;}
	inline size_type & mx1func_size(){ return this->_mx1func_size;}
	// node
	inline const size_type node_N()const
	{
		size_type __size = this->alpha_map::la_max() + 1;
		__size *= (this->alpha_map::la_max() + 2);
		__size *= (this->alpha_map::la_max() + 3);
		__size /= 6;
		return __size;
	}
	inline const size_type node_M()const
	{
		size_type __size = this->alpha_map::lb_max() + 1;
		__size *= (this->alpha_map::lb_max() + 2);
		__size *= (this->alpha_map::lb_max() + 3);
		__size /= 6;
		return __size;
	}
	inline const size_type node_P()const{return (this->alpha_map::l_max() + 1 + this->alpha_map::lso_max());}
	// USAGE Interface
	// node
	__PSEUDO_INTEGRAL_MX3( node, mx3node_type );
	inline void map3node_set_la( const int lx)
	{
		this->_map3node_it_la = lx * (lx + 1) * (lx + 2);
		this->_map3node_it_la /= 6;
	}
	inline void map3node_set_axyz( const int i)
	{
		this->_map3node_it_axyz = this->_map3node_it_la + i;
		this->_map3node_it_axyz *= this->M_node_m();
	}
	inline void map3node_set_lb( const int lx)
	{
		this->_map3node_it_lb = lx * (lx + 1) * (lx + 2);
		this->_map3node_it_lb /= 6;
		this->_map3node_it_lb += this->_map3node_it_axyz;
	}
	inline void map3node_set_bxyz( const int i)
	{
		this->_map3node_it_bxyz = this->_map3node_it_lb + i;
		this->_map3node_it_bxyz *= this->M_node_p();
	}
	inline void map3node_set_lmax(){ this->map3node_set_l( this->alpha_map::l_max() );}
	inline void map3node_set_l( const int l )
	{
		__pseudo_assert__( this->M_node_data() != 0 );
		this->_map3node_it_l  = this->M_node_data();
		this->_map3node_it_l += this->_map3node_it_bxyz;
		this->_map3node_it_l += l;
	}
	// prim
	__PSEUDO_INTEGRAL_PRIM_FUNC_INTERFACE( prim, map1A, map1B, map1C, map3ABC );
	__PSEUDO_INTEGRAL_PRIM_FUNC_INTERFACE( func, map1A_d, map1B_d, map1C_d, map3ABC_d );
	// Usage: 
	// 1 - set A ang.mom.
	// 1.1   %map3node_set_la( la )
	// 1.2   %prim_set_la( la )
	// 2 - set A x,y,z powers
	// 2.1.a %prim_set_axyz( i ), or
	// 2.1.b %map3node_set_axyz( i )
	// 3 - set B ang.mom.
	// 3.1   %map3node_set_lb( lb )
	// 3.2   %prim_set_lb( lb )
	// 4 - set B x,y,z powers
	// 4.1.a %prim_set_bxyz( i ), or
	// 4.1.b %map3node_set_bxyz( i )
	// 5 - set C (pseudo potential) ang.mom.
	// 5.1   %map3node_set_l( l )
	// 5.2   %prim_set_l( l )
	// 6.    %prim_set_ia( i ); set A primitive number (relative to 0'th primitive for fixed 'A ang.mom.' and fixed 'A xyz powers')
	// 7.    %prim_set_ib( i ); set B primitive number --//--
	// 8.    %prim_set_ic( i ); set C primitive number --//--
	// 10.   %prim_set_idx()
	// 11.   %prim_value(); use it
	const_reference get_prim( pseudo_index const & idx );
	const_reference get_func( pseudo_index const & idx );
};

#endif//__PSEUSO_INTEGRAL_H__
