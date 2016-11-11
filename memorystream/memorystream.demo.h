#include"memorystream.h"
#include<iomanip>
#include<vector>

using namespace std;

union mem_reveal {void * v; char * c; unsigned int * z; int i;};

void print_reveal( mem_reveal const & M, const char * name, int i_min, int i_max)
{
	const int wz = 14;
	cout << setw(4) << name << " : " << setw(wz) << "" << " [" << M.v << "]" << endl;
	for(int i = i_min; i <= i_max; ++i)
		cout << setw(4) << i << " : " << setw(wz) << M.z[i] << " [" << &M.z[i] << "]" << endl;
	cout << endl;
}

struct M_streambuf
{
	typedef char char_type;

	char_type * _M_in_beg;
	char_type * _M_in_cur;
	char_type * _M_in_end;
	char_type * _M_out_beg;
	char_type * _M_out_cur;
	char_type * _M_out_end;

	locale _M_buf_locale;

	//virtual ~M_streambuf(){}
};

#include"/home/Ivan/cpp/projects/bitprint/bitprint.hpp"
#include"/home/Ivan/cpp/projects/bitprint/smart.bits.hpp"

int demo_memorystream()
{
	const int size = 1024;
	vector<char> vc;
	vc.resize( size );
	memorystream is, os;
	is.setbuf( vc.data(), vc.size() );
	os = is;
	const char Hello_s[] = "Hello, World!", * pc = Hello_s;
	for(int i = 0; i <= strlen(Hello_s); ++i, ++pc)
		cout << setw(3) << i << " : " << setw(2) << *pc << ' ' << setw(12) << int(*pc) << ' ' << smart_bits(*pc) << endl;
	os << 1 << 5 << 1.25311 << Hello_s << 271;
	cout << "ms.tell : " << is.tell() << endl;
	int z1, z2, z3;
	double f;
	char _buf[128];
	is >> z1 >> z2 >> f;
	is >> _buf;
	pc =  _buf;
	for(int i = 0; i <= strlen(_buf); ++i, ++pc)
		cout << setw(3) << i << " : " << setw(2) << *pc << ' ' << setw(12) << int(*pc) << ' ' << smart_bits(*pc, 8) << endl;
	//is.read( _buf, std::strlen(Hello_s) );
	is >> z3;
	cout << setw(4) << z1 << endl <<
		setw(4) << z2 << endl <<
		f << endl <<
		_buf << endl <<
		setw(4) << z3 << endl;
	return 0;
}
int foo()
{
	streambuf * sbuf = cout.rdbuf();
	mem_reveal M_sbuf, M_beg, M_end, M_cur;
	M_sbuf.v = sbuf;
	M_beg.i = M_sbuf.z[0];

	print_reveal( M_sbuf, "sbuf", -1, 3 );
	print_reveal( M_beg, "beg", -2, 7 );

	M_cur.i = M_beg.z[0];
	print_reveal( M_cur, "cur", -2, 4 );
	cout << "cur.size : " << M_cur.z[1] - M_cur.z[0] << endl;

	union{void * v; streambuf * sb; M_streambuf * m_sb;} buf = {&M_beg.z[-1]};
	cout << "-----------" << endl;
	cout << "M_streambuf [" << buf.m_sb << "]" << endl << endl;
	cout << "in  size : " << setw(14) << buf.m_sb->_M_in_end - buf.m_sb->_M_in_beg << endl;
	cout << "out size : " << setw(14) << buf.m_sb->_M_out_end - buf.m_sb->_M_out_beg << endl;
	cout << "-----------" << endl;

	M_end.i = M_beg.z[1];
	print_reveal( M_end, "end", -2, 7 );

	return 0;
}

int main()
{
	return demo_memorystream();
}
