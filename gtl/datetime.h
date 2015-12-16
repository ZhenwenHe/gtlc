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
	typename ALLOC=std::allocator<ELEMENT> >
class DateTime:public Object<ELEMENT,TRAITS,ALLOC>{
	::tm _time;
public:
	virtual std::basic_string<ELEMENT,TRAITS,ALLOC> getClassName(){
		return "DateTime";
	}
	virtual void writeBuffer(void* & buffer, size_t & count){
		
	}
	virtual void readBuffer(const void *  buffer, size_t count=0){

	}
	virtual Object * clone() {
		return 0;
	}
public:                                   
	 
	inline ::time_t get() {		return ::mktime(&_time);	}
	inline int getYear() const {return _time.tm_year+1900;}
	inline int getMonth() const {return _time.tm_mon+1;}
	inline int getDay() const {return _time.tm_mday;}
	inline int getHour() const {return _time.tm_hour;}
	inline int getMinute() const {return _time.tm_min;}
	inline int getSecond() const {return _time.tm_sec;} 

	DateTime(const std::basic_string<ELEMENT,TRAITS,ALLOC> & timestring /*2009-11-18T10:16:07Z*/){
		std::basic_string<ELEMENT,TRAITS,ALLOC> sz = timestring.substr(0,4);
		trimLeft<char>(sz,'0');
		_time.tm_year = stringToNumber<int,char>(sz);
		_time.tm_year-=1900;

		sz = timestring.substr(5,2);
		trimLeft<char>(sz,'0');
		_time.tm_mon = stringToNumber<int,char>(sz)-1;

		sz = timestring.substr(8,2);
		trimLeft<char>(sz,'0');
		_time.tm_mday = stringToNumber<int,char>(sz);

		sz = timestring.substr(11,2);
		trimLeft<char>(sz,'0');
		if(sz.empty())
			_time.tm_hour=0;
		else
			_time.tm_hour = stringToNumber<int,char>(sz);

		sz = timestring.substr(14,2);
		trimLeft<char>(sz,'0');
		if(sz.empty())
			_time.tm_min=0;
		else
			_time.tm_min = stringToNumber<int,char>(sz);

		sz = timestring.substr(17,2);
		trimLeft<char>(sz,'0');
		if(sz.empty())
			_time.tm_sec=0;
		else
			_time.tm_sec = stringToNumber<int,char>(sz);
	}

	/*2009-11-18-10-16-07*/
	inline std::basic_string<ELEMENT,TRAITS,ALLOC> toString() 
	{
		std::basic_string<ELEMENT,TRAITS,ALLOC> sz = numberToString<char,long>(_time.tm_year+1900);

		if(_time.tm_mon+1<10) sz+= "-0";
		else
			sz+= "-";
		sz+=numberToString<char,long>(_time.tm_mon+1);

		if(_time.tm_mday<10) sz+= "-0";
		else
			sz+= "-";		
		sz+=numberToString<char,long>(_time.tm_mday);

		if(_time.tm_hour<10) sz+= "-0";
		else
			sz+= "-";		
		sz+=numberToString<char,long>(_time.tm_hour);

		if(_time.tm_min<10) sz+= "-0";
		else
			sz+= "-";		
		sz+=numberToString<char,long>(_time.tm_min);

		if(_time.tm_sec<10) sz+= "-0";
		else
			sz+= "-";		
		sz+=numberToString<char,long>(_time.tm_sec); 

		return sz;
	}
public:
	DateTime(){
		time_t now ;
		::time(&now);	 
		::localtime_s(&_time,&now); 
	}

	static DateTime  getCurrentTime(){
		DateTime t;
		return t;
	}
};
end_gtl_namespace
