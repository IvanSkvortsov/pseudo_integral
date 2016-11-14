#ifndef __PSEUDO_TASK_01_READ_H__
#define __PSEUDO_TASK_01_READ_H__
#include<iostream>

template<typename T>
struct _3_centers
{
	T A[3], B[3], C[3];
	static void read( std::istream & inp, T * r);
	void read_a( std::istream & inp );
	void read_b( std::istream & inp );
	void read_c( std::istream & inp );
	void read( std::istream & inp );
};

template<typename T>
struct _3_alphas
{
	T alp_a, alp_b, alp_c;
	static void read( std::istream & inp, T & alp );
	void read_a( std::istream & inp );
	void read_b( std::istream & inp );
	void read_c( std::istream & inp );
	void read( std::istream & inp );
};

struct _3_momenta
{
	int la, ax[3];
	int lb, bx[3];
	int l, nk;
	static int n2;
	static void read_xyz( std::istream & inp, int & _lx_, int * _x_ );
	void read( std::istream & inp );
	void flip();
};

#endif//__PSEUDO_TASK_01_READ_H__
