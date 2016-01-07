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

#include "identifier.h"
#include "assert.h"
#include <cmath>







begin_gtl_namespace
begin_gdb_namespace


/** @defgroup vertex Vertex3f 常用坐标点的结构体定义
*  @{
*/
class CORE_API Vertex3f{
public:
	float x, y, z;
	Vertex3f(float xx, float yy, float zz);
	Vertex3f();
	inline void operator*=(float rhd);
	inline float normalize();
	Vertex3f operator = (const Vertex3f & c);
};
CORE_API const Vertex3f operator*(const Vertex3f& lhv, double rhd);
CORE_API const Vertex3f operator*(float lhd, const Vertex3f& rhv);
CORE_API float operator*(const Vertex3f& lhv, const Vertex3f& rhv);
CORE_API bool operator==(const Vertex3f& lhv, const Vertex3f& rhv);
CORE_API bool operator!=(const Vertex3f& lhv, const Vertex3f& rhv);


inline void Vertex3f::operator*=(float rhd)
{
	x *= rhd;
	y *= rhd;
	z *= rhd;
}
inline float Vertex3f::normalize()
{
	float d = sqrt(x*x + y*y + z*z);
	x /= d;
	y /= d;
	z /= d;
	return d;
}

end_gdb_namespace
end_gtl_namespace


