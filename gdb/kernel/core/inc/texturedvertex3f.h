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
class   TexturedVertex3f : public Vertex3f
{ 
public:	 
	/** 纹理坐标 */
	float u, v; 
public:
 	TexturedVertex3f(float dx = 0.0, float dy = 0.0, float dz = 0.0, float du = 0.0, float dv = 0.0) :Vertex3f(dx, dy, dz), u(du), v(dv)
	{

	}
 
	TexturedVertex3f(Vertex3f& vVertex, float du = 0.0, float dv = 0.0) :Vertex3f(vVertex), u(du), v(dv)
	{

	}
};

end_gdb_namespace
end_gtl_namespace


