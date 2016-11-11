#include"memory.map.h"
#include"memorystream.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include"matrix.cursor.h"
#include<vector>

using namespace std;

template<typename T>
struct myStruct1
{
	int __size;
	T __data;
};
template<typename T>
struct myStruct2
{
	T __data;
	int __size;
};

inline int is_2pown(int z)
{
	return !((z-1)&z);
}
inline int __logn(int z, int __n)
{
	return z%__n;
}
template<typename T>
int demo_mxcrs()
{
	char const file[] = "matrix.cursor.mmap";
	const int bytes = 1024 * 1024;// 1 Mb
	memory_map map;
	map.create( file, bytes );
	cout << "map.file : " << file << endl;
	cout << "map.data : [" << map.data() << "]" << endl;
	cout << "map.size : " << map.size() << endl;
	cout << endl;

	memorystream ms;
	ms.setbuf( map.data(), map.size() );

	matrix_cursor_3<T> mx;
	int * _mx3_size = &mx.n();
	for(int i = 0; i < mx.dimension(); ++i)
		_mx3_size[i] = (i + 1) * (i + 2);
	mx.init_size();
	cout << "matrix        : [" << &mx << "]" << endl;
	cout << "matrix.size   : " << setw(8) << mx.size() << endl;
	for(int i = 0; i < mx.dimension(); ++i)
		cout << "matrix.size " << i << " : " << setw(8) << mx.size(i) << endl;
	cout << "matrix.data   : [" << mx.data() << "]" << endl;
	cout << endl;

	const int mx_pos = 32;
	ms.seek( mx_pos );
	ms.write( &mx, sizeof(mx) );
	int sz_type = sizeof(T);
	ms.write( &sz_type, 4 );
	int cur_pos = ms.tell();

	union{void * _void; matrix_cursor_3<T> * _matrix_3; matrix_cursor_2<T> * _matrix_2; matrix_cursor_1<T> * _matrix_1; char * _char;} _ptr_mx;
	ms.seek( mx_pos );
	_ptr_mx._void = ms.getcur();

	matrix_cursor_3<T> & mx_s = *_ptr_mx._matrix_3;
	cout << "matrix        : [" << &mx_s << "]" << endl;
	cout << "matrix.size   : " << setw(8) << mx_s.size() << " [" << &mx_s.size() << "]" << endl;
	for(int i = 0; i < mx_s.dimension(); ++i)
		cout << "matrix.size " << i << " : " << setw(8) << mx_s.size(i) << " [" << &mx_s.size(i) << "]" << endl;
	cout << "matrix.data   : [" << mx_s.data() << "]" << endl;
	cout << "sizeof        : " <<
		setw(8) << sizeof(typename matrix_cursor_3<T>::_size_class) <<
		setw(8) << sizeof(typename matrix_cursor_3<T>::_data_class) <<
		setw(8) << sizeof(matrix_cursor_3<T>) << endl;
	cout << endl;
	int n = 1, m = 3, k = 7;
	mx_s(n, m, k) = 15.329718192;
	cout << setw(3) << n << setw(3) << m << setw(3) << k << " : " << setw(20) << setprecision(12) << scientific << mx_s(n, m, k) << endl;

	matrix_cursor_2<T> & mx_s2 = *_ptr_mx._matrix_2;
	mx_s2.size(0) = 4;
	mx_s2.size(1) = 6;
	mx_s2.init_size();
	cout << "matrix        : [" << &mx_s2 << "]" << endl;
	cout << "matrix.size   : " << setw(8) << mx_s2.size() << " [" << &mx_s2.size() << "]" << endl;
	for(int i = 0; i < mx_s2.dimension(); ++i)
		cout << "matrix.size " << i << " : " << setw(8) << mx_s2.size(i) << " [" << &mx_s2.size(i) << "]" << endl;
	cout << "matrix.data   : [" << mx_s2.data() << "]" << endl;
	cout << "sizeof        : " <<
		setw(8) << sizeof(typename matrix_cursor_2<T>::_size_class) <<
		setw(8) << sizeof(typename matrix_cursor_2<T>::_data_class) <<
		setw(8) << sizeof(matrix_cursor_2<T>) << endl;
	cout << endl;
	mx_s2(n, m) = 921.282983729;
	cout << setw(3) << n << setw(3) << m << " : " << setw(20) << setprecision(12) << scientific << mx_s2(n, m) << endl;

	ms.seek( cur_pos );
	_ptr_mx._void = ms.getcur();
	matrix_cursor_1<T> & mx_s1 = *_ptr_mx._matrix_1;
	mx_s1.size() = 17;

	cout << endl;
	cout << "matrix        : [" << &mx_s1 << "]" << endl;
	cout << "matrix.size   : " << setw(8) << mx_s1.size() << " [" << &mx_s1.size() << "]" << endl;
	cout << "matrix.data   : [" << mx_s1.data() << "]" << endl;
	cout << "sizeof        : " <<
		setw(8) << sizeof(typename matrix_cursor_1<T>::_size_class) <<
		setw(8) << sizeof(typename matrix_cursor_1<T>::_data_class) <<
		setw(8) << sizeof(matrix_cursor_1<T>) << endl;
	cout << endl;
	mx_s1[k] = 2.238927;
	cout << endl;
	cout << setw(3) << k << " : " << setw(20) << setprecision(12) << scientific << mx_s1[k] << endl;

	cout << endl;
	cout << "myStruct 1 : " << setw(4) << sizeof(myStruct1<T>) << endl;
	cout << "myStruct 2 : " << setw(4) << sizeof(myStruct2<T>) << endl;
	cout << endl;

	for(int i = 0; i < 10; ++i)
		cout << setw(4) << i << " - " << setw(4) << (is_2pown(i) ? "pow(2,n)" : "not pow(2,n)" ) << endl;
	cout << endl;
	union{void * _void; char * _char; int z;} ptr;
	ptr._void = &ptr;
	cout << "pointer [" << &ptr << "] " << setw(10) << ptr.z << setw(4) << ptr.z%sizeof(ptr) << setw(4) << sizeof(ptr) << endl;
	T tval1;
	int zval1;
	T tval2;
	char cval1;

	cout << "T tval1;" << endl;
	cout << "int zval1;" << endl;
	cout << "T tval2;" << endl;
	cout << "char cval1;" << endl << endl;

	ptr._void = &tval1;
	cout << "tval1   [" << ptr._void << "] " << setw(10) << ptr.z << setw(4) << ptr.z%sizeof(tval1) << setw(4) << sizeof(T) << endl;
	ptr._void = &zval1;
	cout << "zval1   [" << ptr._void << "] " << setw(10) << ptr.z << setw(4) << ptr.z%sizeof(zval1) << setw(4) << sizeof(int) <<
		setw(4) << ptr.z%sizeof(T) << endl;
	ptr._void = &tval2;
	cout << "tval2   [" << ptr._void << "] " << setw(10) << ptr.z << setw(4) << ptr.z%sizeof(tval2) << setw(4) << sizeof(T) << endl;
	ptr._void = &cval1;
	cout << "cval1   [" << ptr._void << "] " << setw(10) << ptr.z << setw(4) << ptr.z%sizeof(cval1) << setw(4) << sizeof(char) << endl;

	/*
	vector<T> vec;
	matrix_cursor_3<T> mx3t;
	mx3t.n() = 15;
	mx3t.m() = 5;
	mx3t.p() = 3;
	mx3t.init_size();
	vec.resize( mx3t.size() );
	*/
	return 0;
}

int demo_mxcrs_d()
{
	return demo_mxcrs<double>();
}

int main()
{
	return demo_mxcrs_d();
}
