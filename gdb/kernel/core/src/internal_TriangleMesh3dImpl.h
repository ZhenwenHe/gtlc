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
#include <deque>
#include "geometry3d.h"
#include "internal_Surface3dImpl.h"
#include "internal_LinearRing3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Polygon3dImpl;
class TriangleMesh3dImpl
	: virtual public TriangleMesh3d, virtual public Surface3dImpl
{
	/** @defgroup TriangleMesh3dImpl TriangleMesh3dImpl-三角形网格
	*  @{
	*/
private:
	/** 三角形遍历标示列表 （0-没有遍历 1-遍历），长度为三角形个数 */
	unsigned int* m_uiTriangleMark;

	/** 分组过程中记录的边界的边(存储每条边两个顶点的索引)*/
	std::vector<long> m_vectBoundaryEdges;

	/**存储该边是否已经遍历（0-没有遍历 1-遍历）,该序列的长度等于边界长度的一半*/
	unsigned int* m_uiBoundEdgeMark;

protected:
	/** 三角形索引表 */
	long* m_lTriangleList;

	/** 三角形索引表长度，索引指向顶点列表，长度为三角形个数的3倍 */
	long m_lTriListSize;

	/** 三角形条带索引表 */
	long* m_lStripIndices;

	/** 三角形条带索引表长度，索引指向顶点列表，长度与为三角形个数加上2 */
	long m_lStripListSize;

	/** @} */ // 属性结尾
public:
	/**  获取三角形网格序号结构
	* @param  [in] ib  三角形网格序号结构buffer
	* @param  [out] ib  三角形网格序号结构buffer
	* @return 无
	*/
	virtual void getTriangleIndexBuffer(IndexBuffer & ib){
		ib.clear();
		ib.add(m_lTriangleList, m_lTriListSize);
	}
	/**  获取三角形网格条带序号结构
	* @param  [in] ib  三角形网格条带序号结构buffer
	* @param  [out] ib 三角形网格条带序号结构buffer
	* @return 无
	*/
	virtual void getTriangleStripIndexBuffer(IndexBuffer & ib){
		ib.clear();
		ib.add(m_lStripIndices, m_lStripListSize);
	}
	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_SURFACE:
		{
			*p = (void*)(Surface3d *) this;
			return true;
		}
		case GEOMTYPE_3D_TRIANGLEMESH:
		{
			*p = (void*)(TriangleMesh3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** 设置三角形索引及索引长度
	* @param [in] lTriangleList  三角形索引序列
	* @param [in] lTriListSize 三角形索引长度
	* @return
	*/
	virtual void setTriangles(long* const lTriangleList, long lTriListSize)
	{
		SetTriangles(lTriangleList, lTriListSize);
	}
public:

	/** @defgroup TriangleMesh3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	TriangleMesh3dImpl();
	/** 析构函数
	*/
	virtual ~TriangleMesh3dImpl();

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

	/** 设置顶点列表
	* @param [in]  vVertexList	顶点列表
	* @return
	*/
	void SetVertices(void* const vVertexList)
	{
		m_vVertices = vVertexList;
	}

	/** 设置顶点列表
	* @param [in] vVertexList	顶点列表
	* @param [in] lVertNum	顶点列表长度
	* @return
	*/
	void SetVertices(void* const vVertexList, long lVertNum)
	{
		m_vVertices = vVertexList;
		m_lVertNum = lVertNum;
	}

	/** 设置顶点列表长度
	* @param [in] lVertNum	 顶点列表长度
	* @return
	*/
	void SetVerticesNum(long lVertNum)
	{
		m_lVertNum = lVertNum;
	}

	/** 获得顶点列表1
	* @param [out] vVertexList 顶点列表
	* @return
	*/
	void GetVertices(void*& vVertexList) const
	{
		vVertexList = m_vVertices;
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

	/** 获得顶点列表长度
	* @param
	* @return
	*/
	long GetVerticesNum() const
	{
		return m_lVertNum;
	}

	/** 获得顶点列表2
	* @param [out]  vt3d 顶点列表
	* @return
	*/
	void GetVerticesEx(vector<Vertex3d>& vt3d) const;

	/** 获得三角形索引
	* @param [out] tr3d  三角形索引序列
	* @return
	*/
	void GetTriangles(vector<long>& tr3d)
	{
		tr3d.reserve(m_lTriListSize);
		for (int i = 0; i<m_lTriListSize; i++)
		{
			tr3d.push_back(m_lTriangleList[i]);
		}
	}

	/** 获得三角形条带索引
	* @param [out] st3d  三角形条带索引序列
	* @return
	*/
	void GetStrips(vector<long>& st3d)
	{
		st3d.reserve(m_lStripListSize);
		for (int i = 0; i<m_lStripListSize; i++)
		{
			st3d.push_back(m_lStripIndices[i]);
		}
	}

	/** 设置三角形索引及索引长度
	* @param [in] lTriangleList  三角形索引序列
	* @param [in] lTriListSize 三角形索引长度
	* @return
	*/
	void SetTriangles(long* const lTriangleList, long lTriListSize)
	{
		m_lTriListSize = lTriListSize;
		m_lTriangleList = lTriangleList;
	}

	/** 获得三角形索引及索引长度
	* @param [out] lTriangleList 三角形索引序列
	* @param [out] lTriListSize 三角形索引长度
	* @return
	*/
	void GetTriangles(long*& lTriangleList, long& lTriListSize) const
	{
		lTriListSize = m_lTriListSize;
		lTriangleList = m_lTriangleList;
	}

	/** 设置三角形条带索引及索引长度
	* @param [in] lTriangleList  三角形条带索引序列
	* @param [in] lTriListSize 三角形条带索引长度
	* @return
	*/
	void SetStrips(long* const lStripIndices, long lStripListSize)
	{
		m_lStripIndices = lStripIndices;
		m_lStripListSize = lStripListSize;
	}

	/** 获得三角形条带索引及索引长度
	* @param [out] lTriangleList 三角形条带索引序列
	* @param [out] lTriListSize 三角形条带索引长度
	* @return
	*/
	void GetStrips(long*& lStripIndices, long& lStripListSize) const
	{
		lStripIndices = m_lStripIndices;
		lStripListSize = m_lStripListSize;
	}

	/** 把当前的TriangleMesh转换为Polygon结构，已包含材质和包围盒(不考虑包含多个外环的情况)
	* @param [in] nMode	0--(生成精简的内外环Polygon结构) 1--(快速将每个三角形转换为一个Polygon)
	* @param [out] resPolySoup -- 转换后生成的polygon序列(!!Notice!!使用后要析构掉Polygon，否则会造成内存泄露)
	* @return true -- 转换成功 false -- 转换失败
	*/
	bool ConvertToPoly(const int nMode, vector<Polygon3dImpl*> &resPolySoup);

private:

	/** 根据种子点将三角形进行分组
	* @param [in] lTri 第lTri个三角形（!!notice!!非三角形顶点索引）
	* @return true -- 分组成功 false -- 分组失败
	*/
	bool GrowTriangleGroup(long lTri);

	/** 将m_lBoundaryEdges中的边界重组，生成polygon对象
	* @param [out] newPly 生成的多边形对象
	* @return true -- 分组成功 false -- 分组失败
	*/
	bool TrackBoundaries(Polygon3dImpl*& newPly);

	/** 根据种子边增长寻找共面三角形（找到包含边v1v2但不等于oriTri的共面三角形）
	* @param [in] vNormal 种子面法线
	* @param [in] oriTri 种子三角形索引
	* @param [in] v1 三角形边界顶点1索引
	* @param [in] v2 三角形边界顶点2索引
	* @return true -- 找到共面三角形 false -- 没有找到共面三角形
	*/
	bool TrackTriangleByEdge(Vertex3d vNormal, long oriTri, long v1, long v2);

	/** 根据种子顶点增长寻找边
	* @param [in] toVert to种子
	* @param [in] frmVert from种子
	* @param [in] m_vectRing 一条环中的顶点
	* @return
	*/
	void TrackEdgeByVert(long toVert, long frmVert, std::deque<long>& m_vectRing);

	/** 根据顶点索引生成新的LinearRing
	* @param [in] m_vectRingVerts 环顶点索引
	* @param [in] m_iVertexType 顶点类型
	* @param [in] m_OutorIn 环的类型(0--外环 1--内环)
	* @return 生成的新的LinearRing的指针
	*/
	LinearRing3dImpl* GenNewLinearRing(std::deque<long>& m_vectRingVerts, unsigned char m_iVertexType, int m_OutorIn);

	/** 根据三角形索引计算三角形法线
	* @param [in] lTri 第lTri个三角形（!!notice!!非三角形顶点索引）
	* @return 求得的法线
	*/
	Vertex3d GetTriNormal(long lTri);
public:
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

	//##ModelId=49E3472801C5
	/** 三角网条带化
	* @return 条带化成功与否
	*/
	int Strip();

	//##ModelId=49EF1556000F
	/**
	*	基本无用
	*/
	int Fan();

	/** 对三角网索引进行优化(建议在三角形顶点索引的最大值大于三角形个数+2的情况下使用)
	* @return 列表优化成功与否
	*/
	int OptimizeList();

	/** 判断点与当前几何体的关系
	* @param [in] vPt	需要判断的点
	* @return 点相对几何体的位置 'i'--内部; 'o'--外部; 'E'--边沿; 'V'--顶点; 'F'--面;
	*/
	char RPointWithGeometry(Vertex3d& vPt) const;

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
	* @param [in] precisionFlag 精度标志位 0-外包矩形 1-面凸包 2-自身几何数据
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
