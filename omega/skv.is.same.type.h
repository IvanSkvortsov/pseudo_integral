#ifndef __SKV_IS_SAME_TYPE_H__
#define __SKV_IS_SAME_TYPE_H__

namespace skv
{
	template<typename T, typename U>
	struct is_same_type
	{
		static const bool value;
	};
	template<typename T, typename U> const bool is_same_type<T,U>::value = false;

	template<typename T>
	struct is_same_type<T,T>
	{
		static const bool value;
	};
	template<typename T> const bool is_same_type<T,T>::value = true;
}

#endif//__SKV_IS_SAME_TYPE_H__
