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
#include "bitset.h" 

begin_gtl_namespace 
template <
	int BASE=10,//进制,如二进制、八进制、十六进制等
	typename DIGITSL=unsigned char, //每个数字用一个字节或一个short或一个int表示
	typename T=char,
	typename TRAITS=std::char_traits<T>,
	typename ALLOC=std::allocator<T> >
class Number : public Object <T,TRAITS,ALLOC> {
public:
	virtual std::basic_string<T,TRAITS,ALLOC> getClassName(){
		return "Number";
	}
	virtual void writeBuffer(void*  & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void*  buffer, size_t count=0){
		
	}
	virtual Object * clone() {
		return 0;
	}
public:
	
};

/** @} */ 
////////////////////////////////////////////////////////////////////////////////////////////////////

end_gtl_namespace
