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
#include "texture.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

class TextureImpl : public Texture
{
protected:

	/** 纹理ID */
	unsigned long long m_nTextureID;

	/** 纹理mingch */
	string m_strTextureName;

	/** 纹理类型 */
	int m_nTextureType;

	/** 纹理内存数据 */
	unsigned char* m_pTexdata;

	/** 纹理数据长度 */
	size_t m_nLength;

	int m_wrapMode;

public:
	/** 构造函数
	*/
	TextureImpl();
	/** 析构函数
	*/
	~TextureImpl();
	 
	enum{ BMP, JPG, DDS, TGA, TIF };

	// WRAP模式                  
	enum WrapMode {
		CLAMP = 0x2900,
		REPEAT = 0x2901,
	};

 

	/** 获得纹理ID
	* @param
	* @return  纹理ID
	*/
	unsigned long long GetTextureID() const
	{
		return m_nTextureID;
	}

	/** 设置注记颜色
	* @param [in] nTexI	 纹理ID
	* @return
	*/
	void SetTextureID(unsigned long long nTexID)
	{
		m_nTextureID = nTexID;
	}

	/** 获得纹理名称
	* @param
	* @return 纹理名称
	*/
	std::string GetTextureName() const
	{
		return m_strTextureName;
	}

	/** 设置纹理名称
	* @param [in]  strTexName 纹理名称
	* @return
	*/
	void SetTextureName(std::string strTexName)
	{
		m_strTextureName = strTexName;
	}

	/** 获得纹理类型
	* @param
	* @return 纹理类型
	*/
	int GetTextureType() const
	{
		return m_nTextureType;
	}

	/** 设置纹理类型
	* @param [in] nTexType 纹理类型
	* @return
	*/
	void SetTextureType(int nTexType)
	{
		m_nTextureType = nTexType;
	}

	/** 获得纹理数据长度
	* @param
	* @return  纹理数据长度
	*/
	size_t GetTextureSize() const
	{
		return m_nLength;
	}

	/** 设置纹理数据长度
	* @param [in] length  纹理数据长度
	* @return
	*/
	void SetTextureSize(size_t length)
	{
		m_nLength = length;
	}

	/** 获得纹理内存数据
	* @param
	* @return 纹理内存数据指针
	*/
	unsigned char* GetTextureData() const
	{
		return m_pTexdata;
	}

	/** 设置纹理内存数据
	* @param [in] texdata 纹理内存数据指针
	* @return
	*/
	void SetTextureData(unsigned char* texdata)
	{
		m_pTexdata = texdata;
	}

	/** 获得纹理的WRAP模式
	* @param
	* @return 纹理的WRAP模式
	*/
	int GetWrapMode() const
	{
		return m_wrapMode;
	}

	/** 设置纹理的WRAP模式
	* @param [in] nWrapMode 纹理纹理的WRAP模式
	* @return
	*/
	void SetWrapMode(int nWrapMode)
	{
		m_wrapMode = nWrapMode;
	}
	 

public:

	virtual cn::edu::cug::gdb::Identifier::raw_type getID() const {
		return m_nTextureID;
	}
	virtual void setID(cn::edu::cug::gdb::Identifier::raw_type d) {
		m_nTextureID = d;
	}
	virtual std::string getName() const {
		return m_strTextureName;
	}
	virtual void setName(std::string strTexName){
		m_strTextureName = strTexName;
	}
	virtual int getType() const {
		return this->m_nTextureType;
	}
	virtual void setType(int nTexType){
		m_nTextureType = nTexType;
	}
	virtual size_t getDataSize() const {
		return m_nLength;
	}
	virtual void setDataSize(size_t length) {
		m_nLength = length;
	}
	virtual unsigned char* getData() const{
		return m_pTexdata;
	}
	virtual void setData(unsigned char* texdata){
		m_pTexdata = texdata;
	}

	virtual int getWrapMode() const
	{
		return m_wrapMode;
	}

	virtual void setWrapMode(int nWrapMode)
	{
		m_wrapMode = nWrapMode;
	} 
};
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace