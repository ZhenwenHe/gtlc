#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "Buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "geometry.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


#define FONT_2D_FACESIZE 32
//文本对象中的换行字符
#define GEOMETRY2D_LABEL_NEW_LINE_CHAR '$'

class Element2d;
typedef std::shared_ptr< Element2d >  Element2dSharedPtr;
class ElementCollection2d;
typedef std::shared_ptr< ElementCollection2d >  ElementCollection2dSharedPtr;

class ElementLine2d;
typedef std::shared_ptr< ElementLine2d >  ElementLine2dSharedPtr;
class ElementCircularArc2d;
typedef std::shared_ptr< ElementCircularArc2d >  ElementCircularArc2dSharedPtr;
class ElementEllipticArc2d;
typedef std::shared_ptr< ElementEllipticArc2d >  ElementEllipticArc2dSharedPtr;
class ElementBezierCurve2d;
typedef std::shared_ptr< ElementBezierCurve2d >  ElementBezierCurve2dSharedPtr;

class Path2d;
typedef std::shared_ptr< Path2d >  Path2dSharedPtr;
class PathCollection2d;
typedef std::shared_ptr< PathCollection2d >  PathCollection2dSharedPtr;
class Ring2d;
typedef std::shared_ptr< Ring2d >  Ring2dSharedPtr;
class RingCollection2d;
typedef std::shared_ptr< RingCollection2d >  RingCollection2dSharedPtr;

class Geometry2d;
typedef std::shared_ptr< Geometry2d >  Geometry2dSharedPtr;
class GeometryCollection2d;
typedef std::shared_ptr< GeometryCollection2d >  GeometryCollection2dSharedPtr;
class Pen2d;
typedef std::shared_ptr< Pen2d >  Pen2dSharedPtr;

class GeomPoint2d;
typedef std::shared_ptr< GeomPoint2d >  GeomPoint2dSharedPtr;
class GeomPointSymbol2d;
typedef std::shared_ptr< GeomPointSymbol2d >  GeomPointSymbol2dSharedPtr;
class GeomMultipoint2d;
typedef std::shared_ptr< GeomMultipoint2d >  GeomMultipoint2dSharedPtr;
class GeomLabel2d;
typedef std::shared_ptr< GeomLabel2d >  GeomLabel2dSharedPtr;
class GeomAnnotation2d;
typedef std::shared_ptr< GeomAnnotation2d >  GeomAnnotation2dSharedPtr;
class GeomPolygon2d;
typedef std::shared_ptr< GeomPolygon2d >  GeomPolygon2dSharedPtr;
class GeomPolyline2d;
typedef std::shared_ptr< GeomPolyline2d >  GeomPolyline2dSharedPtr;
class GeomGeometryBag2d;
typedef std::shared_ptr< GeomGeometryBag2d >  GeomGeometryBag2dSharedPtr;
class GeomImage2d;
typedef std::shared_ptr< GeomImage2d >  GeomImage2dSharedPtr;

class GeometrySelectManager2d;
typedef std::shared_ptr< GeometrySelectManager2d >  GeometrySelectManager2dSharedPtr;

class Symbol2d;
typedef std::shared_ptr< Symbol2d >  Symbol2dSharedPtr;
typedef std::vector<Symbol2dSharedPtr> Symbol2dSharedPtrList;

class SymbolLibrary2d;
typedef std::shared_ptr< SymbolLibrary2d >  SymbolLibrary2dSharedPtr;
typedef std::vector<SymbolLibrary2dSharedPtr> SymbolLibrary2dSharedPtrList;

class SymbolManager2d;
typedef std::shared_ptr< SymbolManager2d >  SymbolManager2dSharedPtr;


class Coordsys2d;
typedef Coordsys2d* Coordsys2dPtr;
typedef std::shared_ptr<Coordsys2d> Coordsys2dSharedPtr;


/** @defgroup Geometry2d Element2dType-线段类型
*  @{
*/
enum Element2dType
{
	ELEMENT_TYPE_2D_UNKNOW = 0,
	ELEMENT_TYPE_2D_ELEMENT,		//元素基类 

	ELEMENT_TYPE_2D_LINE,			//线段
	ELEMENT_TYPE_2D_CIRCULARARC,	//圆弧
	ELEMENT_TYPE_2D_ELLIPTICARC,	//椭圆弧
	ELEMENT_TYPE_2D_BEZIERCURVE  	//贝赛尔曲线
};
/** @}*/


/** @addtogroup Geometry2d BezierCurve2dType-样条曲线类型
*  @{
*/
enum BezierCurve2dType
{
	BEZIER_CURVE_2D_UNKNOW = 0,
	BEZIER_CURVE_2D_2_B,			//二次B样条曲线
	BEZIER_CURVE_2D_3_B,			//三次B样条
	BEZIER_CURVE_2D_3,				//三次样条
	BEZIER_CURVE_2D_POLYLINE		//折线
};
/** @}*/

/** @addtogroup Geometry2d MirrorImageType-镜像类型
*  @{
*/
enum MirrorImageType
{
	MIRROR_IMAGE_TYPE_2D_ORIGIN = 0,	//原点镜像
	MIRROR_IMAGE_TYPE_2D_X,				//X轴镜像
	MIRROR_IMAGE_TYPE_2D_Y				//Y轴镜像
};
/** @}*/

/** @addtogroup Geometry2d 符号管理器类型
*  @{
*/
enum Symbol2dManagerType
{
	SYMBOL_MANAGER_TYPE_2D_POINT = 0,	//点符号管理
	SYMBOL_MANAGER_TYPE_2D_LINE,		//线符号管理
	SYMBOL_MANAGER_TYPE_2D_REGION		//填充符号管理
};
/** @}*/

/** @addtogroup Geometry2d 符号排序类型
*  @{
*/
enum SymbolSortType
{
	SYMBOL_SORT_TYPE_2D_BY_ID = 0,	//根据符号的ID排序
	SYMBOL_SORT_TYPE_2D_BY_NAME,	//根据符号的名称排序
	SYMBOL_SORT_TYPE_2D_BY_STDNUM	//根据符号的标准编号排序
};
/** @}*/

/** @addtogroup Geometry2d Point2d类型
*  @{
*/
enum Point2dType
{
	POINT_2D_TYPE_CIRCLE = 0,	//圆点
	POINT_2D_TYPE_SQUARE		//正方形的点
	//POINT_2D_TYPE_TRIANGLE		//三角形的点
};
/** @}*/

/** @addtogroup Geometry2d Element2d 
 *  @{
*/    
class CORE_API Element2d
{
public:
	/**@ 根据指定的元素类型，创建元素
	*  @return Element2dSharedPtr
	*/
	static Element2dSharedPtr create(int type);

	/**@name 指示当前元素几何信息已更改，需要重新计算 边界包围盒
	*  此函数调用后，如果调用getEnvelope，会先调用calculateEnvelope计算
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name 指示当前元素几何信息是否需要重新计算 边界包围盒
	*  @return 是否需要重新计算 边界包围盒
	*/
	virtual bool isDirtyEnvelope( ) = 0;

	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** 获长度
	* @param 无
	* @return 长度
	*/	
	virtual double getLength()=0;
	
	/**       获取几何类型
	* @param  [in] 无
	* @return 几何类型
	*/	
	virtual int getType() = 0;

	/**       获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
	* @param  [in] 无
	* @return 返回边界矩形的引用
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/**       将g所指向的几何对象复制到本对象
	* @param  [in] g Element2dSharedPtr, 指向待拷贝的对象的指针 
	* @return 是否拷贝成功
	*/	
	virtual bool copy( Element2dSharedPtr& g) = 0;

	/** 复制内存
	* @param 无
	* @return Element2d*对象
	*/
	virtual Element2dSharedPtr  clone() = 0;
	
	/**   获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
	*     * @param  [in] 无 
	*     * @return 返回几何对象写入到Buffer中所占用的字节数
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**从Buffer中读取信息填充本几何对象 
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0;
	
	/**将本类的信息写入Buffer中  
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作  
	*  * @param  [in,out] f  std::ostream &，标准输出流对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** 从标准输入流中提取本类的信息   
	*   * @param  [in,out] f  std::istream &，标准输入流对象引用 
	*   * @return 成功返回true，否则返回false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/**@name 接口查询 
	*  @param  [in] signal 
	*  @param  [out] p，查询到的接口指针 
	*  @return 查询是否成功
	*/
	virtual bool queryInterface(int signal, void **p )=0;

protected:
	/** 设置几何类型，主要供派生类使用
	*   * @param  [in] type int 几何对象类型
	*   * @return 无
	*/
	virtual void setType(const int type) = 0;
public:
	/** 设置几何对象的包围边界矩形，主要供派生类使用
	*   * @param  [in] e3d const Envelope3d & 边界矩形
	*   * @return 无
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;

	//////////////////////////////////////////////////////////////////////////
	//////////////////  原Segment部分的接口移到此处  ///////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	/** 获取，设置曲线是否封闭
	* @param [in] b
	* @return 
	*/
	virtual bool isClose() = 0;
	virtual void setClose(bool b) = 0;
	
	/** 线段反向
	* @return
	*/
	virtual void reverseOrientation()=0;
	
	/** 获取线段的起点
	* @return Vertex2d
	*/
	virtual void getFromPoint(Vertex2d& fromPoint)=0;

	/** 获取线段的终点
	* @return Vertex2d
	*/
	virtual void getToPoint(Vertex2d& toPoint)=0;
	
	/** 获取用于显示的线段的点阵
	* @param [in] 无
	* @return 用于显示的线段的点阵的指针
	*/
	virtual VertexCollection2dSharedPtr& getDisplayPointCollection()=0;	
	
