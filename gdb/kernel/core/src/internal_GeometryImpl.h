#pragma once
#include "geometry.h" 
#include "internal_OBBoxImpl.h"
#include "internal_AABBoxImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

enum
{
	GEOTYPE_3D_GEOMETRY = GEOMTYPE_3D_GEOMETRY,
	GEOTYPE_3D_POINT,
	GEOTYPE_3D_VOLUME,
	GEOTYPE_3D_SURFACE,
	GEOTYPE_3D_CURVE,
	GEOTYPE_3D_GROUP,
	GEOTYPE_3D_VOXEL,
	GEOTYPE_3D_SOLID,
	GEOTYPE_3D_MULTISOLID,
	GEOTYPE_3D_PRISM,
	GEOTYPE_3D_TETRAHEDRON,
	GEOTYPE_3D_PARAMETERIZEDSURFACE,
	GEOTYPE_3D_TRIANGLEMESH,
	GEOTYPE_3D_TRIANGLE,
	GEOTYPE_3D_MULTIPOLYGON,
	GEOTYPE_3D_POLYGON,
	GEOTYPE_3D_LINEARRING,
	GEOTYPE_3D_PARAMETERIZEDCURVE,
	GEOTYPE_3D_LINESTRING,
	GEOTYPE_3D_MULTIPOLYLINE,
	GEOTYPE_3D_REFERENCE,
	GEOTYPE_3D_ANNOTATION,
	GEOTYPE_3D_PATHLINE,
	GEOTYPE_3D_PATHPOINT,
	GEOTYPE_3D_DRAWCAMERA,
	GEOTYPE_3D_CUBE,
	GEOTYPE_3D_SINGLEPOINT,
	GEOTYPE_3D_MULTIPOINT,
	GEOTYPE_3D_AUXBASEGROUP,
	GEOTYPE_3D_FEATUREINFOGROUP,

	GEOTYPE_3D_CUBIOD, //长方体
	GEOTYPE_3D_IRREGULARVOL,//不规则体
	GEOTYPE_3D_CYLINDER,//圆柱
	GEOTYPE_3D_SPHERE
};

class GeometryImpl : virtual public Geometry
{
	/** @defgroup GeometryImpl GeometryImpl-几何类（顶层抽象类）
	*  @{
	*/

protected:
	/** @name 属性（成员变量）
	* @{
	*/
	/** 几何对象的类型，每个类型对应一个编号 */
	int m_byteTypeID;

	//##ModelId=49E531E4037A
	/** 几何结构的维数 */
	unsigned char m_byteDim;

	/** 被其他对象引用的次数 */
	int	m_nRefCount;

	/** 几何对象标志信息 */
	int m_nObjFlag;

	/** 几何对象绘制模式，此标志为绘制期间使用，不做编辑和保存 */
	int m_nDrawMode;

	/** 几何对象的名字*/
	string m_sObjName;

	/** @} */ // 属性结尾


	/** @defgroup GeometryImpl 操作（成员函数）
	* @{
	*/


	/** @name 构造与析构函数
	*  @{
	*/

protected:

public:
	GeometryImpl();
	virtual ~GeometryImpl();

public:
	/** 静态函数用于创建指定类型的几何对象
	*  根据传入的对象类型，新建一个相应几何体的指针，并开辟空间
	* @param [in] signal
	* -0   GEOTYPE_3D_POINT
	* -1   GEOTYPE_3D_VOLUME
	* -2   GEOTYPE_3D_SURFACE
	* -3   GEOTYPE_3D_CURVE
	* -4   GEOTYPE_3D_GROUP
	* -5   GEOTYPE_3D_VOXEL
	* -6   GEOTYPE_3D_SOLID
	* -7   GEOTYPE_3D_MULTISOLID
	* -8   GEOTYPE_3D_PRISM
	* -9   GEOTYPE_3D_TETRAHEDRON
	* -10  GEOTYPE_3D_PARAMETERIZEDSURFACE
	* -11  GEOTYPE_3D_TRIANGLEMESH
	* -12  GEOTYPE_3D_TRIANGLE
	* -13  GEOTYPE_3D_MULTIPOLYGON
	* -14  GEOTYPE_3D_POLYGON
	* -15  GEOTYPE_3D_LINEARRING
	* -16  GEOTYPE_3D_PARAMETERIZEDCURVE
	* -17  GEOTYPE_3D_LINESTRING
	* -18  GEOTYPE_3D_MULTIPOLYLINE
	* -20  GEOTYPE_3D_REFERENCE
	* -21  GEOTYPE_3D_ANNOTATION
	*/
	static GeometryImpl * create(int signal);

