#ifndef __PSEUSO_INTEGRAL_HPP__
#define __PSEUSO_INTEGRAL_HPP__
#include"pseudo.integral.h"
#include"n.abc.h"// from angular/
#include"sf.math.h"// pown, fact_n, fact_2n1_n
#include"ixs.angular.idx.h"// to_compute
#include"q.int.h"

template<typename T, typename U> pseudo_integral<T,U>::pseudo_integral(): alpha_val<T,U>(), _mx1prim_size(), _mx1func_size(),
	_M_node(), _M_mx1prim(), _M_mx1func(), __PSEUDO_INTEGRAL_ITER_INIT_LIST {}
template<typename T, typename U> pseudo_integral<T,U>::pseudo_integral( pseudo_integral<T,U> const & v ): alpha_val<T,U>( v ),
	_mx1prim_size( v._mx1prim_size ), _mx1func_size( v._mx1func_size ),
	_M_node( v._M_node ), _M_mx1prim( v._M_mx1prim ), _M_mx1func( v._M_mx1func ), __PSEUDO_INTEGRAL_ITER_INIT_LIST {}
template<typename T, typename U> pseudo_integral<T,U> & pseudo_integral<T,U>::operator=( pseudo_integral<T,U> const & v )
{
	if( this == &v )
		return *this;
	memcpy( this, &v, sizeof(*this) );
	return *this;
}

template<typename T, typename U> typename pseudo_integral<T,U>::const_reference pseudo_integral<T,U>::get_prim( pseudo_index const & idx )
{
	static const T zero = 0; 
	if( this->is_mapping_min() && idx.get_lb()%2 != idx.get_la()%2 && idx.get_l()%2 != idx.get_lb()%2 )
		return zero;
	else if( this->is_mapping_mid() && idx.get_l()%2 != idx.get_lb()%2 && idx.get_l() != this->alpha_map::l_max() )
		return zero;
	// la, {ax, ay, az}
	this->map3node_set_la( idx.get_la() );
	this->alpha_map::map1A_set_lx( idx.get_la() );
	this->map3node_set_axyz( idx.get_axyz() );
	// lb, {bx, by, bz}
	this->map3node_set_lb( idx.get_lb() );
	this->alpha_map::map1B_set_lx( idx.get_lb() );
	this->map3node_set_bxyz( idx.get_bxyz() );
	// l
	this->map3node_set_l( idx.get_l() );
	this->alpha_map::map1C_set_lx( idx.get_l() );
	this->mx1prim_set_it();
	//
	this->prim_set_ia( idx.get_ia_p() );
	this->prim_set_ib( idx.get_ib_p() );
	this->prim_set_ic( idx.get_ic_p() );
	this->prim_set_idx();
	return this->prim_value();
}

template<typename T, typename U> typename pseudo_integral<T,U>::const_reference pseudo_integral<T,U>::get_func( pseudo_index const & idx )
{
	static const T zero = 0; 
	if( this->is_mapping_min() && idx.get_lb()%2 != idx.get_la()%2 && idx.get_l()%2 != idx.get_lb()%2 )
		return zero;
	else if( this->is_mapping_mid() && idx.get_l()%2 != idx.get_lb()%2 )
		return zero;
	// la, {ax, ay, az}
	this->map3node_set_la( idx.get_la() );
	this->alpha_map::map1A_d_set_lx( idx.get_la() );
	this->map3node_set_axyz( idx.get_axyz() );
	// lb, {bx, by, bz}
	this->map3node_set_lb( idx.get_lb() );
	this->alpha_map::map1B_d_set_lx( idx.get_lb() );
	this->map3node_set_bxyz( idx.get_bxyz() );
	// l
	this->map3node_set_l( idx.get_l() );
	this->alpha_map::map1C_d_set_lx( idx.get_l() );
	this->mx1func_set_it();
	//
	this->func_set_ia( idx.get_ia_p() );
	this->func_set_ib( idx.get_ib_p() );
	this->func_set_ic( idx.get_ic_p() );
	this->func_set_idx();
	return this->func_value();
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::comp_size()
{
	size_type __size = 0;
	__size += sizeof(size_struct<3>);
	__size += sizeof(mx3node_type) * this->comp_node_size();

	__size += sizeof(size_struct<1>) * 2;
	__size += sizeof(T) * this->comp_mx1T_2x_size();
	__pseudo_assert__( this->comp_mx1T_2x_size() == this->comp_mx1prim_size() + this->comp_mx1func_size() );
	return __size;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type 
pseudo_integral<T,U>::comp_node_size()const
{
	size_type __size = 0;
	__size = this->node_N();
	__size *= this->node_M();
	__size *= this->node_P();
	return __size;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::comp_mx1T_2x_size()
{
	switch( this->get_mapping() )
	{
	case mapping_struct::minimum: return this->comp_mx1T_2x_size_min();
	case mapping_struct::middle : return this->comp_mx1T_2x_size_mid();
	case mapping_struct::maximum: return this->comp_mx1T_2x_size_max();
	default:
	      this->error("comp_mx1T_2x_size()", "unknown mapping type" );
	      std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
	      exit(1);
	}
}

#define __PSEUDO_INTEGRAL_COMP_SIZE( X, map3X )\
template<typename T, typename U> const typename pseudo_integral<T,U>::size_type \
pseudo_integral<T,U>::comp_mx1##X##_size()\
{\
	switch( this->get_mapping() )\
	{\
	case mapping_struct::minimum: return this->comp_mx1##X##_size_min();\
	case mapping_struct::middle : return this->comp_mx1##X##_size_mid();\
	case mapping_struct::maximum: return this->comp_mx1##X##_size_max();\
	default:\
	      this->error( "comp_mx1xxxx_size()", "unknown mapping type" );\
	      std::cerr << "current mapping type : " << this->get_mapping() << std::endl;\
	      exit(1);\
	}\
}\
\
template<typename T, typename U> const typename pseudo_integral<T,U>::size_type \
pseudo_integral<T,U>::comp_mx1##X##_size_max()\
{\
	__pseudo_assert__( this->is_mapping_max() );\
	size_type __size = 0;\
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )\
	{\
		this->alpha_map::map3X##_set_la( la );\
		_la_size_ = 0;\
		for(int lb = 0, ilb_size = 1, _lb_size_; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )\
		{\
			this->alpha_map::map3X##_set_lb( lb );\
			/* semi-local (+) local*/\
			_lb_size_ = 0;\
			for(int l = 0; l <= this->alpha_map::l_max(); ++l )\
			{\
				this->alpha_map::map3X##_set_l( l );\
				_lb_size_ += this->alpha_map::map3X##_size();\
			}\
			/* TODO: spin-orbit*/\
			_lb_size_ *= ilb_size;\
			_la_size_ += _lb_size_;\
		}\
		_la_size_ *= ila_size;\
		__size += _la_size_;\
	}\
	return __size;\
}\
\
template<typename T, typename U> const typename pseudo_integral<T,U>::size_type \
pseudo_integral<T,U>::comp_mx1##X##_size_mid()\
{\
	__pseudo_assert__( this->is_mapping_mid() );\
	size_type __size = 0;\
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )\
	{\
		this->alpha_map::map3X##_set_la( la );\
		_la_size_ = 0;\
		for(int lb = 0, ilb_size = 1, _lb_size_; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )\
		{\
			this->alpha_map::map3X##_set_lb( lb );\
			/* semi-local*/\
			_lb_size_ = 0;\
			for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )\
			{\
				this->alpha_map::map3X##_set_l( l );\
				_lb_size_ += this->alpha_map::map3X##_size();\
			}\
			/* local*/\
			this->alpha_map::map3X##_set_l( this->alpha_map::l_max() );\
			_lb_size_ += this->alpha_map::map3X##_size();\
			/* TODO: spin-orbit*/\
			_lb_size_ *= ilb_size;\
			_la_size_ += _lb_size_;\
		}\
		_la_size_ *= ila_size;\
		__size += _la_size_;\
	}\
	return __size;\
}\
\
template<typename T, typename U> const typename pseudo_integral<T,U>::size_type \
pseudo_integral<T,U>::comp_mx1##X##_size_min()\
{\
	__pseudo_assert__( this->is_mapping_min() );\
	size_type __size = 0;\
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )\
	{\
		this->alpha_map::map3X##_set_la( la );\
		_la_size_ = 0;\
		for(int lb = la%2, ilb_size = (la%2 ? 3 : 1), _lb_size_; lb <= this->alpha_map::lb_max(); lb += 2, ilb_size += (2*lb+1))\
		{\
			this->alpha_map::map3X##_set_lb( lb );\
			/* semi-local*/\
			_lb_size_ = 0;\
			for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )\
			{\
				this->alpha_map::map3X##_set_l( l );\
				_lb_size_ += this->alpha_map::map3X##_size();\
			}\
			/* local*/\
			this->alpha_map::map3X##_set_l( this->alpha_map::l_max() );\
			_lb_size_ += this->alpha_map::map3X##_size();\
			/* TODO: spin-orbit*/\
			_lb_size_ *= ilb_size;\
			_la_size_ += _lb_size_;\
		}\
		_la_size_ *= ila_size;\
		__size += _la_size_;\
		/*std::clog <<"la = "<<la<"; _la_size_ = "<<_la_size_<<"; _lb_size_ = "<<_lb_size_<<"; __size = "<<__size<<std::endl;*/\
	}\
	return __size;\
}
__PSEUDO_INTEGRAL_COMP_SIZE( prim, map3ABC );
__PSEUDO_INTEGRAL_COMP_SIZE( func, map3ABC_d );

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::comp_mx1T_2x_size_max()
{
	__pseudo_assert__( this->is_mapping_max() );
	size_type __size = 0;
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->alpha_map::map3ABC_set_la( la );
		this->alpha_map::map3ABC_d_set_la( la );
		_la_size_ = 0;
		for(int lb = 0, ilb_size = 1, _lb_size_; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
		{
			this->alpha_map::map3ABC_set_lb( lb );
			this->alpha_map::map3ABC_d_set_lb( lb );
			// semi-local (+) local
			_lb_size_ = 0;
			for(int l = 0; l <= this->alpha_map::l_max(); ++l )
			{
				this->alpha_map::map3ABC_set_l( l );
				this->alpha_map::map3ABC_d_set_l( l );
				_lb_size_ += this->alpha_map::map3ABC_size();
				_lb_size_ += this->alpha_map::map3ABC_d_size();
			}
			// TODO: spin-orbit
			_lb_size_ *= ilb_size;
			_la_size_ += _lb_size_;
		}
		_la_size_ *= ila_size;
		__size += _la_size_;
	}
	return __size;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::comp_mx1T_2x_size_mid()
{
	__pseudo_assert__( this->is_mapping_mid() );
	size_type __size = 0;
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->alpha_map::map3ABC_set_la( la );
		this->alpha_map::map3ABC_d_set_la( la );
		_la_size_ = 0;
		for(int lb = 0, ilb_size = 1, _lb_size_; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
		{
			this->alpha_map::map3ABC_set_lb( lb );
			this->alpha_map::map3ABC_d_set_lb( lb );
			// semi-local
			_lb_size_ = 0;
			for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
			{
				this->alpha_map::map3ABC_set_l( l );
				this->alpha_map::map3ABC_d_set_l( l );
				_lb_size_ += this->alpha_map::map3ABC_size();
				_lb_size_ += this->alpha_map::map3ABC_d_size();
			}
			// local
			this->alpha_map::map3ABC_set_l( this->alpha_map::l_max() );
			this->alpha_map::map3ABC_d_set_l( this->alpha_map::l_max() );
			_lb_size_ += this->alpha_map::map3ABC_size();
			_lb_size_ += this->alpha_map::map3ABC_d_size();
			// TODO: spin-orbit
			_lb_size_ *= ilb_size;
			_la_size_ += _lb_size_;
		}
		_la_size_ *= ila_size;
		__size += _la_size_;
	}
	return __size;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::comp_mx1T_2x_size_min()
{
	__pseudo_assert__( this->is_mapping_min() );
	size_type __size = 0;
	for(int la = 0, ila_size = 1, _la_size_; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->alpha_map::map3ABC_set_la( la );
		this->alpha_map::map3ABC_d_set_la( la );
		_la_size_ = 0;
		for(int lb = la%2, ilb_size = (la%2 ? 3 : 1), _lb_size_; lb <= this->alpha_map::lb_max(); lb += 2, ilb_size += (2*lb+1))
		{
			this->alpha_map::map3ABC_set_lb( lb );
			this->alpha_map::map3ABC_d_set_lb( lb );
			// semi-local
			_lb_size_ = 0;
			for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
			{
				this->alpha_map::map3ABC_set_l( l );
				this->alpha_map::map3ABC_d_set_l( l );
				_lb_size_ += this->alpha_map::map3ABC_size();
				_lb_size_ += this->alpha_map::map3ABC_d_size();
			}
			// local
			this->alpha_map::map3ABC_set_l( this->alpha_map::l_max() );
			this->alpha_map::map3ABC_d_set_l( this->alpha_map::l_max() );
			_lb_size_ += this->alpha_map::map3ABC_size();
			_lb_size_ += this->alpha_map::map3ABC_d_size();
			// TODO: spin-orbit
			_lb_size_ *= ilb_size;
			_la_size_ += _lb_size_;
		}
		_la_size_ *= ila_size;
		__size += _la_size_;
		//std::clog << "la = " << la << "; _la_size_ = " << _la_size_ << "; _lb_size_ = " << _lb_size_ << "; __size = " << __size << std::endl;
	}
	return __size;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::write_map( memorystream & ms )
{
	size_type _seek_start = ms.tell();
	union{
		void * _void;
		matrix_cursor_3<mx3node_type> * _mx3pos;
		matrix_cursor_1<T> * _mx1T;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_node = __cnvrt._mx3pos;
	this->_M_node->n() = this->node_N();
	this->_M_node->m() = this->node_M();
	this->_M_node->p() = this->node_P();
	this->_M_node->init_size();
	ms.seek( sizeof(size_struct<3>) + this->_M_node->size() * sizeof(mx3node_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1prim = __cnvrt._mx1T;
	this->mx1prim_size() = this->comp_mx1prim_size();
	this->_M_mx1prim->size() = this->mx1prim_size();
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1prim->size() * sizeof(T), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1func = __cnvrt._mx1T;
	this->mx1func_size() = this->comp_mx1func_size();
	this->_M_mx1func->size() = this->mx1func_size();
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1func->size() * sizeof(T), seek_dir::cur );

	__pseudo_assert__( this->comp_size() == ms.tell() - _seek_start );
	return ms.tell() - _seek_start;
}

template<typename T, typename U> const typename pseudo_integral<T,U>::size_type
pseudo_integral<T,U>::read_map( memorystream & ms )
{
	size_type _seek_start = ms.tell();
	union{
		void * _void;
		matrix_cursor_3<mx3node_type> * _mx3pos;
		matrix_cursor_1<T> * _mx1T;
	} __cnvrt;
	__cnvrt._void = ms.getcur();
	this->_M_node = __cnvrt._mx3pos;
	ms.seek( sizeof(size_struct<3>) + this->_M_node->size() * sizeof(mx3node_type), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1prim = __cnvrt._mx1T;
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1prim->size() * sizeof(T), seek_dir::cur );

	__cnvrt._void = ms.getcur();
	this->_M_mx1func = __cnvrt._mx1T;
	ms.seek( sizeof(size_struct<1>) + this->_M_mx1func->size() * sizeof(T), seek_dir::cur );

	__pseudo_assert__( this->comp_size() == ms.tell() - _seek_start );
	return ms.tell() - _seek_start;
}

template<typename T, typename U> void pseudo_integral<T,U>::init_map()
{
	switch( this->get_mapping() )
	{
	case mapping_struct::minimum: return this->init_map_min();
	case mapping_struct::middle : return this->init_map_mid();
	case mapping_struct::maximum: return this->init_map_max();
	default:
	      this->error("init_map", "unknown mapping type" );
	      std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
	      exit(1);
	}
}

template<typename T, typename U> void pseudo_integral<T,U>::init_map_max()
{
	__pseudo_assert__( this->is_mapping_max() );
	size_type __prim_pos = 0, __func_pos = 0;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );
		this->func_set_la( la );
		for(int ila = 0; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );
				this->func_set_lb( lb );
				for(int ilb = 0; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					// semi-local (+) local
					for(int l = 0; l <= this->alpha_map::l_max(); ++l)
					{
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );

						this->map3node_prim_pos() = __prim_pos;
						this->map3node_prim_size() = this->alpha_map::map3ABC_size();
				__pseudo_assert__( this->map3ABC_size() == this->map1A_size() * this->map1B_size() * this->map1C_size() );
						__prim_pos += this->alpha_map::map3ABC_size();

						this->map3node_func_pos() = __func_pos;
						this->map3node_func_size() = this->alpha_map::map3ABC_d_size();
				__pseudo_assert__( this->map3ABC_d_size() == this->map1A_d_size() * this->map1B_d_size() * this->map1C_d_size() );
						__func_pos += this->alpha_map::map3ABC_d_size();
					}
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_pos == this->comp_mx1prim_size_max() && __func_pos == this->comp_mx1func_size_max() );
	//std::clog << "pseudo_integral<T,U>::init_map_max() DONE" << std::endl;
	return;
}

template<typename T, typename U> void pseudo_integral<T,U>::init_map_mid()
{
	__pseudo_assert__( this->is_mapping_mid() );
	size_type __prim_pos = 0, __func_pos = 0;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );
		this->func_set_la( la );
		for(int ila = 0; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );
				this->func_set_lb( lb );
				for(int ilb = 0; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );

						this->map3node_prim_pos() = __prim_pos;
						this->map3node_prim_size() = this->alpha_map::map3ABC_size();
						__prim_pos += this->alpha_map::map3ABC_size();

						this->map3node_func_pos() = __func_pos;
						this->map3node_func_size() = this->alpha_map::map3ABC_d_size();
						__func_pos += this->alpha_map::map3ABC_d_size();
					}
					// local
					this->map3node_set_lmax();
					this->prim_set_lmax();
					this->func_set_lmax();

					this->map3node_prim_pos() = __prim_pos;
					this->map3node_prim_size() = this->alpha_map::map3ABC_size();
					__prim_pos += this->alpha_map::map3ABC_size();

					this->map3node_func_pos() = __func_pos;
					this->map3node_func_size() = this->alpha_map::map3ABC_d_size();
					__func_pos += this->alpha_map::map3ABC_d_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_pos == this->comp_mx1prim_size_mid() && __func_pos == this->comp_mx1func_size_mid() );
	return;
}

