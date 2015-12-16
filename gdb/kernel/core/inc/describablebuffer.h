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
#include "buffer.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


class CORE_API DescribableBuffer: public Buffer{
public:
	enum DBF{
		DBF_VOID,

		DBF_INT8,
		DBF_INT16,
		DBF_INT32,
		DBF_INT64,

		DBF_UINT8,
		DBF_UINT16,
		DBF_UINT32,
		DBF_UINT64,



		DBF_FLOAT,
		DBF_3FLOAT,
		DBF_4FLOAT,
		DBF_5FLOAT,
		DBF_6FLOAT,
		DBF_7FLOAT,
		DBF_8FLOAT,
		DBF_9FLOAT,
		DBF_10FLOAT,
		DBF_16FLOAT,

		DBF_DOUBLE,
		DBF_3DOUBLE,
		DBF_4DOUBLE,
		DBF_5DOUBLE,
		DBF_6DOUBLE,
		DBF_7DOUBLE,
		DBF_8DOUBLE,
		DBF_9DOUBLE,
		DBF_10DOUBLE,
		DBF_16DOUBLE


	};

protected:
	DBF  _type;
public:
	static int sizeType(DBF t );
	inline int numberElements() {return size()/sizeType(_type);}
	inline DBF getType() const { return _type;}

public:
	DescribableBuffer();
	DescribableBuffer(void * d,size_t s,DBF t = DBF_VOID,bool b_alloc=false);//构造函数，如果b_alloc为真，则会新申请一块大小为s的内存块，并复制d的内容，反之，则指向d
	void copy(const DescribableBuffer & b);//拷贝Buffer函数，如果b的_alloc成员为假，则拷贝后的Buffer对象与b公用一个内存块，否则为内存块拷贝
	DescribableBuffer(const DescribableBuffer & b);//拷贝构造函数
	DescribableBuffer operator = (const DescribableBuffer & b);//等号赋值操作
	bool changeType(DBF t);
	void get(std::vector<double> & dv);
	void get(int * pc, double ** pp);//需要用户自己释放*pp
	void get(std::vector<float> & fv);
	void get(int * pc, float ** pp);//需要用户自己释放*pp
	void get(std::vector<int> & fv);
	void get(int * pc, int ** pp);//需要用户自己释放*pp
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
