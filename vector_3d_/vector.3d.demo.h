#include"vector.3d.h"
//#include"vector.3d.hpp"
#include<iostream>
#include<iomanip>
#include"mpreal.h"
using namespace std;

template<typename T>
void print_v3d( vector_3d<T> const & v, const char * _name)
{
	cout << "----" << endl;
	cout << setw(4) << _name << endl;
	for(int i = 0; i < 3; ++i)
		cout << setw(4) << i << " : " << setw(14) << v[i] << endl;
	cout << setw(4) << "sqr" << " : " << setw(14) << v.sqr() << endl;
	cout << setw(4) << "len" << " : " << setw(14) << v.len() << endl;
}

template<typename T>
int demo_vec3d()
{
	vector_3d<T> v1, v2, v3;
	v1.x() = 2;
	v1.y() = 3;
	v1.z() = 7;
	print_v3d( v1, "v1");

	cout << "norm" << endl;
	v2 = v1;
	v1.norm();
	cout << setw(4) << "sqr" << " : " << setw(14) << v1.sqr() << endl;
	cout << setw(4) << "len" << " : " << setw(14) << v1.len() << endl;
	v2.z() *= 1.25;
	print_v3d( v1, "v1" );

	print_v3d( v2, "v2" );
	cout << "scalar" << endl;
	cout << setw(4) << "" << " : " << setw(14) << v2.scalar( v1 ) << endl;

	v3 = v1;
	v3 += v2;
	print_v3d( v3, "v3" );

	vector_3d<T> v4( 1, 5, 8);
	print_v3d( v4, "v4" );

	vector_3d<T> v5 = {1, 3, 9};
	print_v3d( v5, "v5" );
	v5 = vector_3d<int>( 3, 4, 7 );// call operator=( vector_3d<int> const & )
	//v5 = {3, 4, 7};// call operator=( vector_3d<T> const & )
	print_v3d( v5, "v5" );

	vector_3d<int> v6 = {5, 9, 11};
	print_v3d( v6, "v6" );
	vector_3d<T> v7( v6 );
	print_v3d( v7, "v7" );
	return 0;
}

struct Foo
{
	int n;
	void log(const char * _method)const
	{
		std::clog << "LOG : [" << this << "] Foo::" << _method << std::endl;
	}
	Foo() : n(0){this->log("Foo()");}
	Foo(const int __n) : n(__n){this->log("Foo(const int )");}
	Foo(Foo const & v) : n(v.n){this->log("Foo(Foo const & )");}
	~Foo(){this->n = 0; this->log("~Foo()");}
	operator int()const{return this->n;}
	Foo & operator=(Foo const & v)
	{
		this->log("operator=(Foo const & )");
		if( this == &v )
			return *this;
		this->n = v;
		return *this;
	}
	Foo & operator=(int const & v)
	{
		this->log("operator=(int const & )");
		this->n = v;
		return *this;
	}
};
template<typename T>
int demo_vec3d_2()
{
	const int size = 3;
	T r1[size] = {1, 2, 4};
	for(int i = 0; i < size; ++i)
		cout << setw(4) << i << " : " << setw(5) << r1[i] << "  [" << r1+i << "]" << endl;
	T r2[size];
	for(int i = 0; i < size; ++i)
	{
		r2[i] = 2*i + 1;
		cout << setw(4) << i << " : " << setw(5) << r2[i] << "  [" << r2+i << "]" << endl;
	}
	return 0;
}

int main()
{
	return demo_vec3d<mpfr::mpreal>();
	//return demo_vec3d<double>();
	//return demo_vec3d_2<Foo>();
}
