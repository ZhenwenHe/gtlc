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
#include "internal_voxel3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class  Cube3dImpl
	: virtual public cn::edu::cug::gdb::Cube3d, public Voxel3dImpl
{
	/** @defgroup Cube3dImpl Cube3dImpl-三维方盒
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 顶点数组（8个），按常规约定排序 */
	Vertex3d m_vVertices[8];

	/** @} */  //属性结尾

public:

	/** @defgroup Cube3dImpl 操作（成员函数）
	* @{
	*/
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
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOXEL:
		{
			*p = (void*)(cn::edu::cug::gdb::Voxel3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_CUBE:
		{
			*p = (void*)(cn::edu::cug::gdb::Cube3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	/** 求取长度
	* @param
	* @return 长度值
	*/
	virtual double getLength() const
	{
		return Length();
	}

	/** 求取宽度
	* @param
	* @return 宽度值
	*/
	virtual double getWidth() const
	{
		return Width();
	}

	/** 求取高度
	* @param
	* @return 高度值
	*/
	virtual double getHeight() const
	{
		return Height();
	}

	/** @name 构造与析构函数
	*  @{
	*/
	Cube3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_CUBE;
		m_iVolumeTypeID = VOXEL_CUBE;
	}
	/** @} */ // 构造与析构函数结尾


	/** @name 数据操作
	*  @{
	*/
	/** 获得顶点坐标
	* @param [out] vVertices	坐标顶点数组首地址的引用
	* @return
	*/
	void SetVertices(Vertex3d*& vVertices)
	{
		for (int i = 0; i<8; i++)
		{
			m_vVertices[i] = vVertices[i];
		}
	}

	/** 获得顶点坐标
	* @param [out] vVertices	坐标顶点数组首地址的引用
	* @return
	*/
	void GetVertices(Vertex3d*& vVertices)
	{
		vVertices = m_vVertices;
	}

	/** @} */ // 数据操作组结尾


	/** @name 计算操作
	*  @{
	*/

	//##ModelId=49E3472800DA
	/** 求取长度
	* @param
	* @return 长度值
	*/
	double Length() const;

	//##ModelId=49E3472800DB
	/** 求取宽度
	* @param
	* @return 宽度值
	*/
	double Width() const;

	//##ModelId=49E3472800DC
	/** 求取高度
	* @param
	* @return 高度值
	*/
	double Height() const;

	//##ModelId=49ECAD70036B
	/** 求取中心点
	* @param
	* @return 中心点
	*/
	Vertex3d Center() const;

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

	/** @} */ //操作结尾

	/** @} */ // 模块结尾
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
