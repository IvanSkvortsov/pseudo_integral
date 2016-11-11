#include"qu.radial.dat.h"
#include"mpreal.h"
#include<iostream>
#include<iomanip>

using namespace std;

template<typename T, typename U>
int demo_qu_dat()
{
	qu_radial_dat<T,U> rad;
	rad.memory_create("qu.radial.dat.txt");
	return 0;
}

int main()
{
	return demo_qu_dat<double, long double>();
}
