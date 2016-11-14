#ifndef __PSEUDO_TASK_01_READ_HPP__
#define __PSEUDO_TASK_01_READ_HPP__
#include"pseudo.task.01.read.h"
#include<iomanip>

template<typename T> void _3_centers<T>::read( std::istream & inp, T * r )
{
	for(int i = 0; i < 3; ++i)
		inp >> r[i];
}
template<typename T> void _3_centers<T>::read_a( std::istream & inp ){ _3_centers<T>::read( inp, this->A ); }
template<typename T> void _3_centers<T>::read_b( std::istream & inp ){ _3_centers<T>::read( inp, this->B ); }
template<typename T> void _3_centers<T>::read_c( std::istream & inp ){ _3_centers<T>::read( inp, this->C ); }
template<typename T> void _3_centers<T>::read( std::istream & inp )
{
	this->read_a( inp );
	this->read_b( inp );
	this->read_c( inp );
}
template<typename T> inline static void vec3_print( std::ostream & out, T const * r3, const char * name, const int w = 25 )
{
	out << std::setw(6) << name << ":";
	for(int i = 0; i < 3; ++i)
		out << std::setw( w ) << r3[i];
	out << std::endl;
}
template<typename T> void _3_centers<T>::print( std::ostream & out, const int w )const
{
	vec3_print<T>( out, this->A, "A", w );
	vec3_print<T>( out, this->B, "B", w );
	vec3_print<T>( out, this->C, "C", w );
}
template<typename T> void _3_centers<T>::flip()
{
	const T _r_[3] = {this->A[0], this->A[1], this->A[2]};
	for(int i = 0; i < 3; ++i)
		this->A[i] = this->B[i];
	for(int i = 0; i < 3; ++i)
		this->B[i] = _r_[i];
}

template<typename T> void _3_alphas<T>::read( std::istream & inp, T & alp ){ inp >> alp; }
template<typename T> void _3_alphas<T>::read_a( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_a ); }
template<typename T> void _3_alphas<T>::read_b( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_b ); }
template<typename T> void _3_alphas<T>::read_c( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_c ); }
template<typename T> void _3_alphas<T>::read( std::istream & inp )
{
	this->read_a( inp );
	this->read_b( inp );
	this->read_c( inp );
}
template<typename T> void _3_alphas<T>::print( std::ostream & out, const int w )const
{
	out << "alp_a: " << std::setw(w) << this->alp_a << std::endl;
	out << "alp_b: " << std::setw(w) << this->alp_b << std::endl;
	out << "alp_c: " << std::setw(w) << this->alp_c << std::endl;
}
template<typename T> void _3_alphas<T>::flip()
{
	T _alp_ = T(this->alp_a);
	this->alp_a = this->alp_b;
	this->alp_b = _alp_;
}

void _3_momenta::read_xyz( std::istream & inp, int & _lx_, int * _x_)
{
	_lx_ = 0;
	for(int i = 0; i < 3; ++i)
	{
		inp >> _x_[i];
		_lx_ += _x_[i];
	}
}
void _3_momenta::read( std::istream & inp )
{
	_3_momenta::read_xyz( inp, this->la, this->ax );
	_3_momenta::read_xyz( inp, this->lb, this->bx );
	inp >> this->l;
	inp >> this->nk;
}
inline static void print_xyz( std::ostream & out, const int _lx_, const int * _x_ )
{
	out << std::setw(4) << "lx" << ": " << std::setw(4) << _lx_;
	for(int i = 0; i < 3; ++i)
		out << std::setw( (i?3:4) ) << _x_[i];
	out << std::endl;
}
void _3_momenta::print( std::ostream & out )const
{
	out << "a"; print_xyz( out, this->la, this->ax );
	out << "b"; print_xyz( out, this->lb, this->bx );
}
void _3_momenta::flip()
{
	const int lx = this->la;
	this->la = this->lb;
	this->lb = lx;

	const int _x_[3] = {this->ax[0], this->ax[1], this->ax[2]};
	for(int i = 0; i < 3; ++i )
		this->ax[i] = this->bx[i];
	for(int i = 0; i < 3; ++i )
		this->bx[i] = _x_[i];
}
int _3_momenta::n2 = 2;

#define __PSEUDO_TASK_01_READ_SPEC( type )\
template class _3_centers<type>;\
template class _3_alphas<type>;

#endif//__PSEUDO_TASK_01_READ_HPP__
