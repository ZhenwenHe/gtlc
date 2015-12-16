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
#include <memory>
#include <vector>
#include <sstream>
#include <fstream>
#include <algorithm>
#include <set>
#include "time.h"
#include "config.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup Bitset  Bitset位集类
*  @{
*/
class GV3DCORE_API Bitset{
	std::vector<unsigned char> _data;//最高位放在[0]位置，每位的默认值为0
public:
	//返回已经设置的位数
	size_t count();

	bool get(size_t pos);
	//设置指定位置为0或1，true表示1，false表示0，如果pos大于数组长度，则自动扩展
	void set(size_t pos, bool b = true);
	//将位数组转换成整数，最高位放在[0]位置
	//例如数组中存放的1011，则返回11，而不是返回13
	unsigned long long to_ullong();
	//返回指定位置的值，如果pos大于位数组长度，自动拓展
	bool operator [] (const size_t pos);
	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset operator = (unsigned long long ull);
	//默认构造
	Bitset();
	//直接整数转化成二进制，赋值给Bitset，最高位放在[0]位置
	Bitset(unsigned long long n);
	//拷贝构造函数
	Bitset(const Bitset & b);
	//将高位与低位互相，如数组存放的是1011，则本函数执行后为1101
	void reverse();
	//返回左边n位，构成新的Bitset
	Bitset left(size_t n);
	//返回右边n位，构成新的Bitset
	Bitset right(size_t n);

	//测试两个Bitset是否相等
	bool operator == (const Bitset & b);
	//判断b包含的位数组是否是本类的位数组的前缀
	bool has_prefix(const Bitset & b);
	//判断本类的位数组是否是b的前缀
	bool is_prefix(unsigned long long & b);
	//
	void clear();

	void resize(size_t newSize);

	void set(const unsigned char * flags, unsigned long long s);
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
