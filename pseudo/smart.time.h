#include"get.work.time.h"
#include<cstdio>

inline const char * smart_time( get_work_time const & t )
{
	static char buf[256];
	snprintf( buf, 256, "%14.4e, sec", t.get_realseconds() );
	return buf;
}
