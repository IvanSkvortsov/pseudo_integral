#ifndef __GET_WORK_TIME_H__
#define __GET_WORK_TIME_H__
#include<ctime>// clock_gettime, timespec, clockid_t, CLOCK_MONOTONIC, ...
#include<iostream>// cerr, endl

typedef unsigned long long int ull_t;
typedef unsigned long int ul_t;
typedef unsigned int u_t;

/************************************************
 *
 * This class counts work time of procedure
 * Usage:
 *
 * 1. create object get_work_time with one of following clock_ID:
 * 1.1. CLOCK_REALTIME
 * 1.2. CLOCK_MONOTONIC ( default )
 * 1.3. CLOCK_PROCESS_CPUTIME_ID
 * 1.4. CLOCK_THREAD_CPUTIME_ID
 *
 * 2. start time counting: 
 * void get_work_time::counting_start()
 *
 * 3. finish counting:
 * void get_work_time::counting_end()
 *
 * 4. get time in seconds ( long double value ): 
 * double get_work_time::get_realseconds()
 *
 * P.S. the essence of this routine is the usage of clock_gettime( clockid_t, timespec * ) function of time.h library
 * timespec - is structure which containes two fields: first is time in seconds (tv_sec) and second is a rest of time in nanoseconds (tv_nsec)
 * so that the sum ( tv_sec + tv_nsec * 10^(-9) ) is total time;
 *
************************************************/
class get_work_time
{
private:
	timespec _start_time, _end_time;
	clockid_t _clock_ID;
	bool _is_counter_started;
	bool _is_counter_complete;
public:
	inline get_work_time( clockid_t __clock_ID = CLOCK_MONOTONIC )
	//inline get_work_time( clockid_t __clock_ID = CLOCK_REALTIME )
	{
		this->_is_counter_started = false;
		this->_is_counter_complete = false;

		this->set_clock_ID( __clock_ID );
	}
	inline ~get_work_time(){};
	inline void set_clock_ID( clockid_t __clock_ID = CLOCK_REALTIME )
	{
		this->_clock_ID = __clock_ID;
	}
	inline void counting_start()
	{
		if( !this->_is_counter_started )
		{
			this->_is_counter_started = true;
			this->_is_counter_complete = false;

			clock_gettime( this->_clock_ID, &this->_start_time );
		}
		else
		{
			std::cerr << " * Warning: counter is already started" << std::endl;
		}
	}
	inline void counting_end()
	{
		if( this->_is_counter_started )
		{
			clock_gettime( this->_clock_ID, &this->_end_time );

			this->_is_counter_started = false;
			this->_is_counter_complete = true;
		}
		else
		{
			std::cerr << " * Warning: can't complete counter which hasn't been started" << std::endl;
		}
	}
	inline ull_t get_nanoseconds( timespec const & time )const
	{
		return time.tv_sec * 1000000000ull + time.tv_nsec; 
	}
	inline ul_t get_seconds()const
	{
		if( this->_is_counter_complete )
		{
			return this->_end_time.tv_sec - this->_start_time.tv_sec;
		}
		else
		{
			std::cerr << " * Error: in order to get time of work you should start and then complete time counting" << std::endl;
		}
	}
	inline ull_t get_nanoseconds()const
	{
		if( this->_is_counter_complete )
		{
			return this->get_nanoseconds( this->_end_time ) - this->get_nanoseconds( this->_start_time );
		}
		else
		{
			std::cerr << " * Error: in order to get time of work you should start and then complete time counting" << std::endl;
		}
	}
	inline double get_realseconds()const
	{
		return this->get_nanoseconds() / 1000000000.;
	}
};

#endif//__GET_WORK_TIME_H__