	 /**重新计算线段的显示点阵， 计算的同时，也计算边界矩形
	 * @param   
	 * @return 返回
	 */
	virtual void calculateDisplayPtList()=0;
};
/** @}*/

/** @addtogroup Geometry2d ElementCollection2d-几何元素集类，可以用来管理所有从GeometryElement2d继承的几何元素集
*  @{
*/
class CORE_API ElementCollection2d 
{
public:
	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的 边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void write(std::ostream & f)=0;
	/**从标准输入流中提取本类的信息 
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void read(std::istream & f)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** 获取对象写入到Buffer中所占用的字节数
	* @param 无
	* @return 获取到的buffer字节数
	*/
	virtual size_t sizeBuffer()=0 ;

	/** 添加元素到元素集中
	* 如果before, after都为空，则添加到元素集的末尾
	* @param [in] pt，要插入的元素
	* @param [in] before，在此索引之前插入
	* @param [in] after，在此索引之后插入
	* @return
	*/
	virtual void addElement(Element2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** 将目标元素集中的元素添加到当前元素集的末尾
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool addCollection(ElementCollection2dSharedPtr&  newElements)=0;

	/** 将目标元素集中的元素插入到当前元素集中
	* @param [in] index，插入位置
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool insertCollection(long index,ElementCollection2dSharedPtr&  newElements)=0;

	/** 获取指定索引的元素
	* @param [in] index: 要获取的元素的索引
	* @return: 获取到的元素的指针，获取失败则返回NULL
	*/
	virtual Element2dSharedPtr&  getElement(long index)=0;

	/** 获取元素的个数
	* @return: 元素的个数
	*/
	virtual long getCount()=0;

	/** 交换元素集中的两个元素
	* @param [in] index1,index2: 要交换的元素在数组中的索引
	* @return: 交换成功返回true，否则返回false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** 从元素集中移除指定索引的元素，并返回移除的元素
	* @param [in] index: 要移除的元素的索引
	* @return: 移除的元素的指针，失败则返回NULL
	*/
	virtual Element2dSharedPtr removeElement(long index)=0;

	/** 从元素集中删除指定索引的元素
	* @param [in] index: 要删除的元素的索引
	* @return: 删除失败则返回false
	*/
	virtual bool deleteElement(long index)=0;

	/** 清空当前元素集
	* @return
	*/
	virtual void clear()=0;

	/** @ 将目标数组 g 中的拷贝到 当前数组中 
	* @param  [in] g , ElementCollection2d *, 指向待拷贝的数组的指针 
	* @return 是否拷贝成功
	*/
	virtual bool copy(ElementCollection2dSharedPtr& g) = 0;
	
	
	/** @  设置当前元素集合，是否有类型限制，如果没有，则可以添加任意类型的元素
	* 如果有，则只能添加m_allowedElementTypeList中允许的元素类型
	* @param  [in] elementTypeLimited: 是否有类型限制
	* @return 
	*/
	virtual void setElementTypeLimited(bool elementTypeLimited) = 0;
	
	/** @  当前元素集合，是否有类型限制，如果没有，则可以添加任意类型的元素
	* 如果有，则只能添加m_allowedElementTypeList中允许的元素类型
	* @param  [in] 
	* @return 是否有类型限制
	*/
	virtual bool isElementTypeLimited() = 0;
	
	/** @  当前元素类型elementType，能否被添加到当前元素集合中
	* @param  [in] elementType: 元素类型
	* @return bool 能否被添加
	*/
	virtual bool canBeAddIn(int elementType) = 0;
	
	/** @  获取可添加元素类型的个数
	* @param  [in] 
	* @return 可添加元素类型的个数
	*/
	virtual long getAllowedElementTypeCount() = 0;
	
	/** @  添加 可添加的元素类型
	* @param  [in] elementType: 可添加的元素类型
	* @return 
	*/
	virtual void addAllowedElementType(int elementType) = 0;
	
	/** @  获取指定索引的 可添加的元素类型
	* @param  [in] lIndex: 索引
	* @return 元素类型
	*/
	virtual int getAllowedElementType(long lIndex) = 0;
	
	/** @  从可添加元素类型列表中，移除指定的 可添加的元素类型elementType
	* 如果指定类型在列表中本来就不存在，则不做任何事
	* @param  [in] elementType: 要移除的元素类型
	* @return 
	*/
	virtual void removeAllowedElementType(int elementType) = 0;
	
	/** @  清除可添加元素类型列表中
	* @return 
	*/
	virtual void clearAllowedElementType() = 0;
};
/** @}*/


/** @addtogroup Geometry2d Font2d-字体结构体
*  @{
*/
#define GV_2D_FACE_SIZE 32
struct Font2d
{
	long      lfHeight;						//字高
	long      lfWidth;						//字宽
	long      lfEscapement;					//
	long      lfOrientation;				//旋转方向
	long      lfWeight;						//字的重量（粗细）
	long      lfItalic;						//斜体
	long      lfUnderline;					//下划线
	long      lfStrikeOut;					//突出
	long      lfCharSet;					//字符集
	long      lfOutPrecision;				//
	long      lfClipPrecision;				//剪切的精度
	long      lfQuality;					//品质
	long      lfPitchAndFamily;				//
	char      lfFaceName[GV_2D_FACE_SIZE];		//字体名称
};
/** @}*/

/** @addtogroup Geometry2d ElementLine2d-直线段类
*  @{
*/
class CORE_API ElementLine2d : virtual public Element2d
{
public:	
	/** 获取组成线段的点阵
	* @return 组成线段点阵的指针
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;	
};
/** @}*/


/** @addtogroup Geometry2d ElementCircularArc2d-圆弧类
*  @{
*/
class CORE_API ElementCircularArc2d : virtual public Element2d
{
public:	
	/** 设置圆弧的参数：圆心
	* @param [in] Vertex2d& pt 圆心坐标
	* @return 
	*/
	virtual void setCenterPoint(Vertex2d& pt) = 0;
	
	/** 设置圆弧的参数：圆心
	* @param [in]  圆心坐标, x, y
	* @return 
	*/
	virtual void setCenterPoint(double x,double y) = 0;

	/** 设置圆弧的参数：半径
	* @param [in] 半径r
	* @return 
	*/
	virtual void setRadius(double r) = 0;

	/** 设置圆弧的参数：起始角（角度单位）
	* @param [in] 起始角（角度单位）fromAng
	* @return 
	*/
	virtual void setFromAngleDeg(double fromAng) = 0;

	/** 设置圆弧的参数：扫描角度（角度单位）
	* @param [in] 扫描角度（角度单位）centralAng
	* @return 
	*/
	virtual void setCentralAngleDeg(double centralAng) = 0;
	//virtual void setClose(bool b) = 0;

	/** 设置圆弧的参数：圆心，半径，起始角（角度单位），扫描角度（角度单位），是否封闭
	* @param [in] Vertex2d& ptCenter,double radius,double fromAngle,double centralAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(Vertex2d& ptCenter,double radius,double fromAngle,double centralAngle,bool isClose) = 0;

	/** 设置圆弧的参数：圆心，半径，起始角（角度单位），扫描角度（角度单位），是否封闭
	* @param [in] double centerX,double centerY,double radius,double fromAngle,double centralAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(double centerX,double centerY,double radius,double fromAngle,double centralAngle,bool isClose) = 0;


	/** 获取圆弧的参数：圆心
	* @return 圆弧的参数：圆心Vertex2d
	*/
	virtual const Vertex2d& getCenterPoint() = 0;

	/** 获取圆弧的参数：圆心
	* @param [out] 圆心 pt
	*/
	virtual void getCenterPoint(Vertex2d& pt) = 0;

	/** 获取圆弧的参数：半径
	* @return 圆弧的参数：半径
	*/
	virtual double  getRadius() = 0;

	/** 获取圆弧的参数：起始角（角度单位）
	* @return 圆弧的参数：起始角（角度单位）
	*/
	virtual double  getFromAngleDeg() = 0;

	/** 获取圆弧的参数：扫描角度（角度单位）
	* @return 圆弧的参数：扫描角度（角度单位）
	*/
	virtual double  getCentralAngleDeg() = 0;
};
/** @}*/

/** @addtogroup Geometry2d ElementEllipticArc2d
*  @{
*/
class CORE_API ElementEllipticArc2d : virtual public Element2d
{
public:
	/** 设置圆弧的参数：圆心
	* @param [in] 圆心pt
	* @return 
	*/
	virtual void setCenterPoint(Vertex2d& pt) = 0;

	/** 设置圆弧的参数：圆心
	* @param [in] 圆心:x,y
	* @return 
	*/
	virtual void setCenterPoint(double x,double y) = 0;

	/** 设置圆弧的参数：长轴半径
	* @param [in] 长轴半径semiMajor
	* @return 
	*/
	virtual void setSemiMajor(double semiMajor) = 0;

	/** 设置圆弧的参数：短轴半径
	* @param [in] 短轴半径semiMinor
	* @return 
	*/
	virtual void setSemiMinor(double semiMinor) = 0;

	/** 设置圆弧的参数：起始角（角度单位）
	* @param [in] 起始角（角度单位）fromAng
	* @return 
	*/
	virtual void setFromAngleDeg(double fromAng) = 0;

	/** 设置圆弧的参数： 扫描角度（角度单位）
	* @param [in] 扫描角度（角度单位）centralAng
	* @return 
	*/
	virtual void setCentralAngleDeg(double centralAng) = 0;

	/** 设置圆弧的参数：旋转角度
	* @param [in] 旋转角度rotateAng
	* @return 
	*/
	virtual void setRotateAngleDeg(double rotateAng) = 0;

	/** 设置圆弧的参数：圆心，长轴半径，短轴半径，起始角（角度单位），
	* 扫描角度（角度单位），旋转角度，是否封闭
	* @param [in] Vertex2d& center,double semiMajor,double minorMajorRatio
	* @param [in] double fromAngle,double centralAngle,double rotationAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(Vertex2d& center,double semiMajor,double minorMajorRatio,
		double fromAngle,double centralAngle,double rotationAngle,bool isClose) = 0;

	/** 设置圆弧的参数：圆心，长轴半径，短轴半径，起始角（角度单位），
	* 扫描角度（角度单位），旋转角度，是否封闭
	* @param [in] double centerX,double centerY,double semiMajor,double minorMajorRatio
	* @param [in] double fromAngle,double centralAngle,double rotationAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(double centerX,double centerY,double semiMajor,double minorMajorRatio,
		double fromAngle,double centralAngle,double rotationAngle,bool isClose) = 0;

	/** 获取圆弧的参数：圆心
	* @return 圆心Vertex2d
	*/
	virtual const Vertex2d& getCenterPoint() = 0;

	/** 获取圆弧的参数：圆心
	* @param [out] 圆心pt
	* @return 
	*/
	virtual void getCenterPoint(Vertex2d& pt) = 0;

	/** 获取圆弧的参数：长轴半径
	* @return 长轴半径
	*/
	virtual double getSemiMajor() = 0;

	/** 获取圆弧的参数：短轴半径
	* @return 短轴半径
	*/
	virtual double getSemiMinor() = 0;

	/** 获取圆弧的参数：扁率
	* @return 扁率
	*/
	virtual double getMinorMajorRatio() = 0;

	/** 获取圆弧的参数：起始角（角度单位）
	* @return 起始角（角度单位）
	*/
	virtual double getFromAngleDeg() = 0;

	/** 获取圆弧的参数：扫描角度（角度单位）
	* @return 扫描角度（角度单位）
	*/
	virtual double getCentralAngleDeg() = 0;

	/** 获取圆弧的参数：旋转角度
	* @return 旋转角度
	*/
	virtual double getRotateAngleDeg() = 0;
	
	/** 通过给定的角度dAngleDeg（角度单位）,计算椭圆在该角度的点的坐标
	* @param [in]  给定的角度dAngleDeg，椭圆参数方程中的角度
	* @param [out] 指定角度上对应的点坐标x,y
	* @return 
	*/	
	virtual void calculatePointByAngleDeg(double dAngleDeg, double &xx, double &yy) = 0;
	
};
/** @}*/

/** @addtogroup Geometry2d ElementBezierCurve2d-贝塞尔曲线
*  @{
*/
class CORE_API ElementBezierCurve2d  : virtual public Element2d
{
public:	
	/** 获取曲线的类型
	* @return 曲线的类型int
	*/
	virtual int getSplineType() = 0;

	/** 设置曲线的类型
	* @param [in] 曲线的类型type
	*/
	virtual void setSplineType(int type) = 0;
	
	/** 获取控制点阵
	* @param [in] 无
	* @return 控制点阵的指针
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;	
};
/** @}*/


/** @addtogroup Geometry2d Path2d-路径类，由连续的Segment2d组成，
*除了路径的第一个和最后一个组成的Segment2d之外，
*每一个Segment2d的起始点都是前一个Segment2d的终止点
*即路径对象中的Segment2d不能出现分离的情况。
*路径可以是任意数目的四种Segment2d类型的组合。
*  @{
*/
class CORE_API Path2d
{
public:	
	/**@name 指示当前元素几何信息已更改，需要重新计算 边界包围盒
	*  此函数调用后，如果调用getEnvelope，会先调用calculateEnvelope计算
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name 指示当前元素几何信息是否需要重新计算 边界包围盒
	*  @return 是否需要重新计算 边界包围盒
	*/
	virtual bool isDirtyEnvelope( ) = 0;
	
	 /**重新计算线段的显示点阵， 计算的同时，也计算边界矩形
	 * @param  [in] 
	 * @return 返回
	 */
	virtual void calculateDisplayPtList() = 0;

	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** 复制内存
	* @param 无
	* @return Element2d*对象
	*/
	virtual Path2dSharedPtr  clone() = 0;
	
	/** 将g所指向的几何对象复制到本对象
	* @param  [in] g Element2dSharedPtr, 指向待拷贝的对象的指针 
	* @return 是否拷贝成功
	*/	
	virtual bool copy( Path2dSharedPtr& g) = 0;

	/** 获长度
	* @param 无
	* @return 长度
	*/	
	virtual double getLength() = 0;
	
	/** 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
	* @param  [in] 无
	* @return 返回边界矩形的引用
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/** 设置几何对象的包围边界矩形，主要供派生类使用
	*   * @param  [in] e3d const Envelope3d & 边界矩形
	*   * @return 无
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;

	/** 获取曲线是否封闭
	* @return 是否封闭
	*/
	virtual bool isClose() = 0;

	/** 设置曲线是否封闭
	* @param [in] b
	* @return 
	*/
	virtual void setClose(bool b) = 0;
	
	/** 获取线段的起点
	* @param [out] fromPoint
	* @return 
	*/
	virtual void getFromPoint(Vertex2d& fromPoint) = 0;

	/** 获取线段的终点
	* @param [out] toPoint
	* @return 
	*/
	virtual void getToPoint(Vertex2d& toPoint) = 0;
		
	/** 获取用于显示的线段的点阵
	* @param [out] vtList：点阵
	* @return 获取是否成功
	*/
	virtual bool getDisplayPointCollection(VertexCollection2d& vtList) = 0;	

	/** 获取当前路径的线段列表
	* @param 无
	* @return SegmentCollection2d指针，当前路径的线段列表
	*/
	virtual ElementCollection2dSharedPtr& getSegmentCollection() = 0;

	/** 使路径有效（每个线段的起点为下个线段的终点）
	* @param 无 
	* @return 空
	*/
	virtual void simplify() = 0;
	
	/**   获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
	*     * @param  [in] 无 
	*     * @return 返回几何对象写入到Buffer中所占用的字节数
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**从Buffer中读取信息填充本几何对象 
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0 ;
	
	/**将本类的信息写入Buffer中  
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作  
	*  * @param  [in,out] f  std::ostream &，标准输出流对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** 从标准输入流中提取本类的信息   
	*   * @param  [in,out] f  std::istream &，标准输入流对象引用 
	*   * @return 成功返回true，否则返回false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/** 线段反向
	* @return
	*/
	virtual void reverseOrientation() = 0;
};
/** @}*/

/** @addtogroup Geometry2d PathCollection2d-路径集类，可以用来管理所有路径Path2d
*  @{
*/
class CORE_API PathCollection2d 
{
public:
	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的 边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void write(std::ostream & f)=0;
	/**从标准输入流中提取本类的信息 
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void read(std::istream & f)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** 获取对象写入到Buffer中所占用的字节数
	* @param 无
	* @return 获取到的buffer字节数
	*/
	virtual size_t sizeBuffer()=0 ;

	/** 添加元素到元素集中
	* 如果before, after都为空，则添加到元素集的末尾
	* @param [in] pt，要插入的元素
	* @param [in] before，在此索引之前插入
	* @param [in] after，在此索引之后插入
	* @return
	*/
	virtual void addElement(Path2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** 将目标元素集中的元素添加到当前元素集的末尾
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool addCollection(PathCollection2dSharedPtr&  newElements)=0;

	/** 将目标元素集中的元素插入到当前元素集中
	* @param [in] index，插入位置
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool insertCollection(long index,PathCollection2dSharedPtr&  newElements)=0;

	/** 获取指定索引的元素
	* @param [in] index: 要获取的元素的索引
	* @return: 获取到的元素的指针，获取失败则返回NULL
	*/
	virtual Path2dSharedPtr&  getElement(long index)=0;

	/** 获取元素的个数
	* @return: 元素的个数
	*/
	virtual long getCount()=0;

	/** 交换元素集中的两个元素
	* @param [in] index1,index2: 要交换的元素在数组中的索引
	* @return: 交换成功返回true，否则返回false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** 从元素集中移除指定索引的元素，并返回移除的元素
	* @param [in] index: 要移除的元素的索引
	* @return: 移除的元素的指针，失败则返回NULL
	*/
	virtual Path2dSharedPtr removeElement(long index)=0;

	/** 从元素集中删除指定索引的元素
	* @param [in] index: 要删除的元素的索引
	* @return: 删除失败则返回false
	*/
	virtual bool deleteElement(long index)=0;

	/** 清空当前元素集
	* @return
	*/
	virtual void clear()=0;

	/** @ 将目标数组 g 中的拷贝到 当前数组中 
	* @param  [in] g , ElementCollection2d *, 指向待拷贝的数组的指针 
	* @return 是否拷贝成功
	*/
	virtual bool copy(PathCollection2dSharedPtr& g) = 0;
};
/** @}*/

/** @addtogroup Geometry2d Ring2dType-环的类型
*  @{
*/
enum Ring2dType
{
	RINT_TYPE_2D_UNKNOW = 0,		//未知类型
	RINT_TYPE_2D_INTERIOR,			//内环
	RINT_TYPE_2D_EXTRRIOR			//外环
};
/** @}*/

/** @addtogroup Geometry2d Ring2d-环类，由连续的Segment2d组成，
*除了环的第一个和最后一个组成的Segment2d之外，
*每一个Segment2d的起始点都是前一个Segment2d的终止点
*即环对象中的Segment2d不能出现分离的情况。
*环可以是任意数目的四种Segment2d类型的组合。
*环中的Segment2d方向必须是一致的
*环必须是封闭的，即环的起始点必须与终止点一样
*环不能自相交
*  @{
*/
class CORE_API Ring2d 
{
public:

	/** 获取当前环的线段列表
	* @return SegmentCollection2d指针
	*/
	virtual ElementCollection2dSharedPtr& getSegmentCollection() = 0;

	/** 获取环的面积
	* @return double
	*/
	virtual double getArea() = 0;

	/** 获取环的几何重心，用于生成标注
	* @return Vertex2d
	*/
	virtual Vertex2d getCentrolid() = 0;

	/** 设定环封闭
	* @return 
	*/
	virtual void setClose() = 0;

	/** 使环有效
	* @return 
	*/
	virtual void simplify() = 0;

	/** 获取环的类型
	* @return 环的类型
	*/
	virtual int getRingType() = 0;

	/** 设置环的类型
	* @param [in] 环的类型ringType
	* @return 
	*/
	virtual void setRingType(Ring2dType ringType) = 0;

	
	/**@name 指示当前元素几何信息已更改，需要重新计算 边界包围盒
	*  此函数调用后，如果调用getEnvelope，会先调用calculateEnvelope计算
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name 指示当前元素几何信息是否需要重新计算 边界包围盒
	*  @return 是否需要重新计算 边界包围盒
	*/
	virtual bool isDirtyEnvelope( ) = 0;
	
	 /**重新计算线段的显示点阵， 计算的同时，也计算边界矩形
	 * @param  [in] 
	 * @return 返回
	 */
	virtual void calculateDisplayPtList() = 0;

	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** 复制内存
	* @param 无
	* @return Element2d*对象
	*/
	virtual Ring2dSharedPtr  clone() = 0;
	
	/** 将g所指向的几何对象复制到本对象
	* @param  [in] g Element2dSharedPtr, 指向待拷贝的对象的指针 
	* @return 是否拷贝成功
	*/	
	virtual bool copy( Ring2dSharedPtr& g) = 0;

	/** 获长度
	* @param 无
	* @return 长度
	*/	
	virtual double getLength() = 0;

	/** 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
	* @param  [in] 无
	* @return 返回边界矩形的引用
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/** 设置几何对象的包围边界矩形，主要供派生类使用
	*   * @param  [in] e3d const Envelope3d & 边界矩形
	*   * @return 无
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;
	
	/** 获取用于显示的线段的点阵
	* @param [out] vtList：点阵
	* @return 获取是否成功
	*/
	virtual bool getDisplayPointCollection(VertexCollection2d& vtList) = 0;	
	
	/**   获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
	*     * @param  [in] 无 
	*     * @return 返回几何对象写入到Buffer中所占用的字节数
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**从Buffer中读取信息填充本几何对象 
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0 ;
	
	/**将本类的信息写入Buffer中  
	*  * @param  [in,out] buf Buffer & ，缓冲区对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作  
	*  * @param  [in,out] f  std::ostream &，标准输出流对象引用 
	*  * @return 成功返回true，否则返回false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** 从标准输入流中提取本类的信息   
	*   * @param  [in,out] f  std::istream &，标准输入流对象引用 
	*   * @return 成功返回true，否则返回false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/** 线段反向
	* @return
	*/
	virtual void reverseOrientation() = 0;
};
/** @}*/

/** @addtogroup Geometry2d RingCollection2d-环集类，可以用来管理所有环Ring2d
*  @{
*/
class CORE_API RingCollection2d 
{
public:
	/**@name 计算当前几何图元的边界包围盒
	*  @param  [out] outEnvp:返回的 边界包围盒
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void write(std::ostream & f)=0;
	/**从标准输入流中提取本类的信息 
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void read(std::istream & f)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** 获取对象写入到Buffer中所占用的字节数
	* @param 无
	* @return 获取到的buffer字节数
	*/
	virtual size_t sizeBuffer()=0 ;

	/** 添加元素到元素集中
	* 如果before, after都为空，则添加到元素集的末尾
	* @param [in] pt，要插入的元素
	* @param [in] before，在此索引之前插入
	* @param [in] after，在此索引之后插入
	* @return
	*/
	virtual void addElement(Ring2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** 将目标元素集中的元素添加到当前元素集的末尾
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool addCollection(RingCollection2dSharedPtr&  newElements)=0;

	/** 将目标元素集中的元素插入到当前元素集中
	* @param [in] index，插入位置
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool insertCollection(long index,RingCollection2dSharedPtr&  newElements)=0;

	/** 获取指定索引的元素
	* @param [in] index: 要获取的元素的索引
	* @return: 获取到的元素的指针，获取失败则返回NULL
	*/
	virtual Ring2dSharedPtr&  getElement(long index)=0;

	/** 获取元素的个数
	* @return: 元素的个数
	*/
	virtual long getCount()=0;

	/** 交换元素集中的两个元素
	* @param [in] index1,index2: 要交换的元素在数组中的索引
	* @return: 交换成功返回true，否则返回false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** 从元素集中移除指定索引的元素，并返回移除的元素
	* @param [in] index: 要移除的元素的索引
	* @return: 移除的元素的指针，失败则返回NULL
	*/
	virtual Ring2dSharedPtr removeElement(long index)=0;

	/** 从元素集中删除指定索引的元素
	* @param [in] index: 要删除的元素的索引
	* @return: 删除失败则返回false
	*/
	virtual bool deleteElement(long index)=0;

	/** 清空当前元素集
	* @return
	*/
	virtual void clear()=0;

	/** @ 将目标数组 g 中的拷贝到 当前数组中 
	* @param  [in] g , ElementCollection2d *, 指向待拷贝的数组的指针 
	* @return 是否拷贝成功
	*/
	virtual bool copy(RingCollection2dSharedPtr& g) = 0;
};
/** @}*/



/** @addtogroup Geometry2d Geometry2d-二维几何类
*  @{
*/
//##ModelId=4D0B58C6005F
class  CORE_API Geometry2d : public Geometry
{
public:
	/**@name 计算当前几何图元的边界包围盒
	*  @param  [in] pCoordSys:坐标系统指针 
	*  @param  [out] outEnvp: 边界包围盒 
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Coordsys2dSharedPtr& pCoorSys, Envelope3d& outEnvp )=0;

	/**@name 计算当前几何图元的边界包围盒
	*        此函数计算出来的点、文本、标注对象的包围盒，并不准确，需要传入Coordsys2d能算出精确的包围盒
	*  @param  [out] outEnvp: 边界包围盒 
	*  @return 计算是否成功 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp )=0;
	
	/** 获取几何对象的颜色
	* @return 几何对象的颜色Color4f
	*/
	virtual Color4f getPenColor() = 0;

	/** 获取几何对象的颜色
	* @param [out] 几何对象的颜色cr
	* @return 
	*/
	virtual void getPenColor(Color4f &cr) = 0;

	/** 设置几何对象的颜色
	* @param [in] 几何对象的颜色cr
	* @return 
	*/
	virtual void setPenColor(Color4f cr) = 0;
	
	/** 设置几何对象的颜色
	* @param [in] 几何对象的颜色r,g,b,a；注意： r,g,b的取值范围在0 - 1
	* @return 
	*/
	virtual void setPenColor(float r, float g, float b, float a)=0;
	
	/** 获取几何对象的填充颜色，仅对填充区对象有效
	* @return 填充颜色Color4f
	*/
	virtual Color4f getBrushColor() = 0;
	
	/** 获取几何对象的填充颜色，仅对填充区对象有效
	* @param [out] 填充颜色cr
	* @return 
	*/
	virtual void getBrushColor(Color4f &cr) = 0;
	
	/** 设置几何对象的填充颜色，仅对填充区对象有效
	* @param [in] 填充颜色cr
	* @return 
	*/
	virtual void setBrushColor(Color4f cr) = 0;
	
	/** 设置几何对象的填充颜色，仅对填充区对象有效
	* @param [in] 填充颜色r,g,b,a
	* @return 
	*/
	virtual void setBrushColor(float r, float g, float b, float a)=0;

	/** 设置状态值：当前对象是否为某个组的成员
	* @param [in] bool b
	* @return 
	*/
	virtual void setGeometryInGroup(bool b) = 0;

	/** 当前几何对象是否为某个组的成员
	* @return 是否为某个组的成员
	*/	
	virtual bool isGeometryInGroup() = 0;

	/** 状态表示操作, 判断图元是否是处于删除状态 
	*/	
	virtual bool isDeleted() = 0;

	/** 返回图元的隐藏状态 
	*/	
	virtual bool isVisible() = 0;

	/** 返回图元的选中状态 
	*/	
	virtual bool isSelected() = 0;

	/** 设置图元的删除状态 
	* @param [in] bool 是否删除
	*/	
	virtual void setDeleteFlag(bool bDelete) = 0;

	/** 设置图元的隐藏状态 
	* @param [in] bool 是否可见
	*/	
	virtual void setVisible(bool bHide) = 0;

	/** 设置图元的选中状态 
	* @param [in] bool 是否选中
	*/	
	virtual void setSelectFlag(bool bSel) = 0;

public:	
	/**当前对象，围绕指定原点 org 进行旋转，角度单位：度 
	* @param [in] 原点 org, 旋转角度dAngleDegree
	*/
	virtual void rotate(Vertex2d &org, double dAngleDegree) = 0;

	/** 当前对象，按指定点 org 为原点进行比例变换 
	* @param [in] scaleX,scaleY为x,y轴的变换因子, org 为原点坐标
	*/
	virtual void transformByScale(Vertex2d &org,double& scaleX, double& scaleY) = 0;

	/**平移 
	* @param [in] 位移：offX, offY
	*/
	virtual void translate(double offX, double offY) = 0;

	/**根据镜像类型，进行镜像变换 
	* @param [in] mirrorType镜像类型
	*/
	virtual void mirrorImage(int mirrorType) = 0;

	/**围绕pt1，pt2组成的线段，进行镜像变换 
	* @param [in] 组成线段的点pt1,pt2
	*/
	virtual void mirrorImage(Vertex2d& pt1,Vertex2d& pt2) = 0;

	//////////////////////////////////////////////////////////////////////////	
	/** 设置几何对象的填充颜色，仅对填充区对象有效
	* @param [in] 填充颜色r,g,b,a
	* @return 
	*/
	//virtual void setBrushColor(int r, int g, int b, int a)=0;

	/** 设置几何对象的颜色
	* @param [in] 几何对象的颜色r255,g255,b255,a255；注意： r,g,b的取值范围在0 - 255
	* @return 
	*/
	//virtual void setPenColor(int r255, int g255, int b255, int a)=0;
	/** 获取几何对象的ID
	* @return 几何对象的ID
	*/
	//virtual FSID getID() = 0;

	/** 设置几何对象的ID
	* @param [in] 几何对象的ID
	* @return 
	*/
	//virtual void setID(FSID id) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeometryCollection2d-几何对象集类
*  @{
*/
class CORE_API GeometryCollection2d 
{
public:
	/**将本类的信息写入输出流中，便于文件读写或缓冲区写操作
	* @param  [in,out] f  std::ostream &，标准输出流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void write(std::ostream & f) = 0;

	/**从标准输入流中提取本类的信息 
	* @param  [in,out] f  std::istream &，标准输入流对象引用
	* @return 成功返回true，否则返回false
	*/
	virtual void read(std::istream & f) = 0;

	/** 添加元素到元素集中
	* 如果before, after都为空，则添加到元素集的末尾
	* @param [in] pt，要插入的元素
	* @param [in] before，在此索引之前插入
	* @param [in] after，在此索引之后插入
	* @return
	*/
	virtual void addGeometry(GeometrySharedPtr& obj,int* before=NULL, int* after=NULL) = 0;

	/** 将目标元素集中的元素添加到当前元素集的末尾
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool addCollection(GeometryCollection2dSharedPtr& newElements) = 0;

	/** 将目标元素集中的元素插入到当前元素集中
	* @param [in] index，插入位置
	* @param [in] newElements，目标元素集
	* @return 添加是否成功
	*/
	virtual bool insertCollection(long index,GeometryCollection2dSharedPtr& newElements) = 0;

	/** 获取指定索引的元素
	* @param [in] index: 要获取的元素的索引
	* @return: 获取到的元素的指针，获取失败则返回NULL
	*/
	virtual GeometrySharedPtr& getGeometry(long index) = 0;

	/** 获取元素的个数
	* @return: 元素的个数
	*/
	virtual long getCount() = 0;

	/** 从元素集中移除指定索引的元素，并返回移除的元素
	* @param [in] index: 要移除的元素的索引
	* @return: 移除的元素的指针，失败则返回NULL
	*/
	virtual GeometrySharedPtr removeGeometry(long index) = 0;

	/** 从元素集中删除指定索引的元素
	* @param [in] index: 要删除的元素的索引
	* @return: 删除失败则返回false
	*/
	virtual bool deleteGeometry(long index) = 0;

	/** 清空当前元素集
	* @return
	*/
	virtual void clear() = 0;
	
	/** 将目标数组 g 中的拷贝到 当前数组中
	*   @param  [in] g const GeometryCollection2d *, 指向待拷贝的数组的指针
	*   @return 是否拷贝成功
	*/
	virtual bool copy(const GeometryCollection2dSharedPtr& g) = 0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*从Buffer中读取信息填充本几何对象
	* @param  [in,out] buf Buffer & ，缓冲区对象引用 
	* @return 成功返回true，否则返回false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** 获取对象写入到Buffer中所占用的字节数
	* @param 无
	* @return 获取到的buffer字节数
	*/
	virtual size_t sizeBuffer()=0 ;

	/** 交换元素集中的两个元素
	* @param [in] index1,index2: 要交换的元素在数组中的索引
	* @return: 交换成功返回true，否则返回false
	*/
	virtual bool swap(long index1,long index2)=0;
	
	/** 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
    * @param  [in] 无
    * @return 返回边界矩形的引用
	*/
	virtual void getEnvelope(Envelope3d & envp)=0;
	
	/**当前对象，围绕指定原点 org 进行旋转，角度单位：度
	* @param [in] 原点 org,旋转角度，dAngleDegree
	*/
	virtual void rotate(Vertex2d &org, double dAngleDegree) = 0;

	/** 当前对象，按指定点 org 为原点进行比例变换 
	* @param [in] scaleX,scaleY为x,y轴的变换因子,原点 org
	*/
	virtual void transformByScale(Vertex2d &org,double& scaleX, double& scaleY) = 0;

	/**平移 
	* @param [in] 位移： offX, offY
	*/
	virtual void translate(double offX, double offY) = 0;

	/**根据镜像类型，进行镜像变换 
	* @param [in] 镜像类型mirrorType
	*/
	virtual void mirrorImage(int mirrorType) = 0;

	/**围绕pt1，pt2组成的线段，进行镜像变换 
	* @param [in] pt1，pt2组成的线段
	*/
	virtual void mirrorImage(Vertex2d& pt1,Vertex2d& pt2) = 0;
};
/** @}*/


/** @addtogroup Geometry2d Pen2dType
*  @{
*/
enum Pen2dType
{
	PEN_TYPE_2D_UNKNOW = 0,
	PEN_TYPE_2D_SYSTEM,			//系统自带画笔（五种基本类型：点线，点划线，划线，双点划线，实线）
	PEN_TYPE_2D_USERDEFINE		//用户自定义画笔（自定义线型，即复合线型）
};
/** @}*/


/** @addtogroup Geometry2d Pen2d-画笔类
*  @{
*/

class CORE_API Pen2d
{
public:
	/**将g所指向的对象复制到本对象 
	*  * @param  [in] g Pen2dSharedPtr, 指向待拷贝对象的指针 
	*  * @return 是否拷贝成功 
	*/
	virtual bool copy( Pen2dSharedPtr& g) = 0;

	/** 获取画笔类型
	* @return画笔类型Pen2dType
	*/
	virtual Pen2dType getType() = 0;

	/** 设置画笔类型
	* @param [in] 画笔类型tp
	* @return
	*/
	virtual void setType(Pen2dType tp) = 0;

	/** 获取自定义画笔的名称，如果为用户自定义画笔时有效
	* @return画笔的名称
	*/
	virtual string getUserDefineTypeName() = 0;

	/** 设置自定义画笔的名称，如果为用户自定义画笔时有效
	* @param [in] 画笔的名称name
	* @return
	*/
	virtual void   setUserDefineTypeName(string name) = 0;

	/** 获取自定义画笔的类型，如果为系统画笔时有效
	* @return 系统画笔的类型
	*/
	virtual int  getSystemTypeName() = 0;

	/** 设置自定义画笔的类型，如果为系统画笔时有效
	* @param [in] 系统画笔的类型type
	* @return
	*/
	virtual void setSystemTypeName(int type) = 0;

	/** 获取画笔的宽度，如果为系统画笔时有效
	* @return 画笔的宽度
	*/
	virtual double  getWidth() = 0;

	/** 设置画笔的宽度，如果为系统画笔时有效
	* @param [in] 画笔的宽度d
	* @return
	*/
	virtual void    setWidth(double d) = 0;

	/** 获取画笔的显示比例，如果为用户自定义画笔时有效
	* @return 画笔的显示比例
	*/
	virtual double  getScale() = 0;

	/** 设置画笔的显示比例，如果为用户自定义画笔时有效
	* @param [in] 画笔的显示比例 d
	* @return
	*/
	virtual void    setScale(double d) = 0;

		
	/** 获取画笔对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建画笔对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
    virtual size_t sizeBuffer()=0;

	/** 从Buffer中读取信息填充本画笔对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf)=0;

	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf)=0;
};
/** @}*/


/** @addtogroup Geometry2d GeomPolygon2d-填充区几何对象类
*  @{
*/

class CORE_API GeomPolygon2d : virtual public Geometry2d
{
public:
	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** 获取组成填充区的环的集合
	* @return 环的集合 RingCollection2dSharedPtr
	*/
	virtual RingCollection2dSharedPtr& getRingCollection() = 0;

	/** 获取画笔
	* @return 画笔 Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** 获取是否填充颜色
	* @return 是否填充颜色
	*/
	virtual bool   isFilledColor() = 0;

	/** 设置是否填充颜色
	* @param [in] 是否填充颜色 bFill
	* @return
	*/
	virtual void   setFilledColorStatus(bool bFill) = 0;

	/** 获取是否填充符号
	* @return 是否填充符号bool
	*/
	virtual bool   isFilledPattern() = 0;

	/** 设置是否填充符号
	* @param [in] 是否填充符号bFill
	* @return
	*/
	virtual void   setFilledPatternStatus(bool bFill) = 0;

	/** 获取面积
	* @return double
	*/	
	virtual double getArea() = 0;

	/** 获取周长
	* @return double
	*/	
	virtual double getPerimeter() = 0;

	/** 获取子图填充比例
	* @return 子图填充比例
	*/
	virtual double getSymbolScale() = 0;

	/** 设置子图填充比例
	* @param [in] 子图填充比例 d
	* @return 
	*/
	virtual void setSymbolScale(double d) = 0;

	/** 获取子图填充角度(角度)
	* @return 子图填充角度(角度)
	*/
	virtual double getSymbolAngleDeg() = 0;

	/** 设置子图填充角度(角度)
	* @param [in] 子图填充角度(角度) d
	* @return 
	*/
	virtual void setSymbolAngleDeg(double d) = 0;

	/** 获取花纹名称
	* @return 花纹名称string
	*/
	virtual string getSymbolName() = 0;

	/** 设置花纹名称
	* @param [in] 花纹名称 name
	* @return 
	*/
	virtual void setSymbolName(string name) = 0;

	/** 设置线条的是否显示点状态 
	* @param [in] 是否显示点状态 b
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** 获取线条的是否显示点状态 
	* @return 是否显示点状态bool
	*/	
	virtual bool isShowPointStatus() = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomPolyline2d-线几何对象类
*  @{
*/
class CORE_API GeomPolyline2d : virtual public Geometry2d
{
public:
	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** 获取组成线条的路径的集合
	* @return 路径的集合PathCollection2dSharedPtr
	*/
	virtual PathCollection2dSharedPtr& getPathCollection() = 0;

	/** 获取画笔
	* @return 画笔 Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** 获取长度
	* @return 长度double
	*/	
	virtual double getLength() = 0;

	/** 设置线条的是否显示点状态 
	* @param [in] 是否显示点 bool
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** 获取线条的是否显示点状态 
	* @return bool是否显示点
	*/	
	virtual bool isShowPointStatus() = 0;

	/** 设置是否显示线条方向 
	* @param [in] bIsDispDirection是否显示线条方向
	* @return 
	*/	
	virtual void setDispDirection(bool bIsDispDirection) = 0;

	/** 判断是否显示线条方向 
	* @return bool是否显示线条方向
	*/	
	virtual bool isDispDirection() = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomLine2d-线几何对象类
*  @{
*/
class CORE_API GeomLine2d : virtual public Geometry2d
{
public:
	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** 根据点阵ptList，把当前的区对象重新构造，具有一个封闭的环
	* @return ptList 点阵
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** 获取组成线条的点阵的集合
	* @return 点阵的集合VertexCollection2dSharedPtr
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;

	/** 获取画笔
	* @return 画笔 Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** 获取长度
	* @return 长度double
	*/	
	virtual double getLength() = 0;

	/** 设置线条的是否显示点状态 
	* @param [in] 是否显示点 bool
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** 获取线条的是否显示点状态 
	* @return bool是否显示点
	*/	
	virtual bool isShowPointStatus() = 0;

	/** 设置是否显示线条方向 
	* @param [in] bIsDispDirection是否显示线条方向
	* @return 
	*/	
	virtual void setDispDirection(bool bIsDispDirection) = 0;

	/** 判断是否显示线条方向 
	* @return bool是否显示线条方向
	*/	
	virtual bool isDispDirection() = 0;

	/** 判断线条是否封闭
	* @return bool 线条是否封闭
	*/	
	virtual bool isClosed() = 0;

	/** 设置线条是否封闭
	* @param [in] b 线条是否封闭
	* @return 
	*/	
	virtual void setClosed(bool b) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeomPoint2d-抽象点类（以圆、正方形、三角形显示的点，有大小）
*  @{
*/

class CORE_API GeomPoint2d : virtual public Geometry2d
{
public:
	/** 点类型的获取（圆，正方形，三角形）
	* @return 点类型（圆，正方形，三角形）
	*/	
	virtual int getPointType() = 0;

	/** 点类型的设置（圆，正方形，三角形）
	* @param [in] nType: 点类型（圆，正方形，三角形）
	* @return 
	*/	
	virtual void setPointType(int nType) = 0;

	/** 符号位置的获取
	* @return 符号位置Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** 符号位置的获取
	* @param [out] 符号位置Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** 符号位置的获取
	* @param [out] 符号位置x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** 符号位置的设置
	* @param [in] 符号位置Vertex2d
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** 符号位置的设置
	* @param [in] 符号位置x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** 点的大小（毫米单位）的获取
	* @return 点的大小（毫米单位）
	*/
	virtual double getSize() = 0;

	/** 点的大小（毫米单位）的设置
	* @param [in] 点的大小（毫米单位）
	* @return 
	*/
	virtual void setSize(double s) = 0;

};
/** @}*/

/** @addtogroup Geometry2d GeomPointSymbol2d-点符号类
*  @{
*/

class CORE_API GeomPointSymbol2d : virtual public Geometry2d
{
public:
	/** 符号名称的获取
	* @return 符号名称string
	*/	
	virtual std::string getSymbolName() = 0;

	/** 符号名称的设置
	* @param [in] 符号名称string
	* @return 
	*/	
	virtual void setSymbolName(std::string strName) = 0;

	/** 符号位置的获取
	* @return 符号位置Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** 符号位置的获取
	* @param [in] 符号位置Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** 符号位置的获取
	* @param [in] 符号位置 x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** 符号位置的设置
	* @param [in] 符号位置 pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** 符号位置的设置
	* @param [in] 符号位置 x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** 符号显示比例的获取
	* @return 符号显示比例
	*/
	virtual double getScale() = 0;

	/** 符号显示比例的设置
	* @param [in] 符号显示比例 s
	* @return 
	*/
	virtual void setScale(double s) = 0;

	/** 符号旋转角度（角度单位）的获取
	* @return 符号旋转角度（角度单位）
	*/
	virtual double getRotateAngleDeg() = 0;

	/** 符号旋转角度（角度单位）的设置
	* @param [in] 符号旋转角度（角度单位）
	* @return 
	*/
	virtual void setRotateAngleDeg(double ang) = 0;
};
/** @}*/



/** @addtogroup Geometry2d GeomMultipoint2d-多点类
*  @{
*/

class CORE_API GeomMultipoint2d : virtual public Geometry2d
{
public:
	/** 获取多点对象的点集
	* @return 获取到的点集, VertexCollection2dSharedPtr
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;

	/** 多点对象的符号名称的获取
	* @return 多点对象的符号名称
	*/	
	virtual std::string getSymbolName() = 0;

	/** 多点对象的符号名称的设置
	* @param [in] 多点对象的符号名称
	* @return 
	*/	
	virtual void setSymbolName(std::string strName) = 0;

	/** 符号显示比例的获取
	* @return 符号显示比例
	*/
	virtual double getScale() = 0;

	/** 符号显示比例的设置
	* @param [in] 符号显示比例
	* @return 
	*/
	virtual void setScale(double s) = 0;

	/** 符号旋转角度（角度单位）的获取
	* @return 符号旋转角度（角度单位）
	*/
	virtual double getRotateAngleDeg() = 0;

	/** 符号旋转角度（角度单位）的设置
	* @param [in] 符号旋转角度（角度单位）
	* @return 
	*/
	virtual void setRotateAngleDeg(double ang) = 0;
};
/** @}*/


/** @addtogroup Geometry2d Label2dStyle-标注风格，即插入点相对于标注的位置
*  @{
*/
enum Label2dStyle
{
	BottomLeft,			//下左
	BottomMiddle,		//下中
	BottomRight,		//下右
	IntervalLeft,		//中左
	IntervalMiddle,		//中中
	IntervalRight,		//中右
	TopLeft,			//上左
	TopMiddle,			//上中
	TopRight			//上右
};
/** @}*/

/** @addtogroup Geometry2d GeomLabel2d-文本标注类
*  @{
*/

class CORE_API GeomLabel2d : virtual public Geometry2d
{
public:
	/** 获取文本
	* @return 文本
	*/
	virtual std::string getText() = 0;

	/** 设置文本
	* @param [in] 文本
	* @return 
	*/
	virtual void setText(std::string str) = 0;

	/** 获取文本的大小
	* @return 文本的大小
	*/
	virtual double getSize() = 0;

	/** 设置文本的大小
	* @param [in] 文本的大小
	* @return 
	*/
	virtual void setSize(double size) = 0;

	/** 获取字符间距
	* @return 字符间距
	*/
	virtual double getOffWide() = 0;

	/** 设置字符间距
	* @param [in] 字符间距 d
	* @return 
	*/
	virtual void setOffWide(double d) = 0;

	/** 获取字符行间距
	* @return 字符行间距
	*/
	virtual double getRowOffWide() = 0;

	/** 设置字符行间距
	* @param [in] 字符行间距 d
	* @return 
	*/
	virtual void setRowOffWide(double d) = 0;

	/** 获取字符旋转角度（单个字符，角度单位）
	* @return 字符旋转角度（单个字符，角度单位）
	*/
	virtual double getFontRotateAngleDeg() = 0;

	/** 设置字符旋转角度（单个字符，角度单位）
	* @param [in] 设置字符旋转角度（单个字符，角度单位）
	* @return 
	*/
	virtual void setFontRotateAngleDeg(double d) = 0;

	/** 获取整个标注的旋转角度（整体，角度单位）
	* @return 标注的旋转角度（整体，角度单位）
	*/
	virtual double getLabelRotateAngleDeg() = 0;

	/** 设置整个标注的旋转角度（整体，角度单位）
	* @param [in] 标注的旋转角度（整体，角度单位）
	* @return 
	*/
	virtual void setLabelRotateAngleDeg(double d) = 0;

	/** 获取文本的字体
	* @return 文本的字体Font2d
	*/
	virtual Font2d getFont() = 0;

	/** 设置文本的字体
	* @param [in] 文本的字体Font2d
	* @return 
	*/
	virtual void setFont(Font2d & font) = 0;

	/** 获取标注的风格
	* @return 标注的风格Label2dStyle
	*/
	virtual Label2dStyle getStyle() = 0;

	/** 设置标注的风格
	* @param [in] 标注的风格Label2dStyle
	* @return 
	*/
	virtual void setStyle(Label2dStyle sty) = 0;
	
	/** 符号位置的获取
	* @return 符号位置Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;
	
	/** 符号位置的获取
	* @param [out] 符号位置Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;
	
	/** 符号位置的获取
	* @param [out] 符号位置 x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;
	
	/** 符号位置的设置
	* @param [in] 符号位置pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;
	
	/** 符号位置的设置
	* @param [in] 符号位置 x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeomAnnotation2d-注记几何对象类，抽象坐标点+注记
*  @{
*/

class CORE_API GeomAnnotation2d : virtual public Geometry2d
{
public:
	/** 注记文本的获取
	* @return 注记文本
	*/	
	virtual std::string getAnnotation() = 0;

	/** 注记文本的设置
	* @param [in] 注记文本strName
	* @return 
	*/	
	virtual void setAnnotation(std::string strName) = 0;

	/** 注记位置的获取
	* @return 注记位置Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** 注记位置的获取
	* @param [out] 注记位置pt
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** 注记位置的获取
	* @param [out] 注记位置 x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** 注记位置的设置
	* @param [in] 注记位置pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** 注记位置的设置
	* @param [in] 注记位置x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** 注记文本大小的获取（毫米单位）
	* @return 注记文本大小（毫米单位）
	*/
	virtual double getSize() = 0;

	/** 注记文本大小的设置（毫米单位）
	* @param [in] 注记文本大小（毫米单位）
	* @return 
	*/
	virtual void setSize(double s) = 0;

	/** 获取文本的字体
	* @return 文本的字体
	*/
	virtual Font2d getFont() = 0;

	/** 设置文本的字体
	* @param [in] 文本的字体 font
	* @return 
	*/
	virtual void setFont(Font2d & font) = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomGeometryBag2d-几何对象包
*  @{
*/

class CORE_API GeomGeometryBag2d : virtual public Geometry2d
{
public:
	/** 获取几何对象列表
	* @return GeometryCollection2dSharedPtr
	*/
	virtual GeometryCollection2dSharedPtr& getGeometryCollection() = 0;
};
/** @}*/


/** @addtogroup Geometry2d Image2dType-图像类型
*  @{
*/
enum Image2dType
{
	IMAGE_TYPE_2D_UNKNOW = 0,
	IMAGE_TYPE_2D_JPG,	
	IMAGE_TYPE_2D_BMP,	
	IMAGE_TYPE_2D_TIFF,	
	IMAGE_TYPE_2D_PNG,	
	IMAGE_TYPE_2D_GIF
};
/** @}*/


/** @addtogroup Geometry2d GeomImage2d-图像
*  @{
*/

class CORE_API GeomImage2d : virtual public Geometry2d
{
public:	
	/** 图像类型的获取
	* @return 图像类型
	*/	
	virtual int getImageType() = 0;
	
	/** 图像类型的设置
	* @param [in] 图像类型
	* @return 
	*/	
	virtual void setImageType(int imgType) = 0;
	
	/** 图像名称的获取
	* @return 图像名称
	*/	
	virtual std::string getName() = 0;
	
	/** 图像名称的设置
	* @param [in] 图像名称
	* @return 
	*/	
	virtual void setName(std::string strName) = 0;

	/** 图像位置的获取
	* @return 图像位置Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** 图像位置的获取
	* @param [out] 图像位置pt
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** 图像位置的获取
	* @param [out] 图像位置 x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** 图像位置的设置
	* @param [in] 图像位置pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** 图像位置的设置
	* @param [in] 图像位置x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** 横轴显示比例的获取
	* @return 横轴显示比例
	*/
	virtual double getXScale() = 0;

	/** 纵轴显示比例的获取
	* @return 纵轴显示比例
	*/
	virtual double getYScale() = 0;

	/** 横轴显示比例的设置
	* @param [in] s 横轴显示比例
	* @return 
	*/
	virtual void setXScale(double s) = 0;

	/** 纵轴显示比例的设置
	* @param [in] s 纵轴显示比例
	* @return 
	*/
	virtual void setYScale(double s) = 0;
};
/** @}*/


// @addtogroup Geometry2d GeometrySelectManager2d 二维几何对象选择管理器类接口
//     *  @{
//    
class CORE_API GeometrySelectManager2d
{
public:
	/**设定选择管理类的坐标系统 
	* @param [in] pCoorSys: 坐标系统 
	* @return 
	*/
	virtual void setCoordsys(Coordsys2dSharedPtr& pCoorSys) = 0;

	/**判断指定几何对象pObj 是否在指定的边界矩形范围内 
	* @param [in] point_left_bottom_in_WP:世界坐标下，边界矩形的左下角点
	* @param [in] point_right_top_in_WP:  世界坐标下，边界矩形的右上角点
	* @return 是否处于指定范围内
	*/
	virtual bool boundIntersect(GeometrySharedPtr& pObj,
		const Vertex2d &  point_left_bottom_in_WP,
		const Vertex2d &  point_right_top_in_WP) = 0;

	/**判断指定几何对象pObj 是否在指定的边界矩形范围内 
	* @param [in] minx, maxx, miny, maxy： 世界坐标下，选择框范围
	* @return 是否处于指定范围内
	*/
	virtual bool boundIntersect(GeometrySharedPtr& pObj,
		double minx, double maxx, double miny, double maxy) = 0;
};
/** @}*/

/** @addtogroup Geometry2d Symbol 符号类
*  @{
*/

class CORE_API Symbol2d
{
public:
	/**获取符号ID
	* @return 符号ID
	*/
	virtual FSID		getID()=0;

	/**设置符号ID
	* @param [in] 符号ID
	* @return 
	*/
	virtual void		setID(FSID id)=0;

	/**获取符号名称 
	* @return 符号名称
	*/
	virtual std::string	getName()=0;

	/**设置符号名称 
	* @param [in] 符号名称 
	* @return 
	*/
	virtual void		setName(std::string sz)=0;

	/**获取标准编号
	* @return 标准编号
	*/
	virtual std::string	getStdNum()=0;

	/**设置标准编号
	* @param [in] 标准编号sz
	* @return 
	*/
	virtual void		setStdNum(std::string sz)=0;

	/**获取符号描述 
	* @return 符号描述
	*/
	virtual std::string	getDescription()=0;

	/**设置符号描述 
	* @param [in] 符号描述 
	* @return 
	*/
	virtual void		setDescription(std::string sz)=0;

	/**获取是否为游离的符号
	* @return 是否为游离的符号
	*/
	virtual bool		isFreedom()=0;

	/**设置 是否为游离的符号
	* @param [in] bfreedom是否为游离的符号
	* @return 
	*/
	virtual void		setFreedom(bool bfreedom)=0;

	/**获取宽度 毫米
	* @return 宽度 毫米
	*/
	virtual double		getWidth()=0;

	/**设置宽度 毫米
	* @param [in] 宽度 毫米
	* @return 
	*/
	virtual void		setWidth(double dw)=0;

	/**获取高度 毫米
	* @return 高度 毫米
	*/
	virtual double		getHeight()=0;

	/**获取、设置高度 毫米
	* @param [in] 高度 毫米
	* @return 
	*/
	virtual void		setHeight(double dw)=0;

	/**获取背景颜色 
	* @return 背景颜色 
	*/
	virtual Color4f		getBackgroundColor()=0;

	/**设置背景颜色 
	* @param [in] cr背景颜色
	* @return 
	*/
	virtual void		setBackgroundColor(Color4f& cr)=0;

	/**获取是否填充背景色 
	* @return 是否填充背景色
	*/
	virtual bool		isFilledBackground()=0;

	/**设置是否填充背景色 
	* @param [in] 是否填充背景色
	* @return 
	*/
	virtual void		setFilledBackground(bool bFilled)=0;
	
	/** 获取符号插入点坐标,仅对点符号有效
	* @return 符号插入点坐标 Vertex2d
	*/
	virtual Vertex2d getInserPosition()=0;

	/** 获取符号插入点,仅对点符号有效
	* @param [out] 符号插入点坐标
	* @return
	*/
	virtual void getInserPosition(double& x,double& y)=0;
	
	/** 设置符号插入点坐标,仅对点符号有效
	* @param [in] 符号插入点坐标vpos
	* @return
	*/
	virtual void setInserPosition(Vertex2d& vpos)=0;

	/** 设置符号插入点坐标,仅对点符号有效
	* @param [in] 符号插入点坐标 x,y
	* @return
	*/
	virtual void setInserPosition(double x,double y)=0;

	/** 获取组成符号的元素集合
	* @return 组成符号的元素集合GeometryCollection2dSharedPtr
	*/
	virtual GeometryCollection2dSharedPtr& getGeometryCollection()=0;
	
	/** 获取几何对象的边界矩形，为三维边界，如果是二维对象，则Z的范围为0
     * @param  [out] 边界矩形Envelope3d
     * @return 
    */
    virtual void getEnvelope(Envelope3d & envp)=0; 

public:	
	/** 克隆本身
	 * @param  [in]  
	 * @return 是否拷贝成功
	 */
	virtual Symbol2dSharedPtr clone()=0;

	/** 获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
    virtual size_t sizeBuffer()=0;
	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf)=0;
	/** 将本类的信息写入Buffer中 
     * @param  [in,out] buf Buffer & ，缓冲区对象引用
     * @return 成功返回true，否则返回false
    */
    virtual bool writeBuffer(Buffer& buf)=0;
	/* 将本类的信息写入输出流中，便于文件读写或缓冲区写操作 
     * @param  [in,out] f  std::ostream &，标准输出流对象引用
     * @return 成功返回true，否则返回false
    */
    virtual void write(std::ostream & f)=0;
	/** 从标准输入流中提取本类的信息  
     * @param  [in,out] f  std::istream &，标准输入流对象引用
     * @return 成功返回true，否则返回false
    */
	virtual void read(std::istream & f)=0;

public:
	/**统改所有几何对象的画笔颜色 
	* @param [in] 几何对象的画笔颜色penColor
	* @return 
	*/
	virtual void batchChangeGeometrysPenColor(Color4f &penColor)=0;

	/**统改所有几何对象的填充颜色 
	* @param [in] 几何对象的填充颜色fillColor
	* @return 
	*/
	virtual void batchChangeGeometrysBrushColor(Color4f &fillColor)=0;

	/**修改符号的大小
	* @param [in] 宽度,高度,单位:毫米, bIsKeepHWScale:是否保持纵横比例
	* @return 
	*/
	virtual void changeSize(double dWidth ,double dHeight,bool bIsKeepHWScale)=0;

	/**将符号充满到指定的宽度,高度,单位:毫米
	* @param [in] dWidth宽度,dHeight高度,单位:毫米, bIsKeepHWScale:是否保持纵横比例 
	* @return 
	*/
	virtual void expandToSize(double dWidth ,double dHeight,bool bIsKeepHWScale)=0;

};
/** @}*/

/** @addtogroup Geometry2d SymbolLibrary2d 符号组类
*  @{
*/

class CORE_API SymbolLibrary2d
{
public:
	/**获取符号组文件名 
	* @return 符号组文件名
	*/
	virtual std::string	getFilePath() = 0;

	/**获取 符号组名称 
	* @return 符号组名称
	*/
	virtual std::string	getName() = 0;

	/**设置 符号组名称 
	* @param [in] sz 符号组名称
	* @return 
	*/
	virtual void		setName(std::string sz) = 0;

	/**获取 符号组说明 
	* @return 符号组说明
	*/
	virtual std::string	getDescription() = 0;

	/**设置 符号组说明 
	* @param [in] sz 符号组说明
	* @return 
	*/
	virtual void		setDescription(std::string sz) = 0;

	/**获取 符号组标准编号 
	* @return 符号组标准编号
	*/
	virtual std::string	getStdNum() = 0;

	/**设置 符号组标准编号 
	* @param [in] sz 符号组标准编号
	* @return 
	*/
	virtual void		setStdNum(std::string sz) = 0;

	/**当前组符号按照符号ID排序 
	* @return 
	*/
	virtual void sortSymbolByID() = 0;

	/**当前组符号按照标准编号排序 
	* @return 
	*/
	virtual void sortSymbolByStdNum() = 0;

	/**当前组符号按照符号名称排序 
	* @return 
	*/
	virtual void sortSymbolByName() = 0;

	/**对当前符号组中的指定位置的符号交换位置 
	* @param [in]  index1,  index2，指定符号的索引
	* @return 
	*/
	virtual void swap(long index1, long index2) = 0;

public:
	/** 获取几何对象写入到Buffer中所占用的字节数，便于从Buffer对象中构建几何对象
     * @param  [in] 无 
     * @return 返回几何对象写入到Buffer中所占用的字节数
	 */
    virtual size_t sizeBuffer() = 0;

	/** 从Buffer中读取信息填充本几何对象
     * @param  [in,out] buf Buffer & ，缓冲区对象引用 
     * @return 成功返回true，否则返回false
    */
    virtual bool readBuffer( Buffer & buf) = 0;
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

	/**保存为符号组文件 
	* @param [in] szPath 文件路径名
	* @return 是否保存成功
	*/
	virtual bool save(std::string szPath) = 0;

	/**打开符号组文件 
	* @param [in] szPath 文件路径名
	* @return 是否打开成功
	*/
	virtual bool load(std::string szPath) = 0;

	/**获取当前组内最大的符号ID 
	* @return 当前组内最大的符号ID
	*/
	virtual long getMaxSymbolID() = 0;

public:
	/**根据ID,查找指定的符号 , 如果存在，返回到出口参数outPtr中
	* @param [in] id，符号ID
	* @param [out] outPtr，查找到的符号
	* @return 
	*/
	virtual bool findSymbolById(FSID id, Symbol2dSharedPtr& outPtr) = 0;

	/**根据名称,查找指定的符号 , 如果存在，返回到出口参数outPtr中 
	* @param [in] szName，符号名称
	* @param [out] outPtr，查找到的符号
	* @return 
	*/
	virtual bool findSymbolByName(std::string szName, Symbol2dSharedPtr& outPtr) = 0;

	/**根据标准编号,查找指定的符号 , 如果存在，返回到出口参数outPtr中 
	* @param [in] szStdNum，符号标准编号
	* @param [out] outPtr，查找到的符号
	* @return 
	*/
	virtual bool findSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtr& outPtr) = 0;

	/**根据名称,查找指定的符号（模糊查找，包含字符即符合条件） , 如果存在，添加到出口参数outList中 
	* @param [in] szName，符号名称
	* @param [out] outPtr，查找到的符号列表
	* @return 查找到的符号个数
	*/
	virtual long fuzzyFindSymbolByName(std::string szName, Symbol2dSharedPtrList& outList) = 0;

	/**根据标准编号,查找指定的符号 , （模糊查找，包含字符即符合条件） , 如果存在，添加到出口参数outList中 
	* @param [in] szStdNum，符号标准编号
	* @param [out] outPtr，查找到的符号列表
	* @return 查找到的符号个数
	*/
	virtual long fuzzyFindSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtrList& outList) = 0;
	
	/** 获取符号列表  
     * @param  [in,out] 
     * @return Symbol2dSharedPtrList 符号列表
    */
	virtual Symbol2dSharedPtrList& getSymbolList() = 0;
};
/** @}*/


/** @addtogroup Geometry2d SymbolManager2d 符号组类
*  @{
*/
class CORE_API SymbolManager2d
{
public:
	/** 获取点符号管理器
	* @return 符号管理器指针
	*/
	static SymbolManager2dSharedPtr& getPointSymbolManager();

	/** 获取线符号管理器
	* @return 符号管理器指针
	*/
	static SymbolManager2dSharedPtr& getLineSymbolManager();

	/** 获取面符号管理器
	* @return 符号管理器指针
	*/
	static SymbolManager2dSharedPtr& getRegionSymbolManager();

	/**根据配置文件，依次按顺序读取配置文件中指定的符号库文件 
	* @return 
	*/
	virtual void reloadSymbolFiles() = 0;

	/**获取默认的符号 
	* @return 默认的符号
	*/
	virtual Symbol2dSharedPtr& getDefaultSymbol() = 0;

	/**获取符号组列表 
	* @return 符号组列表
	*/
	virtual SymbolLibrary2dSharedPtrList& getGroupList() = 0;

	/**对当前符号组中的指定位置的符号交换位置 
	* @param [in] index1, index2，指定符号索引
	* @return 
	*/
	virtual void swap(long index1, long index2) = 0;

	/**查找指定的符号组，如果有多个符合条件，返回第一个 
	* @param [in] szGroupName，符号组名称
	* @param [out] outPtr找到的符号组
	* @return 
	*/
	virtual bool findGroup(std::string szGroupName, SymbolLibrary2dSharedPtr& outPtr) = 0;

	/**创建默认的点符号 
	* @return 默认的点符号
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolPoint() = 0;

	/**创建默认的线符号 
	* @return 默认的线符号
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolLine() = 0;

	/**创建默认的填充符号 
	* @return 默认的填充符号 
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolRegion() = 0;

public:	
	/**根据ID,查找指定的符号 , 如果存在，返回到出口参数outPtr中
	*  如果当前符号库中，有多个符号符合查找条件，只返回查找到的第一个 
	* @param [in] id:符号ID，returnDefaultSymbol:如果为true，查找不到的话，返回默认符号
	* @param [out] outPtr:找到的符号
	* @return 是否查找到
	*/
	virtual bool findSymbolById(FSID id, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**根据名称,查找指定的符号 , 如果存在，返回到出口参数outPtr中
	*  如果当前符号库中，有多个符号符合查找条件，只返回查找到的第一个  
	* @param [in] szName:符号名称，returnDefaultSymbol:如果为true，查找不到的话，返回默认符号
	* @param [out] outPtr:找到的符号
	* @return 是否查找到
	*/
	virtual bool findSymbolByName(std::string szName, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**根据标准编号,查找指定的符号 , 如果存在，返回到出口参数outPtr中
	*  如果当前符号库中，有多个符号符合查找条件，只返回查找到的第一个  
	* @param [in] szStdNum:符号标准编号，returnDefaultSymbol:如果为true，查找不到的话，返回默认符号
	* @param [out] outPtr:找到的符号
	* @return 是否查找到
	*/
	virtual bool findSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**根据名称,查找指定的符号（模糊查找，包含字符即符合条件）
	*  如果存在，添加到出口参数outList中， 返回查找到的符号个数  
	* @param [in] szName:符号名称
	* @param [out] outList:找到的符号列表
	* @return 查找到的符号个数
	*/
	virtual long fuzzyFindSymbolByName(std::string szName, Symbol2dSharedPtrList& outList) = 0;

	/**根据标准编号,查找指定的符号 , （模糊查找，包含字符即符合条件）
	*  如果存在，添加到出口参数outList中 ， 返回查找到的符号个数 
	* @param [in] szStdNum:符号标准编号
	* @param [out] outPtr:找到的符号列表
	* @return 找到的符号个数
	*/
	virtual long fuzzyFindSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtrList& outList) = 0;
};
/** @}*/

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
