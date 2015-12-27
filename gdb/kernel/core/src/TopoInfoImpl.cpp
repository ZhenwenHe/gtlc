#include "internal_topoinfoimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
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
end_cug_namespace
end_edu_namespace
end_cn_namespace
