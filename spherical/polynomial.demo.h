#include"polynomial.h"
#include"poly.n.h"
#include<iostream>
#include<iomanip>
using namespace std;

template<typename T, int psize>
void print_polynomial(std::ostream & out, polynomial<poly_n<T, psize> > const & polynom, int prec = 12)
{
	int w = prec + 10;
	out.setf( std::ios::scientific );
	out.precision( prec );
	poly_n<T,psize> const * ptr = &polynom[0];
	out << "size : " << std::setw(5) << polynom.size() << std::endl;
	for(int i = 0; i < polynom.size(); ++i, ++ptr)
	{
		out << std::setw(5) << i;
		for(int j = 0; j < psize; ++j)
			out << std::setw(4) << ptr->x[j];
		out << std::setw(w) << std::setprecision(prec) << ptr->d << std::endl;
	}
	out << std::endl;
}

template<typename T>
int polynomial_demo()
{
	static const int psize = 3, size = 13;
	polynomial<poly_n<T,psize> > polynom;
	polynom.resize( size );
	poly_n<T,psize> pol;
	pol.d = 1.2;
	int * x = pol.x;
	for(int i = 0; i < psize; ++i)
		x[i] = i;
	polynom *= pol;
	polynom[size/2] *= pol;
	polynom += pol;
	print_polynomial<T>(cout, polynom );
	polynom.optimize_equal();
	polynom.optimize_zero();
	print_polynomial<T>(cout, polynom );
	pol.print(cout, 12, 5);
	return 0;
}

int polynomial_demo_d()
{
	return polynomial_demo<double>();
}
