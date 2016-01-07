#include "internal_solid3dimpl.h"
#include "internal_trianglemesh3dimpl.h"
#include "internal_parameterizedsurface3dimpl.h"
#include "internal_triangle3dimpl.h"
#include "internal_multipolygon3dimpl.h"
#include "internal_polygon3dimpl.h"
#include "internal_spatialfuns.h"


begin_gtl_namespace
begin_gdb_namespace
 

Surface3d *  Solid3dImpl::newSurface(int signal){
	Surface3d * pSurf = 0;
	if (signal == GEOTYPE_3D_TRIANGLEMESH)//11	
	{
		pSurf = (Surface3d *)((Surface3dImpl*)new TriangleMesh3dImpl);
	}
	else if (signal == GEOTYPE_3D_SURFACE)//2
	{
		pSurf = (Surface3d *)(new Surface3dImpl);
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDSURFACE)//10
	{
		pSurf = (Surface3d *)((Surface3dImpl*)new ParameterizedSurface3dImpl);
	}
	else if (signal == GEOTYPE_3D_TRIANGLE)//12
	{
		pSurf = (Surface3d *)((Surface3dImpl*)new Triangle3dImpl);
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYGON)//13
	{
		pSurf = (Surface3d *)((Surface3dImpl*)new MultiPolygon3dImpl);
	}
	else if (signal == GEOTYPE_3D_POLYGON)//14
	{
		pSurf = (Surface3d *)((Surface3dImpl*)new Polygon3dImpl);
	}

	if (pSurf)
	{
		m_vBoundarySurfaces.push_back(dynamic_cast<Surface3dImpl*>(pSurf));
	}

	return pSurf;
}

int Solid3dImpl::FrustumIntersectGeometry(FrustumImpl& frustum)
{
	int nRes = Geometry3dImpl::FrustumIntersectGeometry(frustum);
	if (nRes == 0 || nRes == 1)
		return nRes;

	int nCount = m_vBoundarySurfaces.size();
	for (int i = 0; i<nCount; i++)
	{
		nRes = m_vBoundarySurfaces[i]->FrustumIntersectGeometry(frustum);
		if (nRes == 1)
			return 1;
	}
	return 0;
}

int Solid3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	int nCount = m_vBoundarySurfaces.size();
	double dMin = MathEx::dDoubleMax;
	int bCross = 0;

	for (int k = 0; k<nCount; k++)
	{
		int nGeoType = m_vBoundarySurfaces[k]->getType();
		if (nGeoType != GEOTYPE_3D_TRIANGLEMESH
			&& nGeoType != GEOTYPE_3D_POLYGON
			&& nGeoType != GEOTYPE_3D_TRIANGLE
			&& nGeoType != GEOTYPE_3D_MULTIPOLYGON)
			continue;

		Surface3dImpl *pMesh = m_vBoundarySurfaces[k];
		double dDis;
		int bRes = pMesh->RayCrossGeometry(ray, dDis);

		//		if(bRes && dMin > dDis)  //xx_0128
		if (bRes && ((dMin - dDis) > MathEx::TOL))
		{
			dMin = dDis;
			bCross = 1;
		}
	}
	dLength = dMin;
	return bCross;
}

double Solid3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	int nCount = m_vBoundarySurfaces.size();
	double dMin = 0.0;
	for (int k = 0; k<nCount; k++)
	{
		int nGeoType = m_vBoundarySurfaces[k]->getType();
		if (nGeoType != GEOTYPE_3D_TRIANGLEMESH
			&& nGeoType != GEOTYPE_3D_POLYGON
			&& nGeoType != GEOTYPE_3D_TRIANGLE
			&& nGeoType != GEOTYPE_3D_MULTIPOLYGON)
			continue;

		Surface3dImpl *pMesh = m_vBoundarySurfaces[k];
		double dDis = pMesh->DPointToGeometryEx(vPt);
		if (k == 0)
			dMin = dDis;
		else
		{
			//	if(dMin > dDis)  //xx_0128
			if ((dMin - dDis) > MathEx::TOL)
				dMin = dDis;
		}
	}
	return dMin;
}

