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
#include "sharedModel.h"
#include "internal_Point3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

#if defined (_MSC_VER) && (_MSC_VER > 1200)//高于VC6的版本

typedef SharedModelSharedPtr LinkModelSharedPtr;

#endif//高于VC6的版本

class  Reference3dImpl :virtual public Reference3d, virtual public Point3dImpl 
{
	/** @defgroup Reference3dImpl Reference3dImpl-三维参照图元
	*  @{
	*/
protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** @brief 旋转分量 */
	Quaternion m_qOrientation;

	/** @brief 缩放分量 */
	double m_dScale;

	/** @brief 关联文件类型 */
	int m_nLinkType;

	/** @brief 关联对象ID */
	Identifier::raw_type  m_nLinkID;

	/** @brief 关联对象名称 */
	string m_strLinkName;

#if defined (_MSC_VER) && (_MSC_VER > 1200)//高于VC6的版本
	/** @brief 关联对象共享指针 */ //内存由内部自动共同维护( 用m_nLinkID 关联该指针 )
	LinkModelSharedPtr m_pLinkModel;
#else
	/** @brief 关联对象指针 */	//内存由外部管理( 用m_nLinkID 关联该指针 )
	CLinkModel* m_pLinkModel;
#endif//64位ID定义

	/** @brief 连接数据(占用内存)大小 */
	int m_nExtParaSize;

	/** @brief 连接数据 */
	void* m_pExtParaData;

	/** @} */ // 属性结尾

public:

	/** @defgroup Reference3dImpl 操作（成员函数）
	* @{
	*/

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	Reference3dImpl();
	/** 析构函数
	*/
	virtual ~Reference3dImpl();

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得四元数(旋转分量)
	* @param
	* @return 四元数
	*/
	Quaternion GetOrientation() const
	{
		return m_qOrientation;
	}
	/** 设置四元数(旋转分量)
	* @param [in] orientation 已知四元数
	* @return
	*/
	void SetOrientation(Quaternion orientation)
	{
		m_qOrientation = orientation;
	}

	/** 获得关联文件类型
	* @param
	* @return 关联文件类型
	*/
	int GetLinkType() const
	{
		return m_nLinkType;
	}
	/** 设置关联文件类型
	* @param [in] nLinkType 已知关联文件类型
	* @return
	*/
	void SetLinkType(int nLinkType)
	{
		m_nLinkType = nLinkType;
	}

	/** 获得缩放比例
	* @param
	* @return 缩放比例
	*/
	double GetScale() const
	{
		return m_dScale;
	}
	/** 设置缩放比例
	* @param [in] dScale 缩放比例
	* @return
	*/
	void SetScale(double dScale)
	{
		m_dScale = dScale;
	}

	/** 获得关联对象类型
	* @param
	* @return 关联对象类型
	*/
	Identifier::raw_type GetLinkID() const
	{
		return m_nLinkID;
	}
	/** 设置关联对象类型
	* @param [in] nLinkID 关联对象类型
	* @return
	*/
	void SetLinkID(Identifier::raw_type nLinkID)
	{
		m_nLinkID = nLinkID;
	}

	/** 获得关联对象名称
	* @param
	* @return 关联对象名称
	*/
	std::string GetLinkName() const
	{
		return m_strLinkName;
	}
	/** 设置关联对象名称
	* @param [in] strLinkName 关联对象名称
	* @return
	*/
	void SetLinkName(std::string strLinkName)
	{
		m_strLinkName = strLinkName;
	}

#if defined (_MSC_VER) && (_MSC_VER > 1200)//高于VC6的版本
	/** 获得关联模型对象类指针
	* @return  关联模型对象类指针
	*/
	LinkModelSharedPtr GetLinkModel()
	{
		return m_pLinkModel;
	}

	/** 设置关联模型对象类指针
	* @param [in] pLinkModel 关联模型对象类指针
	* @return
	*/
	void SetLinkModel(LinkModelSharedPtr& pLinkModel)
	{
		m_pLinkModel = pLinkModel;
	}
#else
	/** 获得关联模型对象类指针
	* @return  关联模型对象类指针
	*/
	CLinkModel* GetLinkModel()
	{
		return m_pLinkModel;
	}

	/** 设置关联模型对象类指针
	* @param [in] pLinkModel 关联模型对象类指针
	* @return
	*/
	void SetLinkModel(CLinkModel* pLinkModel)
	{
		m_pLinkModel = pLinkModel;
	}
#endif//高于VC6的版本

	/** 获得连接数据大小及数据首地址
	* @param [out] nExtParaSize  连接数据大小
	* @param [out] pExeParaData  连接数据收地址
	* @return
	*/
	void GetExtParaData(int& nExtParaSize, void*& pExeParaData) const;
	/** 设置连接数据大小及数据首地址
	* @param [in] nExtParaSize 连接数据大小
	* @param [in] pExeParaData 连接数据收地址
	* @return
	*/
	void SetExtParaData(int nExtParaSize, void* pExeParaData);

	/** 计算外部指定的三维点对象到当前几何对象距离的平方
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 点到当前几何对象距离的平方
	*/
	double DPointToGeometryEx(Vertex3d& vPt) const;

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

	/**  获取共享模板
	* @param  无
	* @return 共享模板
	*/
	virtual SharedModelSharedPtr  getSharedModel(){
		return  m_pLinkModel;
	}

	/**  设置共享模板
	* @param  [in] spSharedModel  共享模板的共享指针
	* @return 无
	*/
	virtual void  setSharedModel(SharedModelSharedPtr  p) {
		m_pLinkModel = p;
	}
	/**  获取共享模板ID
	* @param  无
	* @return 共享模板ID
	*/
	virtual MDLID  getSharedModelID() {
		return m_nLinkID;
	}

	/**  获取位置坐标
	* @param  无
	* @return 位置坐标
	*/
	virtual Vertex3d  getVertex(){
		Vertex3d v;
		v.x = m_vPoint.x;
		v.y = m_vPoint.y;
		v.z = m_vPoint.z;
		return v;
	}

	/** 获得缩放比例
	* @param
	* @return 缩放比例
	*/
	virtual double getScale() const
	{
		return m_dScale;
	}

	/** 设置缩放比例
	* @param [in] dScale 缩放比例
	* @return
	*/
	virtual void setScale(double dScale)
	{
		m_dScale = dScale;
	}

	/** 获得关联对象名称
	* @param
	* @return 关联对象名称
	*/
	virtual std::string getShareModelName() const
	{
		return m_strLinkName;
	}

	/** 设置关联对象名称
	* @param [in] strLinkName 关联对象名称
	* @return
	*/
	virtual void setShareModelName(std::string strLinkName)
	{
		m_strLinkName = strLinkName;
	}

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		if (signal == GEOMTYPE_3D_REFERENCE){
			*p = (void*)(Reference3d *) this;
			return true;
		}
		else if (GEOMTYPE_3D_POINT == signal){
			*p = (void*)(Point3d *) this;
			return true;
		}
		else
			return false;
	}
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
