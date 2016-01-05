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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "matrix4x4.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @addtogroup Geometry Frustum-½ØÍ·×¶ÌåÀà
*  @{
*/
class CORE_API Frustum{
public:
	virtual void set(Vertex3d& eye, Vertex3d& cen, double nearD, double farD, double angle, double ratio) = 0;
	virtual	void setCamInternals(double nearD, double farD, double angle, double ratio) = 0;
	virtual void setCamera(Vertex3d& eye, Vertex3d& cen) = 0;
	virtual std::shared_ptr<Frustum> clone() = 0;

	static std::shared_ptr<Frustum> create();
};
typedef std::shared_ptr<Frustum>  FrustumSharedPtr;
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
