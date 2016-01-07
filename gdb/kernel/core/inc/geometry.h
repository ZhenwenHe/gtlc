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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h" 
#include "ray.h"
#include "frustum.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 
class Geometry;
typedef std::shared_ptr<Geometry>        GeometrySharedPtr;
typedef Geometry*                             GeometryPtr;
typedef Geometry&                             GeometryRef;
/** @defgroup Geometry Geometry-几何类
*  @{
*/
enum
{

	GEOMTYPE_3D_GEOMETRY,
	GEOMTYPE_3D_POINT,
	GEOMTYPE_3D_VOLUME,
	GEOMTYPE_3D_SURFACE,
	GEOMTYPE_3D_CURVE,
	GEOMTYPE_3D_GROUP,
	GEOMTYPE_3D_VOXEL,
	GEOMTYPE_3D_SOLID,
	GEOMTYPE_3D_MULTISOLID,
	GEOMTYPE_3D_PRISM,
	GEOMTYPE_3D_TETRAHEDRON,
	GEOMTYPE_3D_PARAMETERIZEDSURFACE,
	GEOMTYPE_3D_TRIANGLEMESH,
	GEOMTYPE_3D_TRIANGLE,
	GEOMTYPE_3D_MULTIPOLYGON,
	GEOMTYPE_3D_POLYGON,
	GEOMTYPE_3D_LINEARRING,
	GEOMTYPE_3D_PARAMETERIZEDCURVE,
	GEOMTYPE_3D_LINESTRING,
	GEOMTYPE_3D_MULTIPOLYLINE,
	GEOMTYPE_3D_REFERENCE,
	GEOMTYPE_3D_ANNOTATION,
	GEOMTYPE_3D_PATHLINE,
	GEOMTYPE_3D_PATHPOINT,
	GEOMTYPE_3D_DRAWCAMERA,
	GEOMTYPE_3D_CUBE,
	GEOMTYPE_3D_SINGLEPOINT,
	GEOMTYPE_3D_MULTIPOINT,
	GEOMTYPE_3D_AUXBASEGROUP,
	GEOMTYPE_3D_FEATUREINFOGROUP,

	GEOMTYPE_3D_CUBIOD, //长方体
	GEOMTYPE_3D_IRREGULARVOL,//不规则体
	GEOMTYPE_3D_CYLINDER,//圆柱
	GEOMTYPE_3D_SPHERE,


	GEOMTYPE_2D_GEOMETRY,	//几何对象基类
	GEOMTYPE_2D_GEOMETRY2D,	//二维几何对象基类
	GEOMTYPE_2D_IMAGE,
	GEOMTYPE_2D_POLYGON,
	GEOMTYPE_2D_LINE,
	GEOMTYPE_2D_POLYLINE,
	GEOMTYPE_2D_ANNOTATION,
	GEOMTYPE_2D_POINT,
	GEOMTYPE_2D_POINT_SYMBOL,
	GEOMTYPE_2D_MULTIPOINT,
	GEOMTYPE_2D_LABEL,
	GEOMTYPE_2D_GEOMETRYBAG,

	GEOMTYPE_2D_SYMBOL
};
/** @} */


/** @addtogroup Geometry Geometry-几何类
*  @{
*/
class  GV3DCORE_API Geometry {
public:

public:
	/** @name 静态成员函数说明
	*  @{
	*/

	/** 构造传入的几何类型对应的几何对象
	* @param  [in]  type int 几何对象类型
	* @return 返回构造的几何对象的指针
	*/
	static GeometrySharedPtr create(int type);
	/** 从Buffer中读取信息构建几何对象
	* @param  [in]   buf Buffer & 缓冲区对象引用
	* @return 返回构造的几何对象的指针
	*/
	static GeometrySharedPtr create(Buffer & buf);
	/** @} */
public:
	/**@name 属性获取与设置函数说明
	*  @{
	*/

	/** 获取几何类型
	* @param  [in] 无
	* @return 几何类型
	*/
	virtual int getType() const = 0;
	/** 获取几何对象的状态，每个BIT表示一个状态开关
	* @param  [in] 无
	* @return 几何对象的状态
	*/
	virtual int getStatus() const = 0;
	/** 设置几何对象的状态，每个BIT表示一个状态开关
	* @param  [in] 新的值
	* @return 无
	*/
	virtual void setStatus(int) = 0;
	/** 获取几何对象的名称
	* @param  [in] 无
	* @return  返回几何对象名称
	*/
	virtual std::string getName()const = 0;
	/** 设置几何对象的名称
	* @param  [in] 新的几何对象名称
	* @return 无
	*/
	virtual void setName(std::string) = 0;
	/** 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
	* @param  [in] 无
	* @return 返回边界矩形的引用
	*/
	virtual void getEnvelope(Envelope3d &) = 0;

	/** 设置几何类型，主要供派生类使用
	* @param  [in] type int 几何对象类型
	* @return 无
	*/
	virtual void setType(const int type) = 0;
	/** 设置几何对象的包围边界矩形，主要供派生类使用
	* @param  [in] e3d const Envelope3d & 边界矩形
	* @return 无
	*/
	virtual void setEnvelope(const Envelope3d & e3d) = 0;
	/** 设置几何对象显示状态为可见或不可见
	* @param  [in] flag 可见或不可见标识，true为可见，false为不可见
	* @return 无
	*/
	virtual void setVisible(bool flag) = 0;
	/** 获取几何对象显示状态
	* @param  无
	* @return 可见或不可见标识，true为可见，false为不可见
	*/
	virtual bool isVisible() = 0;
	/** 统计当前对象包含的几何数据
	* @param
	* @return long*:函数内部开辟空间、外部释放
	*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
	*								面对象数，法向数，转化为等价三角
	*								材质数，纹理数
	*/
	virtual long * statistics() = 0;

	/** @} */


public:
	/** @name 供数据读写的操作函数
	*  @{
	*/

	/** 将g所指向的几何对象复制到本几何对象
	* @param  [in] g const Geometry *, 指向待拷贝的集合对象的指针
	* @return 是否拷贝成功
	*/
	virtual bool copy(const Geometry * g) = 0;
	virtual bool copy(const GeometrySharedPtr g);
	/** 克隆本身
	* @param  [in]
	* @return 是否拷贝成功
	*/
	virtual Geometry * clone() = 0;
	/** 获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
	* @param  [in] 无
	* @return 返回几何对象写入到Buffer中所占用的字节数
	*/
	virtual size_t sizeBuffer() = 0;
	/** 从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer(Buffer & buf) = 0;
	/** 将本类的信息写入Buffer中
	* @param  [in,out] buf Buffer & ，缓冲区对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	/* 将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void write(std::ostream & f) = 0;
	/** 从标准输入流中提取本类的信息
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void read(std::istream & f) = 0;
public:
	/** 通过传入的矩阵对几何数据进行变换
	* @param [in] Matrix4x4： m 变换矩阵
	* @return	void
	*/
	virtual void translateFrom(Matrix4x4 & m) = 0;
	/** 查询指定的接口是否存在
	* @param [in] signal	几何类型
	* @param [out] pObj	    几何对象接口
	* @return bool:执行成功返回true，否则返回false
	*/
	virtual bool queryInterface(int signal, void ** pObj);
	/** 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int rayCross(const RaySharedPtr & ray, double& length) const = 0;
	/** 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int frustumIntersect(const FrustumSharedPtr & frustum) = 0;

	/** @} */
};

/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
