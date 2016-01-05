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
#include "internal_geometry3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Volume3dImpl 	: public Geometry3dImpl, virtual public cn::edu::cug::gdb::Volume3d
{
	/** @defgroup Volume3dImpl Volume3dImpl-三维体（抽象基类）
	*  @{
	*/
protected:

	/** @name 属性（成员变量）
	* @{
	*/
	/** @brief 顶点类型编码 */
	unsigned char m_iVertexType;
	/** @brief 顶点序列 */
	void *m_vVertices;
	/** @brief 顶点数 */
	long m_lVertNum;
	/** @} */ // 属性结尾

public:

	/** @defgroup Volume3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	Volume3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_VOLUME;
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_iVertexType = VERTEXTYPE_V3d;
	}
	/** 析构函数
	*/
	virtual ~Volume3dImpl()
	{
		VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);
	}
	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/
	/**  获取对象位置坐标访问器
	* @param  无
	* @return 对象位置坐标访问器指针
	*/
	virtual cn::edu::cug::gdb::VertexVisitorSharedPtr getVertexVisitor(){
		cn::edu::cug::gdb::VertexVisitorSharedPtr p = VertexVisitor::create(m_iVertexType);
		p->set(m_vVertices, m_lVertNum, m_iVertexType);
		return p;
	}
	/** 获得顶点类型编码
	* @param 无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const
	{
		return m_iVertexType;
	}
	/** 设置顶点类型编码
	* @param [in] iVertexType	 顶点类型编码
	* @return
	*/
	virtual void setVertexType(unsigned char iVertexType)
	{
		m_iVertexType = iVertexType;
	}
	/** 获得三维顶点个数
	* @param
	* @return 三维顶点个数
	*/
	virtual long getVerticesNum() const
	{
		return m_lVertNum;
	}
	/** 设置三维顶点个数和顶点数据
	* @param [in] nVertexNum	三维顶点个数
	* @param [in] vVertices	指向一串三维点对象的指针
	* @return
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
	}
	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const
	{
		vVertices = m_vVertices;
		lVertNum = m_lVertNum;
	}

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(cn::edu::cug::gdb::Volume3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	/** 获取对象的体积
	* @param  无
	* @return 对象体积
	*/
	virtual double getVolume(){ return 0.0; }//$$

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** 求体积
	* @return 体积
	*/
	double Volume(){ return 0.0; }//$$

	/** 开辟一个当前类对象所需的内存空间
	* @param
	* @return 开辟的当前类对象内存空间的首地址
	*/
	GeometryImpl* CreateObject();

	/** 根据外部给定的4维矩阵，实现当前对象的几何变换
	* @param [in] m4x4	给定的4维矩阵
	* @return
	*/
	void TranslateFrom(Matrix4x4& m4x4);

	/** 计算当前对象所需内存的大小
	* @param
	* @return 当前对象所需内存大小
	*/
	size_t sizeBuffer();

	/** 从缓冲区中解析当前对象，并给成员变量赋值
	* @param [in] buf	缓冲区的引用
	* @return 1 -- 解析成功；0 -- 解析失败
	*/
	bool readBuffer(Buffer & buf);

	/** 把当前对象的成员变量数据，打包成一个缓冲区
	* @param [out] buf	缓冲区的引用
	* @return 1 -- 打包成功；0 -- 打包失败
	*/
	bool writeBuffer(Buffer& buf);

	/** 虚函数，在派生类中实现
	* 计算当前对象的几何重心
	* @return 对象的几何重心
	*/
	virtual Vertex3d CalCorePoint();

	/** 虚函数
	* 统计当前对象包含的几何数据
	* @param
	* @return long*:函数内部开辟空间、外部释放
	*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
	*								面对象数，法向数，转化为等价三角
	*								材质数，纹理数，
	*/
	virtual long* GeoStatistics()
	{
		return NULL;
	}

	/** 虚函数
	* 判断当前对象和给定多边形在2D上的关系：相离，相交，包含
	* 判断
	* @param [in] ptArray		多边形
	* @param [in] precisionFlag 精度标志位 0-外包矩形 1-凸包 2-自身几何数据
	* @return 1-相交（临接，包含） 0-相离 -1-其他
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1;
	}

	/** 释放对象数据并将指针和变量赋初始值
	* @return 是否执行成功
	*/
	virtual bool Init();

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
