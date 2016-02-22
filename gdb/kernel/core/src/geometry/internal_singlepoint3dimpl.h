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
#include "internal_point3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class  SinglePoint3dImpl : virtual public SinglePoint3d, virtual public  Point3dImpl
{

public:

	/**  获取点的颜色
	* @param  无
	* @return 点的颜色
	*/
	virtual Color4f getColor() {
		return Color::create(m_vPoint.cR, m_vPoint.cG, m_vPoint.cB, 1.0f);
	}
	/**  设置点的颜色
	* @param  [in] c4f  点的颜色
	* @return 无
	*/
	virtual void setColor(const Color4f & c4f){
		m_vPoint.cR = c4f.r;
		m_vPoint.cG = c4f.g;
		m_vPoint.cB = c4f.b;
	}

	/**  获取点的法线
	* @param  无
	* @return 点的法线
	*/
	virtual Vertex3d getNormal(){
		return Vertex3d(m_vPoint.nx, m_vPoint.ny, m_vPoint.nz);
	}
	/**  设置点的法线
	* @param  [in] v  点的法线向量
	* @return 无
	*/
	virtual void setNormal(const Vertex3d & v){
		m_vPoint.nx = v.x;
		m_vPoint.ny = v.y;
		m_vPoint.nz = v.z;
	}

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_POINT:
		{
			*p = (void*)(Point3d *) this;
			return true;
		}
		case GEOMTYPE_3D_SINGLEPOINT:
		{
			*p = (void*)(SinglePoint3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/**  获取点的大小
	* @param  无
	* @return 点的大小
	*/
	virtual double getPointSize() {
		return m_dPointSize;
	}
	/**  设置点的大小
	* @param  [in] s  点的大小
	* @return 无
	*/
	virtual void setPointSize(double s) {
		m_dPointSize = s;
	}

	/** @defgroup Point3dImpl SinglePoint3dImpl-三维点
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** @brief 三维点大小 */
	double m_dPointSize;

	/** @} */ // 属性结尾

public:

	/** @defgroup SinglePoint3dImpl 操作（成员函数）
	* @{
	*/

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	SinglePoint3dImpl(void);

	/** 析构函数
	*/
	virtual ~SinglePoint3dImpl(void);

	/** @} */ // 构造与析构函数结尾


	/** @name 数据操作
	*  @{
	*/

	/** 设置点数据
	* @param [in] vPoint	点数据
	* @return
	*/
	void SetVertex(ShadedVertex3d vPoint)
	{
		m_vPoint = vPoint;
	}

	/** 获得顶点类型编码
	* @param
	* @return 点数据
	*/
	ShadedVertex3d GetVertex() const
	{
		return m_vPoint;
	}

	/** 设置点大小
	* @param [in] vPoint	点大小
	* @return
	*/
	void SetPointSize(double dPointSize)
	{
		m_dPointSize = dPointSize;
	}

	/** 获得点大小
	* @param
	* @return 点大小
	*/
	double GetPointSize() const
	{
		return m_dPointSize;
	}

	/** @} */ // 数据操作组结尾



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
end_gtl_namespace


