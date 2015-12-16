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
#include "random.h"

begin_gtl_namespace

template<
	typename T =double,
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> >
class RandomNumbers: public std::vector<T>, public Object<ELEMENT,TRAITS, ALLOC>
{
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "RandomNumbers";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){
	}
    virtual Object<ELEMENT,TRAITS, ALLOC> * clone() {
		return 0;
	}

public:
	RandomNumbers(int n, T mind,T maxd){
		/*
		std::vector<int> vs; 
		int v=0; 
		for(int i=0;i<n;i++)
		{
		v=rand()+(i/RAND_MAX)*RAND_MAX;
		if(std::find(vs.begin(),vs.end(),v)==vs.end())
		{
		vs.push_back(v);
		}
		else
		i--;
		}
		int imin=RAND_MAX*(n/RAND_MAX+1), imax=0;
		for(std::vector<int>::iterator it = vs.begin();it!=vs.end();it++)
		{
		if(*it>imax) imax=*it;
		if(*it<imin) imin = *it;
		}
		resize(n);
		std::vector<int>::iterator iit= vs.begin();
		std::vector<double>::iterator dit = begin();
		for(;iit!=vs.end();iit++,dit++)
		{
		*dit=((*iit)-imin)*1.0/(imax-imin)*(maxd-mind)+mind;
		}
		*/
		reset(n,mind,maxd);
	}
	void reset(int n, T mind,T maxd){ 
		Random<T> r;
		clear();
		double v; 
		for(int i=0;i<n;i++)
		{
			v=r.nextDouble((double)mind,(double)maxd);
			if(std::find(begin(),end(),v)==end())
			{
				push_back(v);
			}
			else
				i--;
		}
	}

};
end_gtl_namespace
