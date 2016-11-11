#ifndef __EQUALITY_H__
#define __EQUALITY_H__

#define MATH_LDBL_EPSILON 2.1684043449710088680149056017399e-19L// 2^(-62)
#define MATH_DBL_EPSILON  4.4408920985006261616945266723633e-16// 2^(-51)
#define MATH_FLT_EPSILON  2.384185791015625e-07F// 2^(-22)

// P.S.
// FLT_EPSILON := 2^(-23)
// DBL_EPSILON := 2^(-52)
// LDBL_EPSILON := 2^(-63)

typedef unsigned int u_t;

namespace math
{
	// function template
	template<typename T> bool is_zero(T const & x);
	template<typename T> bool is_equal(T const & a, T const & b);
	template<typename T> bool is_equal(T const * a, T const * b, int const & size);
	// specialization:
	// integer type : int, unsigned int
	template<> bool is_zero(int const & x);
	template<> bool is_zero(u_t const & x);

	template<> bool is_equal(int const & a, int const & b);
	template<> bool is_equal(u_t const & a, u_t const & b);

	template<> bool is_equal(int const * a, int const * b, int const & size);
	template<> bool is_equal(u_t const * a, u_t const * b, int const & size);
}

#endif//__EQUALITY_H__
