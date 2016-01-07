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
#include "internal_volume3dimpl.h" 


begin_gtl_namespace
begin_gdb_namespace
class Solid3dImpl : virtual public Solid3d,virtual public Volume3dImpl
{
	/** @defgroup Solid3dImpl Solid3dImpl-三维实体
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 边界面列表，以vector容器装载面对象指针 */
	std::vector<Surface3dImpl*> m_vBoundarySurfaces;

	/** @} */ // 属性结尾
public:

	/** @defgroup Solid3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	Solid3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_SOLID;
	}

	/** 构造函数2
	*/
	Solid3dImpl(vector<Surface3dImpl*>& vBoundarySurfaces)
	{
		m_byteTypeID = GEOTYPE_3D_SOLID;
		m_vBoundarySurfaces = vBoundarySurfaces;
	}

	/** 析构函数
	*/
	virtual ~Solid3dImpl()
	{
		int nCount = m_vBoundarySurfaces.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vBoundarySurfaces[i])
				delete m_vBoundarySurfaces[i];
		}
		m_vBoundarySurfaces.clear();
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得边界面列表
	* @param [out] vBoundarySurfaces  待赋值的边界面列表
	* @return
	*/
	void GetBoundarySurfaces(vector<Surface3dImpl*>& vBoundarySurfaces) const
	{
		vBoundarySurfaces = m_vBoundarySurfaces;
	}

	/** 设置边界面列表
	* @param [in] 	vBoundarySurfaces  已知的边界面列表
	* @return
	*/
	void SetBoundarySurfaces(const vector<Surface3dImpl*>& vBoundarySurfaces)
	{
		m_vBoundarySurfaces = vBoundarySurfaces;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** 判断点与当前几何体的关系
	* @param [in] vPt	需要判断的点
	* @return 点相对几何体的位置 'i'--内部; 'o'--外部; 'E'--边沿; 'V'--顶点; 'F'--面;
	*/
	char RPointWithGeometry(Vertex3d& vPt) const;

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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
	* @return 三角形到当前几何对象距离的平方
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);


	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	射线
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
	* @return 当前对象所需内存大小
	*/
	size_t sizeBuffer();

	/** 从缓冲区中解析当前对象，并给成员变量赋值
	* @param [in] buf	缓冲区的引用
	* @return 1 -- 解析成功；0 -- 解析失败
	*/
	bool readBuffer(Buffer & buf);

	/** 虚函数，在派生类中实现
	* 计算当前对象的几何重心
	* @return 对象的几何重心
	*/
	virtual Vertex3d CalCorePoint();

	/** 把当前对象的成员变量数据，打包成一个缓冲区
	* @param [out] buf	缓冲区的引用
	* @return 1 -- 打包成功；0 -- 打包失败
	*/
	bool writeBuffer(Buffer& buf);

	/** 判断截头椎体与当前几何体是否相交
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

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_SOLID:
		{
			*p = (void*)(Solid3d *) this;
			return true;
		}
		case GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(Volume3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	virtual void getSurfaces(std::vector<Surface3d*> & f){
		for (std::vector<Surface3dImpl*>::iterator it = m_vBoundarySurfaces.begin(); it != m_vBoundarySurfaces.end(); it++){
			f.push_back((Surface3d*)(*it));
		}
	}
	virtual Surface3d * newSurface(int type);
};

end_gdb_namespace
end_gtl_namespace


