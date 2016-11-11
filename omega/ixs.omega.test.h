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
int ixs_omg_read_lmax( ixs_omega<T,U> & ixs_omg, const char * file )
{
	clog << "[" << &ixs_omg << "] READ : lmax" << endl;
	clog << "file : '" << file << "'" << endl;
	ifstream inp( file );
	if( !inp.is_open() )
		return 1;
	int l_max, lx_max;
	inp >> l_max >> lx_max;
	ixs_omg.set_l_max( l_max );
	ixs_omg.set_lx_max( lx_max );

	ixs_omg.pub_info( clog );
	clog << endl;
	return 0;
}

template<typename T, typename U>
int ixs_omg_set_nodesize( ixs_omega<T,U> & ixs_omg, const char * file )
{
	clog << "[" << &ixs_omg << "] NODE : node1_size, node2_size" << endl;
	clog << "file : '" << file << "'" << endl;
	int __node1_size = 0, __node2_size = 0;
	__node1_size = ixs_omg.M_node1_SIZE();
	__node2_size = ixs_omg.M_node2_SIZE( file );
	if( __node2_size == 0 )
		__node2_size = INTEGRAL_COUNT;
	if( __node1_size <= 0 || __node2_size <= 0 )
		return 1;
	ixs_omg.set_node1_size( __node1_size );
	ixs_omg.set_node2_size( __node2_size );

	ixs_omg.pub_info( clog );
	clog << endl;
	return 0;
}

template<typename T, typename U>
int ixs_omg_open( ixs_omega<T,U> & ixs_omg, const char * file )
{
	clog << "[" << &ixs_omg << "] OPEN : memory mapping" << endl;
	clog << "file : '" << file << "'" << endl;
	ixs_omg.memory_open( file );
	clog << "size : " << setw(8) << ixs_omg.comp_size() << endl;
	ixs_omg.pub_info( clog );
	clog << endl;
	return 0;
}
template<typename T, typename U>
int ixs_omg_create( ixs_omega<T,U> & ixs_omg, const char * file )
{
	clog << "[" << &ixs_omg << "] CREAT : memory mapping" << endl;
	clog << "file : '" << file << "'" << endl;
	clog << "size : " << setw(8) << ixs_omg.comp_size() << endl;
	ixs_omg.memory_create( file );
	ixs_omg.pub_info( clog );
	clog << endl;
	return 0;
}
template<typename T, typename U>
inline int ixs_omg_init( ixs_omega<T,U> & ixs_omg )
{
	clog << "[" << &ixs_omg << "] INIT : node0 initialization" << endl;
	ixs_omg.node0_init();
	clog << endl;
	return 0;
}

int mx_slm_read_files( std::vector<std::string> & vs, const char * file )
{
	clog << "[" << &vs << "] READ : slm files" << endl;
	ifstream inp( file );
	if( !inp.is_open() )
		return 1;
	int n;
	inp >> n;
	if( n <= 0 )
		return 2;
	vs.resize( n );
	for(int i = 0; i < n; ++i)
	{
		if( inp == 0 )
			return 3;
		inp >> vs[i];
	}
	clog << setw(4) << n << endl;
	for(int i = 0; i < n; ++i)
		clog << setw(4) << i << " : '" << vs[i] << "'" << endl;
	clog << endl;
	return 0;
}

template<typename T, typename U>
int xpn_int_read( xyz_pown_integral<U> & xpn_int, ixs_omega<T,U> const & ixs_omg, const char * file )
{
	clog << "[" << &xpn_int << "] READ : xyz pown integral" << endl;
	clog << "file : '" << file << "'" << endl;
	xpn_int.read( file );
	clog << "xpn_int've been read" << endl;
	int lmax = (ixs_omg.node0_l_max() + ixs_omg.node0_lx_max()) * 2;
	if( xpn_int.i_max() < lmax || xpn_int.j_max() < lmax || xpn_int.k_max() < lmax )
	{
		std::cerr << "Error: [xpn_int_read] xyz_pown_integral size" << std::endl;
		std::cerr << "lmax : " << lmax << std::endl;
		std::cerr << "xyz_pown_integral::i_max : " << xpn_int.i_max() << std::endl;
		std::cerr << "xyz_pown_integral::j_max : " << xpn_int.j_max() << std::endl;
		std::cerr << "xyz_pown_integral::k_max : " << xpn_int.k_max() << std::endl;
		return 1;
	}
	clog << endl;
	return 0;
}

template<typename T, typename U>
int ixs_omg_comp( ixs_omega<T,U> & ixs_omg, std::vector<std::string> & vs, const char * file, xyz_pown_integral<U> const & xpn_int )
{
	clog << "[" << &ixs_omg << "] COMPUTE : ixs omega integral" << endl;
	
	char const file_log[] = "ixs.omega.logx";
	ofstream out( file_log );
	streambuf * flog_buf = out.rdbuf();
	clog << "file : '" << file_log << "'" << endl;

	streambuf * cerr_buf = cerr.rdbuf();
	streambuf * clog_buf = clog.rdbuf();
	streambuf * cout_buf = cout.rdbuf();

	cerr.rdbuf( flog_buf );
	clog.rdbuf( flog_buf );
	cout.rdbuf( flog_buf );

	int __size = 0;
	__size = ixs_omg.run_calc( vs, file, xpn_int );
	cout << flush;

	cerr.rdbuf( cerr_buf );
	clog.rdbuf( clog_buf );
	cout.rdbuf( cout_buf );

	clog << "size : " << setw(8) << __size << endl;
	clog << "COMPLETE" << endl;
	clog << endl;

	out.close();
	return 0;
}

template<typename T, typename U>
int ixs_omg_shrink( ixs_omega<T,U> & ixs_omg, const char * file )
{
	clog << "[" << &ixs_omg << "] SHRINK : to fit" << endl;
	clog << "file : '" << file << "'" << endl;
	ixs_omg.shrink_to_fit( file );

	ixs_omg.pub_info( clog );
	clog << endl;
	return 0;
}
template<typename T, typename U>
int ixs_omg_test_lzero( ixs_omega<T,U> & ixs_omg )
{
	ixs_omega<T,U> ixs_omg_x( ixs_omg );
	assert( ixs_omg_x.node0_lx_max() == ixs_omg.node0_lx_max() );
	assert( ixs_omg_x.node0_l_max() == ixs_omg.node0_l_max() );
	assert( ixs_omg_x.M_lsize_l_max() == ixs_omg.M_lsize_l_max() );
	assert( ixs_omg_x.M_lsize_lx_max() == ixs_omg.M_lsize_lx_max() );
	assert( ixs_omg_x.M_lsize_node1_size() == ixs_omg.M_lsize_node1_size() );
	assert( ixs_omg_x.M_lsize_node2_size() == ixs_omg.M_lsize_node2_size() );
	ixs_omg_x.set_lmax();// %node0_set_lmax()
	ixs_omg.set_l( 0 );//   %node0_set_l( int )
	const typename ixs_omega<T,U>::value_struct * p_omg;
	for(int nx = 0; nx <= ixs_omg.node0_lx_max(); ++nx )
	{
		ixs_omg_x.node0_set_lx( nx );
		ixs_omg.node0_set_lx( nx );
		if( ixs_omg_x.node1_size() != ixs_omg.node1_size() )
		{
			cerr << "Error:" << endl;
			cerr << "nx : " << nx << endl;
			cerr << "ixs_omg_x.node1_size : " << ixs_omg_x.node1_size() << endl;
			cerr << "ixs_omg.  node1_size : " << ixs_omg.node1_size() << endl;
		}
		assert( ixs_omg_x.node1_size() == ixs_omg.node1_size() );
		assert( ixs_omg_x.node1_n() == ixs_omg.node1_n() );
		assert( ixs_omg_x.node1_m() == ixs_omg.node1_m() );
		assert( ixs_omg_x.node1_n() == (nx/2+1) );
	cout << "----" << endl;
	cout << setw(4) << "l" << setw(4) << "lx" << setw(4) << "lmb" << setw(4) << "i'l" <<
		setw(4) << "m" << setw(4) << "mu" << setw(26) << "value" << endl << endl;
		for(int i_lmb = 0; i_lmb < ixs_omg_x.node1_n(); ++i_lmb )
		{
			clog << "--------" << endl;
			clog << setw(8) << nx << setw(4) << (nx%2 + i_lmb*2) << setw(4) << i_lmb << setw(4) << ixs_omg_x.node1_n() << endl;
			ixs_omg_x.node1_set_lambda_i( i_lmb );
			ixs_omg.node1_set_lambda_i( i_lmb );
			for(int i_abc = 0; i_abc < ixs_omg_x.node1_m(); ++i_abc )
			{
				ixs_omg_x.node1_set_abc_i( i_abc );
				ixs_omg.node1_set_abc_i( i_abc );
				assert( ixs_omg_x.node2_pos() == ixs_omg.node2_pos() );
				assert( ixs_omg_x.node2_size() == ixs_omg.node2_size() );
				assert( ixs_omg_x.node2_data() == ixs_omg.node2_data() );
				assert( ixs_omg_x.node2_data() != 0 );
				assert( ixs_omg.node2_data() != 0 );

				p_omg = ixs_omg_x.get_omg_data();

				for(int i = 0; i < ixs_omg_x.node2_size(); ++i, ++p_omg)
				{
				assert( p_omg->l == 0 );
				assert( p_omg->lx == nx );
				assert( p_omg->lmb == (nx%2 + i_lmb*2) );
				assert( p_omg->i_abc == i_abc || ixs_omg_x.node2_size() == 0 );
					cout << setw(4) << p_omg->l << setw(4) << p_omg->lx << setw(4) << p_omg->lmb << setw(4) << p_omg->i_abc <<
						setw(4) << p_omg->m << setw(4) << p_omg->mu <<
						setw(26) << setprecision(16) << scientific << p_omg->d << endl;
				}
			}
		}
	}
	return 0;
}

