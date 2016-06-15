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




begin_gtl_namespace
begin_gdb_namespace


/** @addtogroup vertex Vertex2f
*  @{
*/
class CORE_API Vertex2f
{
public:
	float x, y;

	Vertex2f();
	Vertex2f(float xx, float yy);
	Vertex2f(const Vertex2f & vt);
	void operator*=(float rhd);
	Vertex2f operator = (const Vertex2f & c);
};
/** @} */

/** @addtogroup vertex Vertex2dList
*  @{
*/
typedef std::vector<Vertex2f> Vertex2fList;
/** @} */


end_gdb_namespace
end_gtl_namespace



