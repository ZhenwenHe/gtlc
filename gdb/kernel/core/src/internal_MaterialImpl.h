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
#include "material.h"
#include "internal_TextureImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
class MaterialImpl:virtual public Material
{
public:
	/*0-简单纹理，1-复杂纹理*/
	virtual int getType() const { return m_nMatType; }
	virtual Identifier::raw_type getID() { return m_nMatID; }
	virtual void setID(Identifier::raw_type mid) { m_nMatID = mid; }
	virtual std::string getName() { return m_strMatName; }
	virtual void setName(std::string name) { m_strMatName = name; }
protected:
 
	/** 纹理类型 */
	int m_nMatType;

	/** 纹理ID */
	unsigned long long m_nMatID;

	/** 纹理名称 */
	string m_strMatName;

	/** @} */ // 属性结尾

 
protected:
	/** 构造函数
	*/
	MaterialImpl();

public:
	enum{ simpleMat, complexMat };
	/** 析构函数
	*/
	virtual ~MaterialImpl();
public:

	/** 获得纹理类型
	* @param
	* @return 纹理类型
	*/
	int GetMatType()
	{
		return m_nMatType;
	}

	/** 获得纹理ID
	* @param
	* @return 纹理ID
	*/
	unsigned long long GetMatID()
	{
		return m_nMatID;
	}

	/** 设置纹理ID
	* @param [in]  nMatID 纹理ID
	* @return
	*/
	void SetMatID(unsigned long long nMatID)
	{
		m_nMatID = nMatID;
	}

	/** 获得纹理名称
	* @param
	* @return 纹理名称
	*/
	string GetMatName()
	{
		return m_strMatName;
	}

	/** 设置纹理名称
	* @param [in] name 纹理名称
	* @return
	*/
	void SetMatName(string name)
	{
		m_strMatName = name;
	}

	 

};

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace