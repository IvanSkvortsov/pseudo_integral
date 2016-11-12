#ifndef __PSEUSO_STRUCT_H__
#define __PSEUSO_STRUCT_H__

//#define NDEBUG

#include"basis.set.h"
#include"ecp.set.h"

#include"ixs.omega.h"

#include"alpha.mem.h"
#include"alpha.slm.h"
#include"geom.slm.h"

#include"ixs.angular.mem.h"
#include"ixs.angular.dat.h"
#include"ixs.angular.idx.h"

#include"qu.radial.mem.h"
#include"qu.radial.dat.h"

#include"pseudo.integral.mem.h"

#include"get.work.time.h"

#include<fstream>
#include<vector>
#include"vector.3d.h"

#define __PSEUDO_STRUCT_TIME
#define __PSEUDO_STRUCT_DEBUG
#define __PSEUDO_STRUCT_SAFE
#define __PSEUDO_STRUCT_LOG

#if defined (__PSEUDO_STRUCT_TIME) && defined (__PSEUDO_STRUCT_LOG)
  #include"get.work.time.h"
  #include"smart.time.h"
  #define __START_LOG( SRC_PTR, STR ) this->log( (SRC_PTR), (STR) ); get_work_time t2; t2.counting_start()
  #define __END_LOG                   t2.counting_end(); std::clog << smart_time( t2 ) << std::endl
  #define __START_PSEUDO_STRUCT( t1 ) this->log( this, "pseudo_struct()" ); std::clog << std::endl; t1.counting_start()
  #define __END_PSEUDO_STRUCT( t1 )   t1.counting_end(); this->log( this, "~pseudo_struct()" ); std::clog << smart_time( t1 ) << std::endl
#elif defined (__PSEUDO_STRUCT_LOG)
  #define __START_LOG( SRC_PTR, STR ) this->log( (SRC_PTR), (STR) )
  #define __END_LOG
  #define __START_PSEUDO_STRUCT( t1 ) this->log( this, "pseudo_struct()" )
  #define __END_PSEUDO_STRUCT( t1 )   this->log( this, "~pseudo_struct()" )
#endif//__PSEUDO_STRUCT_TIME

#ifdef  __PSEUDO_STRUCT_LOG
  #define __MEMORY_MAPPED( str, _msize )\
	do{\
		const int _sz = (_msize);\
		fprintf( stderr, "%s::size : %d b -- %.2f Kb -- %.2f Mb\n", str, (_sz), (double)(_sz)/1024, (double)(_sz)/(1024*1024) );\
	}while(0)
  #define __MAPPING_TYPE( type )\
	do{\
		std::clog << "mapping type : ";\
		const int _type = (type);\
		switch( _type ){\
		case minimum: std::clog << "min" << std::endl; break;\
		case middle : std::clog << "mid" << std::endl; break;\
		case maximum: std::clog << "max" << std::endl; break;\
		default: std::clog << "unknown" << std::endl;\
		}\
	}while(0)
  #define __INTEGRAL_COUNT( _source )\
	do{\
		if( (_source).get_local_count() < 0 ) break;\
		std::clog << " semi-local count = " << (_source).get_semi_local_count() << "; local count = " << (_source).get_local_count();\
	}while(0)
#else
  #define __MAPPING_TYPE( type )
  #define __MEMORY_MAPPED( str, _msize )
  #define __INTEGRAL_COUNT( _source )
#endif//__PSEUDO_STRUCT_LOG

#define __PSEUDO_STRUCT_DEBUG
#if defined (__PSEUDO_STRUCT_DEBUG) || defined (__PSEUDO_STRUCT_SAFE)
  #include<cassert>
#endif

#define __XSTR( s ) __STR( s )
#define __STR( s ) #s

