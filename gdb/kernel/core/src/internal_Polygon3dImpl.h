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
#include "internal_TriangleMesh3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
typedef struct tagRINGTOPOLOGY //it's a kind of red-black tree
{
	//
	LinearRing3dImpl* m_ptrRing;
	//
	struct tagRINGTOPOLOGY* m_Parent;
	typedef std::vector<tagRINGTOPOLOGY*> _vectorRINGTOP;
	_vectorRINGTOP m_vectorChildRingPtr;
	//
	tagRINGTOPOLOGY()
	{
		m_ptrRing = NULL;
		m_Parent = NULL;
		m_vectorChildRingPtr.clear();
	}
	~tagRINGTOPOLOGY()
	{

	}
}RINGTOP;
typedef std::vector<RINGTOP*> _vectorRINGTOP;

class Polygon3dImpl : virtual public Polygon3d, virtual public Surface3dImpl
{
	/** @defgroup Polygon3dImpl Polygon3dImpl-三维多边形
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 线性环数组，以vector装载指针  */
	typedef std::vector<LinearRing3dImpl*> _vector3DLinearRing;
	_vector3DLinearRing  m_vRings;

	//暂存转换后三角网
	TriangleMesh3dImpl* m_pTriangleMesh;

	/** @} */ // 属性结尾
public:
	virtual void getRings(std::vector<LinearRing3d*> & rings){
		for (std::vector<LinearRing3dImpl*>::iterator it = m_vRings.begin(); it != m_vRings.end(); it++)
			rings.push_back((LinearRing3d*)(*it));
	}
	virtual void addRing(LinearRing3d * pRing){
		LinearRing3dImpl * p = dynamic_cast<LinearRing3dImpl *>(pRing);
		if (p) {
			if (m_pTriangleMesh)
			{
				delete m_pTriangleMesh;
				m_pTriangleMesh = NULL;
			}
			m_vRings.push_back(p);
		}
	}
	virtual void setRings(std::vector<LinearRing3d*> & rings){
		std::vector<LinearRing3dImpl*> rs;
		for (std::vector<LinearRing3d*>::iterator it = rings.begin(); it != rings.end(); it++)
		{
			LinearRing3dImpl * p = dynamic_cast<LinearRing3dImpl *>(*it);
			rs.push_back(p);
		}
		SetRings(rs);
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
			case GEOMTYPE_3D_POLYGON:
			{
				*p = (void*)(Polygon3d *) this;
				return true;
			}
			default:
			{
				return false;
			}
		}
	}
	virtual TriangleMesh3d * asTriangleMesh();
public:

	/** @defgroup Polygon3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	Polygon3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_POLYGON;
		m_pTriangleMesh = NULL;
	}

	/** 构造函数2
	*/
	Polygon3dImpl(vector<LinearRing3dImpl*>& vRings)
	{
		m_byteTypeID = GEOTYPE_3D_POLYGON;
		m_vRings = vRings;
		m_pTriangleMesh = NULL;
	}

	/** 析构函数
	*/
	virtual ~Polygon3dImpl()
	{
		int nCount = m_vRings.size();
		for (int i = nCount - 1; i>-1; i--)
		{
			if (m_vRings[i])
				delete m_vRings[i];
		}
		m_vRings.clear();

		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得线性环数组
	* @param [out] vRings 将被赋值的线性环数组的引用
	* @return
	*/
	void GetRings(vector<LinearRing3dImpl*>& vRings) const
	{
		vRings = m_vRings;
	}

	/** 设置线性环数组
	* @param [in] vRings 已知的线性环数组的引用
	* @return
	*/
	void SetRings(const vector<LinearRing3dImpl*>& vRings)
	{
		if (m_pTriangleMesh)
		{
			delete m_pTriangleMesh;
			m_pTriangleMesh = NULL;
		}

		//////////////////////[2011-11-7,15:53 lizhanglin]//////////////+
		//TODO: 重新对m_vRings进行赋值
		if (vRings.size()>0)
		{
			int nCount = m_vRings.size();
			for (int i = nCount - 1; i>-1; i--)
			{
				if (m_vRings[i])
					delete m_vRings[i];
			}
			m_vRings.clear();

			for (int i = 0; i < vRings.size(); i++)
			{
				LinearRing3dImpl* p3DLinearRing = new LinearRing3dImpl;
				p3DLinearRing->copy(vRings[i]);
				m_vRings.push_back(p3DLinearRing);
			}
		}
		//////////////////////[2011-11-7,15:53 lizhanglin]//////////////-

		// 屏蔽 [2011-11-7 lizhanglin]
		//m_vRings = vRings;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/
	/** 判断点与当前几何体的关系
	* @param [in] vPt	需要判断的点
	* @return 点相对几何体的位置 'i'--内部; 'o'--外部; 'E'--边沿; 'V'--顶点;
	*/
	char  RPointWithGeometry(Vertex3d& vPt) const;

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt);

	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
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

	/** 把当前对象的成员变量数据，打包成一个缓冲区
	* @param [out] buf	缓冲区的引用
	* @return 1 -- 打包成功；0 -- 打包失败
	*/
	bool writeBuffer(Buffer& buf);

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

	/** 虚函数，在派生类中实现
	* 计算当前对象的几何重心
	* @return 对象的几何重心
	*/
	virtual Vertex3d CalCorePoint();

	/** 把当前的Polygon转换为三角形网络，已包含材质和包围盒(支持多个外环情况，不支持环环相交的情况)
	* @param [in] nMode	0--默认(只生成三角网) 1--生成三角网并优化顶点序列 2--生成三角网同时构建三角形条带
	* @param [in] nType	0--默认(内存自行释放) 1--生成Mesh的内存由外部释放
	* @param [in] bIsFoldingTest	0--默认(进行环的自相交测试) 1--(不进行环的自相交测试)
	* @return 当前polygon所构成的三角网络,如果不成功则返回NULL(!!notice!!需要自行析构TriangleMesh3dImpl对象，否则会造成内存泄露)
	*/
	TriangleMesh3dImpl* ConvertToMesh(const int nMode, const int nType, bool bIsFoldingTest = true);

private:
	/** 构建polygon中ring的拓扑结构
	* @param [in] ring -- 需要插入的linearing top--拓扑结构
	* @return true -- 拓扑建立成功; false -- 拓扑错误
	*/
	bool topologyRing(LinearRing3dImpl* ring, RINGTOP* top);

	/** 判断 linearRing refRing 是否包含 linearRing testRing (不考虑环相交情况)
	* @param [in]
	* @return true -- refRing包含testRing; false -- refRing不包含testRing
	*/
	bool IsContain(LinearRing3dImpl* testRing, LinearRing3dImpl* refRing);

	/** 插入polygon中ring拓扑结构的节点
	* @param [in] ring -- 需要插入的linearing top--拓扑结构
	* @return true -- 拓扑建立成功; false -- 拓扑错误
	*/
	bool insertTopNode(LinearRing3dImpl* ring, RINGTOP* top);

	/** 从polygon中找出洞中的种子点
	* @param [in] topPolygon -- polygon中的环拓扑结构
	* @param [out] holemarks -- 洞种子点列表
	*/
	void getPolygonholemarks(RINGTOP* topPolygon, vector<Vertex3d> &holemarks, Vertex3d& refNormal);

	/** 清空拓扑结构
	* @param [in] topPolygon -- polygon中的环拓扑结构
	*/
	void freetopPolygon(RINGTOP* topPolygon);

	/** 比较两个linearRing的外包围盒的大小，返回a是否小于b
	* @param [in] 两个需要比较的环
	* @return true -- a<b; false -- a>=b
	*/
	static bool compLinearRingSize(LinearRing3dImpl*, LinearRing3dImpl*);

	/** 判断ring中的边是否自相交
	* @param [in] verts -- ring中的所有顶点
	* @return true -- 自相交; false -- 不自相交
	*/
	bool isRingSelfIntersect(vector<Vertex3d>& verts);

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
