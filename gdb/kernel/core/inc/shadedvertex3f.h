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
#include "vertex2f.h"
#include "vertex3f.h" 



begin_gtl_namespace
begin_gdb_namespace
class  ShadedVertex3f : public Vertex3f
{ 
public:  
	/** 法线 */
	float nx, ny, nz;

	/** 颜色 */
	float cR, cG, cB; 

public: 

	ShadedVertex3f(float fx = 0.0, float fy = 0.0, float fz = 0.0, float fnx = 0.0, float fny = 0.0, float fnz = 0.0, float r = 0.0, float g = 0.0, float b = 0.0) 
		:Vertex3f(fx, fy, fz), nx(fnx), ny(fny), nz(fnz), cR(r), cG(g), cB(b)
	{

	}

	ShadedVertex3f(Vertex3f& vPos, Vertex3f& vNor, Vertex3f& vColor) :Vertex3f(vPos), nx(vNor.x), ny(vNor.y), nz(vNor.z), cR(vColor.x), cG(vColor.y), cB(vColor.z)
	{

	} 
};
end_gdb_namespace
end_gtl_namespace


