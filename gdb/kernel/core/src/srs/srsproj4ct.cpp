#include "spatialreference.h"
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace
/*
/************************************************************************/
/*                              Proj4CT                                 */
/*基于Proj4实现的坐标转换器，采用的是Proj4的版本是4.92及其以上版本          */
/*不支持Proj4的静态版本,这样需要在getInstallHome的文件夹中存在proj_4_9.dll和*/
/*proj_4_9_d.dll或proj_4_9.so和proj_4_9_d.so,*/
/************************************************************************/
#if(USING_PROJ4_STATIC==0)
#define RAD_TO_DEG	57.29577951308232
#define DEG_TO_RAD	.0174532925199432958
typedef struct { double u, v; } projUV;
typedef void *projPJ;
#define projXY projUV
#define projLP projUV
typedef void *projCtx;
#else
#include "proj_api.h"
#endif


static void *hPROJMutex = NULL;
int acquireMutex(void *, double ) {
	return 0;
}
/*自定义的函数指针*/
static projPJ(*pfn_pj_init_plus)(const char *) = NULL;
static projPJ(*pfn_pj_init)(int, char**) = NULL;
static void(*pfn_pj_free)(projPJ) = NULL;

static int(*pfn_pj_transform)(projPJ, projPJ, long, int,
	double *, double *, double *) = NULL;
static int         *(*pfn_pj_get_errno_ref)(void) = NULL;
static char        *(*pfn_pj_strerrno)(int) = NULL;
static char        *(*pfn_pj_get_def)(projPJ, int) = NULL;
static void(*pfn_pj_dalloc)(void *) = NULL;

static projPJ(*pfn_pj_init_plus_ctx)(projCtx, const char *) = NULL;
static int(*pfn_pj_ctx_get_errno)(projCtx) = NULL;
static projCtx(*pfn_pj_ctx_alloc)(void) = NULL;
static void(*pfn_pj_ctx_free)(projCtx) = NULL;

static std::string getProjLibraryName() {
	std::string sz; 
#if(USING_OS_TYPE==0)        //windows

#if defined(_DEBUG) || defined(DEBUG) 
	sz = gtl::getInstallHome() + std::string("\\proj_4_9_d.dll");
#else
	sz = gtl::getInstallHome() + std::string("\\proj_4_9.dll");
#endif

#elif(USING_OS_TYPE==1) //linux

#if defined(_DEBUG) || defined(DEBUG) 
	sz = gtl::getInstallHome() + std::string("\\libproj_4_9_d.so");
#else
	sz = gtl::getInstallHome() + std::string("\\libproj_4_9.so");
#endif

#else                   //macX OS

#if defined(_DEBUG) || defined(DEBUG) 
	sz = gtl::getInstallHome() + std::string("\\libproj_4_9_d.dylib");
#else
	sz = gtl::getInstallHome() + std::string("\\libproj_4_9.dylib");
#endif

#endif         

	return sz;
}
static int loadProjLibrary() { 
	static int  bTriedToLoad = FALSE;
	const char *pszLibName;

	if (bTriedToLoad)
		return(pfn_pj_transform != NULL);

	bTriedToLoad = TRUE;

	pszLibName = getProjLibraryName().c_str();
#if(USING_PROJ4_STATIC == 1)
	pfn_pj_init = pj_init;
	pfn_pj_init_plus = pj_init_plus;
	pfn_pj_free = pj_free;
	pfn_pj_transform = pj_transform;
	pfn_pj_get_errno_ref = (int *(*)(void)) pj_get_errno_ref;
	pfn_pj_strerrno = pj_strerrno;
	pfn_pj_dalloc = pj_dalloc;
#if PJ_VERSION >= 446
	pfn_pj_get_def = pj_get_def;
#endif
#if PJ_VERSION >= 480
	pfn_pj_ctx_alloc = pj_ctx_alloc;
	pfn_pj_ctx_free = pj_ctx_free;
	pfn_pj_init_plus_ctx = pj_init_plus_ctx;
	pfn_pj_ctx_get_errno = pj_ctx_get_errno;
#endif

#else 

	pfn_pj_init = (projPJ(*)(int, char**)) gtl::getFunctionPointer(pszLibName,
		"pj_init"); 

	if (pfn_pj_init == NULL)
		return(FALSE);

	pfn_pj_init_plus = (projPJ(*)(const char *))
		gtl::getFunctionPointer(pszLibName, "pj_init_plus");
	pfn_pj_free = (void(*)(projPJ))
		gtl::getFunctionPointer(pszLibName, "pj_free");
	pfn_pj_transform = (int(*)(projPJ, projPJ, long, int, double*,
		double*, double*))
		gtl::getFunctionPointer(pszLibName, "pj_transform");
	pfn_pj_get_errno_ref = (int *(*)(void))
		gtl::getFunctionPointer(pszLibName, "pj_get_errno_ref");
	pfn_pj_strerrno = (char *(*)(int))
		gtl::getFunctionPointer(pszLibName, "pj_strerrno");
	 
	pfn_pj_get_def = (char *(*)(projPJ, int))
		gtl::getFunctionPointer(pszLibName, "pj_get_def");
	pfn_pj_dalloc = (void(*)(void*))
		gtl::getFunctionPointer(pszLibName, "pj_dalloc");

	/* PROJ 4.8.0 symbols */
	pfn_pj_ctx_alloc = (projCtx(*)(void))
		gtl::getFunctionPointer(pszLibName, "pj_ctx_alloc");
	pfn_pj_ctx_free = (void(*)(projCtx))
		gtl::getFunctionPointer(pszLibName, "pj_ctx_free");
	pfn_pj_init_plus_ctx = (projPJ(*)(projCtx, const char *))
		gtl::getFunctionPointer(pszLibName, "pj_init_plus_ctx");
	pfn_pj_ctx_get_errno = (int(*)(projCtx))
		gtl::getFunctionPointer(pszLibName, "pj_ctx_get_errno");
#endif
	if (pfn_pj_transform == NULL)
	{ 
		return FALSE;
	}

	return(TRUE);
}


