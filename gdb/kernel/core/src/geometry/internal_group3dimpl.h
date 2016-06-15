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


begin_gtl_namespace
begin_gdb_namespace
class  Group3dImpl :  public Geometry3dImpl, virtual public Group3d
{
	/** @defgroup Group3dImpl Group3dImpl-几何组合体
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 几何对象集合，以vector为容器，存取几何对象的指针 */
	std::vector<Geometry3dImpl*> m_vGeoCollection;

	/** @} */ // 属性结尾

public:

	/** @defgroup Group3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	Group3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
	}

	/** 构造函数2
	*/
	Group3dImpl(const vector<Geometry3dImpl*>& geoCollection)
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
		m_vGeoCollection = geoCollection;
	}

	/** 构造函数3
	*/
	Group3dImpl(Geometry3dImpl* const geoCollection, int nNum)
	{
		m_byteTypeID = GEOTYPE_3D_GROUP;
		m_vGeoCollection.reserve(nNum);
		for (int i = 0; i<nNum; i++)
			m_vGeoCollection.push_back(&geoCollection[i]);
	}

	/** 析构函数
	*/
	virtual ~Group3dImpl()
	{
		Clear();
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/
	/**  获取子单元列表
	* @param  [in] f  子单元列表
	* @param  [out] f 子单元列表
	* @return 无
	*/
	virtual void getElements(std::vector<Geometry*> & f){
		for (std::vector<Geometry3dImpl*>::iterator it = m_vGeoCollection.begin(); it != m_vGeoCollection.end(); it++){
			f.push_back((Geometry*)(*it));
		}
	}
	/** 向存储几何对象指针的容器顶端添加一个元素
	* @param [in] pGeometry	待添加的几何对象的指针
	* @return 元素在容器中的序号
	*/
	virtual int addElement(Geometry* const pGeometry)
	{
		return Insert(dynamic_cast<Geometry3dImpl*>(pGeometry));
	}
	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param  无
	* @return 无
	*/
	virtual void removeAll()
	{
		RemoveAll();
	}
	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param  无
	* @return 无
	*/
	virtual void clear()
	{
		Clear();
	}
	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_GROUP:
		{
			*p = (void*)(Group3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** 设置几何组合体
	* @param [in] color	已知几何组合体
	* @return
	*/
	void SetCollection(const vector<Geometry3dImpl*>& geoCollection)
	{
		m_vGeoCollection = geoCollection;
	}

	/** 向存储几何对象指针的容器中添加数据
	* @param [in] geoCollection	指向一串几何对象的指针
	* @param [in] nNum	待添加的几何对象个数
	* @return
	*/
	void SetCollection(Geometry3dImpl* const geoCollection, int nNum)
	{
		m_vGeoCollection.clear();
		m_vGeoCollection.reserve(nNum);
		for (int i = 0; i<nNum; i++)
			m_vGeoCollection.push_back(&geoCollection[i]);
	}

	/** 获得存储几何对象指针容器的引用
	* @param  geoCollection  待赋值的存储几何对象指针容器的引用
	* @return
	*/
	void GetCollection(vector<Geometry3dImpl*>& geoCollection) const
	{
		geoCollection = m_vGeoCollection;
	}

	/** 向存储几何对象指针的容器顶端添加一个元素
	* @param [in] pGeometry	待添加的几何对象的指针
	* @return
	*/
	int Insert(Geometry3dImpl* const pGeometry)
	{
		m_vGeoCollection.push_back(pGeometry);
		m_eEnvelope.Union(pGeometry->getEnvelope());
		return 1;
	}

	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param
	* @return
	*/
	void RemoveAll()
	{
		m_vGeoCollection.clear();
	}

	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param
	* @return
	*/
	void Clear()
	{
		int nCount = m_vGeoCollection.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vGeoCollection[i])
			{
				delete m_vGeoCollection[i];
			}
		}
		RemoveAll();
	}

	/** @} */ // 数据操作组结尾


	/** @name 计算操作
	*  @{
	*/

	/** 判断外部指定的三维点对象与当前几何对象的空间关系
	* @param [in]  vPt Vertex3d类型三维点对象的引用
	* @return 1--点在当前几何对象的内部或边界；0--点在当前几何对象外部
	*/
	int IsPointInGeometry(Vertex3d& vPt) const;

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 指定点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] vPt1	射线起点
	* @param [in] vPt2	射线方向上以点(与vPt1共同决定射线方向)
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int RayCrossGeometry(RayImpl& ray, double& dLength) const;

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
	* @return 当前对象所需内存的大小
	*/
	size_t sizeBuffer();

	/** 从缓冲区中解析当前对象，并给成员变量赋值
	* @param [in] buf	缓冲区Buffer对象的引用
	* @return 1 -- 解析成功；0 -- 解析失败
	*/
	bool readBuffer(Buffer & buf);

	/** 把当前对象的成员变量数据，打包成一个缓冲区
	* @param [out] buf	缓冲区Buffer的引用
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
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 线段到当前几何对象距离的平方
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);

	/** 虚函数，在派生类中实现
	* 计算外部指定的三角形到当前几何对象距离的平方
	* @param [in] triPt1, triPt2, triPt3Vertex3d类型三维点对象的引用(表示三角行三个顶点)
	* @return 三角形到当前几何对象距离的平方
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

	/** 虚函数
	* 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	int FrustumIntersectGeometry(FrustumImpl& frustum);

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


