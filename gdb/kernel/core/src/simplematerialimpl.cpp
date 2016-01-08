#include "internal_simplematerialimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
SimpleMaterialImpl::SimpleMaterialImpl()
{
	m_nMatType = simpleMat;
	m_nTextureID = 0;
	m_pTexture = NULL;
}


SimpleMaterialImpl::~SimpleMaterialImpl()
{
}
 
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace