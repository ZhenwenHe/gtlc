#include "internal_semanticgeometrytopoinfoimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

bool SematicGeometryTopoInfoImpl::readBuffer(Buffer & buf)
{
	buf.read((void*)(&m_originalItem.first), sizeof(FID));
	buf.read((void*)(&m_originalItem.second), sizeof(FCLSID));
	buf.read((void*)(&m_type), sizeof(m_type));
	unsigned int  _size = 0;//(0);
	buf.read((void*)(&_size), sizeof(unsigned int));
	char* pName = new char[_size];
	buf.read((void*)(pName), _size);
	m_sematicName = string(pName, _size);
	Buffer geoBuf;
	unsigned int _sizeGeo = 0;//(0);
	buf.read((void*)(&_sizeGeo), sizeof(unsigned int));
	if (_sizeGeo > 0)
	{
		char* pGeo = new char[_sizeGeo];
		buf.read(pGeo, _sizeGeo);
		geoBuf.attach((void*)pGeo, _sizeGeo);
		int iGeoType = 0;
		geoBuf.read((char*)&iGeoType, sizeof(int));
		m_geometry = Geometry::create(iGeoType);
		m_geometry->readBuffer(geoBuf);
	}

	return true;
}
bool SematicGeometryTopoInfoImpl::writeBuffer(Buffer& buf)
{
	size_t _buffSize = sizeBuffer();
	char*pBuf = new char[_buffSize];
	buf.attach((void*)pBuf, _buffSize);
	buf.write((void*)(&m_originalItem.first), sizeof(FID));
	buf.write((void*)(&m_originalItem.second), sizeof(FCLSID));
	buf.write((void*)(&m_type), sizeof(m_type));
	unsigned int _size = m_sematicName.size();
	buf.write((void*)(&_size), sizeof(unsigned int));
	buf.write((void*)(m_sematicName.c_str()), _size);
	if (m_geometry)
	{
		Buffer geoBuf;
		m_geometry->writeBuffer(geoBuf);
		size_t _sizeGeo = m_geometry->sizeBuffer();
		buf.write((void*)(&_sizeGeo), sizeof(unsigned int));
		buf.write(geoBuf.data(), geoBuf.size());
	}
	else
	{
		size_t _sizeGeo(0);
		buf.write((void*)(&_sizeGeo), sizeof(unsigned int));
	}
	return true;
}
void SematicGeometryTopoInfoImpl::read(std::istream & f)
{
	f.read((char*)(&m_originalItem.first), sizeof(FID));
	f.read((char*)(&m_originalItem.second), sizeof(FCLSID));
	f.read((char*)(&m_type), sizeof(m_type));
	unsigned int _size = 0;//(0);
	f.read((char*)(&_size), sizeof(unsigned int));
	char* pName = new char[_size];
	f.read((char*)(pName), _size);
	m_sematicName = string(pName, _size);
	//以前采用geomety直接读文件流的方式出错，采用这种一次提取再读buffer的方式-PY-ADD-2010-05-11
	size_t s(0);
	f.read((char*)(&s), sizeof(unsigned int));
	if (s > 0) {
		int nTypeID = 0;
		f.read((char*)(&nTypeID), sizeof(int));
		GeometrySharedPtr pGeo = Geometry::create(nTypeID);
		char *pc = new char[s];
		f.seekg(-(sizeof(int)), ios::cur);//还原读过的typeID长度
		f.read(pc, s);
		Buffer bufGeo;
		bufGeo.attach(pc, s);
		pGeo->readBuffer(bufGeo);
		this->setGeometry(pGeo);
	}
}
void SematicGeometryTopoInfoImpl::write(std::ostream & f)
{
	f.write((char*)(&m_originalItem.first), sizeof(FID));
	f.write((char*)(&m_originalItem.second), sizeof(FCLSID));
	f.write((char*)(&m_type), sizeof(m_type));
	unsigned int _size = m_sematicName.size();
	f.write((char*)(&_size), sizeof(unsigned int));
	f.write((char*)(m_sematicName.c_str()), _size);
	//以前采用geomety直接写文件流的方式出错，采用这种写buffer再写文件的方式-PY-ADD-2010-05-11
	size_t s(0);
	if (m_geometry)
	{
		s = m_geometry->sizeBuffer();
		f.write((char*)(&s), sizeof(unsigned int));
		Buffer geoBuf;
		m_geometry->writeBuffer(geoBuf);
		f.write((char*)geoBuf.data(), geoBuf.size());
	}
	else
	{
		f.write((char*)(&s), sizeof(unsigned int));
	}
}
bool SematicGeometryTopoInfoImpl::copy(TopoInfoImpl *topoInfo)
{
	TopoInfoImpl::copy(topoInfo);
	SematicGeometryTopoInfoImpl * _info = dynamic_cast<SematicGeometryTopoInfoImpl*>(topoInfo);
	m_sematicName = _info->getSemanticName();
	if (_info->getGeometry())
	{
		m_geometry->copy(_info->getGeometry());
	}
	return true;
}
size_t SematicGeometryTopoInfoImpl::sizeBuffer()
{
	size_t _sizeBuf(0);
	_sizeBuf = sizeof(FID)
		+ sizeof(FCLSID)
		+ sizeof(unsigned int) //space of size of m_sematicName
		+ m_sematicName.size()
		+ sizeof(bool);//space of size of m_geometry
	if (m_geometry)
	{
		_sizeBuf += m_geometry->sizeBuffer();
	}
	return _sizeBuf;
}
SematicGeometryTopoInfoImpl::~SematicGeometryTopoInfoImpl()
{
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
