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

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include <locale.h>
#include "config.h"
#include "csl.h"



begin_gtl_namespace
begin_gdb_namespace


/** @defgroup GlobalFunctions GlobalFunctions-全局数字与字符串操作函数集
*  @{
*/
/** 判断列表中是否有相同的对象存在
* @param [in] vv 对象数组
* @return bool:有相同元素则返回true，否则返回false
*/
template<typename T>
bool same(std::vector<T> & vv){
	if (vv.empty()) return false;
	size_t s = vv.size();
	if (s == 1) return false;

	for (typename std::vector<T>::iterator it = vv.begin(); it != vv.end() - 1; it++){
		for (typename std::vector<T>::iterator it2 = it + 1; it2 != vv.end(); it2++){
			if (*it == *it2)
				return true;
		}
	}

	return false;
};


/** 获取系统当前时间，得到系统时间字符串
@param [out] sz 字符串
@return 无
*/
void getCurTime(std::string & sz);

/** 获取系统当前时间，得到系统时间字符串
@param 无
@return 得到系统时间字符串
*/
std::string getCurTime();


bool doubleEqual(double d1, double d2);
bool floatEqual(float d1, float d2);


//角度转换函数
typedef double PACKED_DMS_TYPE;//DDDMMMSSS.SS
double packedDMSToDegree(PACKED_DMS_TYPE dfPacked);
double degreeToPackedDMS(double dfDec);
double DMSToDegree(const char *is);
const char *degreeToDMS(double dfAngle, const char * pszAxis, int nPrecision);


//文件读写补充函数
//读取一行，也即遇到13和10停止
const char * readLine(FILE * fp);
size_t readLine(FILE * fp, std::string & result);

/** @} */
end_gdb_namespace
end_gtl_namespace



