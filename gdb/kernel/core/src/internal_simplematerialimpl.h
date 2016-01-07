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
#include "internal_textureimpl.h"
#include "internal_materialimpl.h"


begin_gtl_namespace
begin_gdb_namespace
class SimpleMaterialImpl : virtual public  SimpleMaterial, virtual public MaterialImpl
{ 
protected:

	/** 纹理ID参数*/
	Identifier::raw_type m_nTextureID;

	/** 关联的纹理指针*/
	TextureImpl* m_pTexture;

public:
	/** 材质参数*/
	MAT_PARAM m_matParam;
 

public:
 

	/** 构造函数
	*/
	SimpleMaterialImpl();
	/** 析构函数
	*/
	virtual ~SimpleMaterialImpl();

 
	/** 设置纹理
	* @param [in] pTextru指向纹理的指针
	*/
	void SetTexture(TextureImpl* pTextrue)
	{
		// 		if(m_pTexture)
		// 		{
		// 			delete m_pTexture;	
		// 			m_pTexture = NULL;
		// 		}

		m_pTexture = pTextrue;
	}

	/** 获取纹理
	* @return 向纹理的指针
	*/
	TextureImpl* GetTexture()
	{
		return m_pTexture;
	}

	/** 获取材质参数
	* @return 材质
	*/
	MAT_PARAM GetMatParam() const
	{
		return m_matParam;
	}
	/** 设置材质参数
	* @param [in] param材质参数
	*/
	void SetMatParam(MAT_PARAM param)
	{
		m_matParam = param;
	}
	/** 获取关联的纹理ID
	* @return 纹理id
	*/
	Identifier::raw_type GetTextureID() const  //xx//
	{
		return m_nTextureID;
	}
	/** 设置关联的纹理ID
	* @param [in] nTexID纹理ID
	*/
	void SetTextureID(Identifier::raw_type nTexID)  //xx//
	{
		m_nTextureID = nTexID;
	}

 
public:
	virtual Identifier::raw_type getTextureID() {
		return m_nTextureID;
	}
	virtual void setTextureID(Identifier::raw_type d) {
		m_nTextureID = d;
	}
	virtual TexturePtr getTexture() {
		return dynamic_cast<TexturePtr>(m_pTexture);
	}
	virtual void setTexture(TexturePtr p){
		m_pTexture = dynamic_cast<TextureImpl*>(p);
	}

	virtual MAT_PARAM & getMaterialParameter() {
		return m_matParam;
	}
	virtual void setMaterialParameter(MAT_PARAM & d) {
		m_matParam = d;
	}
};

end_gdb_namespace
end_gtl_namespace





