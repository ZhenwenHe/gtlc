#include "internal_GeometryImpl.h"
#include "internal_Geometry3dImpl.h"
#include "internal_point3dimpl.h"
#include "internal_SinglePoint3dImpl.h"
#include "internal_MultiPoint3dImpl.h"
#include "internal_Annotation3dImpl.h"
#include "internal_Reference3dImpl.h"

#include "internal_Curve3dImpl.h"
#include "internal_LineString3dImpl.h"
#include "internal_LinearRing3dImpl.h"
#include "internal_MultiPolyline3dImpl.h"
#include "internal_ParameterizedCurve3dImpl.h"

#include "internal_Surface3dImpl.h"
#include "internal_TriangleMesh3dImpl.h"
#include "internal_Polygon3dImpl.h"
#include "internal_MultiPolygon3dImpl.h"
#include "internal_ParameterizedSurface3dImpl.h"
#include "internal_Triangle3dImpl.h"
#include "internal_Rectangle3dImpl.h"

#include "internal_Volume3dImpl.h"
#include "internal_Solid3dImpl.h"
#include "internal_MultiSolid3dImpl.h"
#include "internal_Voxel3dImpl.h"
#include "internal_Cube3dImpl.h"
#include "internal_Prism3dImpl.h"
#include "internal_Tetrahedron3dImpl.h"

#include "internal_Group3dImpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
GeometryImpl::GeometryImpl()
{
	m_byteDim = 0;
	m_nRefCount = 1;
	m_nDrawMode = 0;
	m_nObjFlag = 1;
	m_sObjName = "GEOMETRY";
}
GeometryImpl::~GeometryImpl()
{

}

unsigned char GeometryImpl::GetDimension()
{
	return m_byteDim;
}

/** 增加一个引用记数
* @return
*/
void GeometryImpl::AddRef()
{
	m_nRefCount++;
}

/** 释放对象（减少一个引用记数）
* @return
*/
void GeometryImpl::Release()
{
	m_nRefCount--;
	if (m_nRefCount == 0)
		delete this;
}

/** 获取几何对象绘制模式，此标志为绘制期间使用，不做编辑和保存
* @return 几何对象绘制模式
*/
int GeometryImpl::GetDrawMode()
{
	return m_nDrawMode;
}

/** 设置几何对象绘制模式，此标志为绘制期间使用，不做编辑和保存
* @return
*/
void GeometryImpl::SetDrawMode(int nDrawMode)
{
	m_nDrawMode = nDrawMode;
}

/** 获取几何对象标志
* @return 几何对象标志
*/
int GeometryImpl::GetObjFlag() const
{
	return m_nObjFlag;
}

/** 设置几何对象标志
* @return
*/
void GeometryImpl::SetObjFlag(int nObjFlag)
{
	m_nObjFlag = nObjFlag;
}

bool GeometryImpl::copy(GeometryImpl * geo)
{
	Init();

	bool flag = false;

	size_t nSize = geo->sizeBuffer();
	char* pMem = new char[nSize];
	//buff 接管pMem的内存空间，copy()函数结束，由buff析构函数释放
	Buffer buff;
	buff.attach(pMem, nSize);

	//当前对象this打包，拷贝对象pCopy解包，从而实现对象的拷贝
	flag = geo->writeBuffer(buff);
	if (!flag) return false;
	buff.setPos(0);
	return readBuffer(buff);
}



//复制关联的材质指针和关联的模型指针等内容
void subCloneAttr(GeometryImpl* pSrcGeometry, GeometryImpl* pTarGeometry)
{
	unsigned char signal = pSrcGeometry->getType();
	switch (signal)
	{
	case GEOTYPE_3D_GROUP:
	{
		Group3dImpl* pSrcGroup = dynamic_cast<Group3dImpl*>(pSrcGeometry);
		Group3dImpl* pTarGroup = dynamic_cast<Group3dImpl*>(pTarGeometry);

		vector<Geometry3dImpl*> srcChilds, tarChilds;
		pSrcGroup->GetCollection(srcChilds);
		pTarGroup->GetCollection(tarChilds);

		for (int i = 0; i < srcChilds.size(); i++)
		{
			subCloneAttr(srcChilds[i], tarChilds[i]);
		}
		break;
	}
	case GEOTYPE_3D_SOLID:
	{
		Solid3dImpl* pSrcSolid = dynamic_cast<Solid3dImpl*>(pSrcGeometry);
		Solid3dImpl* pTarSolid = dynamic_cast<Solid3dImpl*>(pTarGeometry);

		vector<Surface3dImpl*> srcChilds, tarChilds;
		pSrcSolid->GetBoundarySurfaces(srcChilds);
		pTarSolid->GetBoundarySurfaces(tarChilds);

		for (int i = 0; i < srcChilds.size(); i++)
		{
			subCloneAttr(srcChilds[i], tarChilds[i]);
		}
		break;
	}
	case GEOTYPE_3D_TRIANGLEMESH:
	case GEOTYPE_3D_POLYGON:
	case GEOTYPE_3D_TRIANGLE:
	case GEOTYPE_3D_MULTIPOLYGON:
	{
		Surface3dImpl* pSrcSurface = dynamic_cast<Surface3dImpl*>(pSrcGeometry);
		Surface3dImpl* pTarSurface = dynamic_cast<Surface3dImpl*>(pTarGeometry);
		pTarSurface->SetMaterial(pSrcSurface->GetMaterial());
		break;
	}
	case GEOTYPE_3D_REFERENCE:
	{
		Reference3dImpl* pSrc = dynamic_cast<Reference3dImpl*>(pSrcGeometry);
		Reference3dImpl* pTar = dynamic_cast<Reference3dImpl*>(pTarGeometry);
		pTar->SetLinkModel(pSrc->GetLinkModel());
		break;
	}
	}
}

/** 将源对象里的属性复制到自身中
* @param [in] pSrcGeometry	源对象
* @return
*/
void GeometryImpl::copyAttr(GeometryImpl* pSrcGeometry)
{
	subCloneAttr(pSrcGeometry, this);
}

GeometryImpl* GeometryImpl::Clone()
{
	//pCopy 开辟的内存空间由调用它的函数负责释放
	GeometryImpl* pCopy = CreateObject();

	size_t nSize = this->sizeBuffer();
	char* pMem = new char[nSize];
	//buff 接管pMem的内存空间，Clone()函数结束，由buff析构函数释放
	Buffer buff;
	buff.attach(pMem, nSize);

	//当前对象this打包，拷贝对象pCopy解包，从而实现对象的拷贝
	this->writeBuffer(buff);
	buff.setPos(0);
	pCopy->readBuffer(buff);

	pCopy->copyAttr(this);

	return pCopy;
}

void GeometryImpl::TranslateFrom(Matrix4x4& m4x4)
{

}

size_t GeometryImpl::sizeBuffer()
{
	size_t nSize = 0;
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.

	nSize += sizeof(int); //m_byteTypeID (GeometryImpl)

	nSize += sizeof(unsigned char); //m_byteODim  (GeometryImpl)
	nSize += sizeof(int);  //m_nRefCount (GeometryImpl)
	nSize += sizeof(int);  //m_nObjFlag  (GeometryImpl)

	nSize += sizeof(int); //用于int charNum 记录字符串中字符个数，需写入文件 
	int charNum = m_sObjName.length();

	nSize += sizeof(char) * charNum; //m_sObjName (GeometryImpl)

	return nSize;
}

bool GeometryImpl::readBuffer(Buffer & buf)
{
	buf.read((char*)&m_byteTypeID, sizeof(int));
	buf.read((char*)&m_byteDim, sizeof(unsigned char));
	buf.read((char*)&m_nRefCount, sizeof(int));
	buf.read((char*)&m_nObjFlag, sizeof(int));

	int ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		m_sObjName.assign(((char*)buf.data()) + buf.getPos(), ll);
		buf.setPos(buf.getPos() + ll);
	}

	return true;
}

//##ModelId=49E5EEA600BB
bool GeometryImpl::writeBuffer(Buffer& buf)
{
	buf.write((char*)&m_byteTypeID, sizeof(int));
	buf.write((char*)&m_byteDim, sizeof(unsigned char));
	buf.write((char*)&m_nRefCount, sizeof(int));
	buf.write((char*)&m_nObjFlag, sizeof(int));

	int ll = 0;
	ll = m_sObjName.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_sObjName.c_str(), sizeof(char) * ll);
	}

	return true;
}

void GeometryImpl::write(std::ostream & f)
{
	bool flag = false;

	unsigned int nSize = this->sizeBuffer();
	f.write((char*)&nSize, sizeof(unsigned int));//下面对象的内存大小,写入文件

	char* pMem = new char[nSize];

	Buffer buf;
	buf.attach(pMem, nSize);
	flag = this->writeBuffer(buf);//将对象在内存中打包
	if (!flag) return;

	f.write((char*)pMem, nSize);//将打包好的对象一次性写入文件
}

void GeometryImpl::read(std::istream & f)
{
	bool flag = false;

	unsigned int nSize = 0;
	f.read((char*)&nSize, sizeof(unsigned int));//读入下面对象的内存大小

	char* pMem = new char[nSize];
	f.read(pMem, nSize);//把一个对象读近内存（包）

	Buffer buf;
	buf.attach(pMem, nSize);//创建了符合当前对象大小缓冲区,并使buf接管pMem开辟的内存：对应一个从文件中读入的对象
	flag = this->readBuffer(buf);//对内存包调用“解包”函数

	assert(flag);
}


