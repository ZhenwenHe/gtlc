#include "internal_topoinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

TopoInfoImpl::TopoInfoImpl(FIDITEM fiditem, int type) :m_originalItem(fiditem), m_type(type)
{
}
bool TopoInfoImpl::copy(TopoInfoImpl * topoInfo)
{
	m_originalItem = topoInfo->getOriginalItem();
	m_type = topoInfo->getType();
	return true;
}
TopoInfoImpl::~TopoInfoImpl()
{
}

end_gdb_namespace
end_gtl_namespace