/************************************************************************/
/*                         proj4Normalize()                          */
/*                                                                      */
/*      This function is really just here since we already have all     */
/*      the code to load libproj.so.  It is intended to "normalize"     */
/*      a proj.4 definition, expanding +init= definitions and so        */
/*      forth as possible.                                              */
/************************************************************************/

char *proj4Normalize(const char *pszProj4Src)

{
	char        *pszNewProj4Def, *pszCopy;
	projPJ      psPJSource = NULL;

	//CPLMutexHolderD(&hPROJMutex);

	if (!loadProjLibrary() || pfn_pj_dalloc == NULL || pfn_pj_get_def == NULL)
		return cslDuplicateString(pszProj4Src);

	psPJSource = pfn_pj_init_plus(pszProj4Src);

	if (psPJSource == NULL)
		return cslDuplicateString(pszProj4Src);

	pszNewProj4Def = pfn_pj_get_def(psPJSource, 0);

	pfn_pj_free(psPJSource);

	if (pszNewProj4Def == NULL)
		return cslDuplicateString(pszProj4Src);

	pszCopy = cslDuplicateString(pszNewProj4Def);
	pfn_pj_dalloc(pszNewProj4Def);

	return pszCopy;
}


class Proj4CT : public CoordinateTransformer
{
	SpatialReference *poSRSSource;
	void        *psPJSource;
	int         bSourceLatLong;
	double      dfSourceToRadians;
	double      dfSourceFromRadians;
	int         bSourceWrap;
	double      dfSourceWrapLong;


	SpatialReference *poSRSTarget;
	void        *psPJTarget;
	int         bTargetLatLong;
	double      dfTargetToRadians;
	double      dfTargetFromRadians;
	int         bTargetWrap;
	double      dfTargetWrapLong;

	int         nErrorCount;

	int         bCheckWithInvertProj;
	double      dfThreshold;

	projCtx     pjctx;

	

