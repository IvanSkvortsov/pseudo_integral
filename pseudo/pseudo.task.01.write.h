#ifndef __PSEUDO_TASK_01_WRITE_H__
#define __PSEUDO_TASK_01_WRITE_H__
#include"pseudo.task.01.read.h"

template<typename T> void pseudo_task_01_write_geom( const char * file, _3_centers<T> const & geom );
template<typename T> void pseudo_task_01_write_basis( const char * file, _3_alphas<T> const & alps, _3_momenta const & angs );
template<typename T> void pseudo_task_01_write_ecp( const char * file, _3_alphas<T> const & alps, _3_momenta const & angs );

#endif//__PSEUDO_TASK_01_WRITE_H__
