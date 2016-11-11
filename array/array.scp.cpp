#include"array.scp.hpp"

template struct array_scp<float>;
template struct array_scp<double>;
template struct array_scp<long double>;

#include"mpreal.use.h"
#ifdef  __MPREAL_USE
  #include"mpreal.h"
  template struct array_scp<mpfr::mpreal>;
#endif

#include"poly.n.h"
#include"spherical.h"

template struct array_scp<poly_n<float, 3> >;
template struct array_scp<poly_n<float, 4> >;
template struct array_scp<poly_n<double, 3> >;
template struct array_scp<poly_n<double, 4> >;
template struct array_scp<poly_n<long double, 3> >;
template struct array_scp<poly_n<long double, 4> >;
//#ifndef __MPREAL_USE
//#error WTF! where is __MPREAL_USE array.scp.cpp
//#endif
#ifdef  __MPREAL_USE
template struct array_scp<poly_n<mpfr::mpreal, 3> >;
template struct array_scp<poly_n<mpfr::mpreal, 4> >;

template struct array_scp<spherical<mpfr::mpreal> >;
#endif

template struct array_scp<spherical<float> >;
template struct array_scp<spherical<double> >;
template struct array_scp<spherical<long double> >;
