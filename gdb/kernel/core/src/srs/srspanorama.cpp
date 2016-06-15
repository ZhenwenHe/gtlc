#include "spatialreference.h" 
//derived from gdal
#define TO_DEGREES 57.2957795130823208766
#define TO_RADIANS 0.017453292519943295769

// XXX: this macro computes zone number from the central meridian parameter.
// Note, that "Panorama" parameters are set in radians.
// In degrees it means formulae:
//
//              zone = (central_meridian + 3) / 6
//
#define TO_ZONE(x) (((x) + 0.05235987755982989) / 0.1047197551196597 + 0.5)

/************************************************************************/
/*  "Panorama" projection codes.                                        */
/************************************************************************/

#define PAN_PROJ_NONE   -1L
#define PAN_PROJ_TM     1L      // Gauss-Kruger (Transverse Mercator)
#define PAN_PROJ_LCC    2L      // Lambert Conformal Conic 2SP
#define PAN_PROJ_STEREO 5L      // Stereographic
#define PAN_PROJ_AE     6L      // Azimuthal Equidistant (Postel)
#define PAN_PROJ_MERCAT 8L      // Mercator
#define PAN_PROJ_POLYC  10L     // Polyconic
#define PAN_PROJ_PS     13L     // Polar Stereographic
#define PAN_PROJ_GNOMON 15L     // Gnomonic
#define PAN_PROJ_UTM    17L     // Universal Transverse Mercator (UTM)
#define PAN_PROJ_WAG1   18L     // Wagner I (Kavraisky VI)
#define PAN_PROJ_MOLL   19L     // Mollweide
#define PAN_PROJ_EC     20L     // Equidistant Conic
#define PAN_PROJ_LAEA   24L     // Lambert Azimuthal Equal Area
#define PAN_PROJ_EQC    27L     // Equirectangular
#define PAN_PROJ_CEA    28L     // Cylindrical Equal Area (Lambert)
#define PAN_PROJ_IMWP   29L     // International Map of the World Polyconic

/************************************************************************/
/*  "Panorama" datum codes.                                             */
/************************************************************************/

#define PAN_DATUM_NONE      -1L
#define PAN_DATUM_PULKOVO42 1L  // Pulkovo 1942
#define PAN_DATUM_WGS84     2L  // WGS84

/************************************************************************/
/*  "Panorama" ellipsod codes.                                          */
/************************************************************************/

#define PAN_ELLIPSOID_NONE          -1L
#define PAN_ELLIPSOID_KRASSOVSKY    1L  // Krassovsky, 1940
#define PAN_ELLIPSOID_WGS72         2L  // WGS, 1972
#define PAN_ELLIPSOID_INT1924       3L  // International, 1924 (Hayford, 1909)
#define PAN_ELLIPSOID_CLARCKE1880   4L  // Clarke, 1880
#define PAN_ELLIPSOID_CLARCKE1866   5L  // Clarke, 1866 (NAD1927)
#define PAN_ELLIPSOID_EVEREST1830   6L  // Everest, 1830
#define PAN_ELLIPSOID_BESSEL1841    7L  // Bessel, 1841
#define PAN_ELLIPSOID_AIRY1830      8L  // Airy, 1830
#define PAN_ELLIPSOID_WGS84         9L  // WGS, 1984 (GPS)
begin_gtl_namespace
begin_gdb_namespace
extern bool
getEllipsoidInfo(int nCode, char ** ppszName,
	double * pdfSemiMajor, double * pdfInvFlattening);

/************************************************************************/
/*  Correspondence between "Panorama" and EPSG datum codes.             */
/************************************************************************/

static const long aoDatums[] =
{
	0,
	4284,   // Pulkovo, 1942
	4326,   // WGS, 1984,
	4277,   // OSGB 1936 (British National Grid)
	0,
	0,
	0,
	0,
	0,
	4200    // Pulkovo, 1995
};

#define NUMBER_OF_DATUMS        (long)(sizeof(aoDatums)/sizeof(aoDatums[0]))

/************************************************************************/
/*  Correspondence between "Panorama" and EPSG ellipsoid codes.         */
/************************************************************************/

static const long aoEllips[] =
{
	0,
	7024,   // Krassovsky, 1940
	7043,   // WGS, 1972
	7022,   // International, 1924 (Hayford, 1909)
	7034,   // Clarke, 1880
	7008,   // Clarke, 1866 (NAD1927)
	7015,   // Everest, 1830
	7004,   // Bessel, 1841
	7001,   // Airy, 1830
	7030    // WGS, 1984 (GPS)
};

#define NUMBER_OF_ELLIPSOIDS    (sizeof(aoEllips)/sizeof(aoEllips[0]))


/************************************************************************/
/*                          importFromPanorama()                        */
/************************************************************************/

/**
* Import coordinate system from "Panorama" GIS projection definition.
*
* This method will import projection definition in style, used by
* "Panorama" GIS.
*
* This function is the equivalent of the C function OSRImportFromPanorama().
*
* @param iProjSys Input projection system code, used in GIS "Panorama".
*
*      <h4>Supported Projections</h4>
* <pre>
*      1:  Gauss-Kruger (Transverse Mercator)
*      2:  Lambert Conformal Conic 2SP
*      5:  Stereographic
*      6:  Azimuthal Equidistant (Postel)
*      8:  Mercator
*      10: Polyconic
*      13: Polar Stereographic
*      15: Gnomonic
*      17: Universal Transverse Mercator (UTM)
*      18: Wagner I (Kavraisky VI)
*      19: Mollweide
*      20: Equidistant Conic
*      24: Lambert Azimuthal Equal Area
*      27: Equirectangular
*      28: Cylindrical Equal Area (Lambert)
*      29: International Map of the World Polyconic
* </pre>
*
* @param iDatum Input coordinate system.
*
*      <h4>Supported Datums</h4>
* <pre>
*       1: Pulkovo, 1942
*       2: WGS, 1984
*       3: OSGB 1936 (British National Grid)
*       9: Pulkovo, 1995
* </pre>
*
* @param iEllips Input spheroid.
*
*      <h4>Supported Spheroids</h4>
* <pre>
*       1: Krassovsky, 1940
*       2: WGS, 1972
*       3: International, 1924 (Hayford, 1909)
*       4: Clarke, 1880
*       5: Clarke, 1866 (NAD1927)
*       6: Everest, 1830
*       7: Bessel, 1841
*       8: Airy, 1830
*       9: WGS, 1984 (GPS)
* </pre>
*
* @param padfPrjParams Array of 8 coordinate system parameters:
*
* <pre>
*      [0]  Latitude of the first standard parallel (radians)
*      [1]  Latitude of the second standard parallel (radians)
*      [2]  Latitude of center of projection (radians)
*      [3]  Longitude of center of projection (radians)
*      [4]  Scaling factor
*      [5]  False Easting
*      [6]  False Northing
*      [7]  Zone number
* </pre>
*
* Particular projection uses different parameters, unused ones may be set to
* zero. If NULL supplied instead of array pointer default values will be used
* (i.e., zeroes).
*
* @return true on success or an error code in case of failure.
*/

bool SpatialReference::importFromPanorama(long iProjSys, long iDatum,
	long iEllips,
	double *padfPrjParams)

