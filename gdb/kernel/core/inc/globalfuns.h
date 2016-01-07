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
#include "config.h"



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

	for (std::vector<T>::iterator it = vv.begin(); it != vv.end() - 1; it++){
		for (std::vector<T>::iterator it2 = it + 1; it2 != vv.end(); it2++){
			if (*it == *it2)
				return true;
		}
	}

	return false;
};


/** 字符串裁剪函数
* @param [in,out] str 字符串对象
* @param [in] c 指定的裁剪字符
* @return 无
*/
template<typename CharT>
void trim(std::basic_string<CharT>  & str, CharT c)
{
	//删除掉两端的指定字符
	std::basic_string<CharT>::size_type pos = str.find_last_not_of(c);
	if (pos != std::basic_string<CharT>::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(c);
		if (pos != std::basic_string<CharT>::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());
	//删除中间的指定字符
	pos = str.find_first_of(c);
	while (pos != std::basic_string<CharT>::npos){
		str.erase(str.begin() + pos);
		pos = str.find_first_of(c);
	}
}

/** 将数字转换成字符串
* @param [in,out] num   需要转换的数字
@return 转换得到的字符串
*/
template<typename CharT, typename NumericT>
std::basic_string<CharT> numberToString(NumericT num)
{
	std::basic_ostringstream<CharT> oss;
	oss << (NumericT)num;
	if (oss.str().find_first_of(',') != std::basic_string<CharT>::npos){//含有格式符号
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, ',');
		return sz;
	}
	/*else if (oss.str().find_first_of('，') != std::basic_string<CharT>::npos){
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, '，');
		return sz;
	}*/
	else{
		return oss.str();
	}
}

/** 字符串转换成数字
* @param [in,out] str 字符串
* @return 转换得到的数字
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const std::basic_string<CharT> &str)
{

	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}
/** 将字符串转换成数字
@param [in,out] str 字符串指针
@return 转换得到的数字
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const CharT * str)
{
	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}
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

/** @} */


end_gdb_namespace
end_gtl_namespace



