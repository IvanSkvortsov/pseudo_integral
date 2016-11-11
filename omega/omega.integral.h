#ifndef __OMEGA_INTEGRAL_H__
#define __OMEGA_INTEGRAL_H__

template<typename polynomialT, typename xyzIntegralT>
inline typename xyzIntegralT::float_type
omega_integral(const int xp, const int yp, const int zp, polynomialT const & sph, xyzIntegralT const & xpn_int, const int l, const int lmb)
{
	static typename polynomialT::value_type::float_type resultValue(0);
	omega_integral<polynomialT, xyzIntegralT>( resultValue, xp, yp, zp, sph, xpn_int, l, lmb);
	return resultValue;
}

template<typename polynomialT, typename xyzIntegralT> void omega_integral(typename xyzIntegralT::float_type & result,
		const int x, const int y, const int z, polynomialT const & v, xyzIntegralT const & xpn_int, const int l, const int lmb);

#endif//__OMEGA_INTEGRAL_H__
