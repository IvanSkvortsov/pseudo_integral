#include"alpha.slm.hpp"
__ALPHA_SLM_SPEC( float );
__ALPHA_SLM_SPEC( double );
__ALPHA_SLM_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
__ALPHA_SLM_SPEC( mpfr::mpreal );
#endif
