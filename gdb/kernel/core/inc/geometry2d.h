#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include "buffer.h"
#include "material.h"
#include "matrix4x4.h"
#include "describablebuffer.h"
#include "geometry.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


#define FONT_2D_FACESIZE 32
//�ı������еĻ����ַ�
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


/** @defgroup Geometry2d Element2dType-�߶�����
*  @{
*/
enum Element2dType
{
	ELEMENT_TYPE_2D_UNKNOW = 0,
	ELEMENT_TYPE_2D_ELEMENT,		//Ԫ�ػ��� 

	ELEMENT_TYPE_2D_LINE,			//�߶�
	ELEMENT_TYPE_2D_CIRCULARARC,	//Բ��
	ELEMENT_TYPE_2D_ELLIPTICARC,	//��Բ��
	ELEMENT_TYPE_2D_BEZIERCURVE  	//����������
};
/** @}*/


/** @addtogroup Geometry2d BezierCurve2dType-������������
*  @{
*/
enum BezierCurve2dType
{
	BEZIER_CURVE_2D_UNKNOW = 0,
	BEZIER_CURVE_2D_2_B,			//����B��������
	BEZIER_CURVE_2D_3_B,			//����B����
	BEZIER_CURVE_2D_3,				//��������
	BEZIER_CURVE_2D_POLYLINE		//����
};
/** @}*/

/** @addtogroup Geometry2d MirrorImageType-��������
*  @{
*/
enum MirrorImageType
{
	MIRROR_IMAGE_TYPE_2D_ORIGIN = 0,	//ԭ�㾵��
	MIRROR_IMAGE_TYPE_2D_X,				//X�᾵��
	MIRROR_IMAGE_TYPE_2D_Y				//Y�᾵��
};
/** @}*/

/** @addtogroup Geometry2d ���Ź���������
*  @{
*/
enum Symbol2dManagerType
{
	SYMBOL_MANAGER_TYPE_2D_POINT = 0,	//����Ź���
	SYMBOL_MANAGER_TYPE_2D_LINE,		//�߷��Ź���
	SYMBOL_MANAGER_TYPE_2D_REGION		//�����Ź���
};
/** @}*/

/** @addtogroup Geometry2d ������������
*  @{
*/
enum SymbolSortType
{
	SYMBOL_SORT_TYPE_2D_BY_ID = 0,	//���ݷ��ŵ�ID����
	SYMBOL_SORT_TYPE_2D_BY_NAME,	//���ݷ��ŵ���������
	SYMBOL_SORT_TYPE_2D_BY_STDNUM	//���ݷ��ŵı�׼�������
};
/** @}*/

/** @addtogroup Geometry2d Point2d����
*  @{
*/
enum Point2dType
{
	POINT_2D_TYPE_CIRCLE = 0,	//Բ��
	POINT_2D_TYPE_SQUARE		//�����εĵ�
	//POINT_2D_TYPE_TRIANGLE		//�����εĵ�
};
/** @}*/

/** @addtogroup Geometry2d Element2d 
 *  @{
*/    
class CORE_API Element2d
{
public:
	/**@ ����ָ����Ԫ�����ͣ�����Ԫ��
	*  @return Element2dSharedPtr
	*/
	static Element2dSharedPtr create(int type);

	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ѹ��ģ���Ҫ���¼��� �߽��Χ��
	*  �˺������ú��������getEnvelope�����ȵ���calculateEnvelope����
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ƿ���Ҫ���¼��� �߽��Χ��
	*  @return �Ƿ���Ҫ���¼��� �߽��Χ��
	*/
	virtual bool isDirtyEnvelope( ) = 0;

	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���صı߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** �񳤶�
	* @param ��
	* @return ����
	*/	
	virtual double getLength()=0;
	
	/**       ��ȡ��������
	* @param  [in] ��
	* @return ��������
	*/	
	virtual int getType() = 0;

	/**       ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
	* @param  [in] ��
	* @return ���ر߽���ε�����
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/**       ��g��ָ��ļ��ζ����Ƶ�������
	* @param  [in] g Element2dSharedPtr, ָ��������Ķ����ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/	
	virtual bool copy( Element2dSharedPtr& g) = 0;

	/** �����ڴ�
	* @param ��
	* @return Element2d*����
	*/
	virtual Element2dSharedPtr  clone() = 0;
	
	/**   ��ȡ���ζ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
	*     * @param  [in] �� 
	*     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**��Buffer�ж�ȡ��Ϣ��䱾���ζ��� 
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0;
	
	/**���������Ϣд��Buffer��  
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**���������Ϣд��������У������ļ���д�򻺳���д����  
	*  * @param  [in,out] f  std::ostream &����׼������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** �ӱ�׼����������ȡ�������Ϣ   
	*   * @param  [in,out] f  std::istream &����׼�������������� 
	*   * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/**@name �ӿڲ�ѯ 
	*  @param  [in] signal 
	*  @param  [out] p����ѯ���Ľӿ�ָ�� 
	*  @return ��ѯ�Ƿ�ɹ�
	*/
	virtual bool queryInterface(int signal, void **p )=0;

protected:
	/** ���ü������ͣ���Ҫ��������ʹ��
	*   * @param  [in] type int ���ζ�������
	*   * @return ��
	*/
	virtual void setType(const int type) = 0;
public:
	/** ���ü��ζ���İ�Χ�߽���Σ���Ҫ��������ʹ��
	*   * @param  [in] e3d const Envelope3d & �߽����
	*   * @return ��
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;

	//////////////////////////////////////////////////////////////////////////
	//////////////////  ԭSegment���ֵĽӿ��Ƶ��˴�  ///////////////////////////
	//////////////////////////////////////////////////////////////////////////
public:
	/** ��ȡ�����������Ƿ���
	* @param [in] b
	* @return 
	*/
	virtual bool isClose() = 0;
	virtual void setClose(bool b) = 0;
	
	/** �߶η���
	* @return
	*/
	virtual void reverseOrientation()=0;
	
	/** ��ȡ�߶ε����
	* @return Vertex2d
	*/
	virtual void getFromPoint(Vertex2d& fromPoint)=0;

	/** ��ȡ�߶ε��յ�
	* @return Vertex2d
	*/
	virtual void getToPoint(Vertex2d& toPoint)=0;
	
	/** ��ȡ������ʾ���߶εĵ���
	* @param [in] ��
	* @return ������ʾ���߶εĵ����ָ��
	*/
	virtual VertexCollection2dSharedPtr& getDisplayPointCollection()=0;	
	
	 /**���¼����߶ε���ʾ���� �����ͬʱ��Ҳ����߽����
	 * @param   
	 * @return ����
	 */
	virtual void calculateDisplayPtList()=0;
};
/** @}*/

/** @addtogroup Geometry2d ElementCollection2d-����Ԫ�ؼ��࣬���������������д�GeometryElement2d�̳еļ���Ԫ�ؼ�
*  @{
*/
class CORE_API ElementCollection2d 
{
public:
	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���ص� �߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**���������Ϣд��������У������ļ���д�򻺳���д����
	* @param  [in,out] f  std::ostream &����׼�������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void write(std::ostream & f)=0;
	/**�ӱ�׼����������ȡ�������Ϣ 
	* @param  [in,out] f  std::istream &����׼��������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void read(std::istream & f)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
	* @param ��
	* @return ��ȡ����buffer�ֽ���
	*/
	virtual size_t sizeBuffer()=0 ;

	/** ���Ԫ�ص�Ԫ�ؼ���
	* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
	* @param [in] pt��Ҫ�����Ԫ��
	* @param [in] before���ڴ�����֮ǰ����
	* @param [in] after���ڴ�����֮�����
	* @return
	*/
	virtual void addElement(Element2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool addCollection(ElementCollection2dSharedPtr&  newElements)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
	* @param [in] index������λ��
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool insertCollection(long index,ElementCollection2dSharedPtr&  newElements)=0;

	/** ��ȡָ��������Ԫ��
	* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
	* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
	*/
	virtual Element2dSharedPtr&  getElement(long index)=0;

	/** ��ȡԪ�صĸ���
	* @return: Ԫ�صĸ���
	*/
	virtual long getCount()=0;

	/** ����Ԫ�ؼ��е�����Ԫ��
	* @param [in] index1,index2: Ҫ������Ԫ���������е�����
	* @return: �����ɹ�����true�����򷵻�false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
	* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
	* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
	*/
	virtual Element2dSharedPtr removeElement(long index)=0;

	/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
	* @param [in] index: Ҫɾ����Ԫ�ص�����
	* @return: ɾ��ʧ���򷵻�false
	*/
	virtual bool deleteElement(long index)=0;

	/** ��յ�ǰԪ�ؼ�
	* @return
	*/
	virtual void clear()=0;

	/** @ ��Ŀ������ g �еĿ����� ��ǰ������ 
	* @param  [in] g , ElementCollection2d *, ָ��������������ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/
	virtual bool copy(ElementCollection2dSharedPtr& g) = 0;
	
	
	/** @  ���õ�ǰԪ�ؼ��ϣ��Ƿ����������ƣ����û�У����������������͵�Ԫ��
	* ����У���ֻ�����m_allowedElementTypeList�������Ԫ������
	* @param  [in] elementTypeLimited: �Ƿ�����������
	* @return 
	*/
	virtual void setElementTypeLimited(bool elementTypeLimited) = 0;
	
	/** @  ��ǰԪ�ؼ��ϣ��Ƿ����������ƣ����û�У����������������͵�Ԫ��
	* ����У���ֻ�����m_allowedElementTypeList�������Ԫ������
	* @param  [in] 
	* @return �Ƿ�����������
	*/
	virtual bool isElementTypeLimited() = 0;
	
	/** @  ��ǰԪ������elementType���ܷ���ӵ���ǰԪ�ؼ�����
	* @param  [in] elementType: Ԫ������
	* @return bool �ܷ����
	*/
	virtual bool canBeAddIn(int elementType) = 0;
	
	/** @  ��ȡ�����Ԫ�����͵ĸ���
	* @param  [in] 
	* @return �����Ԫ�����͵ĸ���
	*/
	virtual long getAllowedElementTypeCount() = 0;
	
	/** @  ��� ����ӵ�Ԫ������
	* @param  [in] elementType: ����ӵ�Ԫ������
	* @return 
	*/
	virtual void addAllowedElementType(int elementType) = 0;
	
	/** @  ��ȡָ�������� ����ӵ�Ԫ������
	* @param  [in] lIndex: ����
	* @return Ԫ������
	*/
	virtual int getAllowedElementType(long lIndex) = 0;
	
	/** @  �ӿ����Ԫ�������б��У��Ƴ�ָ���� ����ӵ�Ԫ������elementType
	* ���ָ���������б��б����Ͳ����ڣ������κ���
	* @param  [in] elementType: Ҫ�Ƴ���Ԫ������
	* @return 
	*/
	virtual void removeAllowedElementType(int elementType) = 0;
	
	/** @  ��������Ԫ�������б���
	* @return 
	*/
	virtual void clearAllowedElementType() = 0;
};
/** @}*/


/** @addtogroup Geometry2d Font2d-����ṹ��
*  @{
*/
#define GV_2D_FACE_SIZE 32
struct Font2d
{
	long      lfHeight;						//�ָ�
	long      lfWidth;						//�ֿ�
	long      lfEscapement;					//
	long      lfOrientation;				//��ת����
	long      lfWeight;						//�ֵ���������ϸ��
	long      lfItalic;						//б��
	long      lfUnderline;					//�»���
	long      lfStrikeOut;					//ͻ��
	long      lfCharSet;					//�ַ���
	long      lfOutPrecision;				//
	long      lfClipPrecision;				//���еľ���
	long      lfQuality;					//Ʒ��
	long      lfPitchAndFamily;				//
	char      lfFaceName[GV_2D_FACE_SIZE];		//��������
};
/** @}*/

/** @addtogroup Geometry2d ElementLine2d-ֱ�߶���
*  @{
*/
class CORE_API ElementLine2d : virtual public Element2d
{
public:	
	/** ��ȡ����߶εĵ���
	* @return ����߶ε����ָ��
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;	
};
/** @}*/


/** @addtogroup Geometry2d ElementCircularArc2d-Բ����
*  @{
*/
class CORE_API ElementCircularArc2d : virtual public Element2d
{
public:	
	/** ����Բ���Ĳ�����Բ��
	* @param [in] Vertex2d& pt Բ������
	* @return 
	*/
	virtual void setCenterPoint(Vertex2d& pt) = 0;
	
