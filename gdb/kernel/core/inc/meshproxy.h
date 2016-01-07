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
#include "bitset.h"
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "indexbuffer.h"
#include "connector.h"
#include "mesh.h"



begin_gtl_namespace
begin_gdb_namespace


class MeshProxy;
typedef std::shared_ptr<MeshProxy> MeshProxySharedPtr;

/** @addtogroup Mesh MeshBuilder-MeshBuilder网格生成管理
*  @{
*/
class CORE_API MeshProxy {
public:
	virtual bool queryByBox(Envelope3d & e3d, std::vector<BlockSharedPtr> & blocks) = 0;
	virtual bool queryByBlockID(const Identifier &id, BlockSharedPtr & block) = 0;
	virtual Envelope3d & getEnvelope(bool recalculate = false) = 0;
	virtual void getBlockColors(BlockSharedPtr & bsp, std::vector<Color4f> & colors) = 0;
};
/** @} */

 

end_gdb_namespace
end_gtl_namespace


