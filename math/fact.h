#ifndef __FACT_H__
#define __FACT_H__

namespace math
{
	template<typename T> T fact_p(int a, unsigned int p);// (a+p-1)!/(a-1)!
	template<typename T> T const & fact_n(int n);// n!
	const unsigned int & fact_n(int n);
	long double const & fact_2n1(int n);// n!!
	long double const & fact_2n1_n(int n);// (2*n+1)!!
}

#endif//__FACT_H__
