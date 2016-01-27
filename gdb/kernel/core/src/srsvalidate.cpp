#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace

/* why would fipszone and zone be paramers when they relate to a composite
projection which renders done into a non-zoned projection? */

static const char *papszParameters[] =
{
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_STANDARD_PARALLEL_2,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_ORIGIN,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	SRS_PP_AZIMUTH,
	SRS_PP_LONGITUDE_OF_POINT_1,
	SRS_PP_LATITUDE_OF_POINT_1,
	SRS_PP_LONGITUDE_OF_POINT_2,
	SRS_PP_LATITUDE_OF_POINT_2,
	SRS_PP_LONGITUDE_OF_POINT_3,
	SRS_PP_LATITUDE_OF_POINT_3,
	SRS_PP_LANDSAT_NUMBER,
	SRS_PP_PATH_NUMBER,
	SRS_PP_PERSPECTIVE_POINT_HEIGHT,
	SRS_PP_FIPSZONE,
	SRS_PP_ZONE,
	SRS_PP_RECTIFIED_GRID_ANGLE,
	SRS_PP_SATELLITE_HEIGHT,
	SRS_PP_PSEUDO_STD_PARALLEL_1,
	SRS_PP_LATITUDE_OF_1ST_POINT,
	SRS_PP_LONGITUDE_OF_1ST_POINT,
	SRS_PP_LATITUDE_OF_2ND_POINT,
	SRS_PP_LONGITUDE_OF_2ND_POINT,
	NULL
};

// the following projection lists are incomplete.  they will likely
// change after the CT RPF response.  Examples show alternate forms with
// underscores instead of spaces.  Should we use the EPSG names were available?
// Plate-Caree has an accent in the spec!

static const char *papszProjectionSupported[] =
{
	SRS_PT_CASSINI_SOLDNER,
	SRS_PT_BONNE,
	SRS_PT_EQUIDISTANT_CONIC,
	SRS_PT_EQUIRECTANGULAR,
	SRS_PT_ECKERT_I,
	SRS_PT_ECKERT_II,
	SRS_PT_ECKERT_III,
	SRS_PT_ECKERT_IV,
	SRS_PT_ECKERT_V,
	SRS_PT_ECKERT_VI,
	SRS_PT_MERCATOR_1SP,
	SRS_PT_MERCATOR_2SP,
	SRS_PT_MOLLWEIDE,
	SRS_PT_ROBINSON,
	SRS_PT_ALBERS_CONIC_EQUAL_AREA,
	SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP,
	SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP,
	SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM,
	SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA,
	SRS_PT_TRANSVERSE_MERCATOR,
	SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED,
	SRS_PT_OBLIQUE_STEREOGRAPHIC,
	SRS_PT_POLAR_STEREOGRAPHIC,
	SRS_PT_HOTINE_OBLIQUE_MERCATOR,
	SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN,
	SRS_PT_HOTINE_OBLIQUE_MERCATOR_AZIMUTH_CENTER,
	SRS_PT_LABORDE_OBLIQUE_MERCATOR,
	SRS_PT_SWISS_OBLIQUE_CYLINDRICAL,
	SRS_PT_AZIMUTHAL_EQUIDISTANT,
	SRS_PT_MILLER_CYLINDRICAL,
	SRS_PT_NEW_ZEALAND_MAP_GRID,
	SRS_PT_SINUSOIDAL,
	SRS_PT_STEREOGRAPHIC,
	SRS_PT_GNOMONIC,
	SRS_PT_GALL_STEREOGRAPHIC,
	SRS_PT_ORTHOGRAPHIC,
	SRS_PT_POLYCONIC,
	SRS_PT_VANDERGRINTEN,
	SRS_PT_GEOSTATIONARY_SATELLITE,
	SRS_PT_TWO_POINT_EQUIDISTANT,
	SRS_PT_IMW_POLYCONIC,
	SRS_PT_WAGNER_I,
	SRS_PT_WAGNER_II,
	SRS_PT_WAGNER_III,
	SRS_PT_WAGNER_IV,
	SRS_PT_WAGNER_V,
	SRS_PT_WAGNER_VI,
	SRS_PT_WAGNER_VII,
	SRS_PT_GAUSSSCHREIBERTMERCATOR,
	SRS_PT_KROVAK,
	SRS_PT_CYLINDRICAL_EQUAL_AREA,
	SRS_PT_GOODE_HOMOLOSINE,
	SRS_PT_IGH,
	NULL
};

static const char *papszProjectionUnsupported[] =
{
	SRS_PT_NEW_ZEALAND_MAP_GRID,
	SRS_PT_TUNISIA_MINING_GRID,
	NULL
};

/*
** List of supported projections with the PARAMETERS[] acceptable for each.
*/
static const char *papszProjWithParms[] = {

	SRS_PT_TRANSVERSE_MERCATOR,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_TUNISIA_MINING_GRID,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ALBERS_CONIC_EQUAL_AREA,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_STANDARD_PARALLEL_2,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_AZIMUTHAL_EQUIDISTANT,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_BONNE,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_CYLINDRICAL_EQUAL_AREA,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_CASSINI_SOLDNER,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_EQUIDISTANT_CONIC,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_STANDARD_PARALLEL_2,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_I,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_II,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_III,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_IV,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_V,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ECKERT_VI,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_EQUIRECTANGULAR,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_GALL_STEREOGRAPHIC,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_GNOMONIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_HOTINE_OBLIQUE_MERCATOR,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_AZIMUTH,
	SRS_PP_RECTIFIED_GRID_ANGLE,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_HOTINE_OBLIQUE_MERCATOR_AZIMUTH_CENTER,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_AZIMUTH,
	SRS_PP_RECTIFIED_GRID_ANGLE,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LATITUDE_OF_POINT_1,
	SRS_PP_LONGITUDE_OF_POINT_1,
	SRS_PP_LATITUDE_OF_POINT_2,
	SRS_PP_LONGITUDE_OF_POINT_2
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_STANDARD_PARALLEL_2,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_STANDARD_PARALLEL_2,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_MILLER_CYLINDRICAL,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_MERCATOR_1SP,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_MERCATOR_2SP,
	SRS_PP_STANDARD_PARALLEL_1,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_MOLLWEIDE,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_NEW_ZEALAND_MAP_GRID,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ORTHOGRAPHIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_POLYCONIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_POLAR_STEREOGRAPHIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_ROBINSON,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_SINUSOIDAL,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_STEREOGRAPHIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_SWISS_OBLIQUE_CYLINDRICAL,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_OBLIQUE_STEREOGRAPHIC,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_VANDERGRINTEN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_GEOSTATIONARY_SATELLITE,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SATELLITE_HEIGHT,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_KROVAK,
	SRS_PP_LATITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_AZIMUTH,
	SRS_PP_PSEUDO_STD_PARALLEL_1,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_TWO_POINT_EQUIDISTANT,
	SRS_PP_LATITUDE_OF_1ST_POINT,
	SRS_PP_LONGITUDE_OF_1ST_POINT,
	SRS_PP_LATITUDE_OF_2ND_POINT,
	SRS_PP_LONGITUDE_OF_2ND_POINT,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_IMW_POLYCONIC,
	SRS_PP_LATITUDE_OF_1ST_POINT,
	SRS_PP_LATITUDE_OF_2ND_POINT,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_I,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_II,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_III,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_IV,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_V,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_VI,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_WAGNER_VII,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_GAUSSSCHREIBERTMERCATOR,
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_SCALE_FACTOR,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_GOODE_HOMOLOSINE,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_FALSE_EASTING,
	SRS_PP_FALSE_NORTHING,
	NULL,

	SRS_PT_IGH,
	NULL,

	NULL
};

static const char *papszAliasGroupList[] = {
	SRS_PP_LATITUDE_OF_ORIGIN,
	SRS_PP_LATITUDE_OF_CENTER,
	NULL,
	SRS_PP_CENTRAL_MERIDIAN,
	SRS_PP_LONGITUDE_OF_CENTER,
	SRS_PP_LONGITUDE_OF_ORIGIN,
	NULL,
	NULL
};


/************************************************************************/
/*                              validate()                              */
/************************************************************************/

/**
* \brief validate SRS tokens.
*
* This method attempts to verify that the spatial reference system is
* well formed, and consists of known tokens.  The validation is not
* comprehensive.
*
* This method is the same as the C function OSRValidate().
*
* @return true if all is fine, false if the SRS is
* not well formed, and false if the SRS is well formed,
* but contains non-standard PROJECTION[] values.
*/

bool SpatialReference::validate()

{
	/* -------------------------------------------------------------------- */
	/*      validate root node.                                             */
	/* -------------------------------------------------------------------- */
	if (poRoot == NULL)
	{
		return false;
	}

	return validate(poRoot);
}


bool SpatialReference::validate(SRSNode *poRoot)
{
	if (!cslIEqualString(poRoot->getValue(), "GEOGCS")
		&& !cslIEqualString(poRoot->getValue(), "PROJCS")
		&& !cslIEqualString(poRoot->getValue(), "LOCAL_CS")
		&& !cslIEqualString(poRoot->getValue(), "GEOCCS")
		&& !cslIEqualString(poRoot->getValue(), "VERT_CS")
		&& !cslIEqualString(poRoot->getValue(), "COMPD_CS"))
	{
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      For a COMPD_CS, validate subparameters and head & tail cs       */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(poRoot->getValue(), "COMPD_CS"))
	{
		SRSNode     *poNode;
		int             i;

		for (i = 1; i < poRoot->getChildCount(); i++)
		{
			poNode = poRoot->getChild(i);

			if (cslIEqualString(poNode->getValue(), "GEOGCS") ||
				cslIEqualString(poNode->getValue(), "PROJCS") ||
				cslIEqualString(poNode->getValue(), "LOCAL_CS") ||
				cslIEqualString(poNode->getValue(), "GEOCCS") ||
				cslIEqualString(poNode->getValue(), "VERT_CS") ||
				cslIEqualString(poNode->getValue(), "COMPD_CS"))
			{
				bool eErr = validate(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else
			{
				return false;
			}
		}

		return true;
	}

	/* -------------------------------------------------------------------- */
	/*      validate VERT_CS                                                */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(poRoot->getValue(), "VERT_CS"))
	{
		SRSNode     *poNode;
		int             i;
		int             bGotVertDatum = FALSE;
		int             bGotUnit = FALSE;
		int             nCountAxis = 0;

		for (i = 1; i < poRoot->getChildCount(); i++)
		{
			poNode = poRoot->getChild(i);

			if (cslIEqualString(poNode->getValue(), "VERT_DATUM"))
			{
				bool eErr = validateVertDatum(poNode);
				if (eErr != true)
					return eErr;
				bGotVertDatum = TRUE;
			}
			else if (cslIEqualString(poNode->getValue(), "UNIT"))
			{
				bool eErr = validateUnit(poNode);
				if (eErr != true)
					return eErr;
				bGotUnit = TRUE;
			}
			else if (cslIEqualString(poNode->getValue(), "AXIS"))
			{
				bool eErr = validateAxis(poNode);
				if (eErr != true)
					return eErr;
				nCountAxis++;
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else
			{
				return false;
			}
		}

		if (!bGotVertDatum)
		{
			return false;
		}

		if (!bGotUnit)
		{
			return false;
		}

		if (nCountAxis > 1)
		{
			return false;
		}
		return true;
	}

	/* -------------------------------------------------------------------- */
	/*      validate GEOCCS                                                 */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(poRoot->getValue(), "GEOCCS"))
	{
		SRSNode     *poNode;
		int             i;
		int             bGotDatum = FALSE;
		int             bGotPrimeM = FALSE;
		int             bGotUnit = FALSE;
		int             nCountAxis = 0;

		for (i = 1; i < poRoot->getChildCount(); i++)
		{
			poNode = poRoot->getChild(i);

			if (cslIEqualString(poNode->getValue(), "DATUM"))
			{
				bGotDatum = TRUE;
			}
			else if (cslIEqualString(poNode->getValue(), "PRIMEM"))
			{
				bGotPrimeM = TRUE;

				if (poNode->getChildCount() < 2
					|| poNode->getChildCount() > 3)
				{
					return false;
				}
			}
			else if (cslIEqualString(poNode->getValue(), "UNIT"))
			{
				bool eErr = validateUnit(poNode);
				if (eErr != true)
					return eErr;
				bGotUnit = TRUE;
			}
			else if (cslIEqualString(poNode->getValue(), "AXIS"))
			{
				bool eErr = validateAxis(poNode);
				if (eErr != true)
					return eErr;
				nCountAxis++;
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else
			{
				return false;
			}
		}

		if (!bGotDatum)
		{
			return false;
		}

		if (!bGotPrimeM)
		{
			return false;
		}

		if (!bGotUnit)
		{
			return false;
		}

		if (nCountAxis != 0 && nCountAxis != 3)
		{
			return false;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      For a PROJCS, validate subparameters (other than GEOGCS).       */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(poRoot->getValue(), "PROJCS"))
	{
		SRSNode     *poNode;
		int             i;

		for (i = 1; i < poRoot->getChildCount(); i++)
		{
			poNode = poRoot->getChild(i);

			if (cslIEqualString(poNode->getValue(), "GEOGCS"))
			{
				/* validated elsewhere */
			}
			else if (cslIEqualString(poNode->getValue(), "UNIT"))
			{
				bool eErr = validateUnit(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "PARAMETER"))
			{
				if (poNode->getChildCount() != 2)
				{
					return false;
				}
				else if (cslIFindString((char **)papszParameters,
					poNode->getChild(0)->getValue()) == -1)
				{
					return false;
				}
			}
			else if (cslIEqualString(poNode->getValue(), "PROJECTION"))
			{
				if (poNode->getChildCount() != 1 && poNode->getChildCount() != 2)
				{
					return false;
				}
				else if (cslIFindString((char **)papszProjectionSupported,
					poNode->getChild(0)->getValue()) == -1
					&& cslIFindString((char **)papszProjectionUnsupported,
						poNode->getChild(0)->getValue()) == -1)
				{
					return false;
				}
				else if (cslIFindString((char **)papszProjectionSupported,
					poNode->getChild(0)->getValue()) == -1)
				{
					return false;
				}

				if (poNode->getChildCount() == 2)
				{
					poNode = poNode->getChild(1);
					if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
					{
						bool eErr = validateAuthority(poNode);
						if (eErr != true)
							return eErr;
					}
					else
					{
						return false;
					}
				}
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "AXIS"))
			{
				bool eErr = validateAxis(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "EXTENSION"))
			{
				// We do not try to control the sub-organization of 
				// EXTENSION nodes.
			}
			else
			{
				return false;
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      validate GEOGCS if found.                                       */
	/* -------------------------------------------------------------------- */
	SRSNode *poGEOGCS = poRoot->getNode("GEOGCS");

	if (poGEOGCS != NULL)
	{
		SRSNode     *poNode;
		int             i;

		for (i = 1; i < poGEOGCS->getChildCount(); i++)
		{
			poNode = poGEOGCS->getChild(i);

			if (cslIEqualString(poNode->getValue(), "DATUM"))
			{
				/* validated elsewhere */
			}
			else if (cslIEqualString(poNode->getValue(), "PRIMEM"))
			{
				if (poNode->getChildCount() < 2
					|| poNode->getChildCount() > 3)
				{
					return false;
				}
			}
			else if (cslIEqualString(poNode->getValue(), "UNIT"))
			{
				bool eErr = validateUnit(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "AXIS"))
			{
				bool eErr = validateAxis(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else
			{
				return false;
			}
		}

		if (poGEOGCS->getNode("DATUM") == NULL)
		{
			return false;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      validate DATUM/SPHEROID.                                        */
	/* -------------------------------------------------------------------- */
	SRSNode *poDATUM = poRoot->getNode("DATUM");

	if (poDATUM != NULL)
	{
		SRSNode     *poSPHEROID;
		int             bGotSpheroid = FALSE;
		int             i;

		if (poDATUM->getChildCount() == 0)
		{
			return false;
		}

		for (i = 1; i < poDATUM->getChildCount(); i++)
		{
			SRSNode *poNode;
			poNode = poDATUM->getChild(i);

			if (cslIEqualString(poNode->getValue(), "SPHEROID"))
			{
				poSPHEROID = poDATUM->getChild(1);
				bGotSpheroid = TRUE;

				if (poSPHEROID->getChildCount() != 3
					&& poSPHEROID->getChildCount() != 4)
				{
					return false;
				}
				else if (cslStringToNumber(poSPHEROID->getChild(1)->getValue()) == 0.0)
				{
					return false;
				}
			}
			else if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
			{
				bool eErr = validateAuthority(poNode);
				if (eErr != true)
					return eErr;
			}
			else if (cslIEqualString(poNode->getValue(), "TOWGS84"))
			{
				if (poNode->getChildCount() != 3
					&& poNode->getChildCount() != 7)
				{
					return false;
				}
			}
			else
			{
				return false;
			}
		}

		if (!bGotSpheroid)
		{
			return false;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      If this is projected, try to validate the detailed set of       */
	/*      parameters used for the projection.                             */
	/* -------------------------------------------------------------------- */
	bool  eErr;

	eErr = validateProjection(poRoot);
	if (eErr != true)
		return eErr;

	return true;
}

/************************************************************************/
/*                             isAliasFor()                             */
/************************************************************************/

/**
* \brief Return whether the first string passed in an acceptable alias for the
* second string according to the AliasGroupList
*
* @param pszParm1 first string
* @param pszParm2 second string
*
* @return TRUE if both strings are aliases according to the AliasGroupList, FALSE otherwise
*/
int SpatialReference::isAliasFor(const char *pszParm1,
	const char *pszParm2)

{
	int         iGroup;

	/* -------------------------------------------------------------------- */
	/*      Look for a group containing pszParm1.                           */
	/* -------------------------------------------------------------------- */
	for (iGroup = 0; papszAliasGroupList[iGroup] != NULL; iGroup++)
	{
		int     i;

		for (i = iGroup; papszAliasGroupList[i] != NULL; i++)
		{
			if (cslIEqualString(pszParm1, papszAliasGroupList[i]))
				break;
		}

		if (papszAliasGroupList[i] == NULL)
			iGroup = i;
		else
			break;
	}

	/* -------------------------------------------------------------------- */
	/*      Does this group also contain pszParm2?                          */
	/* -------------------------------------------------------------------- */
	while (papszAliasGroupList[iGroup] != NULL)
	{
		if (cslIEqualString(papszAliasGroupList[iGroup++], pszParm2))
			return TRUE;
	}

	return FALSE;
}

/************************************************************************/
/*                         validateProjection()                         */
/************************************************************************/

/**
* \brief validate the current PROJECTION's arguments.
*
* @return true if the PROJECTION's arguments validate, an error code
*         otherwise
*/
bool SpatialReference::validateProjection(SRSNode *poRoot)
{
	SRSNode *poPROJCS = poRoot->getNode("PROJCS");

	if (poPROJCS == NULL)
		return true;

	if (poPROJCS->getNode("PROJECTION") == NULL)
	{
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Find the matching group in the proj and parms table.            */
	/* -------------------------------------------------------------------- */
	const char *pszProjection;
	int        iOffset;

	pszProjection = poPROJCS->getNode("PROJECTION")->getChild(0)->getValue();

	for (iOffset = 0;
	papszProjWithParms[iOffset] != NULL
		&& !cslIEqualString(papszProjWithParms[iOffset], pszProjection); )
	{
		while (papszProjWithParms[iOffset] != NULL)
			iOffset++;
		iOffset++;
	}

	if (papszProjWithParms[iOffset] == NULL)
		return false;

	iOffset++;

	/* -------------------------------------------------------------------- */
	/*      Check all parameters, and verify they are in the permitted      */
	/*      list.                                                           */
	/* -------------------------------------------------------------------- */
	int iNode;

	for (iNode = 0; iNode < poPROJCS->getChildCount(); iNode++)
	{
		SRSNode *poParm = poPROJCS->getChild(iNode);
		int          i;
		const char  *pszParmName;

		if (!cslIEqualString(poParm->getValue(), "PARAMETER"))
			continue;

		pszParmName = poParm->getChild(0)->getValue();

		for (i = iOffset; papszProjWithParms[i] != NULL; i++)
		{
			if (cslIEqualString(papszProjWithParms[i], pszParmName))
				break;
		}

		/* This parameter is not an exact match, is it an alias? */
		if (papszProjWithParms[i] == NULL)
		{
			for (i = iOffset; papszProjWithParms[i] != NULL; i++)
			{
				if (isAliasFor(papszProjWithParms[i], pszParmName))
					break;
			}

			if (papszProjWithParms[i] == NULL)
			{
				return false;
			}
			else
			{
				return false;
			}
		}
	}

	return true;
}

/************************************************************************/
/*                         validateVertDatum()                          */
/************************************************************************/

/**
* \brief validate the current VERT_DATUM's arguments.
*
* @return true if the VERT_DATUM's arguments validate, an error code
*         otherwise
*/
bool SpatialReference::validateVertDatum(SRSNode *poRoot)
{
	if (!cslIEqualString(poRoot->getValue(), "VERT_DATUM"))
		return true;

	if (poRoot->getChildCount() < 2)
	{
		return false;
	}

	if (atoi(poRoot->getChild(1)->getValue()) == 0)
	{
		return false;
	}

	SRSNode     *poNode;
	int             i;

	for (i = 2; i < poRoot->getChildCount(); i++)
	{
		poNode = poRoot->getChild(i);

		if (cslIEqualString(poNode->getValue(), "AUTHORITY"))
		{
			bool eErr = validateAuthority(poNode);
			if (eErr != true)
				return eErr;
		}
		else if (cslIEqualString(poNode->getValue(), "EXTENSION"))
		{
			// We do not try to control the sub-organization of
			// EXTENSION nodes.
		}
		else
		{
			return false;
		}
	}

	return true;
}

/************************************************************************/
/*                         validateAuthority()                          */
/************************************************************************/

/**
* \brief validate the current AUTHORITY's arguments.
*
* @return true if the AUTHORITY's arguments validate, an error code
*         otherwise
*/
bool SpatialReference::validateAuthority(SRSNode *poRoot)
{
	if (!cslIEqualString(poRoot->getValue(), "AUTHORITY"))
		return true;

	if (poRoot->getChildCount() != 2)
	{
		return false;
	}

	return true;
}

/************************************************************************/
/*                           validateAxis()                             */
/************************************************************************/

/**
* \brief validate the current AXIS's arguments.
*
* @return true if the AXIS's arguments validate, an error code
*         otherwise
*/
bool SpatialReference::validateAxis(SRSNode *poRoot)
{
	if (!cslIEqualString(poRoot->getValue(), "AXIS"))
		return true;

	if (poRoot->getChildCount() != 2)
	{
		return false;
	}

	return true;
}


/************************************************************************/
/*                           validateUnit()                             */
/************************************************************************/

/**
* \brief validate the current UNIT's arguments.
*
* @return true if the UNIT's arguments validate, an error code
*         otherwise
*/
bool SpatialReference::validateUnit(SRSNode *poRoot)
{
	if (!cslIEqualString(poRoot->getValue(), "UNIT"))
		return true;

	if (poRoot->getChildCount() != 2
		&& poRoot->getChildCount() != 3)
	{
		return false;
	}
	else if (cslStringToNumber(poRoot->getChild(1)->getValue()) == 0.0)
	{
		return false;
	}

	return true;
}



end_gdb_namespace
end_gtl_namespace