{
	clear();

	/* -------------------------------------------------------------------- */
	/*      Use safe defaults if projection parameters are not supplied.    */
	/* -------------------------------------------------------------------- */
	int     bProjAllocated = FALSE;

	if (padfPrjParams == NULL)
	{
		int     i;

		padfPrjParams = (double *)malloc(8 * sizeof(double));
		if (!padfPrjParams)
			return false;
		for (i = 0; i < 7; i++)
			padfPrjParams[i] = 0.0;
		bProjAllocated = TRUE;
	}

	/* -------------------------------------------------------------------- */
	/*      Operate on the basis of the projection code.                    */
	/* -------------------------------------------------------------------- */
	switch (iProjSys)
	{
	case PAN_PROJ_NONE:
		break;

	case PAN_PROJ_UTM:
	{
		long nZone;

		if (padfPrjParams[7] == 0.0)
			nZone = (long)TO_ZONE(padfPrjParams[3]);
		else
			nZone = (long)padfPrjParams[7];

		// XXX: no way to determine south hemisphere. Always assume
		// nothern hemisphere.
		setUTM(nZone, TRUE);
	}
	break;

	case PAN_PROJ_WAG1:
		setWagner(1, 0.0,
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_MERCAT:
		setMercator(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[4],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_PS:
		setPS(TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[4],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_POLYC:
		setPolyconic(TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_EC:
		setEC(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[1],
			TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_LCC:
		setLCC(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[1],
			TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_TM:
	{
		// XXX: we need zone number to compute false easting
		// parameter, because usually it is not contained in the
		// "Panorama" projection definition.
		// FIXME: what to do with negative values?
		long    nZone;
		double  dfCenterLong;

		if (padfPrjParams[7] == 0.0)
		{
			nZone = (long)TO_ZONE(padfPrjParams[3]);
			dfCenterLong = TO_DEGREES * padfPrjParams[3];
		}
		else
		{
			nZone = (long)padfPrjParams[7];
			dfCenterLong = 6 * nZone - 3;
		}

		padfPrjParams[5] = nZone * 1000000.0 + 500000.0;
		padfPrjParams[4] = 1.0;
		setTM(TO_DEGREES * padfPrjParams[2],
			dfCenterLong,
			padfPrjParams[4],
			padfPrjParams[5], padfPrjParams[6]);
	}
	break;

	case PAN_PROJ_STEREO:
		setStereographic(TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[4],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_AE:
		setAE(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_GNOMON:
		setGnomonic(TO_DEGREES * padfPrjParams[2],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_MOLL:
		setMollweide(TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_LAEA:
		setLAEA(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_EQC:
		setEquirectangular(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_CEA:
		setCEA(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	case PAN_PROJ_IMWP:
		setIWMPolyconic(TO_DEGREES * padfPrjParams[0],
			TO_DEGREES * padfPrjParams[1],
			TO_DEGREES * padfPrjParams[3],
			padfPrjParams[5], padfPrjParams[6]);
		break;

	default:
		gtl::debug("OSR_Panorama", "Unsupported projection: %ld", iProjSys);
		setLocalCS(String().printf("\"Panorama\" projection number %ld",
			iProjSys));
		break;

	}

	/* -------------------------------------------------------------------- */
	/*      Try to translate the datum/spheroid.                            */
	/* -------------------------------------------------------------------- */

	if (!isLocal())
	{
		if (iDatum > 0 && iDatum < NUMBER_OF_DATUMS && aoDatums[iDatum])
		{
			SpatialReference oGCS;
			oGCS.importFromEPSG(aoDatums[iDatum]);
			copyGeogCSFrom(&oGCS);
		}

		else if (iEllips > 0
			&& iEllips < (long)NUMBER_OF_ELLIPSOIDS
			&& aoEllips[iEllips])
		{
			char    *pszName = NULL;
			double  dfSemiMajor, dfInvFlattening;

			if (getEllipsoidInfo(aoEllips[iEllips], &pszName,
				&dfSemiMajor, &dfInvFlattening) == true)
			{
				setGeogCS(String().printf(
					"Unknown datum based upon the %s ellipsoid",
					pszName),
					String().printf(
						"Not specified (based on %s spheroid)", pszName),
					pszName, dfSemiMajor, dfInvFlattening,
					NULL, 0.0, NULL, 0.0);
				setAuthority("SPHEROID", "EPSG", aoEllips[iEllips]);
			}
			else
			{
				gtl::error(gtl::ERRORTYPE::GET_WARNING, gtl::ERRORCODE::GEC_APPDEFINED,
					"Failed to lookup ellipsoid code %ld, likely due to"
					" missing GDAL gcs.csv\n"
					" file.  Falling back to use Pulkovo 42.", iEllips);
				setWellKnownGeogCS("EPSG:4284");
			}

			if (pszName)
				free(pszName);
		}

		else
		{
			gtl::error(gtl::ERRORTYPE::GET_WARNING, gtl::ERRORCODE::GEC_APPDEFINED,
				"Wrong datum code %ld. Supported datums are 1--%ld only.\n"
				"Falling back to use Pulkovo 42.",
				iDatum, NUMBER_OF_DATUMS - 1);
			setWellKnownGeogCS("EPSG:4284");
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Grid units translation                                          */
	/* -------------------------------------------------------------------- */
	if (isLocal() || isProjected())
		setLinearUnits(SRS_UL_METER, 1.0);

	fixupOrdering();

	if (bProjAllocated && padfPrjParams)
		free(padfPrjParams);

	return true;
}
 
/************************************************************************/
/*                           exportToPanorama()                         */
/************************************************************************/

/**
* Export coordinate system in "Panorama" GIS projection definition.
*
* This method is the equivalent of the C function OSRExportToPanorama().
*
* @param piProjSys Pointer to variable, where the projection system code will
* be returned.
*
* @param piDatum Pointer to variable, where the coordinate system code will
* be returned.
*
* @param piEllips Pointer to variable, where the spheroid code will be
* returned.
*
* @param piZone Pointer to variable, where the zone for UTM projection
* system will be returned.
*
* @param padfPrjParams an existing 7 double buffer into which the
* projection parameters will be placed. See importFromPanorama()
* for the list of parameters.
*
* @return true on success or an error code on failure.
*/

bool SpatialReference::exportToPanorama(long *piProjSys, long *piDatum,
	long *piEllips, long *piZone,
	double *padfPrjParams) const

{
	assert(padfPrjParams);

	const char  *pszProjection = getAttrValue("PROJECTION");

	/* -------------------------------------------------------------------- */
	/*      Fill all projection parameters with zero.                       */
	/* -------------------------------------------------------------------- */
	int     i;

	*piDatum = 0L;
	*piEllips = 0L;
	*piZone = 0L;
	for (i = 0; i < 7; i++)
		padfPrjParams[i] = 0.0;

	/* ==================================================================== */
	/*      Handle the projection definition.                               */
	/* ==================================================================== */
	if (isLocal())
		*piProjSys = PAN_PROJ_NONE;

	else if (pszProjection == NULL)
	{
#ifdef DEBUG
		gtl::debug("OSR_Panorama",
			"Empty projection definition, considered as Geographic");
#endif
		*piProjSys = PAN_PROJ_NONE;
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MERCATOR_1SP))
	{
		*piProjSys = PAN_PROJ_MERCAT;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[4] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLAR_STEREOGRAPHIC))
	{
		*piProjSys = PAN_PROJ_PS;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[4] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLYCONIC))
	{
		*piProjSys = PAN_PROJ_POLYC;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIDISTANT_CONIC))
	{
		*piProjSys = PAN_PROJ_EC;
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		padfPrjParams[1] =
			TO_RADIANS * getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0);
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP))
	{
		*piProjSys = PAN_PROJ_LCC;
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		padfPrjParams[1] =
			TO_RADIANS * getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0);
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_TRANSVERSE_MERCATOR))
	{
		int bNorth;

		*piZone = getUTMZone(&bNorth);

		if (*piZone != 0)
		{
			*piProjSys = PAN_PROJ_UTM;
			if (!bNorth)
				*piZone = -*piZone;
		}
		else
		{
			*piProjSys = PAN_PROJ_TM;
			padfPrjParams[3] =
				TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
			padfPrjParams[2] =
				TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
			padfPrjParams[4] =
				getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
			padfPrjParams[5] =
				getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
			padfPrjParams[6] =
				getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		}
	}

	else if (cslIEqualString(pszProjection, SRS_PT_WAGNER_I))
	{
		*piProjSys = PAN_PROJ_WAG1;
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_STEREOGRAPHIC))
	{
		*piProjSys = PAN_PROJ_STEREO;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[4] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_AZIMUTHAL_EQUIDISTANT))
	{
		*piProjSys = PAN_PROJ_AE;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0);
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_GNOMONIC))
	{
		*piProjSys = PAN_PROJ_GNOMON;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MOLLWEIDE))
	{
		*piProjSys = PAN_PROJ_MOLL;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA))
	{
		*piProjSys = PAN_PROJ_LAEA;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIRECTANGULAR))
	{
		*piProjSys = PAN_PROJ_EQC;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_CYLINDRICAL_EQUAL_AREA))
	{
		*piProjSys = PAN_PROJ_CEA;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[2] =
			TO_RADIANS * getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_IMW_POLYCONIC))
	{
		*piProjSys = PAN_PROJ_IMWP;
		padfPrjParams[3] =
			TO_RADIANS * getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		padfPrjParams[0] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_1ST_POINT, 0.0);
		padfPrjParams[1] =
			TO_RADIANS * getNormProjParm(SRS_PP_LATITUDE_OF_2ND_POINT, 0.0);
		padfPrjParams[5] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		padfPrjParams[6] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	// Projection unsupported by "Panorama" GIS
	else
	{
		gtl::debug("OSR_Panorama",
			"Projection \"%s\" unsupported by \"Panorama\" GIS. "
			"Geographic system will be used.", pszProjection);
		*piProjSys = PAN_PROJ_NONE;
	}

	/* -------------------------------------------------------------------- */
	/*      Translate the datum.                                            */
	/* -------------------------------------------------------------------- */
	const char  *pszDatum = getAttrValue("DATUM");

	if (pszDatum == NULL)
	{
		*piDatum = PAN_DATUM_NONE;
		*piEllips = PAN_ELLIPSOID_NONE;
	}
	else if (cslIEqualString(pszDatum, "Pulkovo_1942"))
	{
		*piDatum = PAN_DATUM_PULKOVO42;
		*piEllips = PAN_ELLIPSOID_KRASSOVSKY;
	}
	else if (cslIEqualString(pszDatum, SRS_DN_WGS84))
	{
		*piDatum = PAN_DATUM_WGS84;
		*piEllips = PAN_ELLIPSOID_WGS84;
	}

	// If not found well known datum, translate ellipsoid
	else
	{
		double      dfSemiMajor = getSemiMajor();
		double      dfInvFlattening = getInvFlattening();
		size_t      i;

#ifdef DEBUG
		gtl::debug("OSR_Panorama",
			"Datum \"%s\" unsupported by \"Panorama\" GIS. "
			"Trying to translate an ellipsoid definition.", pszDatum);
#endif

		for (i = 0; i < NUMBER_OF_ELLIPSOIDS; i++)
		{
			if (aoEllips[i])
			{
				double  dfSM = 0.0;
				double  dfIF = 1.0;

				if (getEllipsoidInfo(aoEllips[i], NULL,
					&dfSM, &dfIF) == true
					&& doubleEqual(dfSemiMajor, dfSM)
					&& doubleEqual(dfInvFlattening, dfIF))
				{
					*piEllips = (long) i;
					break;
				}
			}
		}

		if (i == NUMBER_OF_ELLIPSOIDS)    // Didn't found matches.
		{
#ifdef DEBUG
			gtl::debug("OSR_Panorama",
				"Ellipsoid \"%s\" unsupported by \"Panorama\" GIS.",
				pszDatum);
#endif
			*piDatum = PAN_DATUM_NONE;
			*piEllips = PAN_ELLIPSOID_NONE;
		}
	}

	return true;
}


end_gdb_namespace
end_gtl_namespace