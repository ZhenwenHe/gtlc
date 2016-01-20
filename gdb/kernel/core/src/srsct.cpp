#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace
void CoordinateTransformer::destroy(CoordinateTransformer* poCT)
{
	delete poCT;
}

CoordinateTransformer* CoordinateTransformer::create(SpatialReference *s, SpatialReference * t)
{
	CoordinateTransformer* poCT = 0;
	return  poCT;
}
end_gdb_namespace
end_gtl_namespace


