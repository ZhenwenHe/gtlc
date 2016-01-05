#include "internal_topoitemimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

TopoItemSharedPtr TopoItem::newTopoItem(){
	return TopoItemSharedPtr(new TopoItemImpl());
}

TopoItemSharedPtr TopoItem::newTopoItem(int topoType, FID fidMain, FCLSID fclsidMain, FID fidSub, FCLSID fclsidSub){
	return TopoItemSharedPtr(new TopoItemImpl(topoType, fidMain, fclsidMain, fidSub, fclsidSub));
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
