#include"alpha.mem.h"

#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<cmath>
#include"sf.math.h"
#include"mpreal.h"
#include"vector.3d.h"

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
int demo_mem(const char * file)
{
	typedef typename alpha_map::_lmax_struct _lmax_struct;
	typedef typename alpha_map::_alpsize_struct _alpsize_struct;
	ifstream inp( file );
	if( !inp.is_open() )
	{
		cerr << "Error: [demo_val] can't open file '" << file << "'" << endl;
		return 1;
	}

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
	for(int i = 0; i < 3; ++i)
	{
		inp >> CA[i];
		cout << setw(3) << i << " : " << setw(24) << setprecision(16) << scientific << CA[i] << endl;
	}
	for(int i = 0; i < 3; ++i)
	{
		inp >> CB[i];
		cout << setw(3) << i << " : " << setw(24) << setprecision(16) << scientific << CB[i] << endl;
	}
	CACB = CA.scalar( CB );

	vector<U> _Avec( _a_size ), _Bvec( _b_size ), _Cvec( _c_size );
	read_alp( "alpha A", inp, _Avec.data(), _a_size );
	read_alp( "alpha B", inp, _Bvec.data(), _b_size );
	read_alp( "alpha C", inp, _Cvec.data(), _c_size );

	alpha_mem<T,U> m_alp;
	m_alp.set_mapping_max();
	_alpsize_struct _alpsize = {._a_size = _a_size, _b_size, _c_size};
	m_alp.set_lmax( _lmax );
	m_alp.set_alpsize( _alpsize );
	m_alp.set_A_size( _Asize );
	m_alp.set_B_size( _Bsize );
	m_alp.set_C_size( _Csize );
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
	m_alp.memory_create( "alpha.mem.map" );

	m_alp.init_map();// init_map
	// init_mxalp
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
	m_alp.init_mxpow(CA.data(), _Avec.data(), CB.data(), _Bvec.data() );// init_mxpow

	cout << endl;
	print_i( "mx1A.size", m_alp.M_mx1A_size() );
	print_i( "mx1B.size", m_alp.M_mx1B_size() );
	print_i( "mx1C.size", m_alp.M_mx1C_size() );
	if( m_alp.is_mapping_mid() || m_alp.is_mapping_max() )
	{
		print_i( "mx1kA.size", m_alp.M_mx1kA_size() );
		//print_i( "mx1powkA.size", m_alp.mx1powkA_size() );
	}
	if( m_alp.is_mapping_max() )
	{
		print_i( "mx1kB.size", m_alp.M_mx1kB_size() );
		print_i( "mx2k.size", m_alp.M_mx2k_size() );
		print_i( "mx2slm.size", m_alp.M_mx2slm_size() );
		//print_i( "mx1powkB.size", m_alp.mx1powkB_size() );
		//print_i( "mx2powk.size", m_alp.mx2powk_size() );
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
	return 0;
}

int demo_mem_d(const char * file)
{
	//mpfr::mpreal::set_default_prec( mpfr::digits2bits(40) );
	//return demo_val<double, mpfr::mpreal>( file );
	return demo_mem<double, long double>( file );
}

int main(int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_mem_d( argv[1] );
}
