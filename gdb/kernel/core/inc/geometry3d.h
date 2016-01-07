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
#include "indexbuffer.h"
#include "geometry.h"
#include "sharedmodel.h"
#include "identifier.h"
#include "mesh.h"
#include "terrain.h"
#include "vertexvisitor.h"



begin_gtl_namespace
begin_gdb_namespace

/** @addtogroup Geometry Point3d-三维点类
*  @{
*/
class CORE_API Point3d : virtual public Geometry
{
public:
	/**  获取点位置坐标
	* @param  无
	* @return 位置坐标
	*/
	virtual Vertex3d getPosition() = 0;

	/**  设置点位置坐标
	* @param  [in] vPosition  点位置坐标
	* @return 无
	*/
	virtual void setPosition(const Vertex3d & vPosition) = 0;
};
/** @} */

/** @addtogroup Geometry SinglePoint3d-三维单点类
*  @{
*/
class CORE_API SinglePoint3d : virtual public Point3d
{
public:
	/**  获取点的颜色
	* @param  无
	* @return 点的颜色
	*/
	virtual Color4f getColor() = 0;

	/**  设置点的颜色
	* @param  [in] c4f  点的颜色
	* @return 无
	*/
	virtual void setColor(const Color4f &c4f) = 0;

	/**  获取点的法线
	* @param  无
	* @return 点的法线
	*/
	virtual Vertex3d getNormal() = 0;

	/**  设置点的法线
	* @param  [in] v  点的法线向量
	* @return 无
	*/
	virtual void setNormal(const Vertex3d & v) = 0;

	/**  获取点的大小
	* @param  无
	* @return 点的大小
	*/
	virtual double getPointSize() = 0;

	/**  设置点的大小
	* @param  [in] s  点的大小
	* @return 无
	*/
	virtual void setPointSize(double s) = 0;
};
/** @} */

/** @addtogroup Geometry MultiPoint3d-三维多点类
*  @{
*/
class CORE_API MultiPoint3d : virtual public Point3d
{
public:
	/**  获取点位置坐标列
	* @param  无
	* @return 点位置坐标列
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor() = 0;

	/** 获得顶点类型编码
	* @param 无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const = 0;
	/** 设置顶点类型编码
	* @param [in] iVertexType	 顶点类型编码
	* @return 无
	*/
	virtual void setVertexType(unsigned char iVertexType) = 0;
	/** 获得三维多点顶点个数
	* @param  无
	* @return 三维多点顶点个数
	*/
	virtual long getVerticesNum() const = 0;
	/** 设置三维多点顶点个数和顶点数据
	* @param [in] nVertexNum	三维多点顶点个数
	* @param [in] vVertices	指向一串三维点对象的指针
	* @return 无
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices) = 0;
	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return 无
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const = 0;

	/**  获取点的属性
	* @param  [out] size  属性的字节长度
	* @param  [out] att 点的属性
	* @return 无
	*/
	virtual void getAttributes(long & size, void ** att) = 0;
	/**  设置点的属性
	* @param  [in] size  属性的字节长度
	* @param  [in] pAttributes 点的属性
	* @return 无
	*/
	virtual void setAttributes(const long  size, void* const pAttributes) = 0;
};
/** @} */

/** @addtogroup Geometry Annotation3d-三维注记类
*  @{
*/
class CORE_API Annotation3d : virtual public Point3d
{
public:
	/** 获得平移坐标值
	* @param	无
	* @return	平移坐标值
	*/
	virtual Vertex3d getPosition() const = 0;
	/** 设定平移坐标值
	* @param [in] pos	三维点
	* @return 无
	*/
	virtual void setPosition(Vertex3d pos) = 0;

	/**  获取方位参数
	* @param  无
	* @return 方位参数
	*/
	virtual Quaternion getOrientation() = 0;
	/**  设置方位参数
	* @param  [in] qOrientation  方位参数
	* @return 无
	*/
	virtual void setOrientation(const Quaternion & qOrientation) = 0;

