#include "internal_Reference3dImpl.h"
#include "internal_spatialfuns.h"
#include "internal_SharedModelImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
Reference3dImpl::Reference3dImpl()
{
	m_byteTypeID = GEOTYPE_3D_REFERENCE;

	m_nLinkType = 0;

	m_nExtParaSize = 0;

	m_pExtParaData = NULL;

	m_nLinkID = -1;
	m_strLinkName = "";

#if defined (_MSC_VER) && (_MSC_VER > 1200)//高于VC6的版本
#else
	m_pLinkModel = NULL;
#endif//高于VC6的版本


	m_dScale = 1;

}

Reference3dImpl::~Reference3dImpl()
{
	// 	if(m_pExtParaData)
	// 	{
	// 		delete m_pExtParaData;
	// 	}
	// 	if(m_pLinkModel)
	// 	{
	// 		delete m_pLinkModel;
	// 	}
}

void Reference3dImpl::GetExtParaData(int& nExtParaSize, void*& pExeParaData) const
{
	nExtParaSize = m_nExtParaSize;
	pExeParaData = m_pExtParaData;
}

void Reference3dImpl::SetExtParaData(int nExtParaSize, void* pExeParaData)
{
	m_nExtParaSize = nExtParaSize;
	m_pExtParaData = pExeParaData;
}

double Reference3dImpl::DPointToGeometryEx(Vertex3d& vPt) const
{
	Geometry3dImpl* pRefGeo = dynamic_cast<Geometry3dImpl*>(m_pLinkModel->getGeometry().get());

	//得到拷贝的REF对象(注意析构)(接下来，是对该拷贝对象的几何变换，以保证不改变模型库数据)
	GeometryImpl* pCopyRef = pRefGeo->Clone();//改函数负责释放开辟的内存

	//对RefGeo进行几何变换到当前位置
	Vertex3d center(0.0, 0.0, 0.0);
	Vertex3d scale(m_dScale, m_dScale, m_dScale);
	Matrix4x4 m4x4;
	m4x4.buildTransformMatrix(m_vPoint, scale, m_qOrientation, center, center);
	pCopyRef->TranslateFrom(m4x4);

	double dis = (dynamic_cast<Geometry3dImpl*>(pCopyRef))->DPointToGeometryEx(vPt);

	//释放pCopyRef的内存空间
	delete pCopyRef;
	return dis;
}

bool Reference3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;
	flag = Point3dImpl::readBuffer(buf);

	//特有信息*************************************
	Vertex3d vPosition;
	buf.read((char*)&vPosition, sizeof(Vertex3d));
	buf.read((char*)&m_qOrientation, sizeof(Quaternion));
	buf.read((char*)&m_dScale, sizeof(double));
	buf.read((char*)&m_nLinkType, sizeof(int));
	buf.read((char*)&m_nLinkID, sizeof(Identifier::raw_type));
	m_vPoint.x = vPosition.x, m_vPoint.y = vPosition.y, m_vPoint.z = vPosition.z;

	int ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		char tempbuf[50];
		buf.read(tempbuf, sizeof(char) * ll);
		tempbuf[ll] = '\0';
		m_strLinkName = tempbuf;
	}

	buf.read((char*)&m_nExtParaSize, sizeof(int));//外部链接数据
	if (m_nExtParaSize > 0)
	{
		m_pExtParaData = new int[m_nExtParaSize];
		buf.read((char*)m_pExtParaData, m_nExtParaSize);
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


bool Reference3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;
	flag = Point3dImpl::writeBuffer(buf);

	//特有信息*************************************
	Vertex3d vPosition(m_vPoint);
	buf.write((char*)&vPosition, sizeof(Vertex3d));
	buf.write((char*)&m_qOrientation, sizeof(Quaternion));
	buf.write((char*)&m_dScale, sizeof(double));
	buf.write((char*)&m_nLinkType, sizeof(int));
	buf.write((char*)&m_nLinkID, sizeof(Identifier::raw_type));

	int ll = m_strLinkName.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_strLinkName.c_str(), sizeof(char) * ll);
	}


	buf.write((char*)&m_nExtParaSize, sizeof(int));//外部链接数据
	if (m_nExtParaSize > 0)
	{
		buf.write((char*)m_pExtParaData, m_nExtParaSize);
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

size_t Reference3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Point3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(Vertex3d); // m_vPosition
	nSize += sizeof(Quaternion); // m_qOrientation
	nSize += sizeof(double); //m_dScale
	nSize += sizeof(int); // m_nLinkType
	nSize += sizeof(Identifier::raw_type); // m_nLinkID  //xx//

	nSize += sizeof(int); //用于int charNum 记录字符串中字符个数，需写入文件 
	int charNum = m_strLinkName.length();
	nSize += sizeof(char) * charNum; //m_strLinkName

	nSize += sizeof(int);  // m_nExtParaSize
	nSize += m_nExtParaSize; // void* m_pExtParaData

	//*************************************************

	return nSize;
}

int Reference3dImpl::RayCrossGeometry(RayImpl& ray, double& dLength) const
{
	if (0 == Geometry3dImpl::RayCrossGeometry(ray, dLength))
		return 0;

	Matrix4x4 newm4x4;
	Vertex3d scale(m_dScale, m_dScale, m_dScale);
	Vertex3d scaleCen = m_eEnvelope.CenterPoint();
	Vertex3d RotateCen = m_eEnvelope.CenterPoint();
	newm4x4.buildTransformMatrix(m_vPoint, scale, m_qOrientation, scaleCen, RotateCen); // 原矩阵

	Geometry3dImpl* pRefGeo = dynamic_cast<Geometry3dImpl*>(m_pLinkModel->getGeometry().get());
	if (pRefGeo)
	{
		Geometry3dImpl* pRefCopy = dynamic_cast<Geometry3dImpl*>(pRefGeo->Clone());
		pRefCopy->TranslateFrom(newm4x4);

		int nRes = pRefCopy->RayCrossGeometry(ray, dLength);

		//析构拷贝对象
		delete pRefCopy;

		return nRes;
	}

	return 0;
}

void Reference3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Point3dImpl::TranslateFrom(m4x4);

	Matrix3x3 m3x3;
	m4x4.extract3x3Matrix(m3x3);
	double *pData = m3x3.getData();
	int iIndex[6] = { 1, 2, 3, 5, 6, 7 };


	//由m4x4更新便变换参数,并到新的变换矩阵
	//由原变换参数计算矩阵，并和传入的矩阵相乘，得到新矩阵
	Matrix4x4 newm4x4;

	Vertex3d scale(m_dScale, m_dScale, m_dScale);
	if (m_nLinkType == 1)
	{
		scale.x = m_eEnvelope.XSize();
		scale.y = m_eEnvelope.YSize();
		scale.z = m_eEnvelope.ZSize();
	}
	Vertex3d scaleCen = m_vPoint;//m_eEnvelope.CenterPoint() ;
	Vertex3d RotateCen = m_vPoint;//_eEnvelope.CenterPoint() ;
	newm4x4.buildTransformMatrix(m_vPoint, scale, m_qOrientation, scaleCen, RotateCen); // 原矩阵
	newm4x4 *= m4x4;// 新矩阵

	Vertex3d oldPos = m_vPoint;

	Matrix3x3 m3x3Fromnewm4x4;
	newm4x4.extract3x3Matrix(m3x3Fromnewm4x4);
	m_qOrientation.fromRotateMatrix(m3x3Fromnewm4x4);
	double *p4new = newm4x4.getData();

	Vertex3d oldCen = m_eEnvelope.CenterPoint();

	// 	RotateCen -= m_vPosition;
	// 	newm4x4.TransformAffine(RotateCen);

	// 	Vertex3d maxPt = m_eEnvelope.GetMax();
	// 	maxPt -= m_vPosition;
	// 	newm4x4.TransformAffine(maxPt);

	Vertex3d vPosition = m_vPoint;
	vPosition = newm4x4.getTranslateVector();
	m_vPoint.x = vPosition.x, m_vPoint.y = vPosition.y, m_vPoint.z = vPosition.z;

	int i = 0;
	for (i = 0; i<6; i++)
	{
		if (fabs(pData[iIndex[i]]) > MathEx::TOL) // 有一个不为0  说明不是缩放矩阵
			break;
	}

	if (i >= 6) // 缩放矩阵
	{
		if (fabs(pData[0] - 1.0) > MathEx::TOL) //  不为1
			m_dScale *= pData[0];
	}

	//重新计算外包矩形
	m_eEnvelope.Empty();
	Geometry3dImpl* pRefGeo = dynamic_cast<Geometry3dImpl*>(m_pLinkModel->getGeometry().get());
	if (pRefGeo)
	{
		Geometry3dImpl* pRefCopy = dynamic_cast<Geometry3dImpl*>(pRefGeo->Clone());//(Geometry3dImpl*)m_pLinkModel->getGeometry()->Clone();
		pRefCopy->TranslateFrom(newm4x4);
		m_eEnvelope = pRefCopy->getEnvelope();

		//析构拷贝对象
		delete pRefCopy;
	}

}

