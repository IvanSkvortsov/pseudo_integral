#ifndef __PRINT_HYPERG_H__
#define __PRINT_HYPERG_H__
#include<iostream>
#include<iomanip>

#define __PRINT_EXPONENT_VALUE
#ifdef  __PRINT_EXPONENT_VALUE
  #include<cmath>// exp
#endif

#define __PRINT_FLT_PREC 18

inline void print_title( std::ostream & out, std::string title, int t_sz );
template<typename T> inline static void print_consts( std::ostream & out, int ws, int w,
		int const & MAXITER, T const & SER_EPSILON, T const & SER_MAX );
template<typename T> inline static void print_consts( std::ostream & out, int ws, int w,
		int const & MAXITER, T const & SER_EPSILON, T const & SER_MAX, T const & SER_MIN );
template<typename T> inline static void print_args( std::ostream & out, int ws, int w, T const & a, T const & b, T const & x );
template<typename T> inline static void print_args( std::ostream & out, int ws, int w,
		T const & a, T const & b1, T const & b2, T const & x1, T const & x2 );
template<typename T> inline static void print_func( std::ostream & out, int ws, int w, const char * fname, T const & value, const int ex );
template<typename T> inline static void print_func( std::ostream & out, int ws, int w,
		const char * fname1, const char * fname2, T const & value1, T const & value2, const int ex );
template<typename T> inline static void print_err( std::ostream & out, int ws, int w, const char * name, T const & value );
template<typename T> inline static void print_err( std::ostream & out, int ws, int w,
		const char * name1, const char * name2, T const & value1, T const & value2 );
inline static void print_iter( std::ostream & out, int ws, int w, const int iter );
inline static void print_type( std::ostream & out, int ws, int w, const int _type );

template<typename T>
inline void print_hyperg_series1( std::ostream & out, std::string const & name, const char * func, const char * args,
		T const & a, T const & b, T const & x, 
		T const & value, const int ex, T const & err,
		const int iter,
		T const & SER_MAX, T const & SER_EPSILON, int const & MAXITER )
{
	const int prec = __PRINT_FLT_PREC, w = prec + 10, we = 10, ws = 20;
	const int t_sz = ws + w;
	std::string fname = std::string( func ) + args;

	out.precision( prec );

	print_title( out, name, t_sz );
	print_iter( out, ws, w, iter );
	print_func( out, ws, w, fname.c_str(), value, ex );
#ifdef  __PRINT_EXPONENT_VALUE
	print_func( out, ws, w, "exp( x )", exp( x ), 0 );
#endif
	print_args( out, ws, w, a, b, x );
	print_consts( out, ws, w, MAXITER, SER_EPSILON, SER_MAX );
	print_err( out, ws, w, "err", err );

	out << std::endl;
}

template<typename T>
inline void print_hyperg_series2( std::ostream & out, std::string const & name, const char * func, const char * args1, const char * args2,
		T const & a, T const & b, T const & x, 
		T const & value1, T const & value2, const int ex, T const & err1, T const & err2,
		const int iter,
		T const & SER_MAX, T const & SER_EPSILON, int const & MAXITER )
{
	const int prec = __PRINT_FLT_PREC, w = prec + 10, we = 10, ws = 20;
	const int t_sz = ws + w;
	std::string fname1 = std::string( func ) + args1;
	std::string fname2 = std::string( func ) + args2;

	out.precision( prec );

	print_title( out, name, t_sz);
	print_iter( out, ws, w, iter );
	print_func( out, ws, w, fname1.c_str(), fname2.c_str(), value1, value2, ex );
#ifdef  __PRINT_EXPONENT_VALUE
	print_func( out, ws, w, "exp( x )", exp( x ), 0 );
#endif
	print_args( out, ws, w, a, b, x );
	print_consts( out, ws, w, MAXITER, SER_EPSILON, SER_MAX );
	print_err( out, ws, w, "error1", "error2", err1, err2 );

	out << std::endl;
}

template<typename T>
inline void print_hyperg_1f11_series( std::ostream & out, std::string const & name, const char * func, const char * args1, const char * args2,
		T const & a, T const & b1, T const & b2, T const & x1, T const & x2,
		T const & value1, T const & value2, const int ex,
		int const & MAXITER, T const & SER_EPSILON, T const & SER_MAX, T const & SER_MIN, const int hyperg_1f11_n )
{
	const int prec = __PRINT_FLT_PREC, w = prec + 10, we = 10, ws = 20;
	const int t_sz = ws + w;
	std::string fname1 = std::string( func ) + args1;
	std::string fname2 = std::string( func ) + args2;

	out.precision( prec );

	print_title( out, name, t_sz);
	print_type( out, ws, w, hyperg_1f11_n );
	print_func( out, ws, w, fname1.c_str(), fname2.c_str(), value1, value2, ex );
#ifdef  __PRINT_EXPONENT_VALUE
	const T x = ( sqrt( x1 ) + sqrt( x2 ) ) * ( sqrt( x1 ) + sqrt( x2 ) );
	print_func( out, ws, w, "exp( x )", exp( x ), 0 );
	print_func( out, ws, w, "x", "( sqrt(x1) + sqrt(x2) )^2", 0 );
#endif
	print_args( out, ws, w, a, b1, b2, x1, x2 );
	print_consts( out, ws, w, MAXITER, SER_EPSILON, SER_MAX, SER_MIN );

	out << std::endl;
}

inline void print_hyperg_1f11_title( std::ostream & out )
{
	const int w = __PRINT_FLT_PREC + 10;
	for(int k = 0; k < 4*w + 8; ++k)
		out << '-';
	out << std::endl;
	out << "--> Map1 = M(a+1+n,b2,x2), del = x1^n*a_n/(b1_n*n!), el_v = Map1*u, value = sum_{i=0}^{n}el_v_{i} <--" << std::endl;
	for(int k = 0; k < 4*w + 8; ++k)
		out << '-';
	out << std::endl;
	out <<
		std::setw( w ) << "Map1" <<
		std::setw( w ) << "del" <<
		std::setw( w ) << "el_v" <<
		std::setw( w ) << "value" <<
		std::setw( 8 ) << "n" <<
		std::endl << std::endl;
}

template<typename T> inline void print_hyperg_1f11_series_idx( std::ostream & out,
		T const & Map1, T const & del, T const & el_v, T const & val, const int ex, T const & err_v, const int n )
{
	const int prec = __PRINT_FLT_PREC, w = prec + 10;
	if( n % 50 == 1 || (n < 100 && n%2 == 1) || (err_v < 1.0e-15 && n%2 == 0) )
	{
	out <<
		std::setw( w ) << std::setprecision( prec ) << Map1 <<
		std::setw( w ) << std::setprecision( prec ) << del <<
		std::setw( w ) << std::setprecision( prec ) << el_v <<
		std::setw( w ) << std::setprecision( prec ) << val <<
		std::setw( 8 ) << n <<
		std::setw( 10) << std::setprecision( 2 ) << err_v <<
		std::setw( 8 ) << ex <<
		std::endl;
	}
}

inline void print_title( std::ostream & out, std::string title, int t_sz )
{
	for(int i = 0; i < t_sz; ++i)
		out << '-';
	out << std::endl;
	out << std::setw( (t_sz + title.size())/2 ) << title << std::endl;
	for(int i = 0; i < t_sz; ++i)
		out << '-';
	out << std::endl;
}

template<typename T>
inline static void print_consts( std::ostream & out, int ws, int w, int const & MAXITER, T const & SER_EPSILON, T const & SER_MAX )
{
	out << std::scientific <<
		std::setw( ws ) << "EPSILON : "  << std::setw( w ) << SER_EPSILON << std::endl <<
		std::setw( ws ) << "MAXITER : "  << std::setw( w ) << MAXITER << std::endl <<
		std::setw( ws ) << "MAXVAL : "   << std::setw( w ) << SER_MAX << std::endl;
}

template<typename T>
inline static void print_consts( std::ostream & out, int ws, int w, int const & MAXITER, T const & SER_EPSILON, T const & SER_MAX, T const & SER_MIN )
{
	print_consts( out, ws, w, MAXITER, SER_EPSILON, SER_MAX );
	out << std::scientific <<
		std::setw( ws ) << "MINVAL : "   << std::setw( w ) << SER_MIN << std::endl;
}

template<typename T>
inline static void print_args( std::ostream & out, int ws, int w, T const & a, T const & b, T const & x )
{
	out << std::scientific <<
		std::setw( ws ) << "a : " << std::setw(w) << a << std::endl <<
		std::setw( ws ) << "b : " << std::setw(w) << b << std::endl <<
		std::setw( ws ) << "x : " << std::setw(w) << x << std::endl;
}

template<typename T>
inline static void print_args( std::ostream & out, int ws, int w, T const & a, T const & b1, T const & b2, T const & x1, T const & x2 )
{
	out << std::scientific <<
		std::setw( ws ) << "a : " << std::setw(w) << a << std::endl <<
		std::setw( ws ) << "b1 : " << std::setw(w) << b1 << std::endl <<
		std::setw( ws ) << "b2 : " << std::setw(w) << b2 << std::endl <<
		std::setw( ws ) << "x1 : " << std::setw(w) << x1 << std::endl <<
		std::setw( ws ) << "x2 : " << std::setw(w) << x2 << std::endl;
}

template<typename T> inline static void print_func( std::ostream & out, int ws, int w, const char * fname, T const & value, const int ex )
{
	out << std::scientific <<
		std::setw( ws-3 ) << fname << " : " << std::setw(w) << value << std::setw(10) << ex << std::endl;
}
template<typename T> inline static void print_func( std::ostream & out, int ws, int w,
		const char * fname1, const char * fname2, T const & value1, T const & value2, const int ex )
{
	print_func( out, ws, w, fname1, value1, ex );
	print_func( out, ws, w, fname2, value2, ex );
}
template<typename T> inline static void print_err( std::ostream & out, int ws, int w, const char * name, T const & value )
{
	out << std::scientific <<
		std::setw( ws-3 ) << name << " : "  << std::setw(w) << std::setprecision(3) << value << std::endl;
	out.precision( w - 10 );
}
template<typename T> inline static void print_err( std::ostream & out, int ws, int w,
		const char * name1, const char * name2, T const & value1, T const & value2 )
{
	print_err( out, ws, w, name1, value1 );
	print_err( out, ws, w, name2, value2 );
}
inline static void print_iter( std::ostream & out, int ws, int w, const int iter )
{
	out << std::setw( ws ) << "iter : " << std::setw( w ) << iter << std::endl;
}
inline static void print_type( std::ostream & out, int ws, int w, const int _type )
{
	out << std::setw( ws ) << "hyperg_t : " << std::setw(w) << sf_hyperg::type( _type ) << std::endl;
}
#endif//__PRINT_HYPERG_H__
