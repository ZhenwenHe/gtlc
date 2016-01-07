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
#include "indexbuffer.h"
#include "geometry3d.h"
#include "internal_curve3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class MultiPolyline3dImpl : virtual public MultiPolyline3d, virtual public Curve3dImpl
{
	/** @defgroup MultiPolyline3dImpl MultiPolyline3dImpl-三维多线
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/

	//线对象的名称
	//##ModelId=49E6DE6E01B5
	/** @brief 对象的名称 */
	string m_name;

	//g颜色
	//##ModelId=49E6DF62006D
	/** @brief g颜色 */
	unsigned long m_byG;

	//##ModelId=49E73513037A
	/** @brief 缺省颜色 */
	unsigned long m_defaultColor;

	//##ModelId=49E7373F004E
	/** @brief 线宽 */
	double m_dLineWidth;

	//##ModelId=49E735280119
	/** @brief 点色数组 */
	std::vector<unsigned long> m_color; //点色数组 

	//##ModelId=49E73533036B
	/** @brief 是否使用纹理 */
	int m_bApplyDotColor;

	//##ModelId=49E7353C0213
	/** @brief 顶点索引数组 */
	std::vector<int> m_indices; // 顶点索引

	/** @} */ // 属性结尾

public:

	/** @defgroup MultiPolyline3dImpl 操作（成员函数）
	* @{
	*/

	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case GEOMTYPE_3D_CURVE:
		{
			*p = (void*)(Curve3d *) this;
			return true;
		}
		case GEOMTYPE_3D_MULTIPOLYLINE:
		{
			*p = (void*)(MultiPolyline3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/** 获取线的宽度
	* @param  [in] lineWidth  折线宽度
	* @return 无
	*/
	virtual void setLineWidth(double& lineWidth)
	{
		m_dLineWidth = lineWidth;
	}

	/** 获取线的宽度
	* @param
	* @return 线的宽度
	*/
	virtual double getLineWidth()
	{
		return m_dLineWidth;
	}

	/** 获取线的长度
	* @param
	* @return 线长
	*/
	virtual double getLength(){ return 0.0; }//$$

	/**  获取组合折线的个数
	* @param  无
	* @return 组合折线的个数
	*/
	virtual long getPolylineNumber(){ return 0; }	//$$

	/**  获取折线结构序号列
	* @param  [in] ib  折线结构序号列
	* @return 无
	*/
	virtual void getPolylineIndexBuffer(IndexBuffer & ib){}	//$$

	/**  获取组合折线各自的结点数目列
	* @param  [in] ib  组合折线各自的结点数目列
	* @return 无
	*/
	virtual void getVertexNumberPerPolyline(IndexBuffer & ib){}	//$$

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	MultiPolyline3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTIPOLYLINE;
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

	//##ModelId=49E6DE6101C5
	/** 设置线的宽度
	* @param [in] wWidth 线宽
	* @return 线宽
	*/
	void SetWidth(unsigned int wWidth);

	//##ModelId=49E6DE730119
	/** 获取线的宽度
	* @param
	* @return 线宽
	*/
	unsigned int GetWidth();

	//##ModelId=49E6DE92029F
	/** 获取线的长度
	* @param
	* @return 线长
	*/
	double Length();

	//##ModelId=49E7362E004E
	/** 获取多线几何上的组合数
	* @param
	* @return 组合数
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E73663005D
	/** 获取数据量
	* @param
	* @return 数据量
	*/
	unsigned int GetDataSize();

	//##ModelId=49E73670034B
	/** 获取顶点索引尺寸
	* @param
	* @return 顶点索引尺寸
	*/
	unsigned int GetIndexSize();

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


