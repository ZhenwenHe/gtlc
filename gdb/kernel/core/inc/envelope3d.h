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
#include "envelope2d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Envelope3d;
typedef Envelope3d Bound6d;
typedef Envelope3d& Envelope3dRef;

/** @defgroup Envelope Envelope3d-包围边界结构体定义
*  @{
*/
class CORE_API Envelope3d{
public:
	double minx, maxx;
	double miny, maxy;
	double minz, maxz;
	int  recPlaneIndex;
public:
	Envelope3d();
	Envelope3d(double x1, double x2, double y1, double y2, double z1 = -10000, double z2 = 10000);
	Envelope3d(const Envelope2d & e2d, double z1 = -10000, double z2 = 10000);
	Envelope3d(const Envelope3d & e);
	Envelope3d operator = (const Envelope3d & e);
public:
	inline double getMinX(){ return minx; }
	inline double getMinY(){ return miny; }
	inline double getMinZ(){ return minz; }
	inline double getMaxX(){ return maxx; }
	inline double getMaxY(){ return maxy; }
	inline double getMaxZ(){ return maxz; }

	inline const double getMinX() const { return minx; }
	inline const double getMinY()const{ return miny; }
	inline const double getMinZ()const{ return minz; }
	inline const double getMaxX()const{ return maxx; }
	inline const double getMaxY()const{ return maxy; }
	inline const double getMaxZ()const{ return maxz; }

	inline double lengthX()const { return maxx - minx; }
	inline double lengthY()const { return maxy - miny; }
	inline double lengthZ()const { return maxz - minz; }


	inline void moveTo(double x, double y, double z);
	inline void moveTo(Vertex3d & v);
	inline const Vertex3d getMin() const;
	inline const Vertex3d getMax() const;
	/*
					7---------------6
				   /               /|
				  /				  / |
                 /               /  |
                /               /5  |
			  4|----------------|	|			
			   |	3			|   |2
			   |				|  /
			   |				| /	
			 0 ------------------1

	*/                      
	inline void getCorner(unsigned int i, Vertex3d & v) const;
	inline Vertex3d getCorner(unsigned int i);
	inline Vertex3d center();
	//获取对角线长度
	inline double getDiagonal();
	inline void setMin(const Vertex3d   v);
	inline void setMax(const Vertex3d   v);
	inline void get(double v[6]);
	inline void set(double v[6]);
	inline void get(double minv[3], double maxv[3]);
	inline void set(double minv[3], double maxv[3]);
	inline void set(const Vertex3d& minv, const Vertex3d & maxv);
	inline void set(double x1, double x2, double y1, double y2, double z1 = -10000, double z2 = 10000);
	inline double volume()const { return lengthX()*lengthY()*lengthZ(); }
public:
	virtual void extend(const Vertex3d & v);
	virtual void extend(const Envelope3d & e);
	virtual void extend(const double v[6]);
	virtual bool valid() const;
	virtual Envelope3d merge(const Vertex3d& pnt);
	virtual Envelope3d merge(const Vertex3d* pnts, int nPntNum);
	virtual Envelope3d merge(const std::vector<Vertex3d>& pnts);
	virtual Envelope3d merge(const Envelope3d& otherEnvlp);
	virtual int  intersect(const Envelope3d& otherbox, Envelope3d& retBox) const;
public:
	virtual int test(const Vertex3d& vPt) const;
	virtual int test(const Envelope3d &)const;
	virtual int test(const Envelope3d& inBox, char proAxis, Envelope3d * intersectBox) const;
	/*
	* 0-表示两个包围盒相离
	* 1-表示两个包围盒a和b相交
	* 2-表示包围盒a在b中
	* 3-表示包围盒b在a中
	*/
	static int test(Envelope3d & a, Envelope3d & b);
};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

