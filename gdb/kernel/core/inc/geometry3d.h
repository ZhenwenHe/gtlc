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

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 

/** @addtogroup Geometry Point3d-��ά����
*  @{
*/
class CORE_API Point3d : virtual public Geometry
{
public:
	/**  ��ȡ��λ������
	* @param  ��
	* @return λ������
	*/
	virtual Vertex3d getPosition()=0;

	/**  ���õ�λ������
	* @param  [in] vPosition  ��λ������
	* @return ��
	*/
	virtual void setPosition(const Vertex3d & vPosition)=0;
};
/** @} */ 

/** @addtogroup Geometry SinglePoint3d-��ά������ 
*  @{
*/
class CORE_API SinglePoint3d : virtual public Point3d
{
public:	
	/**  ��ȡ�����ɫ
	* @param  ��
	* @return �����ɫ
	*/
	virtual Color4f getColor()=0;

	/**  ���õ����ɫ
	* @param  [in] c4f  �����ɫ
	* @return ��
	*/
	virtual void setColor(const Color4f &c4f)=0;

	/**  ��ȡ��ķ���
	* @param  ��
	* @return ��ķ���
	*/
	virtual Vertex3d getNormal()=0;

	/**  ���õ�ķ���
	* @param  [in] v  ��ķ�������
	* @return ��
	*/
	virtual void setNormal(const Vertex3d & v)=0;

	/**  ��ȡ��Ĵ�С
	* @param  ��
	* @return ��Ĵ�С
	*/
	virtual double getPointSize()=0;

	/**  ���õ�Ĵ�С
	* @param  [in] s  ��Ĵ�С
	* @return ��
	*/
	virtual void setPointSize(double s)=0;
};
/** @} */

/** @addtogroup Geometry MultiPoint3d-��ά����� 
*  @{
*/
class CORE_API MultiPoint3d : virtual public Point3d
{
public:
	/**  ��ȡ��λ��������
	* @param  ��
	* @return ��λ��������
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor()=0;

	/** ��ö������ͱ���	
	* @param ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const=0;
	/** ���ö������ͱ���
	* @param [in] iVertexType	 �������ͱ���
	* @return ��
	*/ 
	virtual void setVertexType(unsigned char iVertexType)=0;
	/** �����ά��㶥�����	
	* @param  ��
	* @return ��ά��㶥�����
	*/
	virtual long getVerticesNum() const=0;
	/** ������ά��㶥������Ͷ�������
	* @param [in] nVertexNum	��ά��㶥�����	
	* @param [in] vVertices	ָ��һ����ά������ָ��	
	* @return ��
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)=0;
	/** �����ά��㶥������	
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return ��
	*/
	virtual void getVertices( long& lVertNum, void*& vVertices ) const=0;
	
	/**  ��ȡ�������
	* @param  [out] size  ���Ե��ֽڳ���
	* @param  [out] att �������
	* @return ��
	*/
	virtual void getAttributes(long & size, void ** att)=0;
	/**  ���õ������
	* @param  [in] size  ���Ե��ֽڳ���
	* @param  [in] pAttributes �������
	* @return ��
	*/
	virtual void setAttributes(const long  size, void* const pAttributes)=0;
};
/** @} */

/** @addtogroup Geometry Annotation3d-��άע����
*  @{
*/
class CORE_API Annotation3d : virtual public Point3d
{
public:	
	/** ���ƽ������ֵ
	* @param	��
	* @return	ƽ������ֵ
    */
	virtual Vertex3d getPosition() const = 0;
	/** �趨ƽ������ֵ
	* @param [in] pos	��ά��	
	* @return ��
    */
	virtual void setPosition(Vertex3d pos)= 0;

	/**  ��ȡ��λ����
	* @param  ��
	* @return ��λ����
	*/
	virtual Quaternion getOrientation()=0;
	/**  ���÷�λ����
	* @param  [in] qOrientation  ��λ����
	* @return ��
	*/
	virtual void setOrientation(const Quaternion & qOrientation)=0;
	
	/**  ��ȡע������
	* @param  ��
	* @return ע������
	*/
	virtual long getAnnotationType() =0;
	/**  ����ע������
	* @param  [in] lAnnotationType  ע������
	* @return ��
	*/
	virtual void setAnnotationType(long lAnnotationType)=0;

	/**  ��ȡע�Ƿ��
	* @param  ��
	* @return ע�Ƿ��
	*/
	virtual unsigned char getAnnotationStyle() =0;
	/**  ����ע�Ƿ��
	* @param  [in] cAnnotationStyle  ע�Ƿ��
	* @return ��
	*/
	virtual void setAnnotationStyle(unsigned char cAnnotationStyle)=0;
	
	/**  ��ȡע���ִ�
	* @param  ��
	* @return ע���ִ�
	*/
	virtual std::string getAnnotationString()=0;
	/**  ����ע���ִ�
	* @param  [in] szAnnotationString  ע���ִ�
	* @return ��
	*/
	virtual void setAnnotationString(const std::string &  szAnnotationString)=0;
	
	/**  ��ȡ��������
	* @param  ��
	* @return ��������
	*/
	virtual std::string getFontName()=0;
	/**  ������������
	* @param  [in] szFontName  ��������
	* @return ��
	*/
	virtual void setFontName(const std::string & szFontName)=0;

	/** ע���ָߺ��ֿ���16Ϊ�ֿ���16λΪ�ָ�;0.01mmΪ��λ */
	/**  ��ȡע���ָ�
	* @param  ��
	* @return ע���ָ�
	*/
	virtual long getHeight()=0;
	/**  ����ע���ָ�
	* @param  [in] lHeight  ע���ָ�
	* @return ��
	*/
	virtual void setHeight(long lHeight)=0;

	/** ע���ִ����ֿ��Ե���ע����Ч���Բ������Ч;0.01mmΪ��λ */
	/**  ��ȡע���ִ��Ŀ��
	* @param  ��
	* @return ע���ִ��Ŀ��
	*/
	virtual long getWidth()=0;
	/**  ����ע���ִ��Ŀ��
	* @param  [in] lWidth  ע���ִ��Ŀ��
	* @return ��
	*/
	virtual void setWidth(long lWidth)=0;

	/** ע���ּ��;0.01mmΪ��λ */
	/**  ��ȡע���ּ��
	* @param  ��
	* @return ע���ּ��
	*/
	virtual long getSpaces()=0;
	/**  ����ע���ּ��
	* @param  [in] lSpaces  ע���ּ��
	* @return ��
	*/
	virtual void setSpaces(long lSpaces)=0;

	/** ע����ɫ */
	/**  ��ȡע����ɫ
	* @param  ��
	* @return ע����ɫ
	*/
	virtual Color4f getColor()=0;
	/**  ����ע����ɫ
	* @param  [in] clAnnotation  ע����ɫ
	* @return ��
	*/
	virtual void setColor(const Color4f & clAnnotation)=0;
};
/** @} */

/** @addtogroup Geometry Reference3d-��ά������ 
*  @{
*/
class CORE_API Reference3d : virtual public Point3d
{
public:
	/**  ��ȡ����ģ��
	* @param  ��
	* @return ����ģ��
	*/
	virtual SharedModelSharedPtr  getSharedModel()=0;
	/**  ���ù���ģ��
	* @param  [in] spSharedModel  ����ģ��Ĺ���ָ��
	* @return ��
	*/
	virtual void  setSharedModel(SharedModelSharedPtr spSharedModel)=0;
	/**  ��ȡ����ģ��ID
	* @param  ��
	* @return ����ģ��ID
	*/
	virtual MDLID  getSharedModelID()=0;

	/** ������ű���	
	* @param  ��
	* @return ���ű���
	*/
	virtual double getScale() const=0;

	/** �������ű���	
	* @param [in] dScale ���ű���
	* @return ��
	*/
	virtual void setScale(double dScale)=0;

	/** ��ù�����������	
	* @param  ��
	* @return ������������
	*/
	virtual std::string getShareModelName() const=0;

	/** ���ù�����������
	* @param [in] strLinkName ������������
	* @return ��
	*/
	virtual void setShareModelName(std::string strLinkName)=0;
};
/** @} */

