#include "internal_linearring3dimpl.h"
#include "internal_spatialfuns.h"


begin_gtl_namespace
begin_gdb_namespace
double LinearRing3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	vector<Vertex3d> vPts;
	GetVerticesEx(vPts);

	Vertex3d tempPt;
	return DPointToPolygonEx(vPt, vPts, &tempPt);
}

char  LinearRing3dImpl::RPointWithGeometry(Vertex3d& vPt) const
{
	vector<Vertex3d> vPts;
	GetVerticesEx(vPts);

	return RPointWithPolygon(vPt, vPts);
}


int LinearRing3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	vector<Vertex3d> vPts;
	GetVerticesEx(vPts);
	Vertex3d IntersectPt;

	int bCross = 0;
	Vertex3d vNormal;
	int nIndex = GetPointsNormal(vPts, vNormal);
	if (nIndex == -1)	// 所有点共线
		return -1;

	PLANE3D plane;
	GetPlaneEquation(vNormal, vPts[0], plane);

	Vertex3d vPt1 = ray.origin;
	Vertex3d vPt2 = ray.origin + ray.direction;

	char cRes = RLineWithPlane(vPt1, vPt2, plane, &IntersectPt);
	if (cRes == '1')
	{
		if (IsPointInPolygon(IntersectPt, vPts))//点在多边形内
		{
			//	if(((vPt2 - vPt1) * (IntersectPt - vPt1)) > 1e-6) //xx_0128
			if (((vPt2 - vPt1) * (IntersectPt - vPt1)) > MathEx::TOL)
			{
				// 这里要注意的是 需要比较先穿过哪个墙壁
				double tempDis = DPointToPointEx(IntersectPt, vPt1);//得到距离
				//if(tempDis < dLength)
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

bool LinearRing3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Geometry3dImpl::readBuffer(buf);

	//专有信息******************************

	buf.read((char*)&m_iVertexType, sizeof(unsigned char));
	buf.read((char*)&m_lVertNum, sizeof(long));

	if (m_lVertNum > 0)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)
		{
			m_vVertices = new Vertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(Vertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			m_vVertices = new Vertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(Vertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			m_vVertices = new TexturedVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(TexturedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			m_vVertices = new TexturedVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(TexturedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			m_vVertices = new ShadedVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(ShadedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			m_vVertices = new ShadedVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(ShadedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			m_vVertices = new VersatileVertex3d[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(VersatileVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			m_vVertices = new VersatileVertex3f[m_lVertNum];
			buf.read((char*)m_vVertices, sizeof(VersatileVertex3f) * m_lVertNum);
		}
	}

	buf.read((char*)&m_bInOrOutRing, sizeof(int));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}

}


bool LinearRing3dImpl::writeBuffer(Buffer& buf)
{

	int flag = 0;
	flag = Geometry3dImpl::writeBuffer(buf);

	//专有信息******************************

	buf.write((char*)&m_iVertexType, sizeof(unsigned char));
	buf.write((char*)&m_lVertNum, sizeof(long));
	if (m_lVertNum > 0)
	{

		if (m_iVertexType == VERTEXTYPE_V3d)
		{
			Vertex3d* pVetexList = (Vertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(Vertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(Vertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			TexturedVertex3d* pVetexList = (TexturedVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(TexturedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(TexturedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(ShadedVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(ShadedVertex3f) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(VersatileVertex3d) * m_lVertNum);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			buf.write((char*)pVetexList, sizeof(VersatileVertex3f) * m_lVertNum);
		}
	}

	buf.write((char*)&m_bInOrOutRing, sizeof(int));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t LinearRing3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	nSize += Geometry3dImpl::sizeBuffer();
	//专有信息
	//*************************************************

	if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertexList;	
	{
		nSize += sizeof(Vertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3f)
	{
		nSize += sizeof(Vertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		nSize += sizeof(TexturedVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2f)
	{
		nSize += sizeof(TexturedVertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
	{
		nSize += sizeof(ShadedVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		nSize += sizeof(ShadedVertex3f) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		nSize += sizeof(VersatileVertex3d) * m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
	{
		nSize += sizeof(VersatileVertex3f) * m_lVertNum;
	}
	nSize += sizeof(long); // m_lVertNum
	nSize += sizeof(int); //m_bInOrOutRing
	nSize += sizeof(unsigned char); //m_iVertexType

	//*************************************************

	return nSize;
}

void LinearRing3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);

	//变换坐标的同时，重新计算外包矩形
	m_eEnvelope.Empty();
	Vertex3d v;

	long i;
	for (i = 0; i < m_lVertNum; i++)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertices;	
		{
			Vertex3d* pVetexList = (Vertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;
			m4x4.transformAffine(pVetexList[i]);
			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);

		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2d)
		{
			TexturedVertex3d* pVetexList = (TexturedVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);
			//pVetexList[i] *= m4x4;	

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);
			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			//pVetexList[i] *= m4x4;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			//pVetexList[i] *= m4x4;	
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
	}
}

void LinearRing3dImpl::GetVerticesEx(vector<Vertex3d>& vt3d) const
{
	vt3d.reserve(m_lVertNum);
	if (m_iVertexType == VERTEXTYPE_V3d)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			Vertex3d tempVt = ((Vertex3d*)m_vVertices)[i];
			vt3d.push_back(tempVt);
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			Vertex3f tempVt = ((Vertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fT2f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			TexturedVertex3f tempVt = ((TexturedVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			TexturedVertex3d tempVt = ((TexturedVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			ShadedVertex3d tempVt = ((ShadedVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			ShadedVertex3f tempVt = ((ShadedVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			VersatileVertex3f tempVt = ((VersatileVertex3f*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}

	else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		for (int i = 0; i<m_lVertNum; i++)
		{
			VersatileVertex3d tempVt = ((VersatileVertex3d*)m_vVertices)[i];
			vt3d.push_back(Vertex3d(tempVt.x, tempVt.y, tempVt.z));
		}
	}
}

GeometryImpl* LinearRing3dImpl::CreateObject()
{
	return new LinearRing3dImpl;
}

//计算外部指定的三维线段到当前几何对象距离的平方
double LinearRing3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	Vertex3d p1, p2, p3;

	std::vector<Vertex3d> positions;
	GetVerticesEx(positions);

	for (long i = 0; i<m_lVertNum - 2; i++)//遍历每个三角形
	{
		p1 = positions[0];
		p2 = positions[i + 1];
		p3 = positions[i + 2];

		dis = DLineSegToTriangleEx(begvPt, endvPt, p1, p2, p3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	return mindis;
}

//计算外部指定的三角形到当前几何对象距离的平方
double LinearRing3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	Vertex3d p1, p2, p3;

	std::vector<Vertex3d> positions;
	GetVerticesEx(positions);

	for (long i = 0; i<m_lVertNum - 2; i++)//遍历每个三角形
	{
		p1 = positions[0];
		p2 = positions[i + 1];
		p3 = positions[i + 2];

		dis = DTriangleToTriangleEx(triPt1, triPt2, triPt3, p1, p2, p3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

//计算当前对象的几何重心
Vertex3d LinearRing3dImpl::CalCorePoint()
{
	if (m_lVertNum == 0)
		return Vertex3d(1e+14, 1e+14, 1e+14);

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	Vertex3d corePt;
	for (long i = 0; i < m_lVertNum; i++)
		corePt += positons[i];
	corePt /= m_lVertNum;

	return corePt;
}

int LinearRing3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);

	if (nRes == 0 || nRes == 1)
		return nRes;

	vector<Vertex3d> vt3d;
	GetVerticesEx(vt3d);

	if (RFrustumWithPolygon(frustum, vt3d) == '1')
		return 1;
	else
		return 0;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* LinearRing3dImpl::GeoStatistics()
{
	long* statis = new long[8];

	statis[0] = m_lVertNum;

	if (m_iVertexType == VERTEXTYPE_V3d || m_iVertexType == VERTEXTYPE_V3f)
	{
		statis[1] = 0;
		statis[2] = 0;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2f || m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		statis[1] = m_lVertNum;
		statis[2] = 0;
	}
	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f || m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		statis[1] = 0;
		statis[2] = m_lVertNum;
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f || m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		statis[1] = m_lVertNum;
		statis[2] = m_lVertNum;
	}

	for (int i = 3; i < 8; i++)
		statis[i] = 0;

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool LinearRing3dImpl::Init()
{
	bool res = Geometry3dImpl::Init();
	if (false == res) return false;

	VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);
	m_iVertexType = VERTEXTYPE_V3d;
	m_lVertNum = 0;
	m_bInOrOutRing = 0; // 外环
	return true;
}

int LinearRing3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2) // 线环直接求解，不用计算凸包
	{
		if (res == 0) return 0;

		vector<Vertex3d> pts;
		GetVerticesEx(pts);
		return RPolygonWithPolygon2D(pts, ptArray);
	}
	else
		return -1;
}

end_gdb_namespace
end_gtl_namespace

