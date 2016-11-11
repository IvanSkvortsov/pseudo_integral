#ifndef __XYZ_POWN_INTEGRAL_H__
#define __XYZ_POWN_INTEGRAL_H__
#include<cstddef>// size_t
#include<vector>

#define __ERROR_xyz_pown_integral__
#define __INFO_xyz_pown_integral__
//#define __LOG_xyz_pown_integral__

#if defined (__INFO_xyz_pown_integral__) || defined (__LOG_xyz_pown_integral__)
#include<string>
#endif

// xyz_pown_integral
//
// calculate integrals like this
// T xyz_pown_integral<T>::operator()(i, j, k) := int( x^i * y^j * z^k, theta=0..Pi, phi=0..2*Pi);
// integral(i,j,k) value := 4*Pi*(i-1)!!*(j-1)!!*(k-1)!!/(i+j+k+1)!!
// P.S. warning, one should multiply the result of xyz_pown_integral<T>::operator(int , int, int ) by (4*Pi) in order to get mentioned integral value

template<typename T>
class xyz_pown_integral
{
	typedef T value_type;
	typedef std::size_t size_type;
protected:
	size_type _i_max, _j_max, _k_max;
	size_type _half_imax__p1, _half_jmax__p1, _half_kmax__p1;
	std::vector<T> v;
public:
	typedef T float_type;

	xyz_pown_integral();
	~xyz_pown_integral();
	size_type const & i_max()const;
	size_type const & j_max()const;
	size_type const & k_max()const;
	size_type & i_max();
	size_type & j_max();
	size_type & k_max();
	size_type i_max(size_type const & __i_max);
	size_type j_max(size_type const & __j_max);
	size_type k_max(size_type const & __k_max);
	void half_max__p1();
	size_type const max_size()const;
	size_type const size()const;
	T * begin();
	T * end();
	T const * begin()const;
	T const * end()const;
	void set_zero();
	void run(size_type const & __i_max, size_type const & __j_max, size_type const & __k_max);
	void run();
	T const & operator()(size_type i, size_type j, size_type k)const;
	void pub_info()const;

	void write(const char * file, const int prec = 16)const;// write into file
	void read(const char file[] = "xyz.pown.integral.src" );// read from file
private:
	xyz_pown_integral(xyz_pown_integral<T> & );
	xyz_pown_integral(xyz_pown_integral<T> const & );
	xyz_pown_integral<T> & operator=(xyz_pown_integral<T> & );
	xyz_pown_integral<T> & operator=(xyz_pown_integral<T> const & );
#ifdef  __INFO_xyz_pown_integral__
	void info()const;
#endif
#ifdef  __LOG_xyz_pown_integral__
	void log(std::string const & s)const;
#endif
};

#endif//__XYZ_POWN_INTEGRAL_H__
