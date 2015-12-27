#include "internal_sharedmodelimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

SharedModelImpl::SharedModelImpl()
{

}

SharedModelImpl::SharedModelImpl(Identifier::raw_type  mdlid, string mdlname, int mdltype)
{
	m_mdlid = mdlid;
	m_mdlname = mdlname;
	m_mdltype = mdltype;
	m_linkmatids.clear();
}

SharedModelImpl::~SharedModelImpl()
{
	/*if(m_geom)
	{
	delete m_geom;
	}*/
}


bool SharedModelImpl::writeBuffer(Buffer& buf)
{
	buf.write((char*)&m_mdlid, sizeof(Identifier::raw_type));

	int ll = 0;
	ll = m_mdlname.length();
	buf.write((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		buf.write((char*)m_mdlname.c_str(), sizeof(char) * ll);
	}

	buf.write((char*)&m_mdltype, sizeof(int));

	int nCount = m_linkmatids.size();
	buf.write((char*)&nCount, sizeof(int));
	if (nCount > 0)
	{
		buf.write((char*)&m_linkmatids[0], sizeof(Identifier::raw_type) * nCount);
	}

	return true;
}

bool SharedModelImpl::readBuffer(Buffer& buf)
{
	buf.read((char*)&m_mdlid, sizeof(Identifier::raw_type));

	int ll = 0;
	buf.read((char*)&ll, sizeof(int));
	if (ll > 0)
	{
		char tempbuf[50];
		buf.read(tempbuf, sizeof(char) * ll);
		tempbuf[ll] = '\0';
		m_mdlname = tempbuf;
	}

	buf.read((char*)&m_mdltype, sizeof(int));

	int nCount = 0;
	buf.read((char*)&nCount, sizeof(int));
	for (int i = 0; i < nCount; i++)
	{
		Identifier::raw_type thevalue;
		buf.read((char*)&thevalue, sizeof(Identifier::raw_type));
		m_linkmatids.push_back(thevalue);
	}

	return true;
}

size_t SharedModelImpl::sizeBuffer()
{
	size_t nSize = 0;

	nSize += sizeof(Identifier::raw_type); //Identifier::raw_type m_mdlid;

	nSize += sizeof(int);                	//string m_mdlname;
	int charNum = m_mdlname.length();
	nSize += sizeof(char) * charNum;

	nSize += sizeof(int);    //int m_mdltype;

	nSize += sizeof(int);     //vector<int>的元素个数
	nSize += sizeof(Identifier::raw_type) * m_linkmatids.size();   //vector<int> m_linkmatids;

	return nSize;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace

