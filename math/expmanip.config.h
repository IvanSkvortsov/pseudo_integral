#ifndef __EXPMANIP_CONFIG_H__
#define __EXPMANIP_CONFIG_H__

#define __STD_GETEXP
#define __STD_SETEXP
#define __STD_ADDEXP
#define __STD_ZEROEXP

#include"mpreal.use.h"
#ifdef  __MPREAL_USE

#define __MP_GETEXP
#define __MP_SETEXP
#define __MP_ADDEXP
#define __MP_ZEROEXP

#endif

#endif//__EXPMANIP_CONFIG_H__
