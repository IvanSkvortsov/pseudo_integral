#include"alpha.val.h"
#include<iostream>
#include<iomanip>
#include<vector>
#include<fstream>
#include<cmath>
#include"sf.math.h"
#include"mpreal.h"

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
int demo_val(const char * file)
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

	U CA, CB, CACB;
	inp >> CA >> CB >> CACB;

	vector<U> _Avec( _a_size ), _Bvec( _b_size ), _Cvec( _c_size );
	read_alp( "alpha A", inp, _Avec.data(), _a_size );
	read_alp( "alpha B", inp, _Bvec.data(), _b_size );
	read_alp( "alpha C", inp, _Cvec.data(), _c_size );

	alpha_val<T,U> av;
	av.set_mapping_max();
	_alpsize_struct _alpsize = {._a_size = _a_size, _b_size, _c_size};
	cout << "av.size : " << setw(10) << av.comp_size( _lmax, _alpsize, _Asize, _Bsize ) << endl << flush;
	cout << "mapping : " << setw(10) <<
		( av.is_mapping_min() ? "minimum" : (av.is_mapping_mid() ? "middle" : (av.is_mapping_max() ? "maximum" : "undefined") ) ) << endl;
	av.write_mxalp( ms, _lmax, _alpsize );

	av.init_map( _Asize, _Bsize, _Csize );
	if( av.is_mapping_min() )
		av.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data() );
	else if( av.is_mapping_mid() )
		av.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data(), CA );
	else if( av.is_mapping_max() )
		av.init_mxalp( _Avec.data(), _Bvec.data(), _Cvec.data(), CA, CB, CACB );
	else
	{
		cerr << "Error: [demo_val] mapping undefined" << endl;
		return 2;
	}
	av.init_mx1_alp( _Avec.data(), _Bvec.data(), _Cvec.data() );
	av.init_norm( _Avec.data(), _Bvec.data() );

	print_i( "mx1A_norm.size", av.M_mx1A_norm_size() );
	print_i( "mx1A_norm.size", av.mx1A_norm_size() );
	print_i( "mx1B_norm.size", av.M_mx1B_norm_size() );
	print_i( "mx1B_norm.size", av.mx1B_norm_size() );
	print_i( "mx2AB_norm.size", av.M_mx2AB_norm_size() );
	print_i( "mx2AB_norm.size", av.mx2AB_norm_size() );
	print_i( "mx2AB_exp.size", av.M_mx2AB_exp_size() );
	cout << endl;
	print_i( "mx1A.size", av.M_mx1A_size() );
	print_i( "mx1B.size", av.M_mx1B_size() );
	print_i( "mx1C.size", av.M_mx1C_size() );
	if( av.is_mapping_mid() || av.is_mapping_max() )
	{
		print_i( "mx1kA.size", av.M_mx1kA_size() );
	}
	if( av.is_mapping_max() )
	{
		print_i( "mx1kB.size", av.M_mx1kB_size() );
		print_i( "mx2k.size", av.M_mx2k_size() );
	}
	if( av.is_mapping_min() )
		return 0;
	for(int i = 0; i < 52; ++i) cout << '-'; cout << endl;
	cout << setw(4) << "i" << setw(24) << "alpA" << setw(24) << "kA" <<
		setw(24) << "k" << setw(24) << "k" <<
		setw(24) << "k" << setw(24) << "|CA|" << endl;
	U ta, tb, tc, _k;
	for(int la = 0, lb = 2; la < av.la_size(); ++la)
	{
		av.map1A_set_lx( la );
		av.map2AB_set_la( la );
		av.map2AB_set_lb( lb );
		av.map1B_set_lx( lb );
		tb = CB * _Bvec[av.map1B_pos()] * 2;
		tb *= tb;
		for(int it = 0, i = av.map1A_pos(); it < av.map1A_size(); ++i, ++it )
		{
			if( i % 6 != 1 ) continue;
			ta = CA * _Avec[i] * 2;
			ta *= ta;
			tc = 8 * _Avec[i] * _Bvec[av.map1B_pos()] * CACB;
			_k = ta + tc + tb;
			_k = sqrt( _k );
			cout << setw(4) << i <<
				setw(24) << setprecision(16) << scientific << av.M_mx1A( i ) <<
				setw(24) << setprecision(16) << scientific << av.M_mx1kA( i );
			if( av.is_mapping_max() )
			{
				cout <<
				setw(24) << setprecision(16) << scientific << av.mx2k( la, it, lb, 0 ) <<
				setw(24) << setprecision(16) << scientific << av.mx2k( it, 0 );
			}
			cout <<
				setw(24) << setprecision(16) << scientific << math::convert_float<T,U>(_k) <<
				setw(24) << setprecision(16) << scientific << CA << endl;
		}
	}
	cout << endl;
	U sqr_CA = CA * CA, sqr_CB = CB * CB;
	int iter = 0, IT_MAX = 100;
	av.init_exp( _Avec.data(), _Bvec.data(), CA * CA, CB * CB );
	for(int la = 0; la < av.la_size(); ++la)
	{
		av.alpha_map::map1A_set_lx( la );
		av.alpha_map::map2AB_set_la( la );
		for(int lb = 0; lb < av.lb_size(); ++lb)
		{
			av.alpha_map::map1B_set_lx( lb );
			av.alpha_map::map2AB_set_lb( lb );
			for(int ia = 0; ia < av.alpha_map::map1A_size(); ++ia)
			{
				av.alpha_map::map2AB_set_ia( ia );
				av.alpha_map::map1A_set_ix( ia );
				av.alpha_val<T,U>::mx1A_set_idx();
				for(int ib = 0; ib < av.alpha_map::map1B_size(); ++ib)
				{
					av.alpha_map::map2AB_set_ib( ib );
					av.alpha_map::map1B_set_ix( ib );
					av.alpha_val<T,U>::mx1B_set_idx();
					av.mx2AB_exp_set_idx();
					U xa = -sqr_CA * av.mx1A();
					U xb = -sqr_CB * av.mx1B();
					U x = xa + xb;
					if( iter++ >= IT_MAX )
						break;
					if( iter%50 == 1 )
					cout << setw(4) << "la" << setw(4) << "lb" << setw(4) << "ia" << setw(4) << "ib" <<
						setw(5) << "idx" <<
						setw(26) << "-A*|CA|^2" <<
						setw(26) << "-B*|CB|^2" <<
						setw(26) << "-A*|CA|^2 - B*|CB|^2" <<
						setw(26) << "exp(-A*|CA|^2 - B*|CB|^2)" <<
						endl;
					cout << setw(4) << la << setw(4) << lb <<  setw(4) << ia << setw(4) << ib <<
						setw(5) << av.alpha_map::map2AB_idx() <<
						setw(26) << setprecision(16) << scientific << xa <<
						setw(26) << setprecision(16) << scientific << xb <<
						setw(26) << setprecision(16) << scientific << x <<
						setw(26) << setprecision(16) << scientific << av.mx2AB_exp() <<
						" [" << &av.mx2AB_exp() << "]" <<
						setw(5) << &av.mx2AB_exp() - av.M_mx2AB_exp_data() <<
						endl;
				}
			}
			cout << endl;
		}
	}
	return 0;
}

int demo_val_d(const char * file)
{
	//mpfr::mpreal::set_default_prec( mpfr::digits2bits(40) );
	//return demo_val<double, mpfr::mpreal>( file );
	//return demo_val<double, long double>( file );
	return demo_val<double, double>( file );
}

int main(int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	return demo_val_d( argv[1] );
}
