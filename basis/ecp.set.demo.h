#include"ecp.set.h"
#include<iostream>
#include<iomanip>

using namespace std;

template<typename T>
int demo_ecp( const char * file )
{
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_ecp] can't open file '" << file << "'" << endl;
		return 1;
	}
	ecp_set<T> ecp;
	ecp.read( inp );
	ecp.pub_info();
	return 0;
}

template<typename T>
int demo1_ecp()
{
	ecp_set<T> ecp;
	ecp.set_l_max( 5 );
	ecp.set_lso_max( 4 );
	ecp.resize();
	typename ecp_set<T>::pointer p_pl = ecp.data();
	typename ecp_set<T>::value_type::pointer p_pf;
	int __size = 0;
	for(int l = 0; l <= ecp.get_l_max(); ++l, ++p_pl)
	{
		p_pl->resize( 1 );
		p_pf = p_pl->data();
		for(int i = 0; i < p_pl->size(); ++i, ++p_pf )
		{
			p_pf->set_fun_size( 4 + l%2 );
			__size += p_pf->get_fun_size();
		}
	}
	ecp.set_size( __size );
	int __pos = 0;
	p_pl = ecp.data();
	for(int l = 0; l <= ecp.get_l_max(); ++l, ++p_pl)
	{
		p_pf = p_pl->data();
		for(int i = 0; i < p_pl->size(); ++i, ++p_pf )
		{
			p_pf->set_fun_data( ecp.get_fun_data() + __pos );
			p_pf->set_alp_data( ecp.get_alp_data() + __pos );
			p_pf->set_n_data( ecp.get_n_data() + __pos );
			__pos += p_pf->get_fun_size();
		}
	}
	p_pl = ecp.data();
	cout << setw(4) << ecp.get_l_max() << setw(4) << ecp.get_lso_max() << endl;
	for(int l = 0; l <= ecp.get_l_max(); ++l, ++p_pl)
	{
		p_pf = p_pl->data();
		cout << setw(8) << l << setw(4) << p_pl->size() << endl;
		for(int i = 0; i < p_pl->size(); ++i, ++p_pf )
		{
			cout << setw(12) << p_pf->get_fun_size() << endl;
		}
	}
	return 0;
}

int main(int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_ecp<double>(argv[1]);
}
