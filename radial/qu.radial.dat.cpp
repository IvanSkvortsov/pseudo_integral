#include"qu.radial.dat.hpp"
__QU_RADIAL_DAT_SPEC( float );
__QU_RADIAL_DAT_SPEC( double );
__QU_RADIAL_DAT_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __QU_RADIAL_DAT_SPEC( mpfr::mpreal );
#endif
