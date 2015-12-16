#include "internal_MultiPolyline3dImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
//##ModelId=49E6DE6101C5
// void MultiPolyline3dImpl::SetWidth(const unsigned int wWidth)
// {
// 	// TODO: Add your specialized code here.
// }
// 
// //##ModelId=49E6DE730119
// unsigned int MultiPolyline3dImpl::GetWidth()
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49E6DE92029F
// double MultiPolyline3dImpl::Length()
// {
// 	// TODO: Add your specialized code here.
// 	return (double)0;
// }
// 
// //##ModelId=49E7362E004E
// unsigned int MultiPolyline3dImpl::NumberGeometry()
// {
// 	// TODO: Add your specialized code here.
// 	// NOTE: Requires a correct return value to compile.
// }
// 
// //##ModelId=49E73663005D
// int MultiPolyline3dImpl::GetDataSize()
// {
// 	// TODO: Add your specialized code here.
// 	return (int)0;
// }
// 
// //##ModelId=49E73670034B
// MultiPolyline3dImpl::GetIndexSize()
// {
// }

bool MultiPolyline3dImpl::readBuffer(Buffer & buf)
{

	return false;
}


bool MultiPolyline3dImpl::writeBuffer(Buffer& buf)
{

	return false;
}


size_t MultiPolyline3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += sizeof(unsigned char); //m_byteTypeID (3DGeometry)

	nSize += sizeof(unsigned char); //m_byteODim (Geometry)
	nSize += sizeof(int); //m_nRefCount (Geometry)
	nSize += sizeof(int); //m_nObjFlag (Geometry)
	nSize += sizeof(int); //用于int charNum 记录字符串中字符个数，需写入文件 
	int charNum = m_sObjName.length() + 1;
	nSize += sizeof(char) * charNum; //m_sObjName (Geometry)

	nSize += sizeof(int); //m_bClone (3DGeometry)
	nSize += sizeof(AABBoxImpl); //m_eEnvelope (3DGeometry)

	//专有信息
	//*************************************************


	return nSize;
}

void MultiPolyline3dImpl::TranslateFrom(Matrix4x4& m4x4)
{

}

GeometryImpl* MultiPolyline3dImpl::CreateObject()
{
	return new MultiPolyline3dImpl;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace