#include "internal_rectangle3dimpl.h"
#include "internal_polygon3dimpl.h"


begin_gtl_namespace
begin_gdb_namespace
//##ModelId=49E34729000F
// Rectangle3dImpl::Length()
// {
// }
// 
// //##ModelId=49E347290010
// Rectangle3dImpl::Width()
// {
// }
// 
// //##ModelId=49E347290011
// Rectangle3dImpl::Area()
// {
// }

bool Rectangle3dImpl::readBuffer(Buffer & buf)
{
	return false;
}


bool Rectangle3dImpl::writeBuffer(Buffer& buf)
{
	return false;
}

size_t Rectangle3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(Vertex3d) * 4;// m_vVertices[4];


	return nSize;
}

void Rectangle3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	for (int i = 0; i < 4; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}
}
GeometryImpl* Rectangle3dImpl::CreateObject()
{
	return new Rectangle3dImpl;
}
/** 虚函数，在派生类中实现
* 计算外部指定的三维线段到当前几何对象距离的平方
* @param [in] vPt	Vertex3d类型三维点对象的引用
* @return 点到当前几何对象距离的平方
*/
double Rectangle3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	return 0;
}
//计算当前对象的几何重心
Vertex3d Rectangle3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 4; i++)
		corePt += m_vVertices[i];
	corePt /= 4;
	return corePt;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* Rectangle3dImpl::GeoStatistics()
{
	long* statis = new long[8];


	for (int i = 0; i < 8; i++)
		statis[i] = 0;

	// ydcl_xx

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool Rectangle3dImpl::Init()
{
	/*bool res = Polygon3dImpl::Init();
	if(false == res) return false;

	for( int i = 0; i < 4; i++ )
	m_vVertices[i] = Vertex3d(0,0,0);*/

	return true;
}

end_gdb_namespace
end_gtl_namespace

