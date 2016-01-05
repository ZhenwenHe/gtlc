#include "internal_parameterizedcurve3dimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
bool ParameterizedCurve3dImpl::readBuffer(Buffer & buf)
{

	return false;
}


bool ParameterizedCurve3dImpl::writeBuffer(Buffer& buf)
{

	return false;
}

size_t ParameterizedCurve3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Curve3dImpl::sizeBuffer();

	//专有信息
	//*************************************************


	return nSize;
}

void ParameterizedCurve3dImpl::TranslateFrom(Matrix4x4& m4x4)
{

}


GeometryImpl* ParameterizedCurve3dImpl::CreateObject()
{
	return new ParameterizedCurve3dImpl;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace