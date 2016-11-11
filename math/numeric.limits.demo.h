#include"numeric.limits.h"
#include<iostream>

using namespace std;

template<typename T>
int numlim_demo()
{
	string s = "numeric_limits<T>::";
	cout << s << "epsilon : " << math::numeric_limits<T>::epsilon << endl;
	cout << s << "max     : " << math::numeric_limits<T>::max << endl;
	cout << s << "min     : " << math::numeric_limits<T>::min << endl;
	return 0;
}

int numlim_demo_d()
{
	return numlim_demo<double>();
}
