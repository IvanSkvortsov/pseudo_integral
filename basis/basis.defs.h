#ifndef __BASIS_DEFS_H__
#define __BASIS_DEFS_H__

#define __POINTER_INTERFACE( method, source_ptr, type )\
inline void set_##method##_data( type * __ptr){ (source_ptr) = __ptr;}\
inline const type & get_##method(int i)const{ return (source_ptr)[i];}\
inline const type * get_##method##_data(int i)const{ return (source_ptr) + i;}\
inline const type * get_##method##_data()const{ return (source_ptr);}\
inline type & get_##method(int i){ return (source_ptr)[i];}\
inline type * get_##method##_data(int i){ return (source_ptr) + i;}\
inline type * get_##method##_data(){ return (source_ptr);}

#define __REFERENCE_INTERFACE( method, source, type )\
inline void set_##method( type __value ){ (source) = __value; }\
inline const type & get_##method()const{ return (source);}\
inline type & get_##method(){ return (source);}

#define __STL_VECTOR_INTERFACE( method, source, type )\
inline type * get_##method##_data(){ return (source).data(); }\
inline type const * get_##method##_data()const{ return (source).data(); }\
inline type * get_##method##_data(int i){ return (source).data() + i; }\
inline type const * get_##method##_data(int i)const{ return (source).data() + i; }\
inline type & get_##method(int i){ return (source)[i]; }\
inline type const & get_##method(int i)const{ return (source)[i]; }\
inline void set_##method##_size(int __size){ (source).resize( __size ); }\
inline int const get_##method##_size()const{ return (source).size(); }

#endif//__BASIS_DEFS_H__