	/** 新建几何对象（根据Buffer中的type实现对象指针的创建及内存的开辟）
	*从Buffer中读取数据，新建一个几何体,调用readBuffer实现给定内存包到创建对象的拷贝
	* @return 对象的指针
	*/
	static GeometryImpl * create(Buffer & buf);


	/** @} */ // 构造与析构函数结尾

public:

	/** @name 数据操作
	*  @{
	*/

	/** 获得对象的几何类型编号
	* @param
	* @return 对象的几何类型编号
	*/
	virtual int getType() const //返回几何类型
	{
		return m_byteTypeID;
	}
	/** 指定对象的几何类型编号
	* @param [in] type,对象的几何类型编号
	*/
	void setType(const int type)
	{
		m_byteTypeID = type;
	}

	/** 指定几何对象的标志信息
	* @param [in] bVisible,几何对象的标志信息
	*/
	void SetVisibleFlag(int bVisible)
	{
		if (bVisible)
			m_nObjFlag |= 0x01;
		else
			m_nObjFlag &= ~0x01;
	}
	/** 获得几何对象的标志信息
	* @param
	* @return 对象的标志信息
	*/
	int GetVisibleFlag()
	{
		if (m_nObjFlag & 0x01)
			return 1;
		else
			return 0;
	}


	/** 获取几何结构的坐标维数
	* @return 几何坐标维数
	*/
	unsigned char GetDimension();

	/** 增加一个引用记数
	* @return
	*/
	void AddRef();

	/** 释放对象（减少一个引用记数）
	* @return
	*/
	void Release();

	/** 获取几何对象绘制模式，此标志为绘制期间使用，不做编辑和保存
	* @return 几何对象绘制模式
	*/
	virtual int GetDrawMode();

	/** 设置几何对象绘制模式，此标志为绘制期间使用，不做编辑和保存
	* @return
	*/
	virtual void SetDrawMode(int nDrawMode);

	/** 获取几何对象标志
	* @return 几何对象标志
	*/
	int GetObjFlag() const;

	/** 设置几何对象标志
	* @return
	*/
	void SetObjFlag(int nObjFlag);

	/** 获得几何对象名称
	* @return
	*/
	void SetObjName(string strName)
	{
		m_sObjName = strName;
	}

	/** 设置几何对象名称
	* @return
	*/
	string GetObjName()
	{
		return m_sObjName;
	}

	/** @} */ // 数据操作组结尾



	/** @name 计算操作
	*  @{
	*/

	/** 将指定对象拷贝给当前对象
	* @return 拷贝对象指针
	*/
	virtual bool copy(GeometryImpl * g);

	/** 创建一个当前对象的副本
	* @return 拷贝对象指针
	*/
	virtual GeometryImpl* Clone();

	/** 将源对象里的属性复制到自身中
	* @param [in] pSrcGeometry	源对象
	* @return
	*/
	virtual void copyAttr(GeometryImpl* pSrcGeometry);

	/** 使用new 操作符创建子类对象示例并返回
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual GeometryImpl* CreateObject() = 0;

	/** 根据传入的变换矩阵，完成对象的几何变换
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual void TranslateFrom(Matrix4x4& m4x4);

	/** 计算当前对象所需内存的大小
	* @param
	* @return 返回类的参数m_nSize
	*/
	virtual size_t sizeBuffer() = 0;

	/** 从缓存读取对象记录
	* @param [in] pBuffer 缓存
	* @return 读取成功与否
	*/
	virtual bool readBuffer(Buffer & buf) = 0;

	/** 将对象写入缓存
	* @param [in] pBuffer 缓存
	* @return 写入成功与否
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;

	/** 实现流由内存写出文件
	* @param [in] f	输出流的引用
	*/
	virtual void write(std::ostream & f);

