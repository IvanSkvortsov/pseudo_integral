#ifndef __ALPHA_POW_HPP__
#define __ALPHA_POW_HPP__
#include"alpha.pow.h"
#include"sf.math.h"// convert_float
#include"vector.3d.h"
#include<cstring>// memcpy

template<typename T, typename U> alpha_pow<T,U>::alpha_pow(): alpha_val<T,U>(), _M_mx1powkA(), _M_mx1powkB(), _M_mx2powk(), __ALPHA_POW_ITER_INIT_LIST{}
template<typename T, typename U> alpha_pow<T,U>::alpha_pow( alpha_pow<T,U> const & v ): alpha_val<T,U>( v ),
	_M_mx1powkA( v._M_mx1powkA ), _M_mx1powkB( v._M_mx1powkB ), _M_mx2powk( v._M_mx2powk ), __ALPHA_POW_ITER_INIT_LIST {}
template<typename T, typename U> alpha_pow<T,U> & alpha_pow<T,U>::operator=( alpha_pow<T,U> const & v )
{
	if( this == &v )
		return *this;
	alpha_pow<T,U> tmp( v );
	memcpy( this, &tmp, sizeof(*this) );
	return *this;
}

template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::comp_size( _lmax_struct const & __lmax, _alpsize_struct const & __alpsize, const int * __Asize, const int * __Bsize )
{
	size_type __size = 0;
	__size += this->alpha_val<T,U>::comp_size( __lmax, __alpsize, __Asize, __Bsize );

	size_type __l_size = 0;
	switch( this->get_mapping() )
	{
	case alpha_val<T,U>::minimum :
		break;
	case alpha_val<T,U>::middle  :
		// mx1powkA
		__l_size = (__lmax._lb_max+1 >__lmax._l_max ? __lmax._lb_max+1 : __lmax._l_max);
		__size += this->comp_size_mx1powkX( __l_size , __lmax._la_max, __Asize);
		break;
	case alpha_val<T,U>::maximum :
		__size += this->comp_size_mx1powkX( __lmax._l_max, __lmax._la_max, __Asize);// mx1powkA
		__size += this->comp_size_mx1powkX( __lmax._l_max, __lmax._lb_max, __Bsize);// mx1powkB
		__size += this->comp_size_mx2powk( __lmax, __Asize, __Bsize);// mx2powk
		break;
	default :
		this->error("comp_size", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return __size;
}
template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::mx1powkX_size( const size_type __l0_max, const size_type & __lx_max, const int * __Xsize  )
{
	size_type __size = 0;
	for(int lx = 0, __pow_size = __l0_max; lx <= __lx_max; ++lx, ++__pow_size)
		__size += __pow_size * __Xsize[lx];
	return __size;
}
template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::comp_size_mx1powkX( const size_type __l0_max, const size_type & __lx_max, const int * __Xsize  )
{
	size_type __size = this->mx1powkX_size( __l0_max, __lx_max, __Xsize );
	__size *= sizeof(float_type);
	__size += sizeof(size_struct<1>);
	return __size;
}

template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::mx2powk_size( _lmax_struct const & __lmax, const int * __Asize, const int * __Bsize  )
{
	size_type __size = 0;
	for(int la = 0; la <= __lmax._la_max; ++la)
		for(int lb = 0, __pow_size = la + 1; lb <= __lmax._lb_max; ++lb, ++__pow_size)
			__size += __Asize[la] * __Bsize[lb] * __pow_size;
	return __size;
}
template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::comp_size_mx2powk( _lmax_struct const & __lmax, const int * __Asize, const int * __Bsize  )
{
	size_type __size = this->mx2powk_size( __lmax, __Asize, __Bsize );
	__size *= sizeof(float_type);
	__size += sizeof(size_struct<1>);
	return __size;
}

template<typename T, typename U>
void alpha_pow<T,U>::write_mx1powkX( memorystream & ms, matrix_cursor_1<T> * & __mx1,
		const size_type __l0_size, const size_type & __lx_max, const int * __Xsize )
{
	_matrixPtr __cnvrt = {ms.getcur()};
	__mx1 = __cnvrt._matrix1_T;
	__mx1->size() = this->mx1powkX_size( __l0_size, __lx_max, __Xsize );
	ms.seek( sizeof(size_struct<1>) + __mx1->size() * sizeof(float_type), seek_dir::cur );
}
template<typename T, typename U>
void alpha_pow<T,U>::write_mx2powk( memorystream & ms, _lmax_struct const & __lmax, const int * _Asize, const int * _Bsize )
{
	_matrixPtr __cnvrt = {ms.getcur()};
	this->_M_mx2powk = __cnvrt._matrix1_T;
	this->_M_mx2powk->size() = this->mx2powk_size( __lmax, _Asize, _Bsize );
	ms.seek( sizeof(size_struct<1>) + this->_M_mx2powk->size() * sizeof(float_type), seek_dir::cur );
}

template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::write_mxpow( memorystream & ms, _lmax_struct const & __lmax, _alpsize_struct const & __alpsize,
		const int * _Asize, const int * _Bsize )
{
	const size_type _seek_start = ms.tell();
	this->alpha_val<T,U>::write_mxalp( ms, __lmax, __alpsize );

	size_type __l_size = 0;
	switch( this->get_mapping() )
	{
	case alpha_val<T,U>::minimum :
		break;
	case alpha_val<T,U>::middle  :
		__l_size = (__lmax._lb_max + 1 > __lmax._l_max ? __lmax._lb_max + 1 : __lmax._l_max);
		this->write_mx1powkX( ms, this->_M_mx1powkA, __l_size, __lmax._la_max, _Asize );
		break;
	case alpha_val<T,U>::maximum :
		this->write_mx1powkX( ms, this->_M_mx1powkA, __lmax._l_max, __lmax._la_max, _Asize );
		this->write_mx1powkX( ms, this->_M_mx1powkB, __lmax._l_max, __lmax._lb_max, _Bsize );
		this->write_mx2powk( ms, __lmax, _Asize, _Bsize );
		break;
	default :
		this->error("write_mxpow", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return ms.tell() - _seek_start;
}

template<typename T, typename U> const typename alpha_pow<T,U>::size_type
alpha_pow<T,U>::read_mxpow( memorystream & ms )
{
	const size_type _seek_start = ms.tell();
	this->alpha_val<T,U>::read_mxalp( ms );

	switch( this->get_mapping() )
	{
	case alpha_val<T,U>::minimum :
		break;
	case alpha_val<T,U>::middle  :
		this->alpha_val<T,U>::read_mx1T( ms, this->_M_mx1powkA );
		break;
	case alpha_val<T,U>::maximum :
		this->alpha_val<T,U>::read_mx1T( ms, this->_M_mx1powkA );
		this->alpha_val<T,U>::read_mx1T( ms, this->_M_mx1powkB );
		this->alpha_val<T,U>::read_mx1T( ms, this->_M_mx2powk );
		break;
	default :
		this->error("read_mxpow", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
	return ms.tell() - _seek_start;
}

#define __ALPHA_POW_INIT_MX1POWKX( powkX, mx1kX, map1X, mx1X, lx_max, _2CX_len, X_alp, __size, T, U )\
for(int lx = 0; lx <= (lx_max); ++lx)\
{\
	this->map1##powkX##_set_lx( lx );\
	this->map1X##_set_lx( lx );\
	p_mx1powkX = this->_M_mx1##powkX->data() + this->map1##powkX##_pos();\
	p_mx1kX = this->_M_mx1kX->data() + this->map1X##_pos();\
	__size += this->map1X##_size() * this->map1##powkX##_size();\
	for(int i = 0; i < this->map1X##_size(); ++i, ++p_mx1kX)\
	{\
		this->map1X##_set_ix( i );\
		this->map1##powkX##_set_ix( i );\
		__alp_pow_assert__( this->map1X##_idx() < this->M_##mx1X##_size() );\
		__alp_pow_assert__( this->map1X##_idx() == this->map1X##_pos() + i );\
		__alp_pow_assert__( this->map1##powkX##_idx() == this->map1##powkX##_pos() + i * this->map1##powkX##_size() );\
		__alp_pow_assert__( this->map1##powkX##_idx() < this->M_mx1##powkX##_size() );\
		u = 1; *p_mx1powkX++ = 1;\
		_kXlen = _2CX_len;\
		_kXlen *= X_alp[this->map1X##_idx()];\
		*p_mx1kX = math::convert_float<T,U>( _kXlen );\
		for(int k = 1; k < this->map1##powkX##_size(); ++k, ++p_mx1powkX)\
		{\
			u *= _kXlen;\
			*p_mx1powkX = math::convert_float<T,U>( u );\
		}\
	}\
}

// mx1powkA (+ mx1kA)
template<typename T, typename U>
void alpha_pow<T,U>::init_mx1powkA( U const & CX_len, U const * X_alp )
{
	pointer p_mx1powkX, p_mx1kX;
	const U _2CX_len = 2 * CX_len;
	U u(0), _kXlen;
	size_type __size = 0;
	__ALPHA_POW_INIT_MX1POWKX( powkA, mx1kA, map1A, mx1A, this->la_max(), _2CX_len, X_alp, __size, T, U );
	__alp_pow_assert__( __size == this->M_mx1powkA_size() );
}

// mx1powkB (+ mx1kB)
template<typename T, typename U>
void alpha_pow<T,U>::init_mx1powkB( U const & CX_len, U const * X_alp )
{
	pointer p_mx1powkX, p_mx1kX;
	const U _2CX_len = 2 * CX_len;
	U u(0), _kXlen;
	size_type __size = 0;
	__ALPHA_POW_INIT_MX1POWKX( powkB, mx1kB, map1B, mx1B, this->lb_max(), _2CX_len, X_alp, __size, T, U );
	__alp_pow_assert__( __size == this->M_mx1powkB_size() );
}
template<typename T, typename U>
void alpha_pow<T,U>::init_mx2powk(U const * CA, U const * A_alp, U const * CB, U const * B_alp)// TODO: merge together with %init_mx2k
{
	pointer p_mx2powk, p_mx2k;
	U u(0), _klen;
	vector_3d<U> kA, kB, k;
	const U m2_CA[3] = {U(-2 * CA[0]), U(-2 * CA[1]), U(-2 * CA[2]) };
	const U m2_CB[3] = {U(-2 * CB[0]), U(-2 * CB[1]), U(-2 * CB[2]) };
	for(int la = 0; la < this->la_size(); ++la)
	{
		this->map1A_set_lx( la );
		this->map2AB_set_la( la );
		this->map2powk_set_la( la );
		for(int lb = 0; lb < this->lb_size(); ++lb)
		{
			this->map1B_set_lx( lb );
			this->map2AB_set_lb( lb );
			this->map2powk_set_lb( lb );
			p_mx2powk = this->_M_mx2powk->data() + this->map2powk_pos();
			p_mx2k = this->_M_mx2k->data() + this->map2AB_pos();
			for(int i = 0; i < this->map1A_size(); ++i)
			{
				kA = m2_CA;
				kA *= A_alp[i + this->map1A_pos()];
				for(int j = 0; j < this->map1B_size(); ++j, ++p_mx2k)
				{
					kB = m2_CB;
					kB *= B_alp[j + this->map1B_pos()];

					k = kA;
					k += kB;
					k.len( _klen );
					k /= _klen;

					*p_mx2k = math::convert_float<T,U>( _klen );
					if( this->map2powk_size() != (la + lb + 1) )
					{
						this->error("init_mx2powk", "wrong max power");
						std::cerr << "la + lb + 1 : " << la + lb + 1 << std::endl;
						std::cerr << "max_pow + 1 : " << this->map2powk_size() << std::endl;
						exit(1);
					}
					u = 1;
					*p_mx2powk++ = 1;
					for(int q = 1; q < this->map2powk_size(); ++q, ++p_mx2powk)
					{
						u *= _klen;
						*p_mx2powk = math::convert_float<T,U>( u );
					}
				}
			}
		}
	}
}

template<typename T, typename U>
void alpha_pow<T,U>::init_mxpow( U const * CA, U const * A_alp, U const * CB, U const * B_alp)
{
	U CA_len, CB_len;
	vector_3d<U> v3d;
	switch( this->get_mapping() )
	{
	case alpha_val<T,U>::minimum :
		break;
	case alpha_val<T,U>::middle  :
		v3d = CA; v3d.len( CA_len );
		this->init_mx1powkA( CA_len, A_alp );
		break;
	case alpha_val<T,U>::maximum :
		v3d = CA; v3d.len( CA_len );
		this->init_mx1powkA( CA_len, A_alp );
		v3d = CB; v3d.len( CB_len );
		this->init_mx1powkB( CB_len, B_alp );
		this->init_mx2powk( CA,  A_alp, CB, B_alp );
		break;
	default :
		this->error("init_mxpow", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}

template<typename T, typename U> void alpha_pow<T,U>::test_alp_pow_powkA()
{
	std::clog << "Test: [" << this << "] alpha_pow<T,U>::test_alp_powkA()" << std::endl;
	T err, __pow;
	for(int la = 0; la <= this->la_max(); ++la)
	{
		this->map1powkA_set_lx( la );
		this->map1A_set_lx( la );
		for(int ia = 0; ia < this->map1A_size(); ++ia)
		{
			this->map1A_set_ix( ia );
			this->mx1A_set_idx();
			this->mx1kA_set_idx();
			//
			this->map1powkA_set_ix( ia );
			this->mx1powkA_set_idx();
			__alp_pow_assert__( this->map1powkA_size() == this->l_max() + la );
			__alp_pow_assert__( this->map1powkA_idx() < this->M_mx1powkA_size() );
			for(int i = 0; i < this->map1powkA_size(); ++i)
			{
				__pow = this->mx1powkA()[i];
				__alp_pow_assert__( __pow > 1.0e-100 );
				err  = this->mx1powkA()[i];
				err -= math::pown<T>( this->mx1kA(), i );
				err /= this->mx1powkA()[i];
				err = ( err < 0 ? -err : err );
				if( err > std::numeric_limits<T>::epsilon() * 8 )
				{
					this->error("test_alp_pow_powkA()", "mx1powkA calculus error");
					std::cerr << "n : " << i << std::endl;
					std::cerr << "mx1powkA(" << i << ") : " << this->mx1powkA()[i] << std::endl;
					std::cerr << "kA : " << this->mx1kA() << '\t' << math::pown<T>( this->mx1kA(), i) << std::endl;
					std::cerr << "err : " << err << std::endl;
					exit(1);
				}
			}
		}
	}
}

template<typename T, typename U> void alpha_pow<T,U>::test_alp_pow_powkB()
{
	std::clog << "Test: [" << this << "] alpha_pow<T,U>::test_alp_powkB()" << std::endl;
	T err, __pow;
	for(int lb = 0; lb <= this->lb_max(); ++lb)
	{
		this->map1powkB_set_lx( lb );
		this->map1B_set_lx( lb );
		for(int ib = 0; ib < this->map1B_size(); ++ib)
		{
			this->map1B_set_ix( ib );
			this->mx1B_set_idx();
			this->mx1kB_set_idx();
			//
			this->map1powkB_set_ix( ib );
			this->mx1powkB_set_idx();
			__alp_pow_assert__( this->map1powkB_size() == this->l_max() + lb );
			__alp_pow_assert__( this->map1powkB_idx() < this->M_mx1powkB_size() );
			for(int i = 0; i < this->map1powkB_size(); ++i)
			{
				__pow = this->mx1powkB()[i];
				__alp_pow_assert__( __pow > 1.0e-100 );
				err  = this->mx1powkB()[i];
				err -= math::pown<T>( this->mx1kB(), i );
				err /= this->mx1powkB()[i];
				err = ( err < 0 ? -err : err );
				if( err > std::numeric_limits<T>::epsilon() * 8 )
				{
					this->error("test_alp_pow_powkB()", "mx1powkB calculus error");
					std::cerr << "n : " << i << std::endl;
					std::cerr << "mx1powkB(" << i << ") : " << this->mx1powkB()[i] << std::endl;
					std::cerr << "kB : " << this->mx1kB() << '\t' << math::pown<T>( this->mx1kB(), i) << std::endl;
					std::cerr << "err : " << err << std::endl;
					exit(1);
				}
			}
		}
	}
}

template<typename T, typename U> void alpha_pow<T,U>::test_alp_pow_powk()
{
	std::clog << "Test: [" << this << "] alpha_pow<T,U>::test_alp_powk()" << std::endl;
	T err;
	for(int la = 0; la <= this->la_max(); ++la)
	{
		this->map2AB_set_la( la );
		this->map2powk_set_la( la );
		this->map1powkA_set_lx( la );
		this->map1A_set_lx( la );
		for(int lb = 0; lb <= this->lb_max(); ++lb )
		{
			this->map2AB_set_lb( lb );
			this->map2powk_set_lb( lb );
			this->map1powkB_set_lx( lb );
			this->map1B_set_lx( lb );
			for(int ia = 0; ia < this->map1A_size(); ++ia)
			{
				this->map1A_set_ix( ia );
				this->mx1A_set_idx();
				this->mx1kA_set_idx();
				//
				this->map1powkA_set_ix( ia );
				this->mx1powkA_set_idx();
				//
				this->map2AB_set_ia( ia );
				this->map2powk_set_ia( ia );
				for(int i = 0; i < this->map1powkA_size(); ++i)
				{
					err  = this->mx1powkA()[i];
					err -= math::pown<T>( this->mx1kA(), i );
					err /= this->mx1powkA()[i];
					err = ( err < 0 ? -err : err );
					if( err > std::numeric_limits<T>::epsilon() * 8 )
					{
						this->error("test_alp_pow_powkA()", "mx1powkA calculus error");
						std::cerr << "n : " << i << std::endl;
						std::cerr << "mx1powkA(" << i << ") : " << this->mx1powkA()[i] << std::endl;
						std::cerr << "kA : " << this->mx1kA() << '\t' << math::pown<T>( this->mx1kA(), i) << std::endl;
						std::cerr << "err : " << err << std::endl;
						exit(1);
					}
				}
				for(int ib = 0; ib < this->map1B_size(); ++ib )
				{
					this->map1B_set_ix( ib );
					this->mx1B_set_idx();
					this->mx1kB_set_idx();
					//
					this->map1powkB_set_ix( ib );
					this->mx1powkB_set_idx();
					//
					this->map2AB_set_ib( ib );
					this->mx2k_set_idx();
					//
					this->map2powk_set_ib( ib );
					this->mx2powk_set_idx();
					for(int i = 0; i < this->map1powkB_size(); ++i)
					{
						err  = this->mx1powkB()[i];
						err -= math::pown<T>( this->mx1kB(), i );
						err /= this->mx1powkB()[i];
						err = ( err < 0 ? -err : err );
						if( err > std::numeric_limits<T>::epsilon() * 8 )
						{
							this->error("test_alp_pow_powkB()", "mx1powkA calculus error");
							std::cerr << "n : " << i << std::endl;
							std::cerr << "mx1powkB(" << i << ") : " << this->mx1powkB()[i] << std::endl;
							std::cerr << "kB : " << this->mx1kB() << '\t' << math::pown<T>( this->mx1kB(), i) << std::endl;
							std::cerr << "err : " << err << std::endl;
							exit(1);
						}
					}
					for(int i = 0; i < this->map2powk_size(); ++i)
					{
						err  = this->mx2powk()[i];
						err -= math::pown<T>( this->mx2k(), i );
						err /= this->mx2powk()[i];
						err = ( err < 0 ? -err : err );
						if( err > std::numeric_limits<T>::epsilon() * 8 )
						{
							this->error("test_alp_pow_powk()", "mx2powk calculus error");
							std::cerr << "n : " << i << std::endl;
							std::cerr << "mx2powk(" << i << ") : " << this->mx2powk()[i] << std::endl;
							std::cerr << "k : " << this->mx2k() << '\t' << math::pown<T>( this->mx2k(), i) << std::endl;
							std::cerr << "err : " << err << std::endl;
							exit(1);
						}
					}
				}
			}
		}
	}
}
template<typename T, typename U> void alpha_pow<T,U>::test_alp_pow()
{
	std::clog << "Test: [" << this << "] alpha_pow<T,U>::test_alp_pow()" << std::endl;
	switch( this->get_mapping() )
	{
	case alpha_val<T,U>::minimum :
		break;
	case alpha_val<T,U>::middle  :
		this->test_alp_pow_powkA();
		break;
	case alpha_val<T,U>::maximum :
		this->test_alp_pow_powkA();
		this->test_alp_pow_powkB();
		this->test_alp_pow_powk();
		break;
	default :
		this->error("test_alp_pow()", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}

#define __ALPHA_POW_SPEC( typeU )\
template class alpha_pow<float, typeU>;\
template class alpha_pow<double, typeU>;\
template class alpha_pow<long double, typeU>;

#endif//__ALPHA_POW_HPP__
