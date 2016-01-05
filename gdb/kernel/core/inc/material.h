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
#include "texture.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

/** @defgroup material  纹理材质相关类  
    *  @{
    */

// flag :
#define TEXTURE_FLAG_TEXTURE        1//纹理有效
#define TEXTURE_FLAG_MATERIAL       2//材质有效
#define TEXTURE_FLAG_COLOR          4//颜色有效

// transparent: 
#define TEXTURE_TRANSPARENT_DISABLE 0 //不透明
#define TEXTURE_TRANSPARENT_ENABLE  1 //透明，将和背景做混合（如玻璃）, 参数 alpha
#define TEXTURE_TRANSPARENT_SPECIAL 2 //过滤指定部分（如画树滤掉纹理中非树的部分）

//cullface :
#define TEXTURE_CULLFACE_BACK		0//缺省，背面剔除
#define TEXTURE_CULLFACE_DISABLE	1//不剔除(双面可视)
#define TEXTURE_CULLFACE_FRONT		2//正面剔除

//depth:(深度检测)
#define TEXTURE_DEPTH_ENABLE		0//缺省，使用缺省深度检测方式
#define TEXTURE_DEPTH_DISABLE		1//不启用深度检测
#define TEXTURE_DEPTH_ALWAYS		2//深度比较总是通过
#define TEXTURE_DEPTH_WRITEDISABLE	4//深度缓冲只读
/** @} */  

/** @addtogroup material  MAT_PARAM-纹理参数数据结构 
    *  @{
    */
struct MAT_PARAM
{
	/** @name 属性（成员变量）
	* @{
	*/
	
	/** 标志位(纹理、材质、颜色) */
	unsigned char		flag;               
	/** 透明的方式 
	* -TEXTURE_TRANSPARENT_DISABLE
	* -TEXTURE_TRANSPARENT_ENABLE
	* -TEXTURE_TRANSPARENT_SPECIAL
    */
	unsigned char		transparent;        
	/** 面剔除方式 
	* -TEXTURE_CULLFACE_BACK
	* -TEXTURE_CULLFACE_DISABLE
	* -TEXTURE_CULLFACE_FRONT
	*/
	unsigned char		cullface;		
	
	/** 深度检测 */
	unsigned char		depth;	
	
	/** 保留*/
	unsigned char		reserve[4];			

    /** 透明度  [ 0.0 (不透明)  ~  1.0 (完全透明) ] */
	float		alpha;              
	
	/** 环境光 */
	unsigned long	ambient;	
	
	/** 漫反射 */
	unsigned long	diffuse;			

	/** 镜面反射 */
	unsigned long	specular;			

	/** 颜色 */
	unsigned long	color;				

	/** 纹理平移参数 */
	float		uOffset, vOffset;	

	/** 纹理旋转参数（单位为度，旋转轴为 (0, 0, 1)） */
	float		rotAngle;			

	/** 高光*/
	float		shininess;			
	/** 光照模型 */
	float		illumination;
	/** @} */ // 属性结尾
 
	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	MAT_PARAM()
	{
		memset(this, 0, sizeof(MAT_PARAM));
	}
	 /** @} */ // 构造与析构函数结尾
	
};
typedef MAT_PARAM*   LPMAT_PARAM;
/** @} */

/** @addtogroup material  Material 
    *  @{
    */
class Material;
typedef std::shared_ptr<Material> MaterialSharedPtr;
typedef Material* MaterialPtr;
class CORE_API Material  
{
public:
	/*0-简单纹理，1-复杂纹理*/
	virtual int getType() const =0;
	virtual Identifier::raw_type getID()=0;
	virtual void setID(Identifier::raw_type)=0;
	virtual std::string getName()=0;
	virtual void setName(std::string name)=0;

	static MaterialSharedPtr create(int type=0);
};
/** @} */

/** @addtogroup material  SimpleMaterial 
    *  @{
    */
class CORE_API SimpleMaterial  : virtual  public Material
{
public:
	virtual Identifier::raw_type getTextureID()=0;
	virtual void setTextureID(Identifier::raw_type)=0;
	virtual TexturePtr getTexture() =0;
	virtual void setTexture(TexturePtr p) =0;

	virtual MAT_PARAM & getMaterialParameter() =0;
	virtual void setMaterialParameter(MAT_PARAM &) =0;
};
/** @} */

/** @addtogroup material  COMMAT_ITEM 
    *  @{
    */
struct COMMAT_ITEM
{

	/** @name 属性（成员变量）
	* @{
	*/
	
	/** 材质类型 */
	long matType;
	/** 纹理ID */
	int textureID;
	/** 强度 */
	float intensity;
	/** 颜色 */
	unsigned long color;
	/** 在 u 纹理方向上重复的次数 */
	double  uRepeat;	
	/** 在 v 纹理方向上重复的次数 */
	double  vRepeat;	
    /** 纹理平移 */
	float	uOffset;	
	/** 纹理平移 */
	float	vOffset;	
	/** 纹理旋转 */
	float	rotAngle;	
	/** 透明度  [ 0.0 (不透明)  ~  1.0 (完全透明) ] */
	float	alpha;      

	/** @} */ // 属性结尾

	
	/** @name 构造与析构函数
	*  @{
	*/

	/** 构造函数
	*/
	COMMAT_ITEM()
	{
		memset(this, 0, sizeof(COMMAT_ITEM));
	}
    /** @} */ // 构造与析构函数结尾

};
/** @} */

/** @addtogroup material  ComplexMaterial 
    *  @{
    */
class CORE_API ComplexMaterial:virtual public Material {
public:
	/** 复杂材质参数 */
	virtual std::vector<COMMAT_ITEM> & getMaterialItems() =0;
};
/** @} */
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace