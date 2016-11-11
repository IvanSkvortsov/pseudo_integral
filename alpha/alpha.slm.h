#ifndef __ALPHA_SLM_H__
#define __ALPHA_SLM_H__
#include<vector>
#include"alpha.pow.h"
#include"matrix.slm.h"

#define __ALPHA_SLM_TYPENAMESTR
#ifdef  __ALPHA_SLM_TYPENAMESTR
  #include"type.name.str.h"
#endif

#define __ALPHA_SLM_DEBUG
//#define __ALPHA_SLM_LOG
//#define __ALPHA_SLM_TIME

template<typename T, typename U>
struct alpha_slm : public std::vector<U>, public alpha_pow<T,U>
{
protected:
	inline void error(const char * _method, const char _message[] =	"nothing to do here")const
	{
		std::cerr << "Error: [" << this << "] alpha_slm<T,U>::" << _method << ", " << _message <<
#ifdef  __ALPHA_SLM_TYPENAMESTR
			" [with T = " << type_name_str<T>() << "; U = " << type_name_str<U>() << "]" <<
#endif
			std::endl;
	}
	int _map2slm_it_lx;
public:
	alpha_slm();
	alpha_slm( alpha_slm<T,U> const & );
	alpha_slm<T,U> & operator=( alpha_slm<T,U> const & );

	void assign_mx2slm( alpha_pow<T,U> const & palp );
	void init_mxslm( U const * CA, U const * A_alp, U const * CB, U const * B_alp, matrix_slm<U> const & mx_slm );
	void init_mx2slm( U const * CA, U const * A_alp, U const * CB, U const * B_alp, matrix_slm<U> const & mx_slm );

	inline U const * mx2slm()const{ return this->std::vector<U>::data() + this->map2slm_idx(); }
	inline void map2slm_set_lmb( int lx ){ this->_map2slm_it_lx = lx * lx + lx;}

	inline U const & mx2slm(int m)const{ return this->std::vector<U>::operator[](this->map2slm_idx() + this->_map2slm_it_lx + m); }
	inline U const & mx2slm(int lx, int mx)const{ return this->std::vector<U>::operator[]( this->map2slm_idx() + lx * lx + lx + mx); }
	// Usage : see 'alpha.map.h'
};

#endif//__ALPHA_SLM_H__
