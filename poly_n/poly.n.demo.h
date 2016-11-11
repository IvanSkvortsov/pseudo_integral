#include"poly.n.h"
#include<iomanip>

using namespace std;

std::ostream & setp(int prec)
{
	return {prec};
}

//std::_Setw const & setwidth( int width ){return std::setw(width);}

template<typename T>
int demo_polyn()
{
	poly_3<T> p3;
	poly_4<T> p4;
	p3.d = 4.5;
	p4.d = 5.5;
	cin >> std::setw(4);
	return 0;

	/*
	cout << setw(4) << "d" << " : " << setwidth(16) << setprecision(8) << scientific << p3.d << endl << flush;
	for(int i = 0; i < p3.size(); ++i)
	{
		cout << setw(4) << i << " : " <<
			setw(4) << (p3.x[i] = i) <<
			setw(4) << "3" << endl << flush;
	}
	cout << endl;
	cout << setw(4) << "d" << " : " << setw(16) << std::_Setprecision( 8 ) << scientific << p4.d << endl << flush;
	for(int i = 0; i < p4.size(); ++i)
	{
		cout << setw(4) << i << " : " <<
			setw(4) << (p4.x[i] = i) <<
			setw(4) << "4" << endl << flush;
	}
	*/
	return 0;
}

int demo_polyn_d()
{
	return demo_polyn<double>();
}
int main()
{
	return demo_polyn_d();
}

