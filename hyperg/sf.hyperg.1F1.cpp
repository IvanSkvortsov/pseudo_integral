#include"sf.hyperg.1F1.hpp"

SF_HYPERG_1F1_SPEC( float );
SF_HYPERG_1F1_SPEC( double );
SF_HYPERG_1F1_SPEC( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  SF_HYPERG_1F1_SPEC( mpfr::mpreal );
#endif

