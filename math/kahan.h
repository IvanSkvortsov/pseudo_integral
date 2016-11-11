#ifndef __KAHAN_H__
#define __KAHAN_H__

namespace math
{
	template<class T> void kahan_add(T & a, T & a_c, T const & b);
	template<class T> void kahan_sub(T & a, T & a_c, T const & b);
	template<class T> struct kahan
	{
		typedef T value_type;
		typedef T float_type;
		T x, y;

		kahan();
		kahan(kahan<T> const & k);
		kahan(T const & __x);
		kahan(T const & __x, T const & __y);
		~kahan();

		kahan<T> & operator=( kahan<T> const & k );
		kahan<T> & operator=( T const & __x );

		kahan<T> & operator+=( T const & __x );
		kahan<T> & operator-=( T const & __x );
	};
};

#endif//__KAHAN_H__
