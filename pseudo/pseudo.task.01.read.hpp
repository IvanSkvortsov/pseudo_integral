#ifndef __PSEUDO_TASK_01_READ_HPP__
#define __PSEUDO_TASK_01_READ_HPP__
#include"pseudo.task.01.read.h"

template<typename T> void _3_centers<T>::read( std::istream & inp, T * r )
{
	for(int i = 0; i < 3; ++i)
		inp >> r[i];
}
template<typename T> void _3_centers<T>::read_a( std::istream & inp ){ _3_centers<T>::read( inp, this->A ); }
template<typename T> void _3_centers<T>::read_b( std::istream & inp ){ _3_centers<T>::read( inp, this->B ); }
template<typename T> void _3_centers<T>::read_c( std::istream & inp ){ _3_centers<T>::read( inp, this->C ); }

template<typename T> void _3_alphas<T>::read( std::istream & inp, T & alp ){ inp >> alp; }
template<typename T> void _3_alphas<T>::read_a( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_a ); }
template<typename T> void _3_alphas<T>::read_b( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_b ); }
template<typename T> void _3_alphas<T>::read_c( std::istream & inp ){ _3_alphas<T>::read( inp, this->alp_c ); }

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
