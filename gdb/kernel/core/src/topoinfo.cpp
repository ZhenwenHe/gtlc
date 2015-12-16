#include "internal_TopoInfoImpl.h"
#include "internal_SemanticGeometryTopoInfoImpl.h"
#include "internal_SemanticActionTopoInfoImpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

TopoInfoSharedPtr  TopoInfo::newTopoInfo(int nType)
{
	switch (nType)
	{
	case TOPOINFOTYPE_SEMATIC_GEOMETRY:
	{
		return TopoInfoSharedPtr(new SematicGeometryTopoInfoImpl(make_pair(0, 0)));
	}
	break;
	case TOPOINFOTYPE_SEMATIC_ACTION:
	{
		return TopoInfoSharedPtr(new SemanticActionTopoInfoImpl(make_pair(0, 0)));
	}
	break;
	}
	return TopoInfoSharedPtr();
}
TopoInfoSharedPtr  TopoInfo::newTopoInfo(Buffer & buf)
{
	return TopoInfoSharedPtr();
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
