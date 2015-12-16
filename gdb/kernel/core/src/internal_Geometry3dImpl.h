
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
#include "internal_GeometryImpl.h"
#include "internal_rayimpl.h"
#include "internal_frustumimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class Geometry3dImpl 	: virtual public GeometryImpl
{
	/** @defgroup Geometry3dImpl Geometry3dImpl-三维几何（抽象基类）
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 是否为一个已有对象的克隆实例 */
	int m_bClone;

	/** 对象的（轴向）包围盒 */
	AABBoxImpl m_eEnvelope;

	/** @} */ // 属性结尾

public:

	/** @defgroup Geometry3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 默认构造函数
	*/
	Geometry3dImpl();
	/** 默认析构函数
	*/
	virtual ~Geometry3dImpl();

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 设置当前对象的外包矩形
	* @param [in] MaxBox	已知的AABBoxImpl对象
	* @return
	*/
	void setEnvelope(const AABBoxImpl & envelope3d)
	{
		m_eEnvelope = envelope3d;
	}

	/** 获得当前对象是否为一个已有对象的克隆实例的标识
	* @param
	* @return 1--是克隆实例 0--不是克隆实例
	*/
	int IsCloned()
	{
		return m_bClone;
	}

	/** 获得当前对象的外包矩形
	* @param
	* @return 当前对象的外包矩形
	*/
	AABBoxImpl & getEnvelope()
	{
		return m_eEnvelope;
	}

	/** @} */ // 数据操作组结尾

	/** @name 计算操作
	*  @{
	*/

	/** 计算当前对象与外部指定几何对象的最小欧式距离
	* @param [in] pGeometry 外部指定几何对象的指针
	* @return 两几何对象的最小欧式距离距离值
	*/
	virtual double ComputeMinDis(const Geometry3dImpl* pGeometry) const;

	/** 判断外部指定三维点对象与当前几何对象外包矩形的空间关系
	* @param [in]  vPt Vertex3d类型三维点对象的引用
	* @return '0'--点在当前几何对象的外包矩形中；'o'--点不在当前几何对象的外包矩形中
	*/
	virtual  char RPointWithGeometry(Vertex3d& vPt) const
	{
		/* If query point is outside bounding box, finished. */
		srand((int)time(0));
		if (m_eEnvelope.IsPointInBox(vPt))
			return '0';
		else
			return 'o';
	}

	/** 判断外部指定三维点对象与当前几何对象的空间关系
	* @param [in]  vPt Vertex3d类型三维点对象的引用
	* @return 1--点在当前几何对象的内部或边界；0--点在当前几何对象外部
	*/
	virtual int  IsPointInGeometry(Vertex3d& vPt) const
	{
		char cRes = RPointWithGeometry(vPt);
		if (cRes == 'V' || cRes == 'E' || cRes == 'F' || cRes == 'i')
			return 1;
		else
			return 0;
	}

	/** 虚函数，在派生类中实现
	* 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	virtual double DPointToGeometryEx(Vertex3d& vPt) const
	{
		return 1e+14;
	}

	/** 计算外部指定的三维点对象和当前几何对象的距离
	* @param [in]  vPt Vertex3d类型三维点对象的引用
	* @return 点和当前对象的距离
	*/
	double DPointToGeometry(Vertex3d& vPt) const
	{
		return sqrt(DPointToGeometryEx(vPt));
	}

	/** 虚函数，在派生类中实现
	* 计算外部指定的三维线段到当前几何对象距离的平方
	* @param [in] begvPt	Vertex3d类型三维点对象的引用
	* @param [in] endvPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
	{
		return 1e+14;
	}

	/** 计算外部指定的三维线段和当前几何对象的距离
	* @param [in] begvPt	Vertex3d类型三维点对象的引用
	* @param [in] endvPt	Vertex3d类型三维点对象的引用
	* @return 点和当前对象的距离
	*/
	double D3DLineSegToGeometry(Vertex3d& begvPt, Vertex3d& endvPt)
	{
		return sqrt(D3DLineSegToGeometryEx(begvPt, endvPt));
	}

	/** 虚函数，在派生类中实现
	* 计算外部指定的三角形到当前几何对象距离的平方
	* @param [in] triPt1, triPt2, triPt3Vertex3d类型三维点对象的引用(表示三角行三个顶点)
	* @return 点到当前几何对象距离的平方
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
	{
		return 1e+14;
	}

	/** 虚函数，在派生类中实现
	* 计算当前对象的几何重心
	* @return 对象的几何重心
	*/
	virtual Vertex3d CalCorePoint()
	{
		Vertex3d corePt;
		return corePt;
	}

	/** 计算外部指定的三角形和当前几何对象的距离
	* 计算外部指定的三角形到当前几何对象距离的平方
	* @param [in] triPt1, triPt2, triPt3Vertex3d类型三维点对象的引用(表示三角行三个顶点)
	* @return 点和当前对象的距离
	*/
	double D3DTriangleToGeometry(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
	{
		return sqrt(D3DTriangleToGeometryEx(triPt1, triPt2, triPt3));
	}

	/** 虚函数
	* 统计当前对象包含的几何数据
	* @param
	* @return long*:函数内部开辟空间、外部释放
	*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
	*								面对象数，法向数，转化为等价三角面片数，
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
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0);

	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

	/** 虚函数
	* 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int FrustumIntersectGeometry(FrustumImpl& frustum);

	/** 虚函数
	* 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int FrustumIntersectGeometryEx(FrustumImpl& frustum, vector<GeometryImpl*>& pGeometrys)
	{
		pGeometrys.push_back(this);
		return FrustumIntersectGeometry(frustum);
	}

	/**判断截头椎体与当前几何体的外包框是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	int FrustumIntersectAABB(FrustumImpl& frustum);

	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离的平方
	* @param [in] vPt1	射线起点
	* @param [in] vPt2	射线方向上以点(与vPt1共同决定射线方向)
	* @param [out] dLength	射线起点到与几何体交点的最小距离的平方，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int RayCrossGeometryEx(Vertex3d& vPt1, Vertex3d& vPt2, double& dLength, Geometry3dImpl*& pSubGeometry)
	{
		pSubGeometry = this;
		Vertex3d vDir = vPt2 - vPt1;
		vDir.normalize();
		RayImpl ray(vPt1, vDir);
		return RayCrossGeometry(ray, dLength);
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

	/** 释放对象数据并将指针和变量赋初始值
	* @return 是否执行成功
	*/
	virtual bool Init();

	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int rayCross(const RaySharedPtr & ray, double& length) const{
		return RayCrossGeometry(*((RayImpl*) ray.get()), length);
	}

	/** 虚函数
	/** 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int frustumIntersect(const FrustumSharedPtr & frustum){
		return FrustumIntersectGeometry(*((FrustumImpl*) frustum.get()));
	}
	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace


