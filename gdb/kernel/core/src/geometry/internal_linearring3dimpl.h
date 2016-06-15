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
#include "internal_curve3dimpl.h"
#include "internal_rayimpl.h"
#include "internal_frustumimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class LinearRing3dImpl :public Curve3dImpl , virtual public LinearRing3d
{
	/** @defgroup LinearRing3dImpl LinearRing3dImpl-线环
	*  @{
	*/

protected:

	/** @name 属性（成员变量）
	* @{
	*/
	/** @brief 内环还是外环的标志 */
	int m_bInOrOutRing;

	/** @} */ // 属性结尾
public:

	/**  获取线环标志
	* @param  无
	* @return 1 -- 内环标志；0 -- 外环标志
	*/
	virtual int getRingFlag() {
		return m_bInOrOutRing;
	}
	/**  设置线环标志
	* @param  [in] nFlag 线环标志 (1 -- 内环标志；0 -- 外环标志)
	* @return 无
	*/
	virtual void setRingFlag(int f){
		m_bInOrOutRing = f;
	}

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
		case GEOMTYPE_3D_LINEARRING:
		{
			*p = (void*)(LinearRing3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** 获取线的长度
	* @param
	* @return 线长
	*/
	virtual double getLength(){ return 0.0; }	//$$

public:

	/** @defgroup LinearRing3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	LinearRing3dImpl()
	{
		m_lVertNum = 0;
		m_vVertices = NULL;
		m_bInOrOutRing = 0; // 外环
		m_byteTypeID = GEOTYPE_3D_LINEARRING;
		m_iVertexType = VERTEXTYPE_V3d;
	}

	/** 构造函数2
	*/
	LinearRing3dImpl(int nVertexNum, Vertex3d* const vVertices, int bInOrOutRing = 0)
		: m_bInOrOutRing(bInOrOutRing)
	{
		m_byteTypeID = GEOTYPE_3D_LINEARRING;
		m_iVertexType = VERTEXTYPE_V3d;
		m_lVertNum = nVertexNum;
		m_vVertices = vVertices;
	}

	/** 析构函数
	*/
	virtual ~LinearRing3dImpl()
	{
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 设置顶点类型编码
	* @param [in] iVertexType	 顶点类型编码
	* @return
	*/
	void SetVertexType(unsigned char iVertexType)
	{
		m_iVertexType = iVertexType;
	}

	/** 获得顶点类型编码
	* @param
	* @return 顶点类型编码
	*/
	unsigned char GetVertexType() const
	{
		return m_iVertexType;
	}

	/** 设置线环顶点个数及顶点值
	* @param [in] lVertNum	顶点个数
	* @param [in] vVertices	指向一串顶点对象的指针
	* @return
	*/
	void SetVertices(long lVertNum, void* const vVertices)
	{
		m_lVertNum = lVertNum;
		m_vVertices = vVertices;
	}

	/** 获得当前对象的顶点序列的指针
	* @param [in] vVertices 待赋值的顶点序列的指针
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	/** 获得当前对象的顶点个数
	* @param
	* @return 当前对象的顶点个数
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** 获得顶点列表和顶点列表长度
	* @param [out] vVertexList	顶点列表
	* @param [out] lVertNum	顶点列表长度
	* @return
	*/
	void GetVertices(void*& vVertexList, long& lVertNum) const
	{
		vVertexList = m_vVertices;
		lVertNum = m_lVertNum;
	}

	/** 获得顶点列表2
	* @param [out]  vt3d 顶点列表
	* @return
	*/
	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** 获得是内环还是外环的标志值
	* @param
	* @return 1 -- 内环标志；0 -- 外环标志
	*/
	int GetInOutRingFlag()
	{
		return m_bInOrOutRing;
	}

	/** 设置是内环还是外环的标志
	* @param [in] bFlag	标志值
	* @return
	*/
	void SetInOutRingFlag(int bFlag)
	{
		m_bInOrOutRing = bFlag;
	}

	/** @} */ // 数据操作组结尾


	/** @name 计算操作
	*  @{
	*/

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray 射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

	/** 判断外部指定三维点对象与当前几何对象外包矩形的空间关系
	* @param [in]  vPt Vertex3d类型三维点对象的引用
	* @return:  'o' - 在多边形外； 'i' - 在多边形内；'E' - 在多边形边界上； 'V' - 在多边形点上
	*/
	char  RPointWithGeometry(Vertex3d& vPt) const;

	/** 虚函数
	* 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int FrustumIntersectGeometry(FrustumImpl& frustum);

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


	//int FrustumIntersectGeometry(FrustumImpl& frustum);

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾
};

end_gdb_namespace
end_gtl_namespace


