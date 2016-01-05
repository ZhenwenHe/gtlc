#include "internal_triangle3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
bool Triangle3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;
	flag = Surface3dImpl::readBuffer(buf);

	//对象的独立信息**********************

	buf.read((char*)m_vVetices, sizeof(Vertex3d) * 3);
	buf.read((char*)&m_vNormal, sizeof(Vertex3d));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Triangle3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Surface3dImpl::writeBuffer(buf);

	//对象独立信息**************************

	buf.write((char*)m_vVetices, sizeof(Vertex3d) * 3);
	buf.write((char*)&m_vNormal, sizeof(Vertex3d));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Triangle3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Surface3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(Vertex3d) * 3; //Vertex3d m_vVetices[3];
	nSize += sizeof(Vertex3d); //Vertex3d m_vNormal

	//*************************************************

	return nSize;
}

void Triangle3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Surface3dImpl::TranslateFrom(m4x4);

	//变换坐标的同时，变换法线
	Vertex3d p0 = m_vVetices[0];
	p0 += m_vNormal;

	m4x4.transformAffine(p0);
	for (int i = 0; i < 3; i++)
	{
		m4x4.transformAffine(m_vVetices[i]);
	}

	m_vNormal.x = p0.x - m_vVetices[0].x;
	m_vNormal.y = p0.y - m_vVetices[0].y;
	m_vNormal.z = p0.z - m_vVetices[0].z;

	//重新计算外包矩形
	m_eEnvelope.Empty();
	m_eEnvelope.Union(m_vVetices, 3);
}

GeometryImpl* Triangle3dImpl::CreateObject()
{
	return new Triangle3dImpl;
}
//计算外部指定的三维线段到当前几何对象距离的平方
double Triangle3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double mindis = 1e+14;
	mindis = DLineSegToTriangle(begvPt, endvPt, m_vVetices[0], m_vVetices[1], m_vVetices[2]);
	return mindis*mindis;
}

//计算外部指定的三角形到当前几何对象距离的平方
double Triangle3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	return DTriangleToTriangleEx(triPt1, triPt2, triPt3, m_vVetices[0], m_vVetices[1], m_vVetices[2]);
}

int Triangle3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	Vertex3d vPt1 = ray.origin;
	Vertex3d vPt2 = ray.origin + ray.direction;

	int bCross = 0;
	dLength = 1e+14;
	Vertex3d IntersectPt;
	PLANE3D plane;
	Vertex3d triPt1 = m_vVetices[0];
	Vertex3d triPt2 = m_vVetices[1];
	Vertex3d triPt3 = m_vVetices[2];

	int bRes = GetPlaneEquation(triPt1, triPt2, triPt3, plane);
	if (!bRes)
		return bCross;;
	char cRes = RLineWithPlane(vPt1, vPt2, plane, &IntersectPt);
	if (cRes == '1')
	{
		if (IsPointInTri(triPt1, triPt2, triPt3, IntersectPt))//点在三角形内
		{
			double dRes = (vPt2 - vPt1) * (IntersectPt - vPt1);
			//		if(dRes > 1e-6) //xx_0128
			if (dRes > MathEx::TOL)
			{
				double tempDis = DPointToPointEx(IntersectPt, vPt1);
				//		if(tempDis < dLength)
				if ((tempDis - dLength) < -MathEx::TOL)
				{
					dLength = tempDis;
					bCross = 1;
				}
			}
		}
	}
	return bCross;
}

//计算当前对象的几何重心
Vertex3d Triangle3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	for (long i = 0; i < 3; i++)
		corePt += m_vVetices[i];
	corePt /= 3;
	return corePt;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* Triangle3dImpl::GeoStatistics()
{
	long* upstatis = Surface3dImpl::GeoStatistics();
	long* statis = new long[8];

	statis[0] = 3;
	statis[1] = 0;
	statis[2] = 0;

	statis[3] = 1;
	statis[4] = 1;
	statis[5] = 1;

	statis[6] = upstatis[6];
	statis[7] = upstatis[7];

	if (upstatis)
	{
		delete[]upstatis;
		upstatis = NULL;
	}

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool Triangle3dImpl::Init()
{
	bool res = Surface3dImpl::Init();
	if (false == res) return false;

	for (int i = 0; i < 3; i++)
		m_vVetices[i] = Vertex3d(0, 0, 0);

	m_vNormal = Vertex3d(0, 0, 0);

	return true;
}

int Triangle3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2)
	{
		if (res == 0) return 0;
		vector<Vertex3d> vts;
		for (int i = 0; i < 3; i++)
			vts.push_back(m_vVetices[i]);

		return RPolygonWithPolygon2D(vts, ptArray);
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace