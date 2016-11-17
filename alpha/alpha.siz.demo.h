#include"alpha.siz.h"
#include<iostream>
#include<iomanip>

using namespace std;

void print_i( int value, const char * name )
{
	cout << setw(8) << name << " : " << setw(4) << value << endl;
}

int demo_asiz()
{
	alpha_siz asiz;
	asiz.put_l_max( 4 );
	asiz.put_lso_max( 4 );
	asiz.put_la_max( 4 );
	asiz.put_lb_max( 4 );

	int a_size = 3, b_size = 4;
	int A_size[a_size] = {10, 5, 1}, B_size[b_size] = {11, 8, 4, 2};
	asiz.put_a_size( a_size );
	asiz.set_A_size( A_size );

	asiz.put_b_size( b_size );
	asiz.set_B_size( B_size );

	for(int i = 0; i < asiz.get_a_size(); ++i)
		cout << "A" << setw(3) << i << " : " << setw(4) << asiz.get_A_size(i) << endl;
	for(int i = 0; i < asiz.get_b_size(); ++i)
		cout << "B" << setw(3) << i << " : " << setw(4) << asiz.get_B_size(i) << endl;
	cout << setw(4) << "lmax" << " :" << endl;
	print_i( asiz.get_l_max(), "l_max" );
	print_i( asiz.get_lso_max(), "lso_max" );
	print_i( asiz.get_la_max(), "la_max" );
	print_i( asiz.get_lb_max(), "lb_max" );
	return 0;
}

int main()
{
	return demo_asiz();
}