	/** ����Բ���Ĳ�����Բ��
	* @param [in]  Բ������, x, y
	* @return 
	*/
	virtual void setCenterPoint(double x,double y) = 0;

	/** ����Բ���Ĳ������뾶
	* @param [in] �뾶r
	* @return 
	*/
	virtual void setRadius(double r) = 0;

	/** ����Բ���Ĳ�������ʼ�ǣ��Ƕȵ�λ��
	* @param [in] ��ʼ�ǣ��Ƕȵ�λ��fromAng
	* @return 
	*/
	virtual void setFromAngleDeg(double fromAng) = 0;

	/** ����Բ���Ĳ�����ɨ��Ƕȣ��Ƕȵ�λ��
	* @param [in] ɨ��Ƕȣ��Ƕȵ�λ��centralAng
	* @return 
	*/
	virtual void setCentralAngleDeg(double centralAng) = 0;
	//virtual void setClose(bool b) = 0;

	/** ����Բ���Ĳ�����Բ�ģ��뾶����ʼ�ǣ��Ƕȵ�λ����ɨ��Ƕȣ��Ƕȵ�λ�����Ƿ���
	* @param [in] Vertex2d& ptCenter,double radius,double fromAngle,double centralAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(Vertex2d& ptCenter,double radius,double fromAngle,double centralAngle,bool isClose) = 0;

	/** ����Բ���Ĳ�����Բ�ģ��뾶����ʼ�ǣ��Ƕȵ�λ����ɨ��Ƕȣ��Ƕȵ�λ�����Ƿ���
	* @param [in] double centerX,double centerY,double radius,double fromAngle,double centralAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(double centerX,double centerY,double radius,double fromAngle,double centralAngle,bool isClose) = 0;


	/** ��ȡԲ���Ĳ�����Բ��
	* @return Բ���Ĳ�����Բ��Vertex2d
	*/
	virtual const Vertex2d& getCenterPoint() = 0;

	/** ��ȡԲ���Ĳ�����Բ��
	* @param [out] Բ�� pt
	*/
	virtual void getCenterPoint(Vertex2d& pt) = 0;

	/** ��ȡԲ���Ĳ������뾶
	* @return Բ���Ĳ������뾶
	*/
	virtual double  getRadius() = 0;

	/** ��ȡԲ���Ĳ�������ʼ�ǣ��Ƕȵ�λ��
	* @return Բ���Ĳ�������ʼ�ǣ��Ƕȵ�λ��
	*/
	virtual double  getFromAngleDeg() = 0;

	/** ��ȡԲ���Ĳ�����ɨ��Ƕȣ��Ƕȵ�λ��
	* @return Բ���Ĳ�����ɨ��Ƕȣ��Ƕȵ�λ��
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
	/** ����Բ���Ĳ�����Բ��
	* @param [in] Բ��pt
	* @return 
	*/
	virtual void setCenterPoint(Vertex2d& pt) = 0;

	/** ����Բ���Ĳ�����Բ��
	* @param [in] Բ��:x,y
	* @return 
	*/
	virtual void setCenterPoint(double x,double y) = 0;

	/** ����Բ���Ĳ���������뾶
	* @param [in] ����뾶semiMajor
	* @return 
	*/
	virtual void setSemiMajor(double semiMajor) = 0;

	/** ����Բ���Ĳ���������뾶
	* @param [in] ����뾶semiMinor
	* @return 
	*/
	virtual void setSemiMinor(double semiMinor) = 0;

	/** ����Բ���Ĳ�������ʼ�ǣ��Ƕȵ�λ��
	* @param [in] ��ʼ�ǣ��Ƕȵ�λ��fromAng
	* @return 
	*/
	virtual void setFromAngleDeg(double fromAng) = 0;

	/** ����Բ���Ĳ����� ɨ��Ƕȣ��Ƕȵ�λ��
	* @param [in] ɨ��Ƕȣ��Ƕȵ�λ��centralAng
	* @return 
	*/
	virtual void setCentralAngleDeg(double centralAng) = 0;

	/** ����Բ���Ĳ�������ת�Ƕ�
	* @param [in] ��ת�Ƕ�rotateAng
	* @return 
	*/
	virtual void setRotateAngleDeg(double rotateAng) = 0;

	/** ����Բ���Ĳ�����Բ�ģ�����뾶������뾶����ʼ�ǣ��Ƕȵ�λ����
	* ɨ��Ƕȣ��Ƕȵ�λ������ת�Ƕȣ��Ƿ���
	* @param [in] Vertex2d& center,double semiMajor,double minorMajorRatio
	* @param [in] double fromAngle,double centralAngle,double rotationAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(Vertex2d& center,double semiMajor,double minorMajorRatio,
		double fromAngle,double centralAngle,double rotationAngle,bool isClose) = 0;

	/** ����Բ���Ĳ�����Բ�ģ�����뾶������뾶����ʼ�ǣ��Ƕȵ�λ����
	* ɨ��Ƕȣ��Ƕȵ�λ������ת�Ƕȣ��Ƿ���
	* @param [in] double centerX,double centerY,double semiMajor,double minorMajorRatio
	* @param [in] double fromAngle,double centralAngle,double rotationAngle,bool isClose
	* @return 
	*/
	virtual void putCoordsByAngle(double centerX,double centerY,double semiMajor,double minorMajorRatio,
		double fromAngle,double centralAngle,double rotationAngle,bool isClose) = 0;

	/** ��ȡԲ���Ĳ�����Բ��
	* @return Բ��Vertex2d
	*/
	virtual const Vertex2d& getCenterPoint() = 0;

	/** ��ȡԲ���Ĳ�����Բ��
	* @param [out] Բ��pt
	* @return 
	*/
	virtual void getCenterPoint(Vertex2d& pt) = 0;

	/** ��ȡԲ���Ĳ���������뾶
	* @return ����뾶
	*/
	virtual double getSemiMajor() = 0;

	/** ��ȡԲ���Ĳ���������뾶
	* @return ����뾶
	*/
	virtual double getSemiMinor() = 0;

	/** ��ȡԲ���Ĳ���������
	* @return ����
	*/
	virtual double getMinorMajorRatio() = 0;

	/** ��ȡԲ���Ĳ�������ʼ�ǣ��Ƕȵ�λ��
	* @return ��ʼ�ǣ��Ƕȵ�λ��
	*/
	virtual double getFromAngleDeg() = 0;

	/** ��ȡԲ���Ĳ�����ɨ��Ƕȣ��Ƕȵ�λ��
	* @return ɨ��Ƕȣ��Ƕȵ�λ��
	*/
	virtual double getCentralAngleDeg() = 0;

	/** ��ȡԲ���Ĳ�������ת�Ƕ�
	* @return ��ת�Ƕ�
	*/
	virtual double getRotateAngleDeg() = 0;
	
	/** ͨ�������ĽǶ�dAngleDeg���Ƕȵ�λ��,������Բ�ڸýǶȵĵ������
	* @param [in]  �����ĽǶ�dAngleDeg����Բ���������еĽǶ�
	* @param [out] ָ���Ƕ��϶�Ӧ�ĵ�����x,y
	* @return 
	*/	
	virtual void calculatePointByAngleDeg(double dAngleDeg, double &xx, double &yy) = 0;
	
};
/** @}*/

/** @addtogroup Geometry2d ElementBezierCurve2d-����������
*  @{
*/
class CORE_API ElementBezierCurve2d  : virtual public Element2d
{
public:	
	/** ��ȡ���ߵ�����
	* @return ���ߵ�����int
	*/
	virtual int getSplineType() = 0;

	/** �������ߵ�����
	* @param [in] ���ߵ�����type
	*/
	virtual void setSplineType(int type) = 0;
	
	/** ��ȡ���Ƶ���
	* @param [in] ��
	* @return ���Ƶ����ָ��
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;	
};
/** @}*/


/** @addtogroup Geometry2d Path2d-·���࣬��������Segment2d��ɣ�
*����·���ĵ�һ�������һ����ɵ�Segment2d֮�⣬
*ÿһ��Segment2d����ʼ�㶼��ǰһ��Segment2d����ֹ��
*��·�������е�Segment2d���ܳ��ַ���������
*·��������������Ŀ������Segment2d���͵���ϡ�
*  @{
*/
class CORE_API Path2d
{
public:	
	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ѹ��ģ���Ҫ���¼��� �߽��Χ��
	*  �˺������ú��������getEnvelope�����ȵ���calculateEnvelope����
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ƿ���Ҫ���¼��� �߽��Χ��
	*  @return �Ƿ���Ҫ���¼��� �߽��Χ��
	*/
	virtual bool isDirtyEnvelope( ) = 0;
	
	 /**���¼����߶ε���ʾ���� �����ͬʱ��Ҳ����߽����
	 * @param  [in] 
	 * @return ����
	 */
	virtual void calculateDisplayPtList() = 0;

	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���صı߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** �����ڴ�
	* @param ��
	* @return Element2d*����
	*/
	virtual Path2dSharedPtr  clone() = 0;
	
	/** ��g��ָ��ļ��ζ����Ƶ�������
	* @param  [in] g Element2dSharedPtr, ָ��������Ķ����ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/	
	virtual bool copy( Path2dSharedPtr& g) = 0;

	/** �񳤶�
	* @param ��
	* @return ����
	*/	
	virtual double getLength() = 0;
	
	/** ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
	* @param  [in] ��
	* @return ���ر߽���ε�����
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/** ���ü��ζ���İ�Χ�߽���Σ���Ҫ��������ʹ��
	*   * @param  [in] e3d const Envelope3d & �߽����
	*   * @return ��
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;

	/** ��ȡ�����Ƿ���
	* @return �Ƿ���
	*/
	virtual bool isClose() = 0;

	/** ���������Ƿ���
	* @param [in] b
	* @return 
	*/
	virtual void setClose(bool b) = 0;
	
	/** ��ȡ�߶ε����
	* @param [out] fromPoint
	* @return 
	*/
	virtual void getFromPoint(Vertex2d& fromPoint) = 0;

	/** ��ȡ�߶ε��յ�
	* @param [out] toPoint
	* @return 
	*/
	virtual void getToPoint(Vertex2d& toPoint) = 0;
		
	/** ��ȡ������ʾ���߶εĵ���
	* @param [out] vtList������
	* @return ��ȡ�Ƿ�ɹ�
	*/
	virtual bool getDisplayPointCollection(VertexCollection2d& vtList) = 0;	

	/** ��ȡ��ǰ·�����߶��б�
	* @param ��
	* @return SegmentCollection2dָ�룬��ǰ·�����߶��б�
	*/
	virtual ElementCollection2dSharedPtr& getSegmentCollection() = 0;

	/** ʹ·����Ч��ÿ���߶ε����Ϊ�¸��߶ε��յ㣩
	* @param �� 
	* @return ��
	*/
	virtual void simplify() = 0;
	
	/**   ��ȡ���ζ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
	*     * @param  [in] �� 
	*     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**��Buffer�ж�ȡ��Ϣ��䱾���ζ��� 
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0 ;
	
	/**���������Ϣд��Buffer��  
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**���������Ϣд��������У������ļ���д�򻺳���д����  
	*  * @param  [in,out] f  std::ostream &����׼������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** �ӱ�׼����������ȡ�������Ϣ   
	*   * @param  [in,out] f  std::istream &����׼�������������� 
	*   * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/** �߶η���
	* @return
	*/
	virtual void reverseOrientation() = 0;
};
/** @}*/

/** @addtogroup Geometry2d PathCollection2d-·�����࣬����������������·��Path2d
*  @{
*/
class CORE_API PathCollection2d 
{
public:
	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���ص� �߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**���������Ϣд��������У������ļ���д�򻺳���д����
	* @param  [in,out] f  std::ostream &����׼�������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void write(std::ostream & f)=0;
	/**�ӱ�׼����������ȡ�������Ϣ 
	* @param  [in,out] f  std::istream &����׼��������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void read(std::istream & f)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
	* @param ��
	* @return ��ȡ����buffer�ֽ���
	*/
	virtual size_t sizeBuffer()=0 ;

	/** ���Ԫ�ص�Ԫ�ؼ���
	* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
	* @param [in] pt��Ҫ�����Ԫ��
	* @param [in] before���ڴ�����֮ǰ����
	* @param [in] after���ڴ�����֮�����
	* @return
	*/
	virtual void addElement(Path2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool addCollection(PathCollection2dSharedPtr&  newElements)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
	* @param [in] index������λ��
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool insertCollection(long index,PathCollection2dSharedPtr&  newElements)=0;

	/** ��ȡָ��������Ԫ��
	* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
	* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
	*/
	virtual Path2dSharedPtr&  getElement(long index)=0;

	/** ��ȡԪ�صĸ���
	* @return: Ԫ�صĸ���
	*/
	virtual long getCount()=0;

	/** ����Ԫ�ؼ��е�����Ԫ��
	* @param [in] index1,index2: Ҫ������Ԫ���������е�����
	* @return: �����ɹ�����true�����򷵻�false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
	* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
	* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
	*/
	virtual Path2dSharedPtr removeElement(long index)=0;

	/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
	* @param [in] index: Ҫɾ����Ԫ�ص�����
	* @return: ɾ��ʧ���򷵻�false
	*/
	virtual bool deleteElement(long index)=0;

	/** ��յ�ǰԪ�ؼ�
	* @return
	*/
	virtual void clear()=0;

	/** @ ��Ŀ������ g �еĿ����� ��ǰ������ 
	* @param  [in] g , ElementCollection2d *, ָ��������������ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/
	virtual bool copy(PathCollection2dSharedPtr& g) = 0;
};
/** @}*/

/** @addtogroup Geometry2d Ring2dType-��������
*  @{
*/
enum Ring2dType
{
	RINT_TYPE_2D_UNKNOW = 0,		//δ֪����
	RINT_TYPE_2D_INTERIOR,			//�ڻ�
	RINT_TYPE_2D_EXTRRIOR			//�⻷
};
/** @}*/

/** @addtogroup Geometry2d Ring2d-���࣬��������Segment2d��ɣ�
*���˻��ĵ�һ�������һ����ɵ�Segment2d֮�⣬
*ÿһ��Segment2d����ʼ�㶼��ǰһ��Segment2d����ֹ��
*���������е�Segment2d���ܳ��ַ���������
*��������������Ŀ������Segment2d���͵���ϡ�
*���е�Segment2d���������һ�µ�
*�������Ƿ�յģ���������ʼ���������ֹ��һ��
*���������ཻ
*  @{
*/
class CORE_API Ring2d 
{
public:

	/** ��ȡ��ǰ�����߶��б�
	* @return SegmentCollection2dָ��
	*/
	virtual ElementCollection2dSharedPtr& getSegmentCollection() = 0;

	/** ��ȡ�������
	* @return double
	*/
	virtual double getArea() = 0;

	/** ��ȡ���ļ������ģ��������ɱ�ע
	* @return Vertex2d
	*/
	virtual Vertex2d getCentrolid() = 0;

	/** �趨�����
	* @return 
	*/
	virtual void setClose() = 0;

	/** ʹ����Ч
	* @return 
	*/
	virtual void simplify() = 0;

	/** ��ȡ��������
	* @return ��������
	*/
	virtual int getRingType() = 0;

	/** ���û�������
	* @param [in] ��������ringType
	* @return 
	*/
	virtual void setRingType(Ring2dType ringType) = 0;

	
	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ѹ��ģ���Ҫ���¼��� �߽��Χ��
	*  �˺������ú��������getEnvelope�����ȵ���calculateEnvelope����
	*  @return 
	*/
	virtual void dirtyEnvelope( bool bNeedReCalculateEnvelope ) = 0;

	/**@name ָʾ��ǰԪ�ؼ�����Ϣ�Ƿ���Ҫ���¼��� �߽��Χ��
	*  @return �Ƿ���Ҫ���¼��� �߽��Χ��
	*/
	virtual bool isDirtyEnvelope( ) = 0;
	
	 /**���¼����߶ε���ʾ���� �����ͬʱ��Ҳ����߽����
	 * @param  [in] 
	 * @return ����
	 */
	virtual void calculateDisplayPtList() = 0;

	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���صı߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/** �����ڴ�
	* @param ��
	* @return Element2d*����
	*/
	virtual Ring2dSharedPtr  clone() = 0;
	
	/** ��g��ָ��ļ��ζ����Ƶ�������
	* @param  [in] g Element2dSharedPtr, ָ��������Ķ����ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/	
	virtual bool copy( Ring2dSharedPtr& g) = 0;

	/** �񳤶�
	* @param ��
	* @return ����
	*/	
	virtual double getLength() = 0;

	/** ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
	* @param  [in] ��
	* @return ���ر߽���ε�����
	*/	
	virtual const Envelope3d &getEnvelope() = 0;
	
	/** ���ü��ζ���İ�Χ�߽���Σ���Ҫ��������ʹ��
	*   * @param  [in] e3d const Envelope3d & �߽����
	*   * @return ��
	*/
	virtual void setEnvelope(const Envelope3d &e3d) = 0;
	
	/** ��ȡ������ʾ���߶εĵ���
	* @param [out] vtList������
	* @return ��ȡ�Ƿ�ɹ�
	*/
	virtual bool getDisplayPointCollection(VertexCollection2d& vtList) = 0;	
	
	/**   ��ȡ���ζ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
	*     * @param  [in] �� 
	*     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	*/
	virtual size_t sizeBuffer() = 0;
	
	/**��Buffer�ж�ȡ��Ϣ��䱾���ζ��� 
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool readBuffer(Buffer &buf) = 0 ;
	
	/**���������Ϣд��Buffer��  
	*  * @param  [in,out] buf Buffer & ���������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual bool writeBuffer(Buffer& buf) = 0;
	
	/**���������Ϣд��������У������ļ���д�򻺳���д����  
	*  * @param  [in,out] f  std::ostream &����׼������������� 
	*  * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void write(std::ostream &f) = 0;
	
	/** �ӱ�׼����������ȡ�������Ϣ   
	*   * @param  [in,out] f  std::istream &����׼�������������� 
	*   * @return �ɹ�����true�����򷵻�false 
	*/
	virtual void read(std::istream &f) = 0;
	
	/** �߶η���
	* @return
	*/
	virtual void reverseOrientation() = 0;
};
/** @}*/

/** @addtogroup Geometry2d RingCollection2d-�����࣬���������������л�Ring2d
*  @{
*/
class CORE_API RingCollection2d 
{
public:
	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [out] outEnvp:���ص� �߽��Χ��
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp ) = 0;

	/**���������Ϣд��������У������ļ���д�򻺳���д����
	* @param  [in,out] f  std::ostream &����׼�������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void write(std::ostream & f)=0;
	/**�ӱ�׼����������ȡ�������Ϣ 
	* @param  [in,out] f  std::istream &����׼��������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void read(std::istream & f)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
	* @param ��
	* @return ��ȡ����buffer�ֽ���
	*/
	virtual size_t sizeBuffer()=0 ;

	/** ���Ԫ�ص�Ԫ�ؼ���
	* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
	* @param [in] pt��Ҫ�����Ԫ��
	* @param [in] before���ڴ�����֮ǰ����
	* @param [in] after���ڴ�����֮�����
	* @return
	*/
	virtual void addElement(Ring2dSharedPtr& obj,int* before=NULL, int* after=NULL)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool addCollection(RingCollection2dSharedPtr&  newElements)=0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
	* @param [in] index������λ��
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool insertCollection(long index,RingCollection2dSharedPtr&  newElements)=0;

	/** ��ȡָ��������Ԫ��
	* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
	* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
	*/
	virtual Ring2dSharedPtr&  getElement(long index)=0;

	/** ��ȡԪ�صĸ���
	* @return: Ԫ�صĸ���
	*/
	virtual long getCount()=0;

	/** ����Ԫ�ؼ��е�����Ԫ��
	* @param [in] index1,index2: Ҫ������Ԫ���������е�����
	* @return: �����ɹ�����true�����򷵻�false
	*/
	virtual bool swap(long index1,long index2)=0;

	/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
	* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
	* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
	*/
	virtual Ring2dSharedPtr removeElement(long index)=0;

	/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
	* @param [in] index: Ҫɾ����Ԫ�ص�����
	* @return: ɾ��ʧ���򷵻�false
	*/
	virtual bool deleteElement(long index)=0;

	/** ��յ�ǰԪ�ؼ�
	* @return
	*/
	virtual void clear()=0;

	/** @ ��Ŀ������ g �еĿ����� ��ǰ������ 
	* @param  [in] g , ElementCollection2d *, ָ��������������ָ�� 
	* @return �Ƿ񿽱��ɹ�
	*/
	virtual bool copy(RingCollection2dSharedPtr& g) = 0;
};
/** @}*/



/** @addtogroup Geometry2d Geometry2d-��ά������
*  @{
*/
//##ModelId=4D0B58C6005F
class  CORE_API Geometry2d : public Geometry
{
public:
	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*  @param  [in] pCoordSys:����ϵͳָ�� 
	*  @param  [out] outEnvp: �߽��Χ�� 
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Coordsys2dSharedPtr& pCoorSys, Envelope3d& outEnvp )=0;

	/**@name ���㵱ǰ����ͼԪ�ı߽��Χ��
	*        �˺�����������ĵ㡢�ı�����ע����İ�Χ�У�����׼ȷ����Ҫ����Coordsys2d�������ȷ�İ�Χ��
	*  @param  [out] outEnvp: �߽��Χ�� 
	*  @return �����Ƿ�ɹ� 
	*/
	virtual bool calculateEnvelope( Envelope3d& outEnvp )=0;
	
	/** ��ȡ���ζ������ɫ
	* @return ���ζ������ɫColor4f
	*/
	virtual Color4f getPenColor() = 0;

	/** ��ȡ���ζ������ɫ
	* @param [out] ���ζ������ɫcr
	* @return 
	*/
	virtual void getPenColor(Color4f &cr) = 0;

	/** ���ü��ζ������ɫ
	* @param [in] ���ζ������ɫcr
	* @return 
	*/
	virtual void setPenColor(Color4f cr) = 0;
	
	/** ���ü��ζ������ɫ
	* @param [in] ���ζ������ɫr,g,b,a��ע�⣺ r,g,b��ȡֵ��Χ��0 - 1
	* @return 
	*/
	virtual void setPenColor(float r, float g, float b, float a)=0;
	
	/** ��ȡ���ζ���������ɫ�����������������Ч
	* @return �����ɫColor4f
	*/
	virtual Color4f getBrushColor() = 0;
	
	/** ��ȡ���ζ���������ɫ�����������������Ч
	* @param [out] �����ɫcr
	* @return 
	*/
	virtual void getBrushColor(Color4f &cr) = 0;
	
	/** ���ü��ζ���������ɫ�����������������Ч
	* @param [in] �����ɫcr
	* @return 
	*/
	virtual void setBrushColor(Color4f cr) = 0;
	
	/** ���ü��ζ���������ɫ�����������������Ч
	* @param [in] �����ɫr,g,b,a
	* @return 
	*/
	virtual void setBrushColor(float r, float g, float b, float a)=0;

	/** ����״ֵ̬����ǰ�����Ƿ�Ϊĳ����ĳ�Ա
	* @param [in] bool b
	* @return 
	*/
	virtual void setGeometryInGroup(bool b) = 0;

	/** ��ǰ���ζ����Ƿ�Ϊĳ����ĳ�Ա
	* @return �Ƿ�Ϊĳ����ĳ�Ա
	*/	
	virtual bool isGeometryInGroup() = 0;

	/** ״̬��ʾ����, �ж�ͼԪ�Ƿ��Ǵ���ɾ��״̬ 
	*/	
	virtual bool isDeleted() = 0;

	/** ����ͼԪ������״̬ 
	*/	
	virtual bool isVisible() = 0;

	/** ����ͼԪ��ѡ��״̬ 
	*/	
	virtual bool isSelected() = 0;

	/** ����ͼԪ��ɾ��״̬ 
	* @param [in] bool �Ƿ�ɾ��
	*/	
	virtual void setDeleteFlag(bool bDelete) = 0;

	/** ����ͼԪ������״̬ 
	* @param [in] bool �Ƿ�ɼ�
	*/	
	virtual void setVisible(bool bHide) = 0;

	/** ����ͼԪ��ѡ��״̬ 
	* @param [in] bool �Ƿ�ѡ��
	*/	
	virtual void setSelectFlag(bool bSel) = 0;

public:	
	/**��ǰ����Χ��ָ��ԭ�� org ������ת���Ƕȵ�λ���� 
	* @param [in] ԭ�� org, ��ת�Ƕ�dAngleDegree
	*/
	virtual void rotate(Vertex2d &org, double dAngleDegree) = 0;

	/** ��ǰ���󣬰�ָ���� org Ϊԭ����б����任 
	* @param [in] scaleX,scaleYΪx,y��ı任����, org Ϊԭ������
	*/
	virtual void transformByScale(Vertex2d &org,double& scaleX, double& scaleY) = 0;

	/**ƽ�� 
	* @param [in] λ�ƣ�offX, offY
	*/
	virtual void translate(double offX, double offY) = 0;

	/**���ݾ������ͣ����о���任 
	* @param [in] mirrorType��������
	*/
	virtual void mirrorImage(int mirrorType) = 0;

	/**Χ��pt1��pt2��ɵ��߶Σ����о���任 
	* @param [in] ����߶εĵ�pt1,pt2
	*/
	virtual void mirrorImage(Vertex2d& pt1,Vertex2d& pt2) = 0;

	//////////////////////////////////////////////////////////////////////////	
	/** ���ü��ζ���������ɫ�����������������Ч
	* @param [in] �����ɫr,g,b,a
	* @return 
	*/
	//virtual void setBrushColor(int r, int g, int b, int a)=0;

	/** ���ü��ζ������ɫ
	* @param [in] ���ζ������ɫr255,g255,b255,a255��ע�⣺ r,g,b��ȡֵ��Χ��0 - 255
	* @return 
	*/
	//virtual void setPenColor(int r255, int g255, int b255, int a)=0;
	/** ��ȡ���ζ����ID
	* @return ���ζ����ID
	*/
	//virtual FSID getID() = 0;

