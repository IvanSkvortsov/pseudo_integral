#include"pseudo.struct.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include"vector.3d.h"
#include"mpreal.h"
#include"ixs.angular.idx.h"

using namespace std;

// 0. basis file
// 1. ecp file
// 2. geometry file
// 3. ixs_omega file
// 4. alpha file
// 5. angular map file
// 6. qu radial map file
// 7. matrix slm file
// 8. angular dat file
// 9. qu radial data file
template<typename T, typename U>
int demo_pseudo( char const * file )
{
	pseudo_struct<T,U> pseud;
	pseud.run_pseudo( file );
	pseud.comp_pseudo();
	pseud.qu_dat.test_print( std::clog );

	pseudo_index idx;
	idx.set_la( to_compute::get_la() );
	idx.set_axyz( to_compute::get_ax(), to_compute::get_ay(), to_compute::get_az() );
	idx.set_lb( to_compute::get_lb() );
	idx.set_bxyz( to_compute::get_bx(), to_compute::get_by(), to_compute::get_bz() );
	const int * ax = idx.ax;
	const int * bx = idx.bx;
	std::clog << "la = " << idx.get_la() << ", ax = " << ax[0] << ", ay = " << ax[1] << ", az = " << ax[2] << std::endl;
	std::clog << "lb = " << idx.get_lb() << ", bx = " << bx[0] << ", by = " << bx[1] << ", bz = " << bx[2] << std::endl;

	T v, s = 0;
	for(int l = 0; l < pseud.alp_mem.l_max(); ++l)
	{
		idx.set_l( l );
		idx.set_ia_p( 0 );
		idx.set_ib_p( 0 );
		idx.set_ic_p( 0 );
		v = pseud.pseudo.get_prim( idx );
		s += v;
		std::clog << "prim: l = " << std::setw(4) << idx.get_l() <<
			std::setw(25) << std::setprecision(15) << std::scientific << v <<
			std::setw(25) << std::setprecision(15) << std::scientific << s <<
			std::endl;
	}
	idx.set_l( pseud.alp_mem.l_max() );
	idx.set_ia_p( 0 );
	idx.set_ib_p( 0 );
	idx.set_ic_p( 0 );
	v = pseud.pseudo.get_prim( idx );
	std::clog << "prim: l = " << std::setw(4) << idx.get_l() <<
		std::setw(25) << std::setprecision(15) << std::scientific << v <<
		std::endl;

	return 0;
}

int main( int argc, char ** argv )
{
	if( argc != 2 )
	{
		cerr << "Error: [main] usage './main.exe file.inp'" << endl;
		return 1;
	}
	//mpfr::mpreal::set_default_prec( mpfr::digits2bits(24) );
	//return demo_pseudo<double, mpfr::mpreal>( argv[1] );
	//return demo_pseudo<double, long double>( argv[1] );
	return demo_pseudo<double, double>( argv[1] );
}
