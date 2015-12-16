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
 #pragma once
#include "object.h"
#include "time.h"
begin_gtl_namespace
template<
	typename ELEMENT=char,
	typename TRAITS=std::char_traits<ELEMENT>,
	typename ALLOC=std::allocator<ELEMENT> 
>
class  Timer : public Object<ELEMENT,TRAITS,ALLOC>{
	std::vector<double> _times;
	std::vector<std::basic_string<ELEMENT,TRAITS,ALLOC> > _descriptions;
	clock_t _start,_end;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		std::basic_string<ELEMENT,TRAITS,ALLOC> sz;
		return sz;
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
    virtual Object<ELEMENT,TRAITS,ALLOC> * clone() {
		return 0;
	}
public:
	inline void start(const ELEMENT * sz){
		_start=clock(); 
		_descriptions.push_back(std::basic_string<ELEMENT,TRAITS,ALLOC>(sz));
	}
	inline void end(){
		_end=clock(); 
		_times.push_back((double)(_end-_start)/CLK_TCK);
	}
	inline double total(){
		double s=0;
		for(std::vector<double>::const_iterator it = _times.cbegin();it!=_times.cend();it++)
			s+=(*it);
		return s;
	}
	inline void write(const ELEMENT * szfile){
		std::ofstream ofs(szfile);
        std::vector<std::basic_string<ELEMENT,TRAITS,ALLOC> >::iterator sz=_descriptions.begin();

		for(std::vector<double>::iterator it=_times.begin();it!=_times.end();it++,sz++){
			ofs<<*sz<<" "<<*it<<std::endl;
		}
		ofs.close();
	}
	inline void write(std::basic_ostream<ELEMENT,TRAITS> & ofs){

        std::vector<std::basic_string<ELEMENT,TRAITS,ALLOC> >::iterator sz=_descriptions.begin();

		for(std::vector<double>::iterator it=_times.begin();it!=_times.end();it++,sz++){
			ofs<<*sz<<" "<<*it;
		}

	}
	inline void clear() {
		_times.clear();
		_descriptions.clear();
		_start=0;
		_end=0;
	}
public:
	static void random(double rs, double re, double & s, double & e) {
		//srand(_seed++);
		s=rand()*(re-rs)/RAND_MAX;
		e=rand()*(re-rs)/RAND_MAX;
		while(s>=e){
			s=rand()*(re-rs)/RAND_MAX;
			e=rand()*(re-rs)/RAND_MAX;
		}
	}
};

end_gtl_namespace
