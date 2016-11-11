#ifndef __SF_MACHINE_H__
#define __SF_MACHINE_H__

// float
//
// bit field (32):
// 1  - sign
// 8  - exponent
// 23 - mantissa
//
// mantissa = 1 + Sum_i=0^22 m[i]/2^i
#define SF_FLT_EPSILON       1.1920928955078125e-07f// 2^(-23)
#define SF_SQRT_FLT_EPSILON  3.4526698300124391e-04f
#define SF_FLT_EPSILON_STR  "1.1920928955078125e-07f"

#define SF_FLT_MIN           1.1754943508222875e-38f// 2^(-126)
#define SF_LOG_FLT_MIN      -8.7336544750553109e+01f
#define SF_SQRT_FLT_MIN      1.0842021724855044e-19f
#define SF_FLT_MIN_STR      "1.1754943508222875e-38f"

#define SF_FLT_MAX           3.4028232635611925e+38f// 2^(128) - 2^(128-23)
#define SF_LOG_FLT_MAX       8.8722838992463703e+01f
#define SF_SQRT_FLT_MAX      1.8446742974197891e+19f
#define SF_FLT_MAX_STR      "3.4028232635611925e+38f"

#define SF_FLT_MAX_E_M23      4.0564814371600062e+31f// FLT_MAX * FLT_EPSILON
#define SF_FLT_MAX_E_M23_STR "4.0564814371600062e+31f"

#define SF_FLT_MAX_EXP       127
#define SF_FLT_MIN_EXP      -126
#define SF_FLT_PRECISION     23

// double
//
// bit field (64):
// 1  - sign
// 11 - exponent
// 52 - mantissa
//
// mantissa = 1 + Sum_i=0^51 m[i]/2^i
#define SF_DBL_EPSILON       2.2204460492503131e-16 // 2^(-52)
#define SF_SQRT_DBL_EPSILON  1.4901161193847656e-08
#define SF_DBL_EPSILON_STR  "2.2204460492503131e-16"

#define SF_DBL_MIN           2.2250738585072014e-308// 2^(-1022)
#define SF_LOG_DBL_MIN      -708.3964185322641062244
#define SF_SQRT_DBL_MIN      1.4916681462400413e-154
#define SF_DBL_MIN_STR      "2.2250738585072014e-308"

#define SF_DBL_MAX           1.7976931348623155e+308// 2^(1024) - 2^(1024-52)
#define SF_LOG_DBL_MAX       709.7827128933839966212
#define SF_SQRT_DBL_MAX      1.3407807929942596e+154
#define SF_DBL_MAX_STR      "1.7976931348623155e+308"

#define SF_DBL_MAX_E_M52      3.991680619069439180e+292// DBL_MAX * DBL_EPSILON
#define SF_DBL_MAX_E_M52_STR "3.991680619069439180e+292"

#define SF_DBL_MAX_EXP       1023
#define SF_DBL_MIN_EXP      -1022
#define SF_DBL_PRECISION     52

#define SF_DBL_LOG10_2       3.0102999566398119521e-01
#define SF_DBL_LN_10         2.3025850929940456840e+00

#define SF_DBL_LOG10_2_LLZ0  4599094494223104510ull
#define SF_DBL_LOG10_2_LLZ1  4363533108023570477ull
#define SF_DBL_LOG10_2_MANT_0  919274677828094LL
#define SF_DBL_LOG10_2_EXP_0  -2
#define SF_DBL_LOG10_2_MANT_1  4048668728930349LL
#define SF_DBL_LOG10_2_EXP_1  -55

#define SF_DBL_LOG_10_0      2.30258509299404501
#define SF_DBL_LOG_10_1      6.71102797361900322e-16

#define SF_DBL_INF           SF_DBL_MAX

// long double
//
// bit field (80):
// 1  - sign
// 15 - exponent
// 64 - mantissa
//
// mantissa = 1 + Sum_i=1^63 m[i]/2^i (P.S. here "i=1" isn't misprint)
// so that 1st bit is always set to 1
#define SF_LDBL_EPSILON       1.084202172485504434007e-19L// 2^(-63)
#define SF_SQRT_LDBL_EPSILON  3.292722539913596233357e-10L
#define SF_LDBL_EPSILON_STR  "1.084202172485504434007e-19L"

