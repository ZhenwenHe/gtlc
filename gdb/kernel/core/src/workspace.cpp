#include "internal_workspaceimpl.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
 
WorkspaceSharedPtr Workspace::create(){
	return WorkspaceSharedPtr(new WorkspaceImpl());
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
