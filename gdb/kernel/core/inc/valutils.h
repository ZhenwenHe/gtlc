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
/*
 TYPE          NAME      SIZE       C/C++                 RANGE
GVT_BOOL       bool      1          bool
GVT_INT8       int8      1          signed char
GVT_INT16      int16     2          short
GVT_INT32      int32     4          int
GVT_INT64      int64     8          long long
GVT_UINT8      uint8     1          unsigned char
GVT_UINT16     uint16    2          unsigned short
GVT_UINT32     uint32    4          unsigned int
GVT_UINT64     uint64    8          unsigned long long
GVT_FLOAT32    float32   4          float                 -3.40E+38 ~ +3.40E+38
GVT_FLOAT64    float64   8          double                -1.79E+308 ~ +1.79E+308
GVT_DATE       date      4*3=12     int [3]
GVT_DATETIME   datetime  4*7=28     int [7]
GVT_CHAR8      char8     1-2GB      char
GVT_CHAR16     char16    2-2GB      wchar_t
*/
// general value type
enum GVT {
	GVT_UNKN = 0x0000,   //未知类型

	GVT_BEGIN_BOOLEAN = 0x1000,
	GVT_BOOL,// bool
	GVT_END_BOOLEAN,

	//开始数字型类型的定义,0b 0000 0000 0000 0000 0000 0001 0000 0000,
	GVT_BEGIN_NUMBER = 0x2000,
	//开始定义有符号的整数类型
	GVT_BEGIN_INTEGER = 0x2100,
	GVT_BEGIN_SIGNED_INTEGER = 0x2110,
	GVT_INT8, //signed char
	GVT_INT16,  //2个字节整数
	GVT_SHORT = GVT_INT16,
	GVT_INT32,//4个字节整数
	GVT_INT = GVT_INT32,
	GVT_INT64,//
	GVT_LONGLONG = GVT_INT64,
	GVT_END_SIGNED_INTEGER,
	//开始定义无符号的整数类型
	GVT_BEGIN_UNSIGNED_INTEGER = 0x2120,
	GVT_UINT8,   //字节整数类型
	GVT_BYTE = GVT_UINT8,//和UCHAR等价
	GVT_UCHAR = GVT_UINT8,
	GVT_UINT16,
	GVT_USHORT = GVT_UINT16,
	GVT_UINT32,
	GVT_UINT = GVT_UINT32,
	GVT_UINT64,//unsigned long long 8bytes
	GVT_ULONGLONG = GVT_UINT64,
	GVT_END_UNSIGNED_INTEGER,
	GVT_END_INTEGER,
	//开始定义浮点数类型
	GVT_BEGIN_DECIMAL = 0x2200,
	GVT_FLOAT32,  //4个字节浮点数
	GVT_FLOAT = GVT_FLOAT32,
	GVT_FLOAT64,//8个字节浮点数
	GVT_DOUBLE = GVT_FLOAT64,
	GVT_END_DECIMAL,
	GVT_END_NUMBER,//开始数字型类型的定义

	GVT_BEGIN_TIME = 0x3000,
	GVT_DATE,//YYYY-MM-DD,3 integer numbers
	GVT_DATETIME,//YYYY-MM-DD HH:MM:SS 0000(millisecond)，7 integer numbers
	GVT_END_TIME,

	GVT_BEGIN_CHAR = 0x4000,//
	GVT_CHAR8,   //字符类型
	GVT_CHAR16,   //宽字符类型
	GVT_END_CHAR,


	GVT_MAX   //最大类型值
};
//general value struct
struct VALUE {
	GVT       type;//值类型
	int       count;//值的个数
	union {
		bool               bv;//bool
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
		char               c8;
		wchar_t            c16;
		void *             pv;//值内存块
	    /* 以下变量主要为了访问方便*/
		char *             pc8;
		wchar_t *          pc16;
		unsigned char *    pu8;
		int*               pi32;
		unsigned int*      pu32;
		long long*         pi64;
		unsigned long long*pu64;
		float             *pf32;
		double            *pf64;
		bool *            *pb;
	};
};

/** @defgroup value ValUtils
*  @{
*/
class CORE_API ValUtils{
public:
	/*
	GVT_INT8->GVT_INT16->GVT_INT32->GVT_INT64
	GVT_UINT8->GVT_UINT16->GVT_UINT32->GVT_UINT64
	GVT_FLOAT32->GVT_FLOAT64
	GVT_CHAR8->GVT_CHAR16
	GVT_DATE->GVT_DATETIME	
	*/
	static bool changeType(VALUE & g, GVT pt);	
	/*系统采用的排序规则：（参考SQLITE3的规定）
	1、UNKNOWN和NULL存储类型是最低的类型。一个具有NULL存储类型的值比任何其他的值都小，在NULL值之间没有具体的排序规则。
	2、BOOL类型，高于NULL，true>false;
	3、NUMBER类型，包好整数和浮点数，高于BOOL存储类型，它们之间类值相等，内部通过数值比较。
	4、TIME时间类型，高于NUMBER存储类型，两个时间类型之间，按照年月日数值的大小比较，越靠近时间零点，值越小。
	5、字符类型，高于TIME存储类型，数值永远低于字符类型。两个字符类型之间直接比较字符串的方式进行。
	6、数组类型(非字符串)具有最高的类值，相当于BLOB，比较内存块大小。
	*/
	static int compare(VALUE &  v1, VALUE &v2);
public:
	static bool get(VALUE & g, std::vector<long long> & v);
	static bool get(VALUE & g, std::vector<unsigned long long> & v);
	static bool get(VALUE & g, std::vector<double> & v);
	static bool get(VALUE & g, double & v);
	static bool get(VALUE & g, long long & v);
	static bool get(VALUE & g, unsigned long long & v);
	static bool get(VALUE & g, std::string & sz);
	static bool get(VALUE & g, std::wstring & sz);
	static bool set(VALUE & g, std::vector<long long> & newvals, GVT newtype);
	static bool set(VALUE & g, std::vector<unsigned long long> & newvals, GVT newtype);
	static bool set(VALUE & g, std::vector<double> & newvals, GVT newtype);
	static bool set(VALUE & g, long long  newvals, GVT newtype);
	static bool set(VALUE & g,  unsigned long long  newvals, GVT newtype);
	static bool set(VALUE & g,  double  newvals, GVT newtype);
	static bool set(VALUE & g, const char * sz);
	static bool set(VALUE & g, const wchar_t* wsz);
	static bool set(VALUE & g, std::string & sz);
	static bool set(VALUE & g, std::wstring& wsz);
public:
	static std::string toString(VALUE & g);
	static std::wstring toWString(VALUE & g);
	static void write(Buffer & bs, VALUE & g);
	static void read(Buffer & bs, VALUE & g);
	static void write(std::ostream & f, VALUE & g);
	static void read(std::istream & f, VALUE & g);
	static void initial(VALUE & v);
	static VALUE * newValue();
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
	static bool isText(VALUE & g);
	static bool isBlob(VALUE & g);
	static bool isBoolean(GVT t);
	static bool isBoolean(VALUE & g);
	static bool isTime(VALUE & g);
	static bool isTime(GVT t);
	static int getTypeSize(GVT t);
	static std::string getTypeName(GVT t);
	static std::string getCTypeName(GVT t);
	static std::string getTypeName(GVT t,std::vector<std::string>& names);
	static int getType(const char * gvt_typename);
	static int getSize(const VALUE & v);
	static void copy(VALUE * g, const VALUE & v);
public:
	static void initialChar(VALUE * pv, char * cc, int n = 1);
	static void initialChar(VALUE * pv, char  cc);
	static void initialWChar(VALUE * pv, wchar_t * cc, int n = 1);
	static void initialWChar(VALUE * pv, wchar_t  cc);

	static void initialInt8(VALUE * pv, signed char * cc, int n = 1);
	static void initialInt8(VALUE * pv, signed char  cc);
	static void initialUInt8(VALUE * pv, unsigned char * cc, int n = 1);
	static void initialUInt8(VALUE * pv, unsigned char  cc);	
	static void initialInt16(VALUE * pv, short * cc, int n = 1);
	static void initialInt16(VALUE * pv, short  cc);
	static void initialUInt16(VALUE * pv, unsigned short * cc, int n = 1);
	static void initialUInt16(VALUE * pv, unsigned short  cc);
	static void initialInt32(VALUE * pv, int * cc, int n = 1);
	static void initialInt32(VALUE * pv, int  cc);
	static void initialUInt32(VALUE * pv, unsigned int * cc, int n = 1);
	static void initialUInt32(VALUE * pv, unsigned int  cc);
	static void initialInt32(VALUE * pv, long * cc, int n = 1);
	static void initialInt32(VALUE * pv, long  cc);
	static void initialUInt32(VALUE * pv, unsigned long * cc, int n = 1);
	static void initialUInt32(VALUE * pv, unsigned long cc);
	static void initialUInt64(VALUE * pv, unsigned long long * cc, int n );
	static void initialUInt64(VALUE * pv, unsigned long long c);

	static void initialFloat(VALUE * pv, float * cc, int n = 1);
	static void initialFloat(VALUE * pv, float  cc);
	static void initialDouble(VALUE * pv, double * cc, int n = 1);
	static void initialDouble(VALUE * pv, double  cc);

	static void initialBoolean(VALUE * pv, bool * cc, int n = 1);
	static void initialBoolean(VALUE * pv, bool  cc);

	static void initialDate(VALUE * pv, int year, int month, int day);
	static void initialDateTime(VALUE * pv, int year, int month, int day, int hour, int minute, int second, int millisecond);
protected:
	static int compareBetweenSameTypes(VALUE &  v1, VALUE &v2);
	static int compareBetweenDifferentTypes(VALUE &  v1, VALUE &v2);
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
bool  resetValue(VALUE & g, std::vector<T> & newval, GVT newtype) {
	if (newval.size() == 1)	{
		return resetValue<T>(g, newval[0], newtype);
	}
	ValUtils::clear(g);
	switch (newtype) {
	case  GVT::GVT_CHAR8:
	{
		g.count = newval.size();
		g.type = GVT::GVT_CHAR8;
		char * temp = new char[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (char)(*it);
		}		
		break;
	}
	case  GVT::GVT_CHAR16:
	{
		g.count = newval.size();
		wchar_t * temp = new wchar_t[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (wchar_t)(*it);
		}
		g.type = GVT::GVT_CHAR16;
		break;
	}
	case  GVT::GVT_INT8:
	{
		g.count = newval.size();
		signed char * temp = new signed char[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (signed char)(*it);
		}
		g.type = GVT::GVT_INT8;
		break;
	}
	case  GVT::GVT_INT16:
	{
		g.count = newval.size();
		short * temp = new short[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (short)(*it);
		}
		g.type = GVT::GVT_INT16;
		break;
	}
	case  GVT::GVT_INT32:
	{
		g.count = newval.size();
		int * temp = new int[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = GVT::GVT_INT32;
		break;
	}
	case  GVT::GVT_INT64:
	{
		g.count = newval.size();
		long long * temp = new long long[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (long long)(*it);
		}
		g.type = GVT::GVT_INT64;
		break;
	}
	case  GVT::GVT_UINT8:
	{
		g.count = newval.size();
		unsigned char * temp = new unsigned char[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned char)(*it);
		}
		g.type = GVT::GVT_UINT8;
		break;
	}
	case  GVT::GVT_UINT16:
	{
		g.count = newval.size();
		unsigned short * temp = new unsigned short[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned short)(*it);
		}
		g.type = GVT::GVT_UINT16;
		break;
	}
	case  GVT::GVT_UINT32:
	{
		g.count = newval.size();
		unsigned int * temp = new unsigned int[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned int)(*it);
		}
		g.type = GVT::GVT_UINT32;
		break;
	}
	case  GVT::GVT_UINT64:
	{
		g.count = newval.size();
		unsigned long long * temp = new unsigned long long[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (unsigned long long)(*it);
		}
		g.type = GVT::GVT_UINT64;
		break;
	}
	case  GVT::GVT_FLOAT32:
	{
		g.count = newval.size();
		float * temp = new float[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (float)(*it);
		}
		g.type = GVT::GVT_FLOAT32;
		break;
	}
	case  GVT::GVT_FLOAT64://f64
	{
		g.count = newval.size();
		double * temp = new double[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (double)(*it);
		}
		g.type = GVT::GVT_FLOAT64;
		break;
	}
	case  GVT::GVT_BOOL:
	{
		g.count = newval.size();
		bool * temp = new bool[g.count];
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = ((*it) == 0) ? false : true;
		}
		g.type = GVT::GVT_BOOL;
		break;
	}
	case  GVT::GVT_DATE:
	{
		int tc = newval.size() / 3;
		if (newval.size() % 3 == 0)
			g.count = tc;
		else
			g.count = tc + 1;
		int * temp = new int[g.count * 3];
		memset(temp, 0, g.count * 3*sizeof(int));
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = GVT::GVT_DATE;
		break;
	}
	case  GVT::GVT_DATETIME:
	{
		int tc = newval.size() / 7;
		if (newval.size() % 7 == 0)
			g.count = tc;
		else
			g.count = tc + 1;
		int * temp = new int[g.count * 7];
		memset(temp, 0, g.count * 7*sizeof(int));
		g.pv = (void*)temp;
		for (auto it = newval.begin(); it != newval.end(); it++, temp++) {
			*temp = (int)(*it);
		}
		g.type = GVT::GVT_DATETIME;
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
bool  resetValue(VALUE & g, T & newval, GVT newtype) {
	ValUtils::clear(g);
	switch (newtype)
	{
	case GVT::GVT_CHAR8:
	{
		g.count = 1;
		g.c8 = (char)(newval);
		g.type = GVT::GVT_CHAR8;
		break;
	}
	case  GVT::GVT_CHAR16:
	{
		g.count = 1;
		g.c16 = (wchar_t)(newval);
		g.type = GVT::GVT_CHAR16;
		break;
	}
	case  GVT::GVT_INT8:
	{
		g.count = 1;
		g.i8 = (char)(newval);
		g.type = GVT::GVT_INT8;
		break;
	}
	case  GVT::GVT_INT16:
	{
		g.count = 1;
		g.i16 = (short)(newval);
		g.type = GVT::GVT_INT16;
		break;
	}
	case  GVT::GVT_INT32:
	{
		g.count = 1;
		g.i32 = (int)(newval);
		g.type = GVT::GVT_INT32;
		break;
	}
	case  GVT::GVT_INT64:
	{
		g.count = 1;
		g.i64 = (long long)(newval);
		g.type = GVT::GVT_INT64;
		break;
	}
	case  GVT::GVT_UINT8:
	{
		g.count = 1;
		g.u8 = (unsigned char)(newval);
		g.type = GVT::GVT_UINT8;
		break;
	}
	case  GVT::GVT_UINT16:
	{
		g.count = 1;
		g.u16 = (unsigned short)(newval);
		g.type = GVT::GVT_UINT16;
		break;
	}
	case  GVT::GVT_UINT32:
	{
		g.count = 1;
		g.u32 = (unsigned int)(newval);
		g.type = GVT::GVT_UINT32;
		break;
	}
	case  GVT::GVT_UINT64:
	{
		g.count = 1;
		g.u64 = (unsigned long long)(newval);
		g.type = GVT::GVT_UINT64;
		break;
	}
	case  GVT::GVT_FLOAT32:
	{
		g.count = 1;
		g.f32 = (float)(newval);
		g.type = GVT::GVT_FLOAT32;
		break;
	}
	case  GVT::GVT_FLOAT64://f64
	{
		g.count = 1;
		g.f64 = (double)(newval);
		g.type = GVT::GVT_FLOAT64;
		break;
	}
	case  GVT::GVT_BOOL:
	{
		g.count = 1;
		g.bv = (bool)(newval == 0 ? false : true);
		g.type = GVT::GVT_BOOL;
		break;
	}
	case  GVT::GVT_DATE:
	{
		g.count = 1;
		int * temp = new int[g.count * 3];
		memset(temp, 0, g.count * 3*sizeof(int));
		g.pv = (void*)temp;
		*temp = (int)(newval);
		g.type = GVT::GVT_DATE;
		break;
	}
	case  GVT::GVT_DATETIME:
	{
		g.count = 1;
		int * temp = new int[g.count * 7];
		memset(temp, 0, g.count * 7*sizeof(int));
		g.pv = (void*)temp;
		*temp = (int)(newval);
		g.type = GVT::GVT_DATETIME;
		break;
	}
	default:
		return false;
	}
	return true;
}


template<typename T> //T = double, long long, unsigned long long
bool  extractValue(VALUE & g, std::vector<T> & v)
{
	int n = g.count;
	int i = 0;

	switch (g.type)
	{
	case GVT::GVT_CHAR8://c8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((char*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.c8);
		}
		break;
	}
	case GVT::GVT_CHAR16://c16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((wchar_t*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.c16);
		}
		break;
	}
	case GVT::GVT_INT8://i8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((char*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.i8);
		}
		break;
	}
	case GVT::GVT_INT16://i16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((short*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.i16);
		}
		break;
	}
	case GVT::GVT_INT32://i32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((int*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.i32);
		}
		break;
	}
	case GVT::GVT_INT64://i64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((long long*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.i64);
		}
		break;
	}
	case GVT::GVT_UINT8://u8
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned char*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.u8);
		}
		break;
	}
	case GVT::GVT_UINT16://u16
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned short*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.u16);
		}
		break;
	}
	case GVT::GVT_UINT32://u32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned int*)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.u32);
		}
		break;
	}
	case GVT::GVT_UINT64://u64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((unsigned long long *)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.u64);
		}
		break;
	}
	case GVT::GVT_FLOAT32://f32
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((float *)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.f32);
		}
		break;
	}
	case GVT::GVT_FLOAT64://f64
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((double *)(g.pv))[i]);
			}
		}
		else {
			v[0] = (T)(g.f64);
		}
		break;
	}
	case GVT::GVT_BOOL://bv 
	{
		v.resize(n);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i] = (T)(((bool *)(g.pv))[i] ? 1 : 0);
			}
		}
		else {
			v[0] = (T)(g.bv ? 1 : 0);
		}
		break;
	}
	case GVT::GVT_DATE://DATE 
	{
		v.resize(n * 3);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i * 3] = (T)(((int *)(g.pv))[i * 3]);
				v[i * 3 + 1] = (T)(((int *)(g.pv))[i * 3 + 1]);
				v[i * 3 + 2] = (T)(((int *)(g.pv))[i * 3 + 2]);
			}
		}
		else {
			v[0] = (T)(g.date[0]);
			v[1] = (T)(g.date[1]);
			v[2] = (T)(g.date[2]);
		}
		break;
	}
	case GVT::GVT_DATETIME://DATETIME 
	{
		v.resize(n * 7);
		if (g.count > 1) {
			for (i = 0; i < n; i++) {
				v[i * 7] = (T)(((int *)(g.pv))[i * 7]);
				v[i * 7 + 1] = (T)(((int *)(g.pv))[i * 7 + 1]);
				v[i * 7 + 2] = (T)(((int *)(g.pv))[i * 7 + 2]);
				v[i * 7 + 3] = (T)(((int *)(g.pv))[i * 7 + 3]);
				v[i * 7 + 4] = (T)(((int *)(g.pv))[i * 7 + 4]);
				v[i * 7 + 5] = (T)(((int *)(g.pv))[i * 7 + 5]);
				v[i * 7 + 6] = (T)(((int *)(g.pv))[i * 7 + 6]);
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
bool  extractValue(VALUE & g, T & v)
{
	if (g.count > 1)
		return false;
	switch (g.type)
	{
	case GVT::GVT_CHAR8://c8
	{
		v = (T)(g.c8);
		break;
	}
	case GVT::GVT_CHAR16://c16
	{
		v = (T)(g.c16);
		break;
	}
	case GVT::GVT_INT8://i8
	{
		v = (T)(g.i8);
		break;
	}
	case GVT::GVT_INT16://i16
	{
		v = (T)(g.i16);
		break;
	}
	case GVT::GVT_INT32://i32
	{
		v = (T)(g.i32);
		break;
	}
	case GVT::GVT_INT64://i64
	{
		v = (T)(g.i64);
		break;
	}
	case GVT::GVT_UINT8://u8
	{
		v = (T)(g.u8);
		break;
	}
	case GVT::GVT_UINT16://u16
	{
		v = (T)(g.u16);
		break;
	}
	case GVT::GVT_UINT32://u32
	{
		v = (T)(g.u32);
		break;
	}
	case GVT::GVT_UINT64://u64
	{
		v = (T)(g.u64);
		break;
	}
	case GVT::GVT_FLOAT32://f32
	{
		v = (T)(g.f32);
		break;
	}
	case GVT::GVT_FLOAT64://f64
	{
		v = (T)(g.f64);
		break;
	}
	case GVT::GVT_BOOL://bv 
	{
		v = (T)(g.bv ? 1 : 0);
		break;
	}
	case GVT::GVT_DATE://DATE 
	case GVT::GVT_DATETIME://DATETIME 
		return false;
	default:
		return false;
	}
	return true;
}

end_gdb_namespace
end_gtl_namespace


