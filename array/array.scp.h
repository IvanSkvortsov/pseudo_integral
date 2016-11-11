#ifndef __ARRAY_SCP_H__
#define __ARRAY_SCP_H__

#define __ERROR_MSG_ARRAY_SCP

template<class T>
struct array_scp
{
	T * _data;
	int _size, _capacity;
	array_scp();
	array_scp(array_scp<T> const & v);
	array_scp(int const & __size);
	array_scp(T const * __data, int const & __size);
	virtual ~array_scp();
	array_scp<T> & operator=(array_scp<T> const & v);
	T * data();
	T const * data()const;
	T & operator[](int i);
	T const & operator[](int i)const;
	int const & size()const;
	int const & capacity()const;
	void append(T const & v);
	void resize(int __size);
	void reserve(int __capacity);
};

#endif//__ARRAY_SCP_H__
