#ifndef __MEMORY_MAP_H__
#define __MEMORY_MAP_H__

//#include<stdlib.h>// exit
//#include<stdio.h>// printf, fprintf, FILE
#include<fcntl.h>// open, close, O_RDWR, O_RDONLY, O_WRONLY, O_CREAT, ..., S_IRWXU, S_IRWXG, S_IRWXO, ...
//#include<unistd.h>
//#include<sys/stat.h>// stat, fstat
//#include<sys/mman.h>// mmap, munmap
//#include<string.h>// memset

//#define __MEMORY_MAP_LOG

//#ifdef  __MEMORY_MAP_LOG
#include<stdio.h>// printf, fprintf, FILE
//#endif

class memory_map
{
public:
	typedef off_t off_type;
	typedef mode_t mode_type;
	static const mode_type MODE_644 = S_IRUSR| S_IWUSR| S_IRGRP| S_IROTH;
	static const mode_type MODE_444 = S_IRUSR| S_IRGRP| S_IROTH;
private:
	memory_map( memory_map const & );
	memory_map & operator=( memory_map const & );
protected:
	//struct stat _stat;
	int _fd, _flags;
	mode_t _mode;
	off_t _size;
	void * _data;
#ifdef  __MEMORY_MAP_LOG
	inline void log(FILE * __fp, const char * __method, const char __message[] = "")const;
#endif
	inline void err(FILE * __fp, const char * __method, const char __message[] = "nothing to do here")const;
public:
	memory_map();
	memory_map(char const * file, const int __flags = O_RDWR, const mode_t __mode = (S_IRUSR | S_IWUSR | S_IRGRP | S_IROTH));
	~memory_map();
	void clear();
	const int fd()const;
	const int flags()const;
	const mode_t mode()const;
	const off_t size()const;
	void * data();
	void const * data()const;

	int create(char const * file, const off_t __size, int __flags = O_RDWR|O_CREAT|O_TRUNC, mode_t __mode = S_IRUSR |S_IWUSR |S_IRGRP |S_IROTH);
	int open(char const * file, int __flags = O_RDONLY, mode_t __mode = S_IRUSR | S_IRGRP | S_IROTH);
	int close();
private:
	void zeroize();
	int file_open(char const * file, int __flags, mode_t __mode);
};

#ifdef  __MEMORY_MAP_LOG
void memory_map::log(FILE * __fp, const char * __method, const char * __message)const
{
	fprintf(__fp, "Log: [%p] memory_map::%s %s\n", this, __method, __message);
}
#endif
void memory_map::err(FILE * __fp, const char * __method, const char * __message)const
{
	fprintf(__fp, "Error: [%p] memory_map::%s, %s\n", this, __method, __message);
}
#endif//__MEMORY_MAP_H__
