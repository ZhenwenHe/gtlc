#include "internal_ParameterizedSurface3dImpl.h"
#include "internal_TriangleMesh3dImpl.h"
#include "internal_Polygon3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
TriangleMesh3dImpl* ParameterizedSurface3dImpl::Generate(int nPntU, int nPntV)
{
	// TODO: Add your specialized code here.
	return (TriangleMesh3dImpl*)0;
}

bool ParameterizedSurface3dImpl::readBuffer(Buffer & buf)
{
	return false;
}


bool ParameterizedSurface3dImpl::writeBuffer(Buffer& buf)
{
	return false;
}

size_t ParameterizedSurface3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//专有信息
	//*************************************************


	return nSize;
}

void ParameterizedSurface3dImpl::TranslateFrom(Matrix4x4& m4x4)
{

}

GeometryImpl* ParameterizedSurface3dImpl::CreateObject()
{
	return new ParameterizedSurface3dImpl;
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace