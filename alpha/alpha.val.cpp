#include"alpha.val.hpp"
__ALPHA_VAL_SPEC( float );
__ALPHA_VAL_SPEC( double );
__ALPHA_VAL_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
__ALPHA_VAL_SPEC( mpfr::mpreal );
#endif
