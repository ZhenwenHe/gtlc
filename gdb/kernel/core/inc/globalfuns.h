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
	//std::basic_string<CharT>::size_type pos = str.find_last_not_of(c);
	auto pos = str.find_last_not_of(c);
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
	/* 处理中文字符格式的逗号，Linux下编译不通,VC下可以编译通过，所以这段代码暂时只实现VC下的*/
#ifdef _MSC_VER
	/*else if (oss.str().find_first_of('，') != std::basic_string<CharT>::npos){
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, '，');
		return sz;
	}*/
#endif //_MSC_VER
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


void stringToWString(std::string & s, std::wstring & sd);
//只有处于同一编码方式的情况下才是正确的，如果编码体系不同，则本函数不能正确的执行
void wstringToString(std::wstring & s, std::string & sd);


bool doubleEqual(double d1, double d2);
bool floatEqual(float d1, float d2);


//角度转换函数
typedef double PACKED_DMS_TYPE;//DDDMMMSSS.SS
double packedDMSToDegree(PACKED_DMS_TYPE dfPacked);
double degreeToPackedDMS(double dfDec);
double DMSToDegree(const char *is);
const char *degreeToDMS(double dfAngle, const char * pszAxis, int nPrecision);

//C语言版本的字符串和字符串列表操作函数
typedef char** CSTRLIST;//c style StringList
#define CSLT_HONOURSTRINGS      0x0001
#define CSLT_ALLOWEMPTYTOKENS   0x0002
#define CSLT_PRESERVEQUOTES     0x0004
#define CSLT_PRESERVEESCAPES    0x0008
#define CSLT_STRIPLEADSPACES    0x0010
#define CSLT_STRIPENDSPACES     0x0020
CSTRLIST cslAddString(CSTRLIST old, const char * str);
const char * cslGetString(CSTRLIST sl, int iField);
int cslIFindString(CSTRLIST sl, const char * str);
char * cslDuplicateString(const char * s);
bool cslEqualString(const char * s1, const char * s2);
bool cslIEqualString(const char * s1, const char * s2); 
bool cslNIEqualString(const char * s1, const char * s2,int n);
size_t cslConcatenateString(char* desString, const char * srcString, size_t DesSize);
size_t cslCopyString(char* desString, const char * srcString, int DesSize);
CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int nCSLTFlags);
CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int bHonourStrings, int bAllowEmptyTokens);
CSTRLIST cslDuplicate(CSTRLIST);
int cslCount(CSTRLIST sl);
void cslDestroy(CSTRLIST sl);
double cslStringToNumber(const char *nptr, char **endptr, char point);
double cslStringToNumber(const char *nptr);
double cslMultilingualStringToNumber(const char *nptr);
char* cslReplacePointByLocalePoint(const char* pszNumber, char point); 
bool cslTestBoolean(const char * v);
CSTRLIST cslAddNameValue(CSTRLIST papszStrList, const char *pszName, const char *pszValue);
CSTRLIST cslSetNameValue(CSTRLIST papszList, const char *pszName, const char *pszValue);
const char * cslFetchNameValue(CSTRLIST papszStrList, const char *pszName);
const char *cslParseNameValue(const char *pszNameValue, char **ppszKey);
int cslIFindName(CSTRLIST papszStrList, const char *pszName);

//C++版本的扩展字符串类
typedef std::string _gtl_gdb_string;
class CORE_API String : public _gtl_gdb_string {
public:
	String(void) {}
	String(const std::string &oStr) : _gtl_gdb_string(oStr) {}
	String(const char *pszStr) : _gtl_gdb_string(pszStr) {}

	operator const char* (void) const { return c_str(); }

	char& operator[](std::string::size_type i)
	{
		return _gtl_gdb_string::operator[](i);
	}

	const char& operator[](std::string::size_type i) const
	{
		return _gtl_gdb_string::operator[](i);
	}

	char& operator[](int i)
	{
		return _gtl_gdb_string::operator[](static_cast<std::string::size_type>(i));
	}

	const char& operator[](int i) const
	{
		return _gtl_gdb_string::operator[](static_cast<std::string::size_type>(i));
	}

	void clear() { resize(0); }

	// NULL safe assign and free.
	void seize(char *pszValue)
	{
		if (pszValue == NULL)
			clear();
		else
		{
			*this = pszValue;
			free(pszValue);
		}
	}

	String & String::printf(const char *pszFormat, ...);
	String &String::vprintf(const char *pszFormat, va_list args);
	String & format(double dfValue, const char *pszFormat = NULL);
	String & trim();
	String & recode(const char *pszSrcEncoding, const char *pszDstEncoding);

	/* case insensitive find alternates */
	size_t    ifind(const std::string & str, size_t pos = 0) const;
	size_t    ifind(const char * s, size_t pos = 0) const;
	String &  toupper(void);
	String &  tolower(void);
};
/** @} */
end_gdb_namespace
end_gtl_namespace