#define __PSEUDO_STRUCT_MMANIP( method, source )\
inline void method##_create( char const * file )\
{\
	__START_LOG( &(source), __XSTR( method##_create( const char * ) ) );\
	(source).memory_create( file );\
	__END_LOG;\
	__MEMORY_MAPPED( __XSTR( method ), (source).size() );\
}\
inline void method##_open( char const * file )\
{\
	__START_LOG( &(source), __XSTR( method##_open( const char * ) ) );\
	(source).memory_open( file );\
	__END_LOG;\
	__MEMORY_MAPPED( __XSTR( method ), (source).size() );\
}\
inline void method##_close()\
{\
	__START_LOG( &(source), __XSTR( method##_close() ) );\
	(source).close();\
	__END_LOG;\
}

#define __PSEUDO_STRUCT_BASIS_SET( method, source, U )\
inline void set_##method( basis_set<U> const * __bas_X )\
{\
	__START_LOG( &(source), __XSTR( set_##method( basis_set<U> const * ) ) );\
	(source) = __bas_X;\
	__END_LOG;\
}\
inline basis_set<U> const * get_##method()const{ return (source); }\
inline const int get_##method##_lx_max()const{ return (source)->get_lx_max(); }\
inline const int * get_##method##_alp_lx_data()const{ return (source)->get_alp_lx_data(); }\
inline const int * get_##method##_fun_lx_data()const{ return (source)->get_fun_lx_data(); }\
\
inline const int get_##method##_fun_size()const{ return (source)->get_fun_size(); }\
inline const U * get_##method##_fun_data()const{ return (source)->get_fun_data(); }\
inline const int get_##method##_alp_size()const{ return (source)->get_alp_size(); }\
inline const U * get_##method##_alp_data()const{ return (source)->get_alp_data(); }\
\
inline const int get_##method##_alp_size(int lx)const{ return (*source)[lx].get_alp_size(); }\
inline const U * get_##method##_alp_data(int lx)const{ return (*source)[lx].get_alp_data(); }\
inline const int get_##method##_fun_size(int lx)const{ return (*source)[lx].get_fun_size(); }\
\
inline const int get_##method##_fun_size(int lx, int i)const{ return (*source)[lx][i].get_fun_size(); }\
inline const U * get_##method##_fun_data(int lx, int i)const{ return (*source)[lx][i].get_fun_data(); }

#define __PSEUDO_STRUCT_ECP_SET( method, source, U )\
inline void set_##method( ecp_set<U> const * __ecp )\
{\
	__START_LOG( &(source), __XSTR( set_##method( ecp_set<U> const * ) ) );\
	(source) = __ecp;\
	__END_LOG;\
}\
inline ecp_set<U> const * get_##method()const{ return (source); }\
\
inline const int get_##method##_l_max()const{ return (source)->get_l_max(); }\
inline const int get_##method##_lso_max()const{ return (source)->get_lso_max(); }\
inline const int get_##method##_n_core()const{ return (source)->get_n_core(); }\
inline const int * get_##method##_alp_lx_data()const{ return (source)->get_alp_lx_data(); }\
inline const int * get_##method##_fun_lx_data()const{ return (source)->get_fun_lx_data(); }\
\
inline const U * get_##method##_alp_data()const{ return (source)->get_alp_data(); }\
inline const int get_##method##_alp_size()const{ return (source)->get_alp_size();}\
inline const U * get_##method##_fun_data()const{ return (source)->get_fun_data(); }\
inline const int get_##method##_fun_size()const{ return (source)->get_fun_size();}\
inline const int * get_##method##_n_data()const{ return (source)->get_n_data(); }\
\
inline const U * get_##method##_alp_data( int l )const{ return (*source)[l].get_alp_data();}\
inline const U * get_##method##_fun_data( int l )const{ return (*source)[l].get_fun_data();}\
inline const int get_##method##_fun_size( int l )const{ return (*source)[l].get_fun_size();}\
inline const int * get_##method##_n_data( int l )const{ return (*source)[l].get_n_data();}

#define __PSEUDO_STRUCT_SYNC( source1, source2, source3, source4 )\
inline void sync_mapping()\
{\
	__START_LOG( &(source1), "sync_mapping()");\
	/*this->log("sync_mapping()", &(source1), "alpha_mem<T,U>");*/\
	(source1).mapping_struct::operator=( *this );\
	/*this->log("sync_mapping()", &(source2), "ixs_angular_mem<T,U>");*/\
	(source2).mapping_struct::operator=( *this );\
	/*this->log("sync_mapping()", &(source3), "geom_slm<T,U>");*/\
	(source3).mapping_struct::operator=( *this );\
	/*this->log("sync_mapping()", &(source4), "gu_radial_mem");*/\
	(source4).mapping_struct::operator=( *this );\
	__END_LOG;\
}\
inline void sync_lmax()\
{\
	__START_LOG( &(source1), "sync_lmax()");\
	/*this->log("sync_lmax()", &(source1), "alpha_mem<T,U>");*/\
	(source1).put_l_max( this->get_ecp_l_max() );\
	(source1).put_lso_max( this->get_ecp_lso_max() );\
	(source1).put_la_max( this->get_basA_lx_max() );\
	(source1).put_lb_max( this->get_basB_lx_max() );\
\
	/*this->log("sync_lmax()", &(source2), "ixs_angular_mem<T,U>");*/\
	(source2).set_lmax( (source1).get_lmax() );\
\
	/*this->log("sync_lmax()", &(source3), "geom_slm<T,U>");*/\
	(source3).set_lmax( (source1).get_lmax() );\
	(source3).init_lsize();\
\
	/*this->log("sync_lmax()", &(source4), "gu_radial_mem");*/\
	(source4).set_lmax( (source1).get_lmax() );\
	__END_LOG;\
}\
inline void sync_alpsize()\
{\
	__START_LOG( &(source1), "sync_alpsize()");\
	/*this->log("sync_alpsize()", &(source1), "alpha_mem<T,U>" );*/\
	(source1).put_a_size( this->get_basA_alp_size() );\
	(source1).put_b_size( this->get_basB_alp_size() );\
	(source1).put_c_size( this->get_ecp_alp_size() );\
	(source1).set_A_size( this->get_basA_alp_lx_data() );\
	(source1).set_B_size( this->get_basB_alp_lx_data() );\
	(source1).set_C_size( this->get_ecp_alp_lx_data() );\
	__END_LOG;\
}\
inline void sync_funsize()\
{\
	__START_LOG( &(source1), "sync_funsize()");\
	(source1).set_A_dsize( this->get_basA_fun_lx_data() );\
	(source1).set_B_dsize( this->get_basB_fun_lx_data() );\
	(source1).set_C_dsize( this->get_ecp_fun_lx_data() );\
	__END_LOG;\
}\
inline void sync()\
{\
	this->sync_mapping();\
	this->sync_lmax();\
	this->sync_alpsize();\
	this->sync_funsize();\
}

#define __PSEUDO_STRUCT_DATA_POINTER( method, source )\
inline void set_##method##_data( U const * __ptr )\
{\
	__START_LOG( &(source), __XSTR( set_##method##_data( U const * ) ) );\
	(source) = __ptr;\
	__END_LOG;\
}\
inline U * get_##method##_data(int i){ return (source) + i; }\
inline U const * get_##method##_data(int i)const{ return (source) + i; }\
inline U * get_##method##_data(){ return (source); }\
inline U const * get_##method##_data()const{ return (source); }\
inline U & get_##method(int i){ return (source)[i]; }\
inline U const & get_##method(int i)const{ return (source)[i]; }

#define __IS_VEC3D_ZERO( vec ) ( (vec)[0] == 0 && (vec)[1] == 0 && (vec)[2] == 0 )

// TODO: exp( x ), x = ( -alp_C * (alp_A * |CA|^2 + alp_B * |CB|^2) + -alp_A * alp_B * |BA|^2 ) / alp; alp = alp_A + alp_B + alp_C
//       check for zero integral value - if( |x| > log( std::numeric_limits<T>::max() ) ); then value = 0; fi;
//
// pseudo_struct:
// 
// usage:
// 0.  %ixs_omg_open( const char * )
//
// 1.1 %set_CA_data( U const * )
// 1.2 %set_CB_data( U const * )
//
// 2.1 %set_basA( basis_set<U> const * )
// 2.2 %set_basB( basis_set<U> const * )
// 2.3 %set_ecp( ecp_set<U> const * )
//
// 3.1 %set_mapping_geom()
// 3.2 %flip()
// 3.3 %sync()
// 3.4 %set_sqr_len()
//
// 4.1 %alp_mem_create( const char * )
// 4.2 %ixs_ang_create( const char * )
// 4.3 %geom_slm_write()
// 4.4 %qu_mem_create( const char * )
//
// 5.0 %mxslm_read( const char * )
// 5.1 %init()
// 5.2 %ixs_dat_create( const char * )
// 5.2 %qu_dat_create( const char * )
// 5.3 %init_qu_dat()
// 5.4 %pseudo_create( const char * )
// 5.5 %init_pseudo()
//
// 6.1 %comp_ang()
// 6.2 %comp_qu()
// 6.3 %comp_pseudo()
template<typename T, typename U = T>
struct pseudo_struct : public mapping_struct
{
	typedef typename mapping_struct::mapping_enum mapping_enum;

	basis_set<U> const * bas_A, * bas_B;
	ecp_set<U> const * ecp;
	U const * CA, * CB, * C;
	U sqr_CA, sqr_CB, sqr_AB;

	ixs_omega<T,U> ixs_omg;

	matrix_slm<U> mx_slm;

	alpha_mem<T,U> alp_mem;
	geom_slm<U> geom_s;
	alpha_slm<T,U> alp_s;

	ixs_angular_mem ixs_ang;
	ixs_angular_dat<T,U> ixs_dat;

	qu_radial_mem qu_mem;
	qu_radial_dat<T,U> qu_dat;
#ifdef  __PSEUDO_STRUCT_TIME
	get_work_time t1;
#endif
	bool _to_flip;

	pseudo_integral_mem<T,U> pseudo;

	inline void error( char const * _method, const char _message[] = "nothing to do here" )const
	{ std::cerr << "Error: [" << this << "] pseudo_struct<T,U>::" << _method << ", " << _message << std::endl; }
	inline void log( void const * _source, char const * _method_name  )const
	{
#ifdef  __PSEUDO_STRUCT_LOG
		std::clog << "Log: [" << _source << "]" <<
			std::setw(6) << (const char *)_source - (const char *)this << " pseudo_struct<T,U>::" << _method_name;
  #ifndef __PSEUDO_STRUCT_TIME
		std::clog << std::endl;
  #endif
#endif
	}

	inline pseudo_struct(): mapping_struct(), bas_A(), bas_B(), ecp(), CA(), CB(),
		ixs_omg(), mx_slm(), alp_mem(), geom_s(), alp_s(), ixs_ang(), ixs_dat(), qu_mem(), qu_dat(){ __START_PSEUDO_STRUCT( t1 ); }
	inline ~pseudo_struct(){ __END_PSEUDO_STRUCT( t1 ); }

	__PSEUDO_STRUCT_DATA_POINTER( CA, this->CA );
	__PSEUDO_STRUCT_DATA_POINTER( CB, this->CB );
	__PSEUDO_STRUCT_DATA_POINTER( C, this->C );
	inline void set_sqr_len()
	{
		__START_LOG( &(this->sqr_CA), "set_sqr_len()" );
		this->sqr_CA = 0;
		this->sqr_CB = 0;
		this->sqr_AB = 0;
		switch( this->get_mapping() )
		{
		case minimum: break;
		case middle :
			for(int i = 0; i < 3; ++i)
				this->sqr_CA += this->CA[i] * this->CA[i];
			this->sqr_AB = this->sqr_CA;
			break;
		case maximum:
			for(int i = 0; i < 3; ++i)
				this->sqr_CA += this->CA[i] * this->CA[i];
			for(int i = 0; i < 3; ++i)
				this->sqr_CB += this->CB[i] * this->CB[i];
			for(int i = 0; i < 3; ++i)
				this->sqr_AB += (this->CA[i] - this->CB[i]) * (this->CA[i] - this->CB[i]);
			break;
		}
		__END_LOG;
	}
	inline void set_mapping_geom()
	{
		const int __is_CA_zero = __IS_VEC3D_ZERO( this->CA );
		const int __is_CB_zero = __IS_VEC3D_ZERO( this->CB );
		if( __is_CA_zero && __is_CB_zero )
		{
			this->set_mapping_min();
		} else if( __is_CB_zero ){
			this->set_mapping_mid();
			this->set_not_to_flip();
		} else if( __is_CA_zero ){
			this->set_mapping_mid();
			this->set_to_flip();
		} else {
			this->set_mapping_max();
		}
		__MAPPING_TYPE( this->get_mapping() );
	}
	inline void set_to_flip(){ this->_to_flip = true; }
	inline void set_not_to_flip(){ this->_to_flip = false; }
	inline const bool to_flip()const{ return this->_to_flip; }
	inline void flip()
	{
		if( this->to_flip() && this->is_mapping_mid() )
		{
			U const * _tmp = this->CA;
			this->CA = this->CB;
			this->CB = _tmp;
			basis_set<U> const * _b_tmp = this->bas_A;
			this->bas_A = this->bas_B;
			this->bas_B = _b_tmp;
			// TODO: remove
			const int __ax__[3] = {to_compute::get_ax(), to_compute::get_ay(), to_compute::get_az()};
			const int __bx__[3] = {to_compute::get_bx(), to_compute::get_by(), to_compute::get_bz()};
			to_compute_set_a_xyz( __bx__ );
			to_compute_set_b_xyz( __ax__ );
		}
	}

	__PSEUDO_STRUCT_BASIS_SET( basA, this->bas_A, U );
	__PSEUDO_STRUCT_BASIS_SET( basB, this->bas_B, U );
	__PSEUDO_STRUCT_ECP_SET( ecp, this->ecp, U );

	__PSEUDO_STRUCT_SYNC( this->alp_mem, this->ixs_ang, this->geom_s, this->qu_mem );

	__PSEUDO_STRUCT_MMANIP( alp_mem, this->alp_mem );
	__PSEUDO_STRUCT_MMANIP( ixs_omg, this->ixs_omg );
	__PSEUDO_STRUCT_MMANIP( ixs_ang, this->ixs_ang );
	__PSEUDO_STRUCT_MMANIP( ixs_dat, this->ixs_dat );
	__PSEUDO_STRUCT_MMANIP(  qu_mem, this->qu_mem );
	__PSEUDO_STRUCT_MMANIP(  qu_dat, this->qu_dat );
	__PSEUDO_STRUCT_MMANIP(  pseudo, this->pseudo );

	inline mapping_enum const & get_geom_mapping()const{ return this->geom_s.get_mapping(); }
	inline mapping_enum const & get_alps_mapping()const{ return this->alp_s.get_mapping(); }
	inline mapping_enum const & get_alp_mapping()const{ return this->alp_mem.get_mapping(); }
	inline mapping_enum const & get_ixs_mapping()const{ return this->ixs_ang.get_mapping(); }

	inline void geom_slm_write(){
		__START_LOG( &(this->geom_s), "geom_slm_write" );
		this->geom_s.write();
		__END_LOG;
	}

	inline void mxslm_read( const char * file ){
		__START_LOG( &(this->mx_slm), "mxslm_read( const char * )" );
		this->mx_slm.read( file );
		__END_LOG;
	}
	inline void init_alp()
	{
		__START_LOG( &(this->alp_s), "init_alp" );
		this->alp_mem.init_map();
		this->alp_mem.alpha_val<T,U>::init_mx1_alp( this->get_basA_alp_data(), this->get_basB_alp_data(), this->get_ecp_alp_data() );
		this->alp_mem.alpha_val<T,U>::init_mx1C_nk( this->get_ecp_n_data() );
		this->alp_mem.alpha_val<T,U>::init_norm( this->get_basA_alp_data(), this->get_basB_alp_data() );
		this->alp_mem.alpha_val<T,U>::init_exp( this->get_basA_alp_data(), this->get_basB_alp_data(), this->sqr_CA, this->sqr_CB );
		this->alp_s.alpha_pow<T,U>::operator=( this->alp_mem );
		this->alp_s.init_mxslm( this->CA, this->get_basA_alp_data(), this->CB, this->get_basB_alp_data(), this->mx_slm );
		this->pseudo.alpha_val<T,U>::operator=( this->alp_mem );//set_alpha( this->alp_mem );
		__END_LOG;
	}
	inline void init_ixs()
	{
		__START_LOG( &(this->ixs_dat), "init_ixs" );
		this->ixs_ang.init_map( this->alp_s );
		this->ixs_dat.ixs_angular_map::operator=( this->ixs_ang );
		__END_LOG;
	}
	inline void init_geom()
	{
		__START_LOG( &(this->geom_s), "init_geom" );
		this->geom_s.init_geom( this->CA, this->CB, this->mx_slm );
		__END_LOG;
	}
	inline void init_qu()
	{
		__START_LOG( &(this->qu_dat), "init_qu" );
		this->qu_mem.init_map();
		this->qu_dat.set_rad_map( this->qu_mem );
		this->qu_dat.set_alp_map( this->alp_mem );
		__END_LOG;
	}
	inline void init()
	{
		this->init_alp();
		this->init_ixs();
		this->init_geom();
		this->init_qu();
	}
	inline void init_qu_dat()
	{
		__START_LOG( &(this->qu_dat), "init_qu_dat()" );
		this->qu_dat.init_dat();
		__END_LOG;
	}
	inline void init_pseudo()
	{
		__START_LOG( &(this->pseudo), "init_pseudo()" );
		this->pseudo.init_map();
		__END_LOG;
	}

	inline void comp_ang();
	inline void comp_qu();
	inline void comp_pseudo();
	inline void run_pseudo( const char * file );

	template<typename FloatType> inline static void print_cx( std::ostream & out, FloatType const * r3, const char * name )
	{
		out << std::setw(10) << name << ": ";
		for( int i = 0; i < 3; ++i )
			out << std::setw(25) << std::setprecision(15) << std::scientific << r3[i];
		out << std::endl;
	}
	inline void print_cx( std::ostream & out = std::clog)const
	{
		vector_3d<U> A( this->CA );
		A -= this->C;
		A = -A;
		vector_3d<U> B( this->CB );
		B -= this->C;
		B = -B;
		pseudo_struct<T,U>::print_cx( out, A.data(), "A" );
		pseudo_struct<T,U>::print_cx( out, B.data(), "B" );
		pseudo_struct<T,U>::print_cx( out, this->C,  "C" );
		pseudo_struct<T,U>::print_cx( out, this->CA, "CA" );
		pseudo_struct<T,U>::print_cx( out, this->CB, "CB" );
	}
};

template<typename T, typename U> void pseudo_struct<T,U>::comp_ang()
{
	__START_LOG( &(this->ixs_dat), "comp_ang()" );
#ifdef  __PSEUDO_STRUCT_SAFE
	assert( this->get_mapping() == this->get_geom_mapping() );
	assert( this->get_mapping() == this->get_alps_mapping() );
	assert( this->get_mapping() == this->get_alp_mapping() );
	assert( this->get_mapping() == this->get_ixs_mapping() );
#endif
	this->print_cx();
	this->ixs_dat.comp_ang( this->geom_s, this->alp_s, this->qu_dat, this->ixs_omg );
	__END_LOG;
#ifdef  __PSEUDO_STRUCT_SAFE
	//assert( this->ixs_dat.M_node_size() == 0 );
#endif
}

template<typename T, typename U> void pseudo_struct<T,U>::comp_qu()
{
	__START_LOG( &(this->qu_dat), "comp_qu()" );
#ifdef  __PSEUDO_STRUCT_SAFE
	assert( this->get_mapping() == this->get_geom_mapping() );
	assert( this->get_mapping() == this->get_alps_mapping() );
	assert( this->get_mapping() == this->get_alp_mapping() );
	assert( this->get_mapping() == this->get_ixs_mapping() );
#endif
	this->qu_dat.comp_qu();
	__END_LOG;
}

template<typename T, typename U> void pseudo_struct<T,U>::comp_pseudo()
{
	__START_LOG( &(this->pseudo), "comp_pseudo()" );
#ifdef  __PSEUDO_STRUCT_SAFE
	assert( this->get_mapping() == this->get_geom_mapping() );
	assert( this->get_mapping() == this->get_alps_mapping() );
	assert( this->get_mapping() == this->get_alp_mapping() );
	assert( this->get_mapping() == this->get_ixs_mapping() );
#endif
	this->pseudo.comp_pseudo( this->qu_dat, this->ixs_dat, *(this->bas_A), *(this->ecp), *(this->bas_B) );
	__INTEGRAL_COUNT( this->pseudo );
	__END_LOG;
}

static void read_files( std::vector<std::string> & vf, char const * file )
{
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
		std::cerr << "Error: [files] can't open file '" << file << "'" << std::endl;
		exit(1);
	}
	vf.reserve( 10 );
	vf.clear();
	std::string fs;
	while( true )
	{
		inp >> fs;
		if( inp == 0 ) break;
		vf.push_back( fs );
	}
}

static void print_files( std::string const * fs, const int size )
{
	for(int i = 0; i < size; ++i)
		std::cout << "file[" << i << "] : '" << fs[i] << "'" << std::endl;
}

template<typename T> struct geom_struct
{
	vector_3d<T> A, B, C, CA, CB;
	void read( const char * file );
};

template<typename T> inline void geom_struct<T>::read( const char * file )
{
	std::ifstream inp( file );
	if( !inp.is_open() )
	{
		std::cerr << "Error: [" << this << "] geom_struct<T>::" << "read" << ", can't open file '" << file << "'" << std::endl;
		exit(1);
	}
	for(int i = 0; i < 3; ++i) inp >> this->A[i];
	for(int i = 0; i < 3; ++i) inp >> this->B[i];
	for(int i = 0; i < 3; ++i) inp >> this->C[i];
	this->CA = this->C;
	this->CA -= this->A;
	this->CB = this->C;
	this->CB -= this->B;
}

// 0. basis file
// 1. ecp file
// 2. geometry file
// 3. ixs_omega file
// 4. alpha file
// 5. angular map file
// 6. qu radial map file
// 7. matrix slm file
// 8. angular dat file
// 9. qu radial data file
template<typename T, typename U> void pseudo_struct<T,U>::run_pseudo( char const * file )
{
	// files_read
	std::vector<std::string> vf;
	read_files( vf, file );
	print_files( vf.data(), vf.size() );
	// basis_read, ecp_read
	static basis<U> bs;
	static ecp_set<U> ecp;
	bs.read( vf[0].c_str() );
	ecp.read( vf[1].c_str() );
	// geom_read
	static geom_struct<U> gs;
	gs.read( vf[2].c_str() );
	// ixs_omg_open
	this->ixs_omg_open( vf[3].c_str() );
	// set_CX_data
	this->set_CA_data( gs.CA.data() );
	this->set_CB_data( gs.CB.data() );
	this->set_C_data(  gs.C.data() );
	// set_basX, set_ecp
	this->set_basA( bs.data() );
	if( bs.size() > 1 )
		this->set_basB( bs.data() + 1 );
	else
		this->set_basB( bs.data() );
	this->set_ecp( &ecp );
	// set_mapping
	this->set_mapping_geom();
	this->flip();
	this->sync();
	this->set_sqr_len();
	//
	this->alp_mem_create( vf[4].c_str() );
	this->ixs_ang_create( vf[5].c_str() );
	this->geom_slm_write();
	this->qu_mem_create( vf[6].c_str() );
	//
	this->mxslm_read( vf[7].c_str() );
	this->init();
	this->ixs_dat_create( vf[8].c_str() );
	this->qu_dat_create( vf[9].c_str() );
	this->init_qu_dat();
	this->pseudo_create( vf[10].c_str() );
	this->init_pseudo();
	//this->alp_mem.test_alp_map();
	//this->alp_mem.test_alp_pow();
	//
	this->comp_ang();
	this->comp_qu();
	//this->comp_pseudo();
	pseudo_index idx;
	return;
}

#endif//__PSEUSO_STRUCT_H__
