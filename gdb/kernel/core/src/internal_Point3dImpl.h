#pragma once
#include "internal_geometry3dimpl.h"
#include "shadedvertex3d.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Point3dImpl :virtual public Point3d, virtual public Geometry3dImpl
{
	/** @defgroup Point3dImpl Point3dImpl-三维点
	*  @{
	*/
protected:
	/** @name 属性（成员变量）
	* @{
	*/

	/** @brief 三维点 */
	ShadedVertex3d m_vPoint;

	/** @} */ // 属性结尾
public:

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
		else
			return false;
	}

	/**  获取点位置坐标
	* @param  无
	* @return 位置坐标
	*/
	virtual Vertex3d getPosition(){
		return Vertex3d(m_vPoint.x, m_vPoint.y, m_vPoint.z);
	}

	/**  设置点位置坐标
	* @param  [in] vPosition  点位置坐标
	* @return 无
	*/
	virtual void setPosition(const Vertex3d & vPosition){
		m_vPoint.x = vPosition.x;
		m_vPoint.y = vPosition.y;
		m_vPoint.z = vPosition.z;
	}
public:

	/** @defgroup Point3dImpl 操作（成员函数）
	* @{
	*/

	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	Point3dImpl();
	/** 析构函数
	*/
	virtual ~Point3dImpl();

	/** @} */ // 构造与析构函数结尾



	/** @name 数据操作
	*  @{
	*/

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

	/** 虚函数
	* 统计当前对象包含的几何数据
	* @param
	* @return long*:函数内部开辟空间、外部释放
	*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
	*								面对象数，法向数，转化为等价三角
	*								材质数，纹理数，
	*/
	virtual long* GeoStatistics()
	{
		return NULL;
	}

	/** 虚函数
	* 判断当前对象和给定多边形在2D上的关系：相离，相交，包含
	* 判断
	* @param [in] ptArray		多边形
	* @param [in] precisionFlag 精度标志位 0-外包矩形 1-凸包 2-自身几何数据
	* @return 1-相交（临接，包含） 0-相离 -1-其他
	*/
	virtual int RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag = 0)
	{
		return -1;
	}

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
