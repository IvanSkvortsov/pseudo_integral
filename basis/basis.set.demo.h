#include"basis.set.h"
#include<iostream>
#include<iomanip>

#define NDEBUG

#define __BASIS_DEMO_DEBUG
#ifdef  __BASIS_DEMO_DEBUG
  #include<cassert>
#endif
using namespace std;

template<typename T>
int demo_basis( char const * file )
{
	basis<T> bas;
	bas.read( file );
	for(int i = 0; i < bas.size(); ++i)
		bas[i].pub_info();
	return 0;
}
template<typename T>
int demo1_basis()
{
	basis_set<T> bs;
	bs.set_lx_max( 4 );

	basis_lx<T> * p_bl;
	basis_fun<T> * p_bf;
	p_bl = bs.data();
	int __alp_size = 0;
	int __fun_size = 0, __fun_size_i = 0;
	for(int lx = 0; lx < bs.size(); ++lx, ++p_bl)// basis lx
	{
		p_bl->set_alp_size( 12-2*lx );// number of alphas for fixed lx
		p_bl->resize( 8-lx );// number of functions for fixed lx
		__alp_size += p_bl->get_alp_size();
#ifdef  __BASIS_DEMO_DEBUG
		assert( __alp_size <= bs.get_alp_size() );
#endif
		p_bf = p_bl->data();
		__fun_size_i = 0;
		for(int j = 0; j < p_bl->size(); ++j, ++p_bf)// basis fun
		{
			p_bf->set_fun_size( 8 - j%4 );
			p_bf->set_alp_pos( p_bl->get_alp_size() - p_bf->get_fun_size()  );
			__fun_size_i += p_bf->get_fun_size();
#ifdef  __BASIS_DEMO_DEBUG
			assert( __fun_size + __fun_size_i <= bs.get_fun_size() );
#endif
		}
		__fun_size += __fun_size_i;
	}
	bs.set_alp_size( __alp_size );
	bs.set_fun_size( __fun_size );
	p_bl = bs.data();
	int __alp_pos = 0, __fun_pos = 0;
	for(int lx = 0; lx < bs.size(); ++lx, ++p_bl )
	{
		p_bl->set_alp_data( bs.get_alp_data() + __alp_pos );
		__alp_pos += p_bl->get_alp_size();
		p_bf = p_bl->data();
		for(int j = 0; j < p_bl->size(); ++j, ++p_bf )
		{
			p_bf->set_fun_data( bs.get_fun_data() + __fun_pos );
			p_bf->set_alp_data( p_bl->get_alp_data() + p_bf->get_alp_pos() );
			__fun_pos += p_bf->get_fun_size();
		}
	}
	bs.pub_info();
	return 0;
}

int main(int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_basis<double>(argv[1]);
}