template<typename T, typename U> void pseudo_integral<T,U>::init_map_min()
{
	__pseudo_assert__( this->is_mapping_min() );
	size_type __prim_pos = 0, __func_pos = 0;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );
		this->func_set_la( la );
		for(int ila = 0; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			for(int lb = la%2, ilb_size = (la%2 ? 3 : 1); lb <= this->alpha_map::lb_max(); lb += 2, ilb_size += (2*lb+1))
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );
				this->func_set_lb( lb );
				for(int ilb = 0; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );

						this->map3node_prim_pos() = __prim_pos;
						this->map3node_prim_size() = this->alpha_map::map3ABC_size();
						__prim_pos += this->alpha_map::map3ABC_size();

						this->map3node_func_pos() = __func_pos;
						this->map3node_func_size() = this->alpha_map::map3ABC_d_size();
						__func_pos += this->alpha_map::map3ABC_d_size();
					}
					// local
					this->map3node_set_lmax();
					this->prim_set_lmax();
					this->func_set_lmax();

					this->map3node_prim_pos() = __prim_pos;
					this->map3node_prim_size() = this->alpha_map::map3ABC_size();
					__prim_pos += this->alpha_map::map3ABC_size();

					this->map3node_func_pos() = __func_pos;
					this->map3node_func_size() = this->alpha_map::map3ABC_d_size();
					__func_pos += this->alpha_map::map3ABC_d_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_pos == this->comp_mx1prim_size_min() && __func_pos == this->comp_mx1func_size_min() );
	return;
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_pseudo( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang,
		basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB )
{
	this->comp_prim( qu_rad, ixs_ang );
	//this->comp_func( basA, ecp, basB );
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_func( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB)
{
	switch( this->get_mapping() )
	{
	case mapping_struct::minimum: return this->comp_func_min( basA, ecp, basB );
	case mapping_struct::middle : return this->comp_func_mid( basA, ecp, basB );
	case mapping_struct::maximum: return this->comp_func_max( basA, ecp, basB );
	default:
	      this->error("comp_func", "unknown mapping type" );
	      std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
	      exit(1);
	}
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
	switch( this->get_mapping() )
	{
	case mapping_struct::minimum: return this->comp_prim_min( ixs_ang );
	case mapping_struct::middle : return this->comp_prim_mid( qu_rad, ixs_ang );
	case mapping_struct::maximum: return this->comp_prim_max( qu_rad, ixs_ang );
	default:
	      this->error("comp_prim", "unknown mapping type" );
	      std::cerr << "current mapping type : " << this->get_mapping() << std::endl;
	      exit(1);
	}
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_max( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
	__pseudo_assert__( this->is_mapping_max() );
	size_type __size = 0;
	_lxyz_struct _lxyz;
	this->set_semi_local_count( 0 );// <- static int pseudo_integral<T,U>::semi_local_count
	this->set_local_count( 0 );// <- static int pseudo_integral<T,U>::local_count
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		//this->alpha_map::map1A_set_lx( la );
		this->prim_set_la( la );// <- this->alpha_map::map1A_set_lx( la )
		ixs_ang.map3node_set_la( la );
		qu_rad.qu_dat_set_la( la );
		_lxyz.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			ixs_ang.map3node_set_ia( ila );
			i2abc( la, ila_size, ila, _lxyz.ax, _lxyz.ay ); _lxyz.az = la - _lxyz.ax - _lxyz.ay;
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				//this->alpha_map::map1B_set_lx( lb );
				this->prim_set_lb( lb );// <- this->alpha_map::map1B_set_lx( lb )
				ixs_ang.map3node_set_lb( lb );
				qu_rad.qu_dat_set_lb( lb );
				_lxyz.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					ixs_ang.map3node_set_ib( ilb );
					i2abc( lb, ilb_size, ilb, _lxyz.bx, _lxyz.by ); _lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					//to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					//to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					//if( !to_compute_value ) continue;
					// semi-local
					for(int l = 0; l < this->alpha_map::l_max(); ++l )
					{
						_lxyz.l = l;
						this->map3node_set_l( l );
						//this->alpha_map::map1C_set_lx( l );
						//this->mx1prim_set_it();// M_mx1prim_data() + map3node_pos()
						this->prim_set_l( l );// <- map1C_set_lx( l ); mx1prim_set_it() [M_mx1prim_data() + map3node_pos()]
						ixs_ang.map3node_set_l( l );
						ixs_ang.map3nx2_set_l( l );
						ixs_ang.map2lmbA_set_l( l );
						ixs_ang.map2lmbB_set_l( l );
						qu_rad.qu_dat_set_l( l );
						ixs_ang.mx1ang_set_it();// ixs_ang.M_mx1ang_data() + ixs_ang.map3node_pos()
						// - data of angular integral value - ixs_ang.M_mx1ang_data();
						// - position of ang int value - ixs_ang.ixs_angular_map::map3node_pos();
						// - data of pseudo  integral value - this->M_mx1prim_data();
						// - position of pseudo int value - this->map3node_pos();
						this->comp_prim_max_SemiLocal( _lxyz, qu_rad, ixs_ang );
						__size += this->alpha_map::map3ABC_size();
					}
					// local
					this->map3node_set_l( this->alpha_map::l_max() );
					//this->alpha_map::map1C_set_lx( this->alpha_map::l_max() );
					//this->mx1prim_set_it();
					this->prim_set_l( this->alpha_map::l_max() );// <- map1C_set_lx( l_max ); mx1prim_set_it()
					ixs_ang.map3node_set_lmax();
					ixs_ang.map3nx2_set_lmax();
					ixs_ang.mx1ang_set_it();
					qu_rad.qu_dat_set_lmax();
					this->comp_prim_max_Local( _lxyz, qu_rad, ixs_ang );
					__size += this->alpha_map::map3ABC_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __size == this->comp_mx1prim_size_max() );
	return;
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_max_SemiLocal( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad,
		ixs_angular_dat<T,U> & ixs_ang )
{
	T __prim_value;
	for(int ia = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->prim_set_ia( ia );// <- %map1A_set_ix( ia )
		qu_rad.qu_dat_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->prim_set_ib( ib );// <- %map1B_set_ix( ib )
			qu_rad.qu_dat_set_ib( ib );
			for(int ic = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->prim_set_ic( ic );// <- %map1C_set_ix( ic )
				qu_rad.qu_dat_set_ic( ic );
				this->prim_set_idx();
				this->comp_prim_max_SemiLocal_b( __prim_value, _lxyz, qu_rad, ixs_ang );
				this->prim_value() = __prim_value;
			}
		}
	}
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_max_SemiLocal_b( T & psi_value, _lxyz_struct const & _lxyz,
		qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	static int iter = 0;
#endif
	pointer __p_mx1ang = ixs_ang.mx1ang_data();
	psi_value = 0;
	int __size = 0;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		ixs_ang.map3nx2_set_na( na );
		ixs_ang.map2lmbA_set_nx( na );
		for(int nb = 0, N = na; nb <= _lxyz.lb; ++nb, ++N )
		{
			ixs_ang.map3nx2_set_nb( nb );
			ixs_ang.map2lmbB_set_nx( nb );

			__size += ixs_ang.map3nx2();
			__pseudo_assert__( ixs_ang.map2lmbA_min() == (_lxyz.l < na ? (_lxyz.l+na)%2 : (_lxyz.l - na ) ) );
			__pseudo_assert__( ixs_ang.map2lmbB_min() == (_lxyz.l < nb ? (_lxyz.l+nb)%2 : (_lxyz.l - nb ) ) );
			__pseudo_assert__( ixs_ang.map3nx2() == (ixs_ang.map2lmbA_size() * ixs_ang.map2lmbB_size()) );

			this->incr_semi_local_count( ixs_ang.map3nx2() );// <- semi_local_count
			qu_rad.qu_radial_map::qu_set_N( N );
			for(int i_lmb_a = 0, lmb_a = ixs_ang.map2lmbA_min(); i_lmb_a < ixs_ang.map2lmbA_size(); ++i_lmb_a, lmb_a += 2 )
			{
				qu_rad.qu_set_lmb_a( lmb_a );
				for(int i_lmb_b = 0, lmb_b = ixs_ang.map2lmbB_min(); i_lmb_b < ixs_ang.map2lmbB_size(); ++i_lmb_b, lmb_b += 2,
						++__p_mx1ang )
				{
					if( *__p_mx1ang == 0 )
						continue;
					qu_rad.qu_radial_map::qu_set_lmb_b( lmb_b );

					qu_rad.qu_dat_set_id( qu_rad.qu_radial_map::qu_idx() );
					/*
					this->alpha_val<T,U>::mx1A_set_idx();
					this->alpha_val<T,U>::mx1B_set_idx();
					this->alpha_val<T,U>::mx1C_set_idx();
					T __alp = this->alpha_val<T,U>::mx1A();
					__alp  += this->alpha_val<T,U>::mx1B();
					__alp  += this->alpha_val<T,U>::mx1C();
					this->alpha_val<T,U>::mx1kA_set_idx();
					this->alpha_val<T,U>::mx1kB_set_idx();
					T q_int = q_int_1f11<T>( N + 2, lmb_a, lmb_b, this->mx1kA(), this->mx1kB(), __alp );
					//q_int_1f11<T>( N, lmb_a, lmb_b, ka_len, kb_len, alp );
					psi_value += q_int * *__p_mx1ang;
					*/
					psi_value += qu_rad.qu_dat_value() * *__p_mx1ang;
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
				++iter;
				if( iter % 50 == 0 )
					std::clog <<
					std::setw(3) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" <<
					std::setw(4) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" <<
					std::setw(4) << "l" <<
					std::setw(6) << "na" << std::setw(3) << "nb" << std::setw(4) << "l'a" << std::setw(4) << "l'b" <<
					std::setw(25) << "angular" <<
					std::setw(25) << "radial" <<
					//std::setw(25) << "q_int" <<
					std::setw(25) << "pseudo" <<
					std::endl;
				std::clog <<
					std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
					std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
					std::setw(4) << _lxyz.l <<
					std::setw(6) << na << std::setw(3) << nb << std::setw(4) << lmb_a << std::setw(4) << lmb_b <<
					std::setw(25) << std::setprecision(15) << std::scientific << *__p_mx1ang <<
					std::setw(25) << std::setprecision(15) << std::scientific << qu_rad.qu_dat_value() <<
					//std::setw(25) << std::setprecision(15) << std::scientific << q_int <<
					std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
					std::endl;
#endif
				}
			}
		}
	}
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	std::clog << 
		std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
		std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
		std::setw(4) << _lxyz.l <<
		std::setw(25 * 2 + 17) << "" <<
		std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
		std::endl;
#endif
	__pseudo_assert__( __size == __p_mx1ang - ixs_ang.mx1ang_data() );
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_max_Local( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad,
		ixs_angular_dat<T,U> & ixs_ang )
{
	T psi_value;
	this->map2AB_set_la( _lxyz.la );
	this->map2AB_set_lb( _lxyz.lb );
	for(int ia = 0, ixs_ang_it = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->prim_set_ia( ia );// <- %map1A_set_ix( ia )
		qu_rad.qu_dat_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib, ++ixs_ang_it )
		{
			this->prim_set_ib( ib );// <- %map1B_set_ix( ib )
			qu_rad.qu_dat_set_ib( ib );
			this->map2AB_set_ia( ia );
			this->map2AB_set_ib( ib );
			for(int ic = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->prim_set_ic( ic );// <- %map1C_set_ix( ic )
				qu_rad.qu_dat_set_ic( ic );
				this->prim_set_idx();
				this->comp_prim_max_Local_b( psi_value, _lxyz, qu_rad, ixs_ang, ixs_ang_it );
				this->prim_value() = psi_value;
			}
		}
	}
}
template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_max_Local_b( T & psi_value, _lxyz_struct const & _lxyz,
		qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang, const int ixs_ang_it )
{
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	static int iter = 0;
#endif
	int __size = 0;
	pointer __p_mx1ang = ixs_ang.mx1ang_data() + ixs_ang_it * ixs_ang.map3node_size();
	psi_value = 0;
	for(int na = 0; na <= _lxyz.la; ++na )
	{
		ixs_ang.map3nx2_set_na( na );
		for(int nb = 0, N = na; nb <= _lxyz.lb; ++nb, ++N )
		{
			ixs_ang.map3nx2_set_nb( nb );

			__size += ixs_ang.map3nx2();
			__pseudo_assert__( ixs_ang.map3nx2() == ( N/2 + 1) );
			this->incr_local_count( ixs_ang.map3nx2() );// <- local_count

			qu_rad.qu_radial_map::qu_set_N( N );
			for(int i_lmb = 0, lmb = N%2; i_lmb < ixs_ang.map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang )
			{
				if( *__p_mx1ang == 0 )
					continue;
				qu_rad.qu_radial_map::qu_set_lmb_a( lmb );
				qu_rad.qu_radial_map::qu_set_lmb_b( 0 );

				qu_rad.qu_dat_set_id( qu_rad.qu_radial_map::qu_idx() );
				/*
				this->alpha_val<T,U>::mx1A_set_idx();
				this->alpha_val<T,U>::mx1B_set_idx();
				this->alpha_val<T,U>::mx1C_set_idx();
				T __alp = this->alpha_val<T,U>::mx1A();
				__alp  += this->alpha_val<T,U>::mx1B();
				__alp  += this->alpha_val<T,U>::mx1C();
				this->alpha_val<T,U>::mx2k_set_idx();
				// q_int_1f1<T>( N + 4, lmb, k_len, __alp );
				T q_int = q_int_1f1<T>( N + 2, lmb, this->mx2k(), __alp);
				psi_value += q_int * *__p_mx1ang;
				*/
				psi_value += qu_rad.qu_dat_value() * *__p_mx1ang;
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
				++iter;
				if( iter % 50 == 0 )
					std::clog <<
					std::setw(3) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" <<
					std::setw(4) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" <<
					std::setw(4) << "l" <<
					std::setw(6) << "na" << std::setw(3) << "nb" << std::setw(4) << "lmb" <<
					std::setw(25) << "angular" <<
					std::setw(25) << "radial" <<
					//std::setw(25) << "q_int" <<
					std::setw(25) << "pseudo" <<
					std::endl;
				std::clog <<
					std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
					std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
					std::setw(4) << _lxyz.l <<
					std::setw(6) << na << std::setw(3) << nb << std::setw(4) << lmb <<
					std::setw(25) << std::setprecision(15) << std::scientific << *__p_mx1ang <<
					std::setw(25) << std::setprecision(15) << std::scientific << qu_rad.qu_dat_value() <<
					//std::setw(25) << std::setprecision(15) << std::scientific << q_int <<
					std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
					std::endl;
#endif
			}
		}
	}
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	std::clog << 
		std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
		std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
		std::setw(4) << _lxyz.l <<
		std::setw(25 * 2 + 13) << "" <<
		std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
		std::endl;
#endif
	__pseudo_assert__( __size == __p_mx1ang - (ixs_ang.mx1ang_data() + ixs_ang_it * ixs_ang.map3node_size()) );
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_mid( qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
	__pseudo_assert__( this->is_mapping_mid() );
	size_type __size = 0;
	_lxyz_struct _lxyz;
	this->set_semi_local_count( 0 );// <- semi_local_count
	this->set_local_count( 0 );// <- local_count
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );
		ixs_ang.map3node_set_la( la );
		qu_rad.qu_dat_set_la( la );
		_lxyz.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			ixs_ang.map3node_set_ia( ila );
			i2abc( la, ila_size, ila, _lxyz.ax, _lxyz.ay ); _lxyz.az = la - _lxyz.ax - _lxyz.ay;
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );
				ixs_ang.map3node_set_lb( lb );
				qu_rad.qu_dat_set_lb( lb );
				_lxyz.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					ixs_ang.map3node_set_ib( ilb );
					i2abc( lb, ilb_size, ilb, _lxyz.bx, _lxyz.by ); _lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					//to_compute_set_a( la, _lxyz.ax, _lxyz.ay, _lxyz.az );
					//to_compute_set_b( lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
					//if( !to_compute_value ) continue;
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						_lxyz.l = l;
						this->map3node_set_l( l );
						this->prim_set_l( l );
						ixs_ang.map3node_set_l( l );
						ixs_ang.map3nx2_set_l( l );
						ixs_ang.map2lmbA_set_l( l );
						qu_rad.qu_dat_set_l( l );
						ixs_ang.mx1ang_set_it();// ixs_ang.M_mx1ang_data() + ixs_ang.map3node_pos()
						// - data of angular integral value - ixs_ang.M_mx1ang_data();
						// - position of ang int value - ixs_ang.ixs_angular_map::map3node_pos();
						// - data of pseudo  integral value - this->M_mx1prim_data();
						// - position of pseudo int value - this->map3node_pos();
						this->comp_prim_mid_SemiLocal( _lxyz, qu_rad, ixs_ang );
						__size += this->alpha_map::map3ABC_size();
					}
					// local
					this->map3node_set_l( this->alpha_map::l_max() );
					this->prim_set_l( this->alpha_map::l_max() );
					ixs_ang.map3node_set_lmax();
					ixs_ang.map3nx2_set_lmax();
					ixs_ang.mx1ang_set_it();// ixs_ang.M_mx1ang_data() + ixs_ang.map3node_pos()
					qu_rad.qu_dat_set_lmax();
					this->comp_prim_mid_Local( _lxyz, qu_rad, ixs_ang );
					__size += this->alpha_map::map3ABC_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	//__pseudo_assert__( __size == this->comp_mx1prim_size_mid() );
	return;
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_mid_SemiLocal( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad,
		ixs_angular_dat<T,U> & ixs_ang )
{
	T psi_value;
	for(int ia = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->prim_set_ia( ia );// <- %map1A_set_ix( ia )
		qu_rad.qu_dat_set_ia( ia );
		//this->mx1kA_set_idx();// TODO: remove
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->prim_set_ib( ib );// <- %map1B_set_ix( ib )
			qu_rad.qu_dat_set_ib( ib );
			for(int ic = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->prim_set_ic( ic );// <- %map1C_set_ix( ic )
				qu_rad.qu_dat_set_ic( ic );
				this->prim_set_idx();
				this->comp_prim_mid_SemiLocal_b( psi_value, _lxyz, qu_rad, ixs_ang );
				this->prim_value() = psi_value;
			}
		}
	}
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	std::clog << std::setw(25) << std::setprecision(15) << std::scientific << psi_value << std::endl;
#endif
}
template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_mid_SemiLocal_b( T & psi_value, _lxyz_struct const & _lxyz,
		qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	static int iter = 0;
#endif
	int __size = 0;
	pointer __p_mx1ang = ixs_ang.mx1ang_data();
	psi_value = 0;
	for(int na = 0, N = na + _lxyz.lb; na <= _lxyz.la; ++na, ++N )
	{
		ixs_ang.map2lmbA_set_nx( na );
		ixs_ang.map3nx2_set_na( na );
		ixs_ang.map3nx2_set_nb( 0 );

		__size += ixs_ang.map3nx2();
		__pseudo_assert__( ixs_ang.map3nx2() == ixs_ang.map2lmbA_size() );
		this->incr_semi_local_count( ixs_ang.map3nx2() );// <- semi_local_count

		qu_rad.qu_radial_map::qu_set_N( N );
		for(int i_lmb = 0, lmb = ixs_ang.map2lmbA_min(); i_lmb < ixs_ang.map2lmbA_size(); ++i_lmb, lmb += 2, ++__p_mx1ang )
		{
			//if( *__p_mx1ang == 0 ) continue;
			qu_rad.qu_radial_map::qu_set_lmb_a( lmb );
			qu_rad.qu_radial_map::qu_set_lmb_b( 0 );
			qu_rad.qu_dat_set_id( qu_rad.qu_radial_map::qu_idx() );
			//this->mx1A_set_idx();
			//this->mx1B_set_idx();
			//this->mx1C_set_idx();
			//T _alp = this->mx1A();
			//_alp  += this->mx1B();
			//_alp  += this->mx1C();
			//T q_int = q_int_1f1<T>( N + 4, lmb, this->mx1kA(), _alp );
			//psi_value += q_int * *__p_mx1ang;
			psi_value += qu_rad.qu_dat_value() * *__p_mx1ang;
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
		to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
		to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
		if( to_compute_value )
		{
			if( iter == 0 )
				std::clog << std::endl;
			if( iter % 50 == 0 )
				std::clog <<
				std::setw(3) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" <<
				std::setw(4) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" <<
				std::setw(4) << "l" <<
				std::setw(6) << "N" << std::setw(3) << "na" << std::setw(4) << "lmb" <<
				std::setw(25) << "angular" <<
				std::setw(25) << "radial" <<
				std::setw(25) << "q_int" <<
				std::setw(25) << "pseudo" <<
				std::endl;
			std::clog <<
				std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
				std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
				std::setw(4) << _lxyz.l <<
				std::setw(6) << N << std::setw(3) << na << std::setw(4) << lmb <<
				std::setw(25) << std::setprecision(15) << std::scientific << *__p_mx1ang <<
				std::setw(25) << std::setprecision(15) << std::scientific << qu_rad.qu_dat_value() <<
				//std::setw(25) << std::setprecision(15) << std::scientific << q_int <<
				std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
				std::endl;
			++iter;
		}
#endif
		}
	}
	__pseudo_assert__( __size == __p_mx1ang - ixs_ang.mx1ang_data() );
}
template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_mid_Local( _lxyz_struct const & _lxyz, qu_radial_dat<T,U> & qu_rad,
		ixs_angular_dat<T,U> & ixs_ang )
{
	T psi_value;
	for(int ia = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->prim_set_ia( ia );// <- %map1A_set_ix( ia )
		//this->mx1kA_set_idx();// TODO: remove
		qu_rad.qu_dat_set_ia( ia );
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->prim_set_ib( ib );// <- %map1B_set_ix( ib )
			qu_rad.qu_dat_set_ib( ib );
			for(int ic = 0; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->prim_set_ic( ic );// <- %map1C_set_ix( ic )
				qu_rad.qu_dat_set_ic( ic );
				this->prim_set_idx();
				//std::clog << "Local: " << std::setw(4) << ia << std::setw(4) << ib << std::setw(4) << ic << std::endl;
				this->comp_prim_mid_Local_b( psi_value, _lxyz, qu_rad, ixs_ang );
				this->prim_value() = psi_value;
			}
		}
	}
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	std::clog << std::setw(25) << std::setprecision(15) << std::scientific << psi_value << std::endl;
#endif
}
template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_mid_Local_b( T & psi_value, _lxyz_struct const & _lxyz,
		qu_radial_dat<T,U> & qu_rad, ixs_angular_dat<T,U> & ixs_ang )
{
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	static int iter = 0;
#endif
	int __size = 0;
	pointer __p_mx1ang = ixs_ang.mx1ang_data();
	psi_value = 0;
	for(int na = 0, N = na + _lxyz.lb; na <= _lxyz.la; ++na, ++N )
	{
		ixs_ang.map2lmbA_set_nx( na );
		ixs_ang.map3nx2_set_na( na );
		ixs_ang.map3nx2_set_nb( _lxyz.lb );

		__size += ixs_ang.map3nx2();
		__pseudo_assert__( ixs_ang.map3nx2() == (N/2 + 1) );
		this->incr_local_count( ixs_ang.map3nx2() );// <- local_count
		//std::clog << "local b: " << std::setw(4) << na << std::setw(6) << ixs_ang.map3nx2() << std::endl;

		qu_rad.qu_radial_map::qu_set_N( N );
		for(int i_lmb = 0, lmb = N%2; i_lmb < ixs_ang.map3nx2(); ++i_lmb, lmb += 2, ++__p_mx1ang )
		{
			//if( *__p_mx1ang == 0 ) continue;
			qu_rad.qu_radial_map::qu_set_lmb_a( lmb );
			qu_rad.qu_radial_map::qu_set_lmb_b( 0 );
			qu_rad.qu_dat_set_id( qu_rad.qu_radial_map::qu_idx() );
			//this->mx1A_set_idx();
			//this->mx1B_set_idx();
			//this->mx1C_set_idx();
			//T _alp = this->mx1A(); 
			//_alp  += this->mx1B(); 
			//_alp  += this->mx1C(); 
			//T q_int = q_int_1f1<T>( N + 4, lmb, this->mx1kA(), _alp );
			//psi_value += q_int * *__p_mx1ang;
			psi_value += qu_rad.qu_dat_value() * *__p_mx1ang;
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
		to_compute_set_a( _lxyz.la, _lxyz.ax, _lxyz.ay, _lxyz.az );
		to_compute_set_b( _lxyz.lb, _lxyz.bx, _lxyz.by, _lxyz.bz );
		if( to_compute_value )
		{
			if( iter == 0 )
				std::clog << std::endl;
			if( iter % 50 == 0 )
				std::clog <<
				std::setw(3) << "la" << std::setw(4) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" <<
				std::setw(4) << "lb" << std::setw(4) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" <<
				std::setw(4) << "l" <<
				std::setw(6) << "N" << std::setw(3) << "na" << std::setw(4) << "lmb" <<
				std::setw(25) << "angular" <<
				std::setw(25) << "radial" <<
				std::setw(25) << "q_int" <<
				std::setw(25) << "pseudo" <<
				std::endl;
			std::clog <<
				std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
				std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
				std::setw(4) << _lxyz.l <<
				std::setw(6) << N << std::setw(3) << na << std::setw(4) << lmb <<
				std::setw(25) << std::setprecision(15) << std::scientific << *__p_mx1ang <<
				std::setw(25) << std::setprecision(15) << std::scientific << qu_rad.qu_dat_value() <<
				//std::setw(25) << std::setprecision(15) << std::scientific << q_int <<
				std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
				std::endl;
			++iter;
		}
#endif
		}
	}
#ifdef  __PSEUDO_INTEGRAL_COMP_PRIM_PRINT
	std::clog << 
		std::setw(3) << _lxyz.la << std::setw(4) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
		std::setw(4) << _lxyz.lb << std::setw(4) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
		std::setw(4) << _lxyz.l <<
		std::setw(25 * 3 + 13) << "" <<
		std::setw(25) << std::setprecision(15) << std::scientific << psi_value <<
		std::endl;
#endif
	__pseudo_assert__( __size == __p_mx1ang - ixs_ang.mx1ang_data() );
}

