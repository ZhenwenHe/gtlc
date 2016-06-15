/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/

//derived from gdal

#pragma once

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include "assert.h"
#include "csv.h"
#include "srsapi.h"
#include "srsnode.h"
begin_gtl_namespace
begin_gdb_namespace


class SpatialReference
{
	double      dfFromGreenwich;
	double      dfToMeter;
	double      dfToDegrees;

	SRSNode *poRoot;

	int         nRefCount;
	int         bNormInfoSet;

	static bool validate(SRSNode *poRoot);
	static bool validateAuthority(SRSNode *poRoot);
	static bool validateAxis(SRSNode *poRoot);
	static bool validateUnit(SRSNode *poRoot);
	static bool validateVertDatum(SRSNode *poRoot);
	static bool validateProjection(SRSNode* poRoot);
	static int  isAliasFor(const char *, const char *);
	void        getNormInfo() const;

	bool      importFromURNPart(const char* pszAuthority,
		const char* pszCode,
		const char* pszURN);
public:
	SpatialReference(const SpatialReference&);
	SpatialReference(const char * = NULL);

	virtual    ~SpatialReference();

	static void destroySpatialReference(SpatialReference* poSRS);

	SpatialReference &operator=(const SpatialReference&);

	int         reference();
	int         dereference();
	int         getReferenceCount() const { return nRefCount; }
	void        release();

	SpatialReference *clone() const;
	SpatialReference *cloneGeogCS() const;

	void      dumpReadable();
	bool      exportToWkt(char **) const;
	bool      exportToPrettyWkt(char **, int = 0) const;
	bool      exportToProj4(char **) const;
	bool      exportToPCI(char **, char **, double **) const;
	bool      exportToUSGS(long *, long *, double **, long *) const;
	bool      exportToXML(char **, const char * = NULL) const;
	bool      exportToPanorama(long *, long *, long *, long *,
		double *) const;
	bool      exportToERM(char *pszProj, char *pszDatum, char *pszUnits);

	bool      importFromWkt(char **);
	bool      importFromProj4(const char *);
	bool      importFromEPSG(int);
	bool      importFromEPSGA(int);
	bool      importFromESRI(char **);
	bool      importFromPCI(const char *, const char * v1= NULL,
		double * v2= NULL);
#define USGS_ANGLE_DECIMALDEGREES 0
#define USGS_ANGLE_PACKEDDMS      1 /* 1 */
#define USGS_ANGLE_RADIANS        2
	bool      importFromUSGS(long iProjSys, long iZone,
		double *padfPrjParams, long iDatum,
		int nUSGSAngleFormat = USGS_ANGLE_PACKEDDMS);
	bool      importFromPanorama(long, long, long, double*);
	bool      importFromOzi(const char *, const char *, const char *);
	bool      importFromOzi(const char * const* papszLines);
	bool      importFromWMSAUTO(const char *pszAutoDef);
	bool      importFromXML(const char *);
	bool      importFromDict(const char *pszDict, const char *pszCode);
	bool      importFromURN(const char *);
	bool      importFromCRSURL(const char *);
	bool      importFromERM(const char *pszProj, const char *pszDatum,
		const char *pszUnits);

	bool      morphToESRI();
	bool      morphFromESRI();

	bool      validate();
	bool      stripCTParms(SRSNode * = NULL);
	bool      stripVertical();
	bool      fixupOrdering();
	bool      fixup();

	int         EPSGTreatsAsLatLong();
	int         EPSGTreatsAsNorthingEasting();
	const char *getAxis(const char *pszTargetKey, int iAxis,
		AxisOrientation *peOrientation) const;
	bool      setAxes(const char *pszTargetKey,
		const char *pszXAxisName,
		AxisOrientation eXAxisOrientation,
		const char *pszYAxisName,
		AxisOrientation eYAxisOrientation);

	// Machinary for accessing parse nodes
	SRSNode *getRoot() { return poRoot; }
	const SRSNode *getRoot() const { return poRoot; }
	void        setRoot(SRSNode *);

	SRSNode *getAttrNode(const char *);
	const SRSNode *getAttrNode(const char *v) const;
	const char  *getAttrValue(const char *v, int iAttr= 0) const;

	bool      setNode(const char *, const char *);
	bool      setNode(const char *, double);

	bool      setLinearUnitsAndUpdateParameters(const char *pszName,
		double dfInMeters);
	bool      setLinearUnits(const char *pszName, double dfInMeters);
	bool      setTargetLinearUnits(const char *pszTargetKey,
		const char *pszName, double dfInMeters);
	double     getLinearUnits(char ** = NULL) const;
	double     getTargetLinearUnits(const char *pszTargetKey,
		char ** ppszRetName = NULL) const;

	bool      setAngularUnits(const char *pszName, double dfInRadians);
	double    getAngularUnits(char ** = NULL) const;

	double     getPrimeMeridian(char ** = NULL) const;

