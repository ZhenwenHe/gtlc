#include "internal_geometry3dimpl.h"
#include "internal_spatialfuns.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace


int Geometry3dImpl::FrustumIntersectAABB(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);
	if (nRes == 0)
		return 0;

	return 1;
}

int Geometry3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	double radius = m_eEnvelope.Diagonal() / 2;
	Vertex3d boxCen = m_eEnvelope.CenterPoint();

	// 最外层的包围球相交情况
	if (0 == IsSphereIntersectSphere(boxCen, radius, frustum.sphCen, frustum.radius))
		return 0;

	// frustum的外包cone与sphere相交情况
	if ('0' == RConeWithSphere(frustum.coneFov, frustum.coneCen, frustum.coneDir, boxCen, radius))
		return 0;

	// frustum开始
	if ('0' == RFrustumWithSphere(frustum, boxCen, radius))
		return 0;

	// 与AABB判断
	char cRes = RFrustumWithEnvelope(frustum, m_eEnvelope);
	if ('0' == cRes)
		return 0;

	if ('1' == cRes)
		return 1;

	return 2;
}

int Geometry3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	dLength = 1e+14;
	if (0 == IsRayCrossEnvelope(ray, m_eEnvelope))
		return 0;
	return 1;
}

Geometry3dImpl::Geometry3dImpl()
{
	m_byteDim = 3;
	m_bClone = 0;
	m_byteTypeID = GEOTYPE_3D_GEOMETRY;
}

Geometry3dImpl::~Geometry3dImpl()
{

}


size_t Geometry3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += GeometryImpl::sizeBuffer(); //基类需要的内存大小

	nSize += sizeof(int); //m_bClone (3DGeometry)
	nSize += sizeof(Vertex3d) * 2; //m_eEnvelope (3DGeometry)

	//专有信息
	//*************************************************

	return nSize;
}

//##ModelId=49E5EE8C029E
bool Geometry3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;

	flag = GeometryImpl::readBuffer(buf);

	buf.read((char*)&m_bClone, sizeof(int));

	Vertex3d vMin;
	Vertex3d vMax;
	buf.read((char*)&vMin, sizeof(Vertex3d));
	buf.read((char*)&vMax, sizeof(Vertex3d));
	m_eEnvelope.SetMin(vMin);
	m_eEnvelope.SetMax(vMax);
	//	buf.read((char*)&m_eEnvelope,sizeof(Envelope3D)-sizeof(int));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

//##ModelId=49E5EEA600BB
bool Geometry3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;

	flag = GeometryImpl::writeBuffer(buf);

	buf.write((char*)&m_bClone, sizeof(int));

	Vertex3d vMin = m_eEnvelope.GetMin();
	Vertex3d vMax = m_eEnvelope.GetMax();
	buf.write((char*)&vMin, sizeof(Vertex3d));
	buf.write((char*)&vMax, sizeof(Vertex3d));

	//buf.write((char*)&m_eEnvelope,sizeof(Envelope3D));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//##ModelId=49EEEB92031C
double Geometry3dImpl::ComputeMinDis(const Geometry3dImpl* pGeometry) const
{
	// TODO: Add your specialized code here.
	return (double)0;
}

void Geometry3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	GeometryImpl::TranslateFrom(m4x4);

	//Geometry3dImpl 里的 m_eEnveope3D 这里不做处理
	//而是放到每个具体的派生类转换后重新计算
}

GeometryImpl* Geometry3dImpl::CreateObject()
{
	return new Geometry3dImpl;
}

// 释放对象数据并将指针和变量赋初始值
bool Geometry3dImpl::Init()
{
	bool res = GeometryImpl::Init();
	if (false == res) return false;

	m_byteDim = 3;
	m_bClone = 0;
	m_eEnvelope.Empty();

	return true;
}

int Geometry3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	return m_eEnvelope.IsBoxintersectWithPolygon2D(ptArray);
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
