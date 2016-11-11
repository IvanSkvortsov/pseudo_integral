#include"sf.hyperg.2F0.hpp"

SF_HYPERG_2F0_SPEC( float );
SF_HYPERG_2F0_SPEC( double );
SF_HYPERG_2F0_SPEC( long double );

#include"mpreal.use.h"
#ifdef __MPREAL_USE
  #include"mpreal.h"
  SF_HYPERG_2F0_SPEC( mpfr::mpreal );
#endif

