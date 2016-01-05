#include "internal_prism3dimpl.h"
#include "mathex.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

 double Prism3dImpl::Height() const
 {
 	// TODO: Add your specialized code here.
 	return (double)0;
 }
 
 
 Vertex3d Prism3dImpl::TopCenter() const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }
 
 

 Vertex3d Prism3dImpl::BottomCenter()const
 {
 	// TODO: Add your specialized code here.
 	// NOTE: Requires a correct return value to compile.
	 return Vertex3d();
 }

bool Prism3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Voxel3dImpl::readBuffer(buf);

	//对象特有信息************************	

	buf.read((char*)&m_VertNumPerSide, sizeof(int));
	if (m_VertNumPerSide > 0)
	{
		m_vVertices = new Vertex3d[m_VertNumPerSide * 2];
		buf.read((char*)m_vVertices, sizeof(Vertex3d) * m_VertNumPerSide * 2);
	}

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Prism3dImpl::writeBuffer(Buffer& buf)
{

	int flag = 0;
	flag = Voxel3dImpl::writeBuffer(buf);

	//对象独立信息**************************

	buf.write((char*)&m_VertNumPerSide, sizeof(int));
	if (m_VertNumPerSide > 0)
	{
		buf.write((char*)m_vVertices, sizeof(Vertex3d) * m_VertNumPerSide * 2);
	}

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Prism3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Voxel3dImpl::sizeBuffer();

	//专有信息	
	//*************************************************

	nSize += sizeof(Vertex3d) * m_VertNumPerSide * 2; //Vertex3d* m_vVertices;
	nSize += sizeof(int); // m_VertNumPerSide

	//*************************************************		

	return nSize;
}

void Prism3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Voxel3dImpl::TranslateFrom(m4x4);

	int i;
	for (i = 0; i < m_VertNumPerSide * 2; i++)
	{
		//m_vVertices[i] *= m4x4;
		m4x4.transformAffine(m_vVertices[i]);
	}

	//重新计算外包矩形
	m_eEnvelope.Empty();
	m_eEnvelope.Union(m_vVertices, m_VertNumPerSide * 2);
}

GeometryImpl* Prism3dImpl::CreateObject()
{
	return new Prism3dImpl;
}

double Prism3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	long i = 0;
	//下/上底
	for (i = 0; i < m_VertNumPerSide - 2; i++)
	{
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[i + 1], m_vVertices[i + 2]);
		//if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[m_VertNumPerSide], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[m_VertNumPerSide + i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	//侧面
	for (i = 0; i < m_VertNumPerSide - 1; i++)
	{
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[i], m_vVertices[m_VertNumPerSide + i], m_vVertices[m_VertNumPerSide + i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[i], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[m_VertNumPerSide - 1], m_vVertices[2 * m_VertNumPerSide - 1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DLineSegToTriangle(begvPt, endvPt, m_vVertices[0], m_vVertices[2 * m_VertNumPerSide - 1], m_vVertices[m_VertNumPerSide]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//计算外部指定的三角形到当前几何对象距离的平方
double Prism3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	long i = 0;
	//下/上底
	for (i = 0; i < m_VertNumPerSide - 2; i++)
	{
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[i + 1], m_vVertices[i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[m_VertNumPerSide], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[m_VertNumPerSide + i + 2]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	//侧面
	for (i = 0; i < m_VertNumPerSide - 1; i++)
	{
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[i], m_vVertices[m_VertNumPerSide + i], m_vVertices[m_VertNumPerSide + i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
		dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[i], m_vVertices[m_VertNumPerSide + i + 1], m_vVertices[i + 1]);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[m_VertNumPerSide - 1], m_vVertices[2 * m_VertNumPerSide - 1]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	dis = DTriangleToTriangle(triPt1, triPt2, triPt3, m_vVertices[0], m_vVertices[2 * m_VertNumPerSide - 1], m_vVertices[m_VertNumPerSide]);
	//	if(dis < mindis) mindis = dis;
	if ((dis - mindis) < -MathEx::TOL) mindis = dis;

	return mindis*mindis;
}

//计算当前对象的几何重心
Vertex3d Prism3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 2 * m_VertNumPerSide; i++)
		corePt += m_vVertices[i];
	corePt /= 2 * m_VertNumPerSide;
	return corePt;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* Prism3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = m_VertNumPerSide * 2;

	for (int i = 1; i < 8; i++)
		statis[i] = 0;

	// ydcl_xx_这里没有独立面对象，但根据表面，推算面对象、法向和等价三角面片数，方案有待确认
	statis[3] = m_VertNumPerSide + 2;
	statis[4] = m_VertNumPerSide + 2;
	statis[5] = 4 * (m_VertNumPerSide - 1);

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool Prism3dImpl::Init()
{
	bool res = Voxel3dImpl::Init();
	if (false == res) return false;

	if (m_vVertices)
		delete m_vVertices;
	m_vVertices = NULL;

	m_VertNumPerSide = 0;

	return true;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace