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

protected://用于类型转化
	static bool _numb_to_numb(VALUE & g, GVT pt);
	static bool _numb_to_char(VALUE & g, GVT pt);
	static bool _char_to_char(VALUE & g, GVT pt);
	static bool _char_to_numb(VALUE & g, GVT pt);
protected:
	static bool changeTypeFromNumberArray(VALUE & g, GVT pt);
	static bool changeTypeFromNumber(VALUE & g, GVT pt);
	static bool changeTypeFromCharArray(VALUE & g, GVT pt);
	static bool changeTypeFromChar(VALUE & g, GVT pt);
public:
	/*
	GVT_INT8->GVT_INT16->GVT_INT32->GVT_INT64
	GVT_UINT8->GVT_UINT16->GVT_UINT32->GVT_UINT64
	GVT_FLOAT32->GVT_FLOAT64
	GVT_CHAR->GVT_WCHAR
	GVT_DATE->GVT_DATETIME	
	*/
	static bool changeType(VALUE & g, GVT pt);	
	static double toNumber(VALUE & g);
	static int toNumber(VALUE & g, double ** da, int * n);	
	static int compare(VALUE &  v1, VALUE &v2);
public:
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

end_gdb_namespace
end_gtl_namespace


