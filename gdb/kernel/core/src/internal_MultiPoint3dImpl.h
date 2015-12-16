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
#include "internal_Point3dImpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class MultiPoint3dImpl :virtual public MultiPoint3d, virtual public Point3dImpl
{
public:
	/**  获取点位置坐标列
	* @param  无
	* @return 点位置坐标列
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor(){
		VertexVisitorSharedPtr p = VertexVisitor::create(m_iVertexType);
		p->set(m_vVertices, m_lVertNum, m_iVertexType);
		return p;
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
			case GEOMTYPE_3D_MULTIPOINT:
			{
				*p = (void*)(MultiPoint3d *) this;
				return true;
			}
			default:
			{
				return false;
			}
		}
	}

	/**  获取点的属性
	* @param  [out] size  属性的字节长度
	* @param  [out] att 点的属性
	* @return 无
	*/
	virtual void getAttributes(long & size, void ** att){
		size = m_nAttributeSize;
		*att = m_pAttributes;
	}
	/**  设置点的属性
	* @param  [in] size  属性的字节长度
	* @param  [in] pAttributes 点的属性
	* @return 无
	*/
	virtual void setAttributes(const long  size, void * const att) {
		m_nAttributeSize = size;
		m_pAttributes = att;
	}

	/** @defgroup Point3dImpl MultiPoint3dImpl-三维多点
	*  @{
	*/

	/** 设置顶点类型编码
	* @param [in] iVertexType	 顶点类型编码
	* @return
	*/
	virtual void setVertexType(unsigned char iVertexType)
	{
		SetVertexType(iVertexType);
	};
	/** 获得顶点类型编码
	* @param 无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const
	{
		return m_iVertexType;
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

	/** @brief 属性序列所需内存大小 */
	unsigned int m_nAttributeSize;

	/** @brief 属性序列 */
	void *m_pAttributes;

	/** @brief 每个点的属性个数 */
	long m_lAttrNum;

	/** @} */ // 属性结尾

public:

	/** @defgroup MultiPoint3dImpl 操作（成员函数）
	* @{
	*/

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	MultiPoint3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOINT;

		m_iVertexType = VERTEXTYPE_V3d;
		m_lVertNum = 0;
		m_vVertices = NULL;

		m_nAttributeSize = 0;
		m_pAttributes = NULL;
		m_lAttrNum = 0;
	}

	/** 构造函数2
	*/
	MultiPoint3dImpl(long lVertNum, void* const vVertices) :
		m_lVertNum(lVertNum), m_vVertices(vVertices)
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOINT;

		m_nAttributeSize = 0;
		m_pAttributes = NULL;
		m_lAttrNum = 0;
	}

	/** 析构函数
	*/
	virtual ~MultiPoint3dImpl(void)
	{
		VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);

		if (m_pAttributes)
		{
			delete m_pAttributes;
			m_pAttributes = NULL;
		}
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

	/** 设置三维多点顶点个数和顶点数据
	* @param [in] nVertexNum	三维多点顶点个数
	* @param [in] vVertices	指向一串三维点对象的指针
	* @return
	*/
	void SetVertices(long nVertexNum, void* const vVertices)
	{
		m_vVertices = vVertices;
		m_lVertNum = nVertexNum;
	}

	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @return
	*/
	void GetVertices(void*& vVertices) const
	{
		vVertices = m_vVertices;
	}

	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return
	*/
	void GetVertices(long& lVertNum, void*& vVertices) const
	{
		lVertNum = m_lVertNum;
		vVertices = m_vVertices;
	}

	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** 获得三维多点顶点个数
	* @param
	* @return 三维多点顶点个数
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** 设置属性序列所需内存大小
	* @param [in] nAttributeSize	属性序列大小
	* @return
	*/
	void SetAttrSize(size_t nAttributeSize)
	{
		m_nAttributeSize = nAttributeSize;
	}

	/** 获得属性序列所需内存大小
	* @param [out] nAttributeSize 将被赋值属性序列大小
	* @return
	*/
	void GetAttrSize(size_t& nAttributeSize) const
	{
		nAttributeSize = m_nAttributeSize;
	}

	/** 设置属性序列
	* @param [in] lAttrNum	每个点的属性个数
	* @param [in] pAttributes	属性序列
	* @return
	*/
	void SetAttribute(long lAttrNum, void* const pAttributes)
	{
		m_lAttrNum = lAttrNum;
		m_pAttributes = pAttributes;
	}

	/** 获得属性序列
	* @param [out] nAttributeSize 将被赋值属性序列大小
	* @return
	*/
	void GetAttribute(long& lAttrNum, void*& pAttributes) const
	{
		lAttrNum = m_lAttrNum;
		pAttributes = m_pAttributes;
	}

	/** 设置属性序列
	* @param [in] pAttributes	属性序列
	* @return
	*/
	void SetAttribute(void* const pAttributes)
	{
		m_pAttributes = pAttributes;
	}

	/** 获得属性序列
	* @param [out] pAttributes	属性序列
	* @return
	*/
	void GetAttribute(void*& pAttributes) const
	{
		pAttributes = m_pAttributes;
	}

	/** 单独设置每个点的属性个数
	* @param [in] lAttrNum	每个点的属性个数
	* @return
	*/
	void SetAttributeCount(long lAttrNum)
	{
		m_lAttrNum = lAttrNum;
	}

	/** 单独获取每个点的属性个数
	* @param [out] nAttributeSize 将被赋值属性序列大小
	* @return
	*/
	void GetAttributeCount(long& lAttrNum) const
	{
		lAttrNum = m_lAttrNum;
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