/** @addtogroup Geometry Curve3d-��ά���� 
*  @{
*/
class CORE_API Curve3d : virtual public Geometry
{
public:
	/**  ��ȡ����λ�����������
	* @param  ��
	* @return ����λ�����������ָ��
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor()=0;
	/** ��ö������ͱ���	
	* @param ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const=0;
	/** ���ö������ͱ���
	* @param [in] iVertexType	 �������ͱ���
	* @return ��
	*/ 
	virtual void setVertexType(unsigned char iVertexType)=0;
	/** �����ά�������	
	* @param  ��
	* @return ��ά�������
	*/
	virtual long getVerticesNum() const=0;
	/** ������ά��������Ͷ�������
	* @param [in] nVertexNum	��ά�������	
	* @param [in] vVertices	ָ��һ����ά������ָ��	
	* @return ��
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)=0;
	/** �����ά��㶥������	
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return ��
	*/
	virtual void getVertices( long& lVertNum, void*& vVertices ) const=0;

	/** ��ȡ�ߵĳ���
	* @param  ��
	* @return �߳�
    */
	virtual double getLength()=0;
};
/** @} */

/** @addtogroup Geometry LinearRing3d-��ά�߻��� 
*  @{
*/
class CORE_API LinearRing3d : virtual public Curve3d
{
public:
	/**  ��ȡ�߻���־
	* @param  ��
	* @return 1 -- �ڻ���־��0 -- �⻷��־
	*/
	virtual int getRingFlag()=0;
	/**  �����߻���־
	* @param  [in] nFlag �߻���־ (1 -- �ڻ���־��0 -- �⻷��־)
	* @return ��
	*/
	virtual void setRingFlag(int nFlag)=0;
};
/** @} */

/** @addtogroup Geometry LineString3d-��ά������ 
*  @{
*/
class CORE_API LineString3d : virtual public Curve3d
{
public:

	/**  ��ȡ���߿��
	* @param  [in,out] lineWidth  ���߿��
	* @return ��
	*/
	virtual void getLineWidth(double& lineWidth) =0;
	/**  �������߿��
	* @param  [in] lineWidth  ���߿��
	* @return ��
	*/
	virtual void setLineWidth(double& lineWidth) =0;
};
/** @} */

/** @addtogroup Geometry MultiPolyline3d-��ά���������� 
*  @{
*/
class CORE_API MultiPolyline3d : virtual public Curve3d
{
public:
	/** ��ȡ�ߵĿ��
	* @param  [in] lineWidth  ���߿��
	* @return ��
    */
	virtual void setLineWidth(double& lineWidth)=0;
	/** ��ȡ�ߵĿ��
	* @param  ��
	* @return �ߵĿ��
    */
	virtual double getLineWidth()=0;

	/**  ��ȡ������ߵĸ���
	* @param  ��
	* @return ������ߵĸ���
	*/
	virtual long getPolylineNumber()=0;

	/**  ��ȡ���߽ṹ�����
	* @param  [in] ib  ���߽ṹ�����
	* @return ��
	*/
	virtual void getPolylineIndexBuffer(IndexBuffer & ib)=0;

	/**  ��ȡ������߸��ԵĽ����Ŀ��
	* @param  [in] ib  ������߸��ԵĽ����Ŀ��
	* @return ��
	*/
	virtual void getVertexNumberPerPolyline(IndexBuffer & ib )=0;
};
/** @} */

/** @addtogroup Geometry ParameterizedCurve3d-��ά������������ 
*  @{
*/
class CORE_API ParameterizedCurve3d : virtual public Curve3d
{
public:
	/** ����������ͱ��	
	* @param  ��
	* @return �������ͱ��ֵ
    */
	virtual unsigned char getParameterizedType() const=0;

	/** ������������
	* @param  [in] iType	��֪��������ֵ	
	* @return ��
    */
	virtual void setParameterizedType(unsigned char iType)=0;

	/** ��ÿ��Ƶ�����	
	* @param  [out] vControlPoints ������ֵ�Ŀ��Ƶ����������
	* @return ��
    */
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const=0;

	/** ���ÿ��Ƶ�����
	* @param  [in] vControlPoints	��֪���Ƶ����������	
	* @return ��
    */
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)=0;
};
/** @} */