	int         nMaxCount;
	double     *padfOriX;
	double     *padfOriY;
	double     *padfOriZ;
	double     *padfTargetX;
	double     *padfTargetY;
	double     *padfTargetZ;
protected:
	int         initializeNoLock(SpatialReference *poSourceIn,
		SpatialReference *poTargetIn) {
		if (poSourceIn == NULL || poTargetIn == NULL)
			return FALSE;

		poSRSSource = poSourceIn->clone();
		poSRSTarget = poTargetIn->clone();

		bSourceLatLong = poSRSSource->isGeographic();
		bTargetLatLong = poSRSTarget->isGeographic();

		/* -------------------------------------------------------------------- */
		/*      Setup source and target translations to radians for lat/long    */
		/*      systems.                                                        */
		/* -------------------------------------------------------------------- */
		dfSourceToRadians = DEG_TO_RAD;
		dfSourceFromRadians = RAD_TO_DEG;
		bSourceWrap = FALSE;
		dfSourceWrapLong = 0.0;

		if (bSourceLatLong)
		{
			SRSNode *poUNITS = poSRSSource->getAttrNode("GEOGCS|UNIT");
			if (poUNITS && poUNITS->getChildCount() >= 2)
			{
				dfSourceToRadians = atof(poUNITS->getChild(1)->getValue());
				if (dfSourceToRadians == 0.0)
					dfSourceToRadians = DEG_TO_RAD;
				else
					dfSourceFromRadians = 1 / dfSourceToRadians;
			}
		}

		dfTargetToRadians = DEG_TO_RAD;
		dfTargetFromRadians = RAD_TO_DEG;
		bTargetWrap = FALSE;
		dfTargetWrapLong = 0.0;

		if (bTargetLatLong)
		{
			SRSNode *poUNITS = poSRSTarget->getAttrNode("GEOGCS|UNIT");
			if (poUNITS && poUNITS->getChildCount() >= 2)
			{
				dfTargetToRadians = atof(poUNITS->getChild(1)->getValue());
				if (dfTargetToRadians == 0.0)
					dfTargetToRadians = DEG_TO_RAD;
				else
					dfTargetFromRadians = 1 / dfTargetToRadians;
			}
		}

		/* -------------------------------------------------------------------- */
		/*      Preliminary logic to setup wrapping.                            */
		/* -------------------------------------------------------------------- */
		const char *pszCENTER_LONG;

		if (Config::getSRConfigOptionValue("CENTER_LONG", NULL).c_str() != NULL)
		{
			bSourceWrap = bTargetWrap = TRUE;
			dfSourceWrapLong = dfTargetWrapLong =
				atof(Config::getSRConfigOptionValue("CENTER_LONG", "").c_str());
			gtl::debug("PROJ4CT", "Wrap at %g.", dfSourceWrapLong);
		}

		pszCENTER_LONG = poSRSSource->getExtension("GEOGCS", "CENTER_LONG");
		if (pszCENTER_LONG != NULL)
		{
			dfSourceWrapLong = atof(pszCENTER_LONG);
			bSourceWrap = TRUE;
			gtl::debug("PROJ4CT", "Wrap source at %g.", dfSourceWrapLong);
		}

		pszCENTER_LONG = poSRSTarget->getExtension("GEOGCS", "CENTER_LONG");
		if (pszCENTER_LONG != NULL)
		{
			dfTargetWrapLong = atof(pszCENTER_LONG);
			bTargetWrap = TRUE;
			gtl::debug("PROJ4CT", "Wrap target at %g.", dfTargetWrapLong);
		}

		bCheckWithInvertProj = cslTestBoolean(Config::getSRConfigOptionValue("CHECK_WITH_INVERT_PROJ", "NO").c_str());

		/* The threshold is rather experimental... Works well with the cases of ticket #2305 */
		if (bSourceLatLong)
			dfThreshold = atof(Config::getSRConfigOptionValue("THRESHOLD", ".1").c_str());
		else
			/* 1 works well for most projections, except for +proj=aeqd that requires */
			/* a tolerance of 10000 */
			dfThreshold = atof(Config::getSRConfigOptionValue("THRESHOLD", "10000").c_str());

		/* -------------------------------------------------------------------- */
		/*      Establish PROJ.4 handle for source if projection.               */
		/* -------------------------------------------------------------------- */
		// OGRThreadSafety: The following variable is not a thread safety issue 
		// since the only issue is incrementing while accessing which at worse 
		// means debug output could be one "increment" late. 
		static int   nDebugReportCount = 0;

		char        *pszProj4Defn = NULL;

		if (poSRSSource->exportToProj4(&pszProj4Defn) != TRUE)
		{
			free(pszProj4Defn);
			return FALSE;
		}

		if (strlen(pszProj4Defn) == 0)
		{
			free(pszProj4Defn);
			gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
				"No PROJ.4 translation for source SRS, coordinate\n"
				"transformation initialization has failed.");
			return FALSE;
		}

