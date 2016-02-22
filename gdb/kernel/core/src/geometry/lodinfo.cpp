#include "feature.h"
#include "internal_loadinfoimpl.h"


begin_gtl_namespace
begin_gdb_namespace

LODInfoSharedPtr LODInfo::create(){
	return LODInfoSharedPtr(new LODInfoImpl());
}
end_gdb_namespace
end_gtl_namespace


