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
#include <cstdlib>
#include "buffer.h"



begin_gtl_namespace
begin_gdb_namespace


/** @defgroup value ValUtils
*  @{
*/
class CORE_API ValUtils{
public:

	enum GVT{
		GVT_UNKN = 0,   //未知类型

		GVT_BEGIN_CHAR,
		GVT_CHAR,   //字符类型
		GVT_WCHAR,   //字符类型
		GVT_END_CHAR,

		GVT_BEGIN_NUMBER,//开始数字型类型的定义

		GVT_BEGIN_INTEGER,
		GVT_BEGIN_SIGNED_INTEGER,
		GVT_INT8, //signed char
		GVT_INT16,  //2个字节整数
		GVT_SHORT=GVT_INT16,
		GVT_INT32,//4个字节整数
		GVT_INT = GVT_INT32,
		GVT_INT64,//
		GVT_LONGLONG = GVT_INT64,
		GVT_END_SIGNED_INTEGER,

		GVT_BEGIN_UNSIGNED_INTEGER,
		GVT_UINT8,   //字节整数类型
		GVT_BYTE= GVT_UINT8,//和UCHAR等价
		GVT_UCHAR = GVT_UINT8, 
		GVT_UINT16,
		GVT_USHORT= GVT_UINT16, 
		GVT_UINT32,
		GVT_UINT = GVT_UINT32,
		GVT_UINT64,//unsigned long long 8bytes
		GVT_ULONGLONG=GVT_UINT64,
		GVT_END_UNSIGNED_INTEGER,
		GVT_END_INTEGER,

		GVT_BEGIN_DECIMAL,
		GVT_FLOAT32,  //4个字节浮点数
		GVT_FLOAT= GVT_FLOAT32,
		GVT_FLOAT64,//8个字节浮点数
		GVT_DOUBLE = GVT_FLOAT64,
		GVT_END_DECIMAL,

		GVT_END_NUMBER,//开始数字型类型的定义

		GVT_BEGIN_TIME,

		GVT_DATE,//YYYY-MM-DD,3 integer numbers

		GVT_DATETIME,//YYYY-MM-DD HH:MM:SS 0000(millisecond)，7 integer numbers

		GVT_END_TIME,

		GVT_BEGIN_BOOLEAN,

		GVT_BOOL,// bool

		GVT_END_BOOLEAN,

		GVT_MAX   //最大类型值
	};

	struct VALUE{
		GVT       type;//值类型
		int       count;//值的个数
		union {
			void *             pvalue;//值内存块
			char *             carray;
			wchar_t *          wcarray;
			unsigned char *    barray;
			char               c8;
			wchar_t            c16;
			signed char        i8;
			unsigned char      u8;
			short              i16;
			unsigned short     u16;			
			int                i32;
			unsigned int       u32;
			long long          i64;
			unsigned long long u64;
			float              f32;
			double             f64;
			int *              date;//3 integer numbers
			int *              datetime;// 7 integer numbers
			bool               boolval;//bool
		};
		
	};

public:
	/*
	GVT_INT8->GVT_INT16->GVT_INT32->GVT_INT64
	GVT_UINT8->GVT_UINT16->GVT_UINT32->GVT_UINT64
	GVT_FLOAT32->GVT_FLOAT64
	GVT_CHAR->GVT_WCHAR
	GVT_DATE->GVT_DATETIME	
	*/
	static bool changeType(VALUE & g, GVT pt);	
	static int compare(VALUE &  v1, VALUE &v2);
	static bool getNumbers(VALUE & g, std::vector<long long> & v);
	static bool getNumbers(VALUE & g, std::vector<unsigned long long> & v);
	static bool getNumbers(VALUE & g, std::vector<double> & v);
	static bool getNumber(VALUE & g, double & v);
	static bool getNumber(VALUE & g, long long & v);
	static bool getNumber(VALUE & g, unsigned long long & v);
	static bool reset(VALUE & g, std::vector<long long> & newvals, GVT newtype);
	static bool reset(VALUE & g, std::vector<unsigned long long> & newvals, GVT newtype);
	static bool reset(VALUE & g, std::vector<double> & newvals, GVT newtype);
	static bool reset(VALUE & g, long long  newvals, GVT newtype);
	static bool reset(VALUE & g,  unsigned long long  newvals, GVT newtype);
	static bool reset(VALUE & g,  double  newvals, GVT newtype);
public:
	static std::string toString(VALUE & g);
	static std::wstring toWString(VALUE & g);
	static void write(Buffer & bs, VALUE & g);
	static void read(Buffer & bs, VALUE & g);
	static void initial(VALUE & v);
	static void clear(VALUE & g);
public:
	static bool isDecimal(GVT pt);
	static bool isInteger(GVT pt); 
	static bool isNumber(GVT pt);
	static bool isChar(GVT pt);
	static bool isDecimal(VALUE & g);
	static bool isInteger(VALUE & g);
	static bool isNumber(VALUE & g);
	static bool isChar(VALUE & g);
	static bool isArray(VALUE & g);
	static bool isBoolean(GVT t);
	static bool isBoolean(VALUE & g);
	static bool isTime(VALUE & g);
	static bool isTime(GVT t);
	static int getTypeSize(GVT t);
	static int getSize(const VALUE & v);
	static void copy(VALUE * g, const VALUE & v);
public:
	static void initialChar(VALUE * pv, char * cc, int n = 1);
	static void initialChar(VALUE * pv, char  cc);
	static void initialInt8(VALUE * pv, signed char * cc, int n = 1);
	static void initialInt8(VALUE * pv, signed char  cc);
	static void initialUChar(VALUE * pv, unsigned char * cc, int n = 1);
	static void initialUChar(VALUE * pv, unsigned char  cc);
	static void initialWChar(VALUE * pv, wchar_t * cc, int n = 1);
	static void initialWChar(VALUE * pv, wchar_t  cc);
	static void initialShort(VALUE * pv, short * cc, int n = 1);
	static void initialShort(VALUE * pv, short  cc);
	static void initialUShort(VALUE * pv, unsigned short * cc, int n = 1);
	static void initialUShort(VALUE * pv, unsigned short  cc);
	static void initialInt(VALUE * pv, int * cc, int n = 1);
	static void initialInt(VALUE * pv, int  cc);
	static void initialUInt(VALUE * pv, unsigned int * cc, int n = 1);
	static void initialUInt(VALUE * pv, unsigned int  cc);
	static void initialLong(VALUE * pv, long * cc, int n = 1);
	static void initialLong(VALUE * pv, long  cc);
	static void initialULong(VALUE * pv, unsigned long * cc, int n = 1);
	static void initialULong(VALUE * pv, unsigned long cc);
	static void initialULongLong(VALUE * pv, unsigned long long * cc, int n );
	static void initialULongLong(VALUE * pv, unsigned long long c);
	static void initialFloat(VALUE * pv, float * cc, int n = 1);
	static void initialFloat(VALUE * pv, float  cc);
	static void initialDouble(VALUE * pv, double * cc, int n = 1);
	static void initialDouble(VALUE * pv, double  cc);
	static void initialBoolean(VALUE * pv, bool * cc, int n = 1);
	static void initialBoolean(VALUE * pv, bool  cc);
	static void initialDate(VALUE * pv, int year, int month, int day);
	static void initialDateTime(VALUE * pv, int year, int month, int day, int hour, int minute, int second, int millisecond);

	static void stringToWString(std::string & s, std::wstring & sd);
	//只有处于同一编码方式的情况下才是正确的，如果编码体系不同，则本函数不能正确的执行
	static void wstringToString(std::wstring & s, std::string & sd);

	static bool doubleEqual(double d1, double d2);
	static bool floatEqual(float d1, float d2);

};
/** @} */

/*
T = char, wchar_t, unsigned char, short,unsigned short,
    int , unsigned int, 
	long long, unsigned long long,
	float, double
	bool
*/
template<typename T> 
bool  resetValue(ValUtils::VALUE & g, std::vector<T> & newval, ValUtils::GVT newtype) {
	if (newval.size() == 1)	{
		return resetValue<T>(g, newval[0], newtype);
	}
	ValUtils::clear(g);
	switch (newtype) {
	case  ValUtils::GVT::GVT_CHAR:
	{
		g.count = newval.size();
		g.type = ValUtils::GVT::GVT_CHAR;
		char * temp = new char[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (char)(*it);
		}		
		break;
	}
	case  ValUtils::GVT::GVT_WCHAR:
	{
		g.count = newval.size();
		wchar_t * temp = new wchar_t[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (wchar_t)(*it);
		}
		g.type = ValUtils::GVT::GVT_WCHAR;
		break;
	}
	case  ValUtils::GVT::GVT_INT8:
	{
		g.count = newval.size();
		signed char * temp = new signed char[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (signed char)(*it);
		}
		g.type = ValUtils::GVT::GVT_INT8;
		break;
	}
	case  ValUtils::GVT::GVT_INT16:
	{
		g.count = newval.size();
		short * temp = new short[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (short)(*it);
		}
		g.type = ValUtils::GVT::GVT_INT16;
		break;
	}
	case  ValUtils::GVT::GVT_INT32:
	{
		g.count = newval.size();
		int * temp = new int[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = ValUtils::GVT::GVT_INT32;
		break;
	}
	case  ValUtils::GVT::GVT_INT64:
	{
		g.count = newval.size();
		long long * temp = new long long[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (long long)(*it);
		}
		g.type = ValUtils::GVT::GVT_INT64;
		break;
	}
	case  ValUtils::GVT::GVT_UINT8:
	{
		g.count = newval.size();
		unsigned char * temp = new unsigned char[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned char)(*it);
		}
		g.type = ValUtils::GVT::GVT_UINT8;
		break;
	}
	case  ValUtils::GVT::GVT_UINT16:
	{
		g.count = newval.size();
		unsigned short * temp = new unsigned short[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned short)(*it);
		}
		g.type = ValUtils::GVT::GVT_UINT16;
		break;
	}
	case  ValUtils::GVT::GVT_UINT32:
	{
		g.count = newval.size();
		unsigned int * temp = new unsigned int[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned int)(*it);
		}
		g.type = ValUtils::GVT::GVT_UINT32;
		break;
	}
	case  ValUtils::GVT::GVT_UINT64:
	{
		g.count = newval.size();
		unsigned long long * temp = new unsigned long long[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned long long)(*it);
		}
		g.type = ValUtils::GVT::GVT_UINT64;
		break;
	}
	case  ValUtils::GVT::GVT_FLOAT32:
	{
		g.count = newval.size();
		float * temp = new float[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (float)(*it);
		}
		g.type = ValUtils::GVT::GVT_FLOAT32;
		break;
	}
	case  ValUtils::GVT::GVT_FLOAT64://f64
	{
		g.count = newval.size();
		double * temp = new double[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (double)(*it);
		}
		g.type = ValUtils::GVT::GVT_FLOAT64;
		break;
	}
	case  ValUtils::GVT::GVT_BOOL:
	{
		g.count = newval.size();
		bool * temp = new bool[g.count];
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = ((*it) == 0) ? false : true;
		}
		g.type = ValUtils::GVT::GVT_BOOL;
		break;
	}
	case  ValUtils::GVT::GVT_DATE:
	{
		int tc = newval.size() / 3;
		if (newval.size() % 3 == 0)
			g.count = tc;
		else
			g.count = tc + 1;
		int * temp = new int[g.count * 3];
		memset(temp, 0, g.count * 3*sizeof(int));
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = ValUtils::GVT::GVT_DATE;
		break;
	}
	case  ValUtils::GVT::GVT_DATETIME:
	{
		int tc = newval.size() / 7;
		if (newval.size() % 7 == 0)
			g.count = tc;
		else
			g.count = tc + 1;
		int * temp = new int[g.count * 7];
		memset(temp, 0, g.count * 7*sizeof(int));
		g.pvalue = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = ValUtils::GVT::GVT_DATETIME;
		break;
	}
	default:
		return false;
	}
	return true;
}
/*
T = char, wchar_t, unsigned char, short,unsigned short,
int , unsigned int,
long long, unsigned long long,
float, double
bool
*/
template<typename T>  
bool  resetValue(ValUtils::VALUE & g, T & newval, ValUtils::GVT newtype) {
	ValUtils::clear(g);
	switch (newtype)
	{
	case ValUtils::GVT::GVT_CHAR:
	{
		g.count = 1;
		g.c8 = (char)(newval);
		g.type = ValUtils::GVT::GVT_CHAR;
		break;
	}
	case  ValUtils::GVT::GVT_WCHAR:
	{
		g.count = 1;
		g.c16 = (wchar_t)(newval);
		g.type = ValUtils::GVT::GVT_WCHAR;
		break;
	}
	case  ValUtils::GVT::GVT_INT8:
	{
		g.count = 1;
		g.i8 = (char)(newval);
		g.type = ValUtils::GVT::GVT_INT8;
		break;
	}
	case  ValUtils::GVT::GVT_INT16:
	{
		g.count = 1;
		g.i16 = (short)(newval);
		g.type = ValUtils::GVT::GVT_INT16;
		break;
	}
	case  ValUtils::GVT::GVT_INT32:
	{
		g.count = 1;
		g.i32 = (int)(newval);
		g.type = ValUtils::GVT::GVT_INT32;
		break;
	}
	case  ValUtils::GVT::GVT_INT64:
	{
		g.count = 1;
		g.i64 = (long long)(newval);
		g.type = ValUtils::GVT::GVT_INT64;
		break;
	}
	case  ValUtils::GVT::GVT_UINT8:
	{
		g.count = 1;
		g.u8 = (unsigned char)(newval);
		g.type = ValUtils::GVT::GVT_UINT8;
		break;
	}
	case  ValUtils::GVT::GVT_UINT16:
	{
		g.count = 1;
		g.u16 = (unsigned short)(newval);
		g.type = ValUtils::GVT::GVT_UINT16;
		break;
	}
	case  ValUtils::GVT::GVT_UINT32:
	{
		g.count = 1;
		g.u32 = (unsigned int)(newval);
		g.type = ValUtils::GVT::GVT_UINT32;
		break;
	}
	case  ValUtils::GVT::GVT_UINT64:
	{
		g.count = 1;
		g.u64 = (unsigned long long)(newval);
		g.type = ValUtils::GVT::GVT_UINT64;
		break;
	}
	case  ValUtils::GVT::GVT_FLOAT32:
	{
		g.count = 1;
		g.f32 = (float)(newval);
		g.type = ValUtils::GVT::GVT_FLOAT32;
		break;
	}
	case  ValUtils::GVT::GVT_FLOAT64://f64
	{
		g.count = 1;
		g.f64 = (double)(newval);
		g.type = ValUtils::GVT::GVT_FLOAT64;
		break;
	}
	case  ValUtils::GVT::GVT_BOOL:
	{
		g.count = 1;
		g.boolval = (bool)(newval == 0 ? false : true);
		g.type = ValUtils::GVT::GVT_BOOL;
		break;
	}
	case  ValUtils::GVT::GVT_DATE:
	{
		g.count = 1;
		int * temp = new int[g.count * 3];
		memset(temp, 0, g.count * 3*sizeof(int));
		g.pvalue = (void*)temp;
		*temp = (int)(newval);
		g.type = ValUtils::GVT::GVT_DATE;
		break;
	}
	case  ValUtils::GVT::GVT_DATETIME:
	{
		g.count = 1;
		int * temp = new int[g.count * 7];
		memset(temp, 0, g.count * 7*sizeof(int));
		g.pvalue = (void*)temp;
		*temp = (int)(newval);
		g.type = ValUtils::GVT::GVT_DATETIME;
		break;
	}
	default:
		return false;
	}
	return true;
}


template<typename T> //T = double, long long, unsigned long long
bool  extractValue(ValUtils::VALUE & g, std::vector<T> & v)
{
	int n = g.count;
	int i = 0;

	switch (g.type)
	{
	case ValUtils::GVT::GVT_CHAR://c8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((char*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.c8);
		}
		break;
	}
	case ValUtils::GVT::GVT_WCHAR://c16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((wchar_t*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.c16);
		}
		break;
	}
	case ValUtils::GVT::GVT_INT8://i8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((char*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.i8);
		}
		break;
	}
	case ValUtils::GVT::GVT_INT16://i16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((short*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.i16);
		}
		break;
	}
	case ValUtils::GVT::GVT_INT32://i32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((int*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.i32);
		}
		break;
	}
	case ValUtils::GVT::GVT_INT64://i64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((long long*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.i64);
		}
		break;
	}
	case ValUtils::GVT::GVT_UINT8://u8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned char*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.u8);
		}
		break;
	}
	case ValUtils::GVT::GVT_UINT16://u16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned short*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.u16);
		}
		break;
	}
	case ValUtils::GVT::GVT_UINT32://u32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned int*)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.u32);
		}
		break;
	}
	case ValUtils::GVT::GVT_UINT64://u64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned long long *)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.u64);
		}
		break;
	}
	case ValUtils::GVT::GVT_FLOAT32://f32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((float *)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.f32);
		}
		break;
	}
	case ValUtils::GVT::GVT_FLOAT64://f64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((double *)(g.pvalue))[i]);
			}
		}
		else {
			v[0] = (T)(g.f64);
		}
		break;
	}
	case ValUtils::GVT::GVT_BOOL://boolval 
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((bool *)(g.pvalue))[i] ? 1 : 0);
			}
		}
		else {
			v[0] = (T)(g.boolval ? 1 : 0);
		}
		break;
	}
	case ValUtils::GVT::GVT_DATE://DATE 
	{
		v.resize(n * 3);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i * 3] = (T)(((int *)(g.pvalue))[i * 3]);
				v[i * 3 + 1] = (T)(((int *)(g.pvalue))[i * 3 + 1]);
				v[i * 3 + 2] = (T)(((int *)(g.pvalue))[i * 3 + 2]);
			}
		}
		else {
			v[0] = (T)(g.date[0]);
			v[1] = (T)(g.date[1]);
			v[2] = (T)(g.date[2]);
		}
		break;
	}
	case ValUtils::GVT::GVT_DATETIME://DATETIME 
	{
		v.resize(n * 7);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i * 7] = (T)(((int *)(g.pvalue))[i * 7]);
				v[i * 7 + 1] = (T)(((int *)(g.pvalue))[i * 7 + 1]);
				v[i * 7 + 2] = (T)(((int *)(g.pvalue))[i * 7 + 2]);
				v[i * 7 + 3] = (T)(((int *)(g.pvalue))[i * 7 + 3]);
				v[i * 7 + 4] = (T)(((int *)(g.pvalue))[i * 7 + 4]);
				v[i * 7 + 5] = (T)(((int *)(g.pvalue))[i * 7 + 5]);
				v[i * 7 + 6] = (T)(((int *)(g.pvalue))[i * 7 + 6]);
			}
		}
		else {
			v[0] = (T)(g.datetime[0]);
			v[1] = (T)(g.datetime[1]);
			v[2] = (T)(g.datetime[2]);
			v[3] = (T)(g.datetime[3]);
			v[4] = (T)(g.datetime[4]);
			v[5] = (T)(g.datetime[5]);
			v[6] = (T)(g.datetime[6]);
		}
		break;
	}
	default:
		return false;
	}
	return true;
}


template<typename T> //T = double, long long, unsigned long long
bool  extractValue(ValUtils::VALUE & g, T & v)
{
	if (g.count > 1)
		return false;
	switch (g.type)
	{
	case ValUtils::GVT::GVT_CHAR://c8
	{
		v = (T)(g.c8);
		break;
	}
	case ValUtils::GVT::GVT_WCHAR://c16
	{
		v = (T)(g.c16);
		break;
	}
	case ValUtils::GVT::GVT_INT8://i8
	{
		v = (T)(g.i8);
		break;
	}
	case ValUtils::GVT::GVT_INT16://i16
	{
		v = (T)(g.i16);
		break;
	}
	case ValUtils::GVT::GVT_INT32://i32
	{
		v = (T)(g.i32);
		break;
	}
	case ValUtils::GVT::GVT_INT64://i64
	{
		v = (T)(g.i64);
		break;
	}
	case ValUtils::GVT::GVT_UINT8://u8
	{
		v = (T)(g.u8);
		break;
	}
	case ValUtils::GVT::GVT_UINT16://u16
	{
		v = (T)(g.u16);
		break;
	}
	case ValUtils::GVT::GVT_UINT32://u32
	{
		v = (T)(g.u32);
		break;
	}
	case ValUtils::GVT::GVT_UINT64://u64
	{
		v = (T)(g.u64);
		break;
	}
	case ValUtils::GVT::GVT_FLOAT32://f32
	{
		v = (T)(g.f32);
		break;
	}
	case ValUtils::GVT::GVT_FLOAT64://f64
	{
		v = (T)(g.f64);
		break;
	}
	case ValUtils::GVT::GVT_BOOL://boolval 
	{
		v = (T)(g.boolval ? 1 : 0);
		break;
	}
	case ValUtils::GVT::GVT_DATE://DATE 
	case ValUtils::GVT::GVT_DATETIME://DATETIME 
		return false;
	default:
		return false;
	}
	return true;
}

end_gdb_namespace
end_gtl_namespace