//下列静态函数用于创建指定类型的几何对象
//根据传入的对象类型，新建一个相应几何体的指针，并开辟空间
GeometryImpl * GeometryImpl::create(int signal)
{
	if (signal == GEOTYPE_3D_TRIANGLEMESH)//11	
	{
		return dynamic_cast<GeometryImpl*>(new TriangleMesh3dImpl());
	}
	else if (signal == GEOTYPE_3D_POINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new Point3dImpl());
	}
	else if (signal == GEOTYPE_3D_SINGLEPOINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new SinglePoint3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOINT)//0
	{
		return dynamic_cast<GeometryImpl*>(new MultiPoint3dImpl());;
	}
	else if (signal == GEOTYPE_3D_ANNOTATION)//21
	{
		return dynamic_cast<GeometryImpl*>(new Annotation3dImpl());
	}
	else if (signal == GEOTYPE_3D_REFERENCE)//20
	{
		return dynamic_cast<GeometryImpl*>(new Reference3dImpl());
	}
	else if (signal == GEOTYPE_3D_VOLUME)//1
	{
		return dynamic_cast<GeometryImpl*>(new Volume3dImpl());
	}
	else if (signal == GEOTYPE_3D_SURFACE)//2
	{
		return dynamic_cast<GeometryImpl*>(new Surface3dImpl());
	}
	else if (signal == GEOTYPE_3D_CURVE)//3
	{
		return dynamic_cast<GeometryImpl*>(new Curve3dImpl());
	}
	else if (signal == GEOTYPE_3D_GROUP)//4
	{
		return dynamic_cast<GeometryImpl*>(new Group3dImpl());
	}
	else if (signal == GEOTYPE_3D_VOXEL)//5
	{
		return dynamic_cast<GeometryImpl*>(new Voxel3dImpl());
	}
	else if (signal == GEOTYPE_3D_SOLID)//6
	{
		return dynamic_cast<GeometryImpl*>(new Solid3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTISOLID)//7
	{
		return dynamic_cast<GeometryImpl*>(new MultiSolid3dImpl());
	}
	else if (signal == GEOTYPE_3D_PRISM)//8
	{
		return dynamic_cast<GeometryImpl*>(new Prism3dImpl());
	}
	else if (signal == GEOTYPE_3D_TETRAHEDRON)//9
	{
		return dynamic_cast<GeometryImpl*>(new Tetrahedron3dImpl());
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDSURFACE)//10
	{
		return dynamic_cast<GeometryImpl*>(new ParameterizedSurface3dImpl());
	}
	else if (signal == GEOTYPE_3D_TRIANGLE)//12
	{
		return dynamic_cast<GeometryImpl*>(new Triangle3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYGON)//13
	{
		return dynamic_cast<GeometryImpl*>(new MultiPolygon3dImpl());
	}
	else if (signal == GEOTYPE_3D_POLYGON)//14
	{
		return dynamic_cast<GeometryImpl*>(new Polygon3dImpl());
	}
	else if (signal == GEOTYPE_3D_LINEARRING)//15
	{
		return dynamic_cast<GeometryImpl*>(new LinearRing3dImpl());
	}
	else if (signal == GEOTYPE_3D_PARAMETERIZEDCURVE)//16
	{
		return dynamic_cast<GeometryImpl*>(new ParameterizedCurve3dImpl());
	}
	else if (signal == GEOTYPE_3D_LINESTRING)//17
	{
		return dynamic_cast<GeometryImpl*>(new LineString3dImpl());
	}
	else if (signal == GEOTYPE_3D_MULTIPOLYLINE)//18
	{
		return dynamic_cast<GeometryImpl*>(new MultiPolyline3dImpl());
	}
	else
	{
		return 0;
	}
	 
}


//新建几何对象（根据Buffer中的type实现对象指针的创建及内存的开辟）
//调用readBuffer实现给定内存包到创建对象的拷贝
//返回对象的指针
GeometryImpl * GeometryImpl::create(Buffer & buf)//从Buffer中读取数据，新建一个几何体
{
	//获得对象的 TypeID 以此判别是哪一类对象
	unsigned char signal = -1;
	//	char* p = (char*)buf.data() + buf.getPos();
	char* p = (char*)buf.data();
	memcpy((char*)&signal, p, sizeof(unsigned char));

	GeometryImpl* pGeometry = create(signal);
	if (!pGeometry)
		return pGeometry;

	//将给定的内存包buf 解包到相应对象中并返回对象指针

	if (true == pGeometry->readBuffer(buf))
		return pGeometry;
	else
	{
		delete pGeometry;
		return NULL;
	}
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace