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
#include "envelope3d.h"
#include "mathex.h"
#include "matrix4x4.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class OBBoxImpl
{
	/** @defgroup OBBoxImpl OBBoxImpl-朝向包围盒
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	//##ModelId=49E347290280
	/** 最小点 */
	Vertex3d m_VMin;

	//##ModelId=49E347290290
	/** 最大点 */
	Vertex3d m_VMax;

	//##ModelId=49E34729029F
	/** 朝向，以矩阵表示 */
	Matrix3x3 m_vOrientation;

	/** @} */ // 属性结尾

public:

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	OBBoxImpl()
	{
		const double big_number = 1e37f;
		m_VMin.x = m_VMin.y = m_VMin.z = big_number;
		m_VMax.x = m_VMax.y = m_VMax.z = -big_number;
	}

	/** 构造函数2
	*/
	OBBoxImpl(const Vertex3d& vmin, const Vertex3d& vmax, const Matrix3x3& matrix)
	{
		m_VMin = vmin;
		m_VMax = vmax;
		m_vOrientation = matrix;
	}

	/** 构造函数3
	*/
	OBBoxImpl(const OBBoxImpl& box)
	{
		m_VMax = box.m_VMax;
		m_VMin = box.m_VMin;
		m_vOrientation = box.m_vOrientation;
	}


	/** @} */ // 构造与析构函数结尾


	/** @name 数据操作
	*  @{
	*/

	/** 获得AABB的最小顶点位置(X,Y,Z最小）
	* @return AABB的最小顶点位置
	*/
	Vertex3d GetMin() const
	{
		return m_VMin;
	}

	/** 获得AABB的最大顶点位置(X,Y,Z最大）
	* @return AABB的最大顶点位置
	*/
	Vertex3d GetMax() const
	{
		return m_VMax;
	}

	/** 设置AABB的最大顶点位置(X,Y,Z最小）
	* @return AABB的最大顶点位置
	*/
	void SetMax(const Vertex3d& maxV)
	{
		m_VMax = maxV;
	}

	/** 设置AABB的最小顶点位置(X,Y,Z最小）
	* @param [in] minV AABB的最小顶点位置
	*/
	void SetMin(const Vertex3d& minV)
	{
		m_VMin = minV;
	}

	/** 获得AABB的朝向
	* @return AABB的朝向
	*/
	Matrix3x3 GetOrientation() const
	{
		return m_vOrientation;
	}

	/** 设置AABB的朝向
	* @param [in] matrix AABB的朝向
	*/
	void SetOrientation(const Matrix3x3& matrix)
	{
		m_vOrientation = matrix;
	}

	/** @} */ // 数据操作组结尾


	/** @name 计算操作
	*  @{
	*/

	//##ModelId=49E347290271
	/** 计算OBB的长度
	* @return OBB的长度
	*/
	double Length() const;

	//##ModelId=49E3472902B0
	/** 计算OBB的宽度
	* @return OBB的长度
	*/
	double Width() const;

	//##ModelId=49E3472902B1
	/** 计算OBB的宽度
	* @return OBB的高度
	*/
	double Height() const;

	//##ModelId=49EC8E1F035B
	/** 计算OBB的中心点位置
	* @return OBB的中心点
	*/
	Vertex3d CenterPoint() const;


	//##ModelId=49EF0C1401B5
	/** 计算当前OBB与传入OBB是否相交
	* @return 1 -- 相交；0 -- 不相交
	*/
	int IsOBBoxIntersect(const OBBoxImpl& obb) const;

	//##ModelId=49EF0C1401B5
	/** 计算传入点是否位于当前OBB内
	* @return 1 -- 在其内；0 -- 不在其内
	*/
	int IsPointInOBB(const Vertex3d& vPt) const;


	/** @}*/  // 计算操作组结尾

	/** @} */ // 模块结尾
};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
