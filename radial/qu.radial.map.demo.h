#include"qu.radial.mem.h"
#include<iostream>
#include<iomanip>
#include<cstdio>
#include<cassert>
#include<vector>

using namespace std;

//#undef  __QU_RADIAL_MAP_MIDDLE_SPEC

int demo_qu_map()
{
	typedef typename alpha_siz::_lmax_struct _lmax_struct;
	typedef typename qu_radial_map::map3qu_pos_type map3qu_pos_type;

	int la_max, lb_max, l_max, lso_max;
	cout << "Enter lmax:" << endl;
	cout << "la_max  > "; cin >> la_max; cout << setw(3) << la_max << endl;
	cout << "lb_max  > "; cin >> lb_max; cout << setw(3) << lb_max << endl;
	cout << "l_max   > "; cin >> l_max; cout << setw(3) << l_max << endl;
	cout << "lso_max > "; cin >> lso_max; cout << setw(3) << lso_max << endl;
	if( la_max < 0 || lb_max < 0 || l_max < 0 || lso_max < 0 )
	{
		cerr << "Error: [demo_qu] negative anular moment(um|a)" << endl;
		cerr << "la_max  : " << setw(8) << la_max << endl;
		cerr << "lb_max  : " << setw(8) << lb_max << endl;
		cerr << "l_max   : " << setw(8) << l_max << endl;
		cerr << "lso_max : " << setw(8) << lso_max << endl;
		return 1;
	}
	_lmax_struct lmax;
	lmax._la_max = la_max;
	lmax._lb_max = lb_max;
	lmax._l_max = l_max;
	lmax._lso_max = lso_max;

	//qu_radial_map qu_map;
	qu_radial_mem qu_map;
	qu_map.set_lmax( lmax );
	int _map_t;
	cout << "Enter number (mapping type):\n1 - maximum;\n2 - middle;\n3 - minimum;\n> "; cin >> _map_t;
	switch( _map_t )
	{
	case 1:
		qu_map.set_mapping_max();
		cout << "maximum" << endl;
		break;
	case 2:
		qu_map.set_mapping_mid();
		cout << "middle" << endl;
		break;
	case 3:
		qu_map.set_mapping_min();
		cout << "minimum" << endl;
		break;
	default:
		cerr << "Error: [demo_qu_map] You've entered wrong number " << _map_t << endl;
		exit(1);
	}
	static const char file_map[] = "qu.radial.map.txt";
	qu_map.memory_create( file_map );
	//vector<int> Pool;
	//Pool.resize( 1024 * 1024 );
	//memorystream ms;
	//ms.setbuf( Pool.data(), Pool.size() * sizeof(int) );

	const int rad_size = qu_map.comp_size( lmax );
	assert( rad_size <= qu_map.bufsize() );

	qu_map.init_map();

	cout << " map.data : [" << qu_map.data() << "]" << endl;
	cout << " map.size : " << qu_map.size() << endl;
	cout << "comp_size : " << setw(8) << rad_size << endl;
	
	cout << "map3qu.size : " << setw(8) << qu_map.M_map3qu_pos_size() << endl;
	cout << "map3qu.n    : " << setw(8) << qu_map.M_map3qu_pos_n() << endl;
	cout << "map3qu.m    : " << setw(8) << qu_map.M_map3qu_pos_m() << endl;
	cout << "map3qu.p    : " << setw(8) << qu_map.M_map3qu_pos_p() << endl;

	//qu_radial_map rad;
	qu_radial_mem rad;
	rad.set_mapping( qu_map.get_mapping() );
	int _run_t = 0;
	cout << "Enter number:\n1 - to run copy\n2 - to run new\n> "; cin >> _run_t;
	switch( _run_t )
	{
	case 1:
		rad = qu_map;
		cout << "copy" << endl;
		break;
	case 2:
		qu_map.close();
		rad.memory_open( file_map );
		cout << "new" << endl;
		break;
	default:
		cerr << "Error: [demo_qu_map] You've entered not mentioned number " << _run_t << endl;
		exit(1);
	}
	int iter = 0;
	cout << setw(6) << "iter" << " : " << setw(4) << "la" << setw(4) << "lb" << setw(4) << "l" <<
		setw(8) << "pos" << setw(4) << "siz" << setw(4) << "n" << setw(4) << "m" << setw(4) << "p" << setw(6) << "size" <<
		setw(8) << "%" <<
		endl << endl;
	int _it = 0, _it_all = 0;
	for(int la = 0; la <= rad.M_get_la_max(); ++la)
	{
		rad.qu_set_la( la );
		for(int lb = 0; lb <= rad.M_get_lb_max(); ++lb)
		{
			rad.qu_set_lb( lb );
			/*
			for(int l = 0; l < rad.M_get_l_max(); ++l)
			{
#ifdef  __QU_RADIAL_MAP_MIDDLE_SPEC
				if( l == rad.M_get_l_max() );
				else if( l%2 != lb%2 || l > lb )
					continue;
#endif
*/
#ifdef  __QU_RADIAL_MAP_MIDDLE_SPEC
			for(int l = lb%2; l < rad.M_get_l_max() && l <= lb; l += 2 )
#else
			for(int l = 0; l < rad.M_get_l_max(); ++l )
#endif
			{
				rad.qu_set_l( l );
				cout << setw(6) << iter++ << " : " << setw(4) << la << setw(4) << lb << setw(4) << l <<
					setw(8) << rad.map1qu_pos() << setw(4) << rad.qu_size() <<
					setw(4) << rad.map1qu_n() << setw(4) << rad.map1qu_m() << setw(4) << rad.map1qu_p() <<
					setw(6) << rad.map1qu_size();
				double per_cent;
				if( rad.map1qu_size() == 0 )
					per_cent = 0;
				else
				{
					per_cent = rad.qu_size();
					per_cent /= rad.map1qu_size();
					per_cent *= 100;
				}
				fprintf(stdout, "%8.2f\n", per_cent );
				//fprintf(stdout, "%8.2f\n", double(rad.qu_size())/rad.map1qu_size() *100);
				_it += rad.qu_size();
				_it_all += rad.map1qu_size();
			}
			int l = rad.M_get_l_max();
			rad.qu_set_l( l );
			cout << setw(6) << iter++ << " : " << setw(4) << la << setw(4) << lb << setw(4) << l <<
				setw(8) << rad.map1qu_pos() << setw(4) << rad.qu_size() <<
				setw(4) << rad.map1qu_n() << setw(4) << rad.map1qu_m() << setw(4) << rad.map1qu_p() <<
				setw(6) << rad.map1qu_size();
			double per_cent;
			if( rad.map1qu_size() == 0 )
				per_cent = 0;
			else
			{
				per_cent = rad.qu_size();
				per_cent /= rad.map1qu_size();
				per_cent *= 100;
			}
			fprintf(stdout, "%8.2f\n", per_cent );
			//fprintf(stdout, "%8.2f\n", double(rad.qu_size())/rad.map1qu_size() *100);
			_it += rad.qu_size();
			_it_all += rad.map1qu_size();
		}
		cout << endl;
	}
				cout << setw(6) << "" << " : " << setw(12) << "" <<
					 setw(12) << _it <<
					setw(8) << "" << setw(10) << _it_all;
				fprintf(stdout, "%8.2f\n", double(_it)/_it_all *100);
	//cout << "total.size : " << setw(8) << rad.get_mx3().size() << endl;
	//for(int i = 0; i < rad.get_mx3().size(); ++i)
	//	cout << setw(8) << i << " : " << setw(8) << rad.get_mx3()[i] << endl;
	//cout << endl;
	int la, lb, l;
	while( true )
	{
		cout << "-1 to exit" << endl;
		cout << "la > "; cin >> la; cout << setw(3) << la << endl; if( la < 0 || la > lmax._la_max ) break;
		cout << "lb > "; cin >> lb; cout << setw(3) << lb << endl; if( lb < 0 || lb > lmax._lb_max ) break;
		cout << "l  > "; cin >> l;  cout << setw(3) << l << endl; if( l  < 0 || l  > lmax._l_max  ) break;
		rad.qu_set_la( la );
		rad.qu_set_lb( lb );
		rad.qu_set_l( l );

		cout << "map1qu.data : [" << rad.map1qu_data() << "]" << endl;
		cout << "map1qu.size : " << setw(8) << rad.map1qu_size() << endl;
		cout << "map1qu.n    : " << setw(8) << rad.map1qu_n() << endl;
		cout << "map1qu.m    : " << setw(8) << rad.map1qu_m() << endl;
		cout << "map1qu.p    : " << setw(8) << rad.map1qu_p() << endl;
		cout << "map1qu.pos  : " << setw(8) << rad.map1qu_pos() << endl;
		cout << "    qu.size : " << setw(8) << rad.qu_size() << endl;

		cout << setw(4) << "na" << setw(4) << "nb" <<
			setw(4) << "N" << setw(4) << "l'a" << setw(4) << "l'b" << setw(8) << "idx" << endl << endl;
		if( l == lmax._l_max )
		{
			for(int N = ( rad.is_mapping_mid() ? lb : 0 ); N <= la+lb; ++N)
			{
				for(int lmb = N%2; lmb <= N; lmb += 2)
				{
					rad.qu_set_lmbx( N, lmb );
					cout << setw(4) << '-' << setw(4) << '-' <<
						setw(4) << N << setw(4) << lmb << setw(4) << '0' << setw(8) << rad.qu_idx() <<
						" [" << &rad.qu_it() << "]" << setw(8) << &rad.qu_it() - rad.map1qu_data() << endl;
				}
			}
			cout << endl;
			continue;
		}
		for(int na = 0; na <= la; ++na)
		{
			for(int nb = ( rad.is_mapping_mid() ? lb : 0 ), N = na + nb; nb <= lb; ++nb, ++N)
			{
				for(int lmb_a = (l < na ? (l+na)%2 : (l-na)); lmb_a <= l+na; lmb_a += 2)
				{
					for(int lmb_b = (l < nb ? (l+nb)%2 : (l-nb)); lmb_b <= l+nb; lmb_b += 2)
					{
						if( rad.is_mapping_mid() )
							lmb_b = 0;
						rad.map1qu_dat_set( N, lmb_a, lmb_b );
						cout << setw(4) << na << setw(4) << nb <<
							setw(4) << N << setw(4) << lmb_a <<
							setw(4) << lmb_b << setw(8) << rad.qu_idx() <<
							" [" << &rad.qu_it() << "]" << setw(8) << &rad.qu_it() - rad.map1qu_data() << endl;
						if( rad.is_mapping_mid() ) break;
					}
				}
			}
		}
		cout << endl;
	}
	return 0;
}

int main()
{
	return demo_qu_map();
}
