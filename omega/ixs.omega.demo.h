#include<iostream>
#include<iomanip>
#include<fstream>
#include<cstdlib>
#include<cstring>// memset
#include<cstdio>// snprintf
#include"ixs.omega.h"
#include"mpreal.h"

using namespace std;

std::string itos_dig2(int i)
{
	char i_char[10];
	snprintf(i_char, 10, "%02d", i);
	return i_char;
}

template<typename T, typename U>
int demo_ixs(char * file, char * file_lmax, char * file_angsize, char * file_vecs, char * file_xpn )
{
	ixs_omega<T, U> ixs_omg;
	cout << "----------------" << endl;
	cout << "ixs_omega info 1:" << endl << endl;
	ixs_omg.pub_info();
	cout << endl;

	int l_max, lx_max;
	ifstream inp( file_lmax );
	inp >> l_max >> lx_max;
	inp.close();
	ixs_omg.set_l_max( l_max );
	ixs_omg.set_lx_max( lx_max );

	int node1_size = 0, node2_size = 0;
	const char file_num[] = "angsize.file";
	node1_size = ixs_omg.M_node1_SIZE();
	node2_size = ixs_omg.M_node2_SIZE( file_num );
	ixs_omg.set_node1_size( node1_size );
	if( node2_size == 0 ) ixs_omg.set_node2_size( INTEGRAL_COUNT );
	else ixs_omg.set_node2_size( node2_size );

	ixs_omg.memory_create( file );
	cout << "----------------" << endl;
	cout << "ixs_omega info 1:" << endl << endl;
	cout << "file '" << file << "'" << endl;
	cout << "comp_size : " << ixs_omg.comp_size() << endl;
	ixs_omg.pub_info();

	ixs_omg.node0_init();

	std::vector<std::string> file_vs;
	inp.open( file_vecs );
	int num;
	inp >> num;
	file_vs.resize( num );
	for(int i = 0; i < file_vs.size(); ++i)
		inp >> file_vs[i];
	inp.close();
	cout << "file: vector slm" << endl;
	for(int i = 0; i < file_vs.size(); ++i)
		cout << setw(4) << i << " : '" << file_vs[i] << "'" << endl;

	xyz_pown_integral<U> xpn_int;
	//xpn_int.read( file_xpn );
	//cerr << "xpn've been read" << endl;
	int lmax = (l_max + lx_max) * 2;
	xpn_int.run( lmax, lmax, lmax );
	if( xpn_int.i_max() < lmax || xpn_int.j_max() < lmax || xpn_int.k_max() < lmax )
	{
		std::cerr << "Error: [demo_omega] xyz_pown_integral size" << std::endl;
		std::cerr << "lmax : " << lmax << std::endl;
		std::cerr << "xyz_pown_integral::i_max : " << xpn_int.i_max() << std::endl;
		std::cerr << "xyz_pown_integral::j_max : " << xpn_int.j_max() << std::endl;
		std::cerr << "xyz_pown_integral::k_max : " << xpn_int.k_max() << std::endl;
		exit(1);
	}

	cerr << "calculation of omega starts..." << endl;
	int size = ixs_omg.run_calc( file_vs, file_angsize, xpn_int );
	cout << "ixs_omega::node2_size : " << size << endl;
	cout << "shrink_to_fit" << endl;
	string new_file = file;
	new_file += ".fit";
	ixs_omg.shrink_to_fit(new_file.c_str());
	cout << "----------------" << endl;
	cout << "ixs_omega info 1:" << endl << endl;
	ixs_omg.pub_info();

	int l = 0, lx = 3, i_lmb = 1, i_abc = 2;
	ixs_omg.node0_set_l( l );
	ixs_omg.node0_set_lx( lx );
	ixs_omg.node1_set_lambda_i( i_lmb );
	ixs_omg.node1_set_abc_i( i_abc );
	cout << setw(8) << "l" << " : " << setw(4) << l << endl <<
		setw(8) << "lx" << " : " << setw(4) << lx << endl <<
		setw(8) << "i_lmb" << " : " << setw(4) << i_lmb << endl <<
		setw(8) << "i_abc" << " : " << setw(4) << i_abc << endl;
	typename ixs_omega<T,U>::_value_struct const * p = ixs_omg.node2_data();
	cout << "[" << p << "]" << endl;
	for(int i = 0; i < ixs_omg.node2_size(); ++i, ++p)
		cout << setw(4) << i << " : " << setw(4) << p->m << setw(4) << p->mu <<
			setw(28) << setprecision(18) << scientific << p->d << "  [" << p << "]" << endl;
	cout << endl;
	ixs_omg.node0_set_lmax();
	ixs_omg.node0_set_lx( lx );
	ixs_omg.node1_set_lambda_i( i_lmb );
	ixs_omg.node1_set_abc_i( i_abc );
	p = ixs_omg.node2_data();
	cout << setw(8) << "l" << " : " << setw(3) << "" << "set_lmax" << endl <<
		setw(8) << "lx" << " : " << setw(4) << lx << endl <<
		setw(8) << "i_lmb" << " : " << setw(4) << i_lmb << endl <<
		setw(8) << "i_abc" << " : " << setw(4) << i_abc << endl;
	cout << "[" << p << "]" << endl;
	for(int i = 0; i < ixs_omg.node2_size(); ++i, ++p)
		cout << setw(4) << i << " : " << setw(4) << p->m << setw(4) << p->mu <<
			setw(28) << setprecision(18) << scientific << p->d << "  [" << p << "]" << endl;
	cout << endl;

	return 0;
}


template<typename T>
int open_ixs(char * file)
{
	ixs_omega<T, T> ixs;
	ixs.memory_open( file );
	cout << "ixs_omega" << endl;
	ixs.pub_info();
	int iter = 321;

	return 0;
}

int demo_ixs_d(char * file, char * file_lmax, char * file_angsize, char * file_vecs, char * file_xpn)
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(100) );
	//return demo_ixs<long double, mpfr::mpreal>( file, file_lmax, file_angsize, file_vecs, file_xpn );
	return demo_ixs<double, mpfr::mpreal>( file, file_lmax, file_angsize, file_vecs, file_xpn );
}

int main(int argc, char ** argv)
{
	if( argc != 6 )
	{
		cerr << "Error: [main] usage './main.exe file.map file.lmax file.angsize file.vecs file.xpn'" << endl;
		cerr << "need 6 argumets" << endl;
		cerr << "1 : memory mapping filename" << endl;
		cerr << "2 : maximum angular momentum filename" << endl;
		cerr << "3 : integral size filename" << endl;
		cerr << "4 : products of spherical harmonics multiplication filename" << endl;
		cerr << "5 : values of xyz_pown_integral filename" << endl;
		return 1;
	}
	return demo_ixs_d( argv[1], argv[2], argv[3], argv[4], argv[5] );
}
