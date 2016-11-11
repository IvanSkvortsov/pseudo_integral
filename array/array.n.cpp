#include"array.n.hpp"

__ARRAY_N_SPEC( int );
__ARRAY_N_SPEC( long int );
__ARRAY_N_SPEC( long long int );
__ARRAY_N_SPEC( unsigned int );
__ARRAY_N_SPEC( unsigned long int );
__ARRAY_N_SPEC( unsigned long long int );
__ARRAY_N_SPEC( float );
__ARRAY_N_SPEC( double );
__ARRAY_N_SPEC( long double );

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __ARRAY_N_SPEC( mpfr::mpreal );
#endif

