#include <stdlib.h>
#include <stdio.h> 
#include "config.h"
#include "globalfuns.h"

begin_gtl_namespace
begin_gdb_namespace
/*
[SpatialReferenceConfiguration]
SRS_USE_ETMERC = TRUE, SRS_USE_PROJ4 = TRUE, SRS_OVERRIDE_PROJ_DATUM_WITH_TOWGS84 = YES
*/
std::string Config::srsConfiguration;

std::string  & Config::getSpatialReferenceConfiguration() {
	return srsConfiguration;
}

//获取设置的SR的对应字段的值，如果没有则返回默认值
std::string Config::getSRConfigOptionValue(const char * name, const char * defaultValue) {

	std::string  stmp = Config::getSpatialReferenceConfiguration();
	CSTRLIST  csltemp = cslTokenizeString(stmp.c_str(), ",", true, false);
	int i = cslIFindName(csltemp, name);
	if (i >= 0) {
		stmp = cslParseNameValue(csltemp[i], 0);
		cslDestroy(csltemp);
		if (defaultValue)
			stmp = defaultValue;
		return stmp;
	}
	else {
		cslDestroy(csltemp);
		if (defaultValue)
			stmp = defaultValue;
		return stmp;
	}

}
end_gdb_namespace
end_gtl_namespace