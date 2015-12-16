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
#include "vertex3f.h"
#include "vertex3d.h"
#include "vertex4d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class ShadedVertex3d : public Vertex3d
{ 
public: 
	/** 法线 */
	double nx, ny, nz;

	/** 颜色 */
	float cR, cG, cB; 
public:
	  

	/** 构造函数1
	*/
	ShadedVertex3d(double fx = 0.0, double fy = 0.0, double fz = 0.0, double fnx = 0.0, double fny = 0.0, double fnz = 0.0, float r = 0.0, float g = 0.0, float b = 0.0)
		:Vertex3d(fx, fy, fz), nx(fnx), ny(fny), nz(fnz), cR(r), cG(g), cB(b)
	{

	}

	/** 构造函数2
	*/
	ShadedVertex3d(Vertex3d& vPos, Vertex3d& vNor, Vertex3f& vColor) :Vertex3d(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), cR(vColor.x), cG(vColor.y), cB(vColor.z)
	{

	}
	 
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
