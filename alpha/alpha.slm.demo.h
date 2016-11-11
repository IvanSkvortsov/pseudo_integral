#include"alpha.slm.h"
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<cmath>
#include"sf.math.h"
#include"mpreal.h"
#include"vector.3d.h"
#include"get.work.time.h"
#include<cassert>

using namespace std;

inline void print_i( char const * name, const int value)
{
	cout << setw(14) << name << " : " << setw(10) << value << endl;
}

inline const int read_lxsize( const char * name, istream & inp, int * _lxsize, const int _size )
{
	int __size = 0, _num;
	inp >> _num;
	cout << setw(10) << name << " : ";
	for(int i = 0; i < _size || i < _num; ++i)
	{
		inp >> _lxsize[i];
		if( i < _size )
		{
			__size += _lxsize[i];
			cout << setw(4) << _lxsize[i];
		}
	}
	cout << endl;
	cout << setw(4) << __size << endl << endl;
	return __size;
}
template<typename T>
inline void read_alp( const char * name, istream & inp, T * _vec, const int _size )
{
	cout << setw(10) << name << " :" << endl;
	for(int i = 0; i < _size; ++i)
	{
		inp >> _vec[i];
		if( i%8 == 1 )
			cout << setw(4) << i << " : " << setw(24) << setprecision(14) << scientific << _vec[i] << endl;
	}
	cout << endl;
}
template<typename T, typename U>
int demo_slm(const char * file)
{
	typedef typename alpha_map::_lmax_struct _lmax_struct;
	typedef typename alpha_map::_alpsize_struct _alpsize_struct;
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_val] can't open file '" << file << "'" << endl;
		return 1;
	}

	vector<int> pool( (1<<23) );// ~10 Mb
	memorystream ms;
	ms.setbuf( pool.data(), pool.size() * sizeof(int) );
	print_i( "pool.size", ms.bufsize() );
	cout << endl;

	_lmax_struct _lmax;
	inp >> _lmax._la_max >> _lmax._lb_max >> _lmax._l_max >> _lmax._lso_max;
	print_i( "la_max", _lmax._la_max );
	print_i( "lb_max", _lmax._lb_max );
	print_i( "l_max", _lmax._l_max );
	print_i( "lso_max", _lmax._lso_max );
	cout << endl;

	int _Asize[20], _Bsize[20], _Csize[20];
	int _a_size, _b_size, _c_size;
	_a_size = read_lxsize( "alpha A", inp, _Asize, _lmax._la_max + 1 );
	_b_size = read_lxsize( "alpha B", inp, _Bsize, _lmax._lb_max + 1 );
	_c_size = read_lxsize( "alpha C", inp, _Csize, _lmax._l_max + _lmax._lso_max + 1 );

	vector_3d<U> CA, CB;
	U CACB;
	for(int i = 0; i < 3; ++i) inp >> CA[i];// geometry
	for(int i = 0; i < 3; ++i) inp >> CB[i];// geometry
	CACB = CA.scalar( CB );

	vector<U> _Avec( _a_size ), _Bvec( _b_size ), _Cvec( _c_size );
	read_alp( "alpha A", inp, _Avec.data(), _a_size );// alphas
	read_alp( "alpha B", inp, _Bvec.data(), _b_size );
	read_alp( "alpha C", inp, _Cvec.data(), _c_size );

	alpha_pow<T,U> m_alp;
	m_alp.set_mapping_max();
	_alpsize_struct _alpsize = {._a_size = _a_size, _b_size, _c_size};
	cout << "m_alp.size : " << setw(10) << m_alp.comp_size( _lmax, _alpsize, _Asize, _Bsize ) << endl << flush;
	if( m_alp.is_mapping_min() )
		cout << "mapping : " << setw(10) << "minimum" << endl;
	else if( m_alp.is_mapping_mid() )
		cout << "mapping : " << setw(10) << "middle" << endl;
	else if( m_alp.is_mapping_max() )
		cout << "mapping : " << setw(10) << "maximum" << endl;
	else
		cout << "mapping : " << setw(10) << "undefined" << endl;

	cout << "sizeof class : " << setw(4) << sizeof(m_alp) << endl;
	m_alp.write_mxpow( ms, _lmax, _alpsize, _Asize, _Bsize );

	m_alp.init_map( _Asize, _Bsize, _Csize );// init_map

	matrix_slm<U> mx_slm;
	const char mx_slm_file[] = "vector.slm.32.src";
	get_work_time t1;
	t1.counting_start();
	mx_slm.read( mx_slm_file );
	t1.counting_end();
	std::clog << "matrix_slm<T>::read : " << t1.get_realseconds() << " , sec" << std::endl;

	alpha_slm<T,U> aslm;
	aslm.assign_mx2slm( m_alp );
	print_i( "mx2slm.size", aslm.size() );
	aslm.init_mxslm( CA.data(), _Avec.data(), CB.data(), _Bvec.data(), mx_slm );
	// init_mxalp
	/*
	if( m_alp.is_mapping_min() )
		m_alp.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data() );
	else if( m_alp.is_mapping_mid() )
		m_alp.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data(), CA.len() );
	else if( m_alp.is_mapping_max() )
		m_alp.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data(), CA.len(), CB.len(), CACB );
	else
	{
		cerr << "Error: [demo_val] mapping undefined" << endl;
		return 2;
	}
	m_alp.init_mxpow();// init_mxpow
	*/

	cout << endl;
	print_i( "mx1A.size", m_alp.M_mx1A_size() );
	print_i( "mx1B.size", m_alp.M_mx1B_size() );
	print_i( "mx1C.size", m_alp.M_mx1C_size() );
	if( m_alp.is_mapping_mid() || m_alp.is_mapping_max() )
	{
		print_i( "mx1kA.size", m_alp.M_mx1kA_size() );
		//print_i( "mx1powkA.size", m_alp.M_mx1powkA_size() );
	}
	if( m_alp.is_mapping_max() )
	{
		print_i( "mx1kB.size", m_alp.M_mx1kB_size() );
		print_i( "mx2k.size", m_alp.M_mx2k_size() );
		print_i( "mx2slm.size", m_alp.M_mx2slm_size() );
		//print_i( "mx1powkB.size", m_alp.M_mx1powkB_size() );
		//print_i( "mx2powk.size", m_alp.M_mx2powk_size() );
	}
	if( m_alp.is_mapping_min() )
		return 0;
	for(int i = 0; i < 52; ++i) cout << '-'; cout << endl;
	cout << setw(4) << "i" << setw(24) << "alpA" << setw(24) << "kA" << endl;
	U ta, tb, tc, _k;

	for(int la = 0, lb = 2; la < m_alp.la_size(); ++la)
	{
		m_alp.map1A_set_lx( la );
		m_alp.map2AB_set_la( la );
		m_alp.map2AB_set_lb( lb );
		m_alp.map1B_set_lx( lb );
		m_alp.map2powk_set_la( la );
		m_alp.map2powk_set_lb( lb );
		tb = CB.len() * _Bvec[m_alp.map1B_pos()] * 2;
		tb *= tb;
		for(int it = 0, i = m_alp.map1A_pos(); it < m_alp.map1A_size(); ++i, ++it )
		{
			if( i % 6 != 1 && i > 2) continue;
			ta = CA.len() * _Avec[i] * 2;
			ta *= ta;
			tc = 8 * _Avec[i] * _Bvec[m_alp.map1B_pos()] * CACB;
			_k = ta + tc + tb;
			_k = sqrt( _k );
			cout << setw(4) << i <<
				//setw(24) << setprecision(16) << scientific << m_alp.mx1A( i ) <<
				setw(24) << setprecision(16) << scientific << m_alp.mx1kA( la, it ) <<
				setw(24) << setprecision(16) << scientific << m_alp.mx1powkA( la, it)[2] <<
				setw(24) << setprecision(16) << scientific << m_alp.mx1kA( la, it ) * m_alp.mx1kA( la, it );
			if( m_alp.is_mapping_max() )
			{
				cout <<
				//setw(24) << setprecision(16) << scientific << m_alp.mx2k( la, it, lb, 0 ) <<
				setw(24) << setprecision(16) << scientific << m_alp.mx2k( it, 0 ) <<

				//setw(24) << setprecision(16) << scientific << m_alp.mx2powk( it, 0 )[2] <<
				setw(24) << setprecision(16) << scientific << m_alp.mx2powk( la, it, lb, 0 )[2] <<
				//" [" << m_alp.mx2powk( la, it, lb, 0 ) + 1 << "]";
				setw(24) << setprecision(16) << scientific << m_alp.mx2k( it, 0) * m_alp.mx2k( it, 0);
			}
			cout << endl;
		}
	}
	cout << endl;

	int la = 1, lb = 2;
	aslm.map2slm_set_la( la );
	aslm.map2slm_set_lb( lb );
	aslm.map2slm_set_ia( 7 );
	aslm.map2slm_set_ib( 3 );
	assert( aslm.map2slm_size() == (la + lb + 1) * (la + lb + 1) );
	for(int i = 0; i <= la+lb; ++i)
	{
		aslm.map2slm_set_lmb( i );
		for(int m = -i; m <= i; ++m)
		{
			cout << setw(3) << i << setw(3) << m << " : " <<
				setw(24) << setprecision(16) << scientific << aslm.mx2slm(m) <<
				" [" << aslm.mx2slm() + i * i + i + m << "]" << endl;
		}
	}
	return 0;
}

int demo_slm_d(const char * file)
{
	mpfr::mpreal::set_default_prec( mpfr::digits2bits(40) );
	//return demo_val<double, mpfr::mpreal>( file );
	//return demo_pow<double, long double>( file );
	//return demo_slm<double, long double>( file );
	return demo_slm<double, mpfr::mpreal>( file );
}

int main(int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_slm_d( argv[1] );
}
