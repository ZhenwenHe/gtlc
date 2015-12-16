#pragma once
#include "sharedModel.h"

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
 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
class SharedModelImpl : public SharedModel
{
	/** @defgroup CMdlInfo CMdlInfo-关联模型管理类
	*  @{
	*/
protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/**关联模型ID */
	Identifier::raw_type m_mdlid;

	/**关联模型名称 */
	//有待处理
	string m_mdlname;

	/**关联模型类型 */
	int m_mdltype;

	/**关联模型关联的材质ID */
	vector<Identifier::raw_type> m_linkmatids;

	/**关联模型的几何数据 */
	//暂时由TrackleFile负责析构
	GeometrySharedPtr m_geom;

	/** @} */ // 属性结尾

public:

	/** @name 操作函数
	* @{
	*/

	/** 默认构造函数
	* @param [in]
	*/
	SharedModelImpl();

	/** 构造函数
	* @param [in] mdlid			关联模型ID
	* @param [in] mdlname		关联模型名称
	* @param [in] mdltype		关联模型类型
	*/
	SharedModelImpl(Identifier::raw_type mdlid, string mdlname, int mdltype);

	/** 析构函数
	* @param [in]
	*/
	~SharedModelImpl();

		/** 将对象写入缓存
	* @param [in] buf 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf);

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer& buf);

	/** 计算对象所占内存字节数
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer();


	/** @} */ // 操作函数结尾


	/** @} */ // 模块结尾
public:
	/** 获取关联模型ID
	* @return 关联模型ID
	*/
	virtual Identifier::raw_type getID() {
		return m_mdlid;
	}

	/** 设置关联模型ID
	* @param [in] mdlid 关联模型ID
	*/
	virtual void setID(Identifier::raw_type mdlid){
		m_mdlid = mdlid;
	}

	/** 获取关联模型名称
	* @return 关联模型名称
	*/
	virtual string getName() {
		return m_mdlname;
	}

	/** 设置关联模型名称
	* @param [in] mdlname 关联模型名称
	*/
	virtual void setName(string mdlname) {
		m_mdlname = mdlname;
	}

	/** 获取关联模型类型
	* @return 关联模型类型
	*/
	virtual int getType() {
		return m_mdltype;
	}

	/** 设置关联模型类型
	* @param [in] mdltype 关联模型类型
	*/
	virtual void setType(int mdltype){
		m_mdltype = mdltype;
	}


	/** 获取关联模型关联的材质ID
	* @return 关联模型关联的材质ID
	*/
	virtual vector<Identifier::raw_type>& getMaterialIDs(){
		return m_linkmatids;
	}

	/** 设置关联模型关联的材质ID
	* @param [in] linkmatids 关联模型关联的材质ID
	*/
	virtual void setMaterialIDs(vector<Identifier::raw_type>& linkmatids) {
		m_linkmatids.resize(linkmatids.size());
		std::copy(linkmatids.begin(), linkmatids.end(), m_linkmatids.begin());
	}

	/** 设置关联模型的几何数据
	* @param [in] p 关联模型关联的几何数据
	*/
	virtual void setGeometry(GeometrySharedPtr p) {
		m_geom = p;
	}

	/** 获取关联模型的几何数据
	* @return C3DGeometry的引用,关联模型关联的几何数据
	*/
	virtual GeometrySharedPtr getGeometry() const  {
		return   m_geom;
	}
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace



