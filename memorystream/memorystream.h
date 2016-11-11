#ifndef __MEMORYSTREAM_H__
#define __MEMORYSTREAM_H__
#include<iostream>// cerr, endl
#include<cstring>// memset, memcpy
#include<cstdlib>// exit

struct _mSetw{int _M_n;};
inline _mSetw msetw(int __w){return {__w};}

union _mPtr{void * v; char * c;};
struct _mStreambuf
{
	_mPtr _M_beg, _M_cur, _M_end;
};

// memory stream
class memorystream
{
public:
	enum seek_dir{beg,cur,end};
	typedef int pos_type;
	typedef int streamsize;
	typedef unsigned long long int off_type;
protected:
	_mPtr _M_beg, _M_cur, _M_end;
	int _M_n;

	inline void error(const char * methodname, const char message[] = "")const;
public:
	memorystream();
	memorystream( memorystream const & ms );
	memorystream( void * __beg, off_type __off);
	~memorystream();

	memorystream & operator=(memorystream const & ms);

	_mStreambuf * rdbuf(_mStreambuf * msb);
	_mStreambuf * rdbuf()const;

	inline void setbuf(void * __beg, off_type __off);
	inline char * getbuf();
	inline char const * getbuf()const;
	inline char * getcur();
	inline char const * getcur()const;

	inline void width(int __w);
	inline int width()const;
	inline pos_type tell()const;
	inline pos_type bufsize()const;
	
	inline void inline_write(void const * __f, streamsize __size);
	inline void inline_read( void * __f, streamsize __size);
	inline void inline_write_zero( streamsize __size );

	memorystream & seek(pos_type __pos );
	memorystream & seek(pos_type __pos, seek_dir __way);
	int peek();// get next character

	memorystream & write(void const * __buffer, int __buffersize);
	memorystream & read(void * __buffer, int __buffersize);
	template<typename _T> memorystream & operator>>(_T & __f);
	template<typename _T> memorystream & operator<<(_T const & __f); 

	friend memorystream & operator>>(memorystream & ms, char * __f);
	friend memorystream & operator<<(memorystream & ms, char const * __f);
};

memorystream & operator<<(memorystream & ms, _mSetw __f);
memorystream & operator>>(memorystream & ms, _mSetw __f);

void memorystream::inline_write_zero( streamsize __size )
{
	if( this->_M_cur.c + __size >= this->_M_end.c )
	{
		this->error( "inline_write_zero", "overflow" );
		exit(1);
	}
	std::memset( this->_M_cur.v, 0, __size );
	this->_M_cur.c += __size;
}
void memorystream::inline_write(void const * __f, streamsize __size)
{
	if( this->_M_cur.c + __size >= this->_M_end.c )
	{
		this->error( "inline_write", "overflow" );
		exit(1);
	}
	std::memcpy( this->_M_cur.v, __f, __size );
	this->_M_cur.c += __size;
}
void memorystream::inline_read(void * __f, streamsize __size)
{
	if( this->_M_cur.c + __size >= this->_M_end.c )
	{
		this->error( "inline_read", "overflow" );
		exit(1);
	}
	std::memcpy( __f, this->_M_cur.v, __size );
	this->_M_cur.c += __size;
}
char * memorystream::getcur()
{
	return this->_M_cur.c;
}
char const * memorystream::getcur()const
{
	return this->_M_cur.c;
}
char * memorystream::getbuf()
{
	return this->_M_beg.c;
}
char const * memorystream::getbuf()const
{
	return this->_M_beg.c;
}
memorystream::pos_type memorystream::tell()const
{
	return (this->_M_cur.c - this->_M_beg.c);
}
memorystream::pos_type memorystream::bufsize()const
{
	return (this->_M_end.c - this->_M_beg.c);
}
void memorystream::setbuf( void * __v, off_type __off)
{
	this->_M_beg = {__v};
	this->_M_cur = this->_M_beg;
	this->_M_end.c = this->_M_beg.c + __off;
	this->_M_n = 0;
}
void memorystream::width(int __w)
{
	this->_M_n = __w;
}
int memorystream::width()const
{
	return this->_M_n;
}

void memorystream::error(const char * methodname, const char * message)const
{
	std::cerr << "Error: [" << this << "] memorystream::" << methodname << ", " << message << std::endl;
}
#endif//__MEMORYSTREAM_H__
