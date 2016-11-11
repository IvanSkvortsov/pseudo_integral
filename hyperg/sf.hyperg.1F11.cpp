#include"sf.hyperg.1F11.hpp"

SF_HYPERG_1F11_SER_SPEC( float );
SF_HYPERG_1F11_SER_SPEC( double );
SF_HYPERG_1F11_SER_SPEC( long double );
#include"mpreal.use.h"
#ifdef __MPREAL_USE
  #include"mpreal.h"
  SF_HYPERG_1F11_SER_SPEC( mpfr::mpreal );
#endif

