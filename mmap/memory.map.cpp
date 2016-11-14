#include"memory.map.h"

#include<stdlib.h>// exit
#include<stdio.h>// printf, fprintf, FILE
#include<fcntl.h>// open, close, O_RDWR, O_RDONLY, O_WRONLY, O_CREAT, ..., S_IRWXU, S_IRWXG, S_IRWXO, ...
#include<unistd.h>// lseek, write, ::open, ::close
#include<sys/stat.h>// stat, fstat
#include<sys/mman.h>// mmap, munmap
#include<string.h>// memset

memory_map::memory_map():_fd(0), _flags(0), _mode(0), _size(0ull), _data(0ul)
{
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "memory_map()");
#endif
	//memset(&_stat, 0, sizeof(_stat));
}
void memory_map::zeroize()
{
	memset( this, 0, sizeof(*this) );
}
memory_map::memory_map(char const * file, const int __flags, const mode_t __mode)
{
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "memory_map(char const *, const int , const mode_t )");
#endif
	int __z = this->file_open( file, __flags, __mode);
	if( __z )
	{
		//fprintf(stderr, "Error [%p]: memory_map::memory_map(char const * , const int , const mode_t )\n", this);
		this->err(stderr, "memory_map(char cosnt * , const int , const mode_t )", "" );
	}
	else
	{
		this->_data = mmap(0, this->_size, PROT_READ | PROT_WRITE, MAP_SHARED, this->_fd, 0);
		::close( this->_fd );
	}
}
memory_map::~memory_map()
{
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "~memory_map()");
#endif
	this->clear();
}
void memory_map::clear()
{
	if( this->_size || this->_data )
		munmap(this->_data, this->_size);
	this->zeroize();
}
int memory_map::close()
{
	if( this->_size || this->_data )
	{
		munmap( this->_data, this->_size );
		this->zeroize();
		return 0;
	}
	this->zeroize();
	return 1;
}
const int memory_map::fd()const{return this->_fd;}
const int memory_map::flags()const{return this->_flags;}
const mode_t memory_map::mode()const{return this->_mode;}
const off_t memory_map::size()const{return this->_size;}
void * memory_map::data(){return this->_data;}
void const * memory_map::data()const{return this->_data;}

int memory_map::create(char const * file, const off_t __size, int __flags, mode_t __mode)
{
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "create(char const * , const off_t )");
#endif
	this->clear();
	int __z = this->file_open( file, __flags, __mode);
	if( __z )
		return __z;
	this->_size = __size;
	lseek(this->_fd, __size, SEEK_SET);
	write(this->_fd, "", 1);
	lseek(this->_fd, 0, SEEK_SET);
	this->_data = mmap(0, __size, PROT_READ | PROT_WRITE, MAP_SHARED, this->_fd, 0);
	union
	{
		void * ptr;
		int z;
	} ptr_z;
	ptr_z.z = -1;
	if( ptr_z.ptr == this->_data )
	{
		this->err(stderr, "create(char const * , int , mode_t )", "mapping failed" );
		fprintf(stderr, "data : [%p]\n", this->_data );
		fprintf(stderr, "size : %lld\n", this->_size );
		fprintf(stderr, "file : %s\n", file );
		return 1;
	}
	::close( this->_fd );
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "create(char const * , const off_t )", "SUCCESS");
	fprintf(stdout, "file : %s\n", file);
	fprintf(stdout, "data : [%p]\n", this->data());
#endif
	return 0;
}
int memory_map::open(char const * file, int __flags, mode_t __mode)
{
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "open(char const * )");
#endif
	this->clear();
	int __z = this->file_open( file, __flags, __mode);
	if( __z )
		return __z;
	this->_data = mmap(0, this->_size, PROT_READ, MAP_SHARED, this->_fd, 0);
	::close( this->_fd );
#ifdef  __MEMORY_MAP_LOG
	this->log(stdout, "open(char const * )", "SUCCESS");
	fprintf(stdout, "file : %s\n", file);
	fprintf(stdout, "data : [%p]\n", this->data());
#endif
	return 0;
}
int memory_map::file_open(char const * file, int __flags, mode_t __mode)
{
	this->_fd = ::open( file, __flags, __mode );
	if( this->_fd == -1 )
	{
		//fprintf(stderr, "Error [%p]: [memory_map::file_open(char const * , int , mode_t )] can't open file \"%s\"\n", this, file);
		this->err(stderr, "file_open(char const * , int , mode_t )", "can't open file" );
		fprintf(stderr, "file '%s'\n", file );
		return 1;
	}
	this->_flags = __flags;
	this->_mode = __mode;
	struct stat _stat;
	int __status_z = fstat( this->_fd, &_stat );// get file status( _stat ) by file descriptor( _fd )
	if( __status_z != 0 )
	{
		//fprintf(stderr, "Error [%p]: [memory_map::file_open(char const * , int , mode_t )] fstat(int, struct stat *) error\n", this);
		this->err(stderr, "file_open(char const * , int , mode_t )", "fstat(int , struct stat * ) error" );
		return 2;
	}
	this->_size = _stat.st_size;// get file size (in bytes)
	return 0;
}

void memory_map::info()const
{
	char inf_logo[64];
	snprintf( inf_logo, 256, "INF: [%p] memory_map::", this );
	fprintf( stdout, "%s%4s: %zu\n", inf_logo, "size", this->size() );
	fprintf( stdout, "%s%4s: [%p]\n", inf_logo, "data", this->data() );
	fprintf( stdout, "%s%4s: %d\n", inf_logo, "fd", this->_fd );
	fprintf( stdout, "%s%4s: %d\n", inf_logo, "flag", this->_flags );
	fprintf( stdout, "%s%4s: %d%d%d\n", inf_logo, "mode", this->_mode&0700, this->_mode&070, this->_mode&07 );
}