	/** ���ü��ζ����ID
	* @param [in] ���ζ����ID
	* @return 
	*/
	//virtual void setID(FSID id) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeometryCollection2d-���ζ�����
*  @{
*/
class CORE_API GeometryCollection2d 
{
public:
	/**���������Ϣд��������У������ļ���д�򻺳���д����
	* @param  [in,out] f  std::ostream &����׼�������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void write(std::ostream & f) = 0;

	/**�ӱ�׼����������ȡ�������Ϣ 
	* @param  [in,out] f  std::istream &����׼��������������
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual void read(std::istream & f) = 0;

	/** ���Ԫ�ص�Ԫ�ؼ���
	* ���before, after��Ϊ�գ�����ӵ�Ԫ�ؼ���ĩβ
	* @param [in] pt��Ҫ�����Ԫ��
	* @param [in] before���ڴ�����֮ǰ����
	* @param [in] after���ڴ�����֮�����
	* @return
	*/
	virtual void addGeometry(GeometrySharedPtr& obj,int* before=NULL, int* after=NULL) = 0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ����ӵ���ǰԪ�ؼ���ĩβ
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool addCollection(GeometryCollection2dSharedPtr& newElements) = 0;

	/** ��Ŀ��Ԫ�ؼ��е�Ԫ�ز��뵽��ǰԪ�ؼ���
	* @param [in] index������λ��
	* @param [in] newElements��Ŀ��Ԫ�ؼ�
	* @return ����Ƿ�ɹ�
	*/
	virtual bool insertCollection(long index,GeometryCollection2dSharedPtr& newElements) = 0;

	/** ��ȡָ��������Ԫ��
	* @param [in] index: Ҫ��ȡ��Ԫ�ص�����
	* @return: ��ȡ����Ԫ�ص�ָ�룬��ȡʧ���򷵻�NULL
	*/
	virtual GeometrySharedPtr& getGeometry(long index) = 0;

	/** ��ȡԪ�صĸ���
	* @return: Ԫ�صĸ���
	*/
	virtual long getCount() = 0;

	/** ��Ԫ�ؼ����Ƴ�ָ��������Ԫ�أ��������Ƴ���Ԫ��
	* @param [in] index: Ҫ�Ƴ���Ԫ�ص�����
	* @return: �Ƴ���Ԫ�ص�ָ�룬ʧ���򷵻�NULL
	*/
	virtual GeometrySharedPtr removeGeometry(long index) = 0;

	/** ��Ԫ�ؼ���ɾ��ָ��������Ԫ��
	* @param [in] index: Ҫɾ����Ԫ�ص�����
	* @return: ɾ��ʧ���򷵻�false
	*/
	virtual bool deleteGeometry(long index) = 0;

	/** ��յ�ǰԪ�ؼ�
	* @return
	*/
	virtual void clear() = 0;
	
	/** ��Ŀ������ g �еĿ����� ��ǰ������
	*   @param  [in] g const GeometryCollection2d *, ָ��������������ָ��
	*   @return �Ƿ񿽱��ɹ�
	*/
	virtual bool copy(const GeometryCollection2dSharedPtr& g) = 0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool writeBuffer (Buffer &buf)=0;

	/*��Buffer�ж�ȡ��Ϣ��䱾���ζ���
	* @param  [in,out] buf Buffer & ���������������� 
	* @return �ɹ�����true�����򷵻�false
	*/
	virtual bool readBuffer (Buffer &buf)=0;

	/** ��ȡ����д�뵽Buffer����ռ�õ��ֽ���
	* @param ��
	* @return ��ȡ����buffer�ֽ���
	*/
	virtual size_t sizeBuffer()=0 ;

	/** ����Ԫ�ؼ��е�����Ԫ��
	* @param [in] index1,index2: Ҫ������Ԫ���������е�����
	* @return: �����ɹ�����true�����򷵻�false
	*/
	virtual bool swap(long index1,long index2)=0;
	
	/** ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
    * @param  [in] ��
    * @return ���ر߽���ε�����
	*/
	virtual void getEnvelope(Envelope3d & envp)=0;
	
	/**��ǰ����Χ��ָ��ԭ�� org ������ת���Ƕȵ�λ����
	* @param [in] ԭ�� org,��ת�Ƕȣ�dAngleDegree
	*/
	virtual void rotate(Vertex2d &org, double dAngleDegree) = 0;

	/** ��ǰ���󣬰�ָ���� org Ϊԭ����б����任 
	* @param [in] scaleX,scaleYΪx,y��ı任����,ԭ�� org
	*/
	virtual void transformByScale(Vertex2d &org,double& scaleX, double& scaleY) = 0;

	/**ƽ�� 
	* @param [in] λ�ƣ� offX, offY
	*/
	virtual void translate(double offX, double offY) = 0;

	/**���ݾ������ͣ����о���任 
	* @param [in] ��������mirrorType
	*/
	virtual void mirrorImage(int mirrorType) = 0;

	/**Χ��pt1��pt2��ɵ��߶Σ����о���任 
	* @param [in] pt1��pt2��ɵ��߶�
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
	PEN_TYPE_2D_SYSTEM,			//ϵͳ�Դ����ʣ����ֻ������ͣ����ߣ��㻮�ߣ����ߣ�˫�㻮�ߣ�ʵ�ߣ�
	PEN_TYPE_2D_USERDEFINE		//�û��Զ��廭�ʣ��Զ������ͣ����������ͣ�
};
/** @}*/


/** @addtogroup Geometry2d Pen2d-������
*  @{
*/

class CORE_API Pen2d
{
public:
	/**��g��ָ��Ķ����Ƶ������� 
	*  * @param  [in] g Pen2dSharedPtr, ָ������������ָ�� 
	*  * @return �Ƿ񿽱��ɹ� 
	*/
	virtual bool copy( Pen2dSharedPtr& g) = 0;

	/** ��ȡ��������
	* @return��������Pen2dType
	*/
	virtual Pen2dType getType() = 0;

	/** ���û�������
	* @param [in] ��������tp
	* @return
	*/
	virtual void setType(Pen2dType tp) = 0;

	/** ��ȡ�Զ��廭�ʵ����ƣ����Ϊ�û��Զ��廭��ʱ��Ч
	* @return���ʵ�����
	*/
	virtual string getUserDefineTypeName() = 0;

	/** �����Զ��廭�ʵ����ƣ����Ϊ�û��Զ��廭��ʱ��Ч
	* @param [in] ���ʵ�����name
	* @return
	*/
	virtual void   setUserDefineTypeName(string name) = 0;

	/** ��ȡ�Զ��廭�ʵ����ͣ����Ϊϵͳ����ʱ��Ч
	* @return ϵͳ���ʵ�����
	*/
	virtual int  getSystemTypeName() = 0;

	/** �����Զ��廭�ʵ����ͣ����Ϊϵͳ����ʱ��Ч
	* @param [in] ϵͳ���ʵ�����type
	* @return
	*/
	virtual void setSystemTypeName(int type) = 0;

	/** ��ȡ���ʵĿ�ȣ����Ϊϵͳ����ʱ��Ч
	* @return ���ʵĿ��
	*/
	virtual double  getWidth() = 0;

	/** ���û��ʵĿ�ȣ����Ϊϵͳ����ʱ��Ч
	* @param [in] ���ʵĿ��d
	* @return
	*/
	virtual void    setWidth(double d) = 0;

	/** ��ȡ���ʵ���ʾ���������Ϊ�û��Զ��廭��ʱ��Ч
	* @return ���ʵ���ʾ����
	*/
	virtual double  getScale() = 0;

	/** ���û��ʵ���ʾ���������Ϊ�û��Զ��廭��ʱ��Ч
	* @param [in] ���ʵ���ʾ���� d
	* @return
	*/
	virtual void    setScale(double d) = 0;

		
	/** ��ȡ���ʶ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ʶ���
     * @param  [in] �� 
     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	 */
    virtual size_t sizeBuffer()=0;

	/** ��Buffer�ж�ȡ��Ϣ��䱾���ʶ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf)=0;

	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf)=0;
};
/** @}*/


/** @addtogroup Geometry2d GeomPolygon2d-��������ζ�����
*  @{
*/

class CORE_API GeomPolygon2d : virtual public Geometry2d
{
public:
	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** ��ȡ���������Ļ��ļ���
	* @return ���ļ��� RingCollection2dSharedPtr
	*/
	virtual RingCollection2dSharedPtr& getRingCollection() = 0;

	/** ��ȡ����
	* @return ���� Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** ��ȡ�Ƿ������ɫ
	* @return �Ƿ������ɫ
	*/
	virtual bool   isFilledColor() = 0;

	/** �����Ƿ������ɫ
	* @param [in] �Ƿ������ɫ bFill
	* @return
	*/
	virtual void   setFilledColorStatus(bool bFill) = 0;

	/** ��ȡ�Ƿ�������
	* @return �Ƿ�������bool
	*/
	virtual bool   isFilledPattern() = 0;

	/** �����Ƿ�������
	* @param [in] �Ƿ�������bFill
	* @return
	*/
	virtual void   setFilledPatternStatus(bool bFill) = 0;

	/** ��ȡ���
	* @return double
	*/	
	virtual double getArea() = 0;

	/** ��ȡ�ܳ�
	* @return double
	*/	
	virtual double getPerimeter() = 0;

	/** ��ȡ��ͼ������
	* @return ��ͼ������
	*/
	virtual double getSymbolScale() = 0;

	/** ������ͼ������
	* @param [in] ��ͼ������ d
	* @return 
	*/
	virtual void setSymbolScale(double d) = 0;

	/** ��ȡ��ͼ���Ƕ�(�Ƕ�)
	* @return ��ͼ���Ƕ�(�Ƕ�)
	*/
	virtual double getSymbolAngleDeg() = 0;

	/** ������ͼ���Ƕ�(�Ƕ�)
	* @param [in] ��ͼ���Ƕ�(�Ƕ�) d
	* @return 
	*/
	virtual void setSymbolAngleDeg(double d) = 0;

	/** ��ȡ��������
	* @return ��������string
	*/
	virtual string getSymbolName() = 0;

	/** ���û�������
	* @param [in] �������� name
	* @return 
	*/
	virtual void setSymbolName(string name) = 0;

	/** �����������Ƿ���ʾ��״̬ 
	* @param [in] �Ƿ���ʾ��״̬ b
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** ��ȡ�������Ƿ���ʾ��״̬ 
	* @return �Ƿ���ʾ��״̬bool
	*/	
	virtual bool isShowPointStatus() = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomPolyline2d-�߼��ζ�����
*  @{
*/
class CORE_API GeomPolyline2d : virtual public Geometry2d
{
public:
	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** ��ȡ���������·���ļ���
	* @return ·���ļ���PathCollection2dSharedPtr
	*/
	virtual PathCollection2dSharedPtr& getPathCollection() = 0;

	/** ��ȡ����
	* @return ���� Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** ��ȡ����
	* @return ����double
	*/	
	virtual double getLength() = 0;

	/** �����������Ƿ���ʾ��״̬ 
	* @param [in] �Ƿ���ʾ�� bool
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** ��ȡ�������Ƿ���ʾ��״̬ 
	* @return bool�Ƿ���ʾ��
	*/	
	virtual bool isShowPointStatus() = 0;

	/** �����Ƿ���ʾ�������� 
	* @param [in] bIsDispDirection�Ƿ���ʾ��������
	* @return 
	*/	
	virtual void setDispDirection(bool bIsDispDirection) = 0;

	/** �ж��Ƿ���ʾ�������� 
	* @return bool�Ƿ���ʾ��������
	*/	
	virtual bool isDispDirection() = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomLine2d-�߼��ζ�����
*  @{
*/
class CORE_API GeomLine2d : virtual public Geometry2d
{
public:
	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2d & ptList ) = 0;

	/** ���ݵ���ptList���ѵ�ǰ�����������¹��죬����һ����յĻ�
	* @return ptList ����
	*/
	virtual void resetFromPointList( VertexCollection2dSharedPtr ptList ) = 0;

	/** ��ȡ��������ĵ���ļ���
	* @return ����ļ���VertexCollection2dSharedPtr
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;

	/** ��ȡ����
	* @return ���� Pen2dSharedPtr
	*/
	virtual Pen2dSharedPtr& getPen() = 0;

	/** ��ȡ����
	* @return ����double
	*/	
	virtual double getLength() = 0;

	/** �����������Ƿ���ʾ��״̬ 
	* @param [in] �Ƿ���ʾ�� bool
	*/	
	virtual void setShowPointStatus(bool b) = 0;

	/** ��ȡ�������Ƿ���ʾ��״̬ 
	* @return bool�Ƿ���ʾ��
	*/	
	virtual bool isShowPointStatus() = 0;

	/** �����Ƿ���ʾ�������� 
	* @param [in] bIsDispDirection�Ƿ���ʾ��������
	* @return 
	*/	
	virtual void setDispDirection(bool bIsDispDirection) = 0;

	/** �ж��Ƿ���ʾ�������� 
	* @return bool�Ƿ���ʾ��������
	*/	
	virtual bool isDispDirection() = 0;

	/** �ж������Ƿ���
	* @return bool �����Ƿ���
	*/	
	virtual bool isClosed() = 0;

	/** ���������Ƿ���
	* @param [in] b �����Ƿ���
	* @return 
	*/	
	virtual void setClosed(bool b) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeomPoint2d-������ࣨ��Բ�������Ρ���������ʾ�ĵ㣬�д�С��
*  @{
*/

class CORE_API GeomPoint2d : virtual public Geometry2d
{
public:
	/** �����͵Ļ�ȡ��Բ�������Σ������Σ�
	* @return �����ͣ�Բ�������Σ������Σ�
	*/	
	virtual int getPointType() = 0;

	/** �����͵����ã�Բ�������Σ������Σ�
	* @param [in] nType: �����ͣ�Բ�������Σ������Σ�
	* @return 
	*/	
	virtual void setPointType(int nType) = 0;

	/** ����λ�õĻ�ȡ
	* @return ����λ��Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** ����λ�õĻ�ȡ
	* @param [out] ����λ��Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** ����λ�õĻ�ȡ
	* @param [out] ����λ��x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** ����λ�õ�����
	* @param [in] ����λ��Vertex2d
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** ����λ�õ�����
	* @param [in] ����λ��x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** ��Ĵ�С�����׵�λ���Ļ�ȡ
	* @return ��Ĵ�С�����׵�λ��
	*/
	virtual double getSize() = 0;

	/** ��Ĵ�С�����׵�λ��������
	* @param [in] ��Ĵ�С�����׵�λ��
	* @return 
	*/
	virtual void setSize(double s) = 0;

};
/** @}*/

/** @addtogroup Geometry2d GeomPointSymbol2d-�������
*  @{
*/

class CORE_API GeomPointSymbol2d : virtual public Geometry2d
{
public:
	/** �������ƵĻ�ȡ
	* @return ��������string
	*/	
	virtual std::string getSymbolName() = 0;

	/** �������Ƶ�����
	* @param [in] ��������string
	* @return 
	*/	
	virtual void setSymbolName(std::string strName) = 0;

	/** ����λ�õĻ�ȡ
	* @return ����λ��Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** ����λ�õĻ�ȡ
	* @param [in] ����λ��Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** ����λ�õĻ�ȡ
	* @param [in] ����λ�� x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** ����λ�õ�����
	* @param [in] ����λ�� pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** ����λ�õ�����
	* @param [in] ����λ�� x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** ������ʾ�����Ļ�ȡ
	* @return ������ʾ����
	*/
	virtual double getScale() = 0;

	/** ������ʾ����������
	* @param [in] ������ʾ���� s
	* @return 
	*/
	virtual void setScale(double s) = 0;

	/** ������ת�Ƕȣ��Ƕȵ�λ���Ļ�ȡ
	* @return ������ת�Ƕȣ��Ƕȵ�λ��
	*/
	virtual double getRotateAngleDeg() = 0;

	/** ������ת�Ƕȣ��Ƕȵ�λ��������
	* @param [in] ������ת�Ƕȣ��Ƕȵ�λ��
	* @return 
	*/
	virtual void setRotateAngleDeg(double ang) = 0;
};
/** @}*/



/** @addtogroup Geometry2d GeomMultipoint2d-�����
*  @{
*/

class CORE_API GeomMultipoint2d : virtual public Geometry2d
{
public:
	/** ��ȡ������ĵ㼯
	* @return ��ȡ���ĵ㼯, VertexCollection2dSharedPtr
	*/
	virtual VertexCollection2dSharedPtr& getPointCollection() = 0;

	/** ������ķ������ƵĻ�ȡ
	* @return ������ķ�������
	*/	
	virtual std::string getSymbolName() = 0;

	/** ������ķ������Ƶ�����
	* @param [in] ������ķ�������
	* @return 
	*/	
	virtual void setSymbolName(std::string strName) = 0;

	/** ������ʾ�����Ļ�ȡ
	* @return ������ʾ����
	*/
	virtual double getScale() = 0;

	/** ������ʾ����������
	* @param [in] ������ʾ����
	* @return 
	*/
	virtual void setScale(double s) = 0;

	/** ������ת�Ƕȣ��Ƕȵ�λ���Ļ�ȡ
	* @return ������ת�Ƕȣ��Ƕȵ�λ��
	*/
	virtual double getRotateAngleDeg() = 0;

	/** ������ת�Ƕȣ��Ƕȵ�λ��������
	* @param [in] ������ת�Ƕȣ��Ƕȵ�λ��
	* @return 
	*/
	virtual void setRotateAngleDeg(double ang) = 0;
};
/** @}*/


/** @addtogroup Geometry2d Label2dStyle-��ע��񣬼����������ڱ�ע��λ��
*  @{
*/
enum Label2dStyle
{
	BottomLeft,			//����
	BottomMiddle,		//����
	BottomRight,		//����
	IntervalLeft,		//����
	IntervalMiddle,		//����
	IntervalRight,		//����
	TopLeft,			//����
	TopMiddle,			//����
	TopRight			//����
};
/** @}*/

/** @addtogroup Geometry2d GeomLabel2d-�ı���ע��
*  @{
*/

class CORE_API GeomLabel2d : virtual public Geometry2d
{
public:
	/** ��ȡ�ı�
	* @return �ı�
	*/
	virtual std::string getText() = 0;

	/** �����ı�
	* @param [in] �ı�
	* @return 
	*/
	virtual void setText(std::string str) = 0;

	/** ��ȡ�ı��Ĵ�С
	* @return �ı��Ĵ�С
	*/
	virtual double getSize() = 0;

	/** �����ı��Ĵ�С
	* @param [in] �ı��Ĵ�С
	* @return 
	*/
	virtual void setSize(double size) = 0;

	/** ��ȡ�ַ����
	* @return �ַ����
	*/
	virtual double getOffWide() = 0;

	/** �����ַ����
	* @param [in] �ַ���� d
	* @return 
	*/
	virtual void setOffWide(double d) = 0;

	/** ��ȡ�ַ��м��
	* @return �ַ��м��
	*/
	virtual double getRowOffWide() = 0;

	/** �����ַ��м��
	* @param [in] �ַ��м�� d
	* @return 
	*/
	virtual void setRowOffWide(double d) = 0;

	/** ��ȡ�ַ���ת�Ƕȣ������ַ����Ƕȵ�λ��
	* @return �ַ���ת�Ƕȣ������ַ����Ƕȵ�λ��
	*/
	virtual double getFontRotateAngleDeg() = 0;

	/** �����ַ���ת�Ƕȣ������ַ����Ƕȵ�λ��
	* @param [in] �����ַ���ת�Ƕȣ������ַ����Ƕȵ�λ��
	* @return 
	*/
	virtual void setFontRotateAngleDeg(double d) = 0;

	/** ��ȡ������ע����ת�Ƕȣ����壬�Ƕȵ�λ��
	* @return ��ע����ת�Ƕȣ����壬�Ƕȵ�λ��
	*/
	virtual double getLabelRotateAngleDeg() = 0;

	/** ����������ע����ת�Ƕȣ����壬�Ƕȵ�λ��
	* @param [in] ��ע����ת�Ƕȣ����壬�Ƕȵ�λ��
	* @return 
	*/
	virtual void setLabelRotateAngleDeg(double d) = 0;

	/** ��ȡ�ı�������
	* @return �ı�������Font2d
	*/
	virtual Font2d getFont() = 0;

	/** �����ı�������
	* @param [in] �ı�������Font2d
	* @return 
	*/
	virtual void setFont(Font2d & font) = 0;

	/** ��ȡ��ע�ķ��
	* @return ��ע�ķ��Label2dStyle
	*/
	virtual Label2dStyle getStyle() = 0;

	/** ���ñ�ע�ķ��
	* @param [in] ��ע�ķ��Label2dStyle
	* @return 
	*/
	virtual void setStyle(Label2dStyle sty) = 0;
	
	/** ����λ�õĻ�ȡ
	* @return ����λ��Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;
	
	/** ����λ�õĻ�ȡ
	* @param [out] ����λ��Vertex2d
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;
	
	/** ����λ�õĻ�ȡ
	* @param [out] ����λ�� x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;
	
	/** ����λ�õ�����
	* @param [in] ����λ��pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;
	
	/** ����λ�õ�����
	* @param [in] ����λ�� x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;
};
/** @}*/

/** @addtogroup Geometry2d GeomAnnotation2d-ע�Ǽ��ζ����࣬���������+ע��
*  @{
*/

class CORE_API GeomAnnotation2d : virtual public Geometry2d
{
public:
	/** ע���ı��Ļ�ȡ
	* @return ע���ı�
	*/	
	virtual std::string getAnnotation() = 0;

	/** ע���ı�������
	* @param [in] ע���ı�strName
	* @return 
	*/	
	virtual void setAnnotation(std::string strName) = 0;

	/** ע��λ�õĻ�ȡ
	* @return ע��λ��Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** ע��λ�õĻ�ȡ
	* @param [out] ע��λ��pt
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** ע��λ�õĻ�ȡ
	* @param [out] ע��λ�� x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** ע��λ�õ�����
	* @param [in] ע��λ��pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** ע��λ�õ�����
	* @param [in] ע��λ��x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** ע���ı���С�Ļ�ȡ�����׵�λ��
	* @return ע���ı���С�����׵�λ��
	*/
	virtual double getSize() = 0;

	/** ע���ı���С�����ã����׵�λ��
	* @param [in] ע���ı���С�����׵�λ��
	* @return 
	*/
	virtual void setSize(double s) = 0;

	/** ��ȡ�ı�������
	* @return �ı�������
	*/
	virtual Font2d getFont() = 0;

	/** �����ı�������
	* @param [in] �ı������� font
	* @return 
	*/
	virtual void setFont(Font2d & font) = 0;
};
/** @}*/


/** @addtogroup Geometry2d GeomGeometryBag2d-���ζ����
*  @{
*/

class CORE_API GeomGeometryBag2d : virtual public Geometry2d
{
public:
	/** ��ȡ���ζ����б�
	* @return GeometryCollection2dSharedPtr
	*/
	virtual GeometryCollection2dSharedPtr& getGeometryCollection() = 0;
};
/** @}*/


/** @addtogroup Geometry2d Image2dType-ͼ������
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


/** @addtogroup Geometry2d GeomImage2d-ͼ��
*  @{
*/

class CORE_API GeomImage2d : virtual public Geometry2d
{
public:	
	/** ͼ�����͵Ļ�ȡ
	* @return ͼ������
	*/	
	virtual int getImageType() = 0;
	
	/** ͼ�����͵�����
	* @param [in] ͼ������
	* @return 
	*/	
	virtual void setImageType(int imgType) = 0;
	
	/** ͼ�����ƵĻ�ȡ
	* @return ͼ������
	*/	
	virtual std::string getName() = 0;
	
	/** ͼ�����Ƶ�����
	* @param [in] ͼ������
	* @return 
	*/	
	virtual void setName(std::string strName) = 0;

	/** ͼ��λ�õĻ�ȡ
	* @return ͼ��λ��Vertex2d
	*/
	virtual const Vertex2d& queryCoord() = 0;

	/** ͼ��λ�õĻ�ȡ
	* @param [out] ͼ��λ��pt
	* @return 
	*/
	virtual void queryCoord(Vertex2d& pt) = 0;

	/** ͼ��λ�õĻ�ȡ
	* @param [out] ͼ��λ�� x,y
	* @return 
	*/
	virtual void queryCoord(double& x,double& y) = 0;

	/** ͼ��λ�õ�����
	* @param [in] ͼ��λ��pt
	* @return 
	*/
	virtual void putCoord(Vertex2d & pt) = 0;

	/** ͼ��λ�õ�����
	* @param [in] ͼ��λ��x,y
	* @return 
	*/
	virtual void putCoord(double x,double y) = 0;

	/** ������ʾ�����Ļ�ȡ
	* @return ������ʾ����
	*/
	virtual double getXScale() = 0;

	/** ������ʾ�����Ļ�ȡ
	* @return ������ʾ����
	*/
	virtual double getYScale() = 0;

	/** ������ʾ����������
	* @param [in] s ������ʾ����
	* @return 
	*/
	virtual void setXScale(double s) = 0;

	/** ������ʾ����������
	* @param [in] s ������ʾ����
	* @return 
	*/
	virtual void setYScale(double s) = 0;
};
/** @}*/


// @addtogroup Geometry2d GeometrySelectManager2d ��ά���ζ���ѡ���������ӿ�
//     *  @{
//    
class CORE_API GeometrySelectManager2d
{
public:
	/**�趨ѡ������������ϵͳ 
	* @param [in] pCoorSys: ����ϵͳ 
	* @return 
	*/
	virtual void setCoordsys(Coordsys2dSharedPtr& pCoorSys) = 0;

	/**�ж�ָ�����ζ���pObj �Ƿ���ָ���ı߽���η�Χ�� 
	* @param [in] point_left_bottom_in_WP:���������£��߽���ε����½ǵ�
	* @param [in] point_right_top_in_WP:  ���������£��߽���ε����Ͻǵ�
	* @return �Ƿ���ָ����Χ��
	*/
	virtual bool boundIntersect(GeometrySharedPtr& pObj,
		const Vertex2d &  point_left_bottom_in_WP,
		const Vertex2d &  point_right_top_in_WP) = 0;

	/**�ж�ָ�����ζ���pObj �Ƿ���ָ���ı߽���η�Χ�� 
	* @param [in] minx, maxx, miny, maxy�� ���������£�ѡ���Χ
	* @return �Ƿ���ָ����Χ��
	*/
	virtual bool boundIntersect(GeometrySharedPtr& pObj,
		double minx, double maxx, double miny, double maxy) = 0;
};
/** @}*/

/** @addtogroup Geometry2d Symbol ������
*  @{
*/

class CORE_API Symbol2d
{
public:
	/**��ȡ����ID
	* @return ����ID
	*/
	virtual FSID		getID()=0;

	/**���÷���ID
	* @param [in] ����ID
	* @return 
	*/
	virtual void		setID(FSID id)=0;

	/**��ȡ�������� 
	* @return ��������
	*/
	virtual std::string	getName()=0;

	/**���÷������� 
	* @param [in] �������� 
	* @return 
	*/
	virtual void		setName(std::string sz)=0;

	/**��ȡ��׼���
	* @return ��׼���
	*/
	virtual std::string	getStdNum()=0;

	/**���ñ�׼���
	* @param [in] ��׼���sz
	* @return 
	*/
	virtual void		setStdNum(std::string sz)=0;

	/**��ȡ�������� 
	* @return ��������
	*/
	virtual std::string	getDescription()=0;

	/**���÷������� 
	* @param [in] �������� 
	* @return 
	*/
	virtual void		setDescription(std::string sz)=0;

	/**��ȡ�Ƿ�Ϊ����ķ���
	* @return �Ƿ�Ϊ����ķ���
	*/
	virtual bool		isFreedom()=0;

	/**���� �Ƿ�Ϊ����ķ���
	* @param [in] bfreedom�Ƿ�Ϊ����ķ���
	* @return 
	*/
	virtual void		setFreedom(bool bfreedom)=0;

	/**��ȡ��� ����
	* @return ��� ����
	*/
	virtual double		getWidth()=0;

	/**���ÿ�� ����
	* @param [in] ��� ����
	* @return 
	*/
	virtual void		setWidth(double dw)=0;

	/**��ȡ�߶� ����
	* @return �߶� ����
	*/
	virtual double		getHeight()=0;

	/**��ȡ�����ø߶� ����
	* @param [in] �߶� ����
	* @return 
	*/
	virtual void		setHeight(double dw)=0;

	/**��ȡ������ɫ 
	* @return ������ɫ 
	*/
	virtual Color4f		getBackgroundColor()=0;

	/**���ñ�����ɫ 
	* @param [in] cr������ɫ
	* @return 
	*/
	virtual void		setBackgroundColor(Color4f& cr)=0;

	/**��ȡ�Ƿ���䱳��ɫ 
	* @return �Ƿ���䱳��ɫ
	*/
	virtual bool		isFilledBackground()=0;

	/**�����Ƿ���䱳��ɫ 
	* @param [in] �Ƿ���䱳��ɫ
	* @return 
	*/
	virtual void		setFilledBackground(bool bFilled)=0;
	
	/** ��ȡ���Ų��������,���Ե������Ч
	* @return ���Ų�������� Vertex2d
	*/
	virtual Vertex2d getInserPosition()=0;

	/** ��ȡ���Ų����,���Ե������Ч
	* @param [out] ���Ų��������
	* @return
	*/
	virtual void getInserPosition(double& x,double& y)=0;
	
	/** ���÷��Ų��������,���Ե������Ч
	* @param [in] ���Ų��������vpos
	* @return
	*/
	virtual void setInserPosition(Vertex2d& vpos)=0;

	/** ���÷��Ų��������,���Ե������Ч
	* @param [in] ���Ų�������� x,y
	* @return
	*/
	virtual void setInserPosition(double x,double y)=0;

	/** ��ȡ��ɷ��ŵ�Ԫ�ؼ���
	* @return ��ɷ��ŵ�Ԫ�ؼ���GeometryCollection2dSharedPtr
	*/
	virtual GeometryCollection2dSharedPtr& getGeometryCollection()=0;
	
	/** ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
     * @param  [out] �߽����Envelope3d
     * @return 
    */
    virtual void getEnvelope(Envelope3d & envp)=0; 

public:	
	/** ��¡����
	 * @param  [in]  
	 * @return �Ƿ񿽱��ɹ�
	 */
	virtual Symbol2dSharedPtr clone()=0;

	/** ��ȡ���ζ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
     * @param  [in] �� 
     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	 */
    virtual size_t sizeBuffer()=0;
	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf)=0;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf)=0;
	/* ���������Ϣд��������У������ļ���д�򻺳���д���� 
     * @param  [in,out] f  std::ostream &����׼�������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual void write(std::ostream & f)=0;
	/** �ӱ�׼����������ȡ�������Ϣ  
     * @param  [in,out] f  std::istream &����׼��������������
     * @return �ɹ�����true�����򷵻�false
    */
	virtual void read(std::istream & f)=0;

public:
	/**ͳ�����м��ζ���Ļ�����ɫ 
	* @param [in] ���ζ���Ļ�����ɫpenColor
	* @return 
	*/
	virtual void batchChangeGeometrysPenColor(Color4f &penColor)=0;

	/**ͳ�����м��ζ���������ɫ 
	* @param [in] ���ζ���������ɫfillColor
	* @return 
	*/
	virtual void batchChangeGeometrysBrushColor(Color4f &fillColor)=0;

	/**�޸ķ��ŵĴ�С
	* @param [in] ���,�߶�,��λ:����, bIsKeepHWScale:�Ƿ񱣳��ݺ����
	* @return 
	*/
	virtual void changeSize(double dWidth ,double dHeight,bool bIsKeepHWScale)=0;

	/**�����ų�����ָ���Ŀ��,�߶�,��λ:����
	* @param [in] dWidth���,dHeight�߶�,��λ:����, bIsKeepHWScale:�Ƿ񱣳��ݺ���� 
	* @return 
	*/
	virtual void expandToSize(double dWidth ,double dHeight,bool bIsKeepHWScale)=0;

};
/** @}*/

/** @addtogroup Geometry2d SymbolLibrary2d ��������
*  @{
*/

class CORE_API SymbolLibrary2d
{
public:
	/**��ȡ�������ļ��� 
	* @return �������ļ���
	*/
	virtual std::string	getFilePath() = 0;

	/**��ȡ ���������� 
	* @return ����������
	*/
	virtual std::string	getName() = 0;

	/**���� ���������� 
	* @param [in] sz ����������
	* @return 
	*/
	virtual void		setName(std::string sz) = 0;

	/**��ȡ ������˵�� 
	* @return ������˵��
	*/
	virtual std::string	getDescription() = 0;

	/**���� ������˵�� 
	* @param [in] sz ������˵��
	* @return 
	*/
	virtual void		setDescription(std::string sz) = 0;

	/**��ȡ �������׼��� 
	* @return �������׼���
	*/
	virtual std::string	getStdNum() = 0;

	/**���� �������׼��� 
	* @param [in] sz �������׼���
	* @return 
	*/
	virtual void		setStdNum(std::string sz) = 0;

	/**��ǰ����Ű��շ���ID���� 
	* @return 
	*/
	virtual void sortSymbolByID() = 0;

	/**��ǰ����Ű��ձ�׼������� 
	* @return 
	*/
	virtual void sortSymbolByStdNum() = 0;

	/**��ǰ����Ű��շ����������� 
	* @return 
	*/
	virtual void sortSymbolByName() = 0;

	/**�Ե�ǰ�������е�ָ��λ�õķ��Ž���λ�� 
	* @param [in]  index1,  index2��ָ�����ŵ�����
	* @return 
	*/
	virtual void swap(long index1, long index2) = 0;

public:
	/** ��ȡ���ζ���д�뵽Buffer����ռ�õ��ֽ��������ڴ�Buffer�����й������ζ���
     * @param  [in] �� 
     * @return ���ؼ��ζ���д�뵽Buffer����ռ�õ��ֽ���
	 */
    virtual size_t sizeBuffer() = 0;

	/** ��Buffer�ж�ȡ��Ϣ��䱾���ζ���
     * @param  [in,out] buf Buffer & ���������������� 
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool readBuffer( Buffer & buf) = 0;
	/** ���������Ϣд��Buffer�� 
     * @param  [in,out] buf Buffer & ����������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual bool writeBuffer(Buffer& buf) = 0;
	/* ���������Ϣд��������У������ļ���д�򻺳���д���� 
     * @param  [in,out] f  std::ostream &����׼�������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual void write(std::ostream & f) = 0;
	/** �ӱ�׼����������ȡ�������Ϣ  
     * @param  [in,out] f  std::istream &����׼��������������
     * @return �ɹ�����true�����򷵻�false
    */
	virtual void read(std::istream & f) = 0;

	/**����Ϊ�������ļ� 
	* @param [in] szPath �ļ�·����
	* @return �Ƿ񱣴�ɹ�
	*/
	virtual bool save(std::string szPath) = 0;

	/**�򿪷������ļ� 
	* @param [in] szPath �ļ�·����
	* @return �Ƿ�򿪳ɹ�
	*/
	virtual bool load(std::string szPath) = 0;

	/**��ȡ��ǰ�������ķ���ID 
	* @return ��ǰ�������ķ���ID
	*/
	virtual long getMaxSymbolID() = 0;

public:
	/**����ID,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr��
	* @param [in] id������ID
	* @param [out] outPtr�����ҵ��ķ���
	* @return 
	*/
	virtual bool findSymbolById(FSID id, Symbol2dSharedPtr& outPtr) = 0;

	/**��������,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr�� 
	* @param [in] szName����������
	* @param [out] outPtr�����ҵ��ķ���
	* @return 
	*/
	virtual bool findSymbolByName(std::string szName, Symbol2dSharedPtr& outPtr) = 0;

	/**���ݱ�׼���,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr�� 
	* @param [in] szStdNum�����ű�׼���
	* @param [out] outPtr�����ҵ��ķ���
	* @return 
	*/
	virtual bool findSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtr& outPtr) = 0;

	/**��������,����ָ���ķ��ţ�ģ�����ң������ַ������������� , ������ڣ���ӵ����ڲ���outList�� 
	* @param [in] szName����������
	* @param [out] outPtr�����ҵ��ķ����б�
	* @return ���ҵ��ķ��Ÿ���
	*/
	virtual long fuzzyFindSymbolByName(std::string szName, Symbol2dSharedPtrList& outList) = 0;

	/**���ݱ�׼���,����ָ���ķ��� , ��ģ�����ң������ַ������������� , ������ڣ���ӵ����ڲ���outList�� 
	* @param [in] szStdNum�����ű�׼���
	* @param [out] outPtr�����ҵ��ķ����б�
	* @return ���ҵ��ķ��Ÿ���
	*/
	virtual long fuzzyFindSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtrList& outList) = 0;
	
	/** ��ȡ�����б�  
     * @param  [in,out] 
     * @return Symbol2dSharedPtrList �����б�
    */
	virtual Symbol2dSharedPtrList& getSymbolList() = 0;
};
/** @}*/


/** @addtogroup Geometry2d SymbolManager2d ��������
*  @{
*/
class CORE_API SymbolManager2d
{
public:
	/** ��ȡ����Ź�����
	* @return ���Ź�����ָ��
	*/
	static SymbolManager2dSharedPtr& getPointSymbolManager();

	/** ��ȡ�߷��Ź�����
	* @return ���Ź�����ָ��
	*/
	static SymbolManager2dSharedPtr& getLineSymbolManager();

	/** ��ȡ����Ź�����
	* @return ���Ź�����ָ��
	*/
	static SymbolManager2dSharedPtr& getRegionSymbolManager();

	/**���������ļ������ΰ�˳���ȡ�����ļ���ָ���ķ��ſ��ļ� 
	* @return 
	*/
	virtual void reloadSymbolFiles() = 0;

	/**��ȡĬ�ϵķ��� 
	* @return Ĭ�ϵķ���
	*/
	virtual Symbol2dSharedPtr& getDefaultSymbol() = 0;

	/**��ȡ�������б� 
	* @return �������б�
	*/
	virtual SymbolLibrary2dSharedPtrList& getGroupList() = 0;

	/**�Ե�ǰ�������е�ָ��λ�õķ��Ž���λ�� 
	* @param [in] index1, index2��ָ����������
	* @return 
	*/
	virtual void swap(long index1, long index2) = 0;

	/**����ָ���ķ����飬����ж���������������ص�һ�� 
	* @param [in] szGroupName������������
	* @param [out] outPtr�ҵ��ķ�����
	* @return 
	*/
	virtual bool findGroup(std::string szGroupName, SymbolLibrary2dSharedPtr& outPtr) = 0;

	/**����Ĭ�ϵĵ���� 
	* @return Ĭ�ϵĵ����
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolPoint() = 0;

	/**����Ĭ�ϵ��߷��� 
	* @return Ĭ�ϵ��߷���
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolLine() = 0;

	/**����Ĭ�ϵ������� 
	* @return Ĭ�ϵ������� 
	*/
	virtual Symbol2dSharedPtr createDefaultSymbolRegion() = 0;

public:	
	/**����ID,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr��
	*  �����ǰ���ſ��У��ж�����ŷ��ϲ���������ֻ���ز��ҵ��ĵ�һ�� 
	* @param [in] id:����ID��returnDefaultSymbol:���Ϊtrue�����Ҳ����Ļ�������Ĭ�Ϸ���
	* @param [out] outPtr:�ҵ��ķ���
	* @return �Ƿ���ҵ�
	*/
	virtual bool findSymbolById(FSID id, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**��������,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr��
	*  �����ǰ���ſ��У��ж�����ŷ��ϲ���������ֻ���ز��ҵ��ĵ�һ��  
	* @param [in] szName:�������ƣ�returnDefaultSymbol:���Ϊtrue�����Ҳ����Ļ�������Ĭ�Ϸ���
	* @param [out] outPtr:�ҵ��ķ���
	* @return �Ƿ���ҵ�
	*/
	virtual bool findSymbolByName(std::string szName, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**���ݱ�׼���,����ָ���ķ��� , ������ڣ����ص����ڲ���outPtr��
	*  �����ǰ���ſ��У��ж�����ŷ��ϲ���������ֻ���ز��ҵ��ĵ�һ��  
	* @param [in] szStdNum:���ű�׼��ţ�returnDefaultSymbol:���Ϊtrue�����Ҳ����Ļ�������Ĭ�Ϸ���
	* @param [out] outPtr:�ҵ��ķ���
	* @return �Ƿ���ҵ�
	*/
	virtual bool findSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtr& outPtr, bool returnDefaultSymbol) = 0;

	/**��������,����ָ���ķ��ţ�ģ�����ң������ַ�������������
	*  ������ڣ���ӵ����ڲ���outList�У� ���ز��ҵ��ķ��Ÿ���  
	* @param [in] szName:��������
	* @param [out] outList:�ҵ��ķ����б�
	* @return ���ҵ��ķ��Ÿ���
	*/
	virtual long fuzzyFindSymbolByName(std::string szName, Symbol2dSharedPtrList& outList) = 0;

	/**���ݱ�׼���,����ָ���ķ��� , ��ģ�����ң������ַ�������������
	*  ������ڣ���ӵ����ڲ���outList�� �� ���ز��ҵ��ķ��Ÿ��� 
	* @param [in] szStdNum:���ű�׼���
	* @param [out] outPtr:�ҵ��ķ����б�
	* @return �ҵ��ķ��Ÿ���
	*/
	virtual long fuzzyFindSymbolByStdNum(std::string szStdNum, Symbol2dSharedPtrList& outList) = 0;
};
/** @}*/

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
