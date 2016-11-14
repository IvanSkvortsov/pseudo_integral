#include"pseudo.struct.h"
#include<iostream>
#include<iomanip>
#include<fstream>
#include<vector>
#include"vector.3d.h"
#include"mpreal.h"
#include"ixs.angular.idx.h"
#include"pseudo.task.01.read.h"
#include"pseudo.task.01.write.h"

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
int demo_pseudo( char const * file, const int task_num = 1 )
{
	pseudo_struct<T,U> pseud;
	_3_centers<T> _geoms;
	_3_alphas<T> _alps;
	_3_momenta _angs;
	if( task_num == 1 )
	{
		const char file_task_01[] = "file.inp";
		ifstream inp( file_task_01 );
		if( !inp.is_open() )
		{
			cerr << "Error: [demo_pseudo] can't open file '" << file_task_01 << "'" << endl;
			return 1;
		}
		_geoms.read( inp );
		_alps.read( inp );
		_angs.read( inp );
		inp.close();
		const char file_basis[] = "basis.txt";
		const char file_geom[] = "geom.txt";
		const char file_ecp[] = "ecp.txt";
		pseudo_task_01_write_geom<T>( file_geom, _geoms );
		pseudo_task_01_write_basis<T>( file_basis, _alps, _angs );
		pseudo_task_01_write_ecp<T>( file_ecp, _alps, _angs );

		to_compute_set_a_xyz( _angs.ax );
		to_compute_set_b_xyz( _angs.bx );
	}
	pseud.run_pseudo( file );
	if( pseud.is_mapping_mid() )
		pseud.ixs_dat.test_ang_mid();
	else if( pseud.is_mapping_max() )
		pseud.ixs_dat.test_ang_max( pseud.alp_s );
	pseud.comp_pseudo();
	pseud.qu_dat.test_print( std::clog );

	if( (pseud.to_flip() && pseud.is_mapping_mid()) )
	{
		_geoms.flip();
		_alps.flip();
		_angs.flip();
	}

	pseudo_index idx;
	const int * ax = _angs.ax;
	const int * bx = _angs.bx;
	idx.set_la( _angs.la );
	idx.set_axyz( ax[0], ax[1], ax[2] );
	idx.set_lb( _angs.lb );
	idx.set_bxyz( bx[0], bx[1], bx[2] );
	ax = idx.ax;
	bx = idx.bx;
	pseud.print_cx();
	_geoms.print( std::clog );
	_alps.print( std::clog );
	_angs.print( std::clog );

	std::clog << "la = " << idx.get_la() << ", ax = " << ax[0] << ", ay = " << ax[1] << ", az = " << ax[2] << std::endl;
	std::clog << "lb = " << idx.get_lb() << ", bx = " << bx[0] << ", by = " << bx[1] << ", bz = " << bx[2] << std::endl;

	std::clog << "to_compute:" << std::endl;
	std::clog << "la = " << to_compute::get_la() <<
		", ax = " << to_compute::get_ax() << ", ay = " << to_compute::get_ay() << ", az = " << to_compute::get_az() << std::endl;
	std::clog << "lb = " << to_compute::get_lb() <<
		", bx = " << to_compute::get_bx() << ", by = " << to_compute::get_by() << ", bz = " << to_compute::get_bz() << std::endl;

	std::clog << "pseudo alphas" << std::endl;
	std::clog << "alp_a: " << std::setw(25) << pseud.pseudo.get_alp_a( idx ) << std::endl;
	std::clog << "alp_b: " << std::setw(25) << pseud.pseudo.get_alp_b( idx ) << std::endl;
	std::clog << "alp_c: " << std::setw(25) << pseud.pseudo.get_alp_c( idx ) << std::endl;

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
