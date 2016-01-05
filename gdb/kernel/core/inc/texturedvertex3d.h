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
#include "vertex2d.h"
#include "vertex3d.h"
#include "vertex4d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class  TexturedVertex3d : public Vertex3d
{
public: 
	/** 纹理坐标 */
	double u, v; 
public: 

	/** 构造函数1
	*/
	TexturedVertex3d(double dx = 0.0, double dy = 0.0, double dz = 0.0, double du = 0.0, double dv = 0.0) :Vertex3d(dx, dy, dz), u(du), v(dv)
	{

	}

	/** 构造函数2
	*/
	TexturedVertex3d(Vertex3d& vVertex, double du = 0.0, double dv = 0.0) :Vertex3d(vVertex), u(du), v(dv)
	{

	}

	/** 构造函数3
	*/
	TexturedVertex3d(Vertex3d& vVertex, Vertex2d& vText) :Vertex3d(vVertex), u(vText.x), v(vText.y)
	{

	} 
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
