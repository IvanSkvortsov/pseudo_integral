#ifndef __BINOM_HPP__
#define __BINOM_HPP__
#include"binom.h"

#if defined(__BINOM_0)
#include"binom.uint32.0.hpp"
#include"binom.uint64.0.hpp"
#elif defined(__BINOM_1)
#include"binom.uint32.1.hpp"
#include"binom.uint64.1.hpp"
#elif defined(__BINOM_2)
#include"binom.uint32.2.hpp"
#include"binom.uint64.2.hpp"
#endif

#include<stdlib.h>// exit
#include<iostream>

/*
typedef   signed int s32_t;
typedef unsigned int u32_t;
typedef   signed long long int s64_t;
typedef unsigned long long int u64_t;
*/

template<typename T>
static T const & cnk_data(int n, int k)
{
	return cnk_uint32_data[cnk_uint32_idx[n] + k];
}
template<> u32_t const & cnk_data<u32_t>(int n, int k)
{
	return cnk_uint32_data[cnk_uint32_idx[n] + k];
}
template<> u64_t const & cnk_data<u64_t>(int n, int k)
{
	return cnk_uint64_data[cnk_uint64_idx[n] + k];
}
/*
template<typename T>
T const & math::cnk_data(int n, int k);
{
	return cnk_uint32_data[cnk_uint32_idx[n] + k];
}
template<> u32_t const & math::cnk_data(int n, int k)
{
	return cnk_uint32_data[cnk_uint32_idx[n] + k];
}
template<> u64_t const & math::cnk_data<u64_t>(int n, int k)
{
	return cnk_uint64_data[cnk_uint64_idx[n] + k];
}
*/

template<typename T>
static int cnk_nmax(){return BINOM_UINT32_NMAX;}

template<> int cnk_nmax<u32_t>(){return BINOM_UINT32_NMAX;}
template<> int cnk_nmax<u64_t>(){return BINOM_UINT64_NMAX;}

template<typename T>
T math::cnk(int n, int k)
{
	if( n > cnk_nmax<T>())
	{
		std::cerr << "Error: [cnk] n/NMAX : " << n << '/' << cnk_nmax<T>() << std::endl;
		exit(1);
	}
	else if (n < 0 )
	{
		std::cerr << "Error: [cnk] n : " << n << std::endl;
		exit(1);
	}
	if( k < 0 || k > n )
	{
		std::cerr << "Error: [cnk] n/k : " << n << '/' << k << std::endl;
		exit(1);
	}
#if defined(__BINOM_1) || defined(__BINOM_2)
	if( k > (n>>1u) ) k = n-k;
#endif
#if defined(__BINOM_0) || defined(__BINOM_1)
	return cnk_data<T>(n, k);
#elif defined(__BINOM_2)
	if( k == 0 ) return 1;
	else if( k == n ) return n;
	else return cnk_data<T>(n, k);
#endif
}

u32_t math::cnk(int n, int k)
{
	return math::cnk<u32_t>(n, k);
}

template u32_t math::cnk<u32_t>(int n, int k);
template u64_t math::cnk<u64_t>(int n, int k);

#endif//__BINOM_HPP__
