#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace

bool SpatialReference::importFromEPSG(int v) {
	return false;
}
bool SpatialReference::importFromEPSGA(int v) {
	return false;
}

int  SpatialReference::EPSGTreatsAsLatLong() {
	return 0;
}
int  SpatialReference::EPSGTreatsAsNorthingEasting() {
	return 0;
}

bool      SpatialReference::autoIdentifyEPSG() {
	return 0;
}
int         SpatialReference::getEPSGGeogCS() {
	return 0;
}
bool     SpatialReference::setStatePlane(int nZone, int bNAD83,
	const char *pszOverrideUnitName,
	double dfOverrideUnit) {
	return 0;
}
end_gdb_namespace
end_gtl_namespace