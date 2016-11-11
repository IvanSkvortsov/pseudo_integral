#ifndef __NUMERIC_CONSTANTS_HPP__
#define __NUMERIC_CONSTANTS_HPP__
#include"numeric.constants.h"

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
#endif

namespace math
{
	template<class T> const T numeric_constants<T>::sqrt_pi_pow5 = 17.4934183276248628462628216798;// PI^(+2.5)
	template<class T> const T numeric_constants<T>::sqr_pi       = 9.86960440108935861883449099988;// PI^(+2.0)
	template<class T> const T numeric_constants<T>::sqrt_pi_pow3 = 5.56832799683170784528481798211;// PI^(+1.5)
	template<class T> const T numeric_constants<T>::pi           = 3.14159265358979323846264338328;// PI^(+1.0)
	template<class T> const T numeric_constants<T>::sqrt_pi      = 1.77245385090551602729816748334;// PI^(+0.5)
	template<class T> const T numeric_constants<T>::i_sqrt_pi    = .564189583547756286948079451561;// PI^(-0.5)
	template<class T> const T numeric_constants<T>::i_pi         = .318309886183790671537767526745;// PI^(-1.0)
	template<class T> const T numeric_constants<T>::sqrt_2       = 1.41421356237309504880168872421;
	template<class T> const T numeric_constants<T>::e            = 2.71828182845904523536028747135;
	template<class T> const T numeric_constants<T>::log10_e      = .434294481903251827651128918917;
	template<class T> const T numeric_constants<T>::log10_2      = .301029995663981195213738894724;
	template<class T> const T numeric_constants<T>::log_2        = .693147180559945309417232121458;
	template<class T> const T numeric_constants<T>::log_10       = 2.30258509299404568401799145468;
	template<class T> const T numeric_constants<T>::log2_e       = 1.44269504088896340735992468100;
	template<class T> const T numeric_constants<T>::log2_10      = 3.32192809488736234787031942949;

	const float numeric_constants<float>::sqrt_pi_pow5 = 17.4934183276248628462628216798f;// PI^(+2.5)
	const float numeric_constants<float>::sqr_pi       = 9.86960440108935861883449099988f;// PI^(+2.0)
	const float numeric_constants<float>::sqrt_pi_pow3 = 5.56832799683170784528481798211f;// PI^(+1.5)
	const float numeric_constants<float>::pi           = 3.14159265358979323846264338328f;// PI^(+1.0)
	const float numeric_constants<float>::sqrt_pi      = 1.77245385090551602729816748334f;// PI^(+0.5)
	const float numeric_constants<float>::i_sqrt_pi    = .564189583547756286948079451561f;// PI^(-0.5)
	const float numeric_constants<float>::i_pi         = .318309886183790671537767526745f;// PI^(-1.0)
	const float numeric_constants<float>::sqrt_2       = 1.41421356237309504880168872421f;
	const float numeric_constants<float>::e            = 2.71828182845904523536028747135f;
	const float numeric_constants<float>::log10_e      = .434294481903251827651128918917f;
	const float numeric_constants<float>::log10_2      = .301029995663981195213738894724f;
	const float numeric_constants<float>::log_2        = .693147180559945309417232121458f;
	const float numeric_constants<float>::log_10       = 2.30258509299404568401799145468f;
	const float numeric_constants<float>::log2_e       = 1.44269504088896340735992468100f;
	const float numeric_constants<float>::log2_10      = 3.32192809488736234787031942949f;

