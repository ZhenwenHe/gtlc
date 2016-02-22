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
#include "quaternion.h"
#include "geometry3d.h"
#include "internal_point3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class Annotation3dImpl : public Point3dImpl, public Annotation3d
{
public:

	/** 获得平移坐标值
	* @param
	* @return	平移坐标值
	*/
	virtual Vertex3d getPosition() const{
		return Vertex3d(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	}
	/** 设定平移坐标值
	* @param [in] pos	三维点
	* @return
	*/
	virtual void setPosition(Vertex3d pos){
		m_vPoint.x = pos.x;
		m_vPoint.y = pos.y;
		m_vPoint.z = pos.z;
	}

	/**  获取方位参数
	* @param  无
	* @return 方位参数
	*/
	virtual Quaternion getOrientation(){
		return Quaternion(m_qOrientation.x(), m_qOrientation.y(), m_qOrientation.z(), m_qOrientation.w());
	}
	/**  设置方位参数
	* @param  [in] q  方位参数
	* @return 无
	*/
	virtual void setOrientation(const Quaternion & q) {
		SetOrientation(Quaternion(q.w(), q.x(), q.y(), q.z()));
	}

	/**  获取注记类型
	* @param  无
	* @return 注记类型
	*/
	virtual long getAnnotationType(){
		return GetAnnoType();
	}
	/**  设置注记类型
	* @param  [in] t  注记类型
	* @return 无
	*/
	virtual void setAnnotationType(long t){
		SetAnnoType(t);
	}

	/**  获取注记风格
	* @param  无
	* @return 注记风格
	*/
	virtual unsigned char getAnnotationStyle() {
		return m_byteStyle;
	}
	/**  设置注记风格
	* @param  [in] c  注记风格
	* @return 无
	*/
	virtual void setAnnotationStyle(unsigned char c){
		m_byteStyle = c;
	}

	/**  获取注记字串
	* @param  [in] 无
	* @return 注记字串
	*/
	virtual std::string getAnnotationString(){
		return m_strAnnoString;
	}
	/**  设置注记字串
	* @param  [in] s  注记字串
	* @return 无
	*/
	virtual void setAnnotationString(const std::string &  s){
		SetAnnoString(s);
	}

	/**  获取字体名称
	* @param  无
	* @return 字体名称
	*/
	virtual std::string getFontName(){
		return GetFontName();
	}
	/**  设置字体名称
	* @param  [in] s  字体名称
	* @return 无
	*/
	virtual void setFontName(const std::string & s){
		SetFontName(s);
	}

	/** 注记字高和字宽：高16为字宽；低16位为字高;0.01mm为单位 */
	/**  获取注记字高
	* @param  无
	* @return 注记字高
	*/
	virtual long getHeight() {
		return GetHeight();
	}
	/**  设置注记字高
	* @param  [in] h  注记字高
	* @return 无
	*/
	virtual void setHeight(long h){
		SetHeight(h);
	}

	/** 注记字串的字宽，对单点注记有效，对布点等无效;0.01mm为单位 */
	/**  获取注记字串的宽度
	* @param  无
	* @return 注记字串的宽度
	*/
	virtual long getWidth(){
		return GetWidth();
	}
	/**  设置注记字串的宽度
	* @param  [in] w  注记字串的宽度
	* @return 无
	*/
	virtual void setWidth(long w){
		SetWidth(w);
	}

	/** 注记字间距;0.01mm为单位 */
	/**  获取注记字间距
	* @param  无
	* @return 注记字间距
	*/
	virtual long getSpaces(){
		return GetSpaces();
	}
	/**  设置注记字间距
	* @param  [in] s  注记字间距
	* @return 无
	*/
	virtual void setSpaces(long s){
		SetSpaces(s);
	}

	/** 注记颜色 */
	/**  获取注记颜色
	* @param  无
	* @return 注记颜色
	*/
	virtual Color4f getColor() {
		Color4f c4f;
		Color::convert(GetColor(), c4f);
		return c4f;
	}
	/**  设置注记颜色
	* @param  [in] clAnnotation  注记颜色
	* @return 无
	*/
	virtual void setColor(const Color4f & c4f){
		long c;
		Color::convert(c4f, c);
		SetColor(c);
	}


	/**  查询对象接口
	* @param  [in] signal  对象类型标识
	* @param  [out] p 对应的对象指针
	* @return 1 -- 查询成功；0 -- 查询失败
	*/
	virtual bool queryInterface(int signal, void ** p){
		if (GEOMTYPE_3D_POINT == signal){
			*p = (void*)(Point3d *) this;
			return true;
		}
		else if (GEOMTYPE_3D_ANNOTATION == signal)
		{
			*p = (void*)(Annotation3d *)this;
			return true;
		}
		else
			return false;
	}
	/** @defgroup Annotation3dImpl Annotation3dImpl-三维注释
	*  @{
	*/
protected:

	/** @name 属性（成员变量）
	* @{
	*/

	/** @brief 旋转分量 */
	Quaternion m_qOrientation;

	/** @brief 注记类型 */
	long m_lAnnoType;

	/** @brief 注记风格 */
	unsigned char m_byteStyle;

	/** @brief 注记字符串 */
	string m_strAnnoString;

	/** @brief 注记字高和字宽：高16为字宽；低16位为字高;0.01mm为单位 */
	string m_strFontName;

	/** @brief 注记字高和字宽：高16为字宽；低16位为字高;0.01mm为单位 */
	long m_lHeight;

	/** @brief 注记字串的字宽，对单点注记有效，对布点等无效;0.01mm为单位 */
	long m_lWidth;

	/** @brief 注记字间距;0.01mm为单位 */
	long m_lSpaces;

	/** @brief 注记颜色 */
	long m_lColor;

	/** @} */ // 属性结尾


	/** @defgroup Annotation3dImpl 操作（成员函数）
	* @{
	*/
public:

	/** @name 构造与析构函数
	*  @{
	*/

	/** 默认构造函数
	*/
	Annotation3dImpl(void);
	/** 默认析构函数
	*/
	virtual ~Annotation3dImpl(void);

	/** @} */ // 构造与析构函数结尾


	/** @name 数据操作
	*  @{
	*/

	/** 获得平移坐标值
	* @param
	* @return	平移坐标值
	*/
	Vertex3d GetPosition() const;
	/** 设定平移坐标值
	* @param [in] pos	三维点
	* @return
	*/
	void SetPosition(Vertex3d pos);

	/** 获得四元数：几何变换中的旋转分量
	* @param
	* @return	四元数值
	*/
	Quaternion GetOrientation() const;
	/** 设定四元数
	* @param [in] orientation	已知四元数
	* @return
	*/
	void SetOrientation(Quaternion orientation);

	/** 获得注记类型
	* @param
	* @return	注记类型
	*/
	long GetAnnoType() const;
	/** 设置注记类型
	* @param [in] lAnnoType	已知注记类型
	* @return
	*/
	void SetAnnoType(long lAnnoType);

	/** 获得字符串内容
	* @param
	* @return	字符串内容
	*/
	std::string GetAnnoString() const;
	/** 获得字符串内容
	* @param [in] annoString	已知字符串
	* @return
	*/
	void SetAnnoString(std::string annoString);

	/** 获得注记字体
	* @param
	* @return	注记字体
	*/
	string GetFontName() const;
	/** 设置注记字体
	* @param [in] fontName	已知注记字体
	* @return
	*/
	void SetFontName(string fontName);

	/** 获得注记风格
	* @param
	* @return 注记风格
	*/
	unsigned char GetStyle() const;
	/** 设置注记风格
	* @param [in] bStyle	已知注记风格
	* @return
	*/
	void SetStyle(unsigned char bStyle);

	/** 获得注记字高
	* @param
	* @return 注记字高
	*/
	long GetHeight() const;
	/** 设置注记字高
	* @param [in] lHeight	已知字高
	* @return
	*/
	void SetHeight(long lHeight);

	/** 获得注记字宽
	* @param
	* @return 注记字宽
	*/
	long GetWidth() const;
	/** 设置注记字高
	* @param [in] width	已知字宽
	* @return
	*/
	void SetWidth(long width);

	/** 获得注记字间距
	* @param
	* @return 注记字间距
	*/
	long GetSpaces() const;
	/** 设置注记字间距
	* @param [in] lSpace	已知字间距
	* @return
	*/
	void SetSpaces(long lSpace);

	/** 获得注记颜色
	* @param
	* @return 注记颜色
	*/
	long GetColor() const;
	/** 设置注记颜色
	* @param [in] color	已知注记颜色
	* @return
	*/
	void SetColor(long color);

	/** @} */ // 数据操作组结尾

public:

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
	* @param [in] vPt	Vertex3d类型三维点对象的引用
	* @return 线段到当前几何对象距离的平方
	*/
	virtual double D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt);


	/** 虚函数，在派生类中实现
	* 计算外部指定的三角形到当前几何对象距离的平方
	* @param [in] triPt1, triPt2, triPt3Vertex3d类型三维点对象的引用(表示三角行三个顶点)
	* @return 线段到当前几何对象距离的平方
	*/
	virtual double D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3);

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
end_gtl_namespace


