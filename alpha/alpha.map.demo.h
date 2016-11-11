#include"alpha.map.h"
#include<iostream>
#include<iomanip>
#include<vector>

using namespace std;

inline void print_i( const char * name, const int value )
{
	cout << setw(14) << name << " : " << setw(4) << value << endl;
}
int demo_alpmap()
{
	typedef typename alpha_map::_lmax_struct _lmax_struct;

	vector<int> pool( 8192 );
	memorystream ms;
	ms.setbuf( pool.data(), pool.size() * sizeof(int) );
	print_i( "pool.size", ms.bufsize() );

	_lmax_struct _lmax;
	_lmax._l_max = 4;
	_lmax._lso_max = 4;
	_lmax._la_max = 4;
	_lmax._lb_max = 4;
	const int Asize[] = {15, 10,  5,  3, 1, 1, 1, 1, 1, 1, 1};
	const int Bsize[] = {17, 13,  8,  5, 3, 1, 1, 1, 1, 1, 1};
	const int Csize[] = { 8,  5,  3,  2, 7, 5, 4, 4, 3, 1, 1, 1, 1, 1, 1, 1, 1};

	print_i( "l_max", _lmax._l_max );
	print_i( "lso_max", _lmax._lso_max );
	print_i( "la_max", _lmax._la_max );
	print_i( "lb_max", _lmax._lb_max );
	cout << endl;

	alpha_map am;
	am.set_mapping_min();
	print_i( "size", am.comp_size( _lmax ) );
	am.write_map( ms, _lmax );
	am.init_map( Asize, Bsize, Csize );
	cout << "[" << &am << "]" << endl;
	cout << "A : "; for(int i = 0; i < am.M_map1A_size(); ++i) cout << setw(3) << am.map1A_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am.M_map1A_size(); ++i) cout << setw(3) << Asize[i]; cout << endl;
	cout << "B : "; for(int i = 0; i < am.M_map1B_size(); ++i) cout << setw(3) << am.map1B_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am.M_map1B_size(); ++i) cout << setw(3) << Bsize[i]; cout << endl;
	cout << "C : "; for(int i = 0; i < am.M_map1C_size(); ++i) cout << setw(3) << am.map1C_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am.M_map1C_size(); ++i) cout << setw(3) << Csize[i]; cout << endl;
	cout << endl;
	am.test_alp_map();
	/*
	for(int la = 0, map1A_size, map1B_size, map1C_size; la <= am.la_max(); ++la)
	{
		am.map1A_set_lx( la );
		map1A_size = am.map1A_size();
		am.map3ABC_set_la( la );
		for(int lb = 0; lb <= am.lb_max(); ++lb)
		{
			am.map1B_set_lx( lb );
			map1B_size = am.map1B_size();
			am.map3ABC_set_lb( lb );
			for(int l = 0; l <= am.l_max(); ++l)
			{
				am.map1C_set_lx( l );
				map1C_size = am.map1C_size();
				am.map3ABC_set_l( l );
				cout << setw(3) << la << setw(3) << lb << setw(3) << l <<
					setw(14) << map1A_size * map1B_size * map1C_size <<
					setw(14) << am.map3ABC_size();
				if( map1A_size * map1B_size * map1C_size != am.map3ABC_size() )
				{
					cout << setw(14) << "error" << endl;
				} else {
					cout << endl;
				}
			}
		}
	}
	*/

	alpha_map am2;
	ms.seek( 0 );
	am2.read_map( ms );
	cout << "[" << &am2 << "]" << endl;
	cout << "A : "; for(int i = 0; i < am2.M_map1A_size(); ++i) cout << setw(3) << am2.map1A_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am2.M_map1A_size(); ++i) cout << setw(3) << Asize[i]; cout << endl;
	cout << "B : "; for(int i = 0; i < am2.M_map1B_size(); ++i) cout << setw(3) << am2.map1B_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am2.M_map1B_size(); ++i) cout << setw(3) << Bsize[i]; cout << endl;
	cout << "C : "; for(int i = 0; i < am2.M_map1C_size(); ++i) cout << setw(3) << am2.map1C_size(i); cout << endl;
	cout << "  : "; for(int i = 0; i < am2.M_map1C_size(); ++i) cout << setw(3) << Csize[i]; cout << endl;
	cout << endl;

	return 0;
}

int main()
{
	return demo_alpmap();
}
