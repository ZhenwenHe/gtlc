#include "internal_topoitemimpl.h"


begin_gtl_namespace
begin_gdb_namespace

TopoItemSharedPtr TopoItem::newTopoItem(){
	return TopoItemSharedPtr(new TopoItemImpl());
}

TopoItemSharedPtr TopoItem::newTopoItem(int topoType, FID fidMain, FCLSID fclsidMain, FID fidSub, FCLSID fclsidSub){
	return TopoItemSharedPtr(new TopoItemImpl(topoType, fidMain, fclsidMain, fidSub, fclsidSub));
}

end_gdb_namespace
end_gtl_namespace