GeometryImpl* Reference3dImpl::CreateObject()
{
	return new Reference3dImpl;
}

//计算外部指定的三维线段到当前几何对象距离的平方(取外包矩形)
double Reference3dImpl::D3DLineSegToGeometryEx(Vertex3d& begvPt, Vertex3d& endvPt)
{
	double mindis = m_eEnvelope.GetClosestLineDisToAABBBox(begvPt, endvPt);
	return mindis * mindis;;
}

//计算外部指定的三角形到当前几何对象距离的平方
double Reference3dImpl::D3DTriangleToGeometryEx(Vertex3d& triPt1, Vertex3d& triPt2, Vertex3d& triPt3)
{
	double mindis = m_eEnvelope.GetClosestTriangleDisToAABBBox(triPt1, triPt2, triPt3);
	return mindis*mindis;
}

//计算当前对象的几何重心
Vertex3d Reference3dImpl::CalCorePoint()
{
	SharedModelImpl * p = dynamic_cast<SharedModelImpl*> (m_pLinkModel.get());
	Vertex3d corePt = (dynamic_cast<Geometry3dImpl*>(p->getGeometry().get()))->CalCorePoint();
	Matrix4x4 m4x4;
	m4x4.buildTransformMatrix(m_vPoint, Vertex3d(m_dScale, m_dScale, m_dScale), m_qOrientation, Vertex3d(0, 0, 0), Vertex3d(0, 0, 0));
	corePt = corePt* m4x4;

	return corePt;
}

/** 虚函数
* 统计当前对象包含的几何数据
* @return long*:函数内部开辟空间、外部释放
*               返回值依次是 -- 顶点数，顶点的有效纹理坐标数，顶点的有效颜色数；
*								面对象数，法向数，转化为等价三角
*								材质数，纹理数，
*/
long* Reference3dImpl::GeoStatistics()
{
	long* statis = NULL;

	if (m_pLinkModel)
	{
		Geometry3dImpl* pref = dynamic_cast<Geometry3dImpl*>(m_pLinkModel->getGeometry().get()); // 用关联模型统计，不作几何变换
		if (pref)
			statis = pref->GeoStatistics();
	}

	return statis;
}

// 释放对象数据并将指针和变量赋初始值
bool Reference3dImpl::Init()
{
	bool res = Point3dImpl::Init();
	if (false == res) return false;

	m_nLinkType = 0;

	m_nExtParaSize = 0; // 这不部分需要释放吗 ？// ydcl
	m_pExtParaData = NULL;

	m_nLinkID = -1;
	m_strLinkName = "";

	m_vPoint.zero();
	m_qOrientation = Quaternion(1, 0, 0, 0);
	m_dScale = 1;

	return true;
}

int Reference3dImpl::RGeoWithPolygon2D(vector<Vertex3d>& ptArray, int precisionFlag)
{
	int res = Geometry3dImpl::RGeoWithPolygon2D(ptArray, precisionFlag);

	if (precisionFlag == 0)
		return res;
	else if (precisionFlag == 1 || precisionFlag == 2)
	{
		if (res == 0) return 0;

		Geometry3dImpl* pref = dynamic_cast<Geometry3dImpl*>(m_pLinkModel->getGeometry().get());
		if (pref)
		{
			GeometryImpl* pCopyRef = NULL;
			pCopyRef = pref->Clone();

			//对RefGeo进行几何变换到当前位置
			Matrix4x4 m4x4;
			m4x4.buildTransformMatrix(m_vPoint, Vertex3d(m_dScale, m_dScale, m_dScale), m_qOrientation, Vertex3d(0, 0, 0), Vertex3d(0, 0, 0));
			pCopyRef->TranslateFrom(m4x4);
			Geometry3dImpl* p = dynamic_cast<Geometry3dImpl*>(pCopyRef);

			int r = p->RGeoWithPolygon2D(ptArray, precisionFlag);
			delete pCopyRef;

			return r;
		}

		return -1;
	}
	else
		return -1;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace