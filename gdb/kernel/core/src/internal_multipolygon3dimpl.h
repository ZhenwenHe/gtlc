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
#include "internal_linearring3dimpl.h"
#include "internal_surface3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class TriangleMesh3dImpl;

class MultiPolygon3dImpl : virtual public MultiPolygon3d, virtual public Surface3dImpl
{
	/** @defgroup MultiPolygon3dImpl MultiPolygon3dImpl-三维多面
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 多边形个数 */
	long m_lPolygonNum;

	/** 每个多边形的顶点数 */
	long* m_pPolygonVertexNums;

	/** 多边形顶点索引列表 */
	long* m_pPolygonVertexIndexs;

	//暂存转换后三角网
	TriangleMesh3dImpl* pTempMesh;
	TriangleMesh3dImpl* m_pTriangleMesh;

	/* @brief 临时线环容器*/
	std::vector<LinearRing3dImpl*> m_vRings;

	/** @} */ // 属性结尾
public:
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
		case GEOMTYPE_3D_MULTIPOLYGON:
		{
			*p = (void*)(MultiPolygon3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
	virtual Surface3d * asSurface() {
		return dynamic_cast<Surface3d*>((Surface3dImpl*)this);
	}
	virtual long getPolygonNumber() {
		return m_lPolygonNum;
	}
	virtual void getPolygonIndexBuffer(IndexBuffer & ib) {
		long sum = 0;
		for (int i = 0; i<m_lPolygonNum; i++){
			sum += m_pPolygonVertexNums[i];
		}
		ib.clear();
		ib.add(m_pPolygonVertexIndexs, sum);
	}
	virtual void getVertexNumberPerPolygon(IndexBuffer & ib){
		ib.clear();
		ib.add(m_pPolygonVertexNums, m_lPolygonNum);
	}

	// 设置拓扑信息，包括：多边形个数 每个多边形的顶点数 多边形顶点索引列表
	virtual void setPolyTopos(const long PolygonNumber, const std::vector<long> & VertexNumberPrePolygon, const std::vector<long> & VertexIndices)
	{
		if (PolygonNumber <1 || VertexNumberPrePolygon.size()<1 || VertexIndices.size()<1)
		{
			return;
		}

		if (m_pPolygonVertexNums)
		{
			delete[]m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}
		if (m_pPolygonVertexIndexs)
		{
			delete[]m_pPolygonVertexIndexs;
			m_pPolygonVertexIndexs = NULL;
		}

		m_lPolygonNum = PolygonNumber;
		m_pPolygonVertexNums = new long[VertexNumberPrePolygon.size()];
		std::copy(VertexNumberPrePolygon.begin(), VertexNumberPrePolygon.end(), m_pPolygonVertexNums);

		m_pPolygonVertexIndexs = new long[VertexIndices.size()];
		std::copy(VertexIndices.begin(), VertexIndices.end(), m_pPolygonVertexIndexs);
	}

public:

	/** @defgroup MultiPolygon3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	MultiPolygon3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOLYGON;
		m_lPolygonNum = 0;
		m_pPolygonVertexNums = NULL;
		m_pPolygonVertexIndexs = NULL;
		pTempMesh = NULL;
		m_pTriangleMesh = NULL;
	}

	/** 析构函数
	*/
	virtual ~MultiPolygon3dImpl()
	{
		if (m_pPolygonVertexNums)
		{
			delete[]m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}
		if (m_pPolygonVertexIndexs)
		{
			delete[]m_pPolygonVertexIndexs;
			m_pPolygonVertexIndexs = NULL;
		}
		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}
		if (pTempMesh)
		{
			delete pTempMesh;
			pTempMesh = NULL;
		}

		for (int i = 0; i < m_vRings.size(); i++)
		{
			if (m_vRings[i])
			{
				delete m_vRings[i];
				m_vRings[i] = NULL;
			}
			m_vRings.clear();
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

	/** 设置多边形个数
	* @param [in] lPolygonNum	多边形个数
	* @return
	*/
	void SetPolygonNum(long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
	}

	/** 获得多边形个数
	* @param [out] m_lPolygonNum 多边形个数
	* @return
	*/
	long GetPolygonNum() const
	{
		return m_lPolygonNum;
	}

	/** 设置MultiPolygon中多边形个数及每个多边形顶点数
	* @param [in] pPolygonVertexNums  每个多边形顶点数
	* @param [in] lPolygonNum 多边形个数
	* @return
	*/
	void SetPolygonVertexNums(long* const pPolygonVertexNums, long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
		m_pPolygonVertexNums = pPolygonVertexNums;
	}

	/** 获得MultiPolygon中多边形个数及每个多边形顶点数
	* @param [out] pPolygonVertexNums 每个多边形顶点数
	* @param [out] lPolygonNum 多边形个数
	* @return
	*/
	void GetPolygonVertexNums(long*& pPolygonVertexNums, long& lPolygonNum) const
	{
		lPolygonNum = m_lPolygonNum;
		pPolygonVertexNums = m_pPolygonVertexNums;
	}

	/** 设置MultiPolygon中多边形个数及每个多边形顶点数
	* @param [in] pPolygonVertexNums  每个多边形顶点数
	* @param [in] lPolygonNum 多边形个数
	* @return
	*/
	void SetPolygonVertexNumsEx(vector<long> const pPolygonVertexNums)
	{
		m_lPolygonNum = pPolygonVertexNums.size();
		if (m_pPolygonVertexNums != NULL)
		{
			delete m_pPolygonVertexNums;
			m_pPolygonVertexNums = NULL;
		}

		m_pPolygonVertexNums = new long[m_lPolygonNum];
		for (long i = 0; i<m_lPolygonNum; i++)
		{
			m_pPolygonVertexNums[i] = pPolygonVertexNums.at(i);
		}
	}

	/** 获得MultiPolygon中多边形个数及每个多边形顶点数
	* @param [out] pPolygonVertexNums 每个多边形顶点数
	* @param [out] lPolygonNum 多边形个数
	* @return
	*/
	void GetPolygonVertexNumsEx(vector<long>& pPolygonVertexNums) const
	{
		pPolygonVertexNums.reserve(m_lPolygonNum);
		for (long i = 0; i<m_lPolygonNum; i++)
		{
			pPolygonVertexNums.push_back(m_pPolygonVertexNums[i]);
		}
	}

	/** 设置多边形索引号、每个多边形索引个数、多边形个数
	* @param [in] pPolygonVertexIndexs  多边形索引序列
	* @param [in] pPolygonVertexNums 每个多边形索引个数
	* @param [in] lPolygonNum 多边形个数
	* @return
	*/
	void SetPolygonVertexIndexs(long* const pPolygonVertexIndexs, long* const pPolygonVertexNums, long lPolygonNum)
	{
		m_lPolygonNum = lPolygonNum;
		m_pPolygonVertexNums = pPolygonVertexNums;
		m_pPolygonVertexIndexs = pPolygonVertexIndexs;
	}

	/** 获得多边形索引号、每个多边形索引个数、多边形个数
	* @param [out] pPolygonVertexIndexs  多边形索引序列
	* @param [out] pPolygonVertexNums 每个多边形索引个数
	* @param [out] lPolygonNum 多边形个数
	* @return
	*/
	void GetPolygonVertexIndexs(long*& const pPolygonVertexIndexs, long*& const pPolygonVertexNums, long& lPolygonNum) const
	{
		lPolygonNum = m_lPolygonNum;
		pPolygonVertexNums = m_pPolygonVertexNums;
		pPolygonVertexIndexs = m_pPolygonVertexIndexs;
	}

	/** 获得多边形索引个数
	* @return
	*/
	long GetPolygonIndexNum() const
	{
		long NUM = 0;
		long i = 0;
		for (i = 0; i < m_lPolygonNum; i++)
		{
			NUM += m_pPolygonVertexNums[i];
		}
		return NUM;
	}

	/** 获得多边形索引号、每个多边形索引个数、多边形个数
	* @param [out] pPolygonVertexIndexs  多边形索引序列
	* @param [out] pPolygonVertexNums  每个多边形索引个数
	* @return
	*/
	void GetPolygonVertexIndexsEx(vector<long>& pPolygonVertexIndexs, vector<long>& pPolygonVertexNums) const
	{
		pPolygonVertexNums.reserve(m_lPolygonNum);
		long i = 0;
		for (i = 0; i < m_lPolygonNum; i++)
		{
			pPolygonVertexNums.push_back(m_pPolygonVertexNums[i]);
		}

		long Num = GetPolygonIndexNum();
		pPolygonVertexIndexs.reserve(Num);
		for (i = 0; i < Num; i++)
		{
			pPolygonVertexIndexs.push_back(m_pPolygonVertexIndexs[i]);
		}
	}

	/** 把当前的MultiPolygon转换为三角形网络_WTS_0408
	* @param [in] nMode	0--默认(只生成三角网) 1--生成三角网同时构建三角形条带 （当前只能为0）
	* @param [in] nType	0--默认(自行管理内存) 1--外部管理生成的内存
	* @return 当前polygon所构成的三角网络,如果不成功则返回NULL(!!notice!!需要自行析构TriangleMesh3dImpl对象内存)
	*/
	TriangleMesh3dImpl* ConvertToMesh(const int nMode, const int nType);

	/** 转换当前multipolygon为多个Ring_WTS_0408
	* @param [in]
	* @param [out] m_pRings 环指针容器
	* @return 成功返回TRUE
	*/
	bool getRings(std::vector<LinearRing3dImpl*>& m_pRings);

	//##ModelId=49E6F2CE00EA
	/** 获取矩形面积
	* @param
	* @return 面积
	*/
	double Area();

	//##ModelId=49E6F3AA002E
	/** 获取多面体几何上的组合数
	* @param
	* @return 组合数
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E7368D01E4
	/** 获取顶点索引尺寸
	* @param
	* @return 顶点索引尺寸
	*/
	unsigned int GetIndexSize();

	//##ModelId=49E736FC0109
	/** 获取数据量（点数）
	* @param
	* @return 数据量（点数）
	*/
	unsigned int GetDataSize();

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
