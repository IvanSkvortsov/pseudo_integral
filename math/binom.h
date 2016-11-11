#ifndef __BINOM_H__
#define __BINOM_H__

//#define __BINOM_0
#define __BINOM_1
//#define __BINOM_2

typedef   signed int s32_t;
typedef unsigned int u32_t;
typedef   signed long long int s64_t;
typedef unsigned long long int u64_t;

namespace math
{
	template<typename T> T cnk(int n, int k);// n!/ (k! * (n-k)!)
	//template<typename T> T const & cnk_data(int n, int k);
	u32_t cnk(int n, int k);
}

#endif//__BINOM_H__
