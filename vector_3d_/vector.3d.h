#ifndef __VECTOR_3D_H__
#define __VECTOR_3D_H__

#define __VECTOR_3D__INLINE
//#define __VECTOR_3D__INLINE inline

//#define __VECTOR_3D_LOG

template<typename T>
struct vector_3d
{
public:
	typedef T value_type;
	typedef T * pointer;
	typedef T const * const_pointer;
	typedef T & reference;
	typedef T const & const_reference;
	typedef T float_type;
protected:
	T _data[3];
	template<typename U>
	void log(const char * _method)const;
	void log(const char * _method)const;
public:
	vector_3d();
	~vector_3d();
	template<typename U>
	vector_3d( U const & _x, U const & _y, U const & _z );
	vector_3d( T const & _x, T const & _y, T const & _z );
	template<typename U>
	vector_3d( U const * v );
	vector_3d( T const * v );
	template<typename U>
	vector_3d( vector_3d<U> const & v );
	vector_3d( vector_3d<T> const & v );

	template<typename U>
	vector_3d<T> & operator=( vector_3d<U> const & v );
	vector_3d<T> & operator=( vector_3d<T> const & v );
	template<typename U>
	vector_3d<T> & operator=( U const * v );
	vector_3d<T> & operator=( T const * v );

	operator T*();
	operator T const *()const;
	const_pointer data()const;
	pointer data();
	const_reference operator[](int i)const;
	reference operator[](int i);
	const_reference x()const;
	const_reference y()const;
	const_reference z()const;
	reference x();
	reference y();
	reference z();

	value_type sqr()const;
	void sqr(reference __sqr)const;

	value_type len()const;
	void len(reference __len)const;

	vector_3d<T> & neg();
	void neg( vector_3d<T> & v)const;

	vector_3d<T> & norm();
	void norm( vector_3d<T> & v )const;

	template<typename U>
	vector_3d<T> & operator+=( vector_3d<U> const & v );
	vector_3d<T> & operator+=( vector_3d<T> const & v );
	template<typename U>
	vector_3d<T> & operator-=( vector_3d<U> const & v );
	vector_3d<T> & operator-=( vector_3d<T> const & v );
	template<typename U>
	vector_3d<T> & operator*=( U const & v );
	vector_3d<T> & operator*=( T const & v );
	template<typename U>
	vector_3d<T> & operator/=( U const & v );
	vector_3d<T> & operator/=( T const & v );
	vector_3d<T> operator-()const;

	static value_type scalar(vector_3d<T> const & lhs, vector_3d<T> const & rhs);
	static void scalar(T & __result, vector_3d<T> const & lhs, vector_3d<T> const & rhs);
	value_type scalar( vector_3d<T> const & v)const;
	void scalar( T & _result, vector_3d<T> const & v)const;
};

template<typename T, typename U> vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<U> const & rhs);
template<typename T> vector_3d<T> operator+( vector_3d<T> const & lhs, vector_3d<T> const & rhs);

template<typename T, typename U> vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<U> const & rhs);
template<typename T> vector_3d<T> operator-( vector_3d<T> const & lhs, vector_3d<T> const & rhs);

template<typename T> vector_3d<T> operator*( vector_3d<T> const & lhs, T const & rhs);
template<typename T, typename U> vector_3d<T> operator*( vector_3d<T> const & lhs, U const & rhs);
template<typename T> vector_3d<T> operator/( vector_3d<T> const & lhs, T const & rhs);
template<typename T, typename U> vector_3d<T> operator/( vector_3d<T> const & lhs, U const & rhs);

#endif//__VECTOR_3D_H__
