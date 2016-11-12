#include"pseudo.integral.mem.hpp"
__PSEUDO_INTEGRAL_MEM_SPEC( float );
__PSEUDO_INTEGRAL_MEM_SPEC( double );
__PSEUDO_INTEGRAL_MEM_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __PSEUDO_INTEGRAL_MEM_SPEC( mpfr::mpreal );
#endif