template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_min( ixs_angular_dat<T,U> & ixs_ang )
{
	__pseudo_assert__( this->is_mapping_min() );
	const int ijk_max = this->alpha_map::la_max() + this->alpha_map::lb_max();
	xyz_pown_integral<T> xpn;
	xpn.run( ijk_max, ijk_max, ijk_max ); 
	T const * xpn_v = 0;
	size_type __size = 0;
	this->set_semi_local_count( 0 );// <- semi_local_count
	this->set_local_count( 0 );// <- local_count
	//_lx_struct _lx;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );
		ixs_ang.map3node_set_la( la );
		//_lx.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			ixs_ang.map3node_set_ia( ila );
			i2abc( la, ila_size, ila, ax, ay ); az = la - ax - ay;
			for(int lb = la%2, ilb_size = (la%2 ? 3 : 1); lb <= this->alpha_map::lb_max(); lb += 2, ilb_size += (2*lb+1))
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );
				ixs_ang.map3node_set_lb( lb );
				//_lx.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					ixs_ang.map3node_set_ib( ilb );
					i2abc( lb, ilb_size, ilb, bx, by ); bz = lb - bx - by;
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						//_lx.l = l;
						this->map3node_set_l( l );
						this->prim_set_l( l );
						ixs_ang.map3node_set_l( l );
						ixs_ang.mx1ang_set_it();// ixs_ang.M_mx1ang_data() + ixs_ang.map3node_pos()
						// - data of angular integral value - ixs_ang.M_mx1ang_data();
						// - position of ang int value - ixs_ang.ixs_angular_map::map3node_pos();
						// - data of pseudo  integral value - this->M_mx1prim_data();
						// - position of pseudo int value - this->map3node_pos();
						if( ixs_ang.mx1ang(0) != 0 )
						{
							this->comp_prim_min( la+lb, ixs_ang.mx1ang(0) );
							//this->incr_semi_local_count( this->alpha_map::map3ABC_size() );// <- semi_local_count
							this->incr_semi_local_count( this->map3node_prim_size() );// <- semi_local_count
						} else {
							//for(int i = 0; i < this->alpha_map::map3ABC_size(); ++i)
							for(int i = 0; i < this->map3node_prim_size(); ++i)
								this->mx1prim_it()[i] = 0;
						}
						//__size += this->alpha_map::map3ABC_size();
						__size += this->map3node_prim_size();
					}
					// local
					this->map3node_set_l( this->alpha_map::l_max() );
					this->prim_set_l( this->alpha_map::l_max() );
					xpn_v = &xpn( ax+bx, ay+by, az+bz );
					if( *xpn_v != 0 )
					{
						this->comp_prim_min( la+lb, *xpn_v );
						//this->incr_local_count( this->alpha_map::map3ABC_size() );// <- local_count
						this->incr_local_count( this->map3node_prim_size() );// <- local_count
					} else {
						//for(int i = 0; i < this->alpha_map::map3ABC_size(); ++i)
						for(int i = 0; i < this->map3node_prim_size(); ++i)
							this->mx1prim_it()[i] = 0;
					}
					//__size += this->alpha_map::map3ABC_size();
					__size += this->map3node_prim_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __size == this->comp_mx1prim_size_min() );
	return;
}

