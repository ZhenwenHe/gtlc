/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose. 
* It is provided "as is" without express or implied warranty.
*/
#pragma  once
#include "object.h" 
begin_gtl_namespace

template<
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class Random:public Object<ELEMENT,TRAITS,ALLOC>
{
	unsigned short __rand48_Seed[3];
	unsigned short __rand48_Mult[3] ;
	unsigned short __rand48_Add ;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		std::basic_string<ELEMENT,TRAITS,ALLOC> r;
		return r;
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	Random();
	Random(unsigned long seed, unsigned short xsubi0);
	virtual ~Random();

	long nextLong();// returns a uniformly distributed long.
	
	unsigned long nextUnsignedLong();// returns a uniformly distributed unsigned long.
	
	long nextLong(long low, long high);// returns a uniformly distributed long in the range [low, high).
	
	unsigned long nextUnsignedLong(unsigned long low, unsigned long high);// returns a uniformly distributed unsigned long in the range [low, high).
	
	long long nextLongLong();// returns a uniformly distributed long long.
	
	unsigned long long nextUnsignedLongLong();// returns a uniformly distributed unsigned long long.
	
	long long nextLongLong(long long low, long long high);// returns a uniformly distributed unsigned long long in the range [low, high).
	
	unsigned long long nextUnsignedLongLong(unsigned long long low, unsigned long long high);// returns a uniformly distributed unsigned long long in the range [low, high).
	
	short nextShort();// returns a uniformly distributed short.
	
	unsigned short nextUnsignedShort();// returns a uniformly distributed unsigned short.
	
	double nextDouble();// returns a uniformly distributed double in the range [0, 1).
	
	double nextDouble(double low, double high);// returns a uniformly distributed double in the range [low, high).
	
	bool flipCoin();

private:
	void initDrand(unsigned long seed, unsigned short xsubi0);

	unsigned short * m_pBuffer;
private:
	/* Internal function to compute next state of the generator.  */
	void		_dorand48(unsigned short xseed[3]);
	
public:
	void	srand48(long seed) ;
	void	lcong48(unsigned short p[7]) ;
	unsigned short *		seed48(unsigned short xseed[3]) ;
	long		nrand48(unsigned short xseed[3]) ;
	long		mrand48(void) ;
	long	lrand48(void) ;
	
	long	jrand48(unsigned short xseed[3]) ;
	double		erand48(unsigned short xseed[3]) ;
	double	drand48(void) ;
}; // Random

end_gtl_namespace

#include "random.inl"