#define SF_LDBL_MIN           3.362103143112093506263e-4932L// 2^(-16382)
#define SF_LOG_LDBL_MIN      -11355.13711193302405887309661L
#define SF_SQRT_LDBL_MIN      1.833603867554847165621e-2466L
#define SF_LDBL_MIN_STR      "3.362103143112093506263e-4932L"

#define SF_LDBL_MAX           1.189731495357231764957e+4932L// 2^(16384) - 2^(16384-63)
#define SF_LOG_LDBL_MAX       11356.52340629414394949182266L
#define SF_SQRT_LDBL_MAX      1.090748135619415929404e+2466L
#define SF_LDBL_MAX_STR      "1.189731495357231764957e+4932L"

#define SF_LDBL_MAX_E_M63      1.289909471940738511748e+4913L// LDBL_MAX * LDBL_EPSILON
#define SF_LDBL_MAX_E_M63_STR "1.289909471940738511748e+4913L"

#define SF_LDBL_MAX_EXP       16383
#define SF_LDBL_MIN_EXP      -16382
#define SF_LDBL_PRECISION     63

// Ivan Skvortsov
// 2015.12.14  15:10
//
// P.S. [float, double, long double]
// 1) Two remained exponent values are used to store special values : Inf and Nan
//    so 'Inf' has sign according to sign bit
//    and 'Nan' (not number) in this case unfortunately has also sign;
//
// 2) Bits printing of these three types of values shew that it isn't so:
//    only one exponent value is used (-128/-1024/-16384)
//    so that if exponent -eq -128/-1024/-16384 then if matissa -eq 1 then it is '+/-Inf' else it is 'Nan';
// 
// According to 2nd statement minimum values defined should be 2 times less
//
// UPD
// Ivan Skvortsov
// 2016.01.19  15:45
//
// 3) Usually -128/-1024/-16384 and -127/-1023/-16383 values of exponent are used to store '-/+Inf' and 'Nan' respectevely,
//    so minimum value of floating point variable is determined by -126/-1022/-16382
//    but according to 2nd statement it isn't true...

#undef  __MP_CONSTANTS

#define SF_MPF_EPSILON       "1.4916681462400413486581930630926e-154"// 2^(-511)
#define SF_MPF_MIN           "2.38256e-323228497"// ~2^(- 2^30)
#define SF_MPF_MAX           "2.09858e+323228496"// ~2^(+ 2^30)

#if defined( __MP_CONSTANTS )

  #ifndef __MPREAL_H__
  #error mpreal.h header required [from sf.machine.h]
  #endif

  #define SF_MPF_EPSILON       mpfr::mpreal::machine_epsilon()
  #define SF_SQRT_MPF_EPSILON  sqrt(mpfr::mpreal::machine_epsilon())

  #define SF_MPF_MIN           mpfr::mpreal::minval()
  #define SF_LOG_MPF_MIN       log(mpfr::mpreal::minval())
  #define SF_SQRT_MPF_MIN      sqrt(mpfr::mpreal::minval())

  #define SF_MPF_MAX           mpfr::mpreal::maxval()
  #define SF_LOG_MPF_MAX       log(mpfr::mpreal::maxval())
  #define SF_SQRT_MPF_MAX      sqrt(mpfr::mpreal::maxval())

#endif//__MP_CONSTANTS

#ifndef __MACHINE_BIT_H__
#define __MACHINE_BIT_H__

#ifndef MANT64
#define MANT64 0x000FFFFFFFFFFFFFull
#endif

#ifndef BIT_53
#define BIT_53 0x0010000000000000ull
#endif

#ifndef BITS51
#define BITS51 0x0007FFFFFFFFFFFFull
#endif

#ifndef BITS52
#define BITS52 0x000FFFFFFFFFFFFFull
#endif

#ifndef BITS53
#define BITS53 0x001FFFFFFFFFFFFFull
#endif

#ifndef SIGN64
#define SIGN64 0X8000000000000000ull;
#endif

#endif//__MACHINE_BIT_H__

#endif//__SF_MACHINE_H__
