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

#include "config.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup ColorStruct 常用颜色结构体定义
*  @{
*/
struct Color3b{
	unsigned char r, g, b;
};

struct Color4b{
	unsigned char r, g, b, a;
};

struct Color3f{
	float r, g, b;
};

struct Color4f{
	float r, g, b, a;
};

class CORE_API Color {
public:
	static void convert(long c, Color4b & c4b);
	static void convert(long c, Color4f & c4f);
	static void convert(long c, Color3b & c3b);
	static void convert(const Color4b &  c4b, Color4f & c4f);
	static void convert(const Color4f &  c4f, long & c);
	static void convert(const Color4f &  c4f, Color4b & c4b);
	static Color4f create(float r, float g, float b, float a);
	static Color3f create(float r, float g, float b);
};

typedef std::vector<Color3b> Color3bList;

class CORE_API ColorSeries
{
	int _index;
	unsigned char  _redvector[256];
	unsigned char  _greenvector[256];
	unsigned char  _bluevector[256];
public:
	ColorSeries(int index = 0);
	ColorSeries(const ColorSeries & cs);
	virtual void copy(const ColorSeries & cs);
	virtual ColorSeries  operator ==(const ColorSeries & cs);
	virtual ~ColorSeries();
	virtual bool getColor(const double & z, const double & zmin, const double & zmax, Color4f  & cf);
	virtual bool getColors(const double * zv, const unsigned int count, std::vector<Color4f>  & cf);
	virtual bool reset(int i);
	inline const int  index() const{ return _index; }
	inline void  index(int i) { _index = i; }
};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