		if (pjctx)
			psPJSource = pfn_pj_init_plus_ctx(pjctx, pszProj4Defn);
		else
			psPJSource = pfn_pj_init_plus(pszProj4Defn);

		if (psPJSource == NULL)
		{
			if (pjctx != NULL)
			{
				int pj_errno = pfn_pj_ctx_get_errno(pjctx);

				/* pfn_pj_strerrno not yet thread-safe in PROJ 4.8.0 */
				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_NOTSUPPORTED,
					"Failed to initialize PROJ.4 with `%s'.\n%s",
					pszProj4Defn, pfn_pj_strerrno(pj_errno));
			}
			else if (pfn_pj_get_errno_ref != NULL
				&& pfn_pj_strerrno != NULL)
			{
				int *p_pj_errno = pfn_pj_get_errno_ref();

				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_NOTSUPPORTED,
					"Failed to initialize PROJ.4 with `%s'.\n%s",
					pszProj4Defn, pfn_pj_strerrno(*p_pj_errno));
			}
			else
			{
				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_NOTSUPPORTED,
					"Failed to initialize PROJ.4 with `%s'.\n",
					pszProj4Defn);
			}
		}

		if (nDebugReportCount < 10)
			gtl::debug("PROJ4CT", "Source: %s", pszProj4Defn);

		free(pszProj4Defn);

		if (psPJSource == NULL)
			return FALSE;

		/* -------------------------------------------------------------------- */
		/*      Establish PROJ.4 handle for target if projection.               */
		/* -------------------------------------------------------------------- */
		pszProj4Defn = NULL;

		if (poSRSTarget->exportToProj4(&pszProj4Defn) != TRUE)
		{
			free(pszProj4Defn);
			return FALSE;
		}

		if (strlen(pszProj4Defn) == 0)
		{
			free(pszProj4Defn);
			gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
				"No PROJ.4 translation for destination SRS, coordinate\n"
				"transformation initialization has failed.");
			return FALSE;
		}

		if (pjctx)
			psPJTarget = pfn_pj_init_plus_ctx(pjctx, pszProj4Defn);
		else
			psPJTarget = pfn_pj_init_plus(pszProj4Defn);

		if (psPJTarget == NULL)
			gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_NOTSUPPORTED,
				"Failed to initialize PROJ.4 with `%s'.",
				pszProj4Defn);

		if (nDebugReportCount < 10)
		{
			gtl::debug("PROJ4CT", "Target: %s", pszProj4Defn);
			nDebugReportCount++;
		}

		free(pszProj4Defn);

		if (psPJTarget == NULL)
			return FALSE;

		return TRUE;
	}
public:
	Proj4CT() {
		poSRSSource = NULL;
		poSRSTarget = NULL;
		psPJSource = NULL;
		psPJTarget = NULL;

		nErrorCount = 0;

		bCheckWithInvertProj = FALSE;
		dfThreshold = 0;

		nMaxCount = 0;
		padfOriX = NULL;
		padfOriY = NULL;
		padfOriZ = NULL;
		padfTargetX = NULL;
		padfTargetY = NULL;
		padfTargetZ = NULL;

		if (pfn_pj_ctx_alloc != NULL)
			pjctx = pfn_pj_ctx_alloc();
		else
			pjctx = NULL;
	}
	virtual ~Proj4CT() {
		if (poSRSSource != NULL)
		{
			if (poSRSSource->dereference() <= 0)
				delete poSRSSource;
		}

		if (poSRSTarget != NULL)
		{
			if (poSRSTarget->dereference() <= 0)
				delete poSRSTarget;
		}

		if (pjctx != NULL)
		{
			pfn_pj_ctx_free(pjctx);

			if (psPJSource != NULL)
				pfn_pj_free(psPJSource);

			if (psPJTarget != NULL)
				pfn_pj_free(psPJTarget);
		}
		else
		{ 
			if (psPJSource != NULL)
				pfn_pj_free(psPJSource);

			if (psPJTarget != NULL)
				pfn_pj_free(psPJTarget);
		}

		free(padfOriX);
		free(padfOriY);
		free(padfOriZ);
		free(padfTargetX);
		free(padfTargetY);
		free(padfTargetZ);
	}

	int   initialize(SpatialReference *poSource,
		SpatialReference *poTarget) {
		if (pjctx != NULL)
		{
			return initializeNoLock(poSource, poTarget);
		}
		 
		return initializeNoLock(poSource, poTarget);
	}

	virtual SpatialReference *getSourceCS() {
		return poSRSSource;
	}
	virtual SpatialReference *getTargetCS() {
		return poSRSTarget;
	}
	virtual int transform(int nCount,
		double *x, double *y, double *z) {

		int *pabSuccess = (int *)malloc(sizeof(int) * nCount);
		int bOverallSuccess, i;

		bOverallSuccess = transform(nCount, x, y, z, pabSuccess);

		for (i = 0; i < nCount; i++)
		{
			if (!pabSuccess[i])
			{
				bOverallSuccess = FALSE;
				break;
			}
		}

		free(pabSuccess);

		return bOverallSuccess;
	}
	virtual int transform(int nCount,
		double *x, double *y, double *z,
		int *pabSuccess) {
		int   err, i;

		/* -------------------------------------------------------------------- */
		/*      Potentially transform to radians.                               */
		/* -------------------------------------------------------------------- */
		if (bSourceLatLong)
		{
			if (bSourceWrap)
			{
				for (i = 0; i < nCount; i++)
				{
					if (x[i] != HUGE_VAL && y[i] != HUGE_VAL)
					{
						if (x[i] < dfSourceWrapLong - 180.0)
							x[i] += 360.0;
						else if (x[i] > dfSourceWrapLong + 180)
							x[i] -= 360.0;
					}
				}
			}

			for (i = 0; i < nCount; i++)
			{
				if (x[i] != HUGE_VAL)
				{
					x[i] *= dfSourceToRadians;
					y[i] *= dfSourceToRadians;
				}
			}
		}

		/* -------------------------------------------------------------------- */
		/*      Do the transformation using PROJ.4.                             */
		/* -------------------------------------------------------------------- */
		//if (pjctx == NULL)
		//{
		//	/* The mutex has already been created */
		//	assert(hPROJMutex != NULL);
		//	acquireMutex(hPROJMutex, 1000.0);
		//}

		if (bCheckWithInvertProj)
		{
			/* For some projections, we cannot detect if we are trying to reproject */
			/* coordinates outside the validity area of the projection. So let's do */
			/* the reverse reprojection and compare with the source coordinates */
			if (nCount > nMaxCount)
			{
				nMaxCount = nCount;
				padfOriX = (double*)realloc(padfOriX, sizeof(double)*nCount);
				padfOriY = (double*)realloc(padfOriY, sizeof(double)*nCount);
				padfOriZ = (double*)realloc(padfOriZ, sizeof(double)*nCount);
				padfTargetX = (double*)realloc(padfTargetX, sizeof(double)*nCount);
				padfTargetY = (double*)realloc(padfTargetY, sizeof(double)*nCount);
				padfTargetZ = (double*)realloc(padfTargetZ, sizeof(double)*nCount);
			}
			memcpy(padfOriX, x, sizeof(double)*nCount);
			memcpy(padfOriY, y, sizeof(double)*nCount);
			if (z)
			{
				memcpy(padfOriZ, z, sizeof(double)*nCount);
			}
			err = pfn_pj_transform(psPJSource, psPJTarget, nCount, 1, x, y, z);
			if (err == 0)
			{
				memcpy(padfTargetX, x, sizeof(double)*nCount);
				memcpy(padfTargetY, y, sizeof(double)*nCount);
				if (z)
				{
					memcpy(padfTargetZ, z, sizeof(double)*nCount);
				}

				err = pfn_pj_transform(psPJTarget, psPJSource, nCount, 1,
					padfTargetX, padfTargetY, (z) ? padfTargetZ : NULL);
				if (err == 0)
				{
					for (i = 0; i < nCount; i++)
					{
						if (x[i] != HUGE_VAL && y[i] != HUGE_VAL &&
							(fabs(padfTargetX[i] - padfOriX[i]) > dfThreshold ||
								fabs(padfTargetY[i] - padfOriY[i]) > dfThreshold))
						{
							x[i] = HUGE_VAL;
							y[i] = HUGE_VAL;
						}
					}
				}
			}
		}
		else
		{
			err = pfn_pj_transform(psPJSource, psPJTarget, nCount, 1, x, y, z);
		}

		/* -------------------------------------------------------------------- */
		/*      Try to report an error through CPL.  Get proj.4 error string    */
		/*      if possible.  Try to avoid reporting thousands of error         */
		/*      ... supress further error reporting on this OGRProj4CT if we    */
		/*      have already reported 20 errors.                                */
		/* -------------------------------------------------------------------- */
		if (err != 0)
		{
			if (pabSuccess)
				memset(pabSuccess, 0, sizeof(int) * nCount);

			if (++nErrorCount < 20)
			{
				if (pjctx != NULL)
					/* pfn_pj_strerrno not yet thread-safe in PROJ 4.8.0 */
					acquireMutex(hPROJMutex, 1000.0);

				const char *pszError = NULL;
				if (pfn_pj_strerrno != NULL)
					pszError = pfn_pj_strerrno(err);

				if (pszError == NULL)
					gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
						"Reprojection failed, err = %d",
						err);
				else
					gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED, "%s", pszError);

				//if (pjctx != NULL)
				//	/* pfn_pj_strerrno not yet thread-safe in PROJ 4.8.0 */
				//	releaseMutex(hPROJMutex);
			}
			else if (nErrorCount == 20)
			{
				gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_APPDEFINED,
					"Reprojection failed, err = %d, further errors will be supressed on the transform object.",
					err);
			}

			/*if (pjctx == NULL)
				releaseMutex(hPROJMutex);*/
			return FALSE;
		}

		/*if (pjctx == NULL)
			releaseMutex(hPROJMutex);*/

		/* -------------------------------------------------------------------- */
		/*      Potentially transform back to degrees.                          */
		/* -------------------------------------------------------------------- */
		if (bTargetLatLong)
		{
			for (i = 0; i < nCount; i++)
			{
				if (x[i] != HUGE_VAL && y[i] != HUGE_VAL)
				{
					x[i] *= dfTargetFromRadians;
					y[i] *= dfTargetFromRadians;
				}
			}

			if (bTargetWrap)
			{
				for (i = 0; i < nCount; i++)
				{
					if (x[i] != HUGE_VAL && y[i] != HUGE_VAL)
					{
						if (x[i] < dfTargetWrapLong - 180.0)
							x[i] += 360.0;
						else if (x[i] > dfTargetWrapLong + 180)
							x[i] -= 360.0;
					}
				}
			}
		}

		/* -------------------------------------------------------------------- */
		/*      Establish error information if pabSuccess provided.             */
		/* -------------------------------------------------------------------- */
		if (pabSuccess)
		{
			for (i = 0; i < nCount; i++)
			{
				if (x[i] == HUGE_VAL || y[i] == HUGE_VAL)
					pabSuccess[i] = FALSE;
				else
					pabSuccess[i] = TRUE;
			}
		}

		return TRUE;
	}

};


CoordinateTransformer* CoordinateTransformer::create(SpatialReference *poSource, SpatialReference * poTarget)
{
	
	Proj4CT  *poCT;

	if (pfn_pj_init == NULL && !loadProjLibrary())
	{
		gtl::error(gtl::ERRORTYPE::GET_FAILURE, gtl::ERRORCODE::GEC_NOTSUPPORTED,
			"Unable to load PROJ.4 library (%s), creation of\n"
			"OGRCoordinateTransformation failed.",
			getProjLibraryName());
		return NULL;
	}

	poCT = new Proj4CT();

	if (!(poCT->initialize(poSource, poTarget)))
	{
		delete poCT;
		return NULL;
	}
	else
	{
		return poCT;
	}

	return  poCT;
}

end_gdb_namespace
end_gtl_namespace