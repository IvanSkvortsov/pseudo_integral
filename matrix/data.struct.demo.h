#include"data.struct.h"
#include<iostream>
#include<vector>
#include<iomanip>
#include"/home/Ivan/cpp/projects/bitprint/bitprint.hpp"
#include"/home/Ivan/cpp/projects/bitprint/smart.bits.hpp"

using namespace std;

typedef unsigned long long int ull_t;
int demo_databool()
{
	vector<int> pool( 1024 );
	data_array<bool> * d;
	union{ void * _void; data_array<bool> * _data_bool;} _converter;
	_converter._void = pool.data();
	d = _converter._data_bool;
	const int size = 43;
	for(int i = 0; i < size; ++i)
		d->operator[](i) = i%2;
	cout << "[" << d << "] : ";
	for(int i = 0; i < size; ++i)
		cout << d->operator[](i);
	cout << endl;

	for(int i = 0; i < size; ++i)
		d->operator[](i) = (i+1)%3;
	cout << "[" << d << "] : ";
	for(int i = 0; i < size; ++i)
		cout << d->operator[](i);
	cout << endl;
	ull_t * p_ull;
	p_ull = (ull_t *)pool.data();
	cout << "ull : " << smart_bits( *p_ull, 32 ) << endl;
	cout << "szof vector<bool>::value_type : " << setw(4) << sizeof(vector<bool>::value_type) << endl;
	cout << "szof vector<bool>::size_type : " << setw(4) << sizeof(vector<bool>::size_type) << endl;
	cout << "szof data_array<bool>::value_type : " << setw(4) << sizeof(data_array<bool>::value_type) << endl;
	return 0;
}

int main()
{
	return demo_databool();
}
