#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace

bool  SpatialReference::exportToPCI(char **, char **, double **) const {
	return false;
}
bool   SpatialReference::importFromPCI(const char *, const char * v1 ,double * v2 ) {
	return false;
}
end_gdb_namespace
end_gtl_namespace