	const double numeric_constants<double>::sqrt_pi_pow5 = 17.4934183276248628462628216798;// PI^(+2.5)
	const double numeric_constants<double>::sqr_pi       = 9.86960440108935861883449099988;// PI^(+2.0)
	const double numeric_constants<double>::sqrt_pi_pow3 = 5.56832799683170784528481798211;// PI^(+1.5)
	const double numeric_constants<double>::pi           = 3.14159265358979323846264338328;// PI^(+1.0)
	const double numeric_constants<double>::sqrt_pi      = 1.77245385090551602729816748334;// PI^(+0.5)
	const double numeric_constants<double>::i_sqrt_pi    = .564189583547756286948079451561;// PI^(-0.5)
	const double numeric_constants<double>::i_pi         = .318309886183790671537767526745;// PI^(-1.0)
	const double numeric_constants<double>::sqrt_2       = 1.41421356237309504880168872421;
	const double numeric_constants<double>::e            = 2.71828182845904523536028747135;
	const double numeric_constants<double>::log10_e      = .434294481903251827651128918917;
	const double numeric_constants<double>::log10_2      = .301029995663981195213738894724;
	const double numeric_constants<double>::log_2        = .693147180559945309417232121458;
	const double numeric_constants<double>::log_10       = 2.30258509299404568401799145468;
	const double numeric_constants<double>::log2_e       = 1.44269504088896340735992468100;
	const double numeric_constants<double>::log2_10      = 3.32192809488736234787031942949;

	const long double numeric_constants<long double>::sqrt_pi_pow5 = 17.4934183276248628462628216798L;// PI^(+2.5)
	const long double numeric_constants<long double>::sqr_pi       = 9.86960440108935861883449099988L;// PI^(+2.0)
	const long double numeric_constants<long double>::sqrt_pi_pow3 = 5.56832799683170784528481798211L;// PI^(+1.5)
	const long double numeric_constants<long double>::pi           = 3.14159265358979323846264338328L;// PI^(+1.0)
	const long double numeric_constants<long double>::sqrt_pi      = 1.77245385090551602729816748334L;// PI^(+0.5)
	const long double numeric_constants<long double>::i_sqrt_pi    = .564189583547756286948079451561L;// PI^(-0.5)
	const long double numeric_constants<long double>::i_pi         = .318309886183790671537767526745L;// PI^(-1.0)
	const long double numeric_constants<long double>::sqrt_2       = 1.41421356237309504880168872421L;
	const long double numeric_constants<long double>::e            = 2.71828182845904523536028747135L;
	const long double numeric_constants<long double>::log10_e      = .434294481903251827651128918917L;
	const long double numeric_constants<long double>::log10_2      = .301029995663981195213738894724L;
	const long double numeric_constants<long double>::log_2        = .693147180559945309417232121458L;
	const long double numeric_constants<long double>::log_10       = 2.30258509299404568401799145468L;
	const long double numeric_constants<long double>::log2_e       = 1.44269504088896340735992468100L;
	const long double numeric_constants<long double>::log2_10      = 3.32192809488736234787031942949L;

#if defined( __MPREAL_USE ) && defined( __MPREAL_H__ )
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::sqrt_pi_pow5 =
		"17.49341832762486284626282167987155377875511178174418037761742290526545379899074304860803156261160296";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::sqr_pi =
		"9.869604401089358618834490999876151135313699407240790626413349376220044822419205243001773403718552232";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::sqrt_pi_pow3 =
		"5.568327996831707845284817982118835702013624390283243910753675818829745533647795702212177687384708490";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::pi =
		"3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::sqrt_pi =
		"1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::i_sqrt_pi =
		".5641895835477562869480794515607725858440506293289988568440857217106424684414934144867436602021073636";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::i_pi =
		".3183098861837906715377675267450287240689192914809128974953346881177935952684530701802276055325061719";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::sqrt_2 =
		"1.414213562373095048801688724209698078569671875376948073176679737990732478462107038850387534327641573";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::e =
		"2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log10_e =
		".4342944819032518276511289189166050822943970058036665661144537831658646492088707747292249493384317484";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log10_2 =
		".3010299956639811952137388947244930267681898814621085413104274611271081892744245094869272521181861721";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log_2 =
		".6931471805599453094172321214581765680755001343602552541206800094933936219696947156058633269964186875";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log_10 =
		"2.302585092994045684017991454684364207601101488628772976033327900967572609677352480235997205089598298";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log2_e =
		"1.442695040888963407359924681001892137426645954152985934135449406931109219181185079885526622893506345";
	const mpfr::mpreal math::numeric_constants<mpfr::mpreal>::log2_10 = 
		"3.321928094887362347870319429489390175864831393024580612054756395815934776608625215850139743359370155";
#endif
}


#endif//__NUMERIC_CONSTANTS_HPP__
