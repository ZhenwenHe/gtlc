#include "internal_simplematerialimpl.h"


begin_gtl_namespace
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
end_gtl_namespace

