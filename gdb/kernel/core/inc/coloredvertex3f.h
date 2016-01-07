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
class ColoredVertex3f
{	
public: 

	/** ¿Õ¼ä×ø±ê */
	float x, y, z;

	/** ÑÕÉ« */
	float cR, cG, cB; 
};
end_gdb_namespace
end_gtl_namespace


