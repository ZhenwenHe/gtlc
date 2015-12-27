
#include "internal_multipoint3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
void MultiPoint3dImpl::GetVerticesEx(vector<Vertex3d>& vt3d) const
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

GeometryImpl* MultiPoint3dImpl::CreateObject()
{
	return new MultiPoint3dImpl;
}

void MultiPoint3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Point3dImpl::TranslateFrom(m4x4);

	//变换坐标的同时，重新计算外包矩形
	m_eEnvelope.Empty();
	Vertex3d v;

	long i;
	for (i = 0; i < m_lVertNum; i++)
	{
		if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertices;	
		{
			Vertex3d* pVetexList = (Vertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3f)
		{
			Vertex3f* pVetexList = (Vertex3f*)m_vVertices;
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

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2f)
		{
			TexturedVertex3f* pVetexList = (TexturedVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
		{
			ShadedVertex3d* pVetexList = (ShadedVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
		{
			ShadedVertex3f* pVetexList = (ShadedVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
		else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
		{
			VersatileVertex3d* pVetexList = (VersatileVertex3d*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			m_eEnvelope.Union(pVetexList[i]);
		}
		else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
		{
			VersatileVertex3f* pVetexList = (VersatileVertex3f*)m_vVertices;
			m4x4.transformAffine(pVetexList[i]);

			v.x = pVetexList[i].x;
			v.y = pVetexList[i].y;
			v.z = pVetexList[i].z;
			m_eEnvelope.Union(v);
		}
	}
}

size_t MultiPoint3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Point3dImpl)
	//*************************************************

	nSize += Point3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(unsigned char); // 	unsigned char m_iVertexType;
	nSize += sizeof(long); // 	long m_lVertNum;

	if (m_iVertexType == VERTEXTYPE_V3d)	//void* m_vVertices;	
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

	nSize += sizeof(long) * m_lVertNum; // long m_lAttrNum;
	nSize += sizeof(unsigned int); //size_t m_nAttributeSize;
	nSize += m_nAttributeSize; //void *m_nAttributes;


	//*************************************************	
	return nSize;
}

bool MultiPoint3dImpl::readBuffer(Buffer & buf)
{

	int flag = 0;
	flag = Point3dImpl::readBuffer(buf);

	//专有信息******************************
	buf.read(&m_iVertexType, sizeof(unsigned char));
	buf.read((char*)&m_lVertNum, sizeof(long));
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

	buf.read((char*)&m_lAttrNum, sizeof(long));
	buf.read((char*)&m_nAttributeSize, sizeof(unsigned int));
	if (m_nAttributeSize > 0)
	{
		m_pAttributes = new char[m_nAttributeSize];
	}
	buf.read((char*)m_pAttributes, m_nAttributeSize);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool MultiPoint3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Point3dImpl::writeBuffer(buf);

	//专有信息******************************	
	buf.write(&m_iVertexType, sizeof(unsigned char));
	buf.write((char*)&m_lVertNum, sizeof(long));


	if (m_iVertexType == VERTEXTYPE_V3d)
	{
		buf.write(m_vVertices, sizeof(Vertex3d) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3f)
	{
		buf.write(m_vVertices, sizeof(Vertex3f) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2d)
	{
		buf.write(m_vVertices, sizeof(TexturedVertex3d) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2f)
	{
		buf.write(m_vVertices, sizeof(TexturedVertex3f) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3dN3dC3f)
	{
		buf.write(m_vVertices, sizeof(ShadedVertex3d) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3fN3fC3f)
	{
		buf.write(m_vVertices, sizeof(ShadedVertex3f) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3dT2dN3dC3f)
	{
		buf.write(m_vVertices, sizeof(VersatileVertex3d) * m_lVertNum);
	}
	else if (m_iVertexType == VERTEXTYPE_V3fT2fN3fC3f)
	{
		buf.write(m_vVertices, sizeof(VersatileVertex3f) * m_lVertNum);
	}

	buf.write((char*)&m_lAttrNum, sizeof(long));
	buf.write((char*)&m_nAttributeSize, sizeof(unsigned int));
	buf.write((char*)m_pAttributes, m_nAttributeSize);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//计算当前对象的几何重心
Vertex3d MultiPoint3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);
	for (long i = 0; i < m_lVertNum; i++)
		corePt += positons[i];
	corePt /= m_lVertNum;

	return corePt;
}

//计算外部指定的三维点到当前几何对象距离的平方
double MultiPoint3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	double dis, mindis = 1e+14;

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	for (long i = 0; i<m_lVertNum; i++)
	{
		dis = DPointToPointEx(positons[i], vPt);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

//计算外部指定的三维线段到当前几何对象距离的平方
double MultiPoint3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	for (long i = 0; i<m_lVertNum; i++)
	{
		dis = DPointToLineSegEx(positons[i], begvPt, endvPt);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

//计算外部指定的三角形到当前几何对象距离的平方
double MultiPoint3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;

	std::vector<Vertex3d> positons;
	GetVerticesEx(positons);

	for (long i = 0; i<m_lVertNum; i++)
	{
		dis = DPointToTriangleEx(positons[i], triPt1, triPt2, triPt3);
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}

	return mindis;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* MultiPoint3dImpl::GeoStatistics()
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
bool MultiPoint3dImpl::Init()
{
	bool res = Point3dImpl::Init();
	if (false == res) return false;

	VertexVisitor::releaseVertexArray(m_iVertexType, m_vVertices);
	if (m_pAttributes)
	{
		delete m_pAttributes;
		m_pAttributes = NULL;
	}

	m_iVertexType = VERTEXTYPE_V3d;
	m_lVertNum = 0;
	m_nAttributeSize = 0;
	m_lAttrNum = 0;

	return true;
}

int MultiPoint3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2)
	{
		if (res == 0) return 0;

		vector<Vertex3d> vt3d;
		GetVerticesEx(vt3d);
		for (long i = 0; i < m_lVertNum; i++)
		{
			char flag = RPointWithPolygon2d(vt3d[i], ptArray);
			if (flag == 'i' || flag == 'V' || flag == 'E')
				return 1;
		}

		return 0;
	}
	else
		return -1;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace