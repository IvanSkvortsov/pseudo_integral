#include"ixs.angular.idx.h"

#define __IXS_ANGULAR_IDX_INIT_A( _x_, _y_, _z_ )\
int to_compute::__ax__ = (_x_);\
int to_compute::__ay__ = (_y_);\
int to_compute::__az__ = (_z_);\
int to_compute::__la__ = to_compute::__ax__ + to_compute::__ay__ + to_compute::__az__;

#define __IXS_ANGULAR_IDX_INIT_B( _x_, _y_, _z_ )\
int to_compute::__bx__ = (_x_);\
int to_compute::__by__ = (_y_);\
int to_compute::__bz__ = (_z_);\
int to_compute::__lb__ = to_compute::__bx__ + to_compute::__by__ + to_compute::__bz__;

bool to_compute::value = 1;
__IXS_ANGULAR_IDX_INIT_A( 1, 1, 0 );
__IXS_ANGULAR_IDX_INIT_B( 1, 0, 1 );