template<typename T, typename U>
int ixs_omg_test_l( ixs_omega<T,U> & ixs_omg, int l )
{
	if( l == 0 )
		ixs_omg.set_lmax();
	else
		ixs_omg.set_l( l );
	const typename ixs_omega<T,U>::value_struct * p_omg;
	for(int nx = (ixs_omg.node0_lx_max()+1) * (l == 0 ? 1 : 0 ); nx <= ixs_omg.node0_lx_max()*( l == 0 ? 2 : 1 ); ++nx )
	{
		ixs_omg.node0_set_lx( nx );
	cout << "----" << endl;
	cout << setw(4) << "l" << setw(4) << "lx" << setw(4) << "lmb" << setw(4) << "i'l" <<
		setw(4) << "m" << setw(4) << "mu" << setw(26) << "value" << endl << endl;
		for(int i_lmb = 0; i_lmb < ixs_omg.node1_n(); ++i_lmb )
		{
			clog << "--------" << endl;
			clog << setw(8) << nx << setw(4) << (nx%2 + i_lmb*2) << setw(4) << i_lmb << setw(4) << ixs_omg.node1_n() << endl;
			ixs_omg.node1_set_lambda_i( i_lmb );
			for(int i_abc = 0; i_abc < ixs_omg.node1_m(); ++i_abc )
			{
				ixs_omg.node1_set_abc_i( i_abc );
				assert( ixs_omg.node2_pos() != 0 );
				assert( ixs_omg.node2_data() != 0 );

				p_omg = ixs_omg.get_omg_data();

				for(int i = 0; i < ixs_omg.node2_size(); ++i, ++p_omg)
				{
				assert( p_omg->l == l );
				assert( p_omg->lx == nx );
				assert( p_omg->lmb == ( l == 0 ? (nx%2 + i_lmb*2) : ( l < nx ? ((l+nx)%2 + i_lmb*2) : (l-nx+i_lmb*2)) ) );
				//assert( p_omg->i_abc == i_abc );
				assert( p_omg->i_abc == i_abc || ixs_omg.get_omg_size() == 0 );
					cout << setw(4) << p_omg->l << setw(4) << p_omg->lx << setw(4) << p_omg->lmb << setw(4) << p_omg->i_abc <<
						setw(4) << p_omg->m << setw(4) << p_omg->mu <<
						setw(26) << setprecision(16) << scientific << p_omg->d << endl;
				}
			}
		}
	}
	return 0;
}
template<typename T, typename U>
int ixs_omg_test_iter( ixs_omega<T,U> & ixs_omg, char const * file )
{
	clog << "[" << &ixs_omg << "] TEST : iteration" << endl;
	clog << "file : '" << file << "'" << endl;
	ofstream out( file );
	streambuf * flog_buf = out.rdbuf();

	streambuf * cerr_buf = cerr.rdbuf(), * cout_buf = cout.rdbuf(), * clog_buf = clog.rdbuf();

	cerr.rdbuf( flog_buf );
	cout.rdbuf( flog_buf );
	clog.rdbuf( flog_buf );

	int _st;
	_st = ixs_omg_test_lzero( ixs_omg );
	_st += ixs_omg_test_l( ixs_omg, 0 );
	for(int l = 1; l <= ixs_omg.node0_l_max(); ++l)
		_st += ixs_omg_test_l( ixs_omg, l );

	cerr.rdbuf( cerr_buf );
	clog.rdbuf( clog_buf );
	cout.rdbuf( cout_buf );

	clog << "COMPLETE" << endl << endl;
	return _st;
}

template<typename T, typename U>
int ixs_omg_demo(char * file, char * file_lmax, char * file_angsize, char * file_vecs, char * file_xpn )
{
	ixs_omega<T, U> ixs_omg;

	int _st = 0;
	_st = ixs_omg_read_lmax( ixs_omg, file_lmax );
	assert( _st == 0 );

	const char file_num[] = "angsize.file";
	_st = ixs_omg_set_nodesize( ixs_omg, file_num );
	assert( _st == 0 );

	ixs_omg_create( ixs_omg, file );

	ixs_omg_init( ixs_omg );

	std::vector<std::string> file_vs;
	_st = mx_slm_read_files( file_vs, file_vecs );
	assert( _st == 0 );

	xyz_pown_integral<U> xpn_int;
	_st = xpn_int_read( xpn_int, ixs_omg, file_xpn );
	assert( _st == 0 );

	ixs_omg_comp( ixs_omg, file_vs, file_angsize, xpn_int );

	string new_file = file;
	new_file += ".fit";
	ixs_omg_shrink( ixs_omg, new_file.c_str() );

	ixs_omg_test_iter( ixs_omg, "ixs.omega.test.log" );

	return 0;
}

template<typename T, typename U>
int ixs_omg_demo(char * file )
{
	ixs_omega<T,U> ixs_omg;
	ixs_omg_open( ixs_omg, file );

	ixs_omg_test_iter( ixs_omg, "ixs.omega.test.log" );

	return 0;
}

int ixs_omg_demo_d(char * file )
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(100) );
	//return ixs_omg_demo<long double, mpfr::mpreal>( file, file_lmax, file_angsize, file_vecs, file_xpn );
	return ixs_omg_demo<double, mpfr::mpreal>( file );
}

int ixs_omg_demo_d(char * file, char * file_lmax, char * file_angsize, char * file_vecs, char * file_xpn)
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(100) );
	//return ixs_omg_demo<long double, mpfr::mpreal>( file, file_lmax, file_angsize, file_vecs, file_xpn );
	return ixs_omg_demo<double, mpfr::mpreal>( file, file_lmax, file_angsize, file_vecs, file_xpn );
}

int main(int argc, char ** argv)
{
	if( argc != 6 && argc != 2 )
	{
		cerr << "Error: [main] usage\n'./main.exe file.map file.lmax file.angsize file.vecs file.xpn' or\n'./main.exe file.map'" << endl;
		cerr << "need 6 argumets" << endl;
		cerr << "1 : memory mapping filename" << endl;
		cerr << "2 : maximum angular momentum filename" << endl;
		cerr << "3 : integral size filename" << endl;
		cerr << "4 : products of spherical harmonics multiplication filename" << endl;
		cerr << "5 : values of xyz_pown_integral filename" << endl;
		cerr << "need 2 argumet" << endl;
		cerr << "1 : memory mapping filename" << endl;
		return 1;
	}
	else if( argc == 6 )
		return ixs_omg_demo_d( argv[1], argv[2], argv[3], argv[4], argv[5] );
	else
		return ixs_omg_demo_d( argv[1] );
}
