#ifndef __SF_HYPERG_H__
#define __SF_HYPERG_H__

namespace sf_hyperg
{
	enum{ _1F1_SERIES = 1, _2F0_SERIES, _1F1_SERIES_LARGE, _FAIL };
	enum{ SUCCESS = 0, ERR_MAXITER, ERR_OVERFLOW, ERR_UNDERFLOW, ERR_DIVZERO, ERR_UNKNOWN };
	const char * type( const int n );
	const char * error( const int n );
}

inline const char * sf_hyperg::type( const int n )
{
	switch( n )
	{
	case sf_hyperg::_1F1_SERIES: return "1F1_SERIES";
	case sf_hyperg::_2F0_SERIES: return "2F0_SERIES";
	case sf_hyperg::_1F1_SERIES_LARGE: return "1F1_SERIES_LARGE";
	case sf_hyperg::_FAIL: return "FAIL";
	default: return "Error: [sf_hyperg::type( const int )]";
	}
}

inline const char * sf_hyperg::error( const int n )
{
	switch( n )
	{
	case sf_hyperg::SUCCESS: return "SUCCESS";
	case sf_hyperg::ERR_MAXITER: return "ERR_MAXITER";
	case sf_hyperg::ERR_OVERFLOW: return "ERR_OVERFLOW";
	case sf_hyperg::ERR_UNDERFLOW: return "ERR_UNDERFLOW";
	case sf_hyperg::ERR_DIVZERO: return "ERR_DIVZERO";
	case sf_hyperg::ERR_UNKNOWN: return "ERR_UNKNOWN";
	default: return "Error: [sf_hyperg::error( const int )]";
	}
}

#endif//__SF_HYPERG_H__
