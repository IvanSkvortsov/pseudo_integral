#ifndef __FMA_H__
#define __FMA_H__
// fused multiply-add

namespace math
{
	template<typename T> T fma(T const & v1, T const & v2, T const & v3);// v1*v2 + v3
}

#endif//__FMA_H__
