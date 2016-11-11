#ifndef __IXS_OMEGA_HPP__
#define __IXS_OMEGA_HPP__
#include"ixs.omega.h"
#include"sf.math.h"
#include"omega.integral.h"// omega_integral( a, b, c, slm(l, m), slm(lmb, mu) )
#include<cstring>// memset
//#include"ixs.test.h"
#include<fstream>

template<typename T, typename U>
ixs_omega<T,U>::ixs_omega(): memory_map(), memorystream(), _lsize(),
	_M_lsize(), _M_node0(), _M_node1(), _M_node2(),
	__IXS_OMEGA_ITER_INIT_LIST {}

template<typename T, typename U>
ixs_omega<T,U>::ixs_omega(ixs_omega<T,U> const & v) : memory_map(), memorystream(v), _lsize( v._lsize ),
	_M_lsize(v._M_lsize), _M_node0(v._M_node0), _M_node1(v._M_node1), _M_node2(v._M_node2),
	__IXS_OMEGA_ITER_INIT_LIST {}

template<typename T, typename U>
ixs_omega<T,U> & ixs_omega<T,U>::operator=( ixs_omega<T,U> const & v)
{
	if( this == &v )
		return *this;
	if( this->data() || this->size() )
		this->close();
	ixs_omega<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(ixs_omega<T,U>) );// Danger
	memset( &tmp, 0, sizeof(ixs_omega<T,U>) );// Danger
	return *this;
}

template<typename T, typename U> const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::node0_N()const{ return this->_lsize._l_max + 1 + 2;}
template<typename T, typename U> const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::node0_M()const{ return this->_lsize._lx_max + 1;}

static const int __node1_n(int l, int lx)
{
	return (l < lx ? (l + lx)/2 + 1 : lx + 1);// l < lx ? ( l + lx - (l+lx)%2 )/2 + 1 : ( l + lx - l + lx )/2 + 1
}
static const int __node1_m(int lx)
{
	static int __m;
	__m = (lx + 1);
	__m *= (lx + 2);
	__m /= 2;
	return __m;
}

template<typename T, typename U> const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::M_node1_SIZE()
{
	size_type __size = 0, __node1_size = 0;
	// l = 0
	for(int lx = 0; lx <= this->_lsize._lx_max * 2; ++lx)
	{
		__node1_size  = __node1_n( 0, lx );
		__node1_size *= __node1_m( lx );
		__size += __node1_size;
	}
	for(int l = 1; l <= this->_lsize._l_max; ++l)
	{
		for(int lx = 0; lx <= this->_lsize._lx_max; ++lx)
		{
			__node1_size  = __node1_n( l, lx );
			__node1_size *= __node1_m( lx );
			__size += __node1_size;
		}
	}
	this->_lsize._node1_size = __size;
	return __size;
}
template<typename T, typename U> const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::M_node2_SIZE( const char * __file)
{
	std::ifstream inp( __file );
	if( !inp.is_open() )
	{
		this->error("M_node2_SIZE", "can't open file");
		std::cerr << "file '" << __file << "'" << std::endl;
		exit(1);
	}
	size_type __n_max, __m_max, __size;
	inp >> __n_max >> __m_max >> __size;
	/*
	if( __size != (__n_max * __m_max) )
	{
		this->error("M_node2_SIZE", "not appropriate size");
		std::cerr << "n : " << __n_max << std::endl;
		std::cerr << "m : " << __m_max << std::endl;
		std::cerr << "size : " << __size << std::endl;
		exit(1);
	}
	*/
	if( __n_max <= this->_lsize._l_max || __m_max <= this->_lsize._lx_max )
	{
		this->error("M_node2_SIZE", "not appropriate size");
		std::cerr << "n : " << __n_max << std::endl;
		std::cerr << "m : " << __m_max << std::endl;
		std::cerr << "this.l_max  : " << this->_lsize._l_max << std::endl;
		std::cerr << "this.lx_max : " << this->_lsize._lx_max << std::endl;
		return 0;
	}
	size_type __n, __m, __nm_size, __nm_maxsize, __integral_size = 0;
	for(int i = 0; i < __size; ++i)
	{
		inp >> __n >> __m >> __nm_size >> __nm_maxsize;
		if( inp == 0 )
		{
			this->error("M_node2_SIZE", "unexpected end of file");
			std::cerr << __file << std::endl;
			exit(1);
		}
		if( __n <= this->_lsize._l_max && __m <= (this->_lsize._lx_max * ( __n == 0 ? 2 : 1)) )
			__integral_size += __nm_size;
	}
	this->_lsize._node2_size = __integral_size;
	return __integral_size;
}