	int         isGeographic() const;
	int         isProjected() const;
	int         isGeocentric() const;
	int         isLocal() const;
	int         isVertical() const;
	int         isCompound() const;
	int         isSameGeogCS(const SpatialReference *) const;
	int         isSameVertCS(const SpatialReference *) const;
	int         isSame(const SpatialReference *) const;

	void        clear();
	bool      setLocalCS(const char *);
	bool      setProjCS(const char *);
	bool      setProjection(const char *);
	bool      setGeocCS(const char * pszGeocName);
	bool      setGeogCS(const char * pszGeogName,
		const char * pszDatumName,
		const char * pszEllipsoidName,
		double dfSemiMajor, double dfInvFlattening,
		const char * pszPMName = NULL,
		double dfPMOffset = 0.0,
		const char * pszUnits = NULL,
		double dfConvertToRadians = 0.0);
	bool      setWellKnownGeogCS(const char *);
	bool      copyGeogCSFrom(const SpatialReference * poSrcSRS);
	bool      setVertCS(const char *pszVertCSName,
		const char *pszVertDatumName,
		int nVertDatumClass = 2005);
	bool      setCompoundCS(const char *pszName,
		const SpatialReference *poHorizSRS,
		const SpatialReference *poVertSRS);

	bool      setFromUserInput(const char *);

	bool      setTOWGS84(
		double dfDX, double dfDY, double dfDZ,
		double dfEX=0.0, double dfEY=0.0, double dfEZ=0.0,
		double dfPPM = 0.0);
	bool      getTOWGS84(double *padfCoef, int nCoeff = 7) const;

	double      getSemiMajor(bool * = NULL) const;
	double      getSemiMinor(bool * = NULL) const;
	double      getInvFlattening(bool * = NULL) const;

	bool      setAuthority(const char * pszTargetKey,
		const char * pszAuthority,
		int nCode);

	bool      autoIdentifyEPSG();
	int         getEPSGGeogCS();

	const char *getAuthorityCode(const char * pszTargetKey) const;
	const char *getAuthorityName(const char * pszTargetKey) const;

	const char *getExtension(const char *pszTargetKey,
		const char *pszName,
		const char *pszDefault = NULL) const;
	bool      setExtension(const char *pszTargetKey,
		const char *pszName,
		const char *pszValue);

	int         findProjParm(const char *pszParameter,
		const SRSNode *poPROJCS = NULL) const;
	bool      setProjParm(const char *, double);
	double     getProjParm(const char * pszName,double dfDefaultValue=0.0,bool *pnErr=NULL) const;

	bool      setNormProjParm(const char * pszName,	double dfValue);
	double     getNormProjParm(const char *, double dfDefaultValue= 0.0, bool* pnErr= NULL)const;
	static int  isAngularParameter(const char *);
	static int  isLongitudeParameter(const char *);
	static int  isLinearParameter(const char *);