/** @addtogroup Geometry Surface3d-��ά���� 
*  @{
*/
class CORE_API Surface3d : virtual public Geometry
{
public:
	/**  ��ȡ����λ�����������
	* @param  ��
	* @return ����λ�����������ָ��
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor()=0;
	/** ��ö������ͱ���	
	* @param  ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const=0;
	/** ���ö������ͱ���
	* @param  [in] iVertexType	 �������ͱ���
	* @return ��
	*/ 
	virtual void setVertexType(unsigned char iVertexType)=0;
	/** �����ά�������	
	* @param  ��
	* @return ��ά�������
	*/
	virtual long getVerticesNum() const=0;
	/** ������ά��������Ͷ�������
	* @param  [in] nVertexNum	��ά�������	
	* @param  [in] vVertices	ָ��һ����ά������ָ��	
	* @return ��
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)=0;
	/** �����ά��㶥������	
	* @param  [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param  [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return ��
	*/
	virtual void getVertices( long& lVertNum, void*& vVertices ) const=0;

	/**  ��ȡ�����ָ��
	* @param  ��
	* @return �����ָ��
	*/
	virtual MaterialPtr  getMaterial()=0;

	/**  ���������
	* @param  [in] pMaterial �����ָ��
	* @return ��
	*/
	virtual void  setMaterial(MaterialPtr pMaterial)=0;
	
	/**  ��ȡ����ID
	* @param  ��
	* @return ����ID
	*/
	virtual Identifier::raw_type  getMaterialID()=0;

	/** �����
	* @param  ��
	* @return ���ֵ
    */
	virtual double getArea() const =0;
};
/** @} */

/** @addtogroup Geometry TriangleMesh3d-��ά������������ 
*  @{
*/
class CORE_API  TriangleMesh3d : virtual public Surface3d
{
public:
	/**  ��ȡ������������Žṹ
	* @param  [in,out] ib  ������������Žṹbuffer
	* @return ��
	*/
	virtual void getTriangleIndexBuffer(IndexBuffer & ib)=0;
	/**  ��ȡ����������������Žṹ
	* @param  [in,out] ib  ����������������Žṹbuffer
	* @return ��
	*/
	virtual void getTriangleStripIndexBuffer(IndexBuffer & ib)=0;

	/** ������������������������
	* @param [in] lTriangleList  ��������������
	* @param [in] lTriListSize ��������������
	* @return ��
    */
	virtual void setTriangles(long* const lTriangleList, long lTriListSize)=0;
};
/** @} */
/** @addtogroup Geometry Polygon3d-��ά������� 
*  @{
*/
class CORE_API Polygon3d : virtual public Surface3d
{
public:
	/** ��ȡ������л�����
	* @param  [in,out] rings ����εĻ�����
	* @return ��
	*/
	virtual void getRings(std::vector<LinearRing3d*> & rings)=0;
	/** ���ö�����л�����
	* @param  [in,out] rings ����εĻ�����
	* @return ��
	*/
	virtual void setRings(std::vector<LinearRing3d*> & rings)=0;
	/** ����������ӻ�
	* @param  [in] pRing ������ָ�루��Ӻ��ָ����Polygon����ά����
	* @return ��
	*/
	virtual void addRing(LinearRing3d * pRing )=0;

	virtual TriangleMesh3d * asTriangleMesh() =0;
};
/** @} */



/** @addtogroup Geometry ParameterizedSurface3d-��ά������������ 
*  @{
*/
class CORE_API ParameterizedSurface3d : virtual public Surface3d
{
public:
	/** ��ò������������ͱ��	
	* @param  ��
	* @return �������������ͱ��ֵ
    */
	virtual unsigned char getParameterizedType() const=0;

	/** ���ò�������������
	* @param [in] iType	��֪��������������ֵ	
	* @return ��
    */
	virtual void setParameterizedType(unsigned char iType)=0;

	/** ��ÿ��Ƶ�����	
	* @param  [out] vControlPoints ������ֵ�Ŀ��Ƶ����������
	* @return ��
    */
	virtual void getControlVertices(vector<Vertex3d>& vControlPoints) const=0;

	/** ���ÿ��Ƶ�����
	* @param  [in] vControlPoints	��֪���Ƶ����������	
	* @return ��
    */
	virtual void setControlVertices(const vector<Vertex3d>& vControlPoints)=0;
};
/** @} */

/** @addtogroup Geometry MultiPolygon3d-��ά������ 
*  @{
*/
class CORE_API MultiPolygon3d : virtual public Surface3d
{
public:
	/**  ��ȡ��϶������ĸ���
	* @param  ��
	* @return ��϶������ĸ���
	*/
	virtual long getPolygonNumber()=0;

	/**  ��ȡ����ṹ�����
	* @param  [in] ib  ��ṹ�����
	* @return ��
	*/
	virtual void getPolygonIndexBuffer(IndexBuffer & ib)=0;
	/**  ��ȡ��϶���θ��ԵĽ����Ŀ��
	* @param  [in] ib  ��϶���θ��ԵĽ����Ŀ��
	* @return ��
	*/
	virtual void getVertexNumberPerPolygon(IndexBuffer & ib )=0;
	
	/**  ����������Ϣ
	* @param  [in] PolygonNumber  ����θ���
	* @param  [in] VertexNumberPrePolygon  ÿ������εĶ�����
	* @param  [in] VertexIndices  ����ζ��������б�
	* @return ��
	*/
	virtual void setPolyTopos(const long PolygonNumber, const std::vector<long> & VertexNumberPrePolygon, const std::vector<long> & VertexIndices )=0;
};
/** @} */

/** @addtogroup Geometry Triangle3d-��ά�������� 
*  @{
*/
class CORE_API Triangle3d : virtual public Surface3d
{
public:
	/**  ���������ζ�������
	* @param  [in] v  ������������
	* @return ��
	*/
	virtual void setVertices(Vertex3d v[3])=0;
	/**  ��ȡ�����ζ�������
	* @param  [in,out] v  ������������
	* @return ��
	*/
	virtual void getVertices(Vertex3d v[3])=0;

	/**  ���������η���
	* @param  [in] vNormal  �����η���
	* @return ��
	*/
	virtual void setNormal(const Vertex3d & vNormal)=0;
	/**  ��ȡ�����η���
	* @param  [in,out] vNormal  �����η���
	* @return ��
	*/
	virtual void getNormal(Vertex3d & vNormal )=0;

	/** ���������ܳ�
	* @param  ��
	* @return �ܳ�
    */
	virtual double getPerimeter()=0;
};
/** @} */

/** @addtogroup Geometry Rectangle3d-��ά������ 
*  @{
*/
class CORE_API Rectangle3d : virtual public Surface3d
{
public:
	/** ����γ���
	* @param  ��
	* @return ����ֵ
    */
	virtual double getLength()=0;

	/** ��ȡ���ο��
	* @param  ��
	* @return ���ֵ
    */
	virtual double getWidth()=0;
};
/** @} */

/** @addtogroup Geometry Volume3d-��ά���� 
*  @{
*/
class CORE_API Volume3d : virtual public Geometry
{
public:
	/** ��ȡ��������
	* @param  ��
	* @return �������
    */
	virtual double getVolume()=0;

	/**  ��ȡ����λ�����������
	* @param  ��
	* @return ����λ�����������ָ��
	*/
	virtual VertexVisitorSharedPtr getVertexVisitor()=0;
	/** ��ö������ͱ���	
	* @param  ��
	* @return �������ͱ���
	*/
	virtual unsigned char getVertexType() const=0;
	/** ���ö������ͱ���
	* @param  [in] iVertexType	 �������ͱ���
	* @return ��
	*/ 
	virtual void setVertexType(unsigned char iVertexType)=0;
	/** �����ά�������	
	* @param  ��
	* @return ��ά�������
	*/
	virtual long getVerticesNum() const=0;
	/** ������ά��������Ͷ�������
	* @param  [in] nVertexNum	��ά�������	
	* @param  [in] vVertices	ָ��һ����ά������ָ��	
	* @return ��
	*/
	virtual void setVertices(long nVertexNum, void* const vVertices)=0;
	/** �����ά��㶥������	
	* @param [out] vVertices ������ֵ��һ����ά�����ָ�������
	* @param [out] vNum ������ֵ��һ����ά�����ָ�������
	* @return ��
	*/
	virtual void getVertices( long& lVertNum, void*& vVertices ) const=0;
};
/** @} */

/** @addtogroup Geometry Solid3d-��ά�������� 
*  @{
*/
class CORE_API Solid3d : virtual public Volume3d
{
public:
	/**  ��ȡ�������
	* @param  [out] f  �������
	* @return ��
	*/
	virtual void getSurfaces(std::vector<Surface3d*> & f)=0;
	/**  �����������һ���߽���
	* @param  [in] type  �߽��������
	* @return �����ı߽���ָ��
	*/
	virtual Surface3d * newSurface(int type) =0;
};
/** @} */

/** @addtogroup Geometry MultiSolid3d-��ά���Ӷ������� 
*  @{
*/
class CORE_API MultiSolid3d : virtual public Volume3d
{
public:
	/**  ��ȡ�������
	* @param  [out] f  �������
	* @return ��
	*/
	virtual void getSurfaces(std::vector<Surface3d*> & f)=0;
};
/** @} */

/** @addtogroup Geometry Voxel3d-��ά��Ԫ�� 
*  @{
*/
class CORE_API Voxel3d : virtual public Volume3d
{
public:
	/** �����Ԫ���ͱ��	
	* @param  ��
	* @return ��Ԫ���ͱ��
    */
	virtual unsigned char getVolumeTypeID()=0;
};
/** @} */

/** @addtogroup Geometry Cube3d-��ά�������� 
*  @{
*/
class CORE_API Cube3d : virtual public Voxel3d
{
public:
	/** ��ȡ����
	* @param  ��
	* @return ����ֵ
    */
	virtual double getLength() const =0;

	/** ��ȡ���
	* @param  ��
	* @return ���ֵ
    */
	virtual double getWidth() const =0;

	/** ��ȡ�߶�
	* @param  ��
	* @return �߶�ֵ
    */
	virtual double getHeight() const = 0;
};
/** @} */

/** @addtogroup Geometry Prism3d-��ά������ 
*  @{
*/
class CORE_API Prism3d : virtual public Voxel3d
{
public:
	/** ��ȡ����
	* @param  ��
	* @return ����ֵ
    */
	virtual double getLength() const =0;

	/** ��ȡ���
	* @param  ��
	* @return ���ֵ
    */
	virtual double getWidth() const = 0;

	/** ��ȡ�߶�
	* @param  ��
	* @return �߶�ֵ
    */
	virtual double getHeight() const = 0;

	/** ��õ������涥�����	
	* @param  ��
	* @return �������涥�����
    */
	virtual int getVerNumPerSide()=0;
	
	/** ���õ������涥�����
	* @param [in] VerNum  �������涥�����
	* @return ��
    */
	virtual void setVerNumPerSide(int VerNum)=0;
};
/** @} */

/** @addtogroup Geometry Tetrahedron3d-��ά�������� 
*  @{
*/
class CORE_API Tetrahedron3d : virtual public Voxel3d
{
public:
};
/** @} */

/** @addtogroup Geometry Group3d-��ά���� 
*  @{
*/
class CORE_API Group3d : virtual public Geometry
{
public:
	/**  ��ȡ�ӵ�Ԫ�б�
	* @param  [in,out] f  �ӵ�Ԫ�б�
	* @return ��
	*/
	virtual void getElements(std::vector<Geometry*> & f)=0;

	/** ��洢���ζ���ָ��������������һ��Ԫ��
	* @param  [in] pGeometry	����ӵļ��ζ����ָ��	
	* @return Ԫ���������е����
    */
	virtual int addElement(Geometry* const pGeometry) = 0;

	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param  ��
	* @return ��
    */
	virtual void removeAll()=0;

	/** ��մ洢���ζ���ָ����������ͷ�������Ԫ��ָ�������ڴ�(��ʱ���ͷ��ڴ�)
	* @param  ��
	* @return ��
    */
	virtual void clear() =0;
};
/** @} */


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