	/**  获取注记类型
	* @param  无
	* @return 注记类型
	*/
	virtual long getAnnotationType() = 0;
	/**  设置注记类型
	* @param  [in] lAnnotationType  注记类型
	* @return 无
	*/
	virtual void setAnnotationType(long lAnnotationType) = 0;

	/**  获取注记风格
	* @param  无
	* @return 注记风格
	*/
	virtual unsigned char getAnnotationStyle() = 0;
	/**  设置注记风格
	* @param  [in] cAnnotationStyle  注记风格
	* @return 无
	*/
	virtual void setAnnotationStyle(unsigned char cAnnotationStyle) = 0;

	/**  获取注记字串
	* @param  无
	* @return 注记字串
	*/
	virtual std::string getAnnotationString() = 0;
	/**  设置注记字串
	* @param  [in] szAnnotationString  注记字串
	* @return 无
	*/
	virtual void setAnnotationString(const std::string &  szAnnotationString) = 0;

	/**  获取字体名称
	* @param  无
	* @return 字体名称
	*/
	virtual std::string getFontName() = 0;
	/**  设置字体名称
	* @param  [in] szFontName  字体名称
	* @return 无
	*/
	virtual void setFontName(const std::string & szFontName) = 0;

	/** 注记字高和字宽：高16为字宽；低16位为字高;0.01mm为单位 */
	/**  获取注记字高
	* @param  无
	* @return 注记字高
	*/
	virtual long getHeight() = 0;
	/**  设置注记字高
	* @param  [in] lHeight  注记字高
	* @return 无
	*/
	virtual void setHeight(long lHeight) = 0;

	/** 注记字串的字宽，对单点注记有效，对布点等无效;0.01mm为单位 */
	/**  获取注记字串的宽度
	* @param  无
	* @return 注记字串的宽度
	*/
	virtual long getWidth() = 0;
	/**  设置注记字串的宽度
	* @param  [in] lWidth  注记字串的宽度
	* @return 无
	*/
	virtual void setWidth(long lWidth) = 0;

	/** 注记字间距;0.01mm为单位 */
	/**  获取注记字间距
	* @param  无
	* @return 注记字间距
	*/
	virtual long getSpaces() = 0;
	/**  设置注记字间距
	* @param  [in] lSpaces  注记字间距
	* @return 无
	*/
	virtual void setSpaces(long lSpaces) = 0;

	/** 注记颜色 */
	/**  获取注记颜色
	* @param  无
	* @return 注记颜色
	*/
	virtual Color4f getColor() = 0;
	/**  设置注记颜色
	* @param  [in] clAnnotation  注记颜色
	* @return 无
	*/
	virtual void setColor(const Color4f & clAnnotation) = 0;
};
/** @} */

/** @addtogroup Geometry Reference3d-三维参照类
*  @{
*/
class CORE_API Reference3d : virtual public Point3d
{
public:
	/**  获取共享模板
	* @param  无
	* @return 共享模板
	*/
	virtual SharedModelSharedPtr  getSharedModel() = 0;
	/**  设置共享模板
	* @param  [in] spSharedModel  共享模板的共享指针
	* @return 无
	*/
	virtual void  setSharedModel(SharedModelSharedPtr spSharedModel) = 0;
	/**  获取共享模板ID
	* @param  无
	* @return 共享模板ID
	*/
	virtual MDLID  getSharedModelID() = 0;

	/** 获得缩放比例
	* @param  无
	* @return 缩放比例
	*/
	virtual double getScale() const = 0;

	/** 设置缩放比例
	* @param [in] dScale 缩放比例
	* @return 无
	*/
	virtual void setScale(double dScale) = 0;

	/** 获得关联对象名称
	* @param  无
	* @return 关联对象名称
	*/
	virtual std::string getShareModelName() const = 0;

	/** 设置关联对象名称
	* @param [in] strLinkName 关联对象名称
	* @return 无
	*/
	virtual void setShareModelName(std::string strLinkName) = 0;
};
/** @} */

/** @addtogroup Geometry Curve3d-三维线类
*  @{
*/
class CORE_API Curve3d : virtual public Geometry
{
public:
	/**  获取对象位置坐标访问器
	* @param  无
	* @return 对象位置坐标访问器指针
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor() = 0;
	/** 获得顶点类型编码
	* @param 无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const = 0;
	/** 设置顶点类型编码
	* @param [in] iVertexType	 顶点类型编码
	* @return 无
	*/
	virtual void setVertexType(unsigned char iVertexType) = 0;
	/** 获得三维顶点个数
	* @param  无
	* @return 三维顶点个数
	*/
	virtual long getVerticesNum() const = 0;
	/** 设置三维顶点个数和顶点数据
	* @param [in] nVertexNum	三维顶点个数
	* @param [in] vVertices	指向一串三维点对象的指针
	* @return 无
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices) = 0;
	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return 无
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const = 0;

	/** 获取线的长度
	* @param  无
	* @return 线长
	*/
	virtual double getLength() = 0;
};
/** @} */

/** @addtogroup Geometry LinearRing3d-三维线环类
*  @{
*/
class CORE_API LinearRing3d : virtual public Curve3d
{
public:
	/**  获取线环标志
	* @param  无
	* @return 1 -- 内环标志；0 -- 外环标志
	*/
	virtual int getRingFlag() = 0;
	/**  设置线环标志
	* @param  [in] nFlag 线环标志 (1 -- 内环标志；0 -- 外环标志)
	* @return 无
	*/
	virtual void setRingFlag(int nFlag) = 0;
};
/** @} */

/** @addtogroup Geometry LineString3d-三维折线类
*  @{
*/
class CORE_API LineString3d : virtual public Curve3d
{
public:

	/**  获取折线宽度
	* @param  [in,out] lineWidth  折线宽度
	* @return 无
	*/
	virtual void getLineWidth(double& lineWidth) = 0;
	/**  设置折线宽度
	* @param  [in] lineWidth  折线宽度
	* @return 无
	*/
	virtual void setLineWidth(double& lineWidth) = 0;
};
/** @} */

/** @addtogroup Geometry MultiPolyline3d-三维复杂折线类
*  @{
*/
class CORE_API MultiPolyline3d : virtual public Curve3d
{
public:
	/** 获取线的宽度
	* @param  [in] lineWidth  折线宽度
	* @return 无
	*/
	virtual void setLineWidth(double& lineWidth) = 0;
	/** 获取线的宽度
	* @param  无
	* @return 线的宽度
	*/
	virtual double getLineWidth() = 0;

	/**  获取组合折线的个数
	* @param  无
	* @return 组合折线的个数
	*/
	virtual long getPolylineNumber() = 0;

	/**  获取折线结构序号列
	* @param  [in] ib  折线结构序号列
	* @return 无
	*/
	virtual void getPolylineIndexBuffer(IndexBuffer & ib) = 0;

	/**  获取组合折线各自的结点数目列
	* @param  [in] ib  组合折线各自的结点数目列
	* @return 无
	*/
	virtual void getVertexNumberPerPolyline(IndexBuffer & ib) = 0;
};
/** @} */

/** @addtogroup Geometry ParameterizedCurve3d-三维参数化曲线类
*  @{
*/
class CORE_API ParameterizedCurve3d : virtual public Curve3d
{
public:
	/** 获得曲线类型标记
	* @param  无
	* @return 曲线类型标记值
	*/
	virtual unsigned char getParameterizedType() const = 0;

	/** 设置曲线类型
	* @param  [in] iType	已知曲线类型值
	* @return 无
	*/
	virtual void setParameterizedType(unsigned char iType) = 0;

	/** 获得控制点数组
	* @param  [out] vControlPoints 将被赋值的控制点数组的引用
	* @return 无
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const = 0;

	/** 设置控制点数组
	* @param  [in] vControlPoints	已知控制点数组的引用
	* @return 无
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints) = 0;
};
/** @} */

/** @addtogroup Geometry Surface3d-三维面类
*  @{
*/
class CORE_API Surface3d : virtual public Geometry
{
public:
	/**  获取对象位置坐标访问器
	* @param  无
	* @return 对象位置坐标访问器指针
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor() = 0;
	/** 获得顶点类型编码
	* @param  无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const = 0;
	/** 设置顶点类型编码
	* @param  [in] iVertexType	 顶点类型编码
	* @return 无
	*/
	virtual void setVertexType(unsigned char iVertexType) = 0;
	/** 获得三维顶点个数
	* @param  无
	* @return 三维顶点个数
	*/
	virtual long getVerticesNum() const = 0;
	/** 设置三维顶点个数和顶点数据
	* @param  [in] nVertexNum	三维顶点个数
	* @param  [in] vVertices	指向一串三维点对象的指针
	* @return 无
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices) = 0;
	/** 获得三维多点顶点数据
	* @param  [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param  [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return 无
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const = 0;

	/**  获取面材质指针
	* @param  无
	* @return 面材质指针
	*/
	virtual MaterialPtr  getMaterial() = 0;

	/**  设置面材质
	* @param  [in] pMaterial 面材质指针
	* @return 无
	*/
	virtual void  setMaterial(MaterialPtr pMaterial) = 0;

	/**  获取材质ID
	* @param  无
	* @return 材质ID
	*/
	virtual Identifier::raw_type  getMaterialID() = 0;

	/** 求面积
	* @param  无
	* @return 面积值
	*/
	virtual double getArea() const = 0;
};
/** @} */

/** @addtogroup Geometry TriangleMesh3d-三维三角形网格类
*  @{
*/
class CORE_API  TriangleMesh3d : virtual public Surface3d
{
public:
	/**  获取三角形网格序号结构
	* @param  [in,out] ib  三角形网格序号结构buffer
	* @return 无
	*/
	virtual void getTriangleIndexBuffer(IndexBuffer & ib) = 0;
	/**  获取三角形网格条带序号结构
	* @param  [in,out] ib  三角形网格条带序号结构buffer
	* @return 无
	*/
	virtual void getTriangleStripIndexBuffer(IndexBuffer & ib) = 0;

	/** 设置三角形索引及索引长度
	* @param [in] lTriangleList  三角形索引序列
	* @param [in] lTriListSize 三角形索引长度
	* @return 无
	*/
	virtual void setTriangles(long* const lTriangleList, long lTriListSize) = 0;
};
/** @} */
/** @addtogroup Geometry Polygon3d-三维多边形类
*  @{
*/
class CORE_API Polygon3d : virtual public Surface3d
{
public:
	/** 获取多边形中环数组
	* @param  [in,out] rings 多边形的环数组
	* @return 无
	*/
	virtual void getRings(std::vector<LinearRing3d*> & rings) = 0;
	/** 设置多边形中环数组
	* @param  [in,out] rings 多边形的环数组
	* @return 无
	*/
	virtual void setRings(std::vector<LinearRing3d*> & rings) = 0;
	/** 向多边形中添加环
	* @param  [in] pRing 环对象指针（添加后该指针由Polygon自行维护）
	* @return 无
	*/
	virtual void addRing(LinearRing3d * pRing) = 0;

	virtual TriangleMesh3d * asTriangleMesh() = 0;
};
/** @} */



/** @addtogroup Geometry ParameterizedSurface3d-三维参数化曲面类
*  @{
*/
class CORE_API ParameterizedSurface3d : virtual public Surface3d
{
public:
	/** 获得参数化曲面类型标记
	* @param  无
	* @return 参数化曲面类型标记值
	*/
	virtual unsigned char getParameterizedType() const = 0;

	/** 设置参数化曲面类型
	* @param [in] iType	已知参数化曲面类型值
	* @return 无
	*/
	virtual void setParameterizedType(unsigned char iType) = 0;

	/** 获得控制点数组
	* @param  [out] vControlPoints 将被赋值的控制点数组的引用
	* @return 无
	*/
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const = 0;

