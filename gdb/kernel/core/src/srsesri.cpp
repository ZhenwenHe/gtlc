#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace
bool      SpatialReference::morphToESRI() {
	return false;
}
bool      SpatialReference::morphFromESRI() {
	return false;
}
bool      SpatialReference::importFromESRIStatePlaneWKT(
	int nCode, const char* pszDatumName, const char* pszUnitsName,
	int nPCSCode, const char* pszCSName  )
{
	return false;
}
bool      SpatialReference::importFromESRIWisconsinWKT(
	const char* pszPrjName, double dfCentralMeridian, double dfLatOfOrigin,
	const char* pszUnitsName, const char* pszCSName  ) {
	return false;
}
end_gdb_namespace
end_gtl_namespace