// int( r^(p) exp( -alp * r^2 ), dr=0..infty) = 1/2 * alp^( -(p+1)/2 ) * Gamma( (p+1)/2 )
template<typename T> static T pseudo_gamma( const int p, T const & alp )
{
	T value;
	const int n_2 = p + 1, n = n_2 / 2;
	if( n_2%2 == 0 ){// Gamma( n ) = (n-1)!
		value = math::fact_n<u64_t>( n - 1 );
		value /= math::pown<T>( alp, n );
		value *= 0.5;
	} else {// Gamma( n + 1/2 ) = (2n-1)!! * sqrt(Pi) / 2^n
		value = math::fact_2n1_n( n - 1 );// (2 * (n - 1) + 1) = (2*n - 1)
		value *= math::numeric_constants<T>::sqrt_pi;
		T _2alp = T(alp);
		_2alp *= 2;
		T pow_alp = math::pown<T>( _2alp, n );// (2 * alp)^n
		pow_alp *= sqrt( alp );
		value /= pow_alp;// Gamma( n + 1/2 ) / alp^(n + 1/2)
		value *= 0.5;
	}
	return value;
}
template<typename T, typename U> void pseudo_integral<T,U>::comp_prim_min( const int lsum, T const & ixs_ang_value )
{
	T _alp;
	for(int ia = 0; ia < this->alpha_map::map1A_size(); ++ia )
	{
		this->prim_set_ia( ia );// <- %map1A_set_ix( ia )
		this->alpha_val<T,U>::mx1A_set_idx();
		for(int ib = 0; ib < this->alpha_map::map1B_size(); ++ib )
		{
			this->prim_set_ib( ib );// <- %map1B_set_ix( ib )
			this->alpha_val<T,U>::mx1B_set_idx();
			for(int ic = 0, nk; ic < this->alpha_map::map1C_size(); ++ic )
			{
				this->prim_set_ic( ic );// <- %map1C_set_ix( ic )
				this->alpha_val<T,U>::mx1C_set_idx();
				this->prim_set_idx();
				_alp  = this->alpha_val<T,U>::mx1A();
				_alp += this->alpha_val<T,U>::mx1B();
				_alp += this->alpha_val<T,U>::mx1C();
				nk = this->alpha_val<T,U>::mx1C_nk() + __QU_RADIAL_DAT_R_POWN;// __QU_RADIAL_DAT_R_POWN = 2, or 0
				this->prim_value() = pseudo_gamma<T>( lsum + nk, _alp );
				this->prim_value() *= ixs_ang_value;
			}
		}
	}
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func_max( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB )
{
	__pseudo_assert__( this->is_mapping_max() );
	size_type __prim_size = 0, __func_size = 0;
	_lxyz_struct _lxyz;
	typename basis_set<U>::const_pointer p_basA_lx, p_basB_lx;
	typename ecp_set<U>::const_pointer p_ecp_lx;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );// <- alpha_map::map1A_set_lx( la )
		this->func_set_la( la );// <- alpha_map::map1A_d_set_lx( la )
		this->alpha_map::map2AB_norm_set_la( la );
		this->alpha_map::map1A_norm_set_lx( la );
		p_basA_lx = basA.data() + la;
		__pseudo_assert__( p_basA_lx->get_fun_size() == this->alpha_map::map1A_d_size() );
		__pseudo_assert__( p_basA_lx->get_alp_size() == this->alpha_map::map1A_size() );
		_lxyz.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			this->alpha_map::map2AB_norm_set_axyz( ila );
			this->alpha_map::map1A_norm_set_xyz( ila );
			i2abc( la, ila_size, ila, _lxyz.ax, _lxyz.ay ); _lxyz.az = la - _lxyz.ax - _lxyz.ay;
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );// <- alpha_map::map1B_set_lx( lb )
				this->func_set_lb( lb );// <- alpha_map::map1B_d_set_lx( lb )
				this->alpha_map::map2AB_norm_set_lb( lb );
				this->alpha_map::map1B_norm_set_lx( lb );
				p_basB_lx = basB.data() + lb;
				__pseudo_assert__( p_basB_lx->get_fun_size() == this->alpha_map::map1B_d_size() );
				__pseudo_assert__( p_basB_lx->get_alp_size() == this->alpha_map::map1B_size() );
				_lxyz.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					this->alpha_map::map2AB_norm_set_bxyz( ilb );
					this->alpha_map::map1B_norm_set_xyz( ilb );
					i2abc( lb, ilb_size, ilb, _lxyz.bx, _lxyz.by ); _lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					// semi-local (+) local
					for(int l = 0; l <= this->alpha_map::l_max(); ++l )
					{
						_lxyz.l = l;
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );
						p_ecp_lx = ecp.data() + l;
						__pseudo_assert__( p_ecp_lx->size() == this->alpha_map::map1C_d_size() );
						//__pseudo_assert__( p_ecp_lx->get_alp_size() == this->alpha_map::map1C_size() );
#ifdef  __PSEUDO_INTEGRAL_COMP_FUNC_PRINT
						this->comp_func( _lxyz, *p_basA_lx, *p_ecp_lx, *p_basB_lx );
#else
						this->comp_func( *p_basA_lx, *p_ecp_lx, *p_basB_lx );
#endif
						__prim_size += this->alpha_map::map3ABC_size();
						__func_size += this->alpha_map::map3ABC_d_size();
					}
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_size == this->comp_mx1prim_size_max() );
	__pseudo_assert__( __func_size == this->comp_mx1func_size_max() );
	return;
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func_mid( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB )
{
	__pseudo_assert__( this->is_mapping_mid() );
	size_type __prim_size = 0, __func_size = 0;
	//_lxyz_struct _lxyz;
	typename basis_set<U>::const_pointer p_basA_lx, p_basB_lx;
	typename ecp_set<U>::const_pointer p_ecp_lx;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );// <- alpha_map::map1A_set_lx( la )
		this->func_set_la( la );// <- alpha_map::map1A_d_set_lx( la )
		this->alpha_map::map2AB_norm_set_la( la );
		p_basA_lx = basA.data() + la;
		__pseudo_assert__( p_basA_lx->get_fun_size() == this->alpha_map::map1A_d_size() );
		__pseudo_assert__( p_basA_lx->get_alp_size() == this->alpha_map::map1A_size() );
		//_lxyz.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			this->alpha_map::map2AB_norm_set_axyz( ila );
			//i2abc( la, ila_size, ila, _lxyz.ax, _lxyz.ay ); _lxyz.az = la - _lxyz.ax - _lxyz.ay;
			for(int lb = 0, ilb_size = 1; lb <= this->alpha_map::lb_max(); ++lb, ilb_size += (lb + 1) )
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );// <- alpha_map::map1B_set_lx( lb )
				this->func_set_lb( lb );// <- alpha_map::map1B_d_set_lx( lb )
				this->alpha_map::map2AB_norm_set_lb( lb );
				p_basB_lx = basB.data() + lb;
				__pseudo_assert__( p_basB_lx->get_fun_size() == this->alpha_map::map1B_d_size() );
				__pseudo_assert__( p_basB_lx->get_alp_size() == this->alpha_map::map1B_size() );
				//_lxyz.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					this->alpha_map::map2AB_norm_set_bxyz( ilb );
					//i2abc( lb, ilb_size, ilb, _lxyz.bx, _lxyz.by ); _lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						//_lxyz.l = l;
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );
						p_ecp_lx = ecp.data() + l;
						__pseudo_assert__( p_ecp_lx->size() == this->alpha_map::map1C_d_size() );
						//__pseudo_assert__( p_ecp_lx->get_alp_size() == this->alpha_map::map1C_size() );
						this->comp_func( *p_basA_lx, *p_ecp_lx, *p_basB_lx );
						__prim_size += this->alpha_map::map3ABC_size();
						__func_size += this->alpha_map::map3ABC_d_size();
					}
					// local
					//_lxyz.l = l;
					this->map3node_set_lmax();
					this->prim_set_lmax();
					this->func_set_lmax();
					p_ecp_lx = ecp.data() + this->alpha_map::l_max();
					__pseudo_assert__( p_ecp_lx->size() == this->alpha_map::map1C_d_size() );
					//__pseudo_assert__( p_ecp_lx->get_alp_size() == this->alpha_map::map1C_size() );
					this->comp_func( *p_basA_lx, *p_ecp_lx, *p_basB_lx );
					__prim_size += this->alpha_map::map3ABC_size();
					__func_size += this->alpha_map::map3ABC_d_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_size == this->comp_mx1prim_size_mid() );
	__pseudo_assert__( __func_size == this->comp_mx1func_size_mid() );
	return;
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func_min( basis_set<U> const & basA, ecp_set<U> const & ecp, basis_set<U> const & basB )
{
	__pseudo_assert__( this->is_mapping_min() );
	size_type __prim_size = 0, __func_size = 0;
	//_lxyz_struct _lxyz;
	typename basis_set<U>::const_pointer p_basA_lx, p_basB_lx;
	typename ecp_set<U>::const_pointer p_ecp_lx;
	for(int la = 0, ila_size = 1; la <= this->alpha_map::la_max(); ++la, ila_size += (la + 1) )
	{
		this->map3node_set_la( la );
		this->prim_set_la( la );// <- alpha_map::map1A_set_lx( la )
		this->func_set_la( la );// <- alpha_map::map1A_d_set_lx( la )
		this->alpha_map::map2AB_norm_set_la( la );
		p_basA_lx = basA.data() + la;
		__pseudo_assert__( p_basA_lx->get_fun_size() == this->alpha_map::map1A_d_size() );
		__pseudo_assert__( p_basA_lx->get_alp_size() == this->alpha_map::map1A_size() );
		//_lxyz.la = la;
		for(int ila = 0, ax, ay, az; ila < ila_size; ++ila )
		{
			this->map3node_set_axyz( ila );
			this->alpha_map::map2AB_norm_set_axyz( ila );
			//i2abc( la, ila_size, ila, _lxyz.ax, _lxyz.ay ); _lxyz.az = la - _lxyz.ax - _lxyz.ay;
			for(int lb = la%2, ilb_size = (la%2 ? 3 : 1); lb <= this->alpha_map::lb_max(); lb += 2, ilb_size += (2*lb+1))
			{
				this->map3node_set_lb( lb );
				this->prim_set_lb( lb );// <- alpha_map::map1B_set_lx( lb )
				this->func_set_lb( lb );// <- alpha_map::map1B_d_set_lx( lb )
				this->alpha_map::map2AB_norm_set_lb( lb );
				p_basB_lx = basB.data() + lb;
				__pseudo_assert__( p_basB_lx->get_fun_size() == this->alpha_map::map1B_d_size() );
				__pseudo_assert__( p_basB_lx->get_alp_size() == this->alpha_map::map1B_size() );
				//_lxyz.lb = lb;
				for(int ilb = 0, bx, by, bz; ilb < ilb_size; ++ilb )
				{
					this->map3node_set_bxyz( ilb );
					this->alpha_map::map2AB_norm_set_bxyz( ilb );
					//i2abc( lb, ilb_size, ilb, _lxyz.bx, _lxyz.by ); _lxyz.bz = lb - _lxyz.bx - _lxyz.by;
					// semi-local
					for(int l = lb%2; l < this->alpha_map::l_max(); l += 2 )
					{
						//_lxyz.l = l;
						this->map3node_set_l( l );
						this->prim_set_l( l );
						this->func_set_l( l );
						p_ecp_lx = ecp.data() + l;
						__pseudo_assert__( p_ecp_lx->size() == this->alpha_map::map1C_d_size() );
						//__pseudo_assert__( p_ecp_lx->get_alp_size() == this->alpha_map::map1C_size() );
						this->comp_func( *p_basA_lx, *p_ecp_lx, *p_basB_lx );
						__prim_size += this->alpha_map::map3ABC_size();
						__func_size += this->alpha_map::map3ABC_d_size();
					}
					// local
					//_lxyz.l = l;
					this->map3node_set_lmax();
					this->prim_set_lmax();
					this->func_set_lmax();
					p_ecp_lx = ecp.data() + this->alpha_map::l_max();
					__pseudo_assert__( p_ecp_lx->size() == this->alpha_map::map1C_d_size() );
					//__pseudo_assert__( p_ecp_lx->get_alp_size() == this->alpha_map::map1C_size() );
					this->comp_func( *p_basA_lx, *p_ecp_lx, *p_basB_lx );
					__prim_size += this->alpha_map::map3ABC_size();
					__func_size += this->alpha_map::map3ABC_d_size();
					// TODO: spin-orbit
				}
			}
		}
	}
	__pseudo_assert__( __prim_size == this->comp_mx1prim_size_min() );
	__pseudo_assert__( __func_size == this->comp_mx1func_size_min() );
	return;
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func( basis_lx<U> const & basA_lx, ecp_lx<U> const & ecp_lx, basis_lx<U> const & basB_lx )
{
	T __func_value;
	typename basis_lx<U>::const_pointer p_basA_f, p_basB_f;
	typename ecp_lx<U>::const_pointer p_ecp_f;
	for(int ia = 0; ia < this->alpha_map::map1A_d_size(); ++ia )
	{
		this->func_set_ia( ia );// <- %map1A_d_set_ix( ia )
		p_basA_f = basA_lx.data() + ia;
		p_basA_f->get_fun_data();
		p_basA_f->get_alp_pos();
		for(int ib = 0; ib < this->alpha_map::map1B_d_size(); ++ib )
		{
			this->func_set_ib( ib );// <- %map1B_d_set_ix( ib )
			p_basB_f = basB_lx.data() + ib;
			p_basB_f->get_fun_data();
			p_basB_f->get_alp_pos();
			for(int ic = 0; ic < this->alpha_map::map1C_d_size(); ++ic )
			{
				this->func_set_ic( ic );// <- %map1C_d_set_ix( ic )
				this->func_set_idx();
				p_ecp_f = ecp_lx.data() + ic;
				p_ecp_f->get_fun_data();
				p_ecp_f->get_alp_data();
				this->comp_func_b( __func_value, *p_basA_f, *p_ecp_f, *p_basB_f );
				this->func_value() = __func_value;
			}
		}
	}
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func_b( T & __func_value, basis_fun<U> const & basA_f, ecp_fun<U> const & ecp_f, basis_fun<U> const & basB_f )
{
	typename basis_fun<U>::float_type const * p_primA, * p_primB;
	typename ecp_fun<U>::float_type const * p_primC;
	U __coef2AB, __prim_coef, __c_value, __func_value__ = 0;
	__func_value = 0;
	for(int ia = basA_f.get_alp_pos(), i = 0; i < basA_f.get_fun_size(); ++i, ++ia )
	{
		this->prim_set_ia( ia );
		this->alpha_map::map2AB_norm_set_ia( ia );
		p_primA = basA_f.get_fun_data( i );
		for(int ib = basB_f.get_alp_pos(), j = 0; j < basB_f.get_fun_size(); ++j, ++ib )
		{
			this->prim_set_ib( ib );
			this->alpha_map::map2AB_norm_set_ib( ib );
			this->alpha_val<T,U>::mx2AB_norm_set_idx();// alpha_val<T,U>::M_mx2AB_norm_data() + alpha_map::map2AB_norm_idx()
			p_primB = basB_f.get_fun_data( j );
			__coef2AB = *p_primA;
			__coef2AB *= *p_primB;
			__c_value = 0;
			for(int ic = ecp_f.get_alp_pos(), k = 0; k < ecp_f.get_fun_size(); ++k, ++ic )
			{
				this->prim_set_ic( ic );
				this->prim_set_idx();
				p_primC = ecp_f.get_fun_data( k );
				__prim_coef = __coef2AB;
				__prim_coef *= *p_primC;
				__prim_coef *= this->prim_value();
				__c_value += __prim_coef;
			}
			__c_value *= this->alpha_val<T,U>::mx2AB_norm();
			__func_value__ += __c_value;
		}
	}
	__func_value = math::convert_float<T,U>(__func_value__);
}

#ifdef  __PSEUDO_INTEGRAL_COMP_FUNC_PRINT
template<typename T, typename U>
void pseudo_integral<T,U>::comp_func( _lxyz_struct const & _lxyz, basis_lx<U> const & basA_lx, ecp_lx<U> const & ecp_lx, basis_lx<U> const & basB_lx )
{
	T __func_value;
	typename basis_lx<U>::const_pointer p_basA_f, p_basB_f;
	typename ecp_lx<U>::const_pointer p_ecp_f;
	for(int ia = 0; ia < this->alpha_map::map1A_d_size(); ++ia )
	{
		this->func_set_ia( ia );// <- %map1A_d_set_ix( ia )
		p_basA_f = basA_lx.data() + ia;
		p_basA_f->get_fun_data();
		p_basA_f->get_alp_pos();
		for(int ib = 0; ib < this->alpha_map::map1B_d_size(); ++ib )
		{
			this->func_set_ib( ib );// <- %map1B_d_set_ix( ib )
			p_basB_f = basB_lx.data() + ib;
			p_basB_f->get_fun_data();
			p_basB_f->get_alp_pos();
			for(int ic = 0; ic < this->alpha_map::map1C_d_size(); ++ic )
			{
				this->func_set_ic( ic );// <- %map1C_d_set_ix( ic )
				this->func_set_idx();
				p_ecp_f = ecp_lx.data() + ic;
				p_ecp_f->get_fun_data();
				p_ecp_f->get_alp_data();
				this->comp_func_b( _lxyz, ia, ib, ic, __func_value, *p_basA_f, *p_ecp_f, *p_basB_f );
				this->func_value() = __func_value;
			}
		}
	}
}

template<typename T, typename U>
void pseudo_integral<T,U>::comp_func_b( _lxyz_struct const & _lxyz, const int ia_d, const int ib_d, const int ic_d,
		T & __func_value, basis_fun<U> const & basA_f, ecp_fun<U> const & ecp_f, basis_fun<U> const & basB_f )
{
	static int iter = 0;
	static const int IT_MAX = 100000;
	//
	typename basis_fun<U>::float_type const * p_primA, * p_primB;
	typename ecp_fun<U>::float_type const * p_primC;
	U __coef2AB, __prim_coef, __c_value, __func_value__ = 0;
	__func_value = 0;
	for(int ia = basA_f.get_alp_pos(), i = 0; i < basA_f.get_fun_size(); ++i, ++ia )
	{
		this->prim_set_ia( ia );
		this->alpha_map::map2AB_norm_set_ia( ia );
		this->alpha_map::map1A_norm_set_ix( ia );
		this->mx1A_norm_set_idx();
		p_primA = basA_f.get_fun_data( i );
		for(int ib = basB_f.get_alp_pos(), j = 0; j < basB_f.get_fun_size(); ++j, ++ib )
		{
			this->prim_set_ib( ib );
			this->alpha_map::map2AB_norm_set_ib( ib );
			this->alpha_map::map1B_norm_set_ix( ib );
			this->mx1B_norm_set_idx();
			this->alpha_val<T,U>::mx2AB_norm_set_idx();// alpha_val<T,U>::M_mx2AB_norm_data() + alpha_map::map2AB_norm_idx()
			/*
			if( this->mx1A_norm() * this->mx1B_norm() != this->mx2AB_norm() )
			{
				this->error("comp_func_b", "mx2AB_norm != mx1A_norm * mx1B_norm" );
				std::cerr << "map2AB_norm_pos  : " << this->map2AB_norm_pos() << std::endl;
				std::cerr << "map2AB_norm_idx  : " << this->map2AB_norm_idx() << std::endl;
				std::cerr << "M_mx2AB_norm_size: " << this->M_mx2AB_norm_size() << std::endl;
				exit(1);
			}
			*/
			p_primB = basB_f.get_fun_data( j );
			__coef2AB = *p_primA;
			__coef2AB *= *p_primB;
			__c_value = 0;
			for(int ic = ecp_f.get_alp_pos(), k = 0; k < ecp_f.get_fun_size(); ++k, ++ic )
			{
				this->prim_set_ic( ic );
				this->prim_set_idx();
				p_primC = ecp_f.get_fun_data( k );
				__prim_coef = __coef2AB;
				__prim_coef *= *p_primC;
				__prim_coef *= this->prim_value();
				__c_value += __prim_coef;

				if( iter++ < IT_MAX )
				{
				if( iter%50 == 1 )
				std::clog << std::setw(8) << "iter" <<
					std::setw(4) << "la" << std::setw(3) << "ax" << std::setw(3) << "ay" << std::setw(3) << "az" <<
					std::setw(4) << "lb" << std::setw(3) << "bx" << std::setw(3) << "by" << std::setw(3) << "bz" <<
					std::setw(4) << "l" <<
					std::setw(4) << "iaf" << std::setw(4) << "ibf" << std::setw(4) << "icf" <<
					std::setw(4) << "iap" << std::setw(4) << "ibp" << std::setw(4) << "icp" <<
					std::setw(22) << "prim_value" <<
					std::setw(22) << "dA * dB * dC" <<
					std::setw(22) << "mx2AB_norm" <<
					std::setw(26) << "+value" <<
					std::endl;
				std::clog << std::setw(8) << iter <<
					std::setw(4) << _lxyz.la << std::setw(3) << _lxyz.ax << std::setw(3) << _lxyz.ay << std::setw(3) << _lxyz.az <<
					std::setw(4) << _lxyz.lb << std::setw(3) << _lxyz.bx << std::setw(3) << _lxyz.by << std::setw(3) << _lxyz.bz <<
					std::setw(4) << _lxyz.l <<
					std::setw(4) << ia_d << std::setw(4) << ib_d << std::setw(4) << ic_d <<
					std::setw(4) << ia << std::setw(4) << ib << std::setw(4) << ic <<
					std::setw(22) << std::setprecision(12) << std::scientific << this->prim_value() <<
					std::setw(22) << std::setprecision(12) << std::scientific << __prim_coef/this->prim_value() <<
					//std::setw(22) << std::setprecision(12) << std::scientific << this->mx2AB_norm() <<
					std::setw(22) << std::setprecision(12) << std::scientific << this->mx1A_norm() * this->mx1B_norm() <<
					std::setw(26) << std::setprecision(16) << std::scientific << __c_value <<
					std::endl;
				}
			}
			//__c_value *= this->alpha_val<T,U>::mx2AB_norm();
			__c_value *= this->alpha_val<T,U>::mx1A_norm();
			__c_value *= this->alpha_val<T,U>::mx1B_norm();
			__func_value__ += __c_value;
		}
	}
	__func_value = math::convert_float<T,U>(__func_value__);
	if( iter < IT_MAX )
		std::clog << std::endl;
}

#endif

#ifdef  __PSEUDO_INTEGRAL_COUNT
  template<typename T, typename U> int pseudo_integral<T,U>::semi_local_count = 0;
  template<typename T, typename U> int pseudo_integral<T,U>::local_count = 0;
#endif
#define __PSEUDO_INTEGRAL_SPEC2( typeT, typeU )\
 template class pseudo_integral<typeT, typeU>;

#define __PSEUDO_INTEGRAL_SPEC( typeU )\
__PSEUDO_INTEGRAL_SPEC2( float, typeU );\
__PSEUDO_INTEGRAL_SPEC2( double, typeU );\
__PSEUDO_INTEGRAL_SPEC2( long double, typeU );

//template class pseudo_integral<float, typeU>;\
template class pseudo_integral<double, typeU>;\
template class pseudo_integral<long double, typeU>;

#endif//__PSEUSO_INTEGRAL_HPP__
