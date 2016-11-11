#include"vector.3d.hpp"
VECTOR_3D_SPEC_INT( int );
VECTOR_3D_SPEC_INT( long int );
VECTOR_3D_SPEC_INT( long long int );
VECTOR_3D_SPEC_INT( unsigned int );
VECTOR_3D_SPEC_INT( unsigned long int );
VECTOR_3D_SPEC_INT( unsigned long long int );

VECTOR_3D_SPEC_REAL( float );
VECTOR_3D_SPEC_REAL( double );
VECTOR_3D_SPEC_REAL( long double );
#include"mpreal.use.h"
#ifdef  __MPREAL_USE
#include"mpreal.h"
VECTOR_3D_SPEC_REAL( mpfr::mpreal );
#endif
