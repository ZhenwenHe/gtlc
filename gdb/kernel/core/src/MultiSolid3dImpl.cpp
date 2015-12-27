#include "internal_multisolid3dimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 unsigned int MultiSolid3dImpl::NumberGeometry()
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return 0;
 }

 void MultiSolid3dImpl::SetMeanNormal(int bMeanNormal)
 {
 }
 
 unsigned int MultiSolid3dImpl::GetDataSize()
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return 0;
 }
 
 void MultiSolid3dImpl::GetAll(const unsigned int n, Vertex3d* pdData)
 {
 }
 
 void MultiSolid3dImpl::SetAll(const unsigned int n, const Vertex3d* pdData)
 {
 }
 unsigned int MultiSolid3dImpl::GetIndexSize()
 {
	 return 0;
 }
 
 //##ModelId=49E736DA02AF
 int MultiSolid3dImpl::GetIndex(int nIndexSize, int* IndexData)
 {
	 return -1;
 }
 
bool MultiSolid3dImpl::readBuffer(Buffer & buf)
{

	return false;
}


bool MultiSolid3dImpl::writeBuffer(Buffer& buf)
{

	return false;
}
size_t MultiSolid3dImpl::sizeBuffer()
{
	return 0;
}

void MultiSolid3dImpl::TranslateFrom(Matrix4x4& m4x4)
{

}

GeometryImpl* MultiSolid3dImpl::CreateObject()
{
	return new MultiSolid3dImpl;
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace