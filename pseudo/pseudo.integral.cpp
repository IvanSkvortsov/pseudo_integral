#include"pseudo.integral.hpp"

__PSEUDO_INTEGRAL_SPEC( float );
__PSEUDO_INTEGRAL_SPEC( double );
__PSEUDO_INTEGRAL_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __PSEUDO_INTEGRAL_SPEC( mpfr::mpreal );
#endif
