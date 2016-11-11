#ifndef __MEMORYSTREAM_HPP__
#define __MEMORYSTREAM_HPP__
#include"memorystream.h"

memorystream::memorystream() : _M_beg({0}), _M_cur({0}), _M_end({0}), _M_n(0) {}
//memorystream::memorystream( memorystream & ms ) : _M_beg(ms._M_beg), _M_cur(ms._M_cur), _M_end(ms._M_end), _M_n(0){}
memorystream::memorystream( memorystream const & ms ) : _M_beg(ms._M_beg), _M_cur(ms._M_cur), _M_end(ms._M_end), _M_n(0){}
memorystream::memorystream( void * __v, off_type __off ): _M_beg({__v}), _M_cur(_M_beg), _M_end({_M_beg.c + __off}), _M_n(0){}
memorystream::~memorystream(){}

memorystream & memorystream::operator=(memorystream const & ms)
{
	if( this == &ms )
		return *this;
	std::memcpy( this, &ms, sizeof(*this) );
	this->_M_n = 0;
	return *this;
}
// rdbuf
_mStreambuf * memorystream::rdbuf( _mStreambuf * sb )
{
	this->_M_beg = sb->_M_beg;
	this->_M_cur = sb->_M_cur;
	this->_M_end = sb->_M_end;
	return sb;
}
_mStreambuf * memorystream::rdbuf()const
{
	_mStreambuf * sb;
	sb->_M_beg = this->_M_beg;
	sb->_M_cur = this->_M_cur;
	sb->_M_end = this->_M_end;
	//sb->_M_beg = {const_cast<void*>(this->_M_beg.v)};
	//sb->_M_cur = {const_cast<void*>(this->_M_cur.v)};
	//sb->_M_end = {const_cast<void*>(this->_M_end.v)};
	return sb;
}
// seek
memorystream & memorystream::seek( pos_type pos )
{
	if( this->_M_beg.v )
		this->_M_cur.c = this->_M_beg.c + pos;
	return *this;
}
memorystream & memorystream::seek( pos_type pos, memorystream::seek_dir way )
{
	switch( way )
	{
		case beg :
			this->_M_cur.c = this->_M_beg.c + pos;
			break;
		case cur : 
			this->_M_cur.c += pos;
			break;
		case end : 
			this->_M_cur.c = this->_M_end.c + pos;
			break;
	}
	return *this;
}
// write
memorystream & memorystream::write( void const * __buffer, int __buffersize )
{
	if( __buffersize <= 0 )
		return *this;
	this->inline_write(__buffer, __buffersize );
	return *this;
}
// read
memorystream & memorystream::read(void * __f, int __size)
{
	if( __size < 0 )
		return *this;
	this->inline_read(__f, __size );
	return *this;
}
// operator<< : write
template<typename _T> memorystream & memorystream::operator<<(_T const & __f)
{
	if( this->width() > sizeof(_T ) )
	{
		this->inline_write( &__f, sizeof(_T) );
		this->inline_write_zero( this->width() - sizeof(_T) );
	}
	else if( this->width() > 0 )
	{
		this->inline_write( &__f, this->width() );
	}
	else
	{
		this->inline_write( &__f, sizeof(_T) );
	}
	this->width( 0 );
	return *this;
}
// operator>> : read
template<typename _T> memorystream & memorystream::operator>>(_T & __f)
{
	if( this->width() <= 0 || this->width() >= sizeof(_T) )
	{
		this->inline_read( &__f, sizeof(_T) );
	}
	else
	{
		this->inline_read( &__f, this->width() );
		_mPtr tmp = {&__f};
		tmp.c += this->width();
		std::memset( tmp.v, 0, sizeof(_T) - this->width() );
	}
	this->width(0);
	return *this;
}

memorystream & operator<<(memorystream & ms, char const * __f)
{
	ms.inline_write( __f, std::strlen(__f)+1 );
	return ms;
}
memorystream & operator>>(memorystream & ms, char * __f)
{
	ms.inline_read(__f, std::strlen(ms._M_cur.c)+1);
	return ms;
}

// msetw
memorystream & operator<<(memorystream & ms, _mSetw __f)
{
	ms.width( __f._M_n );
	return ms;
}
memorystream & operator>>(memorystream & ms, _mSetw __f)
{
	ms.width( __f._M_n );
	return ms;
}

#define MSTREAM_SPEC( type )\
	template memorystream & memorystream::operator<<(type const & __f);\
	template memorystream & memorystream::operator>>(type & __f);

#endif//__MEMORYSTREAM_HPP__
