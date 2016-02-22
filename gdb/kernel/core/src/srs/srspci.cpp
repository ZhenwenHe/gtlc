/*derived from gdal*/
#include "spatialreference.h"

begin_gtl_namespace
begin_gdb_namespace
bool
getEllipsoidInfo(int nCode, char ** ppszName,
	double * pdfSemiMajor, double * pdfInvFlattening);
typedef struct
{
	const char  *pszPCIDatum;
	int         nEPSGCode;
} PCIDatums;

static const PCIDatums asDatums[] =
{
	{ "D-01", 4267 },   // NAD27 (USA, NADCON)
	{ "D-03", 4267 },   // NAD27 (Canada, NTv1)
	{ "D-02", 4269 },   // NAD83 (USA, NADCON)
	{ "D-04", 4269 },   // NAD83 (Canada, NTv1)
	{ "D000", 4326 },   // WGS 1984
	{ "D001", 4322 },   // WGS 1972
	{ "D008", 4296 },   // Sudan
	{ "D013", 4601 },   // Antigua Island Astro 1943
	{ "D029", 4202 },   // Australian Geodetic 1966
	{ "D030", 4203 },   // Australian Geodetic 1984
	{ "D033", 4216 },   // Bermuda 1957
	{ "D034", 4165 },   // Bissau
	{ "D036", 4219 },   // Bukit Rimpah
	{ "D038", 4221 },   // Campo Inchauspe
	{ "D040", 4222 },   // Cape
	{ "D042", 4223 },   // Carthage
	{ "D044", 4224 },   // Chua Astro
	{ "D045", 4225 },   // Corrego Alegre
	{ "D046", 4155 },   // Dabola (Guinea)
	{ "D066", 4272 },   // Geodetic Datum 1949 (New Zealand)
	{ "D071", 4255 },   // Herat North (Afghanistan)
	{ "D077", 4239 },   // Indian 1954 (Thailand, Vietnam)
	{ "D078", 4240 },   // Indian 1975 (Thailand)
	{ "D083", 4244 },   // Kandawala (Sri Lanka)
	{ "D085", 4245 },   // Kertau 1948 (West Malaysia & Singapore)
	{ "D088", 4250 },   // Leigon (Ghana)
	{ "D089", 4251 },   // Liberia 1964 (Liberia)
	{ "D092", 4256 },   // Mahe 1971 (Mahe Island)
	{ "D093", 4262 },   // Massawa (Ethiopia (Eritrea))
	{ "D094", 4261 },   // Merchich (Morocco)
	{ "D098", 4604 },   // Montserrat Island Astro 1958 (Montserrat (Leeward Islands))
	{ "D110", 4267 },   // NAD27 / Alaska
	{ "D139", 4282 },   // Pointe Noire 1948 (Congo)
	{ "D140", 4615 },   // Porto Santo 1936 (Porto Santo, Madeira Islands)
	{ "D151", 4139 },   // Puerto Rico (Puerto Rico, Virgin Islands)
	{ "D153", 4287 },   // Qornoq (Greenland (South))
	{ "D158", 4292 },   // Sapper Hill 1943 (East Falkland Island)
	{ "D159", 4293 },   // Schwarzeck (Namibia)
	{ "D160", 4616 },   // Selvagem Grande 1938 (Salvage Islands)
	{ "D176", 4297 },   // Tananarive Observatory 1925 (Madagascar)
	{ "D177", 4298 },   // Timbalai 1948 (Brunei, East Malaysia (Sabah, Sarawak))
	{ "D187", 4309 },   // Yacare (Uruguay)
	{ "D188", 4311 },   // Zanderij (Suriname)
	{ "D401", 4124 },   // RT90 (Sweden)
	{ "D501", 4312 },   // MGI (Hermannskogel, Austria)
	{ NULL, 0 }
};

static const PCIDatums asEllips[] =
{
	{ "E000", 7008 },     // Clarke, 1866 (NAD1927)
	{ "E001", 7034 },     // Clarke, 1880
	{ "E002", 7004 },     // Bessel, 1841
	{ "E004", 7022 },     // International, 1924 (Hayford, 1909)
	{ "E005", 7043 },     // WGS, 1972
	{ "E006", 7042 },     // Everest, 1830
	{ "E008", 7019 },     // GRS, 1980 (NAD1983)
	{ "E009", 7001 },     // Airy, 1830
	{ "E010", 7018 },     // Modified Everest 
	{ "E011", 7002 },     // Modified Airy
	{ "E012", 7030 },     // WGS, 1984 (GPS)
	{ "E014", 7003 },     // Australian National, 1965
	{ "E015", 7024 },     // Krassovsky, 1940
	{ "E016", 7053 },     // Hough
	{ "E019", 7052 },     // normal sphere
	{ "E333", 7046 },     // Bessel 1841 (Japan By Law)
	{ "E900", 7006 },     // Bessel, 1841 (Namibia)
	{ "E901", 7044 },     // Everest, 1956
	{ "E902", 7056 },     // Everest, 1969
	{ "E903", 7016 },     // Everest (Sabah & Sarawak)
	{ "E904", 7020 },     // Helmert, 1906
	{ "E907", 7036 },     // South American, 1969
	{ "E910", 7041 },     // ATS77
	{ NULL, 0 }
};
 
