#ifndef __ALPHA_SLM_HPP__
#define __ALPHA_SLM_HPP__
#include"alpha.slm.h"
#ifdef __ALPHA_SLM_DEBUG
  #include<cassert>
  #include<iomanip>
#endif
#ifdef __ALPHA_SLM_TIME
  #include"get.work.time.h"
#endif
#include"vector.3d.h"
#include"sf.math.h"// convert_float

template<typename T, typename U> alpha_slm<T,U>::alpha_slm(): std::vector<U>(), alpha_pow<T,U>(){}
template<typename T, typename U> alpha_slm<T,U>::alpha_slm( alpha_slm<T,U> const & v ): std::vector<U>( v ), alpha_pow<T,U>( v ){}
template<typename T, typename U> alpha_slm<T,U> & alpha_slm<T,U>::operator=( alpha_slm<T,U> const & v )
{
	if( this == &v )
		return *this;
	this->std::vector<U>::operator=( v );
	this->alpha_pow<T,U>::operator=( v );
	return *this;
}

template<typename T>
inline static void comp_powers_of( T const & value, const int max_power, T * storage, T & u )
{
	u = 1; *storage++ = 1;
	for(int i = 1; i <= max_power; ++i, ++storage)
	{
		u *= value;
		*storage = u;
	}
}

template<typename T, typename U>
inline static void slm_pown( T * __data, matrix_slm<U> const & mx_slm, U const * _vx, U const * _vy, U const * _vz, const int l_max)
{
	if( mx_slm.lmb_max() < l_max )
	{
		std::cerr << "Error: [slm_pown]" << std::endl;
		std::cerr << "mx_slm.lmax < lmax" << std::endl;
		std::cerr << "mx_slm.lmax : " << mx_slm.lmb_max() << std::endl;
		std::cerr << "lmax : " << l_max << std::endl;
		exit(1);
	}
	U u, value;
	typename matrix_slm<U>::const_pointer _slm = mx_slm.data();
	typename matrix_slm<U>::value_type::const_pointer _pol;
	for(int l = 0; l <= l_max; ++l)
	{
		for(int m = -l; m <= l; ++m, ++__data, ++_slm)
		{
			value = 0;
			_pol = _slm->data();
			for(int i = 0; i < _slm->size(); ++i, ++_pol )
			{
				u  = _vx[ _pol->x[0] ];
				u *= _vy[ _pol->x[1] ];
				u *= _vz[ _pol->x[2] ];
				u *= _pol->d;
				value += u;
			}
			*__data = math::convert_float<T,U>( value );
		}
	}
}