	/** 设置控制点数组
	* @param  [in] vControlPoints	已知控制点数组的引用
	* @return 无
	*/
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints) = 0;
};
/** @} */

/** @addtogroup Geometry MultiPolygon3d-三维多面类
*  @{
*/
class CORE_API MultiPolygon3d : virtual public Surface3d
{
public:
	/**  获取组合多边形面的个数
	* @param  无
	* @return 组合多边形面的个数
	*/
	virtual long getPolygonNumber() = 0;

	/**  获取多面结构序号列
	* @param  [in] ib  面结构序号列
	* @return 无
	*/
	virtual void getPolygonIndexBuffer(IndexBuffer & ib) = 0;
	/**  获取组合多边形各自的结点数目列
	* @param  [in] ib  组合多边形各自的结点数目列
	* @return 无
	*/
	virtual void getVertexNumberPerPolygon(IndexBuffer & ib) = 0;

	/**  设置拓扑信息
	* @param  [in] PolygonNumber  多边形个数
	* @param  [in] VertexNumberPrePolygon  每个多边形的顶点数
	* @param  [in] VertexIndices  多边形顶点索引列表
	* @return 无
	*/
	virtual void setPolyTopos(const long PolygonNumber, const std::vector<long> & VertexNumberPrePolygon, const std::vector<long> & VertexIndices) = 0;
};
/** @} */

/** @addtogroup Geometry Triangle3d-三维三角形类
*  @{
*/
class CORE_API Triangle3d : virtual public Surface3d
{
public:
	/**  设置三角形顶点坐标
	* @param  [in] v  三个顶点坐标
	* @return 无
	*/
	virtual void setVertices(Vertex3d v[3]) = 0;
	/**  获取三角形顶点坐标
	* @param  [in,out] v  三个顶点坐标
	* @return 无
	*/
	virtual void getVertices(Vertex3d v[3]) = 0;

	/**  设置三角形法向
	* @param  [in] vNormal  三角形法向
	* @return 无
	*/
	virtual void setNormal(const Vertex3d & vNormal) = 0;
	/**  获取三角形法向
	* @param  [in,out] vNormal  三角形法向
	* @return 无
	*/
	virtual void getNormal(Vertex3d & vNormal) = 0;

	/** 求三角形周长
	* @param  无
	* @return 周长
	*/
	virtual double getPerimeter() = 0;
};
/** @} */

/** @addtogroup Geometry Rectangle3d-三维矩形类
*  @{
*/
class CORE_API Rectangle3d : virtual public Surface3d
{
public:
	/** 求矩形长度
	* @param  无
	* @return 长度值
	*/
	virtual double getLength() = 0;

	/** 获取矩形宽度
	* @param  无
	* @return 宽度值
	*/
	virtual double getWidth() = 0;
};
/** @} */

/** @addtogroup Geometry Volume3d-三维体类
*  @{
*/
class CORE_API Volume3d : virtual public Geometry
{
public:
	/** 获取对象的体积
	* @param  无
	* @return 对象体积
	*/
	virtual double getVolume() = 0;

	/**  获取对象位置坐标访问器
	* @param  无
	* @return 对象位置坐标访问器指针
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor() = 0;
	/** 获得顶点类型编码
	* @param  无
	* @return 顶点类型编码
	*/
	virtual unsigned char getVertexType() const = 0;
	/** 设置顶点类型编码
	* @param  [in] iVertexType	 顶点类型编码
	* @return 无
	*/
	virtual void setVertexType(unsigned char iVertexType) = 0;
	/** 获得三维顶点个数
	* @param  无
	* @return 三维顶点个数
	*/
	virtual long getVerticesNum() const = 0;
	/** 设置三维顶点个数和顶点数据
	* @param  [in] nVertexNum	三维顶点个数
	* @param  [in] vVertices	指向一串三维点对象的指针
	* @return 无
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices) = 0;
	/** 获得三维多点顶点数据
	* @param [out] vVertices 将被赋值的一串三维点对象指针的引用
	* @param [out] vNum 将被赋值的一串三维点对象指针的引用
	* @return 无
	*/
	virtual void getVertices(long& lVertNum, void*& vVertices) const = 0;
};
/** @} */

/** @addtogroup Geometry Solid3d-三维多面体类
*  @{
*/
class CORE_API Solid3d : virtual public Volume3d
{
public:
	/**  获取组合面列
	* @param  [out] f  组合面列
	* @return 无
	*/
	virtual void getSurfaces(std::vector<Surface3d*> & f) = 0;
	/**  向多面体新增一个边界面
	* @param  [in] type  边界面的类型
	* @return 新增的边界面指针
	*/
	virtual Surface3d * newSurface(int type) = 0;
};
/** @} */

/** @addtogroup Geometry MultiSolid3d-三维复杂多面体类
*  @{
*/
class CORE_API MultiSolid3d : virtual public Volume3d
{
public:
	/**  获取组合面列
	* @param  [out] f  组合面列
	* @return 无
	*/
	virtual void getSurfaces(std::vector<Surface3d*> & f) = 0;
};
/** @} */

/** @addtogroup Geometry Voxel3d-三维体元类
*  @{
*/
class CORE_API Voxel3d : virtual public Volume3d
{
public:
	/** 获得体元类型编号
	* @param  无
	* @return 体元类型编号
	*/
	virtual unsigned char getVolumeTypeID() = 0;
};
/** @} */

/** @addtogroup Geometry Cube3d-三维长方体类
*  @{
*/
class CORE_API Cube3d : virtual public Voxel3d
{
public:
	/** 求取长度
	* @param  无
	* @return 长度值
	*/
	virtual double getLength() const = 0;

	/** 求取宽度
	* @param  无
	* @return 宽度值
	*/
	virtual double getWidth() const = 0;

	/** 求取高度
	* @param  无
	* @return 高度值
	*/
	virtual double getHeight() const = 0;
};
/** @} */

/** @addtogroup Geometry Prism3d-三维棱柱类
*  @{
*/
class CORE_API Prism3d : virtual public Voxel3d
{
public:
	/** 求取长度
	* @param  无
	* @return 长度值
	*/
	virtual double getLength() const = 0;

	/** 求取宽度
	* @param  无
	* @return 宽度值
	*/
	virtual double getWidth() const = 0;

	/** 求取高度
	* @param  无
	* @return 高度值
	*/
	virtual double getHeight() const = 0;

	/** 获得单个底面顶点个数
	* @param  无
	* @return 单个底面顶点个数
	*/
	virtual int getVerNumPerSide() = 0;

	/** 设置单个底面顶点个数
	* @param [in] VerNum  单个底面顶点个数
	* @return 无
	*/
	virtual void setVerNumPerSide(int VerNum) = 0;
};
/** @} */

/** @addtogroup Geometry Tetrahedron3d-三维四面体类
*  @{
*/
class CORE_API Tetrahedron3d : virtual public Voxel3d
{
public:
};
/** @} */

/** @addtogroup Geometry Group3d-三维组类
*  @{
*/
class CORE_API Group3d : virtual public Geometry
{
public:
	/**  获取子单元列表
	* @param  [in,out] f  子单元列表
	* @return 无
	*/
	virtual void getElements(std::vector<Geometry*> & f) = 0;

	/** 向存储几何对象指针的容器顶端添加一个元素
	* @param  [in] pGeometry	待添加的几何对象的指针
	* @return 元素在容器中的序号
	*/
	virtual int addElement(Geometry* const pGeometry) = 0;

	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param  无
	* @return 无
	*/
	virtual void removeAll() = 0;

	/** 清空存储几何对象指针的容器，释放容器中元素指针管理的内存(暂时不释放内存)
	* @param  无
	* @return 无
	*/
	virtual void clear() = 0;
};
/** @} */


end_gdb_namespace
end_gtl_namespace


