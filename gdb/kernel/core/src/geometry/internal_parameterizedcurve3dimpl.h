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
#include <utility>
#include "internal_curve3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
enum
{
	PARAMETERIZEDCURVE_INVALID,
	PARAMETERIZEDCURVE_BEZIER,
	PARAMETERIZEDCURVE_BSPLINE,
	PARAMETERIZEDCURVE_NURBS,
	PARAMETERIZEDCURVE_CATMULLROM,
};
//##ModelId=49E3472700AB
class  ParameterizedCurve3dImpl : public Curve3dImpl, virtual public ParameterizedCurve3d
{
	/** @defgroup ParameterizedCurve3dImpl ParameterizedCurve3dImpl-三维参数曲线
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** 曲线类型，可以是 Bezier, B-Spline, NURBS等，它决定了生成曲线所用的参数方程 */
	unsigned char m_iParameterizedCurveType;

	/** 控制点 */
	std::vector<Vertex3d>	m_vControlPoints;

	/** 生成曲线所限定的参数区间，以浮点数对表示 */
	std::pair<float, float> m_paParamInterval;

	/** 生成曲线所采用的参数步长，它可决定生成曲线点的个数，也可由个数反算出步长 */
	float m_fParamStep;

	/** @} */ // 属性结尾


public:

	/** @defgroup ParameterizedCurve3dImpl 操作（成员函数）
	* @{
	*/
	/** 获得曲线类型标记
	* @param
	* @return 曲线类型标记值
	*/
	virtual unsigned char getParameterizedType() const
	{
		return m_iParameterizedCurveType;
	}

	/** 设置曲线类型
	* @param [in] iType	已知曲线类型值
	* @return 无
	*/
	virtual void setParameterizedType(unsigned char iType)
	{
		m_iParameterizedCurveType = iType;
	}

	/** 获得控制点数组
	* @param [out] vControlPoints 将被赋值的控制点数组的引用
	* @return
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** 设置控制点数组
	* @param [in] vControlPoints	已知控制点数组的引用
	* @return
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	ParameterizedCurve3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDCURVE;
	}
	/** 构造函数2
	*/
	ParameterizedCurve3dImpl(vector<Vertex3d>& vControlPoints, pair<float, float>& paParamInterval, float fParamStep, unsigned char iType = PARAMETERIZEDCURVE_INVALID)
		:m_vControlPoints(vControlPoints), m_paParamInterval(paParamInterval), m_fParamStep(fParamStep), m_iParameterizedCurveType(iType)
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDCURVE;
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得曲线类型标记
	* @param
	* @return 曲线类型标记值
	*/
	unsigned char GetType() const
	{
		return m_iParameterizedCurveType;
	}

	/** 获得控制点数组
	* @param [out] vControlPoints 将被赋值的控制点数组的引用
	* @return
	*/
	void GetControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** 获得生成曲线所采用的参数步长
	* @param
	* @return 参数步长值
	*/
	float GetParamStep() const
	{
		return m_fParamStep;
	}

	/** 获得生成曲线所限定的参数区间
	* @param [out] paParamInterval
	* @return 生成曲线所限定的参数区间范围
	*/
	void GetParamInterval(pair<float, float>& paParamInterval) const
	{
		paParamInterval = m_paParamInterval;
	}

	/** 设置曲线类型
	* @param [in] iType	已知曲线类型值
	* @return
	*/
	void SetType(unsigned char iType)
	{
		m_iParameterizedCurveType = iType;
	}

	/** 设置控制点数组
	* @param [in] vControlPoints	已知控制点数组的引用
	* @return
	*/
	void SetControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** 设置生成曲线所采用的参数步长
	* @param [in] fParamStep  将设定的步长值
	* @return
	*/
	void SetParamStep(float fParamStep)
	{
		m_fParamStep = fParamStep;
	}

	/** 设置生成曲线所限定的参数区间
	* @param [in] paParamInterval	已知参数区间的引用
	* @return
	*/
	void SetParamInterval(const pair<float, float>& paParamInterval)
	{
		m_paParamInterval = paParamInterval;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** 生成曲线
	* @param [in] nPnt 曲线上的点数
	* @return 曲线点序列
	*/
	Vertex3d* Generate(int nPnt);

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

	/** 释放对象数据并将指针和变量赋初始值
	* @return 是否执行成功
	*/
	virtual bool Init()
	{
		return true;
	}

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾
};

end_gdb_namespace
end_gtl_namespace


