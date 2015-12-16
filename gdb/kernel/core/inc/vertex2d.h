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
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


/** @addtogroup vertex Vertex2d
*  @{
*/
class CORE_API Vertex2d
{
public:
	double x, y;

	Vertex2d();
	Vertex2d(double xx, double yy);
	Vertex2d(Vertex2d & vt);
	void operator*=(double rhd);
	Vertex2d operator = (const Vertex2d & c);
};
/** @} */

/** @addtogroup vertex Vertex2dList
*  @{
*/
typedef std::vector<Vertex2d> Vertex2dList;
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

