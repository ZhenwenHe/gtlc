#include "internal_voxel3dimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

bool Voxel3dImpl::readBuffer(Buffer & buf)
{
	int flag = 0;
	flag = Volume3dImpl::readBuffer(buf);

	buf.read((char*)&m_iVolumeTypeID, sizeof(unsigned char));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool Voxel3dImpl::writeBuffer(Buffer& buf)
{

	int flag = 0;
	flag = Volume3dImpl::writeBuffer(buf);

	buf.write((char*)&m_iVolumeTypeID, sizeof(unsigned char));

	if (flag)
	{
		return true;
	}
	else
	{
		return false;
	}
}

size_t Voxel3dImpl::sizeBuffer()
{
	size_t nSize = 0;

	//共有信息(Geometry，3DGeometry)
	//*************************************************

	nSize += Volume3dImpl::sizeBuffer();

	//专有信息
	//*************************************************

	nSize += sizeof(unsigned char); // unsigned char m_iVolumeTypeID

	//*************************************************

	return nSize;
}

void Voxel3dImpl::TranslateFrom(Matrix4x4& m4x4)
{
	Volume3dImpl::TranslateFrom(m4x4);
}

GeometryImpl* Voxel3dImpl::CreateObject()
{
	return new Voxel3dImpl;
}

//计算当前对象的几何重心
Vertex3d Voxel3dImpl::CalCorePoint()
{
	Vertex3d corePt;
	return corePt;
}

// 释放对象数据并将指针和变量赋初始值
bool Voxel3dImpl::Init()
{
	bool res = Volume3dImpl::Init();
	if (false == res) return false;

	m_iVolumeTypeID = -1;

	return true;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace