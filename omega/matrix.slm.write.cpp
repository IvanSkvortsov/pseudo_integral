#include"matrix.slm.write.hpp"

MATRIX_SLM_WRITE_SPEC( float );
MATRIX_SLM_WRITE_SPEC( double );
MATRIX_SLM_WRITE_SPEC( long double );

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
MATRIX_SLM_WRITE_SPEC( mpfr::mpreal );
#endif

