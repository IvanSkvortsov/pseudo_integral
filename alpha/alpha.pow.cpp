#include"alpha.pow.hpp"

__ALPHA_POW_SPEC( float );
__ALPHA_POW_SPEC( double );
__ALPHA_POW_SPEC( long double );

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
__ALPHA_POW_SPEC( mpfr::mpreal );
#endif
