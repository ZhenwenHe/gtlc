#include "internal_point3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

Point3dImpl::Point3dImpl()
{
	m_byteTypeID = GEOTYPE_3D_POINT;
}
Point3dImpl::~Point3dImpl()
{

}

bool Point3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;

	flag = Geometry3dImpl::readBuffer(buf);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Point3dImpl::writeBuffer(Buffer& buf)
{
	int flag = 0;

	flag = Geometry3dImpl::writeBuffer(buf);

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Point3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Geometry3dImpl::sizeBuffer();

	//专有信息
	//*************************************************


	return nSize;
}

void Point3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Geometry3dImpl::TranslateFrom(m4x4);
}

GeometryImpl* Point3dImpl::CreateObject()
{
	return new Point3dImpl;
}

//计算当前对象的几何重心
Vertex3d Point3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	return corePt;
}

// 释放对象数据并将指针和变量赋初始值
bool Point3dImpl::Init()
{
	return Geometry3dImpl::Init();
}


end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

