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
/** @defgroup Geometry Geometry-������ 
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

	GEOMTYPE_3D_CUBIOD, //������
	GEOMTYPE_3D_IRREGULARVOL,//��������
	GEOMTYPE_3D_CYLINDER,//Բ��
	GEOMTYPE_3D_SPHERE,


	GEOMTYPE_2D_GEOMETRY,	//���ζ������
	GEOMTYPE_2D_GEOMETRY2D,	//��ά���ζ������
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


/** @addtogroup Geometry Geometry-������ 
*  @{
*/
class  CORE_API Geometry {
public:

public:
	/** @name ��̬��Ա����˵��  
     *  @{
	 */

	/** ���촫��ļ������Ͷ�Ӧ�ļ��ζ���
     * @param  [in]  type int ���ζ�������
     * @return ���ع���ļ��ζ����ָ��
   */
    static GeometrySharedPtr create(int type);
	/** ��Buffer�ж�ȡ��Ϣ�������ζ���
     * @param  [in]   buf Buffer & ��������������
     * @return ���ع���ļ��ζ����ָ��
    */
    static GeometrySharedPtr create( Buffer & buf);	
	 /** @} */
public:
	 /**@name ���Ի�ȡ�����ú���˵��  
     *  @{
	 */

    /** ��ȡ��������
     * @param  [in] ��
     * @return ��������
    */
    virtual int getType() const =0;
	/** ��ȡ���ζ����״̬��ÿ��BIT��ʾһ��״̬����
     * @param  [in] ��
     * @return ���ζ����״̬
    */
    virtual int getStatus() const =0;
	/** ���ü��ζ����״̬��ÿ��BIT��ʾһ��״̬����
     * @param  [in] �µ�ֵ
     * @return ��
    */
    virtual void setStatus(int )=0;
	/** ��ȡ���ζ��������
     * @param  [in] ��
     * @return  ���ؼ��ζ�������
   */
    virtual std::string getName()const =0;
	/** ���ü��ζ��������
     * @param  [in] �µļ��ζ�������
     * @return ��
    */
    virtual void setName(std::string )=0;
	/** ��ȡ���ζ���ı߽���Σ�Ϊ��ά�߽磬����Ƕ�ά������Z�ķ�ΧΪ0
     * @param  [in] ��
     * @return ���ر߽���ε�����
    */
    virtual void getEnvelope(Envelope3d & ) = 0; 

	/** ���ü������ͣ���Ҫ��������ʹ��
     * @param  [in] type int ���ζ�������
     * @return ��
	 */
    virtual void setType(const int type)=0;	
	/** ���ü��ζ���İ�Χ�߽���Σ���Ҫ��������ʹ��
     * @param  [in] e3d const Envelope3d & �߽����
     * @return ��
	 */
    virtual void setEnvelope(const Envelope3d & e3d) = 0;
	/** ���ü��ζ�����ʾ״̬Ϊ�ɼ��򲻿ɼ�
     * @param  [in] flag �ɼ��򲻿ɼ���ʶ��trueΪ�ɼ���falseΪ���ɼ�
     * @return ��
	 */
	virtual void setVisible(bool flag)=0;
	/** ��ȡ���ζ�����ʾ״̬ 
     * @param  ��  
     * @return �ɼ��򲻿ɼ���ʶ��trueΪ�ɼ���falseΪ���ɼ�
	 */
	virtual bool isVisible()=0;
	/** ͳ�Ƶ�ǰ��������ļ�������
	* @param 
	* @return long*:�����ڲ����ٿռ䡢�ⲿ�ͷ�
	*               ����ֵ������ -- ���������������Ч�������������������Ч��ɫ����
	*								�����������������ת��Ϊ�ȼ�����
	*								��������������
	*/
	virtual long * statistics()=0;

	 /** @} */

	
public:
	/** @name �����ݶ�д�Ĳ�������  
     *  @{
	 */

    /** ��g��ָ��ļ��ζ����Ƶ������ζ���
     * @param  [in] g const Geometry *, ָ��������ļ��϶����ָ�� 
     * @return �Ƿ񿽱��ɹ�
    */
    virtual bool copy(   const Geometry * g)=0;
	virtual bool copy(   const GeometrySharedPtr g) ;
	/** ��¡����
	 * @param  [in]  
	 * @return �Ƿ񿽱��ɹ�
	 */
	virtual Geometry * clone( )=0;
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
    virtual void write(std::ostream & f)=0;
	/** �ӱ�׼����������ȡ�������Ϣ  
     * @param  [in,out] f  std::istream &����׼��������������
     * @return �ɹ�����true�����򷵻�false
    */
    virtual void read(std::istream & f)=0;
  public:
	/** ͨ������ľ���Լ������ݽ��б任
	* @param [in] Matrix4x4�� m �任����
	* @return	void
    */
	 virtual void translateFrom(Matrix4x4 & m)=0;
	 /** ��ѯָ���Ľӿ��Ƿ����
	* @param [in] signal	��������
	* @param [out] pObj	    ���ζ���ӿ�
	* @return bool:ִ�гɹ�����true�����򷵻�false
    */
	 virtual bool queryInterface(int signal, void ** pObj) ;
	/** �ж������Ƿ񴩹���ǰ�����壬��������㵽�뼸���彻�����С����
	* @param [in] ray	��������
	* @param [out] dLength	������㵽�뼸���彻�����С���룬�����ཻ���ֵȡ�����
	* @return	1 -- �ཻ��0 -- ���ཻ
    */
	virtual int rayCross(const RaySharedPtr & ray, double& length) const=0;
	/** �жϽ�ͷ׵���뵱ǰ�������Ƿ��ཻ
	* @param [in] frustum	����׵��
	* @return	1 -- �ཻ��0 -- ���ཻ
    */
	virtual int frustumIntersect(const FrustumSharedPtr & frustum)=0;
	
	 /** @} */
};

/** @} */ 


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
