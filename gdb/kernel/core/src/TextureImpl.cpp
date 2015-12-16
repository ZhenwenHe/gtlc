#include "internal_TextureImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
TextureImpl::TextureImpl()
{
	m_nTextureID = 0;

	//纹理类型
	m_nTextureType = 1;

	//纹理内存数据
	m_pTexdata = NULL;

	//纹理数据长度
	m_nLength = 0;

	// WRAP模式
	m_wrapMode = REPEAT;
}

TextureImpl::~TextureImpl()
{

	if (m_nLength != 0 && m_pTexdata != NULL)
	{
		delete[]m_pTexdata;
		m_pTexdata = NULL;
		m_nLength = 0;
	}
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace