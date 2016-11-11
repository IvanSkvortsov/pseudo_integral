#ifndef __LOGN_HPP__
#define __LOGN_HPP__
#include"logn.h"

int math::logn_z(int x, int base)
{
	if( x < 0 ) return -1;
	if( base <= 1 ) return -2;
	int i = 0;
	//for(unsigned int x_base = base; x >= x_base; x_base *= base, ++i);
	for(; x >= base; x /= base, ++i);
	return i;
}
int math::log10_z(int x)
{
	if( x < 0 ) return -1;
	int i = 0;
	//for(unsigned int x_10 = 10u; x >= x_10; x_10 *= 10u, ++i);
	for(; x >= 10; x /= 10, ++i);
	return i;
}

#endif//__LOGN_HPP__