template<typename T, typename U>
void alpha_slm<T,U>::assign_mx2slm( alpha_pow<T,U> const & palp )
{
#ifdef __ALPHA_SLM_TIME
	get_work_time t1, t2;
	t1.counting_start();
#endif
	this->alpha_pow<T,U>::operator=( palp );
	switch( this->get_mapping() )
	{
	case alpha_map::minimum :
		break;
	case alpha_map::middle  :
		break;
	case alpha_map::maximum :
#ifdef __ALPHA_SLM_TIME
		t2.counting_start();
#endif
		this->std::vector<U>::resize( this->alpha_map::M_mx2slm_size() );
#ifdef __ALPHA_SLM_TIME
		t2.counting_end();
		std::clog << "alpha_slm<T,U>::assign_mx2slm memory allocation : " << t2.get_realseconds() << ", sec" << std::endl;
#endif
		break;
	default :
		this->error("assign_mx2slm", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
#ifdef __ALPHA_SLM_TIME
	t1.counting_end();
	std::clog << "alpha_slm<T,U>::assign_mx2slm : " << t1.get_realseconds() << ", sec" << std::endl;
#endif
}

template<typename T, typename U>
void alpha_slm<T,U>::init_mxslm( U const * CA, U const * A_alp, U const * CB, U const * B_alp, matrix_slm<U> const & mx_slm)
{
	U CA_len, CB_len;
	vector_3d<U> v3d;
	switch( this->get_mapping() )
	{
	case alpha_map::minimum :
		break;
	case alpha_map::middle  :
		v3d = CA; v3d.len( CA_len );
		this->init_mx1powkA( CA_len, A_alp );
		break;
	case alpha_map::maximum :
		v3d = CA; v3d.len( CA_len );
		this->init_mx1powkA( CA_len, A_alp );
		v3d = CB; v3d.len( CB_len );
		this->init_mx1powkB( CB_len, B_alp );
		this->init_mx2slm( CA, A_alp, CB, B_alp, mx_slm );
		break;
	default :
		this->error("init_mxslm", "undefined mapping type");
		std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
		exit(1);
	}
}

template<typename T, typename U>
void alpha_slm<T,U>::init_mx2slm( U const * CA, U const * A_alp, U const * CB, U const * B_alp, matrix_slm<U> const & mx_slm)
{
#ifdef __ALPHA_SLM_TIME
	get_work_time t1;
	t1.counting_start();
#endif
	static std::vector<U> __powk3;
	__powk3.reserve( (this->la_max() + this->lb_max() + 1) * 4 );

	vector_3d<U> kA( CA ), kB( CB ), k;
	U _klen, * _storage_x, * _storage_y, * _storage_z, * _storage_k, u;

	this->std::vector<U>::resize( this->alpha_map::M_mx2slm_size() );

	//
	typename std::vector<U>::pointer __slm, __slm_data = this->std::vector<U>::data();
#ifdef  __ALPHA_SLM_DEBUG
	assert( __slm_data != 0 );
	assert( this->std::vector<U>::size() != 0 );
#endif
#ifdef  __ALPHA_SLM_LOG
	std::clog << std::setw(4) << "la" << std::setw(4) << "lb" << std::endl;
#endif
	typename alpha_pow<T,U>::pointer __mx2powk, __mx2k;

	for(int la = 0; la < this->la_size(); ++la)
	{
		this->map1A_set_lx( la );
		this->map2powk_set_la( la );
		this->map2AB_set_la( la );
		this->map2slm_set_la( la );
		for(int lb = 0, max_power = la; lb < this->lb_size(); ++lb, ++max_power)
		{
			this->map1B_set_lx( lb );
			this->map2powk_set_lb( lb );
			this->map2AB_set_lb( lb );
			this->map2slm_set_lb( lb );
			__mx2k = this->_M_mx2k->data() + this->map2AB_pos();
			__powk3.resize( (max_power + 1) * 4 );
			// P.S. max_power is angular momentum
#ifdef  __ALPHA_SLM_LOG
			std::clog << std::setw(4) << la << std::setw(4) << lb << std::endl;
#endif
			for(int i = 0; i < this->map1A_size(); ++i)
			{
				this->map2powk_set_ia( i );
				this->map2slm_set_ia( i );
				kA = CA;
				kA *= A_alp[i + this->map1A_pos()];
				kA *= -2;
				for(int j = 0; j < this->map1B_size(); ++j, ++__mx2k)
				{
					this->map2powk_set_ib( j );
					this->mx2powk_set_idx();
					this->map2slm_set_ib( j );
#ifdef  __ALPHA_SLM_DEBUG
					assert( this->map2slm_size() == (la + lb + 1) * (la + lb + 1) );
					assert( this->map2powk_size() == (la + lb + 1) );
					assert( mx_slm.lmb_max() >= max_power );
#endif
					kB = CB;
					kB *= B_alp[j + this->map1B_pos()];
					kB *= -2;

					k = kA;
					k += kB;

					k.len( _klen );
					k /= _klen;

					*__mx2k = math::convert_float<T,U>( _klen );

					_storage_x = __powk3.data();
					_storage_y = _storage_x + (max_power + 1);
					_storage_z = _storage_y + (max_power + 1);
					_storage_k = _storage_z + (max_power + 1);

					// kx, ky, kz, klen powers calculation
					comp_powers_of( k.x(), max_power, _storage_x, u );
					comp_powers_of( k.y(), max_power, _storage_y, u );
					comp_powers_of( k.z(), max_power, _storage_z, u );
					comp_powers_of( _klen, max_power, _storage_k, u );

					// slm calculation
					__slm = __slm_data + this->map2slm_idx();
					slm_pown<U,U>( __slm, mx_slm, _storage_x, _storage_y, _storage_z, max_power );
					// copying
					__mx2powk = this->mx2powk();
					for(int it = 0; it <= max_power; ++it)
						__mx2powk[it] = math::convert_float<T,U>( _storage_k[it] );
				}
			}
		}
	}
#ifdef __ALPHA_SLM_TIME
	t1.counting_end();
	std::clog << "Time: [" << this << "] alpha_slm<T,U>::init_mx2slm : ";// << t1.get_realseconds() << ", sec" << std::endl;
	fprintf(stderr, "%14.4f\n", t1.get_realseconds() );
#endif
}

#define __ALPHA_SLM_SPEC( typeU )\
template class alpha_slm<float, typeU>;\
template class alpha_slm<double, typeU>;\
template class alpha_slm<long double, typeU>;

#endif//__ALPHA_SLM_HPP__
