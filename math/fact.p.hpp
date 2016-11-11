#ifndef __FACT_P_HPP__
#define __FACT_P_HPP__
#include"fact.h"
#include<stdlib.h>
#include<iostream>

typedef unsigned int u32_t;

// fact_p(a, p) = a * (a+1) ... (a+p-1)
// equal to fact_n(a+p-1) / fact_n(a-1)
template<typename T>
T math::fact_p(int a, u32_t p)
{
	if( p == 0 ) return 1;
	if( a < 0 )
	{
		std::cerr << "Error: [fact_p] a : " << a << std::endl;
		exit(1);
	}
	T value = T(a++);
	for(u32_t i = 1u; i < p; ++i, ++a)
	{
		if( a == 0 )
		{
			value = 0;
			break;
		}
		value *= a;
	}
	return value;
}

template int math::fact_p<int>(int a, u32_t p);
template float math::fact_p<float>(int a, u32_t p);
template double math::fact_p<double>(int a, u32_t p);
template long double math::fact_p<long double>(int a, u32_t p);

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
template mpfr::mpreal math::fact_p<mpfr::mpreal>(int a, u32_t p);
#endif

#endif//__FACT_P_HPP__
