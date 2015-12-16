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

#include <cmath>
#include "assert.h"
#include "config.h"
#include "vertex2d.h"
#include "vertex3d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Envelope2d;
typedef Envelope2d& Envelope2dRef;

/** @addtogroup Envelope Envelope2d-包围边界结构体定义
*  @{
*/
class CORE_API Envelope2d{
public:
	double minx, maxx;
	double miny, maxy;
public:
	Envelope2d();
	Envelope2d(double x1, double x2, double y1, double y2);
	Envelope2d(const Envelope2d & e);
	Envelope2d operator = (const Envelope2d & e);
	inline void moveTo(double x, double y);
	inline void moveTo(Vertex2d & v);
	inline Vertex2d center();
	inline Vertex2d getMin();
	inline Vertex2d getMax();
	inline void get(double v[4]);
	inline void set(double v[4]);
	inline void get(double minv[2], double maxv[2]);
	inline void set(double minv[2], double maxv[2]);
	inline double lengthX(){ return maxx - minx; }
	inline double lengthY(){ return maxy - miny; }
public:
	/*
	* 0-表示两个包围盒相离
	* 1-表示两个包围盒a和b相交
	* 2-表示包围盒a在b中
	* 3-表示包围盒b在a中
	*/
	static int test(Envelope2d & a, Envelope2d & b);
public:
	virtual void extend(const Vertex2d & v);
	virtual void extend(const Envelope2d & e);
	virtual void extend(const double v[4]);
	virtual bool valid() const;
};


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

