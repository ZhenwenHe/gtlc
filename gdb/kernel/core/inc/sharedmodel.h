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

#include "geometry.h"


begin_gtl_namespace
begin_gdb_namespace

class SharedModel;
typedef std::shared_ptr<SharedModel> SharedModelSharedPtr;
typedef SharedModel* SharedModelPtr;

/** @addtogroup Geometry SharedModel-共享模型类
*  @{
*/
class CORE_API SharedModel{
public:
	/** 获取关联模型ID
	* @return 关联模型ID
	*/
	virtual Identifier::raw_type getID() = 0;

	/** 设置关联模型ID
	* @param [in] mdlid 关联模型ID
	*/
	virtual void setID(Identifier::raw_type mdlid) = 0;

	/** 获取关联模型名称
	* @return 关联模型名称
	*/
	virtual string getName() = 0;

	/** 设置关联模型名称
	* @param [in] mdlname 关联模型名称
	* @return void
	*/
	virtual void setName(string mdlname) = 0;

	/** 获取关联模型类型
	* @return int: 关联模型类型
	*/
	virtual int getType() = 0;

	/** 设置关联模型类型
	* @param [in] mdltype 关联模型类型
	*/
	virtual void setType(int mdltype) = 0;

	/** 获取关联模型关联的材质ID
	* @return 关联模型关联的材质ID
	*/
	virtual vector<Identifier::raw_type>& getMaterialIDs() = 0;

	/** 设置关联模型关联的材质ID
	* @param [in] linkmatids 关联模型关联的材质ID
	*/
	virtual void setMaterialIDs(vector<Identifier::raw_type>& linkmatids) = 0;

	/** 设置关联模型的几何数据
	* @param [in] p 关联模型关联的几何数据
	*/
	virtual void setGeometry(GeometrySharedPtr p) = 0;

	/** 获取关联模型的几何数据
	* @return C3DGeometry的引用,关联模型关联的几何数据
	*/
	virtual GeometrySharedPtr getGeometry() const = 0;

	/** 将对象写入缓存
	* @param [in] buf 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer& buf) = 0;

	/** 计算对象所占内存字节数
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer() = 0;
public:
	static SharedModelSharedPtr create();
	static SharedModelSharedPtr create(Identifier::raw_type d, std::string name, int type);

};
/** @} */




end_gdb_namespace
end_gtl_namespace


