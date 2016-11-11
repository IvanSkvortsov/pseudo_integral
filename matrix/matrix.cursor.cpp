#include"matrix.cursor.h"

MATRIX_POINTER_SPEC( int );
MATRIX_POINTER_SPEC( long int );
MATRIX_POINTER_SPEC( long long int );
MATRIX_POINTER_SPEC( unsigned int );
MATRIX_POINTER_SPEC( unsigned long int );
MATRIX_POINTER_SPEC( unsigned long long int );
MATRIX_POINTER_SPEC( float );
MATRIX_POINTER_SPEC( double );
MATRIX_POINTER_SPEC( long double );

MATRIX_CURSOR_SPEC( int );
MATRIX_CURSOR_SPEC( long int );
MATRIX_CURSOR_SPEC( long long int );
MATRIX_CURSOR_SPEC( unsigned int );
MATRIX_CURSOR_SPEC( unsigned long int );
MATRIX_CURSOR_SPEC( unsigned long long int );
MATRIX_CURSOR_SPEC( float );
MATRIX_CURSOR_SPEC( double );
MATRIX_CURSOR_SPEC( long double );

#include"mpreal.use.h"
#if defined (__MPREAL_USE)
#include"mpreal.h"
MATRIX_CURSOR_SPEC( mpfr::mpreal );
MATRIX_POINTER_SPEC( mpfr::mpreal );
#endif
