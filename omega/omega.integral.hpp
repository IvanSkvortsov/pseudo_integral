#ifndef __OMEGA_INTEGRAL_HPP__
#define __OMEGA_INTEGRAL_HPP__
#include"omega.integral.h"
#include"skv.is.same.type.h"

static int is_omega_zero(const int x, const int y, const int z, const int l, const int lmb )
{
	const int n = x + y + z;
	if( lmb <= (l + n) || l <= (lmb + n) || (l + n + lmb)%2 == 0 )
		return 0;
	return 1;
}

#include<iostream>
#include<cstdlib>// exit
#include<cmath>// fma (fused muliply-add)

template<typename polynomialT, typename xyzIntegralT>
void omega_integral(typename xyzIntegralT::float_type & resultValue,
		const int xp, const int yp, const int zp, polynomialT const & sph, xyzIntegralT const & xpn_int, const int l, const int lmb)
{
	if( !skv::is_same_type<typename polynomialT::value_type::float_type, typename xyzIntegralT::float_type>::value )
	{
		std::cerr << "Error: [omega_integral] different floating point types" << std::endl;
		exit(1);
	}
	typedef typename polynomialT::value_type  polynomialValueT;
	typedef typename polynomialT::value_type::float_type floatT;

	resultValue = 0;
	if( is_omega_zero( xp, yp, zp, l, lmb ) )
		return;
	int const * x = 0;
	polynomialValueT const * p = sph.data();
	for(int i = 0; i < sph.size(); ++i, ++p)
	{
		x = p->x;
		// fma(x, y, z) = (x * y) + z
		//resultValue = fma( p->d, xpn_int( x[0]+xp, x[1]+yp, x[2]+zp ), resultValue );
		resultValue += ( p->d * xpn_int( x[0]+xp, x[1]+yp, x[2]+zp ) );
	}
	return;
}

#include"xyz.pown.integral.h"
#include"poly.n.h"
#include"matrix.cursor.h"

template<typename float_type> using xpi_type = xyz_pown_integral<float_type>;
template<typename float_type> using pol_type = matrix_pointer_1<poly_n<float_type,3> >;
template<typename polinomialT> using pol_float_type = typename polinomialT::value_type::float_type;

#define OMEGA_INTEGRAL_SPEC( type )\
template pol_float_type<pol_type<type> > omega_integral<pol_type<type>, xpi_type<type> >(const int xp, const int yp, const int zp, pol_type<type> const & v, xpi_type<type> const & xpn_int, const int l, const int lmb);\
template void omega_integral<pol_type<type>, xpi_type<type> >(pol_float_type<pol_type<type> > & resultValue, const int xp, const int yp, const int zp, pol_type<type> const & v, xpi_type<type> const & xpn_int, const int l, const int lmb);

//#define OMEGA_INTEGRAL_SPEC( type )\
template class pointer_v<poly_n<type,3> >;\
template typename pol_type<type>::value_type::float_type omega_integral<pol_type<type>, xpi_type<type> >(const int xp, const int yp, const int zp, pol_type<type> const & v, xpi_type<type> const & xpn_int, const int l, const int lmb);\
template void omega_integral<pol_type<type>, xpi_type<type> >(typename pol_type<type>::value_type::float_type & resultValue, const int xp, const int yp, const int zp, pol_type<type> const & v, xpi_type<type> const & xpn_int, const int l, const int lmb);


//#define OMEGA_INTEGRAL_SPEC( type )\
template class pointer_v<poly_n<type,3> >;\
template typename pointer_v<poly_n<type,3> >::value_type::float_type omega_integral<pointer_v<poly_n<type,3> >, xyz_pown_integral<type> >(const int xp, const int yp, const int zp, pointer_v<poly_n<type,3> > const & v, xyz_pown_integral<type> const & xpn_int, const int l, const int lmb);\
template void omega_integral<pointer_v<poly_n<type,3> >, xyz_pown_integral<type> >(typename pointer_v<poly_n<type,3> >::value_type::float_type & resultValue, const int xp, const int yp, const int zp, pointer_v<poly_n<type,3> > const & v, xyz_pown_integral<type> const & xpn_int, const int l, const int lmb);

OMEGA_INTEGRAL_SPEC( float )
OMEGA_INTEGRAL_SPEC( double )
OMEGA_INTEGRAL_SPEC( long double )

#include"mpreal.use.h"
#ifdef __MPREAL_USE
#include"mpreal.h"
OMEGA_INTEGRAL_SPEC( mpfr::mpreal )
#endif


#endif//__OMEGA_INTEGRAL_HPP__
