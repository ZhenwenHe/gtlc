#include "internal_topoinfoimpl.h"
#include "internal_semanticgeometrytopoinfoimpl.h"
#include "internal_semanticactiontopoinfoimpl.h"


begin_gtl_namespace
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
end_gtl_namespace


