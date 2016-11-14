#include"pseudo.task.01.write.hpp"
__PSEUDO_TASK_01_WRITE_SPEC( float );
__PSEUDO_TASK_01_WRITE_SPEC( double );
__PSEUDO_TASK_01_WRITE_SPEC( long double );
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  __PSEUDO_TASK_01_WRITE_SPEC( mpfr::mpreal );
#endif
