#include"legendre.h"
#include<iostream>
#include<iomanip>

using namespace std;

template<typename T>
int legendre_demo()
{
	legendre<T> legen;
	legen.run(3, 5);
	legen.print(cout, "legendre");
	return 0;
}

#include"mpreal.h"

using mpfr::mpreal;

int legendre_demo_d()
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(100) );
	legendre_demo<mpfr::mpreal>();
	//legendre_demo<double>();
}
