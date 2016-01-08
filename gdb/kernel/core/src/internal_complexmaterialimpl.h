
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
#include "internal_materialimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class ComplexMaterialImpl : virtual public ComplexMaterial, virtual public MaterialImpl
{
	 
public:
 
	/** 复杂材质参数 */
	vector<COMMAT_ITEM> materialItemList;

	 
public:
 
	/** 构造函数
	*/
	ComplexMaterialImpl();
	/** 析构函数
	*/
	virtual ~ComplexMaterialImpl();

	/** @} */ // 构造与析构函数结尾

 
public:
	/** 复杂材质参数 */
	virtual std::vector<COMMAT_ITEM> & getMaterialItems() {
		return materialItemList;
	}
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
