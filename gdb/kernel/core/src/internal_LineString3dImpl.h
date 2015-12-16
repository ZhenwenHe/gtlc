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
#include "geometry3d.h"
#include "internal_Curve3dImpl.h"
#include "internal_rayimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class LineString3dImpl : virtual public LineString3d, virtual public Curve3dImpl
{
public:

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_CURVE:
		{
			*p = (void*)(Curve3d *) this;
			return true;
		}
		case GEOMTYPE_3D_LINESTRING:
		{
			*p = (void*)(LineString3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	/** @defgroup LineString3dImpl LineString3dImpl-三维折线
	*  @{
	*/
	/**  获取折线宽度
	* @param  [in] lineWidth  折线宽度
	* @param  [out] lineWidth  折线宽度
	* @return 无
	*/
	virtual void getLineWidth(double& lineWidth){
		lineWidth = m_dLineWidth;
	}
	/**  设置折线宽度
	* @param  [in] lineWidth  折线宽度
	* @return
	*/
	virtual void setLineWidth(double& lineWidth){
		m_dLineWidth = lineWidth;
	}

	/** 获取线的长度
	* @param
	* @return 线长
	*/
	virtual double getLength(){ return 0.0; }	//$$

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** @brief 线宽 */
	double m_dLineWidth;

	/** @} */ // 属性结尾

public:

	/** @defgroup LineString3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/


	/** 构造函数1
	*/
	LineString3dImpl()
	{
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_byteTypeID = GEOTYPE_3D_LINESTRING;
		m_iVertexType = VERTEXTYPE_V3d;
		m_dLineWidth = 0.0;
	}

	/** 构造函数2
	*/
	LineString3dImpl(int nVertNum, void* const vVertices, unsigned char vertexType, double dLineWidth)
		: m_dLineWidth(dLineWidth)
	{
		m_byteTypeID = GEOTYPE_3D_LINESTRING;
		m_lVertNum = nVertNum;
		m_vVertices = vVertices;
		m_iVertexType = vertexType;
	}

	/** 析构函数
	*/
	virtual ~LineString3dImpl()
	{
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 设置三维折线顶点个数和顶点数据
	* @param [in] nVertexNum	三维折线顶点个数
	* @param [in] vVertices	指向一串三维点对象的指针
	* @return
	*/
	void SetVertices(int nVertexNum, void* const vVertices, unsigned char iVertexType)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
		m_iVertexType = iVertexType;
	}

	/** 获得三维折线顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** 获得三维折线顶点个数
	* @param
	* @return 三维折线顶点个数
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** 获得顶点类型编码
	* @param
	* @return 顶点类型编码
	*/
	unsigned char GetVertexType() const
	{
		return m_iVertexType;
	}

	/** 设置线宽
	* @param [in] dLineWidth	线宽
	* @return
	*/
	void SetLineWidth(double dLineWidth)
	{
		m_dLineWidth = dLineWidth;
	}

	/** 获得线宽
	* @param
	* @return 线宽
	*/
	double GetLineWidth() const
	{
		return m_dLineWidth;
	}

	/** @} */ // 数据操作组结尾

	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

	/** @name 计算操作
	*  @{
	*/

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

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	virtual double DPointToGeometryEx(Vertex3d& vPt) const;

	/** 虚函数，在派生类中实现
	* 计算外部指定的三维线段到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);

	/** 虚函数，在派生类中实现
	* 计算外部指定的三角形到当前几何对象距离的平方
	* @param [in] triPt1, triPt2, triPt3Vertex3d类型三维点对象的引用(表示三角行三个顶点)
	* @return 线段到当前几何对象距离的平方
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

	/** 虚函数
	* 统计当前对象包含的几何数据
	* @param
	* @return long*:函数内部开辟空间、外部释放
	*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
	*								面对象数，法向数，转化为等价三角
	*								材质数，纹理数，
	*/
	virtual long* GeoStatistics();

	/** 虚函数
	* 判断当前对象和给定多边形在2D上的关系：相离，相交，包含
	* 判断
	* @param [in] ptArray		多边形
	* @param [in] precisionFlag 精度标志位 0-外包矩形 1-凸包 2-自身几何数据
	* @return 1-相交（临接，包含） 0-相离 -1-其他
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0);

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
