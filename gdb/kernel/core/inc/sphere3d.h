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
#include "envelope3d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 
class Sphere3d;
typedef Sphere3d& Sphere3dRef;
typedef Sphere3d* Sphere3dPtr;
typedef std::shared_ptr<Sphere3d> Sphere3dSharedPtr;

/** @defgroup Sphere3d Sphere3d-三维球
*  @{
*/
class GV3DCORE_API Sphere3d{
	/*breif 球心 */
	Vertex3d _center;
	/*breif 半径 */
	double   _radius;
public:
	Sphere3d(const Vertex3d & c, double r);
	Sphere3d();
	Sphere3d(const Sphere3d & s);
	Sphere3d operator = (const Sphere3d & s);
	Sphere3d operator+=(const Vertex3d &c);
	Sphere3d operator-=(const Vertex3d &c);
public:
	Sphere3d& extend(const Sphere3d&   sp);
	Sphere3d& extend(const Vertex3d&    v);
	Sphere3d& extend(const Envelope3d&  sp);
public:
	inline double & radius() { return _radius; }
	inline Vertex3d & center() { return _center; }
	inline const double & radius() const { return _radius; }
	inline const Vertex3d & center() const { return _center; }
	inline bool valid()const { return _radius == 0; }
	inline void moveTo(double x, double y, double z){ _center.x = x; _center.y = y; _center.z = z; }
	inline void moveTo(Vertex3d & v){ moveTo(v.x, v.y, v.z); }
public:
	/*
	* 0-表示两个球相离
	* 1-表示两个球相切
	* 2-表示两个球相交
	* 3-表示球a在b中
	* 4-表示球b在a中
	*/
	static int test(const Sphere3d&  a, const Sphere3d&  b);
	/*
	* 0-表示点在球外
	* 1-表示点在球内
	* 2-表示点在球上
	*/
	static int test(const Sphere3d&  a, const Vertex3d&  v);
};
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

