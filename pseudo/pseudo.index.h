#ifndef __PSEUDO_INDEX_H__
#define __PSEUDO_INDEX_H__
#include"n.abc.h"// abc2i, i2abc
#include<cstring>// memcpy

#define INLINE_PS_IDX inline
//#define INLINE_PS_IDX

#define __PSEUDO_INDEX_INT_INTERFACE( method, source )\
INLINE_PS_IDX void pseudo_index::set_##method( int __idx ){ (source) = __idx; }\
INLINE_PS_IDX const int pseudo_index::get_##method()const{ return (source); }

#define __PSEUDO_INDEX_INT3_INTERFACE( method, source, lx_source, ilx_source )\
INLINE_PS_IDX void pseudo_index::set_##method( int __x, int __y, int __z )\
{\
	(source)[0] = __x;\
	(source)[1] = __y;\
	(source)[2] = __z;\
	/* this->set_##method( abc2i( (lx_source), this->get_##method(0), this->get_##method(1) ) );*/\
	(ilx_source) = abc2i( (lx_source), (source)[0], (source)[1] );\
}\
INLINE_PS_IDX const int pseudo_index::get_##method(int i)const{ return (source)[i]; }

struct pseudo_index
{
	int la, ax[3], ila, lb, bx[3], ilb, l, ia_f, ib_f, ic_f, ia_p, ib_p, ic_p;
	pseudo_index();
	pseudo_index( pseudo_index const & );
	pseudo_index & operator=( pseudo_index const & );
	void set_la( int );
	const int get_la()const;
	void set_axyz( int, int, int );// ila
	void set_axyz( int );// ila
	const int get_axyz( int )const;
	const int get_axyz()const;
	//
	void set_lb( int );
	const int get_lb()const;
	void set_bxyz( int, int, int );// ilb
	void set_bxyz( int );// ilb
	const int get_bxyz( int )const;
	const int get_bxyz()const;
	//
	void set_l( int );
	const int get_l()const;
	//
	void set_ia_f( int );
	const int get_ia_f()const;
	void set_ib_f( int );
	const int get_ib_f()const;
	void set_ic_f( int );
	const int get_ic_f()const;
	//
	void set_ia_p( int );
	const int get_ia_p()const;
	void set_ib_p( int );
	const int get_ib_p()const;
	void set_ic_p( int );
	const int get_ic_p()const;
};

INLINE_PS_IDX pseudo_index::pseudo_index(): la(), ax{0, 0, 0}, ila(), lb(), bx{0, 0, 0}, ilb(), l(), ia_f(), ib_f(), ic_f(), ia_p(), ib_p(), ic_p(){}
INLINE_PS_IDX pseudo_index::pseudo_index( pseudo_index const & v ): la( v.la ), ax{v.ax[0], v.ax[1], v.ax[2]}, ila( v.ila ),
	lb( v.lb ), bx{v.bx[0], v.bx[1], v.bx[2]}, ilb( v.ilb ), l( v.l ),
	ia_f( v.ia_f ), ib_f( v.ib_f ), ic_f( v.ic_f ), ia_p( v.ia_p ), ib_p( v.ib_p ), ic_p( v.ic_p ){}
INLINE_PS_IDX pseudo_index & pseudo_index::operator=( pseudo_index const & v )
{
	if( this == &v ) return *this;
	memcpy( this, &v, sizeof(*this) );
	return *this;
}
__PSEUDO_INDEX_INT_INTERFACE( la, this->la );
__PSEUDO_INDEX_INT_INTERFACE( lb, this->lb );
__PSEUDO_INDEX_INT_INTERFACE( l, this->l );
__PSEUDO_INDEX_INT_INTERFACE( axyz, this->ila );
__PSEUDO_INDEX_INT_INTERFACE( bxyz, this->ilb );
__PSEUDO_INDEX_INT_INTERFACE( ia_f, this->ia_f );
__PSEUDO_INDEX_INT_INTERFACE( ib_f, this->ib_f );
__PSEUDO_INDEX_INT_INTERFACE( ic_f, this->ic_f );
__PSEUDO_INDEX_INT_INTERFACE( ia_p, this->ia_p );
__PSEUDO_INDEX_INT_INTERFACE( ib_p, this->ib_p );
__PSEUDO_INDEX_INT_INTERFACE( ic_p, this->ic_p );
__PSEUDO_INDEX_INT3_INTERFACE( axyz, this->ax, this->la, this->ila );
__PSEUDO_INDEX_INT3_INTERFACE( bxyz, this->bx, this->lb, this->ilb );

#endif//__PSEUDO_INDEX_H__
