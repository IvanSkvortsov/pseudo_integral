#include"pseudo.task.01.read.hpp"
__PSEUDO_TASK_01_READ_SPEC( float );
__PSEUDO_TASK_01_READ_SPEC( double );
__PSEUDO_TASK_01_READ_SPEC( long double );

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __PSEUDO_TASK_01_READ_SPEC( mpfr::mpreal );
#endif

