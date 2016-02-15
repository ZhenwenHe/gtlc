#include "spatialreference.h"
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace

/************************************************************************/
/*  GCTP projection codes.                                              */
/************************************************************************/

#define GEO     0L      // Geographic
#define UTM     1L      // Universal Transverse Mercator (UTM)
#define SPCS    2L      // State Plane Coordinates
#define ALBERS  3L      // Albers Conical Equal Area
#define LAMCC   4L      // Lambert Conformal Conic
#define MERCAT  5L      // Mercator
#define PS      6L      // Polar Stereographic
#define POLYC   7L      // Polyconic
#define EQUIDC  8L      // Equidistant Conic
#define TM      9L      // Transverse Mercator
#define STEREO  10L     // Stereographic
#define LAMAZ   11L     // Lambert Azimuthal Equal Area
#define AZMEQD  12L     // Azimuthal Equidistant
#define GNOMON  13L     // Gnomonic
#define ORTHO   14L     // Orthographic
#define GVNSP   15L     // General Vertical Near-Side Perspective
#define SNSOID  16L     // Sinusiodal
#define EQRECT  17L     // Equirectangular
#define MILLER  18L     // Miller Cylindrical
#define VGRINT  19L     // Van der Grinten
#define HOM     20L     // (Hotine) Oblique Mercator 
#define ROBIN   21L     // Robinson
#define SOM     22L     // Space Oblique Mercator (SOM)
#define ALASKA  23L     // Alaska Conformal
#define GOODE   24L     // Interrupted Goode Homolosine 
#define MOLL    25L     // Mollweide
#define IMOLL   26L     // Interrupted Mollweide
#define HAMMER  27L     // Hammer
#define WAGIV   28L     // Wagner IV
#define WAGVII  29L     // Wagner VII
#define OBEQA   30L     // Oblated Equal Area
#define ISINUS1 31L     // Integerized Sinusoidal Grid (the same as 99)
#define CEA     97L     // Cylindrical Equal Area (Grid corners set
// in meters for EASE grid) 
#define BCEA    98L     // Cylindrical Equal Area (Grid corners set
// in DMS degs for EASE grid) 
#define ISINUS  99L     // Integerized Sinusoidal Grid
// (added by Raj Gejjagaraguppe ARC for MODIS) 

/************************************************************************/
/*  GCTP ellipsoid codes.                                               */
/************************************************************************/

#define CLARKE1866          0L
#define CLARKE1880          1L
#define BESSEL              2L
#define INTERNATIONAL1967   3L
#define INTERNATIONAL1909   4L
#define WGS72               5L
#define EVEREST             6L
#define WGS66               7L
#define GRS1980             8L
#define AIRY                9L
#define MODIFIED_EVEREST    10L
#define MODIFIED_AIRY       11L
#define WGS84               12L
#define SOUTHEAST_ASIA      13L
#define AUSTRALIAN_NATIONAL 14L
#define KRASSOVSKY          15L
#define HOUGH               16L
#define MERCURY1960         17L
#define MODIFIED_MERCURY    18L
#define SPHERE              19L

/************************************************************************/
/*  Correspondence between GCTP and EPSG ellipsoid codes.               */
/************************************************************************/

static const long aoEllips[] =
{
	7008,   // Clarke, 1866 (NAD1927)
	7034,   // Clarke, 1880
	7004,   // Bessel, 1841
	0,// FIXME: New International, 1967 --- skipped
	7022,   // International, 1924 (Hayford, 1909) XXX?
	7043,   // WGS, 1972
	7042,   // Everest, 1830
	7025,   // FIXME: WGS, 1966
	7019,   // GRS, 1980 (NAD1983)
	7001,   // Airy, 1830
	7018,   // Modified Everest
	7002,   // Modified Airy
	7030,   // WGS, 1984 (GPS)
	0,// FIXME: Southeast Asia --- skipped
	7003,   // Australian National, 1965
	7024,   // Krassovsky, 1940
	7053,   // Hough
	0,// FIXME: Mercury, 1960 --- skipped
	0,// FIXME: Modified Mercury, 1968 --- skipped
	7047,   // Sphere, rad 6370997 m (normal sphere)
	7006,   // Bessel, 1841 (Namibia)
	7016,   // Everest (Sabah & Sarawak)
	7044,   // Everest, 1956OGRSpatialReferenceUSGSUnpackRadian
	7056,   // Everest, Malaysia 1969
	7018,   // Everest, Malay & Singapr 1948
	0,// FIXME: Everest, Pakistan --- skipped
	7022,   // Hayford (International 1924) XXX?
	7020,   // Helmert 1906
	7021,   // Indonesian, 1974
	7036,   // South American, 1969
	0// FIXME: WGS 60 --- skipped
};

#define NUMBER_OF_ELLIPSOIDS    (int)(sizeof(aoEllips)/sizeof(aoEllips[0]))

extern bool
getEllipsoidInfo(int nCode, char ** ppszName,
	double * pdfSemiMajor, double * pdfInvFlattening);

static double SpatialReferenceUSGSUnpackNoOp(double dfVal)
{
	return dfVal;
}

static double SpatialReferenceUSGSUnpackRadian(double dfVal)
{
	return (dfVal * 180.0 / PI);
}

/************************************************************************/
/*                          importFromUSGS()                            */
/************************************************************************/

/**
* \brief Import coordinate system from USGS projection definition.
*
* This method will import projection definition in style, used by USGS GCTP
* software. GCTP operates on angles in packed DMS format (see
* degreeToPackedDMS() function for details), so all angle values (latitudes,
* longitudes, azimuths, etc.) specified in the padfPrjParams array should
* be in the packed DMS format, unless bAnglesInPackedDMSFormat is set to FALSE.
*
* This function is the equivalent of the C function OSRImportFromUSGS().
* Note that the bAnglesInPackedDMSFormat parameter is only present in the C++
* method. The C function assumes bAnglesInPackedFormat = TRUE.
*
* @param iProjSys Input projection system code, used in GCTP.
*
* @param iZone Input zone for UTM and State Plane projection systems. For
* Southern Hemisphere UTM use a negative zone code. iZone ignored for all
* other projections.
*
* @param padfPrjParams Array of 15 coordinate system parameters. These
* parameters differs for different projections.
*
*        <h4>Projection Transformation Package Projection Parameters</h4>
* <pre>
* ----------------------------------------------------------------------------
*                         |                    Array Element
*  Code & Projection Id   |---------------------------------------------------
*                         |   0  |   1  |  2   |  3   |   4   |    5    |6 | 7
* ----------------------------------------------------------------------------
*  0 Geographic           |      |      |      |      |       |         |  |
*  1 U T M                |Lon/Z |Lat/Z |      |      |       |         |  |
*  2 State Plane          |      |      |      |      |       |         |  |
*  3 Albers Equal Area    |SMajor|SMinor|STDPR1|STDPR2|CentMer|OriginLat|FE|FN
*  4 Lambert Conformal C  |SMajor|SMinor|STDPR1|STDPR2|CentMer|OriginLat|FE|FN
*  5 Mercator             |SMajor|SMinor|      |      |CentMer|TrueScale|FE|FN
*  6 Polar Stereographic  |SMajor|SMinor|      |      |LongPol|TrueScale|FE|FN
*  7 Polyconic            |SMajor|SMinor|      |      |CentMer|OriginLat|FE|FN
*  8 Equid. Conic A       |SMajor|SMinor|STDPAR|      |CentMer|OriginLat|FE|FN
*    Equid. Conic B       |SMajor|SMinor|STDPR1|STDPR2|CentMer|OriginLat|FE|FN
*  9 Transverse Mercator  |SMajor|SMinor|Factor|      |CentMer|OriginLat|FE|FN
* 10 Stereographic        |Sphere|      |      |      |CentLon|CenterLat|FE|FN
* 11 Lambert Azimuthal    |Sphere|      |      |      |CentLon|CenterLat|FE|FN
* 12 Azimuthal            |Sphere|      |      |      |CentLon|CenterLat|FE|FN
* 13 Gnomonic             |Sphere|      |      |      |CentLon|CenterLat|FE|FN
* 14 Orthographic         |Sphere|      |      |      |CentLon|CenterLat|FE|FN
* 15 Gen. Vert. Near Per  |Sphere|      |Height|      |CentLon|CenterLat|FE|FN
* 16 Sinusoidal           |Sphere|      |      |      |CentMer|         |FE|FN
* 17 Equirectangular      |Sphere|      |      |      |CentMer|TrueScale|FE|FN
* 18 Miller Cylindrical   |Sphere|      |      |      |CentMer|         |FE|FN
* 19 Van der Grinten      |Sphere|      |      |      |CentMer|OriginLat|FE|FN
* 20 Hotin Oblique Merc A |SMajor|SMinor|Factor|      |       |OriginLat|FE|FN
*    Hotin Oblique Merc B |SMajor|SMinor|Factor|AziAng|AzmthPt|OriginLat|FE|FN
* 21 Robinson             |Sphere|      |      |      |CentMer|         |FE|FN
* 22 Space Oblique Merc A |SMajor|SMinor|      |IncAng|AscLong|         |FE|FN
*    Space Oblique Merc B |SMajor|SMinor|Satnum|Path  |       |         |FE|FN
* 23 Alaska Conformal     |SMajor|SMinor|      |      |       |         |FE|FN
* 24 Interrupted Goode    |Sphere|      |      |      |       |         |  |
* 25 Mollweide            |Sphere|      |      |      |CentMer|         |FE|FN
* 26 Interrupt Mollweide  |Sphere|      |      |      |       |         |  |
* 27 Hammer               |Sphere|      |      |      |CentMer|         |FE|FN
* 28 Wagner IV            |Sphere|      |      |      |CentMer|         |FE|FN
* 29 Wagner VII           |Sphere|      |      |      |CentMer|         |FE|FN
* 30 Oblated Equal Area   |Sphere|      |Shapem|Shapen|CentLon|CenterLat|FE|FN
* ----------------------------------------------------------------------------
*
*       ----------------------------------------------------
*                               |      Array Element       |
*         Code & Projection Id  |---------------------------
*                               |  8  |  9 |  10 | 11 | 12 |
*       ----------------------------------------------------
*        0 Geographic           |     |    |     |    |    |
*        1 U T M                |     |    |     |    |    |
*        2 State Plane          |     |    |     |    |    |
*        3 Albers Equal Area    |     |    |     |    |    |
*        4 Lambert Conformal C  |     |    |     |    |    |
*        5 Mercator             |     |    |     |    |    |
*        6 Polar Stereographic  |     |    |     |    |    |
*        7 Polyconic            |     |    |     |    |    |
*        8 Equid. Conic A       |zero |    |     |    |    |
*          Equid. Conic B       |one  |    |     |    |    |
*        9 Transverse Mercator  |     |    |     |    |    |
*       10 Stereographic        |     |    |     |    |    |
*       11 Lambert Azimuthal    |     |    |     |    |    |
*       12 Azimuthal            |     |    |     |    |    |
*       13 Gnomonic             |     |    |     |    |    |
*       14 Orthographic         |     |    |     |    |    |
*       15 Gen. Vert. Near Per  |     |    |     |    |    |
*       16 Sinusoidal           |     |    |     |    |    |
*       17 Equirectangular      |     |    |     |    |    |
*       18 Miller Cylindrical   |     |    |     |    |    |
*       19 Van der Grinten      |     |    |     |    |    |
*       20 Hotin Oblique Merc A |Long1|Lat1|Long2|Lat2|zero|
*          Hotin Oblique Merc B |     |    |     |    |one |
*       21 Robinson             |     |    |     |    |    |
*       22 Space Oblique Merc A |PSRev|LRat|PFlag|    |zero|
*          Space Oblique Merc B |     |    |     |    |one |
*       23 Alaska Conformal     |     |    |     |    |    |
*       24 Interrupted Goode    |     |    |     |    |    |
*       25 Mollweide            |     |    |     |    |    |
*       26 Interrupt Mollweide  |     |    |     |    |    |
*       27 Hammer               |     |    |     |    |    |
*       28 Wagner IV            |     |    |     |    |    |
*       29 Wagner VII           |     |    |     |    |    |
*       30 Oblated Equal Area   |Angle|    |     |    |    |
*       ----------------------------------------------------
*
*   where
*
*    Lon/Z     Longitude of any point in the UTM zone or zero.  If zero,
*              a zone code must be specified.
*    Lat/Z     Latitude of any point in the UTM zone or zero.  If zero, a
*              zone code must be specified.
*    SMajor    Semi-major axis of ellipsoid.  If zero, Clarke 1866 in meters
*              is assumed.
*    SMinor    Eccentricity squared of the ellipsoid if less than zero,
*              if zero, a spherical form is assumed, or if greater than
*              zero, the semi-minor axis of ellipsoid.
*    Sphere    Radius of reference sphere.  If zero, 6370997 meters is used.
*    STDPAR    Latitude of the standard parallel
*    STDPR1    Latitude of the first standard parallel
*    STDPR2    Latitude of the second standard parallel
*    CentMer   Longitude of the central meridian
*    OriginLat Latitude of the projection origin
*    FE        False easting in the same units as the semi-major axis
*    FN        False northing in the same units as the semi-major axis
*    TrueScale Latitude of true scale
*    LongPol   Longitude down below pole of map
*    Factor    Scale factor at central meridian (Transverse Mercator) or
*              center of projection (Hotine Oblique Mercator)
*    CentLon   Longitude of center of projection
*    CenterLat Latitude of center of projection
*    Height    Height of perspective point
*    Long1     Longitude of first point on center line (Hotine Oblique
*              Mercator, format A)
*    Long2     Longitude of second point on center line (Hotine Oblique
*              Mercator, format A)
*    Lat1      Latitude of first point on center line (Hotine Oblique
*              Mercator, format A)
*    Lat2      Latitude of second point on center line (Hotine Oblique
*              Mercator, format A)
*    AziAng    Azimuth angle east of north of center line (Hotine Oblique
*              Mercator, format B)
*    AzmthPt   Longitude of point on central meridian where azimuth occurs
*              (Hotine Oblique Mercator, format B)
*    IncAng    Inclination of orbit at ascending node, counter-clockwise
*              from equator (SOM, format A)
*    AscLong   Longitude of ascending orbit at equator (SOM, format A)
*    PSRev     Period of satellite revolution in minutes (SOM, format A)
*    LRat      Landsat ratio to compensate for confusion at northern end
*              of orbit (SOM, format A -- use 0.5201613)
*    PFlag     End of path flag for Landsat:  0 = start of path,
*              1 = end of path (SOM, format A)
*    Satnum    Landsat Satellite Number (SOM, format B)
*    Path      Landsat Path Number (Use WRS-1 for Landsat 1, 2 and 3 and
*              WRS-2 for Landsat 4, 5 and 6.)  (SOM, format B)
*    Shapem    Oblated Equal Area oval shape parameter m
*    Shapen    Oblated Equal Area oval shape parameter n
*    Angle     Oblated Equal Area oval rotation angle
*
* Array elements 13 and 14 are set to zero. All array elements with blank
* fields are set to zero too.
* </pre>
*
* @param iDatum Input spheroid.<p>
*
* If the datum code is negative, the first two values in the parameter array
* (parm) are used to define the values as follows:
*
* <ul>
*
* <li> If padfPrjParams[0] is a non-zero value and padfPrjParams[1] is
* greater than one, the semimajor axis is set to padfPrjParams[0] and
* the semiminor axis is set to padfPrjParams[1].
*
* <li> If padfPrjParams[0] is nonzero and padfPrjParams[1] is greater than
* zero but less than or equal to one, the semimajor axis is set to
* padfPrjParams[0] and the semiminor axis is computed from the eccentricity
* squared value padfPrjParams[1]:<p>
*
* semiminor = sqrt(1.0 - ES) * semimajor<p>
*
* where<p>
*
* ES = eccentricity squared
*
* <li> If padfPrjParams[0] is nonzero and padfPrjParams[1] is equal to zero,
* the semimajor axis and semiminor axis are set to padfPrjParams[0].
*
* <li> If padfPrjParams[0] equals zero and padfPrjParams[1] is greater than
* zero, the default Clarke 1866 is used to assign values to the semimajor
* axis and semiminor axis.
*
* <li> If padfPrjParams[0] and padfPrjParams[1] equals zero, the semimajor
* axis is set to 6370997.0 and the semiminor axis is set to zero.
*
* </ul>
*
* If a datum code is zero or greater, the semimajor and semiminor axis are
* defined by the datum code as found in the following table:
*
*      <h4>Supported Datums</h4>
* <pre>
*       0: Clarke 1866 (default)
*       1: Clarke 1880
*       2: Bessel
*       3: International 1967
*       4: International 1909
*       5: WGS 72
*       6: Everest
*       7: WGS 66
*       8: GRS 1980/WGS 84
*       9: Airy
*      10: Modified Everest
*      11: Modified Airy
*      12: Walbeck
*      13: Southeast Asia
*      14: Australian National
*      15: Krassovsky
*      16: Hough
*      17: Mercury 1960
*      18: Modified Mercury 1968
*      19: Sphere of Radius 6370997 meters
* </pre>
*
* @param nUSGSAngleFormat one of USGS_ANGLE_DECIMALDEGREES, USGS_ANGLE_PACKEDDMS, or USGS_ANGLE_RADIANS (default is USGS_ANGLE_PACKEDDMS).
*
* @return true on success or an error code in case of failure.
*/

bool SpatialReference::importFromUSGS(long iProjSys, long iZone,
	double *padfPrjParams,
	long iDatum,
	int nUSGSAngleFormat)

{
	if (!padfPrjParams)
		return false;

	double(*pfnUnpackAnglesFn)(double) = NULL;

	if (nUSGSAngleFormat == USGS_ANGLE_DECIMALDEGREES)
		pfnUnpackAnglesFn = SpatialReferenceUSGSUnpackNoOp;
	else if (nUSGSAngleFormat == USGS_ANGLE_RADIANS)
		pfnUnpackAnglesFn = SpatialReferenceUSGSUnpackRadian;
	else
		pfnUnpackAnglesFn = packedDMSToDegree;

	/* -------------------------------------------------------------------- */
	/*      Operate on the basis of the projection code.                    */
	/* -------------------------------------------------------------------- */
	switch (iProjSys)
	{
	case GEO:
		break;

	case UTM:
	{
		int bNorth = TRUE;

		if (!iZone)
		{
			if (padfPrjParams[2] != 0.0)
				iZone = (long)padfPrjParams[2];
			else if (padfPrjParams[0] != 0.0 && padfPrjParams[1] != 0.0)
			{
				iZone = (long)(((pfnUnpackAnglesFn(padfPrjParams[0])
					+ 180.0) / 6.0) + 1.0);
				if (pfnUnpackAnglesFn(padfPrjParams[0]) < 0)
					bNorth = FALSE;
			}
		}

		if (iZone < 0)
		{
			iZone = -iZone;
			bNorth = FALSE;
		}
		setUTM(iZone, bNorth);
	}
	break;

	case SPCS:
	{
		int bNAD83 = TRUE;

		if (iDatum == 0)
			bNAD83 = FALSE;
		else if (iDatum != 8)
			gtl::error(gtl::ERRORTYPE::GET_WARNING, gtl::ERRORCODE::GEC_APPDEFINED,
				"Wrong datum for State Plane projection %d. "
				"Should be 0 or 8.", (int)iDatum);

		setStatePlane(iZone, bNAD83);
	}
	break;

	case ALBERS:
		setACEA(pfnUnpackAnglesFn(padfPrjParams[2]),
			pfnUnpackAnglesFn(padfPrjParams[3]),
			pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case LAMCC:
		setLCC(pfnUnpackAnglesFn(padfPrjParams[2]),
			pfnUnpackAnglesFn(padfPrjParams[3]),
			pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case MERCAT:
		setMercator(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			1.0,
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case PS:
		setPS(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			1.0,
			padfPrjParams[6], padfPrjParams[7]);

		break;

	case POLYC:
		setPolyconic(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case EQUIDC:
		if (padfPrjParams[8])
		{
			setEC(pfnUnpackAnglesFn(padfPrjParams[2]),
				pfnUnpackAnglesFn(padfPrjParams[3]),
				pfnUnpackAnglesFn(padfPrjParams[5]),
				pfnUnpackAnglesFn(padfPrjParams[4]),
				padfPrjParams[6], padfPrjParams[7]);
		}
		else
		{
			setEC(pfnUnpackAnglesFn(padfPrjParams[2]),
				pfnUnpackAnglesFn(padfPrjParams[2]),
				pfnUnpackAnglesFn(padfPrjParams[5]),
				pfnUnpackAnglesFn(padfPrjParams[4]),
				padfPrjParams[6], padfPrjParams[7]);
		}
		break;

	case TM:
		setTM(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[2],
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case STEREO:
		setStereographic(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			1.0,
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case LAMAZ:
		setLAEA(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case AZMEQD:
		setAE(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case GNOMON:
		setGnomonic(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case ORTHO:
		setOrthographic(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

		// FIXME: GVNSP --- General Vertical Near-Side Perspective skipped

	case SNSOID:
		setSinusoidal(pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case EQRECT:
		setEquirectangular2(0.0,
			pfnUnpackAnglesFn(padfPrjParams[4]),
			pfnUnpackAnglesFn(padfPrjParams[5]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case MILLER:
		setMC(pfnUnpackAnglesFn(padfPrjParams[5]),
			pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case VGRINT:
		setVDG(pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

	case HOM:
		if (padfPrjParams[12])
		{
			setHOM(pfnUnpackAnglesFn(padfPrjParams[5]),
				pfnUnpackAnglesFn(padfPrjParams[4]),
				pfnUnpackAnglesFn(padfPrjParams[3]),
				0.0, padfPrjParams[2],
				padfPrjParams[6], padfPrjParams[7]);
		}
		else
		{
			setHOM2PNO(pfnUnpackAnglesFn(padfPrjParams[5]),
				pfnUnpackAnglesFn(padfPrjParams[9]),
				pfnUnpackAnglesFn(padfPrjParams[8]),
				pfnUnpackAnglesFn(padfPrjParams[11]),
				pfnUnpackAnglesFn(padfPrjParams[10]),
				padfPrjParams[2],
				padfPrjParams[6], padfPrjParams[7]);
		}
		break;

	case ROBIN:
		setRobinson(pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

		// FIXME: SOM --- Space Oblique Mercator skipped

		// FIXME: ALASKA --- Alaska Conformal skipped

		// FIXME: GOODE --- Interrupted Goode skipped

	case MOLL:
		setMollweide(pfnUnpackAnglesFn(padfPrjParams[4]),
			padfPrjParams[6], padfPrjParams[7]);
		break;

		// FIXME: IMOLL --- Interrupted Mollweide skipped

		// FIXME: HAMMER --- Hammer skipped

	case WAGIV:
		setWagner(4, 0.0, padfPrjParams[6], padfPrjParams[7]);
		break;

	case WAGVII:
		setWagner(7, 0.0, padfPrjParams[6], padfPrjParams[7]);
		break;

		// FIXME: OBEQA --- Oblated Equal Area skipped

		// FIXME: ISINUS1 --- Integerized Sinusoidal Grid (the same as 99) skipped

		// FIXME: CEA --- Cylindrical Equal Area skipped (Grid corners set in meters for EASE grid)

		// FIXME: BCEA --- Cylindrical Equal Area skipped (Grid corners set in DMS degs for EASE grid)

		// FIXME: ISINUS --- Integrized Sinusoidal skipped

	default:
		gtl::debug("OSR_USGS", "Unsupported projection: %ld", iProjSys);
		setLocalCS(String().printf("GCTP projection number %ld", iProjSys));
		break;

	}

	/* -------------------------------------------------------------------- */
	/*      Try to translate the datum/spheroid.                            */
	/* -------------------------------------------------------------------- */

	if (!isLocal())
	{
		char    *pszName = NULL;
		double  dfSemiMajor, dfInvFlattening;

		if (iDatum < 0) // Use specified ellipsoid parameters
		{
			if (padfPrjParams[0] > 0.0)
			{
				if (padfPrjParams[1] > 1.0)
				{
					if (fabs(padfPrjParams[0] - padfPrjParams[1]) < 0.01)
						dfInvFlattening = 0.0;
					else
					{
						dfInvFlattening = padfPrjParams[0]
							/ (padfPrjParams[0] - padfPrjParams[1]);
					}
				}
				else if (padfPrjParams[1] > 0.0)
				{
					dfInvFlattening =
						1.0 / (1.0 - sqrt(1.0 - padfPrjParams[1]));
				}
				else
					dfInvFlattening = 0.0;

				setGeogCS("Unknown datum based upon the custom spheroid",
					"Not specified (based on custom spheroid)",
					"Custom spheroid", padfPrjParams[0], dfInvFlattening,
					NULL, 0, NULL, 0);
			}
			else if (padfPrjParams[1] > 0.0)  // Clarke 1866
			{
				if (getEllipsoidInfo(7008, &pszName, &dfSemiMajor,
					&dfInvFlattening) == true)
				{
					setGeogCS(String().printf(
						"Unknown datum based upon the %s ellipsoid",
						pszName),
						String().printf(
							"Not specified (based on %s spheroid)",
							pszName),
						pszName, dfSemiMajor, dfInvFlattening,
						NULL, 0.0, NULL, 0.0);
					setAuthority("SPHEROID", "EPSG", 7008);
				}
			}
			else                              // Sphere, rad 6370997 m
			{
				if (getEllipsoidInfo(7047, &pszName, &dfSemiMajor,
					&dfInvFlattening) == true)
				{
					setGeogCS(String().printf(
						"Unknown datum based upon the %s ellipsoid",
						pszName),
						String().printf(
							"Not specified (based on %s spheroid)",
							pszName),
						pszName, dfSemiMajor, dfInvFlattening,
						NULL, 0.0, NULL, 0.0);
					setAuthority("SPHEROID", "EPSG", 7047);
				}
			}

		}
		else if (iDatum < NUMBER_OF_ELLIPSOIDS && aoEllips[iDatum])
		{
			if (getEllipsoidInfo(aoEllips[iDatum], &pszName,
				&dfSemiMajor, &dfInvFlattening) == true)
			{
				setGeogCS(String().printf("Unknown datum based upon the %s ellipsoid",
					pszName),
					String().printf("Not specified (based on %s spheroid)",
						pszName),
					pszName, dfSemiMajor, dfInvFlattening,
					NULL, 0.0, NULL, 0.0);
				setAuthority("SPHEROID", "EPSG", aoEllips[iDatum]);
			}
			else
			{
				gtl::error(gtl::ERRORTYPE::GET_WARNING, gtl::ERRORCODE::GEC_APPDEFINED,
					"Failed to lookup datum code %d, likely due to missing GDAL gcs.csv\n"
					" file.  Falling back to use WGS84.",
					(int)iDatum);
				setWellKnownGeogCS("WGS84");
			}
		}
		else
		{
			gtl::error(gtl::ERRORTYPE::GET_WARNING, gtl::ERRORCODE::GEC_APPDEFINED,
				"Wrong datum code %d. Supported datums 0--%d only.\n"
				"Setting WGS84 as a fallback.",
				(int)iDatum, NUMBER_OF_ELLIPSOIDS);
			setWellKnownGeogCS("WGS84");
		}

		if (pszName)
			free(pszName);
	}

	/* -------------------------------------------------------------------- */
	/*      Grid units translation                                          */
	/* -------------------------------------------------------------------- */
	if (isLocal() || isProjected())
		setLinearUnits(SRS_UL_METER, 1.0);

	fixupOrdering();

	return true;
}

/************************************************************************/
/*                           exportToUSGS()                             */
/************************************************************************/

/**
* \brief Export coordinate system in USGS GCTP projection definition.
*
* This method is the equivalent of the C function OSRExportToUSGS().
*
* @param piProjSys Pointer to variable, where the projection system code will
* be returned.
*
* @param piZone Pointer to variable, where the zone for UTM and State Plane
* projection systems will be returned.
*
* @param ppadfPrjParams Pointer to which dynamically allocated array of
* 15 projection parameters will be assigned. See importFromUSGS() for
* the list of parameters. Caller responsible to free this array.
*
* @param piDatum Pointer to variable, where the datum code will
* be returned.
*
* @return true on success or an error code on failure.
*/

bool SpatialReference::exportToUSGS(long *piProjSys, long *piZone,
	double **ppadfPrjParams,
	long *piDatum) const

{
	const char  *pszProjection = getAttrValue("PROJECTION");

	/* -------------------------------------------------------------------- */
	/*      Fill all projection parameters with zero.                       */
	/* -------------------------------------------------------------------- */
	int         i;

	*ppadfPrjParams = (double *)malloc(15 * sizeof(double));
	for (i = 0; i < 15; i++)
		(*ppadfPrjParams)[i] = 0.0;

	*piZone = 0L;

	/* ==================================================================== */
	/*      Handle the projection definition.                               */
	/* ==================================================================== */
	if (isLocal())
		*piProjSys = GEO;

	else if (pszProjection == NULL)
	{
#ifdef DEBUG
		gtl::debug("OSR_USGS",
			"Empty projection definition, considered as Geographic");
#endif
		*piProjSys = GEO;
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ALBERS_CONIC_EQUAL_AREA))
	{
		*piProjSys = ALBERS;
		(*ppadfPrjParams)[2] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0));
		(*ppadfPrjParams)[3] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0));
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP))
	{
		*piProjSys = LAMCC;
		(*ppadfPrjParams)[2] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0));
		(*ppadfPrjParams)[3] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0));
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MERCATOR_1SP))
	{
		*piProjSys = MERCAT;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLAR_STEREOGRAPHIC))
	{
		*piProjSys = PS;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_POLYCONIC))
	{
		*piProjSys = POLYC;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIDISTANT_CONIC))
	{
		*piProjSys = EQUIDC;
		(*ppadfPrjParams)[2] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0));
		(*ppadfPrjParams)[3] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_2, 0.0));
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = 1.0;
	}

	else if (cslIEqualString(pszProjection, SRS_PT_TRANSVERSE_MERCATOR))
	{
		int bNorth;

		*piZone = getUTMZone(&bNorth);

		if (*piZone != 0)
		{
			*piProjSys = UTM;
			if (!bNorth)
				*piZone = -*piZone;
		}
		else
		{
			*piProjSys = TM;
			(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
			(*ppadfPrjParams)[4] = degreeToPackedDMS(
				getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
			(*ppadfPrjParams)[5] = degreeToPackedDMS(
				getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
			(*ppadfPrjParams)[6] =
				getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
			(*ppadfPrjParams)[7] =
				getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		}
	}

	else if (cslIEqualString(pszProjection, SRS_PT_STEREOGRAPHIC))
	{
		*piProjSys = STEREO;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA))
	{
		*piProjSys = LAMAZ;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_AZIMUTHAL_EQUIDISTANT))
	{
		*piProjSys = AZMEQD;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_GNOMONIC))
	{
		*piProjSys = GNOMON;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ORTHOGRAPHIC))
	{
		*piProjSys = ORTHO;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_SINUSOIDAL))
	{
		*piProjSys = SNSOID;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_EQUIRECTANGULAR))
	{
		*piProjSys = EQRECT;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_STANDARD_PARALLEL_1, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MILLER_CYLINDRICAL))
	{
		*piProjSys = MILLER;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_VANDERGRINTEN))
	{
		*piProjSys = VGRINT;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_HOTINE_OBLIQUE_MERCATOR))
	{
		*piProjSys = HOM;
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		(*ppadfPrjParams)[3] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_AZIMUTH, 0.0));
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[12] = 1.0;
	}

	else if (cslIEqualString(pszProjection,
		SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN))
	{
		*piProjSys = HOM;
		(*ppadfPrjParams)[2] = getNormProjParm(SRS_PP_SCALE_FACTOR, 1.0);
		(*ppadfPrjParams)[5] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
		(*ppadfPrjParams)[8] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_POINT_1, 0.0));
		(*ppadfPrjParams)[9] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_POINT_1, 0.0));
		(*ppadfPrjParams)[10] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_POINT_2, 0.0));
		(*ppadfPrjParams)[11] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LATITUDE_OF_POINT_2, 0.0));
		(*ppadfPrjParams)[12] = 0.0;
	}

	else if (cslIEqualString(pszProjection, SRS_PT_ROBINSON))
	{
		*piProjSys = ROBIN;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_MOLLWEIDE))
	{
		*piProjSys = MOLL;
		(*ppadfPrjParams)[4] = degreeToPackedDMS(
			getNormProjParm(SRS_PP_CENTRAL_MERIDIAN, 0.0));
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_WAGNER_IV))
	{
		*piProjSys = WAGIV;
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	else if (cslIEqualString(pszProjection, SRS_PT_WAGNER_VII))
	{
		*piProjSys = WAGVII;
		(*ppadfPrjParams)[6] = getNormProjParm(SRS_PP_FALSE_EASTING, 0.0);
		(*ppadfPrjParams)[7] = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);
	}

	// Projection unsupported by GCTP
	else
	{
		gtl::debug("OSR_USGS",
			"Projection \"%s\" unsupported by USGS GCTP. "
			"Geographic system will be used.", pszProjection);
		*piProjSys = GEO;
	}

	/* -------------------------------------------------------------------- */
	/*      Translate the datum.                                            */
	/* -------------------------------------------------------------------- */
	const char  *pszDatum = getAttrValue("DATUM");

	if (pszDatum)
	{
		if (cslIEqualString(pszDatum, SRS_DN_NAD27))
			*piDatum = CLARKE1866;

		else if (cslIEqualString(pszDatum, SRS_DN_NAD83))
			*piDatum = GRS1980;

		else if (cslIEqualString(pszDatum, SRS_DN_WGS84))
			*piDatum = WGS84;

		// If not found well known datum, translate ellipsoid
		else
		{
			double      dfSemiMajor = getSemiMajor();
			double      dfInvFlattening = getInvFlattening();

#ifdef DEBUG
			gtl::debug("OSR_USGS",
				"Datum \"%s\" unsupported by USGS GCTP. "
				"Try to translate ellipsoid definition.", pszDatum);
#endif

			for (i = 0; i < NUMBER_OF_ELLIPSOIDS; i++)
			{
				double  dfSM;
				double  dfIF;

				if (getEllipsoidInfo(aoEllips[i], NULL,
					&dfSM, &dfIF) == true
					&& doubleEqual(dfSemiMajor, dfSM)
					&& doubleEqual(dfInvFlattening, dfIF))
				{
					*piDatum = i;
					break;
				}
			}

			if (i == NUMBER_OF_ELLIPSOIDS)    // Didn't found matches; set
			{                                   // custom ellipsoid parameters
#ifdef DEBUG
				gtl::debug("OSR_USGS",
					"Ellipsoid \"%s\" unsupported by USGS GCTP. "
					"Custom ellipsoid definition will be used.",
					pszDatum);
#endif
				*piDatum = -1;
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
	}
	else
		*piDatum = -1;

	return true;
}


end_gdb_namespace
end_gtl_namespace