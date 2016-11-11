#ifndef __POWN_H__
#define __POWN_H__

namespace math
{
	template<typename T> void pown(T const & x, int n, T & value);
	template<typename T> T pown(T const & x, int n);
	template<typename T> int pown05( T const & x, const int _pown05_max, T * _arr, const int _arr_size );
}

#endif//__POWN_H__
