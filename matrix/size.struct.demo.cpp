#include"size.struct.h"
#include<iostream>
using namespace std;

int demo_size()
{
	size_struct<3> sz3;
	size_struct<2> sz2;
	sz3.n() = 4;
	sz3.m() = 3;
	sz3.p() = 7;
	sz3.init_size();
	cout << "sz3.n : " << sz3.n() << endl;
	cout << "sz3.m : " << sz3.m() << endl;
	cout << "sz3.p : " << sz3.p() << endl;
	cout << "sz3.size : " << sz3.size() << endl;
	cout << endl;
	cout << "sz2 = sz3;" << endl;
	sz2 = sz3;
	cout << "sz2.n : " << sz2.n() << endl;
	cout << "sz2.m : " << sz2.m() << endl;
	cout << "sz2.size : " << sz2.size() << endl;
	return 0;
}

int main()
{
	return demo_size();
}
