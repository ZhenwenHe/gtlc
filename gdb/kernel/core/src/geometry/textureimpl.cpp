#include "internal_textureimpl.h"


begin_gtl_namespace
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
end_gtl_namespace

