#ifndef __N_ABC_H__
#define __N_ABC_H__

//         a   b   c  iter
// -----------------
// (n+1)   0   0   n    0
//         0   1 n-1    1
//         .........
//         0   n   0    n
// -----------------
// (n)     1   0 n-1  n+1
//         1   1 n-2  n+2
//         .........  ...
//         1 n-1   0  2*n
// -----------------
// .................  ...
//
// (2)   n-1   0   1  ...
//       n-1   1   0  ...
// -----------------
// (1)     n   0   0  (n+1)(n+2)/2-1
//
// so that Sum_(i=1)^(i=n+1) = (n+1)*(n+2)/2
//
// problem: how to find %iter if we know %a, %b, %c
// Let S(n) = (n+1) * (n+2)/2;
// S(n) - S(n-a) = a * n + (3*a - a*a)/2; it is new %a values start position,
// and S(n) - S(n-a) + b -- is position of triple{%a, %b, %c} and is our %iter, where %n = %a + %b + %c
//
inline int abc2i(const int n, const int a, const int b)
{
	return a * n + (3*a - a * a)/2 + b;
}

inline void i2abc(const int n, const int np1_np2_d2, const int i, int & a, int & b)
{
	// variant 1
	int iter = np1_np2_d2;// (n+1) * (n+2) / 2
	--iter;
	a = n;
	for(int a_sz = 1; a_sz <= n && iter > i; ++a_sz, iter -= a_sz, --a);
	b = i - iter;
}
// all of them are working methods
inline void i2abc(const int n, const int i, int & a, int & b)
{
	// variant 1
	int size = (n+1);
	size *= (n+2);
	size /= 2;
	return i2abc( n, size, i, a, b );
}
/*
inline void i2abc_01(const int n, const int i, int & a, int & b)
{
	return i2abc(n, i, a, b);
}
inline void i2abc_02(const int n, const int i, int & a, int & b)
{
	// variant 2
	int iter = (n+1);
	a = 0;
	for(int a_sz = n; a_sz >= 0 && iter <= i; iter += a_sz, --a_sz, ++a)
	{
		if( iter > i )
		{
			iter -= a_sz;
			iter -= 1;
			b = i - iter;
			return;
		}
	}
}
*/

#endif//__N_ABC_H__
