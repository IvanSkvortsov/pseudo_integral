#include"numeric.constants.h"
#include<iostream>

using namespace std;

template<typename T>
int numlim_demo()
{
	string s = "numeric_constants<T>::";
	cout << s << "sqrt_pi : " << math::numeric_constants<T>::sqrt_pi << endl;
	cout << s << "pi      : " << math::numeric_constants<T>::pi << endl;
	cout << s << "e       : " << math::numeric_constants<T>::e << endl;
	return 0;
}

int numlim_demo_d()
{
	return numlim_demo<double>();
}
