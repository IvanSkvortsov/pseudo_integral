#ifndef __IXS_ANGULAR_IDX_H__
#define __IXS_ANGULAR_IDX_H__

//#define __TO_COMPUTE__
#ifdef  __TO_COMPUTE__
  #define to_compute_set_a_xyz( ax ) to_compute::set_a_xyz( ax )
  #define to_compute_set_b_xyz( bx ) to_compute::set_b_xyz( bx )
  #define to_compute_set_a( la, ax, ay, az ) to_compute::set_a( la, ax, ay, az )
  #define to_compute_set_b( lb, bx, by, bz ) to_compute::set_b( lb, bx, by, bz )
  #define to_compute_value to_compute::value
#else
  #define to_compute_set_a_xyz( ax )
  #define to_compute_set_b_xyz( bx )
  #define to_compute_set_a( la, ax, ay, az )
  #define to_compute_set_b( lb, bx, by, bz )
  #define to_compute_value true
#endif

#define __IXS_ANGULAR_IDX_INTERFACE( method, source )\
static void set_##method( const int value ){ to_compute::source = value; }\
static const int get_##method(){ return to_compute::source; }

struct to_compute
{
	static bool value;
	static int __ax__;// = 1;
	static int __ay__;// = 1;
	static int __az__;// = 0;
	static int __la__;// = __ax__ + __ay__ + __az__;

	static int __bx__;// = 1;
	static int __by__;// = 0;
	static int __bz__;// = 1;
	static int __lb__;// = __bx__ + __by__ + __bz__;

	__IXS_ANGULAR_IDX_INTERFACE( la, __la__ );
	__IXS_ANGULAR_IDX_INTERFACE( ax, __ax__ );
	__IXS_ANGULAR_IDX_INTERFACE( ay, __ay__ );
	__IXS_ANGULAR_IDX_INTERFACE( az, __az__ );
	static void set_a_xyz( const int * ax )
	{
		to_compute::set_la( ax[0] + ax[1] + ax[2] );
		to_compute::set_ax( ax[0] );
		to_compute::set_ay( ax[1] );
		to_compute::set_az( ax[2] );
	}
	static void set_b_xyz( const int * bx )
	{
		to_compute::set_lb( bx[0] + bx[1] + bx[2] );
		to_compute::set_bx( bx[0] );
		to_compute::set_by( bx[1] );
		to_compute::set_bz( bx[2] );
	}
	static void set_la(){ to_compute::__la__ = to_compute::__ax__ + to_compute::__ay__ + to_compute::__az__; }

	__IXS_ANGULAR_IDX_INTERFACE( lb, __lb__ );
	__IXS_ANGULAR_IDX_INTERFACE( bx, __bx__ );
	__IXS_ANGULAR_IDX_INTERFACE( by, __by__ );
	__IXS_ANGULAR_IDX_INTERFACE( bz, __bz__ );
	static void set_lb(){ to_compute::__lb__ = to_compute::__bx__ + to_compute::__by__ + to_compute::__bz__; }

	static void set_a( const int la, const int ax, const int ay, const int az )
	{
		value = 1;
		value = ( value && (la == to_compute::__la__));
		value = ( value && (ax == to_compute::__ax__));
		value = ( value && (ay == to_compute::__ay__));
		value = ( value && (az == to_compute::__az__));
	}
	static void set_b( const int lb, const int bx, const int by, const int bz )
	{
		value = ( value && (lb == to_compute::__lb__));
		value = ( value && (bx == to_compute::__bx__));
		value = ( value && (by == to_compute::__by__));
		value = ( value && (bz == to_compute::__bz__));
	}
};

#endif//__IXS_ANGULAR_IDX_H__