	/** 从缓冲区中解析当前对象，并给成员变量赋值
	* @param [in] f	输入流的引用
	*/
	virtual void read(std::istream & f);

	/** 获取几何体的包围盒
	* @return AABBoxImpl的引用,几何体的包围盒
	*/
	virtual AABBoxImpl & getEnvelope() = 0;
	/** 设置几何体的包围盒
	* @param [in] e3d	AABBoxImpl的引用,几何体的包围盒
	*/
	virtual void setEnvelope(const AABBoxImpl & e3d) = 0;


	

	/** 得到对象的几何中心
	* @param [out] 以Vertex3d的形式返回
	*/
	virtual Vertex3d CalCorePoint() = 0;

	/** 释放对象数据并将指针和变量赋初始值
	* @return 是否执行成功
	*/
	virtual bool Init(){ return true; }

	virtual long * GeoStatistics(){
		return 0;
	}
public:
	// 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
	// * @param  [in] 无
	// * @return 返回边界矩形的引用
	virtual void getEnvelope(Envelope3d & d)  {
		AABBoxImpl & env3d = getEnvelope();
		Vertex3d vMin = env3d.GetMin();
		Vertex3d vMax = env3d.GetMax();

		d.minx = vMin.x; d.maxx = vMax.x;
		d.miny = vMin.y; d.maxy = vMax.y;
		d.minz = vMin.z; d.maxz = vMax.z;
	}
	// 获取几何对象的状态，每个BIT表示一个状态开关
	// * @param  [in] 无
	// * @return 几何对象的状态
	virtual int getStatus() const 	{
		return 0;
	}
	// 设置几何对象的状态，每个BIT表示一个状态开关
	// * @param  [in] 新的值
	// * @return 无
	//##ModelId=4D21B8750009
	virtual void setStatus(int s) {

	}
	// 获取几何对象的名称
	// * @param  [in] 无
	// * @return  返回几何对象名称
	//##ModelId=4D21B875000C
	virtual std::string getName()const  {
		return m_sObjName;
	}
	// 设置几何对象的名称
	// * @param  [in] 新的几何对象名称
	// * @return 无
	//##ModelId=4D21B875000E
	virtual void setName(std::string s) {
		m_sObjName = s;
	}
	// 将g所指向的几何对象复制到本几何对象
	// * @param  [in] g const Geometry *, 指向待拷贝的集合对象的指针 
	// * @return 是否拷贝成功
	virtual bool copy(const Geometry * g){
		return GeometryImpl::copy(dynamic_cast<const GeometryImpl*>(g));
	}
	// 克隆本身
	// * @param  [in]  
	// * @return 是否拷贝成功
	virtual Geometry * clone(){
		return this->Clone();
	}
	// 设置几何对象的包围边界矩形，主要供派生类使用
	// * @param  [in] e3d const Envelope3d & 边界矩形
	// * @return 无
	virtual void setEnvelope(const Envelope3d & e)  {
		AABBoxImpl   env3d(e.minx, e.miny, e.minz, e.maxx, e.maxy, e.maxz);
		this->setEnvelope(env3d);
	}
	virtual void translateFrom(Matrix4x4& m4x4){
		Matrix4x4 m(m4x4.getData());
		TranslateFrom(m);
	}
	virtual void setVisible(bool b) {
		SetVisibleFlag(b ? 1 : 0);
	}
	virtual bool isVisible() {
		return GetVisibleFlag() == 0 ? false : true;
	}
	virtual long * statistics(){
		return GeoStatistics();
	}
	/** 虚函数
	* 判断射线是否穿过当前几何体，并返回起点到与几何体交点的最小距离
	* @param [in] ray	给定射线
	* @param [out] dLength	射线起点到与几何体交点的最小距离，若不相交则该值取无穷大
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int rayCross(const RaySharedPtr & ray, double& length) const{
		return 0;
	}
	/** @} */

	/** 判断截头椎体与当前几何体是否相交
	* @param [in] frustum	给定椎体
	* @return	1 -- 相交；0 -- 不相交
	*/
	virtual int frustumIntersect(const FrustumSharedPtr & frustum) const
	{
		return 0;
	}

	/** @} */ // 计算操作组结尾

	/** @} */ // 操作结尾

	/** @} */ // 模块结尾
};


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

