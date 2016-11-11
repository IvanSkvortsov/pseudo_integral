#ifndef __MAPPING_TYPE_H__
#define __MAPPING_TYPE_H__

struct mapping_struct
{
	enum mapping_enum { maximum, middle, minimum};
	mapping_enum _mapping_t;

	inline mapping_struct():_mapping_t( maximum ){}
	inline mapping_struct( mapping_struct const & v):_mapping_t( v._mapping_t ){}
	inline mapping_struct( mapping_enum const & v):_mapping_t( v ){}
	inline mapping_struct & operator=( mapping_struct const & v)
	{
		this->_mapping_t = v._mapping_t;
		return *this;
	}
	inline mapping_struct & operator=( mapping_enum const & v)
	{
		this->_mapping_t = v;
		return *this;
	}

	inline void set_mapping( mapping_enum const & v ){ this->_mapping_t = v;}
	inline void set_mapping( mapping_struct const & v ){ this->_mapping_t = v._mapping_t;}
	inline void set_mapping_min(){ this->_mapping_t = mapping_enum::minimum;}
	inline void set_mapping_mid(){ this->_mapping_t = mapping_enum::middle;}
	inline void set_mapping_max(){ this->_mapping_t = mapping_enum::maximum;}
	inline mapping_enum const & get_mapping()const{ return this->_mapping_t;}
	inline const bool is_mapping_min()const{ return this->_mapping_t == mapping_enum::minimum;}
	inline const bool is_mapping_mid()const{ return this->_mapping_t == mapping_enum::middle;}
	inline const bool is_mapping_max()const{ return this->_mapping_t == mapping_enum::maximum;}
};

#endif//__MAPPING_TYPE_H__
