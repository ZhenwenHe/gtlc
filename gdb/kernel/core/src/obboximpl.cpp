#include "internal_obboximpl.h"
#include "internal_spatialfuns.h"


begin_gtl_namespace
begin_gdb_namespace


double OBBoxImpl::Length() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}


double OBBoxImpl::Width() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}

double OBBoxImpl::Height() const
{
	// TODO: Add your specialized code here.
	return (double)0;
}


Vertex3d OBBoxImpl::CenterPoint() const
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return Vertex3d(0, 0, 0);
}



int OBBoxImpl::IsOBBoxIntersect(const OBBoxImpl& obb) const
{
	// TODO: Add your specialized code here.
	// NOTE: Requires a correct return value to compile.
	return 1;
}
end_gdb_namespace
end_gtl_namespace