/************************************************************************/
/*                          importFromPCI()                             */
/************************************************************************/

/**
* \brief Import coordinate system from PCI projection definition.
*
* PCI software uses 16-character string to specify coordinate system
* and datum/ellipsoid. You should supply at least this string to the
* importFromPCI() function.
*
* This function is the equivalent of the C function OSRImportFromPCI().
*
* @param pszProj NULL terminated string containing the definition. Looks
* like "pppppppppppp Ennn" or "pppppppppppp Dnnn", where "pppppppppppp" is
* a projection code, "Ennn" is an ellipsoid code, "Dnnn" --- a datum code.
*
* @param pszUnits Grid units code ("DEGREE" or "METRE"). If NULL "METRE" will
* be used.
*
* @param padfPrjParams Array of 17 coordinate system parameters:
*
* [0]  Spheroid semi major axis
* [1]  Spheroid semi minor axis
* [2]  Reference Longitude
* [3]  Reference Latitude
* [4]  First Standard Parallel
* [5]  Second Standard Parallel
* [6]  False Easting
* [7]  False Northing
* [8]  Scale Factor
* [9]  Height above sphere surface
* [10] Longitude of 1st point on center line
* [11] Latitude of 1st point on center line
* [12] Longitude of 2nd point on center line
* [13] Latitude of 2nd point on center line
* [14] Azimuth east of north for center line
* [15] Landsat satellite number
* [16] Landsat path number
*
* Particular projection uses different parameters, unused ones may be set to
* zero. If NULL suppliet instead of array pointer default values will be
* used (i.e., zeroes).
*
* @return false on success or an error code in case of failure.
*/

bool SpatialReference::importFromPCI(const char *pszProj,
	const char *pszUnits,
	double *padfPrjParams)

{
	clear();

	if (pszProj == NULL || strnlen(pszProj, 16) < 16)
		return false;

	gtl::debug("OSR_PCI", "Trying to import projection \"%s\"", pszProj);

	/* -------------------------------------------------------------------- */
	/*      Use safe defaults if projection parameters are not supplied.    */
	/* -------------------------------------------------------------------- */
	int     bProjAllocated = FALSE;

	if (padfPrjParams == NULL)
	{
		int     i;

		padfPrjParams = (double *)malloc(17 * sizeof(double));
		if (!padfPrjParams)
			return false;
		for (i = 0; i < 17; i++)
			padfPrjParams[i] = 0.0;
		bProjAllocated = TRUE;
	}

	/* -------------------------------------------------------------------- */
	/*      Extract and "normalize" the earthmodel to look like E001,       */
	/*      D-02 or D109.                                                   */
	/* -------------------------------------------------------------------- */
	char szEarthModel[5];
	const char *pszEM;
	int bIsNAD27 = FALSE;

	strcpy(szEarthModel, "");
	pszEM = pszProj + strlen(pszProj) - 1;
	while (pszEM != pszProj)
	{
		if (*pszEM == 'e' || *pszEM == 'E' || *pszEM == 'd' || *pszEM == 'D')
		{
			int nCode = atoi(pszEM + 1);

			if (nCode >= -99 && nCode <= 999)
				sprintf(szEarthModel, "%c%03d", toupper(*pszEM), nCode);

			break;
		}

		pszEM--;
	}

	if (cslIEqualString(pszEM, "E000")
		|| cslIEqualString(pszEM, "D-01")
		|| cslIEqualString(pszEM, "D-03")
		|| cslIEqualString(pszEM, "D-07")
		|| cslIEqualString(pszEM, "D-09")
		|| cslIEqualString(pszEM, "D-11")
		|| cslIEqualString(pszEM, "D-13")
		|| cslIEqualString(pszEM, "D-17"))
		bIsNAD27 = TRUE;

	/* -------------------------------------------------------------------- */
	/*      Operate on the basis of the projection name.                    */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszProj, "LONG/LAT", 8))
	{
	}

	else if (cslNIEqualString(pszProj, "METER", 5)
		|| cslNIEqualString(pszProj, "METRE", 5))
	{
		setLocalCS("METER");
		setLinearUnits("METER", 1.0);
	}

	else if (cslNIEqualString(pszProj, "FEET", 4)
		|| cslNIEqualString(pszProj, "FOOT", 4))
	{
		setLocalCS("FEET");
		setLinearUnits("FEET", atof(SRS_UL_FOOT_CONV));
	}

	else if (cslNIEqualString(pszProj, "ACEA", 4))
	{
		setACEA(padfPrjParams[4], padfPrjParams[5],
			padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "AE", 2))
	{
		setAE(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "CASS ", 5))
	{
		setCS(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "EC", 2))
	{
		setEC(padfPrjParams[4], padfPrjParams[5],
			padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "ER", 2))
	{
		// PCI and GCTP don't support natural origin lat. 
		setEquirectangular2(0.0, padfPrjParams[2],
			padfPrjParams[3],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "GNO", 3))
	{
		setGnomonic(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	// FIXME: GVNP --- General Vertical Near- Side Perspective skipped

	// FIXME: GOOD -- our Goode's is not the interrupted version from pci

	else if (cslNIEqualString(pszProj, "LAEA", 4))
	{
		setLAEA(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "LCC ", 4))
	{
		setLCC(padfPrjParams[4], padfPrjParams[5],
			padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "LCC_1SP ", 7))
	{
		setLCC1SP(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[8],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "MC", 2))
	{
		setMC(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "MER", 3))
	{
		setMercator(padfPrjParams[3], padfPrjParams[2],
			(padfPrjParams[8] != 0.0) ? padfPrjParams[8] : 1.0,
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "OG", 2))
	{
		setOrthographic(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "OM ", 3))
	{
		if (padfPrjParams[10] == 0.0
			&& padfPrjParams[11] == 0.0
			&& padfPrjParams[12] == 0.0
			&& padfPrjParams[13] == 0.0)
		{
			setHOM(padfPrjParams[3], padfPrjParams[2],
				padfPrjParams[14],
				padfPrjParams[14], // use azimuth for grid angle
				padfPrjParams[8],
				padfPrjParams[6], padfPrjParams[7]);
		}
		else
		{
			setHOM2PNO(padfPrjParams[3],
				padfPrjParams[11], padfPrjParams[10],
				padfPrjParams[13], padfPrjParams[12],
				padfPrjParams[8],
				padfPrjParams[6], padfPrjParams[7]);
		}
	}

	else if (cslNIEqualString(pszProj, "PC", 2))
	{
		setPolyconic(padfPrjParams[3], padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "PS", 2))
	{
		setPS(padfPrjParams[3], padfPrjParams[2],
			(padfPrjParams[8] != 0.0) ? padfPrjParams[8] : 1.0,
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "ROB", 3))
	{
		setRobinson(padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "SGDO", 4))
	{
		setOS(padfPrjParams[3], padfPrjParams[2],
			(padfPrjParams[8] != 0.0) ? padfPrjParams[8] : 1.0,
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "SG", 2))
	{
		setStereographic(padfPrjParams[3], padfPrjParams[2],
			(padfPrjParams[8] != 0.0) ? padfPrjParams[8] : 1.0,
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "SIN", 3))
	{
		setSinusoidal(padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	// FIXME: SOM --- Space Oblique Mercator skipped

	else if (cslNIEqualString(pszProj, "SPCS", 4))
	{
		int     iZone;

		iZone = cslStringToInteger((char *)pszProj + 5, 4);

		setStatePlane(iZone, !bIsNAD27);
		setLinearUnitsAndUpdateParameters(SRS_UL_METER, 1.0);
	}

	else if (cslNIEqualString(pszProj, "SPIF", 4))
	{
		int     iZone;

		iZone = cslStringToInteger((char *)pszProj + 5, 4);

		setStatePlane(iZone, !bIsNAD27);
		setLinearUnitsAndUpdateParameters(SRS_UL_FOOT,
			atof(SRS_UL_FOOT_CONV));
	}

	else if (cslNIEqualString(pszProj, "SPAF", 4))
	{
		int     iZone;

		iZone = cslStringToInteger((char *)pszProj + 5, 4);

		setStatePlane(iZone, !bIsNAD27);
		setLinearUnitsAndUpdateParameters(SRS_UL_US_FOOT,
			atof(SRS_UL_US_FOOT_CONV));
	}

	else if (cslNIEqualString(pszProj, "TM", 2))
	{
		setTM(padfPrjParams[3], padfPrjParams[2],
			(padfPrjParams[8] != 0.0) ? padfPrjParams[8] : 1.0,
			padfPrjParams[6], padfPrjParams[7]);
	}

	else if (cslNIEqualString(pszProj, "UTM", 3))
	{
		int     iZone, bNorth = TRUE;

		iZone = cslStringToInteger((char *)pszProj + 4, 5);;
		if (iZone < 0)
		{
			iZone = -iZone;
			bNorth = FALSE;
		}

		// Check for a zone letter. PCI uses, accidentally, MGRS
		// type row lettering in its UTM projection
		char byZoneID = 0;

		if (strlen(pszProj) > 10 && pszProj[10] != ' ')
			byZoneID = pszProj[10];

		// Determine if the MGRS zone falls above or below the equator
		if (byZoneID != 0)
		{
			gtl::debug("OSR_PCI", "Found MGRS zone in UTM projection string: %c",
				byZoneID);

			if (byZoneID >= 'N' && byZoneID <= 'X')
			{
				bNorth = TRUE;
			}
			else if (byZoneID >= 'C' && byZoneID <= 'M')
			{
				bNorth = FALSE;
			}
			else
			{
				// yikes, most likely we got something that was not really
				// an MGRS zone code so we ignore it.
			}
		}

		setUTM(iZone, bNorth);
	}

	else if (cslNIEqualString(pszProj, "VDG", 3))
	{
		setVDG(padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
	}

	else
	{
		gtl::debug("OSR_PCI", "Unsupported projection: %s", pszProj);
		setLocalCS(pszProj);
	}

	/* ==================================================================== */
	/*      Translate the datum/spheroid.                                   */
	/* ==================================================================== */

	/* -------------------------------------------------------------------- */
	/*      We have an earthmodel string, look it up in the datum list.     */
	/* -------------------------------------------------------------------- */
	if (strlen(szEarthModel) > 0
		&& (poRoot == NULL || isProjected() || isGeographic()))
	{
		const PCIDatums   *pasDatum = asDatums;

		// Search for matching datum
		while (pasDatum->pszPCIDatum)
		{
			if (cslNIEqualString(szEarthModel, pasDatum->pszPCIDatum, 4))
			{
				SpatialReference oGCS;
				oGCS.importFromEPSG(pasDatum->nEPSGCode);
				copyGeogCSFrom(&oGCS);
				break;
			}
			pasDatum++;
		}

		/* -------------------------------------------------------------------- */
		/*      If we did not find a datum definition in our incode epsg        */
		/*      lookup table, then try fetching from the pci_datum.txt          */
		/*      file.                                                           */
		/* -------------------------------------------------------------------- */
		char **papszDatumDefn = NULL;

		if (!pasDatum->pszPCIDatum && szEarthModel[0] == 'D')
		{
			const char *pszDatumCSV = gtl::getDataFile("pci_datum.txt").c_str();
			bool fp = false;
			CommaSeparatedValues csv;
			if (pszDatumCSV)
				fp = csv.load(pszDatumCSV);

			if (fp)
			{
				char **papszLineItems = NULL;
				int ic = csv.getLineCount();
				int i = 0;
				while (
					((papszLineItems = csv.parseLine(i)) != NULL)
					&& (i<ic)
					)
				{
					i++;
					if (cslCount(papszLineItems) > 3
						&& cslNIEqualString(papszLineItems[0], szEarthModel, 4))
					{
						papszDatumDefn = papszLineItems;
						strncpy(szEarthModel, papszLineItems[2], 4);
						break;
					}
					cslDestroy(papszLineItems);
				}

				csv.clear();
			}
		}

		/* -------------------------------------------------------------------- */
		/*      If not, look in the ellipsoid/EPSG matching list.               */
		/* -------------------------------------------------------------------- */
		if (!pasDatum->pszPCIDatum)  // No matching; search for ellipsoids
		{
			char    *pszName = NULL;
			double  dfSemiMajor = 0.0;
			double  dfInvFlattening = 0.0;
			int     nEPSGCode = 0;

			pasDatum = asEllips;

			while (pasDatum->pszPCIDatum)
			{
				if (cslNIEqualString(szEarthModel, pasDatum->pszPCIDatum, 4))
				{
					nEPSGCode = pasDatum->nEPSGCode;
					getEllipsoidInfo(pasDatum->nEPSGCode, &pszName,
						&dfSemiMajor, &dfInvFlattening);
					break;

				}
				pasDatum++;
			}

			/* -------------------------------------------------------------------- */
			/*      If we don't find it in that list, do a lookup in the            */
			/*      pci_ellips.txt file.                                            */
			/* -------------------------------------------------------------------- */
			if (!pasDatum->pszPCIDatum && szEarthModel[0] == 'E')
			{
				const char *pszCSV = gtl::getDataFile("pci_ellips.txt").c_str();
				bool fp = false;
				CommaSeparatedValues csv;
				if (pszCSV)
					fp = csv.load(pszCSV);

				if (fp )
				{
					char **papszLineItems = NULL;
					int ic = csv.getLineCount();
					int i = 0;
					while (((papszLineItems = csv.parseLine(i)) != NULL)&&(i<ic))
					{
						i++;
						if (cslCount(papszLineItems) > 3
							&& cslNIEqualString(papszLineItems[0], szEarthModel, 4))
						{
							dfSemiMajor = atof(papszLineItems[2]);
							double dfSemiMinor = atof(papszLineItems[3]);

							if (fabs(dfSemiMajor - dfSemiMinor) < 0.01)
								dfInvFlattening = 0.0;
							else
								dfInvFlattening =
								dfSemiMajor / (dfSemiMajor - dfSemiMinor);
							break;
						}
						cslDestroy(papszLineItems);
					}
					cslDestroy(papszLineItems);

					csv.clear();
				}
			}

			/* -------------------------------------------------------------------- */
			/*      Custom spheroid?                                                */
			/* -------------------------------------------------------------------- */
			if (dfSemiMajor == 0.0 && cslNIEqualString(szEarthModel, "E999", 4)
				&& padfPrjParams[0] != 0.0)
			{
				dfSemiMajor = padfPrjParams[0];

				if (fabs(padfPrjParams[0] - padfPrjParams[1]) < 0.01)
				{
					dfInvFlattening = 0.0;
				}
				else
				{
					dfInvFlattening =
						padfPrjParams[0] / (padfPrjParams[0] - padfPrjParams[1]);
				}
			}

			/* -------------------------------------------------------------------- */
			/*      If nothing else, fall back to WGS84 parameters.                 */
			/* -------------------------------------------------------------------- */
			if (dfSemiMajor == 0.0)
			{
				dfSemiMajor = SRS_WGS84_SEMIMAJOR;
				dfInvFlattening = SRS_WGS84_INVFLATTENING;
			}

			/* -------------------------------------------------------------------- */
			/*      Now try to put this all together into a GEOGCS definition.      */
			/* -------------------------------------------------------------------- */
			String osGCSName, osDatumName, osEllipseName;

			if (pszName)
				osEllipseName = pszName;
			else
				osEllipseName.printf("Unknown - PCI %s", szEarthModel);
			free(pszName);

			if (papszDatumDefn)
				osDatumName = papszDatumDefn[1];
			else
				osDatumName.printf("Unknown - PCI %s", szEarthModel);
			osGCSName = osDatumName;

			setGeogCS(osGCSName, osDatumName, osEllipseName,
				dfSemiMajor, dfInvFlattening);

			// Do we have an ellipsoid EPSG code?
			if (nEPSGCode != 0)
				setAuthority("SPHEROID", "EPSG", nEPSGCode);

			// Do we have 7 datum shift parameters?
			if (cslCount(papszDatumDefn) >= 15
				&& atof(papszDatumDefn[14]) != 0.0)
			{
				double dfScale = atof(papszDatumDefn[14]);

				// we want scale in parts per million off 1.0
				// but pci uses a mix of forms. 
				if (dfScale >= 0.999 && dfScale <= 1.001)
					dfScale = (dfScale - 1.0) * 1000000.0;

				setTOWGS84(atof(papszDatumDefn[3]),
					atof(papszDatumDefn[4]),
					atof(papszDatumDefn[5]),
					atof(papszDatumDefn[11]),
					atof(papszDatumDefn[12]),
					atof(papszDatumDefn[13]),
					dfScale);
			}

			// Do we have 7 datum shift parameters?
			else if (cslCount(papszDatumDefn) == 11
				&& (atof(papszDatumDefn[3]) != 0.0
					|| atof(papszDatumDefn[4]) != 0.0
					|| atof(papszDatumDefn[5]) != 0.0))
			{
				setTOWGS84(atof(papszDatumDefn[3]),
					atof(papszDatumDefn[4]),
					atof(papszDatumDefn[5]));
			}
		}

		cslDestroy(papszDatumDefn);
	}

	/* -------------------------------------------------------------------- */
	/*      Grid units translation                                          */
	/* -------------------------------------------------------------------- */
	if ((isLocal() || isProjected()) && pszUnits)
	{
		if (cslIEqualString(pszUnits, "METRE"))
			setLinearUnits(SRS_UL_METER, 1.0);
		else if (cslIEqualString(pszUnits, "DEGREE"))
			setAngularUnits(SRS_UA_DEGREE, atof(SRS_UA_DEGREE_CONV));
		else
			setLinearUnits(SRS_UL_METER, 1.0);
	}

	fixupOrdering();

	if (bProjAllocated && padfPrjParams)
		free(padfPrjParams);

	return false;
}
 
/************************************************************************/
/*                           exportToPCI()                              */
/************************************************************************/

/**
* \brief Export coordinate system in PCI projection definition.
*
* Converts the loaded coordinate reference system into PCI projection
* definition to the extent possible. The strings returned in ppszProj,
* ppszUnits and ppadfPrjParams array should be deallocated by the caller
* with free() when no longer needed.
*
* LOCAL_CS coordinate systems are not translatable.  An empty string
* will be returned along with false.
*
* This method is the equivelent of the C function OSRExportToPCI().
*
* @param ppszProj pointer to which dynamically allocated PCI projection
* definition will be assigned.
*
* @param ppszUnits pointer to which dynamically allocated units definition
* will be assigned.
*
* @param ppadfPrjParams pointer to which dynamically allocated array of
* 17 projection parameters will be assigned. See importFromPCI() for the list
* of parameters.
*
* @return false on success or an error code on failure.
*/

bool SpatialReference::exportToPCI(char **ppszProj, char **ppszUnits,
	double **ppadfPrjParams) const

{
	const char  *pszProjection = getAttrValue("PROJECTION");

	/* -------------------------------------------------------------------- */
	/*      Fill all projection parameters with zero.                       */
	/* -------------------------------------------------------------------- */
	int         i;

	*ppadfPrjParams = (double *)malloc(17 * sizeof(double));
	for (i = 0; i < 17; i++)
		(*ppadfPrjParams)[i] = 0.0;

	/* -------------------------------------------------------------------- */
	/*      Get the prime meridian info.                                    */
	/* -------------------------------------------------------------------- */
	const SRSNode *poPRIMEM = getAttrNode("PRIMEM");
	double dfFromGreenwich = 0.0;

	if (poPRIMEM != NULL && poPRIMEM->getChildCount() >= 2
		&& atof(poPRIMEM->getChild(1)->getValue()) != 0.0)
	{
		dfFromGreenwich = atof(poPRIMEM->getChild(1)->getValue());
	}

	/* ==================================================================== */
	/*      Handle the projection definition.                               */
	/* ==================================================================== */
	char        szProj[17];

	memset(szProj, 0, sizeof(szProj));

	if (isLocal())
	{
		if (getLinearUnits() > 0.30479999 && getLinearUnits() < 0.3048010)
			cslFillString(szProj, "FEET", 17);
		else
			cslFillString(szProj, "METER", 17);
	}

	else if (pszProjection == NULL)
	{
		cslFillString(szProj, "LONG/LAT", 16);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ALBERS_CONIC_EQUAL_AREA))
	{
		cslFillString(szProj, "ACEA", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[4] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		(*ppadfPrjParams)[5] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_AZIMUTHAL_EQUIDISTANT))
	{
		cslFillString(szProj, "AE", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_CASSINI_SOLDNER))
	{
		cslFillString(szProj, "CASS", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIDISTANT_CONIC))
	{
		cslFillString(szProj, "EC", 16);
		(*ppadfPrjParams)[2] =
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[4] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		(*ppadfPrjParams)[5] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIRECTANGULAR))
	{
		cslFillString(szProj, "ER", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_GNOMONIC))
	{
		cslFillString(szProj, "GNO", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA))
	{
		cslFillString(szProj, "LAEA", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP))
	{
		cslFillString(szProj, "LCC", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[4] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0);
		(*ppadfPrjParams)[5] =
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP))
	{
		cslFillString(szProj, "LCC_1SP", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MILLER_CYLINDRICAL))
	{
		cslFillString(szProj, "MC", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MERCATOR_1SP))
	{
		cslFillString(szProj, "MER", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ORTHOGRAPHIC))
	{
		cslFillString(szProj, "OG", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_HOTINE_OBLIQUE_MERCATOR))
	{
		cslFillString(szProj, "OM", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[3] = getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[14] = getNormProjParm(SRS_PP_AZIMUTH, 0.0);
		// note we are ignoring rectified_grid_angle which has no pci analog.
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN))
	{
		cslFillString(szProj, "OM", 16);
		(*ppadfPrjParams)[3] = getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[11] = getNormProjParm(SRS_PP_LATITUDE_OF_POINT_1, 0.0);
		(*ppadfPrjParams)[10] = getNormProjParm(SRS_PP_LONGITUDE_OF_POINT_1, 0.0);
		(*ppadfPrjParams)[13] = getNormProjParm(SRS_PP_LATITUDE_OF_POINT_2, 0.0);
		(*ppadfPrjParams)[12] = getNormProjParm(SRS_PP_LONGITUDE_OF_POINT_2, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLYCONIC))
	{
		cslFillString(szProj, "PC", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLAR_STEREOGRAPHIC))
	{
		cslFillString(szProj, "PS", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ROBINSON))
	{
		cslFillString(szProj, "ROB", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_OBLIQUE_STEREOGRAPHIC))
	{
		cslFillString(szProj, "SGDO", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_STEREOGRAPHIC))
	{
		cslFillString(szProj, "SG", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[3] =
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_SINUSOIDAL))
	{
		cslFillString(szProj, "SIN", 16);
		(*ppadfPrjParams)[2] =
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_TRANSVERSE_MERCATOR))
	{
		int bNorth;
		int nZone = getUTMZone(&bNorth);

		if (nZone != 0)
		{
			cslFillString(szProj, "UTM", 16);
			if (bNorth)
				StringPrinter::print(szProj + 5, nZone, 4);
			else
				StringPrinter::print(szProj + 5, -nZone, 4);
		}
		else
		{
			cslFillString(szProj, "TM", 16);
			(*ppadfPrjParams)[2] =
				getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
			(*ppadfPrjParams)[3] =
				getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0);
			(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
			(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
			(*ppadfPrjParams)[8] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		}
	}

	else if (cslIEqualString(pszProjection, SRS_PT_VANDERGRINTEN))
	{
		cslFillString(szProj, "VDG", 16);
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0);
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	// Projection unsupported by PCI
	else
	{
		gtl::debug("OSR_PCI",
			"Projection \"%s\" unsupported by PCI. "
			"PIXEL value will be used.", pszProjection);
		cslFillString(szProj, "PIXEL", 16);
	}

	/* ==================================================================== */
	/*      Translate the earth model.                                      */
	/* ==================================================================== */

	/* -------------------------------------------------------------------- */
	/*      Is this a well known datum?                                     */
	/* -------------------------------------------------------------------- */
	const char  *pszDatum = getAttrValue("DATUM");
	char szEarthModel[5];

	memset(szEarthModel, 0, sizeof(szEarthModel));

	if (pszDatum == NULL || strlen(pszDatum) == 0)
		/* do nothing */;
	else if (cslIEqualString(pszDatum, SRS_DN_NAD27))
		cslFillString(szEarthModel, "D-01", 4);

	else if (cslIEqualString(pszDatum, SRS_DN_NAD83))
		cslFillString(szEarthModel, "D-02", 4);

	else if (cslIEqualString(pszDatum, SRS_DN_WGS84))
		cslFillString(szEarthModel, "D000", 4);

	/* -------------------------------------------------------------------- */
	/*      If not a very well known datum, try for an EPSG based           */
	/*      translation.                                                    */
	/* -------------------------------------------------------------------- */
	if (szEarthModel[0] == '\0')
	{
		const char *pszAuthority = getAuthorityName("GEOGCS");

		if (pszAuthority && cslIEqualString(pszAuthority, "EPSG"))
		{
			int nGCS_EPSG = atoi(getAuthorityCode("GEOGCS"));
			int i;

			for (i = 0; asDatums[i].nEPSGCode != 0; i++)
			{
				if (asDatums[i].nEPSGCode == nGCS_EPSG)
				{
					strncpy(szEarthModel, asDatums[i].pszPCIDatum, 5);
					break;
				}
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      If we haven't found something yet, try translating the          */
	/*      ellipsoid.                                                      */
	/* -------------------------------------------------------------------- */
	if (szEarthModel[0] == '\0')
	{
		double      dfSemiMajor = getSemiMajor();
		double      dfInvFlattening = getInvFlattening();

		const PCIDatums   *pasDatum = asEllips;

		while (pasDatum->pszPCIDatum)
		{
			double  dfSM;
			double  dfIF;

			if (getEllipsoidInfo(pasDatum->nEPSGCode, NULL,
				&dfSM, &dfIF) == false
				&& doubleEqual(dfSemiMajor, dfSM)
				&& doubleEqual(dfInvFlattening, dfIF))
			{
				cslFillString(szEarthModel, pasDatum->pszPCIDatum, 4);
				break;
			}

			pasDatum++;
		}

		// Try to find in pci_ellips.txt
		if (szEarthModel[0] == '\0')
		{
			const char *pszCSV = gtl::getDataFile("pci_ellips.txt").c_str();
			bool fp = false;
			CommaSeparatedValues csv1;
			
			double dfSemiMinor;

			if (dfInvFlattening == 0.0)
				dfSemiMinor = dfSemiMajor;
			else
				dfSemiMinor = dfSemiMajor * (1.0 - 1.0 / dfInvFlattening);


			if (pszCSV)
				fp = csv1.load(pszCSV);

			if (fp)
			{
				char **papszLineItems = NULL;
				int ic = csv1.getLineCount();
				int i = 0;
				while (((papszLineItems = csv1.parseLine(i)) != NULL)&& i<ic)
				{
					i++;

					if (cslCount(papszLineItems) >= 4
						&& doubleEqual(dfSemiMajor, atof(papszLineItems[2]))
						&& doubleEqual(dfSemiMinor, atof(papszLineItems[3])))
					{
						strncpy(szEarthModel, papszLineItems[0], 5);
						break;
					}

					cslDestroy(papszLineItems);
					
				}

				cslDestroy(papszLineItems);

				csv1.clear();
			}
		}

		// custom ellipsoid parameters
		if (szEarthModel[0] == '\0')
		{
			cslFillString(szEarthModel, "E999", 4);
			(*ppadfPrjParams)[0] = dfSemiMajor;
			if (fabs(dfInvFlattening) < 0.000000000001)
			{
				(*ppadfPrjParams)[1] = dfSemiMajor;
			}
			else
			{
				(*ppadfPrjParams)[1] =
					dfSemiMajor * (1.0 - 1.0 / dfInvFlattening);
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      If we have a non-parameteric ellipsoid, scan the                */
	/*      pci_datum.txt for a match.                                      */
	/* -------------------------------------------------------------------- */
	if (szEarthModel[0] == 'E'
		&& !cslIEqualString(szEarthModel, "E999")
		&& pszDatum != NULL)
	{
		const char *pszDatumCSV = gtl::getDataFile("pci_datum.txt").c_str();
		bool fp = false;
		CommaSeparatedValues csv2;
		double adfTOWGS84[7];
		int    bHaveTOWGS84;

		bHaveTOWGS84 = (getTOWGS84(adfTOWGS84, 7) == false);

		if (pszDatumCSV)
			fp = csv2.load(pszDatumCSV);

		if (fp )
		{
			char **papszLineItems = NULL;
			int ic = csv2.getLineCount();
			int i = 0;
			while ( ((papszLineItems = csv2.parseLine(i)) != NULL) && i<ic)
			{
				i++;
				// Compare based on datum name.  This is mostly for
				// PCI round-tripping.  We won't usually get exact matches
				// from other sources.
				if (cslCount(papszLineItems) > 3
					&& cslIEqualString(papszLineItems[1], pszDatum)
					&& cslIEqualString(papszLineItems[2], szEarthModel))
				{
					strncpy(szEarthModel, papszLineItems[0], 5);
					break;
				}

				int bTOWGS84Match = bHaveTOWGS84;

				if (cslCount(papszLineItems) < 11)
					bTOWGS84Match = FALSE;

				if (bTOWGS84Match
					&& (!doubleEqual(adfTOWGS84[0], atof(papszLineItems[3]))
						|| !doubleEqual(adfTOWGS84[1], atof(papszLineItems[4]))
						|| !doubleEqual(adfTOWGS84[2], atof(papszLineItems[5]))))
					bTOWGS84Match = FALSE;

				if (bTOWGS84Match && cslCount(papszLineItems) >= 15
					&& (!doubleEqual(adfTOWGS84[3], atof(papszLineItems[11]))
						|| !doubleEqual(adfTOWGS84[4], atof(papszLineItems[12]))
						|| !doubleEqual(adfTOWGS84[5], atof(papszLineItems[13]))))
					bTOWGS84Match = FALSE;

				if (bTOWGS84Match && cslCount(papszLineItems) >= 15)
				{
					double dfScale = atof(papszLineItems[14]);

					// convert to parts per million if is a 1 based scaling.
					if (dfScale >= 0.999 && dfScale <= 1.001)
						dfScale = (dfScale - 1.0) * 1000000.0;

					if (!doubleEqual(adfTOWGS84[6], dfScale))
						bTOWGS84Match = FALSE;
				}

				if (bTOWGS84Match && cslCount(papszLineItems) < 15
					&& (!doubleEqual(adfTOWGS84[3], 0.0)
						|| !doubleEqual(adfTOWGS84[4], 0.0)
						|| !doubleEqual(adfTOWGS84[5], 0.0)
						|| !doubleEqual(adfTOWGS84[6], 0.0)))
					bTOWGS84Match = FALSE;

				if (bTOWGS84Match)
				{
					strncpy(szEarthModel, papszLineItems[0], 5);
					break;
				}

				cslDestroy(papszLineItems);
			}

			cslDestroy(papszLineItems);
		}
	}

	cslFillString(szProj + 12, szEarthModel, 4);

	gtl::debug("OSR_PCI", "Translated as '%s'", szProj);

	/* -------------------------------------------------------------------- */
	/*      Translate the linear units.                                     */
	/* -------------------------------------------------------------------- */
	const char  *pszUnits;

	if (cslNIEqualString(szProj, "LONG/LAT", 8))
		pszUnits = "DEGREE";
	else
		pszUnits = "METRE";

	/* -------------------------------------------------------------------- */
	/*      Report results.                                                 */
	/* -------------------------------------------------------------------- */
	szProj[16] = '\0';
	*ppszProj = cslDuplicateString(szProj);

	*ppszUnits = cslDuplicateString(pszUnits);

	return false;
}


end_gdb_namespace
end_gtl_namespace