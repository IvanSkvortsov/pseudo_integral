#ifndef __NUMERIC_CONSTANTS_H__
#define __NUMERIC_CONSTANTS_H__
#include"mpreal.use.h"

#ifdef __MPREAL_USE
#include"mpreal.h"
#endif

namespace math
{
	template<typename T> class numeric_constants
	{
	public:
		static const T sqrt_pi_pow5;// PI^(+2.5)
		static const T sqr_pi;//       PI^(+2.0)
		static const T sqrt_pi_pow3;// PI^(+1.5)
		static const T pi;//           PI^(+1.0)
		static const T sqrt_pi;//      PI^(+0.5)
		static const T i_sqrt_pi;//    PI^(-0.5)
		static const T i_pi;//         PI^(-1.0)
		static const T sqrt_2;
		static const T e;
		static const T log10_e;
		static const T log10_2;
		static const T log_2;
		static const T log_10;
		static const T log2_e;
		static const T log2_10;
	};
	template<> class numeric_constants<float>
	{	
	public:
		static const float sqrt_pi_pow5;// = 17.4934183276248628462628216798f;// PI^(+2.5)
		static const float sqr_pi;//       = 9.86960440108935861883449099988f;// PI^(+2.0)
		static const float sqrt_pi_pow3;// = 5.56832799683170784528481798211f;// PI^(+1.5)
		static const float pi;//           = 3.14159265358979323846264338328f;// PI^(+1.0)
		static const float sqrt_pi;//      = 1.77245385090551602729816748334f;// PI^(+0.5)
		static const float i_sqrt_pi;//    = .564189583547756286948079451561f;// PI^(-0.5)
		static const float i_pi;//         = .318309886183790671537767526745f;// PI^(-1.0)
		static const float sqrt_2;//       = 1.41421356237309504880168872421f;
		static const float e;//            = 2.71828182845904523536028747135f;
		static const float log10_e;//      = .434294481903251827651128918917f;
		static const float log10_2;//      = .301029995663981195213738894724f;
		static const float log_2;//        = .693147180559945309417232121458f;
		static const float log_10;//       = 2.30258509299404568401799145468f;
		static const float log2_e;//       = 1.44269504088896340735992468100f;
		static const float log2_10;//      = 3.32192809488736234787031942949f;
	};
	template<> class numeric_constants<double>
	{	
	public:
		static const double sqrt_pi_pow5;// = 17.4934183276248628462628216798;// PI^(+2.5)
		static const double sqr_pi;//       = 9.86960440108935861883449099988;// PI^(+2.0)
		static const double sqrt_pi_pow3;// = 5.56832799683170784528481798211;// PI^(+1.5)
		static const double pi;//           = 3.14159265358979323846264338328;// PI^(+1.0)
		static const double sqrt_pi;//      = 1.77245385090551602729816748334;// PI^(+0.5)
		static const double i_sqrt_pi;//    = .564189583547756286948079451561;// PI^(-0.5)
		static const double i_pi;//         = .318309886183790671537767526745;// PI^(-1.0)
		static const double sqrt_2;//       = 1.41421356237309504880168872421;
		static const double e;//            = 2.71828182845904523536028747135;
		static const double log10_e;//      = .434294481903251827651128918917;
		static const double log10_2;//      = .301029995663981195213738894724;
		static const double log_2;//        = .693147180559945309417232121458;
		static const double log_10;//       = 2.30258509299404568401799145468;
		static const double log2_e;//       = 1.44269504088896340735992468100;
		static const double log2_10;//      = 3.32192809488736234787031942949;
	};
	template<> class numeric_constants<long double>
	{	
	public:
		static const long double sqrt_pi_pow5;// = 17.4934183276248628462628216798L;// PI^(+2.5)
		static const long double sqr_pi;//       = 9.86960440108935861883449099988L;// PI^(+2.0)
		static const long double sqrt_pi_pow3;// = 5.56832799683170784528481798211L;// PI^(+1.5)
		static const long double pi;//           = 3.14159265358979323846264338328L;// PI^(+1.0)
		static const long double sqrt_pi;//      = 1.77245385090551602729816748334L;// PI^(+0.5)
		static const long double i_sqrt_pi;//    = .564189583547756286948079451561L;// PI^(-0.5)
		static const long double i_pi;//         = .318309886183790671537767526745L;// PI^(-1.0)
		static const long double sqrt_2;//       = 1.41421356237309504880168872421L;
		static const long double e;//            = 2.71828182845904523536028747135L;
		static const long double log10_e;//      = .434294481903251827651128918917L;
		static const long double log10_2;//      = .301029995663981195213738894724L;
		static const long double log_2;//        = .693147180559945309417232121458L;
		static const long double log_10;//       = 2.30258509299404568401799145468L;
		static const long double log2_e;//       = 1.44269504088896340735992468100L;
		static const long double log2_10;//      = 3.32192809488736234787031942949L;
	};
#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )
	template<> class numeric_constants<mpfr::mpreal>
	{	
	public:
		static const mpfr::mpreal sqrt_pi_pow5;
		static const mpfr::mpreal       sqr_pi;
		static const mpfr::mpreal sqrt_pi_pow3;
		static const mpfr::mpreal           pi;
		static const mpfr::mpreal      sqrt_pi;
		static const mpfr::mpreal    i_sqrt_pi;
		static const mpfr::mpreal         i_pi;
		static const mpfr::mpreal sqrt_2;
		static const mpfr::mpreal e;
		static const mpfr::mpreal log10_e;
		static const mpfr::mpreal log10_2;
		static const mpfr::mpreal log_2;
		static const mpfr::mpreal log_10;
		static const mpfr::mpreal log2_e;
		static const mpfr::mpreal log2_10;
	};
#endif
}

#endif//__NUMERIC_CONSTANTS_H__