char Solid3dImpl::RPointWithGeometry(Vertex3d& vPt) const
{
	char cRes = Geometry3dImpl::RPointWithGeometry(vPt);
	if (cRes == 'o')
		return 'o';

	Vertex3d r;  /* Ray endpoint. */
	Vertex3d p;  /* Intersection point; not used. */
	int f, k = 0, crossings = 0;
	char code = '?';

	double radius = m_eEnvelope.Diagonal();
	int nCount = m_vBoundarySurfaces.size();

LOOP:
	//while( k++ < dFaceSize ) {
	while (1){
		crossings = 0;

		GetRandomRay(r, radius); // 求得任意ray的向量，长度为radius
		r = vPt + r;	// 加上r得到ray的终点坐标

		for (int k = 0; k<nCount; k++)
		{
			if (m_vBoundarySurfaces[k]->getType() != GEOTYPE_3D_TRIANGLEMESH)
				continue;

			TriangleMesh3dImpl *pMesh = dynamic_cast<TriangleMesh3dImpl*>(m_vBoundarySurfaces[k]);
			vector<Vertex3d> vt3d;
			pMesh->GetVerticesEx(vt3d);

			long lTriListSize;
			long *lTriangleList;
			pMesh->GetTriangles(lTriangleList, lTriListSize);
			double dFaceSize = lTriListSize / 3;

			for (f = 0; f < dFaceSize; f++) {  /* Begin check each face */
				AABBoxImpl env;
				env.Union(vt3d[lTriangleList[f * 3]]);
				env.Union(vt3d[lTriangleList[f * 3 + 1]]);
				env.Union(vt3d[lTriangleList[f * 3 + 2]]);

				if ((vPt.x - env.GetMin().x) < -MathEx::TOL && (r.x - env.GetMin().x) < -MathEx::TOL ||
					(vPt.y - env.GetMin().y) < -MathEx::TOL && (r.y - env.GetMin().y) < -MathEx::TOL ||
					(vPt.z - env.GetMin().z) < -MathEx::TOL && (r.z - env.GetMin().z) < -MathEx::TOL)	//	segment与三角形外包框不相交
					code = '0';
				else if ((vPt.x - env.GetMax().x) > MathEx::TOL && (r.x - env.GetMax().x) > MathEx::TOL ||
					(vPt.y - env.GetMax().y) > MathEx::TOL && (r.y - env.GetMax().y) >  MathEx::TOL ||
					(vPt.z - env.GetMax().z) > MathEx::TOL && (r.z - env.GetMax().z) > 	MathEx::TOL)//	segment与三角形外包框不相交
					code = '0';
				else
					code = RRayCrossTri(vPt, r, vt3d[lTriangleList[f * 3]], vt3d[lTriangleList[f * 3 + 1]], vt3d[lTriangleList[f * 3 + 2]]);

				/* If ray is degenerate, then goto outer while to generate another. */
				if (code == 'p' || code == 'v' || code == 'e') {
					goto LOOP;
				}

				/* If ray hits face at interior point, increment crossings. */
				else if (code == 'f') {
					crossings++;
				}

				/* If query endpoint q sits on a V/E/F, return that code. */
				else if (code == 'V' || code == 'E' || code == 'F')
					return(code);

				/* If ray misses triangle, do nothing. */
				else if (code == '0')
					;

				else
					continue;

			} /* End check each face */
		}
		/* No degeneracies encountered: ray is generic, so finished. */
		break;

	} /* End while loop */

	//TRACE( "Crossings = %d\n", crossings );
	/* q strictly interior to polyhedron if an odd number of crossings. */
	if ((crossings % 2) == 1)	// 奇数次相交
		return   'i';
	else return 'o';

}

bool Solid3dImpl::readBuffer(Buffer & buf)
{
	int flag1 = 0;
	flag1 = Volume3dImpl::readBuffer(buf);

	//专有信息******************************
	GeometryImpl* pGeometry = NULL;
	int geoType = -1;
	int nCount = 0;
	int flag = 0;

	buf.read((char*)&nCount, sizeof(int));//体中面的个数
	m_vBoundarySurfaces.reserve(nCount);
	for (int i = 0; i<nCount; i++)
	{
		flag = 0;

		unsigned int nSize = 0;
		buf.read((char*)&nSize, sizeof(unsigned int));//第i个面对象的大小

		char* p = (char*)buf.data() + buf.getPos();
		memcpy((char*)&geoType, p, sizeof(int));
		// 			if(geoType == GEOTYPE_3D_TRIANGLE)
		// 			{
		// 				pSurface = new Triangle3dImpl;			
		// 			}
		// 			else if(geoType == GEOTYPE_3D_TRIANGLEMESH)
		// 			{
		// 				pSurface = new TriangleMesh3dImpl;
		// 			}
		pGeometry = GeometryImpl::create(geoType);

		flag = pGeometry->readBuffer(buf);
		if (!flag) return 0;
		Surface3dImpl* pSurface = dynamic_cast<Surface3dImpl*>(pGeometry);
		m_vBoundarySurfaces.push_back((Surface3dImpl*)pSurface);
	}

	if (flag1)
	{
		return true;
	}
	else
	{
		return false;
	}

}


bool Solid3dImpl::writeBuffer(Buffer& buf)
{
	int flag1 = 0;
	flag1 = Volume3dImpl::writeBuffer(buf);

	//专有信息******************************
	int nCount = m_vBoundarySurfaces.size();
	buf.write((char*)&nCount, sizeof(int));

	for (int i = 0; i<nCount; i++)
	{
		unsigned int nSize = 0;
		nSize = m_vBoundarySurfaces[i]->sizeBuffer();
		buf.write((char*)&nSize, sizeof(unsigned int));//下面对象需要的内存大小

		// 			if(m_vBoundarySurfaces[i]->getType() == GEOTYPE_3D_TRIANGLE)
		// 			{
		// 				((Triangle3dImpl*)m_vBoundarySurfaces[i])->writeBuffer(buf);
		// 			}
		// 			else if(m_vBoundarySurfaces[i]->getType() == GEOTYPE_3D_TRIANGLEMESH)
		// 			{
		// 				((TriangleMesh3dImpl*)m_vBoundarySurfaces[i])->writeBuffer(buf);
		// 			}
		m_vBoundarySurfaces[i]->writeBuffer(buf);
	}

	if (flag1)
	{
		return 1;
	}
	else
	{
		return 0;
	}
}

size_t Solid3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Volume3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(int);//Solid 中面得个数
	for (size_t i = 0; i< m_vBoundarySurfaces.size(); i++)
	{
		nSize += sizeof(unsigned int); // 存放下一个面内存大小的变量
		nSize += m_vBoundarySurfaces[i]->sizeBuffer();
	}

	return nSize;
}

void Solid3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Volume3dImpl::TranslateFrom(m4x4);

	size_t i;
	for (i = 0; i < m_vBoundarySurfaces.size(); i++)
	{
		m_vBoundarySurfaces[i]->TranslateFrom(m4x4);
	}

	//重新计算外包矩形
	m_eEnvelope.Empty();
	for (i = 0; i < m_vBoundarySurfaces.size(); i++)
	{
		m_eEnvelope.Union(m_vBoundarySurfaces[i]->getEnvelope());
	}
}

GeometryImpl* Solid3dImpl::CreateObject()
{
	return new Solid3dImpl;
}
//计算外部指定的三维线段到当前几何对象距离的平方
double Solid3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double dis, mindis = 1e+14;
	for (long i = 0; i < m_vBoundarySurfaces.size(); i++)
	{
		dis = m_vBoundarySurfaces[i]->D3DLineSegToGeometry(begvPt, endvPt);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	return mindis*mindis;
}

//计算外部指定的三角形到当前几何对象距离的平方
double Solid3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double dis, mindis = 1e+14;
	for (long i = 0; i < m_vBoundarySurfaces.size(); i++)
	{
		dis = m_vBoundarySurfaces[i]->D3DTriangleToGeometry(triPt1, triPt2, triPt3);
		//	if(dis < mindis) mindis = dis;
		if ((dis - mindis) < -MathEx::TOL) mindis = dis;
	}
	return mindis*mindis;
}

//计算当前对象的几何重心
Vertex3d Solid3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	long nNun = m_vBoundarySurfaces.size();
	for (long i = 0; i < nNun; i++)
		corePt += m_vBoundarySurfaces.at(i)->CalCorePoint();
	corePt /= nNun;
	return corePt;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* Solid3dImpl::GeoStatistics()
{
	int i = 0;
	long* statis = new long[8];
	for (i = 0; i < 8; i++)
		statis[i] = 0;

	long n = 0;
	for (n = 0; n < m_vBoundarySurfaces.size(); n++)
	{
		long* tempsta = m_vBoundarySurfaces[n]->GeoStatistics();
		for (i = 0; i < 8; i++)
			statis[i] += tempsta[i];
		delete[]tempsta;
	}

	// ydcl_xx_材质纹理没有去除重复

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool Solid3dImpl::Init()
{
	bool res = Volume3dImpl::Init();
	if (false == res) return false;

	int nCount = m_vBoundarySurfaces.size();
	for (int i = nCount - 1; i>-1; i--)
	{
		if (m_vBoundarySurfaces[i])
			delete m_vBoundarySurfaces[i];
	}
	m_vBoundarySurfaces.clear();

	return true;
}

int Solid3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2)
	{
		if (res == 0) return 0;

		for (long i = 0; i < m_vBoundarySurfaces.size(); i++)
			if (1 == m_vBoundarySurfaces[i]->RGeoWithPolygon2D(ptArray, precisionFlag))
				return 1;
		return 0;
	}
	else
		return -1;
}
end_gdb_namespace
end_gtl_namespace

