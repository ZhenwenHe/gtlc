#include "spatialreference.h" 
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace 
/************************************************************************/
/*                            importFromOzi()                           */
/************************************************************************/

/**
* Note : This method is obsolete, but has been kept to avoid breaking the API.
*        It can be removed in GDAL 2.0
*/

/**
* Import coordinate system from OziExplorer projection definition.
*
* This method will import projection definition in style, used by
* OziExplorer software.
*
* This function is the equivalent of the C function OSRImportFromOzi().
*
* @param pszDatum Datum string. This is a fifth string in the
* OziExplorer .MAP file.
*
* @param pszProj Projection string. Search for line starting with
* "Map Projection" name in the OziExplorer .MAP file and supply it as a
* whole in this parameter.
*
* @param pszProjParms String containing projection parameters. Search for
* "Projection Setup" name in the OziExplorer .MAP file and supply it as a
* whole in this parameter.
*
* @return true on success or an error code in case of failure.
*
* @deprecated Use importFromOzi( const char * const* papszLines ) instead
*/

bool SpatialReference::importFromOzi(const char *pszDatum,
	const char *pszProj,
	const char *pszProjParms)

{
	const char* papszLines[8];

	// Fake
	papszLines[0] = "";
	papszLines[1] = "";
	papszLines[2] = "";
	papszLines[3] = "";
	papszLines[4] = pszDatum; /* Must be in that position */
	papszLines[5] = pszProj; /* Must be after 5th line */
	papszLines[6] = pszProjParms; /* Must be after 5th line */
	papszLines[7] = NULL;

	return importFromOzi(papszLines);
}

/************************************************************************/
/*                            importFromOzi()                           */
/************************************************************************/

/**
* Import coordinate system from OziExplorer projection definition.
*
* This method will import projection definition in style, used by
* OziExplorer software.
*
* @param papszLines Map file lines. This is an array of strings containing
* the whole OziExplorer .MAP file. The array is terminated by a NULL pointer.
*
* @return true on success or an error code in case of failure.
*
* @since OGR 1.10
*/

bool SpatialReference::importFromOzi(const char * const* papszLines)
{
	int iLine;
	const char *pszDatum, *pszProj = NULL, *pszProjParms = NULL;

	clear();

	int nLines = cslCount((char**)papszLines);
	if (nLines < 5)
		return false;

	pszDatum = papszLines[4];

	for (iLine = 5; iLine < nLines; iLine++)
	{
		if (cslNIEqualString(papszLines[iLine], "Map Projection", 14))
		{
			pszProj = papszLines[iLine];
		}
		else if (cslNIEqualString(papszLines[iLine], "Projection Setup", 16))
		{
			pszProjParms = papszLines[iLine];
		}
	}

	if (!(pszDatum && pszProj && pszProjParms))
		return false;

	/* -------------------------------------------------------------------- */
	/*      Operate on the basis of the projection name.                    */
	/* -------------------------------------------------------------------- */
	char    **papszProj = cslTokenizeString(pszProj, ",", TRUE, TRUE);
	char    **papszProjParms = cslTokenizeString(pszProjParms, ",",
		TRUE, TRUE);
	char    **papszDatum = NULL;

	if (cslCount(papszProj) < 2)
	{
		goto not_enough_data;
	}

	if (cslNIEqualString(papszProj[1], "Latitude/Longitude", 18))
	{
	}

	else if (cslNIEqualString(papszProj[1], "Mercator", 8))
	{
		if (cslCount(papszProjParms) < 6) goto not_enough_data;
		double dfScale = atof(papszProjParms[3]);
		if (papszProjParms[3][0] == 0) dfScale = 1; /* if unset, default to scale = 1 */
		setMercator(atof(papszProjParms[1]), atof(papszProjParms[2]),
			dfScale,
			atof(papszProjParms[4]), atof(papszProjParms[5]));
	}

	else if (cslNIEqualString(papszProj[1], "Transverse Mercator", 19))
	{
		if (cslCount(papszProjParms) < 6) goto not_enough_data;
		setTM(atof(papszProjParms[1]), atof(papszProjParms[2]),
			atof(papszProjParms[3]),
			atof(papszProjParms[4]), atof(papszProjParms[5]));
	}

	else if (cslNIEqualString(papszProj[1], "Lambert Conformal Conic", 23))
	{
		if (cslCount(papszProjParms) < 8) goto not_enough_data;
		setLCC(atof(papszProjParms[6]), atof(papszProjParms[7]),
			atof(papszProjParms[1]), atof(papszProjParms[2]),
			atof(papszProjParms[4]), atof(papszProjParms[5]));
	}

	else if (cslNIEqualString(papszProj[1], "Sinusoidal", 10))
	{
		if (cslCount(papszProjParms) < 6) goto not_enough_data;
		setSinusoidal(atof(papszProjParms[2]),
			atof(papszProjParms[4]), atof(papszProjParms[5]));
	}

	else if (cslNIEqualString(papszProj[1], "Albers Equal Area", 17))
	{
		if (cslCount(papszProjParms) < 8) goto not_enough_data;
		setACEA(atof(papszProjParms[6]), atof(papszProjParms[7]),
			atof(papszProjParms[1]), atof(papszProjParms[2]),
			atof(papszProjParms[4]), atof(papszProjParms[5]));
	}

	else if (cslNIEqualString(papszProj[1], "(UTM) Universal Transverse Mercator", 35) && nLines > 5)
	{
		/* Look for the UTM zone in the calibration point data */
		for (iLine = 5; iLine < nLines; iLine++)
		{
			if (cslNIEqualString(papszLines[iLine], "Point", 5))
			{
				char    **papszTok = NULL;
				papszTok = cslTokenizeString(papszLines[iLine], ",",
					CSLT_ALLOWEMPTYTOKENS
					| CSLT_STRIPLEADSPACES
					| CSLT_STRIPENDSPACES);
				if (cslCount(papszTok) < 17
					|| cslIEqualString(papszTok[2], "")
					|| cslIEqualString(papszTok[13], "")
					|| cslIEqualString(papszTok[14], "")
					|| cslIEqualString(papszTok[15], "")
					|| cslIEqualString(papszTok[16], ""))
				{
					cslDestroy(papszTok);
					continue;
				}
				setUTM(cslMultilingualStringToNumber(papszTok[13]), cslIEqualString(papszTok[16], "N"));
				cslDestroy(papszTok);
				break;
			}
		}
		if (iLine == nLines)    /* Try to guess the UTM zone */
		{
			float fMinLongitude = INT_MAX;
			float fMaxLongitude = INT_MIN;
			float fMinLatitude = INT_MAX;
			float fMaxLatitude = INT_MIN;
			int bFoundMMPLL = FALSE;
			for (iLine = 5; iLine < nLines; iLine++)
			{
				if (cslNIEqualString(papszLines[iLine], "MMPLL", 5))
				{
					char    **papszTok = NULL;
					papszTok = cslTokenizeString(papszLines[iLine], ",",
						CSLT_ALLOWEMPTYTOKENS
						| CSLT_STRIPLEADSPACES
						| CSLT_STRIPENDSPACES);
					if (cslCount(papszTok) < 4)
					{
						cslDestroy(papszTok);
						continue;
					}
					float fLongitude = cslMultilingualStringToNumber(papszTok[2]);
					float fLatitude = cslMultilingualStringToNumber(papszTok[3]);
					cslDestroy(papszTok);

					bFoundMMPLL = TRUE;

					if (fMinLongitude > fLongitude)
						fMinLongitude = fLongitude;
					if (fMaxLongitude < fLongitude)
						fMaxLongitude = fLongitude;
					if (fMinLatitude > fLatitude)
						fMinLatitude = fLatitude;
					if (fMaxLatitude < fLatitude)
						fMaxLatitude = fLatitude;
				}
			}
			float fMedianLatitude = (fMinLatitude + fMaxLatitude) / 2;
			float fMedianLongitude = (fMinLongitude + fMaxLongitude) / 2;
			if (bFoundMMPLL && fMaxLatitude <= 90)
			{
				int nUtmZone;
				if (fMedianLatitude >= 56 && fMedianLatitude <= 64 &&
					fMedianLongitude >= 3 && fMedianLongitude <= 12)
					nUtmZone = 32;                                             /* Norway exception */
				else if (fMedianLatitude >= 72 && fMedianLatitude <= 84 &&
					fMedianLongitude >= 0 && fMedianLongitude <= 42)
					nUtmZone = (int)((fMedianLongitude + 3) / 12) * 2 + 31;  /* Svalbard exception */
				else
					nUtmZone = (int)((fMedianLongitude + 180) / 6) + 1;
				setUTM(nUtmZone, fMedianLatitude >= 0);
			}
			else
				gtl::debug("OSR_Ozi", "UTM Zone not found");
		}
	}

	else if (cslNIEqualString(papszProj[1], "(I) France Zone I", 17))
	{
		setLCC1SP(49.5, 2.337229167, 0.99987734, 600000, 1200000);
	}

	else if (cslNIEqualString(papszProj[1], "(II) France Zone II", 19))
	{
		setLCC1SP(46.8, 2.337229167, 0.99987742, 600000, 2200000);
	}

	else if (cslNIEqualString(papszProj[1], "(III) France Zone III", 21))
	{
		setLCC1SP(44.1, 2.337229167, 0.99987750, 600000, 3200000);
	}

	else if (cslNIEqualString(papszProj[1], "(IV) France Zone IV", 19))
	{
		setLCC1SP(42.165, 2.337229167, 0.99994471, 234.358, 4185861.369);
	}

	/*
	*  Note : The following projections have not been implemented yet
	*
	*/

	/*
	else if ( cslNIEqualString(papszProj[1], "(BNG) British National Grid", 27) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(IG) Irish Grid", 15) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(NZG) New Zealand Grid", 22) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(NZTM2) New Zealand TM 2000", 27) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(SG) Swedish Grid", 27) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(SUI) Swiss Grid", 26) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(A)Lambert Azimuthual Equal Area", 32) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(EQC) Equidistant Conic", 23) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "Polyconic (American)", 20) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "Van Der Grinten", 15) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "Vertical Near-Sided Perspective", 31) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(WIV) Wagner IV", 15) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "Bonne", 5) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(MT0) Montana State Plane Zone 2500", 35) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "ITA1) Italy Grid Zone 1", 23) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "ITA2) Italy Grid Zone 2", 23) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(VICMAP-TM) Victoria Aust.(pseudo AMG)", 38) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "VICGRID) Victoria Australia", 27) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "(VG94) VICGRID94 Victoria Australia", 35) )
	{
	}

	else if ( cslNIEqualString(papszProj[1], "Gnomonic", 8) )
	{
	}
	*/

	else
	{
		gtl::debug("OSR_Ozi", "Unsupported projection: \"%s\"", papszProj[1]);
		setLocalCS(String().printf("\"Ozi\" projection \"%s\"",
			papszProj[1]));
	}

	/* -------------------------------------------------------------------- */
	/*      Try to translate the datum/spheroid.                            */
	/* -------------------------------------------------------------------- */
	papszDatum = cslTokenizeString(pszDatum, ",",
		CSLT_ALLOWEMPTYTOKENS
		| CSLT_STRIPLEADSPACES
		| CSLT_STRIPENDSPACES);
	if (papszDatum == NULL)
		goto not_enough_data;

	if (!isLocal())
	{

		/* -------------------------------------------------------------------- */
		/*      Verify that we can find the CSV file containing the datums      */
		/* -------------------------------------------------------------------- */
		const char *pszOziDatum = gtl::getDataFile("ozi_datum.csv").c_str();
		CommaSeparatedValues csv(pszOziDatum);

		if (csv.getLine("EPSG_DATUM_CODE","4326", CC_Integer) == NULL)
		{
			gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_OPENFAILED,
				"Unable to open OZI support file %s.\n"
				"Try setting the GDAL_DATA environment variable to point\n"
				"to the directory containing OZI csv files.",pszOziDatum);
			goto other_error;
		}

		/* -------------------------------------------------------------------- */
		/*      Search for matching datum                                       */
		/* -------------------------------------------------------------------- */
		
		//String osDName = CSVGetField(pszOziDatum, "NAME", papszDatum[0],
		//	CC_ApproxString, "NAME");
		String osDName= csv.getValue("NAME", papszDatum[0], CC_ApproxString, "NAME");
		if (strlen(osDName) == 0)
		{
			gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
				"Failed to find datum %s in ozi_datum.csv.",
				papszDatum[0]);
			goto other_error;
		}

		int nDatumCode = atoi(csv.getValue("NAME", papszDatum[0],
			CC_ApproxString, "EPSG_DATUM_CODE").c_str());

		if (nDatumCode > 0) // There is a matching EPSG code
		{
			SpatialReference oGCS;
			oGCS.importFromEPSG(nDatumCode);
			copyGeogCSFrom(&oGCS);
		}
		else // We use the parameters from the CSV files
		{
			String osEllipseCode = csv.getValue("NAME", papszDatum[0],
				CC_ApproxString, "ELLIPSOID_CODE");
			double dfDeltaX = atof(csv.getValue("NAME", papszDatum[0],
				CC_ApproxString, "DELTAX").c_str());
			double dfDeltaY = atof(csv.getValue("NAME", papszDatum[0],
				CC_ApproxString, "DELTAY").c_str());
			double dfDeltaZ = atof(csv.getValue("NAME", papszDatum[0],
				CC_ApproxString, "DELTAZ").c_str());


			/* -------------------------------------------------------------------- */
			/*      Verify that we can find the CSV file containing the ellipsoids  */
			/* -------------------------------------------------------------------- */ 
			const char *pszOziEllipse = gtl::getDataFile("ozi_ellips.csv").c_str();
			CommaSeparatedValues csv2(pszOziEllipse);
			if (csv2.getLine("ELLIPSOID_CODE",
				"20", CC_Integer) == NULL)
			{
				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_OPENFAILED,
					"Unable to open OZI support file %s.\n"
					"Try setting the GDAL_DATA environment variable to point\n"
					"to the directory containing OZI csv files.",
					pszOziEllipse);
				goto other_error;
			}

			/* -------------------------------------------------------------------- */
			/*      Lookup the ellipse code.                                        */
			/* -------------------------------------------------------------------- */
			 
			String osEName = csv2.getValue("ELLIPSOID_CODE", osEllipseCode,
				CC_ApproxString, "NAME");
			if (strlen(osEName) == 0)
			{
				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
					"Failed to find ellipsoid %s in ozi_ellips.csv.",
					osEllipseCode.c_str());
				goto other_error;
			}

			double dfA = atof(csv2.getValue("ELLIPSOID_CODE", osEllipseCode,
				CC_ApproxString, "A").c_str());
			double dfInvF = atof(csv2.getValue("ELLIPSOID_CODE", osEllipseCode,
				CC_ApproxString, "INVF").c_str());

			/* -------------------------------------------------------------------- */
			/*      Create geographic coordinate system.                            */
			/* -------------------------------------------------------------------- */

			setGeogCS(osDName, osDName, osEName, dfA, dfInvF);
			setTOWGS84(dfDeltaX, dfDeltaY, dfDeltaZ);

		}
	}

	/* -------------------------------------------------------------------- */
	/*      Grid units translation                                          */
	/* -------------------------------------------------------------------- */
	if (isLocal() || isProjected())
		setLinearUnits(SRS_UL_METER, 1.0);

	fixupOrdering();

	cslDestroy(papszProj);
	cslDestroy(papszProjParms);
	cslDestroy(papszDatum);

	return true;

not_enough_data:

	cslDestroy(papszProj);
	cslDestroy(papszProjParms);
	cslDestroy(papszDatum);

	return false;

other_error:

	cslDestroy(papszProj);
	cslDestroy(papszProjParms);
	cslDestroy(papszDatum);

	return false;
}


end_gdb_namespace
end_gtl_namespace