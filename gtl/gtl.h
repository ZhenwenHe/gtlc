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

#define begin_gtl_namespace namespace gtl{
#define end_gtl_namespace }
#define using_gtl_namespace using namespace gtl;

//the namespace of spatiotempotal data mode 
#define begin_stm_namespace namespace stm{
#define end_stm_namespace }
#define using_stm_namespace using namespace stm;

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string>
#include <vector>
#include <limits>
#include <memory>
#include <list>
#include <algorithm>
#include <functional>
#include <vector>
#include <cstdlib>			// standard C++ includes
#include <cctype>
#include <iostream>
#include <typeinfo>
#include <tuple>
#include <array>

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <fstream>
#include <sstream>
#include <assert.h>
#include <time.h>
#include "config.h"

#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include <climits>
#include <cfloat>
#include <chrono>    // std::chrono::seconds 
#include <thread>    // std::thread, std::this_thread::sleep_for
#include <atomic>
#include <map>

begin_gtl_namespace

#ifndef INITMAX
#define INITMAX -9999999999999999999.0
#endif

#ifndef INITMIN
#define INITMIN  9999999999999999999.0
#endif

#ifndef MAXPATH
#define MAXPATH 1024
#endif


#ifndef min
#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
#endif

#ifndef max
#define max(a, b)  (((a) > (b)) ? (a) : (b)) 
#endif


//#ifndef TOLERANCE //tolerance
//#define TOLERANCE  DBL_EPSILON
//#endif

static const double TOLERANCE = std::numeric_limits<double>::epsilon();

#ifndef PI 
#define PI 3.1415926535897932384626433832795	// π
#endif

#ifndef Pi 
#define Pi PI	// π
#endif

#ifndef PiOver180 
#define PiOver180 1.74532925199433E-002				// 角度化为弧度的因子
#endif

#ifndef PiUnder180 
#define PiUnder180 5.72957795130823E+001				// 弧度化为角度的因子
#endif

#ifndef M_PI_2
#define M_PI_2 1.57079632679489661922
#endif


template<typename CharT>
void trim( std::basic_string<CharT>  & str, CharT c, bool eraseMid=true/*是否删除中间的字符*/)
{
    //删除掉两端的指定字符
    typename std::basic_string<CharT>::size_type pos = str.find_last_not_of(c);
	if(pos != std::basic_string<CharT>::npos) 
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(c);
		if(pos != std::basic_string<CharT>::npos) 
			str.erase(0, pos);
	}
	else 
		str.erase(str.begin(), str.end());
	//删除中间的指定字符
	if(eraseMid){
		pos = str.find_first_of(c);
		while(pos != std::basic_string<CharT>::npos){
			str.erase(str.begin()+pos);
			pos = str.find_first_of(c);
		}
	}
}

template<typename CharT>
void trimLeft( std::basic_string<CharT>  & str, CharT c)
{
	//删除掉两端的指定字符
    typename std::basic_string<CharT>::size_type pos =  str.find_first_not_of(c);

	if(pos != std::basic_string<CharT>::npos) 
		str.erase(0, pos);
	else 
		str.erase(str.begin(), str.end());
}

template<typename CharT,typename NumericT>
std::basic_string<CharT> numberToString ( NumericT num)
{
	std::basic_ostringstream<CharT> oss;
	oss << (NumericT) num;
	if(oss.str().find_first_of(',')!=std::basic_string<CharT>::npos){//含有格式符号
		std::basic_string<CharT> sz= oss.str();
		trim<CharT>(sz,',');
		return sz;
	}
	else{
		return oss.str();
	}

}

template<typename NumericT,typename CharT>
NumericT stringToNumber(const std::basic_string<CharT> &str)
{
	if(str.find_first_of(',')!=std::basic_string<CharT>::npos){//含有格式符号
		std::basic_string<CharT> sz= str;
		trim<CharT>(sz,',') ;
		std::basic_istringstream<CharT> iss (sz);
		NumericT result;
		iss>>result;
		return result;
	}
	else{
		std::basic_istringstream<CharT> iss (str);
		NumericT result;
		iss>>result;
		return result;
	}

	
}

template<typename NumericT,typename CharT>
NumericT stringToNumber(const CharT * str)
{
	std::basic_istringstream<CharT> iss (str);
	NumericT result;
	iss>>result;
	return result;
}

template<
	typename ELEMENT = char,
	typename TRAITS = std::char_traits<ELEMENT>,
	typename ALLOC = std::allocator<ELEMENT> >
void stringToBuffer(const std::basic_string<ELEMENT, TRAITS, ALLOC> & sz, void * & buf, size_t & count)
{
	count = sz.size()*sizeof(ELEMENT)+sizeof(int);
	buf = (void*)new char[count];
	int * p = (int*)buf;
	*p = (int)count;
	++p;
	memcpy((void*)p, sz.c_str(), sz.size()*sizeof(ELEMENT));
}


template<
	typename ELEMENT = char,
	typename TRAITS = std::char_traits<ELEMENT>,
	typename ALLOC = std::allocator<ELEMENT> >
void bufferToString(const void * buf, std::basic_string<ELEMENT, TRAITS, ALLOC> & sz)
{
	int s = *((const int*)buf);
	s = s - sizeof(int);
	const ELEMENT * p = (const ELEMENT*)(((const int*)buf) + 1);
	sz.assign(p, s / sizeof(ELEMENT));
}

void * merge(void * & buf1, size_t s1, void * & buf2, size_t s2)
{
	assert(buf1 != 0 && buf2 != 0 && s1 != 0 && s2 != 0);

	char* buf = new char[s1 + s2];
	memcpy(buf, buf1, s1);
	memcpy(buf + s1, buf2, s2);
	delete[] buf1;
	delete[] buf2;
	buf1 = 0;
	buf2 = 0;

	return (void*)buf;
}
end_gtl_namespace
