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
#include "color.h"


begin_gtl_namespace
begin_gdb_namespace
class ColoredVertex3d
{ 

public:	 
	/** ¿Õ¼ä×ø±ê */
	double x, y, z;

	/** ÑÕÉ« */
	double cR, cG, cB; 
};

end_gdb_namespace
end_gtl_namespace


