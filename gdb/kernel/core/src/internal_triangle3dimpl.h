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
#include "internal_surface3dimpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Triangle3dImpl : virtual public cn::edu::cug::gdb::Triangle3d, virtual public Surface3dImpl
{
public:
	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_SURFACE:
		{
			*p = (void*)(cn::edu::cug::gdb::Surface3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_TRIANGLE:
		{
			*p = (void*)(cn::edu::cug::gdb::Triangle3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	virtual void setVertices(cn::edu::cug::gdb::Vertex3d v[3])
	{
		for (int i = 0; i<3; i++)
		{
			m_vVetices[i].x = v[i].x;
			m_vVetices[i].y = v[i].y;
			m_vVetices[i].z = v[i].z;
		}
	}
	virtual void getVertices(cn::edu::cug::gdb::Vertex3d v[3])
	{
		for (int i = 0; i<3; i++)
		{
			v[i].x = m_vVetices[i].x;
			v[i].y = m_vVetices[i].y;
			v[i].z = m_vVetices[i].z;
		}
	}

	virtual void setNormal(const cn::edu::cug::gdb::Vertex3d & v)
	{
		m_vNormal.x = v.x;
		m_vNormal.y = v.y;
		m_vNormal.z = v.z;
	}
	virtual void getNormal(cn::edu::cug::gdb::Vertex3d & v)
	{
		v.x = m_vNormal.x;
		v.y = m_vNormal.y;
		v.z = m_vNormal.z;
	}
	/** 求三角形周长
	* @return 周长
	*/
	virtual double getPerimeter()
	{
		return 0.0; //$$
	}
	/** @defgroup Triangle3dImpl Triangle3dImpl-三角形
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	//##ModelId=49E3472801C7
	/** 顶点数组 */
	Vertex3d m_vVetices[3];

	//##ModelId=49E3472801D4
	/** 法线 */
	Vertex3d m_vNormal;

	/** @} */ // 属性结尾

public:

	/** @defgroup Triangle3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	Triangle3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
	}

	/** 构造函数2
	*/
	Triangle3dImpl(Vertex3d& v1, Vertex3d& v2, Vertex3d& v3, Vertex3d& vNormal)
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
		m_vVetices[0] = v1;
		m_vVetices[1] = v2;
		m_vVetices[2] = v3;
		m_vNormal = vNormal;
	}

	/** 构造函数3
	*/
	Triangle3dImpl(const Vertex3d* vVertices, Vertex3d& vNormal)
	{
		m_byteTypeID = GEOTYPE_3D_TRIANGLE;
		m_vVetices[0] = vVertices[0];
		m_vVetices[1] = vVertices[1];
		m_vVetices[2] = vVertices[2];
		m_vNormal = vNormal;
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得顶点的法向量
	* @param
	* @return 顶点的法向量
	*/
	Vertex3d GetNormal() const
	{
		return m_vNormal;
	}

	/** 获得顶点列表
	* @param [out] vVertices  将被赋值的顶点列表
	* @return
	*/
	void GetVertices(Vertex3d* vVertices) const
	{
		vVertices[0] = m_vVetices[0];
		vVertices[1] = m_vVetices[1];
		vVertices[2] = m_vVetices[2];
	}

	/** 设置顶点的法向量
	* @param [in] vNor 顶点的法向量
	* @return
	*/
	void SetNormal(const Vertex3d& vNor)
	{
		m_vNormal = vNor;
	}

	/** 设置注记颜色
	* @param [in] vVertices 顶点列表指针
	* @return
	*/
	void SetVertices(const Vertex3d* vVertices)
	{
		m_vVetices[0] = vVertices[0];
		m_vVetices[1] = vVertices[1];
		m_vVetices[2] = vVertices[2];
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	//##ModelId=49EF08490242
	/** 求三角形面积
	* @return 面积值
	*/
	double Area() const
	{
		Vertex3d v1 = m_vVetices[1] - m_vVetices[0];
		Vertex3d v2 = m_vVetices[2] - m_vVetices[1];
		return 0.5 * (v1 ^ v2).length();
	}

	//##ModelId=49EF08490242
	/** 求三角形有符号面积
	* @param [in] vDirection 观看方向
	* @return 面积值
	*/
	double Area(Vertex3d& vDirection) const
	{
		Vertex3d v1 = m_vVetices[0] - m_vVetices[1];
		Vertex3d v2 = m_vVetices[0] - m_vVetices[2];
		Vertex3d v3 = v1 ^ v2;
		if (v3 * vDirection < -1e-6)
			return -v3.length(); // v3的模
		else
			return v3.length();
	}


	//##ModelId=49EF0853004E
	/** 求三角形周长
	* @return 周长
	*/
	// 与Area函数的区别，注意这里如果设置为const成员函数，那么m_vVetices是不能改变的，也就是const变量。那么是不能传给DPointToPoint的。因此DPointToPoint需要纠正。
	double Perimeter()
	{
		double l1 = DPointToPoint(m_vVetices[0], m_vVetices[1]);
		double l2 = DPointToPoint(m_vVetices[1], m_vVetices[2]);
		double l3 = DPointToPoint(m_vVetices[2], m_vVetices[0]);
		return l1 + l2 + l3;
	}

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const
	{
		Vertex3d tempPt;
		Vertex3d pt0 = m_vVetices[0];
		Vertex3d pt1 = m_vVetices[1];
		Vertex3d pt2 = m_vVetices[2];

		return DPointToTriangleEx(vPt, pt0, pt1, pt2, &tempPt);
	}

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


	/** 虚函数，在派生类中实现
	* 计算外部指定的三维线段到当前几何对象距离的平方
	* @param [in] begvPt	Vertex3d类型三维点对象的引用
	* @param [in] endvPt	Vertex3d类型三维点对象的引用
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
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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
