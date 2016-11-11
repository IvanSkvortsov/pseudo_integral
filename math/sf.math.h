#ifndef __SF_MATH_H__
#define __SF_MATH_H__
#include"sf.machine.h"
#include"expmanip.h"
#include"equality.h"
#include"fact.h"
#include"pown.h"
#include"logn.h"
#include"binom.h"
#include"numeric.constants.h"
#include"numeric.limits.h"
#include"fma.h"
#include"cast.h"
#include"kahan.h"
#include"convert.float.h"
#include"machine.epsilon.h"
#include"precision.error.h"
#include"sf.exponent.h"
/*
#include"expmanip/expmanip.h"
#include"equality/equality.h"
#include"fact/fact.h"
#include"pow/pown.h"
#include"log/logn.h"
#include"binom/binom.h"
*/

#ifndef M_E
#define M_E         2.71828182845904523536028747135
#endif

#ifndef SQRT_PI
#define SQRT_PI     1.77245385090551602729816748334// sqrt( Pi ) 
#endif

#ifndef HALF_SQRT_PI
#define HALF_SQRT_PI 8.86226925452758013649083741671e-01// sqrt( Pi ) / 2 
#endif

#ifndef M_PI
#define M_PI          3.14159265358979323846264338328// Pi
#endif

#define FLT_PI       3.14159265358979323846264338328F// Pi
#define DBL_PI       3.14159265358979323846264338328 // Pi
#define LDBL_PI      3.14159265358979323846264338328L// Pi

#define SQRT_PI_POW3    5.56832799683170784528481798211// sqrt( Pi^3 )
#define SQR_PI          9.86960440108935861883449099988// Pi^2
#define SQRT_PI_POW5    17.4934183276248628462628216798// Pi^( 5/2 )

#define HALF_PI     1.57079632679489661923132169164// Pi/2
#define _2_SQRT_PI   3.54490770181103205459633496668// 2 * sqrt( Pi )

#define ISQRT_2     0.707106781186547524400844362105

#define I4PI   0.795774715459476678844418816863e-1

#ifdef  __MP_CONSTANTS
#ifndef __MPREAL_H__
#error mpreal.h header required [from sf.math.h]
#endif

#ifndef MP_E
#define MP_E        "2.718281828459045235360287471352662497757247093699959574966967627724076630353547594571382178525166427"
#endif

#ifndef MP_SQRT_PI
#define MP_SQRT_PI     "1.772453850905516027298167483341145182797549456122387128213807789852911284591032181374950656738544665"// sqrt( Pi ) 
#endif

#ifndef MP_PI
#define MP_PI          "3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117068"// Pi
#endif

#define MP_SQRT_PI_POW3    "5.568327996831707845284817982118835702013624390283243910753675818829745533647795702212177687384708490"// sqrt( Pi^3 )
#define MP_SQR_PI          "9.869604401089358618834490999876151135313699407240790626413349376220044822419205243001773403718552232"// Pi^2
#define MP_SQRT_PI_POW5    "17.49341832762486284626282167987155377875511178174418037761742290526545379899074304860803156261160296"// Pi^( 5/2 )
//
#define MP_HALF_PI     "1.570796326794896619231321691639751442098584699687552910487472296153908203143104499314017412671058534"// Pi/2
#define MP_2_SQRT_PI   "3.544907701811032054596334966682290365595098912244774256427615579705822569182064362749901313477089330"// 2 * sqrt( Pi )

#define MP_ISQRT_2     ".7071067811865475244008443621048490392848359376884740365883398689953662392310535194251937671638207865"// 1/sqrt(2)

#endif//__MP_CONSTANTS

#ifndef INT32_MAX
#define INT32_MAX   2147483647
#endif

#endif//__SF_MATH_H__