	/** Albers Conic Equal Area */
	bool      setACEA(double dfStdP1, double dfStdP2,
		double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Azimuthal Equidistant */
	bool      setAE(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Bonne */
	bool      setBonne(double dfStdP1, double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Cylindrical Equal Area */
	bool      setCEA(double dfStdP1, double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Cassini-Soldner */
	bool      setCS(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Equidistant Conic */
	bool      setEC(double dfStdP1, double dfStdP2,
		double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Eckert I-VI */
	bool      setEckert(int nVariation, double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	bool      setEckertIV(double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	bool      setEckertVI(double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Equirectangular */
	bool      setEquirectangular(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);
	/** Equirectangular generalized form : */
	bool      setEquirectangular2(double dfCenterLat, double dfCenterLong,
		double dfPseudoStdParallel1,
		double dfFalseEasting, double dfFalseNorthing);

	/** Geostationary Satellite */
	bool      setGEOS(double dfCentralMeridian, double dfSatelliteHeight,
		double dfFalseEasting, double dfFalseNorthing);

	/** Goode Homolosine */
	bool      setGH(double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Interrupted Goode Homolosine */
	bool      setIGH();

	/** Gall Stereograpic */
	bool      setGS(double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Gauss Schreiber Transverse Mercator */
	bool      setGaussSchreiberTMercator(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Gnomonic */
	bool      setGnomonic(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Hotine Oblique Mercator */
	bool      setHOM(double dfCenterLat, double dfCenterLong,
		double dfAzimuth, double dfRectToSkew,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	bool      setHOM2PNO(double dfCenterLat,
		double dfLat1, double dfLong1,
		double dfLat2, double dfLong2,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	bool      setOM(double dfCenterLat, double dfCenterLong,
		double dfAzimuth, double dfRectToSkew,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Hotine Oblique Mercator Azimuth Center / Variant B */
	bool      setHOMAC(double dfCenterLat, double dfCenterLong,
		double dfAzimuth, double dfRectToSkew,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** International Map of the World Polyconic */
	bool      setIWMPolyconic(double dfLat1, double dfLat2,
		double dfCenterLong,
		double dfFalseEasting,
		double dfFalseNorthing);

	/** Krovak Oblique Conic Conformal */
	bool      setKrovak(double dfCenterLat, double dfCenterLong,
		double dfAzimuth, double dfPseudoStdParallelLat,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Lambert Azimuthal Equal-Area */
	bool      setLAEA(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Lambert Conformal Conic */
	bool      setLCC(double dfStdP1, double dfStdP2,
		double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Lambert Conformal Conic 1SP */
	bool      setLCC1SP(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Lambert Conformal Conic (Belgium) */
	bool      setLCCB(double dfStdP1, double dfStdP2,
		double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Miller Cylindrical */
	bool      setMC(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Mercator */
	bool      setMercator(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	bool      setMercator2SP(double dfStdP1,
		double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Mollweide */
	bool      setMollweide(double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** New Zealand Map Grid */
	bool      setNZMG(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Oblique Stereographic */
	bool      setOS(double dfOriginLat, double dfCMeridian,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Orthographic */
	bool      setOrthographic(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Polyconic */
	bool      setPolyconic(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Polar Stereographic */
	bool      setPS(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Robinson */
	bool      setRobinson(double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Sinusoidal */
	bool      setSinusoidal(double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Stereographic */
	bool      setStereographic(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Swiss Oblique Cylindrical */
	bool     setSOC(double dfLatitudeOfOrigin, double dfCentralMeridian,
		double dfFalseEasting, double dfFalseNorthing);

	/** Transverse Mercator */
	bool      setTM(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Transverse Mercator variants. */
	bool     setTMVariant(const char *pszVariantName,
		double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Tunesia Mining Grid  */
	bool      setTMG(double dfCenterLat, double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Transverse Mercator (South Oriented) */
	bool      setTMSO(double dfCenterLat, double dfCenterLong,
		double dfScale,
		double dfFalseEasting, double dfFalseNorthing);

	/** Two Point Equidistant */
	bool     setTPED(double dfLat1, double dfLong1,
		double dfLat2, double dfLong2,
		double dfFalseEasting, double dfFalseNorthing);

	/** VanDerGrinten */
	bool     setVDG(double dfCenterLong,
		double dfFalseEasting, double dfFalseNorthing);

	/** Universal Transverse Mercator */
	bool    setUTM(int nZone, int bNorth = 1);
	int         getUTMZone(int *pbNorth = NULL) const;

	/** Wagner I -- VII */
	bool     setWagner(int nVariation, double dfCenterLat,
		double dfFalseEasting, double dfFalseNorthing);

	/** State Plane */
	bool     setStatePlane(int nZone, int bNAD83 = 1,
		const char *pszOverrideUnitName = NULL,
		double dfOverrideUnit = 0.0);

	bool      importFromESRIStatePlaneWKT(
		int nCode, const char* pszDatumName, const char* pszUnitsName,
		int nPCSCode, const char* pszCSName = 0);
	bool      importFromESRIWisconsinWKT(
		const char* pszPrjName, double dfCentralMeridian, double dfLatOfOrigin,
		const char* pszUnitsName, const char* pszCSName = 0);
};


class CORE_API CoordinateTransformer {
public:
	virtual ~CoordinateTransformer() {}

	static void destroy(CoordinateTransformer* poCT);
	static CoordinateTransformer* create(SpatialReference * sourceCS, SpatialReference * targetCS);

	/** Fetch internal source coordinate system. */
	virtual SpatialReference *getSourceCS() = 0;

	/** Fetch internal target coordinate system. */
	virtual SpatialReference *getTargetCS() = 0;

	/**
	* Transform points from source to destination space.
	*
	* This method is the same as the C function OCTTransform().
	*
	* The method TransformEx() allows extended success information to
	* be captured indicating which points failed to transform.
	*
	* @param nCount number of points to transform.
	* @param x array of nCount X vertices, modified in place.
	* @param y array of nCount Y vertices, modified in place.
	* @param z array of nCount Z vertices, modified in place.
	* @return TRUE on success, or FALSE if some or all points fail to
	* transform.
	*/
	virtual int transform(int nCount,
		double *x, double *y, double *z ) = 0;

	/**
	* Transform points from source to destination space.
	*
	* This method is the same as the C function OCTTransformEx().
	*
	* @param nCount number of points to transform.
	* @param x array of nCount X vertices, modified in place.
	* @param y array of nCount Y vertices, modified in place.
	* @param z array of nCount Z vertices, modified in place.
	* @param pabSuccess array of per-point flags set to TRUE if that point
	* transforms, or FALSE if it does not.
	*
	* @return TRUE if some or all points transform successfully, or FALSE if
	* if none transform.
	*/

	virtual int transform(int nCount,
		 double *x,  double *y,  double *z,
		int *pabSuccess ) = 0;

};

end_gdb_namespace
end_gtl_namespace