template<typename T, typename U>
const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::comp_size()const
{
	size_type __size = 0;
	// 1
	__size += sizeof(int);// value that'll be stored is value of sizeof(T)
	// 2
	__size += sizeof(_lsize_struct);
	// 3
	__size += sizeof(size_struct<2>);
	__size += this->node0_N() * this->node0_M() * sizeof(_pos2_struct);
	// 4
	__size += sizeof(size_struct<1>);
	__size += this->_lsize._node1_size * sizeof(_pos1_struct);
	// 5
	__size += sizeof(size_struct<1>);
	__size += this->_lsize._node2_size * sizeof(_value_struct);
	return __size;
}
// memory_create
template<typename T, typename U> void ixs_omega<T,U>::memory_create( const char * file, int __flags, mode_type __mode)
{
	int _st = this->create( file, this->comp_size(), __flags, __mode );// memory mapping
	if( _st )
	{
		this->error( "memory_create", "can't create file" );
		std::cerr << "map.data : [" << this->data() << "]" << std::endl;
		std::cerr << "map.size : " << this->size() << std::endl;
		std::cerr << "file '" << file << "'" << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	// sizeof(T)
	int _sz = sizeof(T);
	ms.write( &_sz, sizeof(int) );
	// _lsize_struct
	this->_M_lsize = (_lsize_struct *)ms.getcur();
	*this->_M_lsize = this->_lsize;
	ms.seek( sizeof(_lsize_struct), seek_dir::cur );
	// carriage
	union
	{
		void * _void;
		char * _char;
		matrix_cursor_2<_pos2_struct> * _matrix_pos2;
		matrix_cursor_1<_pos1_struct> * _matrix_pos1;
		matrix_cursor_1<_value_struct> * _matrix_val;
	} __carriage;
	// node0
	__carriage._void = ms.getcur();
	this->_M_node0 = __carriage._matrix_pos2;
	this->_M_node0->n() = this->node0_N();
	this->_M_node0->m() = this->node0_M();
	this->_M_node0->init_size();
	ms.seek( sizeof(size_struct<2>) + this->_M_node0->size() * sizeof(_pos2_struct), seek_dir::cur );
	// node1
	__carriage._void = ms.getcur();
	this->_M_node1 = __carriage._matrix_pos1;
	this->_M_node1->size() = this->_lsize._node1_size;
	ms.seek( sizeof(size_struct<1>) + this->_M_node1->size() * sizeof(_pos1_struct), seek_dir::cur );
	// node2
	__carriage._void = ms.getcur();
	this->_M_node2 = __carriage._matrix_val;
	this->_M_node2->size() = this->_lsize._node2_size;
	ms.seek( sizeof(size_struct<1>) + this->_M_node2->size() * sizeof(_value_struct), seek_dir::cur );
	return;
}

// memory mapping: open
template<typename T, typename U> void ixs_omega<T,U>::memory_open( const char * file, int __flags, mode_type __mode )
{
	if( this->open( file, __flags, __mode ) )
	{
		this->error("memory_open", "can't open file");
		std::cerr << "map.data : [" << this->data() << "]" << std::endl;
		std::cerr << "map.size : " << this->size() << std::endl;
		std::cerr << "file '" << file << "'" << std::endl;
		exit(1);
	}
	this->sync_stream();
	memorystream & ms = *this;
	union{ void * _void; char * _char;} _idptr;
	// sizeof(T)
	int _sz = sizeof(T);
	ms.read( &_sz, sizeof(int) );
	if( _sz != sizeof(T) )
	{
		this->error("memory_open", "float type size of mapped memory doesn't coincide with aactual one");
		std::cerr << "this.float_type size : " << sizeof(T) << std::endl;
		std::cerr << "read from memory size: " << _sz << std::endl;
		exit(1);
	}
	// _lsize
	this->_M_lsize = (_lsize_struct *)(ms.getcur());
	this->_lsize = *this->_M_lsize;
	ms.seek( sizeof(_lsize_struct), seek_dir::cur );
	// carriage
	union
	{
		void * _void;
		char * _char;
		matrix_cursor_2<_pos2_struct> * _matrix_pos2;
		matrix_cursor_1<_pos1_struct> * _matrix_pos1;
		matrix_cursor_1<_value_struct> * _matrix_val;
	} __carriage;
	// node
	__carriage._void = ms.getcur();
	this->_M_node0 = __carriage._matrix_pos2;
	ms.seek( sizeof(size_struct<2>) + sizeof(_pos2_struct) * this->_M_node0->size(), seek_dir::cur );
	//
	__carriage._void = ms.getcur();
	this->_M_node1 = __carriage._matrix_pos1;
	ms.seek( sizeof(size_struct<1>) + sizeof(_pos1_struct) * this->_M_node1->size(), seek_dir::cur );
	//
	__carriage._void = ms.getcur();
	this->_M_node2 = __carriage._matrix_val;
	ms.seek( sizeof(size_struct<1>) + sizeof(_value_struct) * this->_M_node2->size(), seek_dir::cur );

#ifdef  __IXS_OMEGA_DEBUG
	if( ms.tell() != this->comp_size() )
	{
		this->error("memory_open", "size inequality" );
		this->pub_info( std::cerr );
		std::cerr << "comp_size : " << this->comp_size() << std::endl;
		std::cerr << "this_tell : " << this->tell() << std::endl;
		std::cerr << "this_bufsize : " << this->bufsize() << std::endl;
		std::cerr << "this_M_node0_size : " << this->_M_node0->size() << std::endl;
		std::cerr << "this_M_node1_size : " << this->M_node1_size() << std::endl;
		std::cerr << "this_M_node2_size : " << this->M_node2_size() << std::endl;
	}
	assert( ms.tell() == this->comp_size() );
#endif
}

template<typename T, typename U>
void ixs_omega<T,U>::node0_init()
{
	size_type __pos = 0;
	this->node0_set_lmax();
	for(int lx = 0; lx <= this->node0_lx_max() * 2; ++lx)
	{
		this->node0_set_lx( lx );
		this->node1_pos(__pos);

		this->node1_n() = __node1_n( 0, lx );
		this->node1_m() = __node1_m( lx );
		this->node1_size() = this->node1_n() * this->node1_m();
		__pos += this->node1_size();
	}
	size_type __pos_lmax = __pos;
	__pos = 0;
	for(int l = 0; l <= this->node0_l_max(); ++l)
	{
		this->node0_set_l( l );
		if( l == 1 )
			__pos = __pos_lmax;
		for(int lx = 0; lx <= this->node0_lx_max(); ++lx)
		{
			this->node0_set_lx( lx );
			this->node1_pos( __pos );

			this->node1_n() = __node1_n( l, lx );
			this->node1_m() = __node1_m( lx );
			this->node1_size() = this->node1_n() * this->node1_m();
			__pos += this->node1_size();
		}
	}
}
// integrals on powers of xyz : int( x^a y^b z^c sin(theta) d theta d phi ) = xpn_int(a, b, c) * 4Pi

template<typename T, typename U>
const typename ixs_omega<T,U>::size_type ixs_omega<T,U>::llx_size()const
{
	size_type __size = 0;
	_pos1_struct const * p = this->node1_data();
	for(int i = 0; i < this->node1_size(); ++i, ++p)
		__size += p->_size;
	return __size;
}
static const int llx_maxsize( const int l, const int lx )
{
	int __size = 0;
	for(int lmb = (l < lx ? (l+lx)%2 : (l-lx)); lmb <= (l+lx); lmb += 2)
		__size += (2*lmb + 1);
	__size *= (2*l+1);
	__size *= (lx+1)*(lx+2);
	__size /= 2;
	return __size;
}

// array:
// ----------------------------
// |                          | - 0 integral value : {int l, n, lmb, i_abc, m, mu; float_type d;}
// |                          | - 1 --//--
// |                          |
// |                          |
// |                          |
// |                          |
// |                          | - (array_size - 1)
// ----------------------------
//
// array_size can't be precalculated (hard nut to cut)
// in order to get array_size one should take 1st calculation and store sizes in matrix
// for that purpose file %file_buf is here =)
// %file_buf structure is follow:
// 1: l_max    la_max    (l_max * la_max)
// 2: 0        0         array_0_0_size                  array_0_0_maxsize
// 3: 0        1         array_0_1_size                  array_0_1_maxsize
// .......................................................................
//  : l_max-1  la_max-1  array_(lmax-1)_(lamax-1)_size   array_(lmax-1)_(lamax-1)_maxsize
// P.S. maxsize is calculated easily
// P.P.S. methods %mxi_size, and %mxi_maxsize calculate size and maxsize
//

template<typename T, typename U> const typename ixs_omega<T,U>::size_type
ixs_omega<T,U>::run_calc( std::vector<std::string> const & vs_filename, const char * file_buf, xyz_pown_integral<U> const & xpn_int)
{
	if( !this->data() || !this->getbuf() || !this->size())
	{
		this->error("run_calc", "no memory mapped, or no memory stream synchronized");
		std::cerr << "mmap : [" << this->data() << "]" << std::endl;
		std::cerr << "mstream : [" << this->getbuf() << "]" << std::endl;
		exit(1);
	}
	// %vs_filename : v - vector, s - spherical harmonic
	// %vs_filename - names of files that store results of spherical harmonics multiplication
	// %file_buf - name of file where numbers of not zero integrals will be written as (%l_max + 1) * (%la_max + 1) sized matrix
	std::ofstream out( file_buf );
	out <<  std::setw(4) << this->node0_l_max()+1u << std::setw(4) << this->node0_lx_max()+1u <<
		std::setw(4) << (this->node0_l_max() + 1u) * (this->node0_lx_max() + 1u) + this->node0_lx_max() <<
		std::endl << std::endl;
	// results of spherical harmonics multiplication: go together with %vs_filename
	matrix_slm<U> mx_slm;// matrix of products of spherical harmonics
	this->seek( (char *)this->M_node2_data() - (char *)this->getbuf() );
	size_type _M_node2_size = 0;
	size_type err_it = 0;
	for(int l = 0, __llx_size = 0; l <= this->node0_l_max(); ++l )
	{
		if( l == 0 )
			this->node0_set_lmax();
		else
			this->node0_set_l( l );
		if( mx_slm.read( vs_filename[l].c_str()) )
		{
			this->error("run_calc", "can't open file");
			std::cerr << vs_filename[l] << std::endl;
			exit(1);
		}
		if( mx_slm.n() != 2*l+1 )
		{
			this->error("run_calc", "l n");
			std::cerr << vs_filename[l] << std::endl;
			std::cerr << "l : " << l << std::endl;
			std::cerr << "n : " << mx_slm.n() << std::endl;
			std::cerr << "n = 2*l+1" << std::endl;
			exit(1);
		}
		for(int lx = 0; lx <= this->node0_lx_max() * ( l == 0 ? 2 : 1 ); ++lx )
		{
			this->node0_set_lx( lx );
			__llx_size = this->calc_omega( _M_node2_size, l, lx, mx_slm, xpn_int);
			_M_node2_size += __llx_size;
			if( __llx_size != this->llx_size() )
			{
				this->error("run_calc", "llx_size");
				std::cerr << "this->llx_size : " << this->llx_size() << std::endl;
				std::cerr << "__llx_size     : " << __llx_size << std::endl;
				++err_it;
				if( err_it > 3 )
					exit(1);
			}
			// %file_buf output : calculate and print sizes
			out <<  std::setw(4) << l << std::setw(4) << lx <<
				std::setw(14) << this->llx_size() <<
				std::setw(14) << llx_maxsize( l, lx ) <<
				std::endl;
		}
		out << std::endl;
	}
	if( this->M_node2_size() > _M_node2_size )
	{
		std::cout << "[" << this << "] ixs_omega<T>::run_calc, array size changed " << std::flush <<
			"from " << this->M_node2_size() << " to " << _M_node2_size << std::endl << std::flush;
		this->M_node2_size() = _M_node2_size;
		this->_M_lsize->_node2_size = _M_node2_size;
		this->_lsize._node2_size = _M_node2_size;
	}
	else if( this->M_node2_size() < _M_node2_size )
	{
		this->error("run_calc", "overflow?");
		std::cerr << "mapped  size : " << this->M_node2_size() << std::endl;
		std::cerr << "current size : " << _M_node2_size << std::endl;
		exit(1);
	}
	return _M_node2_size;
}

#define IXS_OMEGA_CALC_ITERATOR_OUTPUT(iter, iter_total)\
{\
	char buf[256];\
	snprintf(buf, 256, "%8s : %14d\n", "nzero", iter);\
	std::cout << buf;\
	snprintf(buf, 256, "%8s : %14d\n", "total", iter_total);\
	std::cout << buf;\
	snprintf(buf, 256, "%8s : %14.2f\n", "nzero ,%", (iter * 100.) / iter_total);\
	std::cout << buf;\
	std::cout << "----------" << std::endl;\
}

#if defined (__IXS_OMEGA_VALUE_SHORT)
  #define IXS_OMEGA_INTEGRAL_LINE_OUTPUT_TITLE\
	std::cout <<\
		std::setw(4) <<  "l" <<\
		std::setw(4) << "lx" <<\
		std::setw(4) << "lmb" <<\
		std::setw(4) << "in" <<\
		std::setw(4) << "m" << std::setw(4) << "mu" <<\
		std::setw(4) << "sz" << std::setw(8) << "pos" << std::setw(20) << "value" << std::endl << std::endl << std::flush

  #define IXS_OMEGA_INTEGRAL_LINE_OUTPUT( l, lx, lmb, i_abc, m2_size, p ) \
	std::cout <<\
		std::setw(4) << l <<\
		std::setw(4) << lx <<\
		std::setw(4) << lmb <<\
		std::setw(4) << i_abc <<\
		std::setw(4) << p->m << std::setw(4) << p->mu <<\
		std::setw(4) << m2_size << std::setw(10) << p - this->M_node2_data() <<\
		std::setw(20) << std::setprecision(12) << std::scientific << p->d <<\
		" [" << p << "]" <<\
		std::endl
#else
  #define IXS_OMEGA_INTEGRAL_LINE_OUTPUT_TITLE\
	std::cout <<\
		std::setw(4) <<  "l" << std::setw(6) <<  "l'" << ' ' <<\
		std::setw(4) << "lx" << std::setw(6) << "lx'" << ' ' <<\
		std::setw(4) << "lmb" << std::setw(6) << "lmb'" << ' ' <<\
		std::setw(4) << "in" << std::setw(6) << "in'" << ' ' <<\
		std::setw(4) << "m" << std::setw(4) << "mu" << \
		std::setw(4) << "sz" << std::setw(8) << "pos" << std::setw(20) << "value" << std::endl << std::endl << std::flush

  #define IXS_OMEGA_INTEGRAL_LINE_OUTPUT( l, lx, lmb, i_abc, m2_size, p ) \
	std::cout <<\
		std::setw(4) << l << " (" << std::setw(4) << p->l << ")" <<\
		std::setw(4) << lx << " (" << std::setw(4) << p->lx << ")" <<\
		std::setw(4) << lmb << " (" << std::setw(4) << p->lmb << ")" <<\
		std::setw(4) << i_abc << " (" << std::setw(4) << p->i_abc << ")" <<\
		std::setw(4) << p->m << std::setw(4) << p->mu <<\
		std::setw(4) << m2_size << std::setw(10) << p - this->M_node2_data() <<\
		std::setw(20) << std::setprecision(12) << std::scientific << p->d <<\
		" [" << p << "]" <<\
		std::endl
#endif//__IXS_VALUE_SHORT

template<typename T, typename U> const typename ixs_omega<T,U>::size_type
ixs_omega<T,U>::calc_omega( size_type __pos, size_type l, size_type lx, matrix_slm<U> const & mx_slm, xyz_pown_integral<U> const & xpn_int)
{
#if defined (__IXS_OMEGA_CALC_ITERATOR)
	static const size_type ITER_PRINT_POINT = 2000, ITER_PRINT_MAX = 500;// 2048u;// 4096u;// 8192u;
	// ITER_PRINT_MAX - print first %ITER_PRINT_MAX integral values
	// ITER_PRINT_POINT - print each %ITER_PRINT_POINT'th integral value
#endif
	static const U NERE_ZERO = U( math::machine_epsilon<U>() * math::precision_error<U>() );
	static size_type lmb_min, lmb_max;

	lmb_min = (l < lx ? (l+lx)%2 : (l-lx));
	lmb_max = l + lx;

	U omega_value = U(0), tmp(0);
#if defined (__IXS_OMEGA_CALC_ITERATOR)
	size_type iter = 0, iter_total = 0;
	static size_type it_all = 0, it = 0;
#endif
#if defined (__IXS_OMEGA_CALC_PRINT)
	std::cout.setf( std::ios::scientific );
	int prec = 18, w = prec + 8;
  #if defined (__IXS_OMEGA_CALC_ITERATOR)
	if( it < ITER_PRINT_MAX )
  #endif
	{
		IXS_OMEGA_INTEGRAL_LINE_OUTPUT_TITLE;
	}
#endif
	_value_struct * _integral_value;
	size_type __size = 0;
	if( this->node1_size() != ((lmb_max-lmb_min)/2+1) * ((lx+1) * (lx+2))/2 )
	{
		this->error("run_omega", "node1 size");
		exit(1);
	}
	for(size_type lmb = lmb_min, i_lmb = 0, sqr_lmb; lmb <= lmb_max; lmb += 2, ++i_lmb)
	{
		this->node1_set_lambda_i( i_lmb );
		sqr_lmb = lmb * lmb;
#if defined (__IXS_OMEGA_CALC_ITERATOR)
		iter_total += (2*lmb+1);
#endif
		for(size_type a = 0, lx_m_a = lx, i_abc = 0; a <= lx; ++a, --lx_m_a)
		{
			for(size_type b = 0u, c = lx_m_a - b, m2_size; b <= lx_m_a; ++b, --c, ++i_abc)
			{
				this->node1_set_abc_i( i_abc );
				//ixs_test::check_nabc( lx, a, b, c );
				this->node2_pos() = __pos + __size;
				_integral_value = this->node2_data();
				m2_size = 0u;
				for(int m = -int(l), lm_iter = 0; m <= int(l); ++m, ++lm_iter)
				{
					for(int mu = -int(lmb), lmu_iter = sqr_lmb; mu <= int(lmb); ++mu, ++lmu_iter)
					{
						//mx_slm(l+m, lmb * lmb + lmb + mu)
						omega_integral(omega_value, a, b, c, mx_slm(lm_iter, lmu_iter), xpn_int, l, lmb);
					  	tmp = (omega_value < 0 ? -omega_value : omega_value );
#if defined (__IXS_OMEGA_CALC_ITERATOR)
						++it_all;
#endif
						if( tmp < NERE_ZERO )
							continue;
						this->seek( sizeof( _value_struct ), seek_dir::cur );
						++m2_size;// m, mu size
#ifdef  __IXS_OMEGA_VALUE_SHORT
						*_integral_value++ = {.m = m, mu, math::convert_float<T,U>(omega_value)};
#else
						*_integral_value++ = { .l = l, lx, lmb, i_abc, m, mu, math::convert_float<T,U>(omega_value)};
#endif//__IXS_VALUE_SHORT
#if defined (__IXS_OMEGA_CALC_PRINT)
  #if defined (__IXS_OMEGA_CALC_ITERATOR)
						if( it < ITER_PRINT_MAX  )
  #endif
						{
							--_integral_value;
							IXS_OMEGA_INTEGRAL_LINE_OUTPUT( l, lx, lmb, i_abc, m2_size, _integral_value);
							++_integral_value;
						}
  #if defined (__IXS_OMEGA_CALC_ITERATOR)
						else if( it%ITER_PRINT_POINT == 0 )
						{
							std::cout << std::setw(8) << it/ITER_PRINT_POINT << " : " <<
								std::setw(14) << it <<
								std::setw(14) << ITER_PRINT_POINT;
							char buf[256];
							snprintf( buf, 256, "%14d b%14.2f Kb%12.2f Mb\n", this->tell(), this->tell()/1024.,
									this->tell()/(1024.*1024));
							std::cout << buf;
							std::cout << std::flush;
							--_integral_value;
							IXS_OMEGA_INTEGRAL_LINE_OUTPUT( l, lx, lmb, i_abc, m2_size, _integral_value );
							++_integral_value;
						}
  #endif
#endif
#if defined (__IXS_OMEGA_CALC_ITERATOR)
						++iter;
						++it;
#endif
					}
				}
				this->node2_size() = m2_size;// m, mu size
				__size += m2_size;
			}
		}
	}
#if defined (__IXS_OMEGA_CALC_PRINT)
	if( it < ITER_PRINT_MAX )
		std::cout << std::endl;
#endif
#if defined (__IXS_OMEGA_CALC_ITERATOR)
	iter_total *= (2*l+1);
	iter_total *= (lx+1)*(lx+2);
	iter_total /= 2;
	//it_all += iter_total;
#endif
	
#if defined (__IXS_OMEGA_CALC_PRINT) && defined (__IXS_OMEGA_CALC_ITERATOR)
	//if( it < ITER_PRINT_MAX || (l == this->l_max() && lx == this->la_max()) )
	{
	std::cout << std::setw(4) << l << std::setw(4) << lx << std::endl;
	IXS_OMEGA_CALC_ITERATOR_OUTPUT(iter, iter_total);
	IXS_OMEGA_CALC_ITERATOR_OUTPUT(it, it_all);
	std::cout << std::endl;
	}
#endif
	return __size;
}

/*

#define IXS_OMEGA_MATRIX_TITLE( __mx_pos, mx_base_beg, store)\
std::cout << "mx_pos: " << __mx_pos << std::endl;\
std::cout << "mx_base_beg.n : " << mx_base_beg._matrix->n() << std::endl;\
std::cout << "mx_base_beg.m : " << mx_base_beg._matrix->m() << std::endl;\
std::cout << "|   lmax (= mx_base_beg.n-1) : " << mx_base_beg._matrix->n()-1 << std::endl;\
std::cout << "| la_max (= mx_base_beg.m-1) : " << mx_base_beg._matrix->m()-1 << std::endl;\
std::cout << "mx_base_beg.n_total : " << mx_base_beg._matrix->n_total() << std::endl;\
std::cout << "mx_base_beg.m_total : " << mx_base_beg._matrix->m_total() << std::endl;\
std::cout << "|   lmax (= mx_base_beg.n_total-1) : " << mx_base_beg._matrix->n_total()-1 << std::endl;\
std::cout << "| la_max (= mx_base_beg.m_total-1) : " << mx_base_beg._matrix->m_total()-1 << std::endl;\
std::cout << std::endl;\
std::cout << "data [" << store._void << "]" << std::endl;

#define IXS_OMEGA_MATRIX_INFO( it, l, na, mx_store, store, mx_value_cur, mx_base_cur)\
std::cout << "-------------------------------- " << std::setw(3) << it << " --------------------------------" << std::endl;\
std::cout << std::setw(4) << l << std::setw(4) << na << " [" << mx_store._void << "] " <<\
	std::setw(14) << (mx_store._char - store._char) <<\
	std::setw(14) << (mx_value_cur->_1st) <<\
	std::setw(4) << mx_base_cur._matrix->n() << std::setw(4) << mx_base_cur._matrix->m() <<\
	std::endl;

template<typename T, typename U>
void ixs_omega<T,U>::test_set_matrices()
{
	std::cout << "----------------------------------" << std::endl;
	std::cout << "TEST: ixs_omega<T,U>::test_set_matrices() " << sizeof(T) << std::endl << std::endl;
	this->pub_info();
	this->_M_variable._variable->pub_info();
	std::cout << "----------------------------------" << std::endl << std::endl;
	//if( ((var_t&)(*this)) != (*this->_M_variable._variable) )
	if( this->var_t::operator!=(*this->_M_variable._variable) )
	{
		this->error("test_set_matrices", "variable not same");
		this->var_t::pub_info();
		this->_M_variable._variable->pub_info();
		exit(1);
	}
	memorystream __ms = *this;
	__ms.seek( this->mx_pos() );
	_matrixPtr mx_base_beg = {__ms.getcur()}, mx_base_cur;
	ixs_matrix_t * mx_beg = &this->_matrix_beg, * mx_cur = &this->_matrix_cur;
	if( mx_beg->base() != mx_base_beg._matrix )
	{
		this->error( "test_set_matrices", "wrong matrix position");
		std::cerr << "mx_pos : " << this->mx_pos() << std::endl;
		std::cerr << "mx_base_beg : [" << mx_base_beg._void << "]" << std::endl;
		std::cerr << "mx_beg      : [" << mx_beg->base() << "]" << std::endl;
		exit(1);
	}
	union{void * _void; int z; char * _char; mx_t * _matrix; mx_value_t * _mx_value;} store = {this->getbuf()}, mx_store;

	IXS_OMEGA_MATRIX_TITLE( this->mx_pos(), mx_base_beg, store);

	mx_value_t * mx_value_beg = mx_beg->data(), * mx_value_cur;
	__ms.seek( this->arr_pos() );
	_arrayValuePtr __array_value_cur = {__ms.getcur()}, __array_value_beg = __array_value_cur;
	std::cout << std::setw(4) << "l" << std::setw(4) << "na" << std::endl << std::endl;
	static std::size_t err_it = 0u, ERR_ITER_MAX = 10;
	union {void * v; int z;} __ptr;
	for(std::size_t l = 0, it = 0; l < mx_beg->n(); ++l, mx_value_beg += mx_beg->m_total() )
	{
		mx_value_cur = mx_value_beg;
		for(std::size_t na = 0; na < mx_beg->m(); ++na, ++mx_value_cur, ++it )
		{
			__ms.seek( mx_value_cur->_1st );
			mx_base_cur = {__ms.getcur()};
			mx_store._void = mx_base_cur._void;

			IXS_OMEGA_MATRIX_INFO( it, l, na, mx_store, store, mx_value_cur, mx_base_cur);

			if( mx_value_cur->_1st != (mx_store._char - store._char) || mx_base_cur._matrix->n() != (l<na? (l+na)/2+1 : na+1) ||
					mx_base_cur._matrix->m() != ((na+1)*(na+2))/2 )
			{
				std::cerr << "Error: ixs_omega<T,U>::test_set_matrices() " << sizeof(T) << std::endl;
				__ptr.z = mx_value_cur->_1st;
				std::cerr << "store   : " << store._void << std::endl;
				std::cerr << "mx_store: " << mx_store._void << std::endl;
				std::cerr << "mx0.pos : " << mx_value_cur->_1st << ' ' << __ptr.v << std::endl;
				__ptr.z = (mx_store._char - store._char);
				std::cerr << "    pos : " << (mx_store._char - store._char) << ' ' << __ptr.v << std::endl;
				__ptr.z = mx_base_cur._matrix->n();
				std::cerr << "mxi.n   : " << mx_base_cur._matrix->n() << ' ' << __ptr.v << ' ' << __ptr.z - store.z << std::endl;
				__ptr.z = mx_base_cur._matrix->m();
				std::cerr << "mxi.m   : " << mx_base_cur._matrix->m() << ' ' << __ptr.v << std::endl;
				std::cerr << "    n   : " << (l<na? (l+na)/2+1 : na+1) << std::endl;
				std::cerr << "    m   : " << ((na+1)*(na+2))/2 << std::endl;
				std::cerr << "---------" << std::endl;
				if( err_it > ERR_ITER_MAX )
					exit(1);
			}
			this->test_set_matrix( __array_value_cur._value, mx_base_cur._matrix, l, na );
		}
	}
	if( this->array_size() != (__array_value_cur._value - __array_value_beg._value) )
	{
		this->error("test_set_matrices");
		std::cerr << "0 : array.size = " << this->array_size() << std::endl;
		std::cerr << "  : array.size = " << (__array_value_cur._value - __array_value_beg._value) << std::endl;
		exit(1);
	}
}

template<typename T, typename U>
void ixs_omega<T,U>::test_set_matrix( arr_pointer_t & p, mx_t * mx_base_cur, size_type l, size_type na )
{
	static const size_type _ITER_PRINT_MAX_ = 200u;
	static size_type lmb_min, lmb_max;
	//static union {void * v; unsigned char * c;} integral_ref = {p};

	_arrayValuePtr arr = {p};
	lmb_min = (l < na ? (l+na)%2 : (l-na));
	lmb_max = l + na;
	ixs_test::check_lmb( l, na, lmb_min, lmb_max );
	ixs_test::check_eq( mx_base_cur->n(), (lmb_max-lmb_min)/2+1, "test_set_matrix mx_base_cur.n" );
	ixs_test::check_eq( mx_base_cur->m(), ((na+1)*(na+2))/2, "test_set_matrix mx_base_cur.m" );
	T omega_value = T(0), tmp(0);

	size_type m2_size, iter = 0u, iter_total = 0u;
	ixs_matrix_t mx_cur;
	_matrixValuePtr mx_value_cur;

	mx_cur.base( mx_base_cur );
	mx_value_cur._void = mx_base_cur;
	mx_value_cur._char += sizeof(mx_t);
	mx_cur.data( mx_value_cur._value );

	if( arr._value - this->_array_value < _ITER_PRINT_MAX_ )
	{
		std::cout << std::setw(4) << mx_base_cur->n() << std::setw(4) << mx_base_cur->m() << " [" << this->_array_value << "]" << std::endl;
		IXS_OMEGA_INTEGRAL_LINE_OUTPUT_TITLE;
	}
	for(size_type lmb = lmb_min, i_lmb = 0u; lmb <= lmb_max; lmb += 2u, ++i_lmb)
	{
		iter_total += (2*lmb+1);
		for(size_type a = 0u, na_m_a = na, i_abc = 0u; a <= na; ++a, --na_m_a)
		{
			for(size_type b = 0u, c = na_m_a; b <= na_m_a; ++b, --c, ++i_abc)
			{
				ixs_test::check_nabc( na, a, b, c );
				//ixs_test::check_i2abc( na, i_abc, a, b, c );
				//arr.uc = integral_ref.c + (*mx_base_cur)(i_lmb, i_abc)._1st;//
				m2_size = mx_cur(i_lmb, i_abc)._2nd;
				arr._value = this->_array_value + mx_cur(i_lmb, i_abc)._1st;
				for(int i = 0; i < m2_size; ++i, ++arr._value)
				{
					if( arr._value - this->_array_value < _ITER_PRINT_MAX_ )
					{
						IXS_OMEGA_INTEGRAL_LINE_OUTPUT( l, na, lmb, i_abc, m2_size, arr._value, this->_array_value );
					}
#ifndef __IXS_VALUE_SHORT
					ixs_test::check_eq( arr._value->l, l, "test_set_matrix l" );
					ixs_test::check_eq( arr._value->n, na, "test_set_matrix na" );
					ixs_test::check_eq( arr._value->lmb, lmb, "test_set_matrix lmb" );
					ixs_test::check_eq( arr._value->i_abc, i_abc, "test_set_matrix i_abc" );
#endif
					if( arr._value->mu > int(lmb)||arr._value->mu < int(-lmb)||arr._value->m > int(l)||arr._value->m < int(-l) )
					{
						this->error( "test_set_matrix" );
						std::cerr << "m  : " << arr._value->m << std::endl;
						std::cerr << "l  : " << l << std::endl;
						std::cerr << "mu : " << arr._value->mu << std::endl;
						std::cerr << "lmb: " << lmb << std::endl;
						exit(1);
					}
					++iter;
				}
			}
		}
	}
	p = arr._value;
}
*/

template<typename T, typename U>
int ixs_omega<T,U>::shrink_to_fit(char const * file)
{
	if( !this->size() && !this->data() )
		return 0;
	const pos_type __occupied_memory_size = this->comp_size();
	std::cout << "[" << this << "] ixs_omega<T,U>::shrink_to_fit( \"" << file << "\" )" << std::endl;
	std::cout << "current  size : " << std::setw(14) << this->size() << std::endl;
	std::cout << "occupied size : " << std::setw(14) << __occupied_memory_size << std::endl;
	if( this->size() > __occupied_memory_size )
	{
		ixs_omega<T,U> v, tmp;
		memset( &v, 0, sizeof(v) );
		memset( &tmp, 0, sizeof(tmp) );
		v = *this;
		
		v.memory_create( file );
		if( v.size() != __occupied_memory_size )
		{
			this->error("shrink_to_fit", "size inqeuality");
			std::cerr << "occupied size : " << __occupied_memory_size << std::endl;
			std::cerr << "new map  size : " << v.size() << std::endl;
			exit(1);
		}
		memcpy( v.data(), this->data(), __occupied_memory_size );
		// Dangerous code: swap( v, *this );
		// tmp -- is 0
		// this - is %this
		// v ---- is %v
		memcpy( &tmp, this, sizeof(ixs_omega<T,U>) );// %tmp = %this;
		memcpy( this, &v, sizeof(ixs_omega<T,U>) );//   %this = %v;
		memset( &v, 0, sizeof(ixs_omega<T,U>) );//      %v = 0
		// tmp -- is %this
		// this - is %v
		// v ---- is 0
		return 1;
	}
	else if( this->size() == __occupied_memory_size )
		return 0;
	else
		return -1;
}

#define IXS_OMEGA_SPEC( typeU )\
	template class ixs_omega<float,       typeU>;\
	template class ixs_omega<double,      typeU>;\
	template class ixs_omega<long double, typeU>;

IXS_OMEGA_SPEC( float );
IXS_OMEGA_SPEC( double );
IXS_OMEGA_SPEC( long double );

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
IXS_OMEGA_SPEC( mpfr::mpreal );
#endif

#endif//__IXS_OMEGA_HPP__
