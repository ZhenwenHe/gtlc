#include "internal_TopoItemImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace 
 

TopoItemImpl::TopoItemImpl() : nType(0), fidItemMain(0, 0), fidItemSub(0, 0) 	
{
}
TopoItemImpl::TopoItemImpl(int topoType, FID fidMain, FCLSID fclsidMain, FID fidSub, FCLSID fclsidSub)
{
	nType = topoType;
	fidItemMain.first = fidMain;
	fidItemMain.second = fclsidMain;
	fidItemSub.first = fidSub;
	fidItemSub.second = fclsidSub;
}
 

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace