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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <limits>
#include <cmath>
#include "identifier.h"
#include "valutils.h"



begin_gtl_namespace
begin_gdb_namespace

/** @addtogroup value Value 
*  @{
*/
class CORE_API Value
 {
protected:	
	VALUE * m_data;
	/*程序运行期间Value对象销毁的时候，_need_release指示该内存释放是否需要释放，不参与对象存储*/
	bool _need_release;
public:
	typedef VALUE value_type;
public:	
	Value();
	Value(const char * sz);
	Value(const wchar_t * sz);
	Value(signed char i8);
	Value(short i16);
	Value(int i32);
	Value(long long i64);
	Value(unsigned char u8);
	Value(unsigned short u16);
	Value(unsigned int u32);
	Value(unsigned long long u64);
	Value(float f32);
	Value(double f64);
	Value(bool b);
	Value(int year, int month, int day);
	Value(int year, int month, int day,int hour, int minute, int second, int millisecond=0);
	Value(int* i32s,int n);
	Value(long long * i64s,int n);
	Value(unsigned char * u8s,int n); 
	Value(unsigned int * u32s,int n);
	Value(unsigned long long * u64s,int n);
	Value(float * f32s,int n);
	Value(double* f64s,int n);
	Value(VALUE * pv, bool auto_release = true);
	Value(const Value & v);	
	virtual ~Value();

	void clear();
	void attach( VALUE * pv);
	VALUE * detach() ;
	void copy(const Value & v);
	void copy(const VALUE & v);
	virtual void write(Buffer & bs);
	virtual void read(Buffer & bs);
public:
	Value operator = (const Value & v);
	Value operator = (char v);
	Value operator = (unsigned char v);
	Value operator = (short  v);
	Value operator = (unsigned short v);
	Value operator = (int v);
	Value operator = (long v);
	Value operator = (unsigned long v);
	Value operator = (unsigned long long v);
	Value operator = ( Identifier & v);
	Value operator = (float v);
	Value operator = (double v);
	Value operator = (string v);
	Value operator = (const char *  v);
public:
	 inline operator double() { double d; extractValue<double>(*m_data, d); return d; }
	 inline double asDouble() { double d; extractValue<double>(*m_data, d); return d; }
	 inline float asFloat() { float d; extractValue<float>(*m_data, d); return d; }
	 inline int asInt32() { int d; extractValue<int>(*m_data, d); return d; }
	 inline long long asInt64() { long long d; extractValue<long long>(*m_data, d); return d; }
	 inline unsigned long long asUInt64() { unsigned long long d; extractValue<unsigned long long>(*m_data, d); return d; }
	 inline std::string asString() { return ValUtils::toString(*m_data); }
	 inline std::wstring asWString() { return ValUtils::toWString(*m_data); }
public:
	inline int getTypeSize( ){	return ValUtils::getTypeSize(m_data->type);	}
	inline long getCount(){return m_data->count;}
	inline long getSize(){ return m_data->count * getTypeSize();}
	inline VALUE & get(){ return *m_data;}	
	inline bool isDecimal(){return ValUtils::isDecimal(m_data->type);}
	inline bool isInteger(){return ValUtils::isInteger(m_data->type);}
	inline bool isNumber(){return ValUtils::isNumber(m_data->type);	}
	inline bool isChar(){return ValUtils::isChar(m_data->type);	}
	inline bool isArray(){return ValUtils::isArray(*m_data);	}	
	inline int compare( Value & v)  {return ValUtils::compare(*m_data,*v.m_data);	}
};
/** @} */ 

end_gdb_namespace
end_gtl_namespace


