#include"alpha.mem.hpp"
__ALPHA_MEM_SPEC( float );
__ALPHA_MEM_SPEC( double );
__ALPHA_MEM_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
__ALPHA_MEM_SPEC( mpfr::mpreal );
#endif
