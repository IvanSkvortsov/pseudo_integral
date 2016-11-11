#include"kahan.hpp"

MATH_KAHAN_SPEC( float );
MATH_KAHAN_SPEC( double );
MATH_KAHAN_SPEC( long double );

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
MATH_KAHAN_SPEC( mpfr::mpreal );
#endif
