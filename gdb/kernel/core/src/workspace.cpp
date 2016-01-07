#include "internal_workspaceimpl.h"



begin_gtl_namespace
begin_gdb_namespace
 
WorkspaceSharedPtr Workspace::create(){
	return WorkspaceSharedPtr(new WorkspaceImpl());
}
end_gdb_namespace
end_gtl_namespace


