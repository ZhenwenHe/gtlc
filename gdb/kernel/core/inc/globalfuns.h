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
#include <vector>
#include "config.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


/** @defgroup GlobalFunctions GlobalFunctions-ȫ���������ַ�������������
*  @{
*/
/** �ж��б����Ƿ�����ͬ�Ķ������
* @param [in] vv ��������
* @return bool:����ͬԪ���򷵻�true�����򷵻�false
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


/** �ַ����ü�����
* @param [in,out] str �ַ�������
* @param [in] c ָ���Ĳü��ַ�
* @return ��
*/
template<typename CharT>
void trim(std::basic_string<CharT>  & str, CharT c)
{
	//ɾ�������˵�ָ���ַ�
	typename std::basic_string<CharT>::size_type pos = str.find_last_not_of(c);
	if (pos != std::basic_string<CharT>::npos)
	{
		str.erase(pos + 1);
		pos = str.find_first_not_of(c);
		if (pos != std::basic_string<CharT>::npos)
			str.erase(0, pos);
	}
	else
		str.erase(str.begin(), str.end());
	//ɾ���м��ָ���ַ�
	pos = str.find_first_of(c);
	while (pos != std::basic_string<CharT>::npos){
		str.erase(str.begin() + pos);
		pos = str.find_first_of(c);
	}
}

/** ������ת�����ַ���
* @param [in,out] num   ��Ҫת��������
@return ת���õ����ַ���
*/
template<typename CharT, typename NumericT>
std::basic_string<CharT> numberToString(NumericT num)
{
	std::basic_ostringstream<CharT> oss;
	oss << (NumericT)num;
	if (oss.str().find_first_of(',') != std::basic_string<CharT>::npos){//���и�ʽ����
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, ',');
		return sz;
	}
	else if (oss.str().find_first_of('��') != std::basic_string<CharT>::npos){
		std::basic_string<CharT> sz = oss.str();
		trim<CharT>(sz, '��');
		return sz;
	}
	else{
		return oss.str();
	}
}

/** �ַ���ת��������
* @param [in,out] str �ַ���
* @return ת���õ�������
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const std::basic_string<CharT> &str)
{

	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}
/** ���ַ���ת��������
@param [in,out] str �ַ���ָ��
@return ת���õ�������
*/
template<typename NumericT, typename CharT>
NumericT stringToNumber(const CharT * str)
{
	std::basic_istringstream<CharT> iss(str);
	NumericT result;
	iss >> result;
	return result;
}
/** ��ȡϵͳ��ǰʱ�䣬�õ�ϵͳʱ���ַ���
@param [out] sz �ַ���
@return ��
*/
void getCurTime(std::string & sz);

/** ��ȡϵͳ��ǰʱ�䣬�õ�ϵͳʱ���ַ���
@param ��
@return �õ�ϵͳʱ���ַ���
*/
std::string getCurTime();

/** @} */

 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

