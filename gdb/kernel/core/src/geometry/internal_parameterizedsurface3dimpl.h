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
#include "internal_surface3dimpl.h"



begin_gtl_namespace
begin_gdb_namespace
 
class TriangleMesh3dImpl;

enum
{
	PARAMETERIZEDSURFACE_INVALID,
	PARAMETERIZEDSURFACE_BEZIER,
	PARAMETERIZEDSURFACE_BSPLINE,
	PARAMETERIZEDSURFACE_NURBS,
	PARAMETERIZEDSURFACE_CATMULLROM,
};

//##ModelId=49E347280109
class ParameterizedSurface3dImpl :public Surface3dImpl, virtual public ParameterizedSurface3d
{
	/** @defgroup ParameterizedSurface3dImpl ParameterizedSurface3dImpl-三维参数曲面
	*  @{
	*/

protected:

	/** @name 属性（成员变量）
	* @{
	*/

	/** 曲面类型，可以是 Bezier, B-Spline, NURBS等，它决定了生成曲面所用的参数方程 */
	unsigned char m_iParameterizedSurfaceType;

	/** 曲面控制网格的点数组，具有动态尺寸 */
	std::vector<Vertex3d> m_vControlPoints;

	/** 曲面控制网格u方向点个数 */
	int m_nCtrlPntNumU;

	/** 曲面控制网格v方向点个数 */
	int m_nCtrlPntNumV;

	/** 生成曲面所限定的u参数区间，以浮点数对表示 */
	std::pair<float, float> m_paParamInterU;

	/** 生成曲面所限定的v参数区间，以浮点数对表示 */
	std::pair<float, float> m_paParamInterV;

	/** 生成曲线所采用的u参数步长，它可决定将要生成曲面u方向的点数 */
	float m_fParamStepU;

	/** 生成曲线所采用的v参数步长，它可决定将要生成曲面v方向的点数 */
	float m_fParamStepV;

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
		case GEOMTYPE_3D_PARAMETERIZEDSURFACE:
		{
			*p = (void*)(ParameterizedSurface3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}
public:

	/** @defgroup ParameterizedSurface3dImpl 操作（成员函数）
	* @{
	*/
	/** 获得参数化曲面类型标记
	* @param  无
	* @return 参数化曲面类型标记值
	*/
	virtual unsigned char getParameterizedType() const
	{
		return m_iParameterizedSurfaceType;
	}

	/** 设置参数化曲面类型
	* @param [in] iType	已知参数化曲面类型值
	* @return 无
	*/
	virtual void setParameterizedType(unsigned char iType)
	{
		m_iParameterizedSurfaceType = iType;
	}

	/** 获得控制点数组
	* @param [out] vControlPoints 将被赋值的控制点数组的引用
	* @return
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		GetControlVertices(vControlPoints);
	}

	/** 设置控制点数组
	* @param [in] vControlPoints	已知控制点数组的引用
	* @return
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		SetControlVertices(vControlPoints);
	}

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数1
	*/
	ParameterizedSurface3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDSURFACE;
		m_iParameterizedSurfaceType = PARAMETERIZEDSURFACE_INVALID;
	}

	/** 构造函数2
	*/
	ParameterizedSurface3dImpl(vector<Vertex3d>& vControlPoints, int nCtrlPntNumU, int nCtrlPntNumV, pair<float, float>& paParamInterU, pair<float, float>& paParamInterV,
		float fParamStepU, float fParamStepV, unsigned char iType = PARAMETERIZEDSURFACE_INVALID) :
		m_vControlPoints(vControlPoints), m_nCtrlPntNumU(nCtrlPntNumU), m_nCtrlPntNumV(nCtrlPntNumV), m_paParamInterU(paParamInterU), m_paParamInterV(paParamInterV),
		m_fParamStepU(fParamStepU), m_fParamStepV(fParamStepV)
	{
		m_byteTypeID = GEOTYPE_3D_PARAMETERIZEDSURFACE;
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	/** 获得曲面类型标记
	* @param
	* @return 曲面类型
	*/
	unsigned char GetType() const
	{
		return m_iParameterizedSurfaceType;
	}

	/** 获得曲面控制网格的点数组
	* @param [out] vControlPoints 曲面控制网格的点数组的引用
	* @return
	*/
	void GetControlVertices(vector<Vertex3d>& vControlPoints) const
	{
		vControlPoints = m_vControlPoints;
	}

	/** 获得生成曲线所采用的u,v参数步长
	* @param [out] fParamStepU 将被赋值的u参数步长
	* @param [out] fParamStepV 将被赋值的v参数步长
	* @return
	*/
	void GetParamStep(float& fParamStepU, float& fParamStepV) const
	{
		fParamStepU = m_fParamStepU;
		fParamStepV = m_fParamStepV;
	}

	/** 获得生成曲面所限定的u,v参数区间
	* @param [out] paParamInterU 将被赋值的u参数区间
	* @param [out] paParamInterV 将被赋值的v参数区间
	* @return
	*/
	void GetParamInterval(pair<float, float>& paParamInterU, pair<float, float>& paParamInterV) const
	{
		paParamInterU = m_paParamInterU;
		paParamInterV = m_paParamInterV;
	}

	/** 设置曲面类型参数
	* @param [in] iType 已知的曲面类型参数
	* @return
	*/
	void SetType(unsigned char iType)
	{
		m_iParameterizedSurfaceType = iType;
	}

	/** 获得曲面控制网格的点数组
	* @param [in] vControlPoints 已知的曲面控制网格的点数组的引用
	* @return
	*/
	void SetControlVertices(const vector<Vertex3d>& vControlPoints)
	{
		m_vControlPoints = vControlPoints;
	}

	/** 获得生成曲线所采用的u,v参数步长
	* @param [out] fParamStepU 已知的u参数步长参数
	* @param [out] fParamStepV 已知的v参数步长参数
	* @return
	*/
	void SetParamStep(float fParamStepU, float fParamStepV)
	{
		m_fParamStepU = fParamStepU;
		m_fParamStepV = fParamStepV;
	}

	/** 获得生成曲面所限定的u,v参数区间
	* @param [out] paParamInterU 已知的u参数区间的引用
	* @param [out] paParamInterV 已知的v参数区间的引用
	* @return
	*/
	void SetParamInterval(pair<float, float>& paParamInterU, pair<float, float>& paParamInterV)
	{
		m_paParamInterU = paParamInterU;
		m_paParamInterV = paParamInterV;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** 生成曲面网格，以三角网的形式返回结果
	* @param [in] nPntU 曲面网格u方向的点数
	* @param [in] nPntV 曲面网格v方向的点数
	* @return 曲面的三角网的指针
	*/
	TriangleMesh3dImpl* Generate(int nPntU, int nPntV);

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


