#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace
//derived from gdal 1.10
//程序需要的数据放置在gtl::getDataHome()返回的目录下，所以这里的字典名称只需要数据文件名即可
//具体的格式参见epsg.wkt,esri_extra.wkt,cubewerx_extra.wkt
//这些文件中每行最大的字符数<=1024*1024=1Mb
#define MAX_LINE_CHARS 1048576

bool SpatialReference::importFromDict(const char *pszDict, const char *pszCode)
{ 
	bool eErr = false;

	/* -------------------------------------------------------------------- */
	/*      open file.                                             */
	/* -------------------------------------------------------------------- */ 
	std::locale _loc = std::locale::global(std::locale(""));
	string sz = gtl::getDataHome()+"\\";
	sz = sz + pszDict;
	std::ifstream iFstreamDict(sz.c_str(), std::ios_base::in);
	/* -------------------------------------------------------------------- */
	/*      Process lines.                                                  */
	/* -------------------------------------------------------------------- */
	char pszLine[MAX_LINE_CHARS];
	
	
	while (!iFstreamDict.eof())
	{
		iFstreamDict.getline(pszLine, MAX_LINE_CHARS);
		if (pszLine[0] == '#') {
			/*注释行， do nothing */
			continue;
		}
		else if (cslNIEqualString(pszLine, "include ", 8))
		{
			eErr = importFromDict(pszLine + 8, pszCode);
			if (eErr != false)
				break;
		}
		else if (strstr(pszLine, ",") == NULL) {
			/* do nothing */
			continue;
		}
		else if (cslNIEqualString(pszLine, pszCode, strlen(pszCode))
			&& pszLine[strlen(pszCode)] == ',')
		{
			char *pszWKT = (char *)pszLine + strlen(pszCode) + 1;

			eErr = importFromWkt(&pszWKT);
			break;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Cleanup                                                         */
	/* -------------------------------------------------------------------- */
	iFstreamDict.close();
	return eErr;
}
end_gdb_namespace
end_gtl_namespace