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
#include "value.h"

begin_gtl_namespace
begin_gdb_namespace

class FieldDefinition {
protected:
	/** @name 属性成员变量
	*  @{
	*/
	/** 属性列名称,在一个数据表中要求具有唯一性 */
	std::string _name;
	/** 属性列编码 */
	std::string _code;
	/**属性列标题*/
	std::string _caption;
	/** 属性类型的类型字符串，GVT中的有效类型 */
	GVT         _type;
	/** 属性类型长度，按照字节进行计算，双字节记为两个字节,<=0表示不需要这个变量记录的长度
	bool-     1
	int8      1
	int16     2
	int32     4
	int64     8
	uint8     1
	uint16    2
	uint32    4
	uint64    8
	float32   4          -3.40E+38 ~ +3.40E+38
	float64   8          -1.79E+308 ~ +1.79E+308
	date      4*3=12
	datetime  4*7=28
	char8     1-2GB
	char16    2-2GB
	_length只能为上述对应类型的整数倍n，当n>1的时候，表示为字符串或其他类型的数组
	*/
	int         _length;
	/** 属性类型的小数点位数,值为负数的时候为无效值
	*   只有当类型为float32和float64的时候起作用，
	*   float32的有效位数为7，所以_decimal[0-6]
	*   float64的有效位数为16，所以_decimal[0-15]
	*/
	int         _decimal;
	/** 属性类型是否可以为空，1-true表示可以为空，0-false表示不能为空 */
	int        _nullable;
	/** 默认值*/
	VALUE    *  _defaultValue;
	/**Display justification for field values,OJUndefined = 0, OJLeft = 1, OJRight = 2*/
	int         _justification;
	/**是否忽略，1-true,0-false,default=0*/
	int         _ignore;
	/** @} */

public:
	/** @name 构造函数与析构函数
	*  @{
	*/
	FieldDefinition();
	/**
	这是一个简化的构造函数，只需要输入字段名称，字段类型和个数，函数自动计算length的值
	例如：FieldDefinition（"Field1",GVT_CHAR8,50）构建50个字符宽度的文本列，length=50
	FieldDefinition（"Field2",GVT_CHAR16,50）构建50个字符宽度的宽文本列，length=100
	FieldDefinition（"Field3",GVT_INT64,1）构建64BITS的整数列，length=8
	FieldDefinition（"Field4",GVT_INT64,10）构建64BITS的整数数组列，length=80
	FieldDefinition（"Field5",GVT_DATETIME,1）构建datetime列，length=28
	*/
	FieldDefinition(std::string name, GVT type, int count = 1);
	/*
	GVT            NAME      SIZE       C/C++                 RANGE
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
	FieldDefinition(std::string name, std::string  gvt_typename, int count = 1);
	FieldDefinition(std::string name,
		GVT type, int length, int decimal = -1,
		bool nullable = true, const VALUE * defaultVal = 0,
		std::string code = "", std::string caption = "", int justification = 0, bool ignore = false);
	FieldDefinition(const FieldDefinition & t);
	/** @} */

	/** @name 复制与等号赋值重载
	*  @{
	*/
	void copy(const FieldDefinition & t);
	FieldDefinition operator = (const FieldDefinition & t);
	bool operator==(const FieldDefinition & t);
	/** @} */

	/** @name 对象成员变量获取与设置操作函数集
	*  @{
	*/
	inline std::string getName() { return _name; }
	inline  void  setName(const std::string& v) { _name = v; }

	inline std::string getCode() { return _code; }
	inline void   setCode(const std::string& v) { _code = v; }

	inline std::string getCation() { return _caption; }
	inline void   setCaption(const std::string& v) { _caption = v; }

	inline std::string  getTypeName() { return ValUtils::getTypeName(_type); }
	inline void   setType(GVT v) { _type = v; }
	inline int    getType() { return (int)_type; }

	inline int  getLength() { return _length; }
	inline void   setLength(int v) { _length = v; }

	inline int  getDecimal() { return _decimal; }
	inline void   setDecimal(int v) { _decimal = v; }

	inline bool  isNullable() { return _nullable!=0; }
	inline void   setNullable(bool v) { _nullable = v?1:0; }

	inline bool setDefaultValue(const VALUE & g) {
		if (_defaultValue) {
			_defaultValue = ValUtils::newValue();
		}
		ValUtils::copy(_defaultValue, g);
	}
	inline VALUE* getDefaultValue() { return _defaultValue; }

	inline bool isIgnore() { return _ignore != 0 ? 1 : 0; }
	inline void setIgnore(bool b = true) { _ignore = b ? 1 : 0; }

	inline int getJustification() { return _justification; }
	inline void setJustification(int v = 0) { _justification = v; }
	/** @} */

public:
	/**  判断属性类型是否为数字类型
	*	 @param 无
	*   @return 如果属性类型是数字类型，则返回true，否则返回false
	*/
	inline bool isNumber() { return ValUtils::isNumber(_type); }
	inline bool isArray() { return _length / ValUtils::getTypeSize(_type) > 1; }
	inline bool isText() { return  ValUtils::isChar(_type) && isArray(); }
	inline bool isBlob() { return (!ValUtils::isChar(_type)) && isArray(); }
public:
	/**  将属性类型对象写入流对象
	*	 @param [in,out]  f 流对象
	*   @return void
	*/
	virtual void  write(std::ostream & f);
	/**  从流对象中读取属性类型对象
	*	 @param [in,out]  f 流对象
	*   @return void
	*/
	virtual void  read(std::istream & f);
};
end_gdb_namespace
end_gtl_namespace


