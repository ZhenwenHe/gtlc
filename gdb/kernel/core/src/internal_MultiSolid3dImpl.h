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
#include "internal_Volume3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
//要贴在哪一个面, 贴的方向, 是否采用透明显示, 
//是否自动生成纹理坐标, 纹理坐标映射效果
//,S 纹理坐标系数, T 纹理坐标系数, S 纹理坐标的偏移量, T 
//纹理坐标的偏移量,  S 方向绕转方式, T 方向绕转方式,纹理模式, 
// 放大滤镜, 缩小滤镜,
//##ModelId=49E347250222
class MultiSolid3dImpl : virtual public cn::edu::cug::gdb::MultiSolid3d,virtual  public Volume3dImpl
{
	/** @defgroup MultiSolid3dImpl MultiSolid3dImpl-三维多体
	*  @{
	*/

private:
	/** @name 属性（成员变量）
	* @{
	*/

	//##ModelId=49E6F6F302DE
	/** 对象的名称 */
	string m_name;

	//##ModelId=49E72F9F01E4
	/** 缺省颜色 */
	unsigned long m_defaultColor;

	//##ModelId=49E6F72B001F
	/** 点色数组 */
	std::vector<unsigned long> m_colors;//点色数组 

	//##ModelId=49E6F73600AB
	/** 是否使用点色 */
	int m_bApplyDotColor;

	//##ModelId=49E6F742003E
	/** 顶点索引数组 */
	std::vector<int> m_indices;// 顶点索引 

	//##ModelId=49E6F74E000F
	/** 是否使用纹理 */
	int m_bApplyTexture; //

	//##ModelId=49E730070399
	/** 纹理对象ID */
	int m_textureId;//ID 

	//##ModelId=49E730120128
	/** 纹理贴图相关参数 */
	float* m_texpara;

	/** @} */ // 属性结尾

public:

	/** @defgroup MultiSolid3dImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	MultiSolid3dImpl()
	{
		m_byteTypeID = GEOTYPE_3D_MULTISOLID;
	}

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/
	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		switch (signal){
		case cn::edu::cug::gdb::GEOMTYPE_3D_MULTISOLID:
		{
			*p = (void*)(cn::edu::cug::gdb::MultiSolid3d *) this;
			return true;
		}
		case cn::edu::cug::gdb::GEOMTYPE_3D_VOLUME:
		{
			*p = (void*)(cn::edu::cug::gdb::Volume3d *) this;
			return true;
		}
		default:
		{
			return false;
		}
		}
	}

	/**  获取组合面列
	* @param  [out] f  组合面列
	* @return 无
	*/
	virtual void getSurfaces(std::vector<cn::edu::cug::gdb::Surface3d*> & f){} //$$

	//##ModelId=49E6F3B3006D
	/** 获取多面体几何上的组合数
	* @return 组合数
	*/
	unsigned int NumberGeometry();

	//##ModelId=49E6F4870222
	/** 要求多边形法线平均/不平均
	* @param [in] bMeanNormal 是否要求法线平均
	*/
	void SetMeanNormal(int bMeanNormal);

	//##ModelId=49E6F81D008C
	/** 获取数据量（点的个数）
	* @return 数据量（点数）
	*/
	unsigned int GetDataSize();

	//##ModelId=49E732A30232
	/** 获取所有对象
	* @param [in] n 对象数目
	* @param [in] pdData 对象缓存
	*/
	void GetAll(unsigned int n, Vertex3d* pdData);

	//##ModelId=49E732B1033C
	/** 设定所有对象
	* @param [in] n 对象数目
	* @param [in] pdData 对象缓存
	*/
	void SetAll(unsigned int n, const Vertex3d* pdData);

	//##ModelId=49E736D101D4
	/** 获取顶点索引尺寸
	* @return 顶点索引尺寸
	*/
	unsigned int GetIndexSize();

	//##ModelId=49E736DA02AF
	/** 获取顶点索引
	* @param [in] nIndexSize 索引数目
	* @param [in] IndexData 索引缓存
	* @return 操作成功与否
	*/
	int GetIndex(int nIndexSize, int* IndexData);

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
end_cug_namespace
end_edu_namespace
end_cn_namespace
