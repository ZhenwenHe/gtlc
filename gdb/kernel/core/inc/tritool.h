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
#include <map>

#include "Geometry.h"




begin_gtl_namespace
begin_gdb_namespace
/** @defgroup TriTool TriTool 
*  @{
*/
class CORE_API TriTool{
public:
	static bool trianglate(const VertexCollection2d  * in_points, std::vector<int> & tri_index);
	static bool trianglate(const std::vector<Vertex3d> & in_points, std::vector<int> & tri_index);
	static bool trianglate(const Vector3d* in_points, const int point_number,std::vector<int> & tri_index);
	static bool trianglate(std::vector<Vertex3d>& dataXYZ, std::vector<int>& waPs, std::vector<int>& waTris);
};
/** @} */ 
end_gdb_namespace
end_gtl_namespace


