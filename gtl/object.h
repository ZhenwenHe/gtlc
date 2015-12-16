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

#include "gtl.h"
#include "bufferstream.h"

begin_gtl_namespace

template<
	typename CHARTYPE=char,
	typename TRAITS=std::char_traits<CHARTYPE>,
	typename ALLOC=std::allocator<CHARTYPE> >
class Object{
public:
	virtual std::basic_string<CHARTYPE,TRAITS,ALLOC> getClassName()=0;	
	virtual void writeBuffer(void* & buffer, size_t & count)=0;
	virtual void readBuffer(const void *  buffer, size_t count=0)=0;
	virtual Object * clone() = 0;

	virtual void write(std::basic_ostream<CHARTYPE,TRAITS> & bs){
		CHARTYPE * buffer=0;
		uint32_t  count =0;
		size_t s=0;
		writeBuffer((void*&)buffer,s);
		if(buffer && s>0){
			count = (uint32_t) s;
			bs.write((const CHARTYPE* )&count,sizeof(uint32_t));
			bs.write(buffer,count);
			delete [] buffer;
		}
	}
	virtual void read(std::basic_istream<CHARTYPE,TRAITS> & bs){		
		uint32_t  count =0;
		bs.read((CHARTYPE*)&count,sizeof(uint32_t));
		if(count==0) 
			return;
		CHARTYPE * buffer=(CHARTYPE *) new unsigned char[count];
		bs.read(buffer,count);
		readBuffer((const void*)buffer,count);
		delete [] buffer;
	}
};
end_gtl_namespace