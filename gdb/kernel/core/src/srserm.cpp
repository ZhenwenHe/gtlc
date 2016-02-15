#include "spatialreference.h" 
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace 
/************************************************************************/
/*                           importFromERM()                            */
/************************************************************************/

/**
* Create OGR WKT from ERMapper projection definitions.
*
* Generates an SpatialReference definition from an ERMapper datum
* and projection name.  Based on the ecw_cs.wkt dictionary file from
* gdal/data.
*
* @param pszProj the projection name, such as "NUTM11" or "GEOGRAPHIC".
* @param pszDatum the datum name, such as "NAD83".
* @param pszUnits the linear units "FEET" or "METERS".
*
* @return true on success or false if not found.
*/

bool SpatialReference::importFromERM(const char *pszProj,
	const char *pszDatum,
	const char *pszUnits)

{
	clear();

	/* -------------------------------------------------------------------- */
	/*      do we have projection and datum?                                */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(pszProj, "RAW"))
		return true;

	/* -------------------------------------------------------------------- */
	/*      Do we have an EPSG coordinate system?                           */
	/* -------------------------------------------------------------------- */

	if (cslNIEqualString(pszProj, "EPSG:", 5))
		return importFromEPSG(atoi(pszProj + 5));


	if (cslNIEqualString(pszDatum, "EPSG:", 5))
		return importFromEPSG(atoi(pszDatum + 5));

	/* -------------------------------------------------------------------- */
	/*      Set projection if we have it.                                   */
	/* -------------------------------------------------------------------- */
	bool eErr;

	if (cslIEqualString(pszProj, "GEODETIC"))
	{
	}
	else
	{
		eErr = importFromDict("ecw_cs.wkt", pszProj);
		if (eErr != true)
			return eErr;

		if (cslIEqualString(pszUnits, "FEET"))
			setLinearUnits(SRS_UL_US_FOOT, atof(SRS_UL_US_FOOT_CONV));
		else
			setLinearUnits(SRS_UL_METER, 1.0);
	}

	/* -------------------------------------------------------------------- */
	/*      Set the geogcs.                                                 */
	/* -------------------------------------------------------------------- */
	SpatialReference oGeogCS;

	eErr = oGeogCS.importFromDict("ecw_cs.wkt", pszDatum);
	if (eErr != true)
	{
		clear();
		return eErr;
	}

	if (!isLocal())
		copyGeogCSFrom(&oGeogCS);

	return true;
}
 
/************************************************************************/
/*                            exportToERM()                             */
/************************************************************************/

/**
* Convert coordinate system to ERMapper format.
*
* @param pszProj 32 character buffer to receive projection name.
* @param pszDatum 32 character buffer to recieve datum name.
* @param pszUnits 32 character buffer to receive units name.
*
* @return true on success, OGRERR_SRS_UNSUPPORTED if not translation is
* found, or OGRERR_FAILURE on other failures.
*/

bool SpatialReference::exportToERM(char *pszProj, char *pszDatum,char *pszUnits)
{
	strcpy(pszProj, "RAW");
	strcpy(pszDatum, "RAW");
	strcpy(pszUnits, "METERS");

	if (!isProjected() && !isGeographic())
		return TRUE;

	/* -------------------------------------------------------------------- */
	/*      Try to find the EPSG code.                                      */
	/* -------------------------------------------------------------------- */
	int nEPSGCode = 0;

	if (isProjected())
	{
		const char *pszAuthName = getAuthorityName("PROJCS");

		if (pszAuthName != NULL && cslIEqualString(pszAuthName, "epsg"))
		{
			nEPSGCode = atoi(getAuthorityCode("PROJCS"));
		}
	}
	else if (isGeographic())
	{
		const char *pszAuthName = getAuthorityName("GEOGCS");

		if (pszAuthName != NULL && cslIEqualString(pszAuthName, "epsg"))
		{
			nEPSGCode = atoi(getAuthorityCode("GEOGCS"));
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Is our GEOGCS name already defined in ecw_cs.dat?               */
	/* -------------------------------------------------------------------- */
	SpatialReference oSRSWork;
	const char *pszWKTDatum = getAttrValue("DATUM");

	if (pszWKTDatum != NULL
		&& oSRSWork.importFromDict("ecw_cs.wkt", pszWKTDatum) == true)
	{
		strncpy(pszDatum, pszWKTDatum, 32);
		pszDatum[31] = '\0';
	}

	/* -------------------------------------------------------------------- */
	/*      Is this a "well known" geographic coordinate system?            */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(pszDatum, "RAW"))
	{
		int nEPSGGCSCode = getEPSGGeogCS();

		if (nEPSGGCSCode == 4326)
			strcpy(pszDatum, "WGS84");

		else if (nEPSGGCSCode == 4322)
			strcpy(pszDatum, "WGS72DOD");

		else if (nEPSGGCSCode == 4267)
			strcpy(pszDatum, "NAD27");

		else if (nEPSGGCSCode == 4269)
			strcpy(pszDatum, "NAD83");

		else if (nEPSGGCSCode == 4277)
			strcpy(pszDatum, "OSGB36");

		else if (nEPSGGCSCode == 4278)
			strcpy(pszDatum, "OSGB78");

		else if (nEPSGGCSCode == 4201)
			strcpy(pszDatum, "ADINDAN");

		else if (nEPSGGCSCode == 4202)
			strcpy(pszDatum, "AGD66");

		else if (nEPSGGCSCode == 4203)
			strcpy(pszDatum, "AGD84");

		else if (nEPSGGCSCode == 4209)
			strcpy(pszDatum, "ARC1950");

		else if (nEPSGGCSCode == 4210)
			strcpy(pszDatum, "ARC1960");

		else if (nEPSGGCSCode == 4275)
			strcpy(pszDatum, "NTF");

		else if (nEPSGGCSCode == 4283)
			strcpy(pszDatum, "GDA94");

		else if (nEPSGGCSCode == 4284)
			strcpy(pszDatum, "PULKOVO");
	}

	/* -------------------------------------------------------------------- */
	/*      Are we working with a geographic (geodetic) coordinate system?  */
	/* -------------------------------------------------------------------- */

	if (isGeographic())
	{
		if (cslIEqualString(pszDatum, "RAW"))
			return false;
		else
		{
			strcpy(pszProj, "GEODETIC");
			return true;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Is this a UTM projection?                                       */
	/* -------------------------------------------------------------------- */
	int bNorth, nZone;

	nZone = getUTMZone(&bNorth);
	if (nZone > 0)
	{
		if (cslIEqualString(pszDatum, "GDA94") && !bNorth && nZone >= 48 && nZone <= 58)
		{
			sprintf(pszProj, "MGA%02d", nZone);
		}
		else
		{
			if (bNorth)
				sprintf(pszProj, "NUTM%02d", nZone);
			else
				sprintf(pszProj, "SUTM%02d", nZone);
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Is our PROJCS name already defined in ecw_cs.dat?               */
	/* -------------------------------------------------------------------- */
	else
	{
		const char *pszPROJCS = getAttrValue("PROJCS");

		if (pszPROJCS != NULL
			&& oSRSWork.importFromDict("ecw_cs.wkt", pszPROJCS) == true
			&& oSRSWork.isProjected())
		{
			strncpy(pszProj, pszPROJCS, 32);
			pszProj[31] = '\0';
		}
	}

	/* -------------------------------------------------------------------- */
	/*      If we have not translated it yet, but we have an EPSG code      */
	/*      then use EPSG:n notation.                                       */
	/* -------------------------------------------------------------------- */
	if ((cslIEqualString(pszDatum, "RAW") || cslIEqualString(pszProj, "RAW")) && nEPSGCode != 0)
	{
		sprintf(pszProj, "EPSG:%d", nEPSGCode);
		sprintf(pszDatum, "EPSG:%d", nEPSGCode);
	}

	/* -------------------------------------------------------------------- */
	/*      Handle the units.                                               */
	/* -------------------------------------------------------------------- */
	double dfUnits = getLinearUnits();

	if (fabs(dfUnits - 0.3048) < 0.0001)
		strcpy(pszUnits, "FEET");
	else
		strcpy(pszUnits, "METERS");

	if (cslIEqualString(pszProj, "RAW"))
		return false;
	else
		return true;
}

end_gdb_namespace
end_gtl_namespace