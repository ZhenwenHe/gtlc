#include "spatialreference.h" 
begin_gtl_namespace
begin_gdb_namespace
// The current opinion is that WKT longitudes like central meridian
// should be relative to greenwich, not the prime meridian in use. 
// Define the following if they should be relative to the prime meridian
// of then geogcs.
#undef WKT_LONGITUDE_RELATIVE_TO_PM

/************************************************************************/
/*                           printDouble()                           */
/************************************************************************/

void printDouble(char * pszStrBuf, double dfValue)

{
	sprintf(pszStrBuf, "%.16g", dfValue);

	int nLen = strlen(pszStrBuf);

	// The following hack is intended to truncate some "precision" in cases
	// that appear to be roundoff error. 
	if (nLen > 15
		&& (strcmp(pszStrBuf + nLen - 6, "999999") == 0
			|| strcmp(pszStrBuf + nLen - 6, "000001") == 0))
	{
		sprintf(pszStrBuf, "%.15g", dfValue);
	}

	// force to user periods regardless of locale.
	if (strchr(pszStrBuf, ',') != NULL)
	{
		char *pszDelim = strchr(pszStrBuf, ',');
		*pszDelim = '.';
	}
}

/************************************************************************/
/*                        SpatialReference()                         */
/************************************************************************/

/**
* \brief Constructor.
*
* This constructor takes an optional string argument which if passed
* should be a WKT representation of an SRS.  Passing this is equivalent
* to not passing it, and then calling importFromWkt() with the WKT string.
*
* Note that newly created objects are given a reference count of one.
*
* The C function OSRNewSpatialReference() does the same thing as this
* constructor.
*
* @param pszWKT well known text definition to which the object should
* be initialized, or NULL (the default).
*/

SpatialReference::SpatialReference(const char * pszWKT)

{
	bNormInfoSet = FALSE;
	nRefCount = 1;
	poRoot = NULL;

	if (pszWKT != NULL)
		importFromWkt((char **)&pszWKT);
}
/************************************************************************/
/*                        SpatialReference()                         */
/*                                                                      */
/*      Simple copy constructor.  See also clone().                     */
/************************************************************************/

SpatialReference::SpatialReference(const SpatialReference &oOther)

{
	bNormInfoSet = FALSE;
	nRefCount = 1;
	poRoot = NULL;

	if (oOther.poRoot != NULL)
		poRoot = oOther.poRoot->clone();
}

/************************************************************************/
/*                        ~SpatialReference()                        */
/************************************************************************/

/**
* \brief SpatialReference destructor.
*
* The C function OSRDestroySpatialReference() does the same thing as this
* method. Preferred C++ method : SpatialReference::destroySpatialReference()
*
* @deprecated
*/

SpatialReference::~SpatialReference()

{
	if (poRoot != NULL)
		delete poRoot;
}

/************************************************************************/
/*                      destroySpatialReference()                       */
/************************************************************************/

/**
* \brief SpatialReference destructor.
*
* This static method will destroy a SpatialReference.  It is
* equivalent to calling delete on the object, but it ensures that the
* deallocation is properly executed within the OGR libraries heap on
* platforms where this can matter (win32).
*
* This function is the same as OSRDestroySpatialReference()
*
* @param poSRS the object to delete
*
* @since GDAL 1.7.0
*/

void SpatialReference::destroySpatialReference(SpatialReference* poSRS)
{
	delete poSRS;
}

/************************************************************************/
/*                               clear()                                */
/************************************************************************/

/**
* \brief Wipe current definition.
*
* Returns SpatialReference to a state with no definition, as it
* exists when first created.  It does not affect reference counts.
*/

void SpatialReference::clear()

{
	if (poRoot)
		delete poRoot;

	poRoot = NULL;

	bNormInfoSet = FALSE;
	dfFromGreenwich = 1.0;
	dfToMeter = 1.0;
	dfToDegrees = 1.0;
}

/************************************************************************/
/*                             operator=()                              */
/************************************************************************/

SpatialReference &
SpatialReference::operator=(const SpatialReference &oSource)

{
	clear();

	if (oSource.poRoot != NULL)
		poRoot = oSource.poRoot->clone();

	return *this;
}

/************************************************************************/
/*                             reference()                              */
/************************************************************************/

/**
* \brief Increments the reference count by one.
*
* The reference count is used keep track of the number of OGRGeometry objects
* referencing this SRS.
*
* The method does the same thing as the C function OSRReference().
*
* @return the updated reference count.
*/

int SpatialReference::reference()

{
	return nRefCount++;
}

/************************************************************************/
/*                            dereference()                             */
/************************************************************************/

/**
* \brief Decrements the reference count by one.
*
* The method does the same thing as the C function OSRDereference().
*
* @return the updated reference count.
*/

int SpatialReference::dereference()
{
	if (nRefCount <= 0)
		printf_s("SR: dereference() called on an object with refcount %d, likely already destroyed!",nRefCount);
	nRefCount--;
	return nRefCount;
}
 
/************************************************************************/
/*                         getReferenceCount()                          */
/************************************************************************/

/**
* \fn int SpatialReference::getReferenceCount() const;
*
* \brief Fetch current reference count.
*
* @return the current reference count.
*/

/************************************************************************/
/*                              release()                               */
/************************************************************************/

/**
* \brief Decrements the reference count by one, and destroy if zero.
*
* The method does the same thing as the C function OSRRelease().
*/

void SpatialReference::release()

{
	assert(NULL != this);

	if (dereference() <= 0)
		delete this;
}
 
/************************************************************************/
/*                              setRoot()                               */
/************************************************************************/

/**
* \brief Set the root SRS node.
*
* If the object has an existing tree of OGR_SRSNodes, they are destroyed
* as part of assigning the new root.  Ownership of the passed SRSNode is
* is assumed by the SpatialReference.
*
* @param poNewRoot object to assign as root.
*/

void SpatialReference::setRoot(SRSNode * poNewRoot)

{
	if (poRoot != NULL)
		delete poRoot;

	poRoot = poNewRoot;
}

/************************************************************************/
/*                            getAttrNode()                             */
/************************************************************************/

/**
* \brief Find named node in tree.
*
* This method does a pre-order traversal of the node tree searching for
* a node with this exact value (case insensitive), and returns it.  Leaf
* nodes are not considered, under the assumption that they are just
* attribute value nodes.
*
* If a node appears more than once in the tree (such as UNIT for instance),
* the first encountered will be returned.  Use getNode() on a subtree to be
* more specific.
*
* @param pszNodePath the name of the node to search for.  May contain multiple
* components such as "GEOGCS|UNIT".
*
* @return a pointer to the node found, or NULL if none.
*/

SRSNode *SpatialReference::getAttrNode(const char * pszNodePath)

{
	char        **papszPathTokens;
	SRSNode *poNode;

	papszPathTokens = cslTokenizeString(pszNodePath, "|", TRUE, FALSE);

	if (cslCount(papszPathTokens) < 1)
	{
		cslDestroy(papszPathTokens);
		return NULL;
	}

	poNode = getRoot();
	for (int i = 0; poNode != NULL && papszPathTokens[i] != NULL; i++)
	{
		poNode = poNode->getNode(papszPathTokens[i]);
	}

	cslDestroy(papszPathTokens);

	return poNode;
}

const SRSNode *
SpatialReference::getAttrNode(const char * pszNodePath) const

{
	SRSNode *poNode;

	poNode = ((SpatialReference *) this)->getAttrNode(pszNodePath);

	return poNode;
}

/************************************************************************/
/*                            getAttrValue()                            */
/************************************************************************/

/**
* \brief Fetch indicated attribute of named node.
*
* This method uses getAttrNode() to find the named node, and then extracts
* the value of the indicated child.  Thus a call to getAttrValue("UNIT",1)
* would return the second child of the UNIT node, which is normally the
* length of the linear unit in meters.
*
* This method does the same thing as the C function OSRGetAttrValue().
*
* @param pszNodeName the tree node to look for (case insensitive).
* @param iAttr the child of the node to fetch (zero based).
*
* @return the requested value, or NULL if it fails for any reason.
*/

const char *SpatialReference::getAttrValue(const char * pszNodeName,
	int iAttr) const

{
	const SRSNode *poNode;

	poNode = getAttrNode(pszNodeName);
	if (poNode == NULL)
		return NULL;

	if (iAttr < 0 || iAttr >= poNode->getChildCount())
		return NULL;

	return poNode->getChild(iAttr)->getValue();
}


/************************************************************************/
/*                               clone()                                */
/************************************************************************/

/**
* \brief Make a duplicate of this SpatialReference.
*
* This method is the same as the C function OSRClone().
*
* @return a new SRS, which becomes the responsibility of the caller.
*/

SpatialReference *SpatialReference::clone() const

{
	SpatialReference *poNewRef;

	poNewRef = new SpatialReference();

	if (poRoot != NULL)
		poNewRef->poRoot = poRoot->clone();

	return poNewRef;
}


/************************************************************************/
/*                            dumpReadable()                            */
/*                                                                      */
/*      Dump pretty wkt to stdout, mostly for debugging.                */
/************************************************************************/

void SpatialReference::dumpReadable()

{
	char *pszPrettyWkt = NULL;

	exportToPrettyWkt(&pszPrettyWkt, FALSE);
	printf("%s\n", pszPrettyWkt);
	free(pszPrettyWkt);
}

/************************************************************************/
/*                         exportToPrettyWkt()                          */
/************************************************************************/

/**
* Convert this SRS into a a nicely formatted WKT string for display to a person.
*
* Note that the returned WKT string should be freed with OGRFree() or
* free() when no longer needed.  It is the responsibility of the caller.
*
* This method is the same as the C function OSRExportToPrettyWkt().
*
* @param ppszResult the resulting string is returned in this pointer.
* @param bSimplify TRUE if the AXIS, AUTHORITY and EXTENSION nodes should be stripped off
*
* @return currently true is always returned, but the future it
* is possible error conditions will develop.
*/

bool SpatialReference::exportToPrettyWkt(char ** ppszResult,
	int bSimplify) const

{
	if (poRoot == NULL)
	{
		*ppszResult = cslDuplicateString("");
		return true;
	}

	if (bSimplify)
	{
		SpatialReference *poSimpleClone = clone();
		bool eErr;

		poSimpleClone->getRoot()->stripNodes("AXIS");
		poSimpleClone->getRoot()->stripNodes("AUTHORITY");
		poSimpleClone->getRoot()->stripNodes("EXTENSION");
		eErr = poSimpleClone->getRoot()->exportToPrettyWkt(ppszResult, 1);
		delete poSimpleClone;
		return eErr;
	}
	else
		return poRoot->exportToPrettyWkt(ppszResult, 1);
}
 
/************************************************************************/
/*                            exportToWkt()                             */
/************************************************************************/

/**
* \brief Convert this SRS into WKT format.
*
* Note that the returned WKT string should be freed with OGRFree() or
* free() when no longer needed.  It is the responsibility of the caller.
*
* This method is the same as the C function OSRExportToWkt().
*
* @param ppszResult the resulting string is returned in this pointer.
*
* @return currently true is always returned, but the future it
* is possible error conditions will develop.
*/

bool SpatialReference::exportToWkt(char ** ppszResult) const

{
	if (poRoot == NULL)
	{
		*ppszResult = cslDuplicateString("");
		return true;
	}
	else
	{
		return poRoot->exportToWkt(ppszResult);
	}
}
 
/************************************************************************/
/*                           importFromWkt()                            */
/************************************************************************/

/**
* \brief Import from WKT string.
*
* This method will wipe the existing SRS definition, and
* reassign it based on the contents of the passed WKT string.  Only as
* much of the input string as needed to construct this SRS is consumed from
* the input string, and the input string pointer
* is then updated to point to the remaining (unused) input.
*
* This method is the same as the C function OSRImportFromWkt().
*
* @param ppszInput Pointer to pointer to input.  The pointer is updated to
* point to remaining unused input text.
*
* @return true if import succeeds, or false if it
* fails for any reason.
*/

bool SpatialReference::importFromWkt(char ** ppszInput)

{
	if (!ppszInput || !*ppszInput)
		return false;

	clear();

	poRoot = new SRSNode();

	bool eErr = poRoot->importFromWkt(ppszInput);
	if (eErr != true)
		return eErr;

	/* -------------------------------------------------------------------- */
	/*      The following seems to try and detect and unconsumed            */
	/*      VERTCS[] coordinate system definition (ESRI style) and to       */
	/*      import and attach it to the existing root.  Likely we will      */
	/*      need to extend this somewhat to bring it into an acceptable     */
	/*      SpatialReference organization at some point.                 */
	/* -------------------------------------------------------------------- */
	if (strlen(*ppszInput) > 0 && strstr(*ppszInput, "VERTCS"))
	{
		if (((*ppszInput)[0]) == ',')
			(*ppszInput)++;
		SRSNode *poNewChild = new SRSNode();
		poRoot->addChild(poNewChild);
		return poNewChild->importFromWkt(ppszInput);
	}

	return eErr;
}


/************************************************************************/
/*                              setNode()                               */
/************************************************************************/

/**
* \brief Set attribute value in spatial reference.
*
* Missing intermediate nodes in the path will be created if not already
* in existance.  If the attribute has no children one will be created and
* assigned the value otherwise the zeroth child will be assigned the value.
*
* This method does the same as the C function OSRSetAttrValue().
*
* @param pszNodePath full path to attribute to be set.  For instance
* "PROJCS|GEOGCS|UNIT".
*
* @param pszNewNodeValue value to be assigned to node, such as "meter".
* This may be NULL if you just want to force creation of the intermediate
* path.
*
* @return true on success.
*/

bool SpatialReference::setNode(const char * pszNodePath,
	const char * pszNewNodeValue)

{
	char        **papszPathTokens;
	int         i;
	SRSNode *poNode;

	papszPathTokens = cslTokenizeString(pszNodePath, "|", TRUE, FALSE);

	if (cslCount(papszPathTokens) < 1)
		return false;

	if (getRoot() == NULL || !cslIEqualString(papszPathTokens[0], getRoot()->getValue()))
	{
		setRoot(new SRSNode(papszPathTokens[0]));
	}

	poNode = getRoot();
	for (i = 1; papszPathTokens[i] != NULL; i++)
	{
		int     j;

		for (j = 0; j < poNode->getChildCount(); j++)
		{
			if (cslIEqualString(poNode->getChild(j)->getValue(), papszPathTokens[i]))
			{
				poNode = poNode->getChild(j);
				j = -1;
				break;
			}
		}

		if (j != -1)
		{
			SRSNode *poNewNode = new SRSNode(papszPathTokens[i]);
			poNode->addChild(poNewNode);
			poNode = poNewNode;
		}
	}

	cslDestroy(papszPathTokens);

	if (pszNewNodeValue != NULL)
	{
		if (poNode->getChildCount() > 0)
			poNode->getChild(0)->setValue(pszNewNodeValue);
		else
			poNode->addChild(new SRSNode(pszNewNodeValue));
	}

	return true;
}
 
/************************************************************************/
/*                              setNode()                               */
/************************************************************************/

bool SpatialReference::setNode(const char *pszNodePath,
	double dfValue)

{
	char        szValue[64];

	if (fabs(dfValue - (int)dfValue) == 0.0)
		sprintf(szValue, "%d", (int)dfValue);
	else
		printDouble(szValue, dfValue);

	return setNode(pszNodePath, szValue);
}

/************************************************************************/
/*                          setAngularUnits()                           */
/************************************************************************/

/**
* \brief Set the angular units for the geographic coordinate system.
*
* This method creates a UNIT subnode with the specified values as a
* child of the GEOGCS node.
*
* This method does the same as the C function OSRSetAngularUnits().
*
* @param pszUnitsName the units name to be used.  Some preferred units
* names can be found in ogr_srs_api.h such as SRS_UA_DEGREE.
*
* @param dfInRadians the value to multiple by an angle in the indicated
* units to transform to radians.  Some standard conversion factors can
* be found in ogr_srs_api.h.
*
* @return true on success.
*/

bool SpatialReference::setAngularUnits(const char * pszUnitsName,
	double dfInRadians)

{
	SRSNode *poCS;
	SRSNode *poUnits;
	char        szValue[128];

	bNormInfoSet = FALSE;

	poCS = getAttrNode("GEOGCS");

	if (poCS == NULL)
		return false;

	printDouble(szValue, dfInRadians);

	if (poCS->findChild("UNIT") >= 0)
	{
		poUnits = poCS->getChild(poCS->findChild("UNIT"));
		if (poUnits->getChildCount() < 2)
			return false;
		poUnits->getChild(0)->setValue(pszUnitsName);
		poUnits->getChild(1)->setValue(szValue);
	}
	else
	{
		poUnits = new SRSNode("UNIT");
		poUnits->addChild(new SRSNode(pszUnitsName));
		poUnits->addChild(new SRSNode(szValue));

		poCS->addChild(poUnits);
	}

	return true;
}

/************************************************************************/
/*                          getAngularUnits()                           */
/************************************************************************/

/**
* \brief Fetch angular geographic coordinate system units.
*
* If no units are available, a value of "degree" and SRS_UA_DEGREE_CONV
* will be assumed.  This method only checks directly under the GEOGCS node
* for units.
*
* This method does the same thing as the C function OSRGetAngularUnits().
*
* @param ppszName a pointer to be updated with the pointer to the
* units name.  The returned value remains internal to the SpatialReference
* and shouldn't be freed, or modified.  It may be invalidated on the next
* SpatialReference call.
*
* @return the value to multiply by angular distances to transform them to
* radians.
*/

double SpatialReference::getAngularUnits(char ** ppszName) const

{
	const SRSNode *poCS = getAttrNode("GEOGCS");

	if (ppszName != NULL)
		*ppszName = (char*) "degree";

	if (poCS == NULL)
		return cslStringToNumber(SRS_UA_DEGREE_CONV);

	for (int iChild = 0; iChild < poCS->getChildCount(); iChild++)
	{
		const SRSNode     *poChild = poCS->getChild(iChild);

		if (cslIEqualString(poChild->getValue(), "UNIT")
			&& poChild->getChildCount() >= 2)
		{
			if (ppszName != NULL)
				*ppszName = (char *)poChild->getChild(0)->getValue();

			return cslStringToNumber(poChild->getChild(1)->getValue());
		}
	}

	return 1.0;
}
 

/************************************************************************/
/*                 setLinearUnitsAndUpdateParameters()                  */
/************************************************************************/

/**
* \brief Set the linear units for the projection.
*
* This method creates a UNIT subnode with the specified values as a
* child of the PROJCS or LOCAL_CS node.   It works the same as the
* setLinearUnits() method, but it also updates all existing linear
* projection parameter values from the old units to the new units.
*
* @param pszName the units name to be used.  Some preferred units
* names can be found in ogr_srs_api.h such as SRS_UL_METER, SRS_UL_FOOT
* and SRS_UL_US_FOOT.
*
* @param dfInMeters the value to multiple by a length in the indicated
* units to transform to meters.  Some standard conversion factors can
* be found in ogr_srs_api.h.
*
* @return true on success.
*/

bool SpatialReference::setLinearUnitsAndUpdateParameters(
	const char *pszName, double dfInMeters)

{
	double dfOldInMeters = getLinearUnits();
	SRSNode *poPROJCS = getAttrNode("PROJCS");

	if (dfInMeters == 0.0)
		return false;

	if (dfInMeters == dfOldInMeters || poPROJCS == NULL)
		return setLinearUnits(pszName, dfInMeters);

	for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
	{
		const SRSNode     *poChild = poPROJCS->getChild(iChild);

		if (cslIEqualString(poChild->getValue(), "PARAMETER")
			&& poChild->getChildCount() > 1)
		{
			char *pszParmName = cslDuplicateString(poChild->getChild(0)->getValue());

			if (isLinearParameter(pszParmName))
			{
				double dfOldValue = getProjParm(pszParmName);

				setProjParm(pszParmName,
					dfOldValue * dfOldInMeters / dfInMeters);
			}

			free(pszParmName);
		}
	}

	return setLinearUnits(pszName, dfInMeters);
}
 
/************************************************************************/
/*                           setLinearUnits()                           */
/************************************************************************/

/**
* \brief Set the linear units for the projection.
*
* This method creates a UNIT subnode with the specified values as a
* child of the PROJCS, GEOCCS or LOCAL_CS node.
*
* This method does the same as the C function OSRSetLinearUnits().
*
* @param pszUnitsName the units name to be used.  Some preferred units
* names can be found in ogr_srs_api.h such as SRS_UL_METER, SRS_UL_FOOT
* and SRS_UL_US_FOOT.
*
* @param dfInMeters the value to multiple by a length in the indicated
* units to transform to meters.  Some standard conversion factors can
* be found in ogr_srs_api.h.
*
* @return true on success.
*/

bool SpatialReference::setLinearUnits(const char * pszUnitsName,
	double dfInMeters)

{
	return setTargetLinearUnits(NULL, pszUnitsName, dfInMeters);
}
 
/************************************************************************/
/*                        setTargetLinearUnits()                        */
/******************************* *****************************************/

/**
* \brief Set the linear units for the projection.
*
* This method creates a UNIT subnode with the specified values as a
* child of the target node.
*
* This method does the same as the C function OSRSetTargetLinearUnits().
*
* @param pszTargetKey the keyword to set the linear units for.  ie. "PROJCS" or "VERT_CS"
*
* @param pszUnitsName the units name to be used.  Some preferred units
* names can be found in ogr_srs_api.h such as SRS_UL_METER, SRS_UL_FOOT
* and SRS_UL_US_FOOT.
*
* @param dfInMeters the value to multiple by a length in the indicated
* units to transform to meters.  Some standard conversion factors can
* be found in ogr_srs_api.h.
*
* @return true on success.
*
* @since OGR 1.9.0
*/

bool SpatialReference::setTargetLinearUnits(const char *pszTargetKey,
	const char * pszUnitsName,
	double dfInMeters)

{
	SRSNode *poCS;
	SRSNode *poUnits;
	char        szValue[128];

	bNormInfoSet = FALSE;

	if (pszTargetKey == NULL)
	{
		poCS = getAttrNode("PROJCS");

		if (poCS == NULL)
			poCS = getAttrNode("LOCAL_CS");
		if (poCS == NULL)
			poCS = getAttrNode("GEOCCS");
		if (poCS == NULL && isVertical())
			poCS = getAttrNode("VERT_CS");
	}
	else
		poCS = getAttrNode(pszTargetKey);

	if (poCS == NULL)
		return false;

	if (dfInMeters == (int)dfInMeters)
		sprintf(szValue, "%d", (int)dfInMeters);
	else
		printDouble(szValue, dfInMeters);

	if (poCS->findChild("UNIT") >= 0)
	{
		poUnits = poCS->getChild(poCS->findChild("UNIT"));
		if (poUnits->getChildCount() < 2)
			return false;
		poUnits->getChild(0)->setValue(pszUnitsName);
		poUnits->getChild(1)->setValue(szValue);
		if (poUnits->findChild("AUTHORITY") != -1)
			poUnits->destroyChild(poUnits->findChild("AUTHORITY"));
	}
	else
	{
		poUnits = new SRSNode("UNIT");
		poUnits->addChild(new SRSNode(pszUnitsName));
		poUnits->addChild(new SRSNode(szValue));

		poCS->addChild(poUnits);
	}

	return true;
}
 
/************************************************************************/
/*                           getLinearUnits()                           */
/************************************************************************/

/**
* \brief Fetch linear projection units.
*
* If no units are available, a value of "Meters" and 1.0 will be assumed.
* This method only checks directly under the PROJCS, GEOCCS or LOCAL_CS node
* for units.
*
* This method does the same thing as the C function OSRGetLinearUnits()/
*
* @param ppszName a pointer to be updated with the pointer to the
* units name.  The returned value remains internal to the SpatialReference
* and shouldn't be freed, or modified.  It may be invalidated on the next
* SpatialReference call.
*
* @return the value to multiply by linear distances to transform them to
* meters.
*/

double SpatialReference::getLinearUnits(char ** ppszName) const

{
	return getTargetLinearUnits(NULL, ppszName);
}
 

/************************************************************************/
/*                        getTargetLinearUnits()                        */
/************************************************************************/

/**
* \brief Fetch linear units for target.
*
* If no units are available, a value of "Meters" and 1.0 will be assumed.
*
* This method does the same thing as the C function OSRGetTargetLinearUnits()/
*
* @param pszTargetKey the key to look on. ie. "PROJCS" or "VERT_CS".
* @param ppszName a pointer to be updated with the pointer to the
* units name.  The returned value remains internal to the SpatialReference
* and shouldn't be freed, or modified.  It may be invalidated on the next
* SpatialReference call.
*
* @return the value to multiply by linear distances to transform them to
* meters.
*
* @since OGR 1.9.0
*/

double SpatialReference::getTargetLinearUnits(const char *pszTargetKey,
	char ** ppszName) const

{
	const SRSNode *poCS;

	if (pszTargetKey == NULL)
	{
		poCS = getAttrNode("PROJCS");

		if (poCS == NULL)
			poCS = getAttrNode("LOCAL_CS");
		if (poCS == NULL)
			poCS = getAttrNode("GEOCCS");
		if (poCS == NULL && isVertical())
			poCS = getAttrNode("VERT_CS");
	}
	else
		poCS = getAttrNode(pszTargetKey);

	if (ppszName != NULL)
		*ppszName = (char*) "unknown";

	if (poCS == NULL)
		return 1.0;

	for (int iChild = 0; iChild < poCS->getChildCount(); iChild++)
	{
		const SRSNode     *poChild = poCS->getChild(iChild);

		if (cslIEqualString(poChild->getValue(), "UNIT")
			&& poChild->getChildCount() >= 2)
		{
			if (ppszName != NULL)
				*ppszName = (char *)poChild->getChild(0)->getValue();

			return cslStringToNumber(poChild->getChild(1)->getValue());
		}
	}

	return 1.0;
}
 
/************************************************************************/
/*                          getPrimeMeridian()                          */
/************************************************************************/

/**
* \brief Fetch prime meridian info.
*
* Returns the offset of the prime meridian from greenwich in degrees,
* and the prime meridian name (if requested).   If no PRIMEM value exists
* in the coordinate system definition a value of "Greenwich" and an
* offset of 0.0 is assumed.
*
* If the prime meridian name is returned, the pointer is to an internal
* copy of the name. It should not be freed, altered or depended on after
* the next OGR call.
*
* This method is the same as the C function OSRGetPrimeMeridian().
*
* @param ppszName return location for prime meridian name.  If NULL, name
* is not returned.
*
* @return the offset to the GEOGCS prime meridian from greenwich in decimal
* degrees.
*/

double SpatialReference::getPrimeMeridian(char **ppszName) const

{
	const SRSNode *poPRIMEM = getAttrNode("PRIMEM");

	if (poPRIMEM != NULL && poPRIMEM->getChildCount() >= 2
		&& cslStringToNumber(poPRIMEM->getChild(1)->getValue()) != 0.0)
	{
		if (ppszName != NULL)
			*ppszName = (char *)poPRIMEM->getChild(0)->getValue();
		return cslStringToNumber(poPRIMEM->getChild(1)->getValue());
	}

	if (ppszName != NULL)
		*ppszName = (char*)SRS_PM_GREENWICH;

	return 0.0;
}
 
/************************************************************************/
/*                             setGeogCS()                              */
/************************************************************************/

/**
* \brief Set geographic coordinate system.
*
* This method is used to set the datum, ellipsoid, prime meridian and
* angular units for a geographic coordinate system.  It can be used on it's
* own to establish a geographic spatial reference, or applied to a
* projected coordinate system to establish the underlying geographic
* coordinate system.
*
* This method does the same as the C function OSRSetGeogCS().
*
* @param pszGeogName user visible name for the geographic coordinate system
* (not to serve as a key).
*
* @param pszDatumName key name for this datum.  The OpenGIS specification
* lists some known values, and otherwise EPSG datum names with a standard
* transformation are considered legal keys.
*
* @param pszSpheroidName user visible spheroid name (not to serve as a key)
*
* @param dfSemiMajor the semi major axis of the spheroid.
*
* @param dfInvFlattening the inverse flattening for the spheroid.
* This can be computed from the semi minor axis as
* 1/f = 1.0 / (1.0 - semiminor/semimajor).
*
* @param pszPMName the name of the prime merdidian (not to serve as a key)
* If this is NULL a default value of "Greenwich" will be used.
*
* @param dfPMOffset the longitude of greenwich relative to this prime
* meridian.
*
* @param pszAngularUnits the angular units name (see ogr_srs_api.h for some
* standard names).  If NULL a value of "degrees" will be assumed.
*
* @param dfConvertToRadians value to multiply angular units by to transform
* them to radians.  A value of SRS_UL_DEGREE_CONV will be used if
* pszAngularUnits is NULL.
*
* @return true on success.
*/

bool
SpatialReference::setGeogCS(const char * pszGeogName,
	const char * pszDatumName,
	const char * pszSpheroidName,
	double dfSemiMajor, double dfInvFlattening,
	const char * pszPMName, double dfPMOffset,
	const char * pszAngularUnits,
	double dfConvertToRadians)

{
	bNormInfoSet = FALSE;

	/* -------------------------------------------------------------------- */
	/*      For a geocentric coordinate system we want to set the datum     */
	/*      and ellipsoid based on the GEOGCS.  Create the GEOGCS in a      */
	/*      temporary srs and use the copy method which has special         */
	/*      handling for GEOCCS.                                            */
	/* -------------------------------------------------------------------- */
	if (isGeocentric())
	{
		SpatialReference oGCS;

		oGCS.setGeogCS(pszGeogName, pszDatumName, pszSpheroidName,
			dfSemiMajor, dfInvFlattening,
			pszPMName, dfPMOffset,
			pszAngularUnits, dfConvertToRadians);
		return copyGeogCSFrom(&oGCS);
	}

	/* -------------------------------------------------------------------- */
	/*      Do we already have a GEOGCS?  If so, blow it away so it can     */
	/*      be properly replaced.                                           */
	/* -------------------------------------------------------------------- */
	if (getAttrNode("GEOGCS") != NULL)
	{
		SRSNode *poCS;

		if (cslIEqualString(getRoot()->getValue(), "GEOGCS"))
			clear();
		else if ((poCS = getAttrNode("PROJCS")) != NULL
			&& poCS->findChild("GEOGCS") != -1)
			poCS->destroyChild(poCS->findChild("GEOGCS"));
		else
			return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Set defaults for various parameters.                            */
	/* -------------------------------------------------------------------- */
	if (pszGeogName == NULL)
		pszGeogName = "unnamed";

	if (pszPMName == NULL)
		pszPMName = SRS_PM_GREENWICH;

	if (pszDatumName == NULL)
		pszDatumName = "unknown";

	if (pszSpheroidName == NULL)
		pszSpheroidName = "unnamed";

	if (pszAngularUnits == NULL)
	{
		pszAngularUnits = SRS_UA_DEGREE;
		dfConvertToRadians = cslStringToNumber(SRS_UA_DEGREE_CONV);
	}

	/* -------------------------------------------------------------------- */
	/*      Build the GEOGCS object.                                        */
	/* -------------------------------------------------------------------- */
	char                szValue[128];
	SRSNode         *poGeogCS, *poSpheroid, *poDatum, *poPM, *poUnits;

	poGeogCS = new SRSNode("GEOGCS");
	poGeogCS->addChild(new SRSNode(pszGeogName));

	/* -------------------------------------------------------------------- */
	/*      Setup the spheroid.                                             */
	/* -------------------------------------------------------------------- */
	poSpheroid = new SRSNode("SPHEROID");
	poSpheroid->addChild(new SRSNode(pszSpheroidName));

	printDouble(szValue, dfSemiMajor);
	poSpheroid->addChild(new SRSNode(szValue));

	printDouble(szValue, dfInvFlattening);
	poSpheroid->addChild(new SRSNode(szValue));

	/* -------------------------------------------------------------------- */
	/*      Setup the Datum.                                                */
	/* -------------------------------------------------------------------- */
	poDatum = new SRSNode("DATUM");
	poDatum->addChild(new SRSNode(pszDatumName));
	poDatum->addChild(poSpheroid);

	/* -------------------------------------------------------------------- */
	/*      Setup the prime meridian.                                       */
	/* -------------------------------------------------------------------- */
	if (dfPMOffset == 0.0)
		strcpy(szValue, "0");
	else
		printDouble(szValue, dfPMOffset);

	poPM = new SRSNode("PRIMEM");
	poPM->addChild(new SRSNode(pszPMName));
	poPM->addChild(new SRSNode(szValue));

	/* -------------------------------------------------------------------- */
	/*      Setup the rotational units.                                     */
	/* -------------------------------------------------------------------- */
	printDouble(szValue, dfConvertToRadians);

	poUnits = new SRSNode("UNIT");
	poUnits->addChild(new SRSNode(pszAngularUnits));
	poUnits->addChild(new SRSNode(szValue));

	/* -------------------------------------------------------------------- */
	/*      Complete the GeogCS                                             */
	/* -------------------------------------------------------------------- */
	poGeogCS->addChild(poDatum);
	poGeogCS->addChild(poPM);
	poGeogCS->addChild(poUnits);

	/* -------------------------------------------------------------------- */
	/*      Attach below the PROJCS if there is one, or make this the root. */
	/* -------------------------------------------------------------------- */
	if (getRoot() != NULL && cslIEqualString(getRoot()->getValue(), "PROJCS"))
		poRoot->insertChild(poGeogCS, 1);
	else
		setRoot(poGeogCS);

	return true;
}
 
/************************************************************************/
/*                         setWellKnownGeogCS()                         */
/************************************************************************/

/**
* \brief Set a GeogCS based on well known name.
*
* This may be called on an empty SpatialReference to make a geographic
* coordinate system, or on something with an existing PROJCS node to
* set the underlying geographic coordinate system of a projected coordinate
* system.
*
* The following well known text values are currently supported:
* <ul>
* <li> "WGS84": same as "EPSG:4326" but has no dependence on EPSG data files.
* <li> "WGS72": same as "EPSG:4322" but has no dependence on EPSG data files.
* <li> "NAD27": same as "EPSG:4267" but has no dependence on EPSG data files.
* <li> "NAD83": same as "EPSG:4269" but has no dependence on EPSG data files.
* <li> "EPSG:n": same as doing an ImportFromEPSG(n).
* </ul>
*
* @param pszName name of well known geographic coordinate system.
* @return true on success, or false if the name isn't
* recognised, the target object is already initialized, or an EPSG value
* can't be successfully looked up.
*/

bool SpatialReference::setWellKnownGeogCS(const char * pszName)

{
	SpatialReference   oSRS2;
	bool eErr;

	/* -------------------------------------------------------------------- */
	/*      Check for EPSG authority numbers.                               */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszName, "EPSG:", 5))
	{
		eErr = oSRS2.importFromEPSG(atoi(pszName + 5));
		if (eErr != true)
			return eErr;

		if (!oSRS2.isGeographic())
			return false;

		return copyGeogCSFrom(&oSRS2);
	}

	/* -------------------------------------------------------------------- */
	/*      Check for EPSGA authority numbers.                               */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszName, "EPSGA:", 6))
	{
		eErr = oSRS2.importFromEPSGA(atoi(pszName + 6));
		if (eErr != true)
			return eErr;

		if (!oSRS2.isGeographic())
			return false;

		return copyGeogCSFrom(&oSRS2);
	}

	/* -------------------------------------------------------------------- */
	/*      Check for simple names.                                         */
	/* -------------------------------------------------------------------- */
	char        *pszWKT = NULL;

	if (cslIEqualString(pszName, "WGS84") || cslIEqualString(pszName, "CRS84") || cslIEqualString(pszName, "CRS:84"))
		pszWKT = (char*)SRS_WKT_WGS84;

	else if (cslIEqualString(pszName, "WGS72"))
		pszWKT = (char*) "GEOGCS[\"WGS 72\",DATUM[\"WGS_1972\",SPHEROID[\"WGS 72\",6378135,298.26,AUTHORITY[\"EPSG\",\"7043\"]],TOWGS84[0,0,4.5,0,0,0.554,0.2263],AUTHORITY[\"EPSG\",\"6322\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4322\"]]";

	else if (cslIEqualString(pszName, "NAD27") || cslIEqualString(pszName, "CRS27") || cslIEqualString(pszName, "CRS:27"))
		pszWKT = (char*) "GEOGCS[\"NAD27\",DATUM[\"North_American_Datum_1927\",SPHEROID[\"Clarke 1866\",6378206.4,294.978698213898,AUTHORITY[\"EPSG\",\"7008\"]],AUTHORITY[\"EPSG\",\"6267\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4267\"]]";

	else if (cslIEqualString(pszName, "NAD83") || cslIEqualString(pszName, "CRS83") || cslIEqualString(pszName, "CRS:83"))
		pszWKT = (char*) "GEOGCS[\"NAD83\",DATUM[\"North_American_Datum_1983\",SPHEROID[\"GRS 1980\",6378137,298.257222101,AUTHORITY[\"EPSG\",\"7019\"]],TOWGS84[0,0,0,0,0,0,0],AUTHORITY[\"EPSG\",\"6269\"]],PRIMEM[\"Greenwich\",0,AUTHORITY[\"EPSG\",\"8901\"]],UNIT[\"degree\",0.0174532925199433,AUTHORITY[\"EPSG\",\"9108\"]],AUTHORITY[\"EPSG\",\"4269\"]]";

	else
		return false;

	/* -------------------------------------------------------------------- */
	/*      Import the WKT                                                  */
	/* -------------------------------------------------------------------- */
	eErr = oSRS2.importFromWkt(&pszWKT);
	if (eErr != true)
		return eErr;

	/* -------------------------------------------------------------------- */
	/*      Copy over.                                                      */
	/* -------------------------------------------------------------------- */
	return copyGeogCSFrom(&oSRS2);
}
 
/************************************************************************/
/*                           copyGeogCSFrom()                           */
/************************************************************************/

/**
* \brief Copy GEOGCS from another SpatialReference.
*
* The GEOGCS information is copied into this SpatialReference from another.
* If this object has a PROJCS root already, the GEOGCS is installed within
* it, otherwise it is installed as the root.
*
* @param poSrcSRS the spatial reference to copy the GEOGCS information from.
*
* @return true on success or an error code.
*/


bool SpatialReference::copyGeogCSFrom(
	const SpatialReference * poSrcSRS)

{
	const SRSNode  *poGeogCS = NULL;

	bNormInfoSet = FALSE;

	/* -------------------------------------------------------------------- */
	/*      Handle geocentric coordinate systems specially.  We just        */
	/*      want to copy the DATUM and PRIMEM nodes.                        */
	/* -------------------------------------------------------------------- */
	if (isGeocentric())
	{
		if (getRoot()->findChild("DATUM") != -1)
			getRoot()->destroyChild(getRoot()->findChild("DATUM"));
		if (getRoot()->findChild("PRIMEM") != -1)
			getRoot()->destroyChild(getRoot()->findChild("PRIMEM"));

		const SRSNode *poDatum = poSrcSRS->getAttrNode("DATUM");
		const SRSNode *poPrimeM = poSrcSRS->getAttrNode("PRIMEM");

		if (poDatum == NULL || poPrimeM == NULL)
			return false;

		poRoot->insertChild(poDatum->clone(), 1);
		poRoot->insertChild(poPrimeM->clone(), 2);

		return true;

	}

	/* -------------------------------------------------------------------- */
	/*      Do we already have a GEOGCS?  If so, blow it away so it can     */
	/*      be properly replaced.                                           */
	/* -------------------------------------------------------------------- */
	if (getAttrNode("GEOGCS") != NULL)
	{
		SRSNode *poPROJCS;

		if (cslIEqualString(getRoot()->getValue(), "GEOGCS"))
			clear();
		else if ((poPROJCS = getAttrNode("PROJCS")) != NULL
			&& poPROJCS->findChild("GEOGCS") != -1)
			poPROJCS->destroyChild(poPROJCS->findChild("GEOGCS"));
		else
			return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Find the GEOGCS node on the source.                             */
	/* -------------------------------------------------------------------- */
	poGeogCS = poSrcSRS->getAttrNode("GEOGCS");
	if (poGeogCS == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      Attach below the PROJCS if there is one, or make this the root. */
	/* -------------------------------------------------------------------- */
	if (getRoot() != NULL && cslIEqualString(getRoot()->getValue(), "PROJCS"))
		poRoot->insertChild(poGeogCS->clone(), 1);
	else
		setRoot(poGeogCS->clone());

	return true;
}
 
/************************************************************************/
/*                          setFromUserInput()                          */
/************************************************************************/

/**
* \brief Set spatial reference from various text formats.
*
* This method will examine the provided input, and try to deduce the
* format, and then use it to initialize the spatial reference system.  It
* may take the following forms:
*
* <ol>
* <li> Well Known Text definition - passed on to importFromWkt().
* <li> "EPSG:n" - number passed on to importFromEPSG().
* <li> "EPSGA:n" - number passed on to importFromEPSGA().
* <li> "AUTO:proj_id,unit_id,lon0,lat0" - WMS auto projections.
* <li> "urn:ogc:def:crs:EPSG::n" - ogc urns
* <li> PROJ.4 definitions - passed on to importFromProj4().
* <li> filename - file read for WKT, XML or PROJ.4 definition.
* <li> well known name accepted by setWellKnownGeogCS(), such as NAD27, NAD83,
* WGS84 or WGS72.
* <li> WKT (directly or in a file) in ESRI format should be prefixed with
* ESRI:: to trigger an automatic morphFromESRI().
* <li> "IGNF:xxx" - "+init=IGNF:xxx" passed on to importFromProj4().
* </ol>
*
* It is expected that this method will be extended in the future to support
* XML and perhaps a simplified "minilanguage" for indicating common UTM and
* State Plane definitions.
*
* This method is intended to be flexible, but by it's nature it is
* imprecise as it must guess information about the format intended.  When
* possible applications should call the specific method appropriate if the
* input is known to be in a particular format.
*
* This method does the same thing as the OSRSetFromUserInput() function.
*
* @param pszDefinition text definition to try to deduce SRS from.
*
* @return true on success, or an error code if the name isn't
* recognised, the definition is corrupt, or an EPSG value can't be
* successfully looked up.
*/

bool SpatialReference::setFromUserInput(const char * pszDefinition)

{
	int     bESRI = FALSE;
	bool  err;

	if (cslNIEqualString(pszDefinition, "ESRI::", 6))
	{
		bESRI = TRUE;
		pszDefinition += 6;
	}

	/* -------------------------------------------------------------------- */
	/*      Is it a recognised syntax?                                      */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszDefinition, "PROJCS", 6)
		|| cslNIEqualString(pszDefinition, "GEOGCS", 6)
		|| cslNIEqualString(pszDefinition, "COMPD_CS", 8)
		|| cslNIEqualString(pszDefinition, "GEOCCS", 6)
		|| cslNIEqualString(pszDefinition, "VERT_CS", 7)
		|| cslNIEqualString(pszDefinition, "LOCAL_CS", 8))
	{
		err = importFromWkt((char **)&pszDefinition);
		if (err == true && bESRI)
			err = morphFromESRI();

		return err;
	}

	if (cslNIEqualString(pszDefinition, "EPSG:", 5)
		|| cslNIEqualString(pszDefinition, "EPSGA:", 6))
	{
		bool eStatus;

		if (cslNIEqualString(pszDefinition, "EPSG:", 5))
			eStatus = importFromEPSG(atoi(pszDefinition + 5));

		else /* if( cslNIEqualString(pszDefinition,"EPSGA:",6) ) */
			eStatus = importFromEPSGA(atoi(pszDefinition + 6));

		// Do we want to turn this into a compound definition
		// with a vertical datum?
		if (eStatus == true && strchr(pszDefinition, '+') != NULL)
		{
			SpatialReference oVertSRS;

			eStatus = oVertSRS.importFromEPSG(
				atoi(strchr(pszDefinition, '+') + 1));
			if (eStatus == true)
			{
				SRSNode *poHorizSRS = getRoot()->clone();

				clear();

				String osName = poHorizSRS->getChild(0)->getValue();
				osName += " + ";
				osName += oVertSRS.getRoot()->getValue();

				setNode("COMPD_CS", osName);
				getRoot()->addChild(poHorizSRS);
				getRoot()->addChild(oVertSRS.getRoot()->clone());
			}

			return eStatus;
		}
		else
			return eStatus;
	}

	if (cslNIEqualString(pszDefinition, "urn:ogc:def:crs:", 16)
		|| cslNIEqualString(pszDefinition, "urn:ogc:def:crs,crs:", 20)
		|| cslNIEqualString(pszDefinition, "urn:x-ogc:def:crs:", 18)
		|| cslNIEqualString(pszDefinition, "urn:opengis:crs:", 16)
		|| cslNIEqualString(pszDefinition, "urn:opengis:def:crs:", 20))
		return importFromURN(pszDefinition);

	if (cslNIEqualString(pszDefinition, "http://opengis.net/def/crs", 26)
		|| cslNIEqualString(pszDefinition, "http://www.opengis.net/def/crs", 30)
		|| cslNIEqualString(pszDefinition, "www.opengis.net/def/crs", 23))
		return importFromCRSURL(pszDefinition);

	if (cslNIEqualString(pszDefinition, "AUTO:", 5))
		return importFromWMSAUTO(pszDefinition);

	if (cslNIEqualString(pszDefinition, "OGC:", 4))  // WMS/WCS OGC codes like OGC:CRS84
		return setWellKnownGeogCS(pszDefinition + 4);

	if (cslNIEqualString(pszDefinition, "CRS:", 4))
		return setWellKnownGeogCS(pszDefinition);

	if (cslNIEqualString(pszDefinition, "DICT:", 5)
		&& strstr(pszDefinition, ","))
	{
		char *pszFile = cslDuplicateString(pszDefinition + 5);
		char *pszCode = strstr(pszFile, ",") + 1;

		pszCode[-1] = '\0';

		err = importFromDict(pszFile, pszCode);
		free(pszFile);

		if (err == true && bESRI)
			err = morphFromESRI();

		return err;
	}

	if (cslIEqualString(pszDefinition, "NAD27")
		|| cslIEqualString(pszDefinition, "NAD83")
		|| cslIEqualString(pszDefinition, "WGS84")
		|| cslIEqualString(pszDefinition, "WGS72"))
	{
		clear();
		return setWellKnownGeogCS(pszDefinition);
	}

	if (strstr(pszDefinition, "+proj") != NULL
		|| strstr(pszDefinition, "+init") != NULL)
		return importFromProj4(pszDefinition);

	if (cslNIEqualString(pszDefinition, "IGNF:", 5))
	{
		char* pszProj4Str = (char*)malloc(6 + strlen(pszDefinition) + 1);
		strcpy(pszProj4Str, "+init=");
		strcat(pszProj4Str, pszDefinition);
		err = importFromProj4(pszProj4Str);
		free(pszProj4Str);

		return err;
	}

	if (cslNIEqualString(pszDefinition, "http://", 7))
	{
		return err;// importFromUrl(pszDefinition);
	}

	if (cslIEqualString(pszDefinition, "osgb:BNG"))
	{
		return importFromEPSG(27700);
	}

	/* -------------------------------------------------------------------- */
	/*      Try to open it as a file.                                       */
	/* -------------------------------------------------------------------- */
	FILE        *fp;
	int         nBufMax = 100000;
	char        *pszBufPtr, *pszBuffer;
	int         nBytes;

	fp = fopen(pszDefinition, "rt");
	if (fp == NULL)
		return false;

	pszBuffer = (char *)malloc(nBufMax);
	nBytes = fread(pszBuffer, 1, nBufMax - 1, fp);
	fclose(fp);

	if (nBytes == nBufMax - 1)
	{
		free(pszBuffer);
		return false;
	}

	pszBuffer[nBytes] = '\0';

	pszBufPtr = pszBuffer;
	while (pszBufPtr[0] == ' ' || pszBufPtr[0] == '\n')
		pszBufPtr++;

	if (pszBufPtr[0] == '<')
		err = importFromXML(pszBufPtr);
	else if ((strstr(pszBuffer, "+proj") != NULL
		|| strstr(pszBuffer, "+init") != NULL)
		&& (strstr(pszBuffer, "EXTENSION") == NULL
			&& strstr(pszBuffer, "extension") == NULL))
		err = importFromProj4(pszBufPtr);
	else
	{
		if (cslNIEqualString(pszBufPtr, "ESRI::", 6))
		{
			bESRI = TRUE;
			pszBufPtr += 6;
		}

		err = importFromWkt(&pszBufPtr);
		if (err == true && bESRI)
			err = morphFromESRI();
	}

	free(pszBuffer);

	return err;
}

/************************************************************************/
/*                         importFromURNPart()                          */
/************************************************************************/
bool SpatialReference::importFromURNPart(const char* pszAuthority,
	const char* pszCode,
	const char* pszURN)
{

	/* -------------------------------------------------------------------- */
	/*      Is this an EPSG code? Note that we import it with EPSG          */
	/*      preferred axis ordering for geographic coordinate systems!      */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszAuthority, "EPSG", 4))
		return importFromEPSGA(atoi(pszCode));

	/* -------------------------------------------------------------------- */
	/*      Is this an IAU code?  Lets try for the IAU2000 dictionary.      */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszAuthority, "IAU", 3))
		return importFromDict("IAU2000.wkt", pszCode);

	/* -------------------------------------------------------------------- */
	/*      Is this an OGC code?                                            */
	/* -------------------------------------------------------------------- */
	if (!cslNIEqualString(pszAuthority, "OGC", 3))
	{
		return false;
	}

	if (cslNIEqualString(pszCode, "CRS84", 5))
		return setWellKnownGeogCS(pszCode);
	else if (cslNIEqualString(pszCode, "CRS83", 5))
		return setWellKnownGeogCS(pszCode);
	else if (cslNIEqualString(pszCode, "CRS27", 5))
		return setWellKnownGeogCS(pszCode);

	/* -------------------------------------------------------------------- */
	/*      Handle auto codes.  We need to convert from format              */
	/*      AUTO42001:99:8888 to format AUTO:42001,99,8888.                 */
	/* -------------------------------------------------------------------- */
	else if (cslNIEqualString(pszCode, "AUTO", 4))
	{
		char szWMSAuto[100];
		int i;

		if (strlen(pszCode) > sizeof(szWMSAuto) - 2)
			return false;

		strcpy(szWMSAuto, "AUTO:");
		strcpy(szWMSAuto + 5, pszCode + 4);
		for (i = 5; szWMSAuto[i] != '\0'; i++)
		{
			if (szWMSAuto[i] == ':')
				szWMSAuto[i] = ',';
		}

		return importFromWMSAUTO(szWMSAuto);
	}

	/* -------------------------------------------------------------------- */
	/*      Not a recognise OGC item.                                       */
	/* -------------------------------------------------------------------- */
	
	return false;
}

/************************************************************************/
/*                           importFromURN()                            */
/*                                                                      */
/*      See OGC recommendation paper 06-023r1 or later for details.     */
/************************************************************************/

/**
* \brief Initialize from OGC URN.
*
* Initializes this spatial reference from a coordinate system defined
* by an OGC URN prefixed with "urn:ogc:def:crs:" per recommendation
* paper 06-023r1.  Currently EPSG and OGC authority values are supported,
* including OGC auto codes, but not including CRS1 or CRS88 (NAVD88).
*
* This method is also support through setFromUserInput() which can
* normally be used for URNs.
*
* @param pszURN the urn string.
*
* @return true on success or an error code.
*/

bool SpatialReference::importFromURN(const char *pszURN)

{
	const char *pszCur;

	if (cslNIEqualString(pszURN, "urn:ogc:def:crs:", 16))
		pszCur = pszURN + 16;
	else if (cslNIEqualString(pszURN, "urn:ogc:def:crs,crs:", 20))
		pszCur = pszURN + 20;
	else if (cslNIEqualString(pszURN, "urn:x-ogc:def:crs:", 18))
		pszCur = pszURN + 18;
	else if (cslNIEqualString(pszURN, "urn:opengis:crs:", 16))
		pszCur = pszURN + 16;
	else if (cslNIEqualString(pszURN, "urn:opengis:def:crs:", 20))
		pszCur = pszURN + 20;
	else
	{
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Clear any existing definition.                                  */
	/* -------------------------------------------------------------------- */
	if (getRoot() != NULL)
	{
		delete poRoot;
		poRoot = NULL;
	}

	/* -------------------------------------------------------------------- */
	/*      Find code (ignoring version) out of string like:                */
	/*                                                                      */
	/*      authority:[version]:code                                        */
	/* -------------------------------------------------------------------- */
	const char *pszAuthority = pszCur;

	// skip authority
	while (*pszCur != ':' && *pszCur)
		pszCur++;
	if (*pszCur == ':')
		pszCur++;

	// skip version
	const char* pszBeforeVersion = pszCur;
	while (*pszCur != ':' && *pszCur)
		pszCur++;
	if (*pszCur == ':')
		pszCur++;
	else
		/* We come here in the case, the content to parse is authority:code (instead of authority::code) */
		/* which is probably illegal according to http://www.opengeospatial.org/ogcUrnPolicy */
		/* but such content is found for example in what is returned by GeoServer */
		pszCur = pszBeforeVersion;

	const char *pszCode = pszCur;

	const char* pszComma = strchr(pszCur, ',');
	if (pszComma == NULL)
		return importFromURNPart(pszAuthority, pszCode, pszURN);


	/* There's a second part with the vertical SRS */
	pszCur = pszComma + 1;
	if (strncmp(pszCur, "crs:", 4) != 0)
	{
		return false;
	}

	pszCur += 4;

	char* pszFirstCode = cslDuplicateString(pszCode);
	pszFirstCode[pszComma - pszCode] = '\0';
	bool eStatus = importFromURNPart(pszAuthority, pszFirstCode, pszURN);
	free(pszFirstCode);

	// Do we want to turn this into a compound definition
	// with a vertical datum?
	if (eStatus == true)
	{
		SpatialReference oVertSRS;

		/* -------------------------------------------------------------------- */
		/*      Find code (ignoring version) out of string like:                */
		/*                                                                      */
		/*      authority:[version]:code                                        */
		/* -------------------------------------------------------------------- */
		pszAuthority = pszCur;

		// skip authority
		while (*pszCur != ':' && *pszCur)
			pszCur++;
		if (*pszCur == ':')
			pszCur++;

		// skip version
		pszBeforeVersion = pszCur;
		while (*pszCur != ':' && *pszCur)
			pszCur++;
		if (*pszCur == ':')
			pszCur++;
		else
			pszCur = pszBeforeVersion;

		pszCode = pszCur;

		eStatus = oVertSRS.importFromURNPart(pszAuthority, pszCode, pszURN);
		if (eStatus == true)
		{
			SRSNode *poHorizSRS = getRoot()->clone();

			clear();

			String osName = poHorizSRS->getChild(0)->getValue();
			osName += " + ";
			osName += oVertSRS.getRoot()->getValue();

			setNode("COMPD_CS", osName);
			getRoot()->addChild(poHorizSRS);
			getRoot()->addChild(oVertSRS.getRoot()->clone());
		}

		return eStatus;
	}
	else
		return eStatus;
}

/************************************************************************/
/*                           importFromCRSURL()                         */
/*                                                                      */
/*      See OGC Best Practice document 11-135 for details.              */
/************************************************************************/

/**
* \brief Initialize from OGC URL.
*
* Initializes this spatial reference from a coordinate system defined
* by an OGC URL prefixed with "http://opengis.net/def/crs" per best practice
* paper 11-135.  Currently EPSG and OGC authority values are supported,
* including OGC auto codes, but not including CRS1 or CRS88 (NAVD88).
*
* This method is also supported through setFromUserInput() which can
* normally be used for URLs.
*
* @param pszURL the URL string.
*
* @return true on success or an error code.
*/

bool SpatialReference::importFromCRSURL(const char *pszURL)

{
	const char *pszCur;

	if (cslNIEqualString(pszURL, "http://opengis.net/def/crs", 26))
		pszCur = pszURL + 26;
	else if (cslNIEqualString(pszURL, "http://www.opengis.net/def/crs", 30))
		pszCur = pszURL + 30;
	else if (cslNIEqualString(pszURL, "www.opengis.net/def/crs", 23))
		pszCur = pszURL + 23;
	else
	{
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Clear any existing definition.                                  */
	/* -------------------------------------------------------------------- */
	if (getRoot() != NULL)
	{
		delete poRoot;
		poRoot = NULL;
	}

	if (cslNIEqualString(pszCur, "-compound?1=", 12))
	{
		/* -------------------------------------------------------------------- */
		/*      It's a compound CRS, of the form:                               */
		/*                                                                      */
		/*      http://opengis.net/def/crs-compound?1=URL1&2=URL2&3=URL3&..     */
		/* -------------------------------------------------------------------- */
		pszCur += 12;

		// extract each component CRS URL
		int iComponentUrl = 2;

		String osName = "";
		clear();

		while (iComponentUrl != -1)
		{
			char searchStr[5];
			sprintf(searchStr, "&%d=", iComponentUrl);

			const char* pszUrlEnd = strstr(pszCur, searchStr);

			// figure out the next component URL
			char* pszComponentUrl;

			if (pszUrlEnd)
			{
				size_t nLen = pszUrlEnd - pszCur;
				pszComponentUrl = (char*)malloc(nLen + 1);
				strncpy(pszComponentUrl, pszCur, nLen);
				pszComponentUrl[nLen] = '\0';

				++iComponentUrl;
				pszCur += nLen + strlen(searchStr);
			}
			else
			{
				if (iComponentUrl == 2)
				{
					return false;
				}
				else
				{
					pszComponentUrl = cslDuplicateString(pszCur);
					// no more components
					iComponentUrl = -1;
				}
			}

			SpatialReference oComponentSRS;
			bool eStatus = oComponentSRS.importFromCRSURL(pszComponentUrl);

			free(pszComponentUrl);
			pszComponentUrl = NULL;

			if (eStatus == true)
			{
				if (osName.length() != 0)
				{
					osName += " + ";
				}
				osName += oComponentSRS.getRoot()->getValue();
				setNode("COMPD_CS", osName);
				getRoot()->addChild(oComponentSRS.getRoot()->clone());
			}
			else
				return eStatus;
		}


		return true;
	}
	else
	{
		/* -------------------------------------------------------------------- */
		/*      It's a normal CRS URL, of the form:                             */
		/*                                                                      */
		/*      http://opengis.net/def/crs/AUTHORITY/VERSION/CODE               */
		/* -------------------------------------------------------------------- */
		++pszCur;
		const char *pszAuthority = pszCur;

		// skip authority
		while (*pszCur != '/' && *pszCur)
			pszCur++;
		if (*pszCur == '/')
			pszCur++;


		// skip version
		while (*pszCur != '/' && *pszCur)
			pszCur++;
		if (*pszCur == '/')
			pszCur++;

		const char *pszCode = pszCur;

		return importFromURNPart(pszAuthority, pszCode, pszURL);
	}
}

/************************************************************************/
/*                         importFromWMSAUTO()                          */
/************************************************************************/

/**
* \brief Initialize from WMSAUTO string.
*
* Note that the WMS 1.3 specification does not include the
* units code, while apparently earlier specs do.  We try to
* guess around this.
*
* @param pszDefinition the WMSAUTO string
*
* @return true on success or an error code.
*/
bool SpatialReference::importFromWMSAUTO(const char * pszDefinition)

{
	char **papszTokens;
	int nProjId, nUnitsId;
	double dfRefLong, dfRefLat = 0.0;

	/* -------------------------------------------------------------------- */
	/*      Tokenize                                                        */
	/* -------------------------------------------------------------------- */
	if (cslNIEqualString(pszDefinition, "AUTO:", 5))
		pszDefinition += 5;

	papszTokens = cslTokenizeString(pszDefinition, ",", FALSE, TRUE);

	if (cslCount(papszTokens) == 4)
	{
		nProjId = atoi(papszTokens[0]);
		nUnitsId = atoi(papszTokens[1]);
		dfRefLong = cslStringToNumber(papszTokens[2]);
		dfRefLat = cslStringToNumber(papszTokens[3]);
	}
	else if (cslCount(papszTokens) == 3 && atoi(papszTokens[0]) == 42005)
	{
		nProjId = atoi(papszTokens[0]);
		nUnitsId = atoi(papszTokens[1]);
		dfRefLong = cslStringToNumber(papszTokens[2]);
		dfRefLat = 0.0;
	}
	else if (cslCount(papszTokens) == 3)
	{
		nProjId = atoi(papszTokens[0]);
		nUnitsId = 9001;
		dfRefLong = cslStringToNumber(papszTokens[1]);
		dfRefLat = cslStringToNumber(papszTokens[2]);

	}
	else if (cslCount(papszTokens) == 2 && atoi(papszTokens[0]) == 42005)
	{
		nProjId = atoi(papszTokens[0]);
		nUnitsId = 9001;
		dfRefLong = cslStringToNumber(papszTokens[1]);
	}
	else
	{
		cslDestroy(papszTokens);
		return false;
	}

	cslDestroy(papszTokens);

	/* -------------------------------------------------------------------- */
	/*      Build coordsys.                                                 */
	/* -------------------------------------------------------------------- */
	clear();

	switch (nProjId)
	{
	case 42001: // Auto UTM
		setUTM((int)floor((dfRefLong + 180.0) / 6.0) + 1,
			dfRefLat >= 0.0);
		break;

	case 42002: // Auto TM (strangely very UTM-like).
		setTM(0, dfRefLong, 0.9996,
			500000.0, (dfRefLat >= 0.0) ? 0.0 : 10000000.0);
		break;

	case 42003: // Auto Orthographic.
		setOrthographic(dfRefLat, dfRefLong, 0.0, 0.0);
		break;

	case 42004: // Auto Equirectangular
		setEquirectangular(dfRefLat, dfRefLong, 0.0, 0.0);
		break;

	case 42005:
		setMollweide(dfRefLong, 0.0, 0.0);
		break;

	default:
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Set units.                                                      */
	/* -------------------------------------------------------------------- */

	switch (nUnitsId)
	{
	case 9001:
		setLinearUnits(SRS_UL_METER, 1.0);
		break;

	case 9002:
		setLinearUnits("Foot", 0.3048);
		break;

	case 9003:
		setLinearUnits("US survey foot", cslStringToNumber(SRS_UL_US_FOOT_CONV));
		break;

	default:
		return false;
	}

	setAuthority("PROJCS|UNIT", "EPSG", nUnitsId);

	/* -------------------------------------------------------------------- */
	/*      Set WGS84.                                                      */
	/* -------------------------------------------------------------------- */
	setWellKnownGeogCS("WGS84");

	return true;
}

/************************************************************************/
/*                            getSemiMajor()                            */
/************************************************************************/

/**
* \brief Get spheroid semi major axis.
*
* This method does the same thing as the C function OSRGetSemiMajor().
*
* @param pnErr if non-NULL set to false if semi major axis
* can be found.
*
* @return semi-major axis, or SRS_WGS84_SEMIMAJOR if it can't be found.
*/

double SpatialReference::getSemiMajor(bool * pnErr) const

{
	const SRSNode *poSpheroid = getAttrNode("SPHEROID");

	if (pnErr != NULL)
		*pnErr = true;

	if (poSpheroid != NULL && poSpheroid->getChildCount() >= 3)
	{
		return cslStringToNumber(poSpheroid->getChild(1)->getValue());
	}
	else
	{
		if (pnErr != NULL)
			*pnErr = false;

		return SRS_WGS84_SEMIMAJOR;
	}
}
 
/************************************************************************/
/*                          getInvFlattening()                          */
/************************************************************************/

/**
* \brief Get spheroid inverse flattening.
*
* This method does the same thing as the C function OSRGetInvFlattening().
*
* @param pnErr if non-NULL set to false if no inverse flattening
* can be found.
*
* @return inverse flattening, or SRS_WGS84_INVFLATTENING if it can't be found.
*/

double SpatialReference::getInvFlattening(bool * pnErr) const

{
	const SRSNode *poSpheroid = getAttrNode("SPHEROID");

	if (pnErr != NULL)
		*pnErr = true;

	if (poSpheroid != NULL && poSpheroid->getChildCount() >= 3)
	{
		return cslStringToNumber(poSpheroid->getChild(2)->getValue());
	}
	else
	{
		if (pnErr != NULL)
			*pnErr = false;

		return SRS_WGS84_INVFLATTENING;
	}
}
 
/************************************************************************/
/*                            getSemiMinor()                            */
/************************************************************************/

/**
* \brief Get spheroid semi minor axis.
*
* This method does the same thing as the C function OSRGetSemiMinor().
*
* @param pnErr if non-NULL set to false if semi minor axis
* can be found.
*
* @return semi-minor axis, or WGS84 semi minor if it can't be found.
*/

double SpatialReference::getSemiMinor(bool * pnErr) const

{
	double      dfInvFlattening, dfSemiMajor;

	dfSemiMajor = getSemiMajor(pnErr);
	dfInvFlattening = getInvFlattening(pnErr);

	if (fabs(dfInvFlattening) < 0.000000000001)
		return dfSemiMajor;
	else
		return dfSemiMajor * (1.0 - 1.0 / dfInvFlattening);
}


/************************************************************************/
/*                             setLocalCS()                             */
/************************************************************************/

/**
* \brief Set the user visible LOCAL_CS name.
*
* This method is the same as the C function OSRSetLocalCS().
*
* This method will ensure a LOCAL_CS node is created as the root,
* and set the provided name on it.  It must be used before setLinearUnits().
*
* @param pszName the user visible name to assign.  Not used as a key.
*
* @return true on success.
*/

bool SpatialReference::setLocalCS(const char * pszName)

{
	SRSNode *poCS = getAttrNode("LOCAL_CS");

	if (poCS == NULL && getRoot() != NULL)
	{
		return false;
	}
	else
	{
		setNode("LOCAL_CS", pszName);
		return true;
	}
}

/************************************************************************/
/*                             setGeocCS()                              */
/************************************************************************/

/**
* \brief Set the user visible GEOCCS name.
*
* This method is the same as the C function OSRSetGeocCS().

* This method will ensure a GEOCCS node is created as the root,
* and set the provided name on it.  If used on a GEOGCS coordinate system,
* the DATUM and PRIMEM nodes from the GEOGCS will be tarnsferred over to
* the GEOGCS.
*
* @param pszName the user visible name to assign.  Not used as a key.
*
* @return true on success.
*
* @since OGR 1.9.0
*/

bool SpatialReference::setGeocCS(const char * pszName)

{
	SRSNode *poGeogCS = NULL;
	SRSNode *poGeocCS = getAttrNode("GEOCCS");

	if (poRoot != NULL && cslIEqualString(poRoot->getValue(), "GEOGCS"))
	{
		poGeogCS = poRoot;
		poRoot = NULL;
	}

	if (poGeocCS == NULL && getRoot() != NULL)
	{
		return false;
	}

	setNode("GEOCCS", pszName);

	if (poGeogCS != NULL)
	{
		SRSNode *poDatum = poGeogCS->getNode("DATUM");
		SRSNode *poPRIMEM = poGeogCS->getNode("PRIMEM");
		if (poDatum != NULL && poPRIMEM != NULL)
		{
			poRoot->insertChild(poDatum->clone(), 1);
			poRoot->insertChild(poPRIMEM->clone(), 2);
		}
		delete poGeogCS;
	}

	return true;
} 

/************************************************************************/
/*                             setVertCS()                              */
/************************************************************************/

/**
* \brief Set the user visible VERT_CS name.
*
* This method is the same as the C function OSRSetVertCS().

* This method will ensure a VERT_CS node is created if needed.  If the
* existing coordinate system is GEOGCS or PROJCS rooted, then it will be
* turned into a COMPD_CS.
*
* @param pszVertCSName the user visible name of the vertical coordinate
* system. Not used as a key.
*
* @param pszVertDatumName the user visible name of the vertical datum.  It
* is helpful if this matches the EPSG name.
*
* @param nVertDatumType the OGC vertical datum type, usually 2005.
*
* @return true on success.
*
* @since OGR 1.9.0
*/

bool SpatialReference::setVertCS(const char * pszVertCSName,
	const char * pszVertDatumName,
	int nVertDatumType)

{
	/* -------------------------------------------------------------------- */
	/*      Handle the case where we want to make a compound coordinate     */
	/*      system.                                                         */
	/* -------------------------------------------------------------------- */
	if (isProjected() || isGeographic())
	{
		SRSNode *poNewRoot = new SRSNode("COMPD_CS");
		poNewRoot->addChild(poRoot);
		poRoot = poNewRoot;
	}

	else if (getAttrNode("VERT_CS") == NULL)
		clear();

	/* -------------------------------------------------------------------- */
	/*      If we already have a VERT_CS, wipe and recreate the root        */
	/*      otherwise create the VERT_CS now.                               */
	/* -------------------------------------------------------------------- */
	SRSNode *poVertCS = getAttrNode("VERT_CS");

	if (poVertCS != NULL)
	{
		poVertCS->clearChildren();
	}
	else
	{
		poVertCS = new SRSNode("VERT_CS");
		if (poRoot != NULL && cslIEqualString(poRoot->getValue(), "COMPD_CS"))
		{
			poRoot->addChild(poVertCS);
		}
		else
			setRoot(poVertCS);
	}

	/* -------------------------------------------------------------------- */
	/*      Set the name, datumname, and type.                              */
	/* -------------------------------------------------------------------- */
	SRSNode *poVertDatum;

	poVertCS->addChild(new SRSNode(pszVertCSName));

	poVertDatum = new SRSNode("VERT_DATUM");
	poVertCS->addChild(poVertDatum);

	poVertDatum->addChild(new SRSNode(pszVertDatumName));

	String osVertDatumType;
	osVertDatumType.printf("%d", nVertDatumType);
	poVertDatum->addChild(new SRSNode(osVertDatumType));

	// add default axis node.
	SRSNode *poAxis = new SRSNode("AXIS");

	poAxis->addChild(new SRSNode("Up"));
	poAxis->addChild(new SRSNode("UP"));

	poVertCS->addChild(poAxis);

	return true;
}
 
/************************************************************************/
/*                           setCompoundCS()                            */
/************************************************************************/

/**
* \brief Setup a compound coordinate system.
*
* This method is the same as the C function OSRSetCompoundCS().

* This method is replace the current SRS with a COMPD_CS coordinate system
* consisting of the passed in horizontal and vertical coordinate systems.
*
* @param pszName the name of the compound coordinate system.
*
* @param poHorizSRS the horizontal SRS (PROJCS or GEOGCS).
*
* @param poVertSRS the vertical SRS (VERT_CS).
*
* @return true on success.
*/

bool
SpatialReference::setCompoundCS(const char *pszName,
	const SpatialReference *poHorizSRS,
	const SpatialReference *poVertSRS)

{
	/* -------------------------------------------------------------------- */
	/*      Verify these are legal horizontal and vertical coordinate       */
	/*      systems.                                                        */
	/* -------------------------------------------------------------------- */
	if (!poVertSRS->isVertical())
	{
		return false;
	}
	if (!poHorizSRS->isProjected()
		&& !poHorizSRS->isGeographic())
	{
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Replace with compound srs.                                      */
	/* -------------------------------------------------------------------- */
	clear();

	poRoot = new SRSNode("COMPD_CS");
	poRoot->addChild(new SRSNode(pszName));
	poRoot->addChild(poHorizSRS->getRoot()->clone());
	poRoot->addChild(poVertSRS->getRoot()->clone());

	return true;
}

/************************************************************************/
/*                             setProjCS()                              */
/************************************************************************/

/**
* \brief Set the user visible PROJCS name.
*
* This method is the same as the C function OSRSetProjCS().
*
* This method will ensure a PROJCS node is created as the root,
* and set the provided name on it.  If used on a GEOGCS coordinate system,
* the GEOGCS node will be demoted to be a child of the new PROJCS root.
*
* @param pszName the user visible name to assign.  Not used as a key.
*
* @return true on success.
*/

bool SpatialReference::setProjCS(const char * pszName)

{
	SRSNode *poGeogCS = NULL;
	SRSNode *poProjCS = getAttrNode("PROJCS");

	if (poRoot != NULL && cslIEqualString(poRoot->getValue(), "GEOGCS"))
	{
		poGeogCS = poRoot;
		poRoot = NULL;
	}

	if (poProjCS == NULL && getRoot() != NULL)
	{
		return false;
	}

	setNode("PROJCS", pszName);

	if (poGeogCS != NULL)
		poRoot->insertChild(poGeogCS, 1);

	return true;
}

/************************************************************************/
/*                           setProjection()                            */
/************************************************************************/

/**
* \brief Set a projection name.
*
* This method is the same as the C function OSRSetProjection().
*
* @param pszProjection the projection name, which should be selected from
* the macros in ogr_srs_api.h, such as SRS_PT_TRANSVERSE_MERCATOR.
*
* @return true on success.
*/

bool SpatialReference::setProjection(const char * pszProjection)

{
	SRSNode *poGeogCS = NULL;
	bool eErr;

	if (poRoot != NULL && cslIEqualString(poRoot->getValue(), "GEOGCS"))
	{
		poGeogCS = poRoot;
		poRoot = NULL;
	}

	if (!getAttrNode("PROJCS"))
	{
		setNode("PROJCS", "unnamed");
	}

	eErr = setNode("PROJCS|PROJECTION", pszProjection);
	if (eErr != true)
		return eErr;

	if (poGeogCS != NULL)
		poRoot->insertChild(poGeogCS, 1);

	return true;
}
 
/************************************************************************/
/*                            setProjParm()                             */
/************************************************************************/

/**
* \brief Set a projection parameter value.
*
* Adds a new PARAMETER under the PROJCS with the indicated name and value.
*
* This method is the same as the C function OSRSetProjParm().
*
* Please check http://www.remotesensing.org/geotiff/proj_list pages for
* legal parameter names for specific projections.
*
*
* @param pszParmName the parameter name, which should be selected from
* the macros in ogr_srs_api.h, such as SRS_PP_CENTRAL_MERIDIAN.
*
* @param dfValue value to assign.
*
* @return true on success.
*/

bool SpatialReference::setProjParm(const char * pszParmName,
	double dfValue)

{
	SRSNode *poPROJCS = getAttrNode("PROJCS");
	SRSNode *poParm;
	char        szValue[64];

	if (poPROJCS == NULL)
		return false;

	printDouble(szValue, dfValue);

	/* -------------------------------------------------------------------- */
	/*      Try to find existing parameter with this name.                  */
	/* -------------------------------------------------------------------- */
	for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
	{
		poParm = poPROJCS->getChild(iChild);

		if (cslIEqualString(poParm->getValue(), "PARAMETER")
			&& poParm->getChildCount() == 2
			&& cslIEqualString(poParm->getChild(0)->getValue(), pszParmName))
		{
			poParm->getChild(1)->setValue(szValue);
			return true;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Otherwise create a new parameter and append.                    */
	/* -------------------------------------------------------------------- */
	poParm = new SRSNode("PARAMETER");
	poParm->addChild(new SRSNode(pszParmName));
	poParm->addChild(new SRSNode(szValue));

	poPROJCS->addChild(poParm);

	return true;
}

/************************************************************************/
/*                            findProjParm()                            */
/************************************************************************/

/**
* \brief Return the child index of the named projection parameter on
* its parent PROJCS node.
*
* @param pszParameter projection parameter to look for
* @param poPROJCS projection CS node to look in. If NULL is passed,
*        the PROJCS node of the SpatialReference object will be searched.
*
* @return the child index of the named projection parameter. -1 on failure
*/
int SpatialReference::findProjParm(const char *pszParameter,
	const SRSNode *poPROJCS) const

{
	const SRSNode *poParameter = NULL;

	if (poPROJCS == NULL)
		poPROJCS = getAttrNode("PROJCS");

	if (poPROJCS == NULL)
		return -1;

	/* -------------------------------------------------------------------- */
	/*      Search for requested parameter.                                 */
	/* -------------------------------------------------------------------- */
	int iChild;

	for (iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
	{
		poParameter = poPROJCS->getChild(iChild);

		if (cslIEqualString(poParameter->getValue(), "PARAMETER")
			&& poParameter->getChildCount() == 2
			&& cslIEqualString(poPROJCS->getChild(iChild)->getChild(0)->getValue(),
				pszParameter))
		{
			return iChild;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Try similar names, for selected parameters.                     */
	/* -------------------------------------------------------------------- */
	iChild = -1;

	if (cslIEqualString(pszParameter, SRS_PP_LATITUDE_OF_ORIGIN))
	{
		iChild = findProjParm(SRS_PP_LATITUDE_OF_CENTER, poPROJCS);
	}
	else if (cslIEqualString(pszParameter, SRS_PP_CENTRAL_MERIDIAN))
	{
		iChild = findProjParm(SRS_PP_LONGITUDE_OF_CENTER, poPROJCS);
		if (iChild == -1)
			iChild = findProjParm(SRS_PP_LONGITUDE_OF_ORIGIN, poPROJCS);
	}

	return iChild;
}

/************************************************************************/
/*                            getProjParm()                             */
/************************************************************************/

/**
* \brief Fetch a projection parameter value.
*
* NOTE: This code should be modified to translate non degree angles into
* degrees based on the GEOGCS unit.  This has not yet been done.
*
* This method is the same as the C function OSRGetProjParm().
*
* @param pszName the name of the parameter to fetch, from the set of
* SRS_PP codes in ogr_srs_api.h.
*
* @param dfDefaultValue the value to return if this parameter doesn't exist.
*
* @param pnErr place to put error code on failure.  Ignored if NULL.
*
* @return value of parameter.
*/

double SpatialReference::getProjParm(const char * pszName,
	double dfDefaultValue,
	bool *pnErr) const

{
	const SRSNode *poPROJCS = getAttrNode("PROJCS");

	if (pnErr != NULL)
		*pnErr = true;

	/* -------------------------------------------------------------------- */
	/*      Find the desired parameter.                                     */
	/* -------------------------------------------------------------------- */
	int iChild = findProjParm(pszName, poPROJCS);

	if (iChild != -1)
	{
		const SRSNode *poParameter = NULL;
		poParameter = poPROJCS->getChild(iChild);
		return cslStringToNumber(poParameter->getChild(1)->getValue());
	}

	/* -------------------------------------------------------------------- */
	/*      Return default value on failure.                                */
	/* -------------------------------------------------------------------- */
	if (pnErr != NULL)
		*pnErr = false;

	return dfDefaultValue;
}

/************************************************************************/
/*                          getNormProjParm()                           */
/************************************************************************/

/**
* \brief Fetch a normalized projection parameter value.
*
* This method is the same as getProjParm() except that the value of
* the parameter is "normalized" into degrees or meters depending on
* whether it is linear or angular.
*
* This method is the same as the C function OSRGetNormProjParm().
*
* @param pszName the name of the parameter to fetch, from the set of
* SRS_PP codes in ogr_srs_api.h.
*
* @param dfDefaultValue the value to return if this parameter doesn't exist.
*
* @param pnErr place to put error code on failure.  Ignored if NULL.
*
* @return value of parameter.
*/

double SpatialReference::getNormProjParm(const char * pszName,
	double dfDefaultValue,
	bool *pnErr) const

{
	double dfRawResult;
	bool nError;

	if (pnErr == NULL)
		pnErr = &nError;

	getNormInfo();

	dfRawResult = getProjParm(pszName, dfDefaultValue, pnErr);

	// If we got the default just return it unadjusted.
	if (*pnErr != true)
		return dfRawResult;

	if (dfToDegrees != 1.0 && isAngularParameter(pszName))
		dfRawResult *= dfToDegrees;

	if (dfToMeter != 1.0 && isLinearParameter(pszName))
		return dfRawResult * dfToMeter;
#ifdef WKT_LONGITUDE_RELATIVE_TO_PM
	else if (dfFromGreenwich != 0.0 && isLongitudeParameter(pszName))
		return dfRawResult + dfFromGreenwich;
#endif
	else
		return dfRawResult;
}

/************************************************************************/
/*                          setNormProjParm()                           */
/************************************************************************/

/**
* \brief Set a projection parameter with a normalized value.
*
* This method is the same as setProjParm() except that the value of
* the parameter passed in is assumed to be in "normalized" form (decimal
* degrees for angular values, meters for linear values.  The values are
* converted in a form suitable for the GEOGCS and linear units in effect.
*
* This method is the same as the C function OSRSetNormProjParm().
*
* @param pszName the parameter name, which should be selected from
* the macros in ogr_srs_api.h, such as SRS_PP_CENTRAL_MERIDIAN.
*
* @param dfValue value to assign.
*
* @return true on success.
*/

bool SpatialReference::setNormProjParm(const char * pszName,
	double dfValue)

{
	getNormInfo();

	if ((dfToDegrees != 1.0 || dfFromGreenwich != 0.0)
		&& isAngularParameter(pszName))
	{
#ifdef WKT_LONGITUDE_RELATIVE_TO_PM
		if (dfFromGreenwich != 0.0 && isLongitudeParameter(pszName))
			dfValue -= dfFromGreenwich;
#endif

		dfValue /= dfToDegrees;
	}
	else if (dfToMeter != 1.0 && isLinearParameter(pszName))
		dfValue /= dfToMeter;

	return setProjParm(pszName, dfValue);
}

/************************************************************************/
/*                               setTM()                                */
/************************************************************************/

bool SpatialReference::setTM(double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_TRANSVERSE_MERCATOR);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                            setTMVariant()                            */
/************************************************************************/

bool SpatialReference::setTMVariant(
	const char *pszVariantName,
	double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(pszVariantName);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setTMSO()                               */
/************************************************************************/

bool SpatialReference::setTMSO(double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_TRANSVERSE_MERCATOR_SOUTH_ORIENTED);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setTMSO()                               */
/************************************************************************/

bool SpatialReference::setTPED(double dfLat1, double dfLong1,
	double dfLat2, double dfLong2,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_TWO_POINT_EQUIDISTANT);
	setNormProjParm(SRS_PP_LATITUDE_OF_1ST_POINT, dfLat1);
	setNormProjParm(SRS_PP_LONGITUDE_OF_1ST_POINT, dfLong1);
	setNormProjParm(SRS_PP_LATITUDE_OF_2ND_POINT, dfLat2);
	setNormProjParm(SRS_PP_LONGITUDE_OF_2ND_POINT, dfLong2);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                               setTMG()                               */
/************************************************************************/

bool
SpatialReference::setTMG(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting, double dfFalseNorthing)

{
	setProjection(SRS_PT_TUNISIA_MINING_GRID);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setACEA()                               */
/************************************************************************/

bool SpatialReference::setACEA(double dfStdP1, double dfStdP2,
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_ALBERS_CONIC_EQUAL_AREA);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_2, dfStdP2);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                               setAE()                                */
/************************************************************************/

bool SpatialReference::setAE(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_AZIMUTHAL_EQUIDISTANT);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setBonne()                              */
/************************************************************************/

bool SpatialReference::setBonne(
	double dfStdP1, double dfCentralMeridian,
	double dfFalseEasting, double dfFalseNorthing)

{
	setProjection(SRS_PT_BONNE);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                               setCEA()                               */
/************************************************************************/

bool SpatialReference::setCEA(double dfStdP1, double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_CYLINDRICAL_EQUAL_AREA);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                               setCS()                                */
/************************************************************************/

bool SpatialReference::setCS(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_CASSINI_SOLDNER);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setEC()                                */
/************************************************************************/

bool SpatialReference::setEC(double dfStdP1, double dfStdP2,
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_EQUIDISTANT_CONIC);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_2, dfStdP2);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                             setEckert()                              */
/************************************************************************/

bool SpatialReference::setEckert(int nVariation /* 1-6 */,
	double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	if (nVariation == 1)
		setProjection(SRS_PT_ECKERT_I);
	else if (nVariation == 2)
		setProjection(SRS_PT_ECKERT_II);
	else if (nVariation == 3)
		setProjection(SRS_PT_ECKERT_III);
	else if (nVariation == 4)
		setProjection(SRS_PT_ECKERT_IV);
	else if (nVariation == 5)
		setProjection(SRS_PT_ECKERT_V);
	else if (nVariation == 6)
		setProjection(SRS_PT_ECKERT_VI);
	else
	{
		return false;
	}

	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setEckertIV()                             */
/*                                                                      */
/*      Deprecated                                                      */
/************************************************************************/

bool SpatialReference::setEckertIV(double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_ECKERT_IV);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setEckertVI()                             */
/*                                                                      */
/*      Deprecated                                                      */
/************************************************************************/

bool SpatialReference::setEckertVI(double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_ECKERT_VI);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                         setEquirectangular()                         */
/************************************************************************/

bool SpatialReference::setEquirectangular(
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_EQUIRECTANGULAR);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                         setEquirectangular2()                        */
/* Generalized form                                                     */
/************************************************************************/

bool SpatialReference::setEquirectangular2(
	double dfCenterLat, double dfCenterLong,
	double dfStdParallel1,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_EQUIRECTANGULAR);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdParallel1);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setGS()                                */
/************************************************************************/

bool SpatialReference::setGS(double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_GALL_STEREOGRAPHIC);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setGH()                                */
/************************************************************************/

bool SpatialReference::setGH(double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_GOODE_HOMOLOSINE);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                              setIGH()                                */
/************************************************************************/

bool SpatialReference::setIGH()

{
	setProjection(SRS_PT_IGH);

	return true;
}
 
/************************************************************************/
/*                              setGEOS()                               */
/************************************************************************/

bool SpatialReference::setGEOS(double dfCentralMeridian,
	double dfSatelliteHeight,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_GEOSTATIONARY_SATELLITE);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_SATELLITE_HEIGHT, dfSatelliteHeight);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                       setGaussSchreiberTMercator()                   */
/************************************************************************/

bool SpatialReference::setGaussSchreiberTMercator(
	double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_GAUSSSCHREIBERTMERCATOR);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                            setGnomonic()                             */
/************************************************************************/

bool SpatialReference::setGnomonic(
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_GNOMONIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setHOMAC()                              */
/************************************************************************/

/**
* \brief Set an Hotine Oblique Mercator Azimuth Center projection using
* azimuth angle.
*
* This projection corresponds to EPSG projection method 9815, also
* sometimes known as hotine oblique mercator (variant B).
*
* This method does the same thing as the C function OSRSetHOMAC().
*
* @param dfCenterLat Latitude of the projection origin.
* @param dfCenterLong Longitude of the projection origin.
* @param dfAzimuth Azimuth, measured clockwise from North, of the projection
* centerline.
* @param dfRectToSkew ?.
* @param dfScale Scale factor applies to the projection origin.
* @param dfFalseEasting False easting.
* @param dfFalseNorthing False northing.
*
* @return true on success.
*/

bool SpatialReference::setHOMAC(double dfCenterLat, double dfCenterLong,
	double dfAzimuth, double dfRectToSkew,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_HOTINE_OBLIQUE_MERCATOR_AZIMUTH_CENTER);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_AZIMUTH, dfAzimuth);
	setNormProjParm(SRS_PP_RECTIFIED_GRID_ANGLE, dfRectToSkew);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setHOM()                               */
/************************************************************************/

/**
* \brief Set a Hotine Oblique Mercator projection using azimuth angle.
*
* This projection corresponds to EPSG projection method 9812, also
* sometimes known as hotine oblique mercator (variant A)..
*
* This method does the same thing as the C function OSRSetHOM().
*
* @param dfCenterLat Latitude of the projection origin.
* @param dfCenterLong Longitude of the projection origin.
* @param dfAzimuth Azimuth, measured clockwise from North, of the projection
* centerline.
* @param dfRectToSkew ?.
* @param dfScale Scale factor applies to the projection origin.
* @param dfFalseEasting False easting.
* @param dfFalseNorthing False northing.
*
* @return true on success.
*/

bool SpatialReference::setHOM(double dfCenterLat, double dfCenterLong,
	double dfAzimuth, double dfRectToSkew,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_HOTINE_OBLIQUE_MERCATOR);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_AZIMUTH, dfAzimuth);
	setNormProjParm(SRS_PP_RECTIFIED_GRID_ANGLE, dfRectToSkew);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                             setHOM2PNO()                             */
/************************************************************************/

/**
* \brief Set a Hotine Oblique Mercator projection using two points on projection
* centerline.
*
* This method does the same thing as the C function OSRSetHOM2PNO().
*
* @param dfCenterLat Latitude of the projection origin.
* @param dfLat1 Latitude of the first point on center line.
* @param dfLong1 Longitude of the first point on center line.
* @param dfLat2 Latitude of the second point on center line.
* @param dfLong2 Longitude of the second point on center line.
* @param dfScale Scale factor applies to the projection origin.
* @param dfFalseEasting False easting.
* @param dfFalseNorthing False northing.
*
* @return true on success.
*/

bool SpatialReference::setHOM2PNO(double dfCenterLat,
	double dfLat1, double dfLong1,
	double dfLat2, double dfLong2,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_HOTINE_OBLIQUE_MERCATOR_TWO_POINT_NATURAL_ORIGIN);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LATITUDE_OF_POINT_1, dfLat1);
	setNormProjParm(SRS_PP_LONGITUDE_OF_POINT_1, dfLong1);
	setNormProjParm(SRS_PP_LATITUDE_OF_POINT_2, dfLat2);
	setNormProjParm(SRS_PP_LONGITUDE_OF_POINT_2, dfLong2);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                            setIWMPolyconic()                         */
/************************************************************************/

bool SpatialReference::setIWMPolyconic(
	double dfLat1, double dfLat2,
	double dfCenterLong,
	double dfFalseEasting, double dfFalseNorthing)

{
	setProjection(SRS_PT_IMW_POLYCONIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_1ST_POINT, dfLat1);
	setNormProjParm(SRS_PP_LATITUDE_OF_2ND_POINT, dfLat2);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                             setKrovak()                              */
/************************************************************************/

bool SpatialReference::setKrovak(double dfCenterLat, double dfCenterLong,
	double dfAzimuth,
	double dfPseudoStdParallel1,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_KROVAK);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_AZIMUTH, dfAzimuth);
	setNormProjParm(SRS_PP_PSEUDO_STD_PARALLEL_1, dfPseudoStdParallel1);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                              setLAEA()                               */
/************************************************************************/

bool SpatialReference::setLAEA(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_LAMBERT_AZIMUTHAL_EQUAL_AREA);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setLCC()                               */
/************************************************************************/

bool SpatialReference::setLCC(double dfStdP1, double dfStdP2,
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_2, dfStdP2);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                             setLCC1SP()                              */
/************************************************************************/

bool SpatialReference::setLCC1SP(double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_LAMBERT_CONFORMAL_CONIC_1SP);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                              setLCCB()                               */
/************************************************************************/

bool SpatialReference::setLCCB(double dfStdP1, double dfStdP2,
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_LAMBERT_CONFORMAL_CONIC_2SP_BELGIUM);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	setNormProjParm(SRS_PP_STANDARD_PARALLEL_2, dfStdP2);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                               setMC()                                */
/************************************************************************/

bool SpatialReference::setMC(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_MILLER_CYLINDRICAL);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfCenterLat);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setMercator()                             */
/************************************************************************/

bool SpatialReference::setMercator(double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_MERCATOR_1SP);

	if (dfCenterLat != 0.0)
		setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);

	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                           setMercator2SP()                           */
/************************************************************************/

bool SpatialReference::setMercator2SP(
	double dfStdP1,
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_MERCATOR_2SP);

	setNormProjParm(SRS_PP_STANDARD_PARALLEL_1, dfStdP1);
	if (dfCenterLat != 0.0)
		setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);

	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setMollweide()                            */
/************************************************************************/

bool SpatialReference::setMollweide(double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_MOLLWEIDE);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                              setNZMG()                               */
/************************************************************************/

bool SpatialReference::setNZMG(double dfCenterLat, double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_NEW_ZEALAND_MAP_GRID);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setOS()                                */
/************************************************************************/

bool SpatialReference::setOS(double dfOriginLat, double dfCMeridian,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_OBLIQUE_STEREOGRAPHIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfOriginLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCMeridian);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                          setOrthographic()                           */
/************************************************************************/

bool SpatialReference::setOrthographic(
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting, double dfFalseNorthing)

{
	setProjection(SRS_PT_ORTHOGRAPHIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setPolyconic()                            */
/************************************************************************/

bool SpatialReference::setPolyconic(
	double dfCenterLat, double dfCenterLong,
	double dfFalseEasting, double dfFalseNorthing)

{
	// note: it seems that by some definitions this should include a
	//       scale_factor parameter.

	setProjection(SRS_PT_POLYCONIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                               setPS()                                */
/************************************************************************/

bool SpatialReference::setPS(
	double dfCenterLat, double dfCenterLong,
	double dfScale,
	double dfFalseEasting, double dfFalseNorthing)

{
	setProjection(SRS_PT_POLAR_STEREOGRAPHIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCenterLong);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                            setRobinson()                             */
/************************************************************************/

bool SpatialReference::setRobinson(double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_ROBINSON);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
} 
/************************************************************************/
/*                           setSinusoidal()                            */
/************************************************************************/

bool SpatialReference::setSinusoidal(double dfCenterLong,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_SINUSOIDAL);
	setNormProjParm(SRS_PP_LONGITUDE_OF_CENTER, dfCenterLong);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}

/************************************************************************/
/*                          setStereographic()                          */
/************************************************************************/

bool SpatialReference::setStereographic(
	double dfOriginLat, double dfCMeridian,
	double dfScale,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_STEREOGRAPHIC);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfOriginLat);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCMeridian);
	setNormProjParm(SRS_PP_SCALE_FACTOR, dfScale);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setSOC()                               */
/*                                                                      */
/*      NOTE: This definition isn't really used in practice any more    */
/*      and should be considered deprecated.  It seems that swiss       */
/*      oblique mercator is now define as Hotine_Oblique_Mercator       */
/*      with an azimuth of 90 and a rectified_grid_angle of 90.  See    */
/*      EPSG:2056 and Bug 423.                                          */
/************************************************************************/

bool SpatialReference::setSOC(double dfLatitudeOfOrigin,
	double dfCentralMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_SWISS_OBLIQUE_CYLINDRICAL);
	setNormProjParm(SRS_PP_LATITUDE_OF_CENTER, dfLatitudeOfOrigin);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCentralMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setVDG()                               */
/************************************************************************/

bool SpatialReference::setVDG(double dfCMeridian,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	setProjection(SRS_PT_VANDERGRINTEN);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, dfCMeridian);
	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                               setUTM()                               */
/************************************************************************/

/**
* \brief Set UTM projection definition.
*
* This will generate a projection definition with the full set of
* transverse mercator projection parameters for the given UTM zone.
* If no PROJCS[] description is set yet, one will be set to look
* like "UTM Zone %d, {Northern, Southern} Hemisphere".
*
* This method is the same as the C function OSRSetUTM().
*
* @param nZone UTM zone.
*
* @param bNorth TRUE for northern hemisphere, or FALSE for southern
* hemisphere.
*
* @return true on success.
*/

bool SpatialReference::setUTM(int nZone, int bNorth)

{
	setProjection(SRS_PT_TRANSVERSE_MERCATOR);
	setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0);
	setNormProjParm(SRS_PP_CENTRAL_MERIDIAN, nZone * 6 - 183);
	setNormProjParm(SRS_PP_SCALE_FACTOR, 0.9996);
	setNormProjParm(SRS_PP_FALSE_EASTING, 500000.0);

	if (bNorth)
		setNormProjParm(SRS_PP_FALSE_NORTHING, 0);
	else
		setNormProjParm(SRS_PP_FALSE_NORTHING, 10000000);

	if (cslIEqualString(getAttrValue("PROJCS"), "unnamed"))
	{
		char    szUTMName[128];

		if (bNorth)
			sprintf(szUTMName, "UTM Zone %d, Northern Hemisphere", nZone);
		else
			sprintf(szUTMName, "UTM Zone %d, Southern Hemisphere", nZone);

		setNode("PROJCS", szUTMName);
	}

	setLinearUnits(SRS_UL_METER, 1.0);

	return true;
}
 
/************************************************************************/
/*                             getUTMZone()                             */
/*                                                                      */
/*      Returns zero if it isn't UTM.                                   */
/************************************************************************/

/**
* \brief Get utm zone information.
*
* This is the same as the C function OSRGetUTMZone().
*
* In SWIG bindings (Python, Java, etc) the getUTMZone() method returns a
* zone which is negative in the southern hemisphere instead of having the
* pbNorth flag used in the C and C++ interface.
*
* @param pbNorth pointer to in to set to TRUE if northern hemisphere, or
* FALSE if southern.
*
* @return UTM zone number or zero if this isn't a UTM definition.
*/

int SpatialReference::getUTMZone (int * pbNorth) const{
	const char  *pszProjection = getAttrValue("PROJECTION");

	if (pszProjection == NULL
		|| !cslIEqualString(pszProjection, SRS_PT_TRANSVERSE_MERCATOR))
		return 0;

	if (getNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, 0.0) != 0.0)
		return 0;

	if (getProjParm(SRS_PP_SCALE_FACTOR, 1.0) != 0.9996)
		return 0;

	if (fabs(getNormProjParm(SRS_PP_FALSE_EASTING, 0.0) - 500000.0) > 0.001)
		return 0;

	double      dfFalseNorthing = getNormProjParm(SRS_PP_FALSE_NORTHING, 0.0);

	if (dfFalseNorthing != 0.0
		&& fabs(dfFalseNorthing - 10000000.0) > 0.001)
		return 0;

	if (pbNorth != NULL)
		*pbNorth = (dfFalseNorthing == 0);

	double      dfCentralMeridian = getNormProjParm(SRS_PP_CENTRAL_MERIDIAN,
		0.0);
	double      dfZone = (dfCentralMeridian + 186.0) / 6.0;

	if (fabs(dfZone - (int)dfZone - 0.5) > 0.00001
		|| dfCentralMeridian < -177.00001
		|| dfCentralMeridian > 177.000001)
		return 0;
	else
		return (int)dfZone;
}
 
/************************************************************************/
/*                             setWagner()                              */
/************************************************************************/

bool SpatialReference::setWagner(int nVariation /* 1 -- 7 */,
	double dfCenterLat,
	double dfFalseEasting,
	double dfFalseNorthing)

{
	if (nVariation == 1)
		setProjection(SRS_PT_WAGNER_I);
	else if (nVariation == 2)
		setProjection(SRS_PT_WAGNER_II);
	else if (nVariation == 3)
	{
		setProjection(SRS_PT_WAGNER_III);
		setNormProjParm(SRS_PP_LATITUDE_OF_ORIGIN, dfCenterLat);
	}
	else if (nVariation == 4)
		setProjection(SRS_PT_WAGNER_IV);
	else if (nVariation == 5)
		setProjection(SRS_PT_WAGNER_V);
	else if (nVariation == 6)
		setProjection(SRS_PT_WAGNER_VI);
	else if (nVariation == 7)
		setProjection(SRS_PT_WAGNER_VII);
	else
	{
		return false;
	}

	setNormProjParm(SRS_PP_FALSE_EASTING, dfFalseEasting);
	setNormProjParm(SRS_PP_FALSE_NORTHING, dfFalseNorthing);

	return true;
}
 
/************************************************************************/
/*                            setAuthority()                            */
/************************************************************************/

/**
* \brief Set the authority for a node.
*
* This method is the same as the C function OSRSetAuthority().
*
* @param pszTargetKey the partial or complete path to the node to
* set an authority on.  ie. "PROJCS", "GEOGCS" or "GEOGCS|UNIT".
*
* @param pszAuthority authority name, such as "EPSG".
*
* @param nCode code for value with this authority.
*
* @return true on success.
*/

bool SpatialReference::setAuthority(const char *pszTargetKey,
	const char * pszAuthority,
	int nCode)

{
	/* -------------------------------------------------------------------- */
	/*      Find the node below which the authority should be put.          */
	/* -------------------------------------------------------------------- */
	SRSNode  *poNode = getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      If there is an existing AUTHORITY child blow it away before     */
	/*      trying to set a new one.                                        */
	/* -------------------------------------------------------------------- */
	int iOldChild = poNode->findChild("AUTHORITY");
	if (iOldChild != -1)
		poNode->destroyChild(iOldChild);

	/* -------------------------------------------------------------------- */
	/*      Create a new authority node.                                    */
	/* -------------------------------------------------------------------- */
	char   szCode[32];
	SRSNode *poAuthNode;

	sprintf(szCode, "%d", nCode);

	poAuthNode = new SRSNode("AUTHORITY");
	poAuthNode->addChild(new SRSNode(pszAuthority));
	poAuthNode->addChild(new SRSNode(szCode));

	poNode->addChild(poAuthNode);

	return true;
} 
/************************************************************************/
/*                          getAuthorityCode()                          */
/************************************************************************/

/**
* \brief Get the authority code for a node.
*
* This method is used to query an AUTHORITY[] node from within the
* WKT tree, and fetch the code value.
*
* While in theory values may be non-numeric, for the EPSG authority all
* code values should be integral.
*
* This method is the same as the C function OSRGetAuthorityCode().
*
* @param pszTargetKey the partial or complete path to the node to
* get an authority from.  ie. "PROJCS", "GEOGCS", "GEOGCS|UNIT" or NULL to
* search for an authority node on the root element.
*
* @return value code from authority node, or NULL on failure.  The value
* returned is internal and should not be freed or modified.
*/

const char *
SpatialReference::getAuthorityCode(const char *pszTargetKey) const

{
	/* -------------------------------------------------------------------- */
	/*      Find the node below which the authority should be put.          */
	/* -------------------------------------------------------------------- */
	const SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return NULL;

	/* -------------------------------------------------------------------- */
	/*      Fetch AUTHORITY child if there is one.                          */
	/* -------------------------------------------------------------------- */
	if (poNode->findChild("AUTHORITY") == -1)
		return NULL;

	poNode = poNode->getChild(poNode->findChild("AUTHORITY"));

	/* -------------------------------------------------------------------- */
	/*      Create a new authority node.                                    */
	/* -------------------------------------------------------------------- */
	if (poNode->getChildCount() < 2)
		return NULL;

	return poNode->getChild(1)->getValue();
}
 
/************************************************************************/
/*                          getAuthorityName()                          */
/************************************************************************/

/**
* \brief Get the authority name for a node.
*
* This method is used to query an AUTHORITY[] node from within the
* WKT tree, and fetch the authority name value.
*
* The most common authority is "EPSG".
*
* This method is the same as the C function OSRGetAuthorityName().
*
* @param pszTargetKey the partial or complete path to the node to
* get an authority from.  ie. "PROJCS", "GEOGCS", "GEOGCS|UNIT" or NULL to
* search for an authority node on the root element.
*
* @return value code from authority node, or NULL on failure. The value
* returned is internal and should not be freed or modified.
*/

const char *
SpatialReference::getAuthorityName(const char *pszTargetKey) const

{
	/* -------------------------------------------------------------------- */
	/*      Find the node below which the authority should be put.          */
	/* -------------------------------------------------------------------- */
	const SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return NULL;

	/* -------------------------------------------------------------------- */
	/*      Fetch AUTHORITY child if there is one.                          */
	/* -------------------------------------------------------------------- */
	if (poNode->findChild("AUTHORITY") == -1)
		return NULL;

	poNode = poNode->getChild(poNode->findChild("AUTHORITY"));

	/* -------------------------------------------------------------------- */
	/*      Create a new authority node.                                    */
	/* -------------------------------------------------------------------- */
	if (poNode->getChildCount() < 2)
		return NULL;

	return poNode->getChild(0)->getValue();
}
 
/************************************************************************/
/*                           stripVertical()                            */
/************************************************************************/

/**
* \brief Convert a compound cs into a horizontal CS.
*
* If this SRS is of type COMPD_CS[] then the vertical CS and the root COMPD_CS
* nodes are stripped resulting and only the horizontal coordinate system
* portion remains (normally PROJCS, GEOGCS or LOCAL_CS).
*
* If this is not a compound coordinate system then nothing is changed.
*
* @since OGR 1.8.0
*/

bool SpatialReference::stripVertical()

{
	if (getRoot() == NULL
		|| !cslIEqualString(getRoot()->getValue(), "COMPD_CS"))
		return true;

	SRSNode *poHorizontalCS = getRoot()->getChild(1);
	if (poHorizontalCS != NULL)
		poHorizontalCS = poHorizontalCS->clone();
	setRoot(poHorizontalCS);

	return true;
}

/************************************************************************/
/*                            stripCTParms()                            */
/************************************************************************/

/**
* \brief Strip OGC CT Parameters.
*
* This method will remove all components of the coordinate system
* that are specific to the OGC CT Specification.  That is it will attempt
* to strip it down to being compatible with the Simple Features 1.0
* specification.
*
* This method is the same as the C function OSRStripCTParms().
*
* @param poCurrent node to operate on.  NULL to operate on whole tree.
*
* @return true on success or an error code.
*/

bool SpatialReference::stripCTParms(SRSNode * poCurrent)

{
	if (poCurrent == NULL)
	{
		stripVertical();
		poCurrent = getRoot();
	}

	if (poCurrent == NULL)
		return true;

	if (poCurrent == getRoot() && cslIEqualString(poCurrent->getValue(), "LOCAL_CS"))
	{
		delete poCurrent;
		poRoot = NULL;

		return true;
	}

	if (poCurrent == NULL)
		return true;

	poCurrent->stripNodes("AUTHORITY");
	poCurrent->stripNodes("TOWGS84");
	poCurrent->stripNodes("AXIS");
	poCurrent->stripNodes("EXTENSION");

	return true;
}
 
/************************************************************************/
/*                             isCompound()                             */
/************************************************************************/

/**
* \brief Check if coordinate system is compound.
*
* This method is the same as the C function OSRIsCompound().
*
* @return TRUE if this is rooted with a COMPD_CS node.
*/

int SpatialReference::isCompound() const

{
	if (poRoot == NULL)
		return FALSE;

	return cslIEqualString(poRoot->getValue(), "COMPD_CS");
}
 
/************************************************************************/
/*                            isProjected()                             */
/************************************************************************/

/**
* \brief Check if projected coordinate system.
*
* This method is the same as the C function OSRIsProjected().
*
* @return TRUE if this contains a PROJCS node indicating a it is a
* projected coordinate system.
*/

int SpatialReference::isProjected() const

{
	if (poRoot == NULL)
		return FALSE;

	if (cslIEqualString(poRoot->getValue(), "PROJCS"))
		return TRUE;
	else if (cslIEqualString(poRoot->getValue(), "COMPD_CS"))
		return getAttrNode("PROJCS") != NULL;
	else
		return FALSE;
}
 
/************************************************************************/
/*                            isGeocentric()                            */
/************************************************************************/

/**
* \brief Check if geocentric coordinate system.
*
* This method is the same as the C function OSRIsGeocentric().
*
* @return TRUE if this contains a GEOCCS node indicating a it is a
* geocentric coordinate system.
*
* @since OGR 1.9.0
*/

int SpatialReference::isGeocentric() const

{
	if (poRoot == NULL)
		return FALSE;

	if (cslIEqualString(poRoot->getValue(), "GEOCCS"))
		return TRUE;
	else
		return FALSE;
} 
/************************************************************************/
/*                            isGeographic()                            */
/************************************************************************/

/**
* \brief Check if geographic coordinate system.
*
* This method is the same as the C function OSRIsGeographic().
*
* @return TRUE if this spatial reference is geographic ... that is the
* root is a GEOGCS node.
*/

int SpatialReference::isGeographic() const

{
	if (getRoot() == NULL)
		return FALSE;

	if (cslIEqualString(poRoot->getValue(), "GEOGCS"))
		return TRUE;
	else if (cslIEqualString(poRoot->getValue(), "COMPD_CS"))
		return getAttrNode("GEOGCS") != NULL
		&& getAttrNode("PROJCS") == NULL;
	else
		return FALSE;
}
 
/************************************************************************/
/*                              isLocal()                               */
/************************************************************************/

/**
* \brief Check if local coordinate system.
*
* This method is the same as the C function OSRIsLocal().
*
* @return TRUE if this spatial reference is local ... that is the
* root is a LOCAL_CS node.
*/

int SpatialReference::isLocal() const

{
	if (getRoot() == NULL)
		return FALSE;

	return cslIEqualString(getRoot()->getValue(), "LOCAL_CS");
}
 
/************************************************************************/
/*                            isVertical()                              */
/************************************************************************/

/**
* \brief Check if vertical coordinate system.
*
* This method is the same as the C function OSRIsVertical().
*
* @return TRUE if this contains a VERT_CS node indicating a it is a
* vertical coordinate system.
*
* @since OGR 1.8.0
*/

int SpatialReference::isVertical() const

{
	if (poRoot == NULL)
		return FALSE;

	if (cslIEqualString(poRoot->getValue(), "VERT_CS"))
		return TRUE;
	else if (cslIEqualString(poRoot->getValue(), "COMPD_CS"))
		return getAttrNode("VERT_CS") != NULL;
	else
		return FALSE;
}
 
/************************************************************************/
/*                            cloneGeogCS()                             */
/************************************************************************/

/**
* \brief Make a duplicate of the GEOGCS node of this SpatialReference object.
*
* @return a new SRS, which becomes the responsibility of the caller.
*/
SpatialReference *SpatialReference::cloneGeogCS() const

{
	const SRSNode *poGeogCS;
	SpatialReference * poNewSRS;

	/* -------------------------------------------------------------------- */
	/*      We have to reconstruct the GEOGCS node for geocentric           */
	/*      coordinate systems.                                             */
	/* -------------------------------------------------------------------- */
	if (isGeocentric())
	{
		const SRSNode *poDatum = getAttrNode("DATUM");
		const SRSNode *poPRIMEM = getAttrNode("PRIMEM");
		SRSNode *poGeogCS;

		if (poDatum == NULL || poPRIMEM == NULL)
			return NULL;

		poGeogCS = new SRSNode("GEOGCS");
		poGeogCS->addChild(new SRSNode("unnamed"));
		poGeogCS->addChild(poDatum->clone());
		poGeogCS->addChild(poPRIMEM->clone());

		poNewSRS = new SpatialReference();
		poNewSRS->setRoot(poGeogCS);

		poNewSRS->setAngularUnits("degree", cslStringToNumber(SRS_UA_DEGREE_CONV));

		return poNewSRS;
	}

	/* -------------------------------------------------------------------- */
	/*      For all others we just search the tree, and duplicate.          */
	/* -------------------------------------------------------------------- */
	poGeogCS = getAttrNode("GEOGCS");
	if (poGeogCS == NULL)
		return NULL;

	poNewSRS = new SpatialReference();
	poNewSRS->setRoot(poGeogCS->clone());

	return poNewSRS;
}
 
/************************************************************************/
/*                            isSameGeogCS()                            */
/************************************************************************/

/**
* \brief Do the GeogCS'es match?
*
* This method is the same as the C function OSRIsSameGeogCS().
*
* @param poOther the SRS being compared against.
*
* @return TRUE if they are the same or FALSE otherwise.
*/

int SpatialReference::isSameGeogCS(const SpatialReference *poOther) const

{
	const char *pszThisValue, *pszOtherValue;

	/* -------------------------------------------------------------------- */
	/*      Does the datum name match?  Note that we assume                 */
	/*      compatibility if either is missing a datum.                     */
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("DATUM");
	pszOtherValue = poOther->getAttrValue("DATUM");

	if (pszThisValue != NULL && pszOtherValue != NULL
		&& !cslIEqualString(pszThisValue, pszOtherValue))
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Do the datum TOWGS84 values match if present?                   */
	/* -------------------------------------------------------------------- */
	double adfTOWGS84[7], adfOtherTOWGS84[7];
	int i;

	this->getTOWGS84(adfTOWGS84, 7);
	poOther->getTOWGS84(adfOtherTOWGS84, 7);

	for (i = 0; i < 7; i++)
	{
		if (fabs(adfTOWGS84[i] - adfOtherTOWGS84[i]) > 0.00001)
			return FALSE;
	}

	/* -------------------------------------------------------------------- */
	/*      Do the prime meridians match?  If missing assume a value of zero.*/
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("PRIMEM", 1);
	if (pszThisValue == NULL)
		pszThisValue = "0.0";

	pszOtherValue = poOther->getAttrValue("PRIMEM", 1);
	if (pszOtherValue == NULL)
		pszOtherValue = "0.0";

	if (cslStringToNumber(pszOtherValue) != cslStringToNumber(pszThisValue))
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Do the units match?                                             */
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("GEOGCS|UNIT", 1);
	if (pszThisValue == NULL)
		pszThisValue = SRS_UA_DEGREE_CONV;

	pszOtherValue = poOther->getAttrValue("GEOGCS|UNIT", 1);
	if (pszOtherValue == NULL)
		pszOtherValue = SRS_UA_DEGREE_CONV;

	if (fabs(cslStringToNumber(pszOtherValue) - cslStringToNumber(pszThisValue)) > 0.00000001)
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Does the spheroid match.  Check semi major, and inverse         */
	/*      flattening.                                                     */
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("SPHEROID", 1);
	pszOtherValue = poOther->getAttrValue("SPHEROID", 1);
	if (pszThisValue != NULL && pszOtherValue != NULL
		&& fabs(cslStringToNumber(pszThisValue) - cslStringToNumber(pszOtherValue)) > 0.01)
		return FALSE;

	pszThisValue = this->getAttrValue("SPHEROID", 2);
	pszOtherValue = poOther->getAttrValue("SPHEROID", 2);
	if (pszThisValue != NULL && pszOtherValue != NULL
		&& fabs(cslStringToNumber(pszThisValue) - cslStringToNumber(pszOtherValue)) > 0.0001)
		return FALSE;

	return TRUE;
}
 
/************************************************************************/
/*                            isSameVertCS()                            */
/************************************************************************/

/**
* \brief Do the VertCS'es match?
*
* This method is the same as the C function OSRIsSameVertCS().
*
* @param poOther the SRS being compared against.
*
* @return TRUE if they are the same or FALSE otherwise.
*/

int SpatialReference::isSameVertCS(const SpatialReference *poOther) const

{
	const char *pszThisValue, *pszOtherValue;

	/* -------------------------------------------------------------------- */
	/*      Does the datum name match?                                      */
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("VERT_DATUM");
	pszOtherValue = poOther->getAttrValue("VERT_DATUM");

	if (pszThisValue == NULL || pszOtherValue == NULL
		|| !cslIEqualString(pszThisValue, pszOtherValue))
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Do the units match?                                             */
	/* -------------------------------------------------------------------- */
	pszThisValue = this->getAttrValue("VERT_CS|UNIT", 1);
	if (pszThisValue == NULL)
		pszThisValue = "1.0";

	pszOtherValue = poOther->getAttrValue("VERT_CS|UNIT", 1);
	if (pszOtherValue == NULL)
		pszOtherValue = "1.0";

	if (fabs(cslStringToNumber(pszOtherValue) - cslStringToNumber(pszThisValue)) > 0.00000001)
		return FALSE;

	return TRUE;
}
 
/************************************************************************/
/*                               isSame()                               */
/************************************************************************/

/**
* \brief Do these two spatial references describe the same system ?
*
* @param poOtherSRS the SRS being compared to.
*
* @return TRUE if equivalent or FALSE otherwise.
*/

int SpatialReference::isSame(const SpatialReference * poOtherSRS) const

{
	if (getRoot() == NULL && poOtherSRS->getRoot() == NULL)
		return TRUE;
	else if (getRoot() == NULL || poOtherSRS->getRoot() == NULL)
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Compare geographic coordinate system.                           */
	/* -------------------------------------------------------------------- */
	if (!isSameGeogCS(poOtherSRS))
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Do the have the same root types?  Ie. is one PROJCS and one     */
	/*      GEOGCS or perhaps LOCALCS?                                      */
	/* -------------------------------------------------------------------- */
	if (!cslIEqualString(getRoot()->getValue(), poOtherSRS->getRoot()->getValue()))
		return FALSE;

	/* -------------------------------------------------------------------- */
	/*      Compare projected coordinate system.                            */
	/* -------------------------------------------------------------------- */
	if (isProjected())
	{
		const char *pszValue1, *pszValue2;
		const SRSNode *poPROJCS = getAttrNode("PROJCS");

		pszValue1 = this->getAttrValue("PROJECTION");
		pszValue2 = poOtherSRS->getAttrValue("PROJECTION");
		if (pszValue1 == NULL || pszValue2 == NULL
			|| !cslIEqualString(pszValue1, pszValue2))
			return FALSE;

		for (int iChild = 0; iChild < poPROJCS->getChildCount(); iChild++)
		{
			const SRSNode    *poNode;

			poNode = poPROJCS->getChild(iChild);
			if (!cslIEqualString(poNode->getValue(), "PARAMETER")
				|| poNode->getChildCount() != 2)
				continue;

			/* this this eventually test within some epsilon? */
			if (this->getProjParm(poNode->getChild(0)->getValue())
				!= poOtherSRS->getProjParm(poNode->getChild(0)->getValue()))
				return FALSE;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      If they are LOCALCS/PROJCS, do they have the same units?        */
	/* -------------------------------------------------------------------- */
	if (isLocal() || isProjected())
	{
		if (getLinearUnits() != 0.0)
		{
			double      dfRatio;

			dfRatio = poOtherSRS->getLinearUnits() / getLinearUnits();
			if (dfRatio < 0.9999999999 || dfRatio > 1.000000001)
				return FALSE;
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Compare vertical coordinate system.                             */
	/* -------------------------------------------------------------------- */
	if (isVertical() && !isSameVertCS(poOtherSRS))
		return FALSE;

	return TRUE;
}
/************************************************************************/
/*                             setTOWGS84()                             */
/************************************************************************/

/**
* \brief Set the Bursa-Wolf conversion to WGS84.
*
* This will create the TOWGS84 node as a child of the DATUM.  It will fail
* if there is no existing DATUM node.  Unlike most SpatialReference
* methods it will insert itself in the appropriate order, and will replace
* an existing TOWGS84 node if there is one.
*
* The parameters have the same meaning as EPSG transformation 9606
* (Position Vector 7-param. transformation).
*
* This method is the same as the C function OSRSetTOWGS84().
*
* @param dfDX X child in meters.
* @param dfDY Y child in meters.
* @param dfDZ Z child in meters.
* @param dfEX X rotation in arc seconds (optional, defaults to zero).
* @param dfEY Y rotation in arc seconds (optional, defaults to zero).
* @param dfEZ Z rotation in arc seconds (optional, defaults to zero).
* @param dfPPM scaling factor (parts per million).
*
* @return true on success.
*/

bool SpatialReference::setTOWGS84(double dfDX, double dfDY, double dfDZ,
	double dfEX, double dfEY, double dfEZ,
	double dfPPM)

{
	SRSNode     *poDatum, *poTOWGS84;
	int             iPosition;
	char            szValue[64];

	poDatum = getAttrNode("DATUM");
	if (poDatum == NULL)
		return false;

	if (poDatum->findChild("TOWGS84") != -1)
		poDatum->destroyChild(poDatum->findChild("TOWGS84"));

	iPosition = poDatum->getChildCount();
	if (poDatum->findChild("AUTHORITY") != -1)
	{
		iPosition = poDatum->findChild("AUTHORITY");
	}

	poTOWGS84 = new SRSNode("TOWGS84");

	printDouble(szValue, dfDX);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfDY);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfDZ);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfEX);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfEY);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfEZ);
	poTOWGS84->addChild(new SRSNode(szValue));

	printDouble(szValue, dfPPM);
	poTOWGS84->addChild(new SRSNode(szValue));

	poDatum->insertChild(poTOWGS84, iPosition);

	return true;
}

/************************************************************************/
/*                             getTOWGS84()                             */
/************************************************************************/

/**
* \brief Fetch TOWGS84 parameters, if available.
*
* @param padfCoeff array into which up to 7 coefficients are placed.
* @param nCoeffCount size of padfCoeff - defaults to 7.
*
* @return true on success, or false if there is no
* TOWGS84 node available.
*/

bool SpatialReference::getTOWGS84(double * padfCoeff,
	int nCoeffCount) const

{
	const SRSNode   *poNode = getAttrNode("TOWGS84");

	memset(padfCoeff, 0, sizeof(double) * nCoeffCount);

	if (poNode == NULL)
		return false;

	for (int i = 0; i < nCoeffCount && i < poNode->getChildCount(); i++)
	{
		padfCoeff[i] = cslStringToNumber(poNode->getChild(i)->getValue());
	}

	return true;
}

/************************************************************************/
/*                         isAngularParameter()                         */
/*                                                                      */
/*      Is the passed projection parameter an angular one?              */
/************************************************************************/

int SpatialReference::isAngularParameter(const char *pszParameterName)

{
	if (cslNIEqualString(pszParameterName, "long", 4)
		|| cslNIEqualString(pszParameterName, "lati", 4)
		|| cslIEqualString(pszParameterName, SRS_PP_CENTRAL_MERIDIAN)
		|| cslNIEqualString(pszParameterName, "standard_parallel", 17)
		|| cslIEqualString(pszParameterName, SRS_PP_AZIMUTH)
		|| cslIEqualString(pszParameterName, SRS_PP_RECTIFIED_GRID_ANGLE))
		return TRUE;
	else
		return FALSE;
}

/************************************************************************/
/*                        isLongitudeParameter()                        */
/*                                                                      */
/*      Is the passed projection parameter an angular longitude         */
/*      (relative to a prime meridian)?                                 */
/************************************************************************/

int SpatialReference::isLongitudeParameter(const char *pszParameterName)

{
	if (cslNIEqualString(pszParameterName, "long", 4)
		|| cslIEqualString(pszParameterName, SRS_PP_CENTRAL_MERIDIAN))
		return TRUE;
	else
		return FALSE;
}

/************************************************************************/
/*                         isLinearParameter()                          */
/*                                                                      */
/*      Is the passed projection parameter an linear one measured in    */
/*      meters or some similar linear measure.                          */
/************************************************************************/

int SpatialReference::isLinearParameter(const char *pszParameterName)

{
	if (cslNIEqualString(pszParameterName, "false_", 6)
		|| cslIEqualString(pszParameterName, SRS_PP_SATELLITE_HEIGHT))
		return TRUE;
	else
		return FALSE;
}

/************************************************************************/
/*                            getNormInfo()                             */
/************************************************************************/

/**
* \brief Set the internal information for normalizing linear, and angular values.
*/
void SpatialReference::getNormInfo(void) const

{
	if (bNormInfoSet)
		return;

	/* -------------------------------------------------------------------- */
	/*      Initialize values.                                              */
	/* -------------------------------------------------------------------- */
	SpatialReference *poThis = (SpatialReference *) this;

	poThis->bNormInfoSet = TRUE;

	poThis->dfFromGreenwich = getPrimeMeridian(NULL);
	poThis->dfToMeter = getLinearUnits(NULL);
	poThis->dfToDegrees = getAngularUnits(NULL) / cslStringToNumber(SRS_UA_DEGREE_CONV);
	if (fabs(poThis->dfToDegrees - 1.0) < 0.000000001)
		poThis->dfToDegrees = 1.0;
}

/************************************************************************/
/*                           fixupOrdering()                            */
/************************************************************************/

/**
* \brief Correct parameter ordering to match CT Specification.
*
* Some mechanisms to create WKT using SpatialReference, and some
* imported WKT fail to maintain the order of parameters required according
* to the BNF definitions in the OpenGIS SF-SQL and CT Specifications.  This
* method attempts to massage things back into the required order.
*
* This method is the same as the C function OSRFixupOrdering().
*
* @return true on success or an error code if something goes
* wrong.
*/

bool SpatialReference::fixupOrdering()

{
	if (getRoot() != NULL)
		return getRoot()->fixupOrdering();
	else
		return true;
}
 
/************************************************************************/
/*                               fixup()                                */
/************************************************************************/

/**
* \brief fixup as needed.
*
* Some mechanisms to create WKT using SpatialReference, and some
* imported WKT, are not valid according to the OGC CT specification.  This
* method attempts to fill in any missing defaults that are required, and
* fixup ordering problems (using OSRFixupOrdering()) so that the resulting
* WKT is valid.
*
* This method should be expected to evolve over time to as problems are
* discovered.  The following are amoung the fixup actions this method will
* take:
*
* - fixup the ordering of nodes to match the BNF WKT ordering, using
* the fixupOrdering() method.
*
* - Add missing linear or angular units nodes.
*
* This method is the same as the C function OSRFixup().
*
* @return true on success or an error code if something goes
* wrong.
*/

bool SpatialReference::fixup()

{
	/* -------------------------------------------------------------------- */
	/*      Ensure linear units defaulted to METER if missing for PROJCS,   */
	/*      GEOCCS or LOCAL_CS.                                             */
	/* -------------------------------------------------------------------- */
	const SRSNode *poCS = getAttrNode("PROJCS");

	if (poCS == NULL)
		poCS = getAttrNode("LOCAL_CS");

	if (poCS == NULL)
		poCS = getAttrNode("GEOCCS");

	if (poCS != NULL && poCS->findChild("UNIT") == -1)
		setLinearUnits(SRS_UL_METER, 1.0);

	/* -------------------------------------------------------------------- */
	/*      Ensure angular units defaulted to degrees on the GEOGCS.        */
	/* -------------------------------------------------------------------- */
	poCS = getAttrNode("GEOGCS");
	if (poCS != NULL && poCS->findChild("UNIT") == -1)
		setAngularUnits(SRS_UA_DEGREE, cslStringToNumber(SRS_UA_DEGREE_CONV));

	return fixupOrdering();
}

/************************************************************************/
/*                            getExtension()                            */
/************************************************************************/

/**
* \brief Fetch extension value.
*
* Fetch the value of the named EXTENSION item for the identified
* target node.
*
* @param pszTargetKey the name or path to the parent node of the EXTENSION.
* @param pszName the name of the extension being fetched.
* @param pszDefault the value to return if the extension is not found.
*
* @return node value if successful or pszDefault on failure.
*/

const char *SpatialReference::getExtension(const char *pszTargetKey,
	const char *pszName,
	const char *pszDefault) const

{
	/* -------------------------------------------------------------------- */
	/*      Find the target node.                                           */
	/* -------------------------------------------------------------------- */
	const SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return NULL;

	/* -------------------------------------------------------------------- */
	/*      Fetch matching EXTENSION if there is one.                       */
	/* -------------------------------------------------------------------- */
	for (int i = poNode->getChildCount() - 1; i >= 0; i--)
	{
		const SRSNode *poChild = poNode->getChild(i);

		if (cslIEqualString(poChild->getValue(), "EXTENSION")
			&& poChild->getChildCount() >= 2)
		{
			if (cslIEqualString(poChild->getChild(0)->getValue(), pszName))
				return poChild->getChild(1)->getValue();
		}
	}

	return pszDefault;
}

/************************************************************************/
/*                            setExtension()                            */
/************************************************************************/
/**
* \brief Set extension value.
*
* Set the value of the named EXTENSION item for the identified
* target node.
*
* @param pszTargetKey the name or path to the parent node of the EXTENSION.
* @param pszName the name of the extension being fetched.
* @param pszValue the value to set
*
* @return true on success
*/

bool SpatialReference::setExtension(const char *pszTargetKey,
	const char *pszName,
	const char *pszValue)

{
	/* -------------------------------------------------------------------- */
	/*      Find the target node.                                           */
	/* -------------------------------------------------------------------- */
	SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      Fetch matching EXTENSION if there is one.                       */
	/* -------------------------------------------------------------------- */
	for (int i = poNode->getChildCount() - 1; i >= 0; i--)
	{
		SRSNode *poChild = poNode->getChild(i);

		if (cslIEqualString(poChild->getValue(), "EXTENSION")
			&& poChild->getChildCount() >= 2)
		{
			if (cslIEqualString(poChild->getChild(0)->getValue(), pszName))
			{
				poChild->getChild(1)->setValue(pszValue);
				return true;
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Create a new EXTENSION node.                                    */
	/* -------------------------------------------------------------------- */
	SRSNode *poAuthNode;

	poAuthNode = new SRSNode("EXTENSION");
	poAuthNode->addChild(new SRSNode(pszName));
	poAuthNode->addChild(new SRSNode(pszValue));

	poNode->addChild(poAuthNode);

	return true;
}

/************************************************************************/
/*                              getAxis()                               */
/************************************************************************/

/**
* \brief Fetch the orientation of one axis.
*
* Fetches the the request axis (iAxis - zero based) from the
* indicated portion of the coordinate system (pszTargetKey) which
* should be either "GEOGCS" or "PROJCS".
*
* No CPLError is issued on routine failures (such as not finding the AXIS).
*
* This method is equivalent to the C function OSRGetAxis().
*
* @param pszTargetKey the coordinate system part to query ("PROJCS" or "GEOGCS").
* @param iAxis the axis to query (0 for first, 1 for second).
* @param peOrientation location into which to place the fetch orientation, may be NULL.
*
* @return the name of the axis or NULL on failure.
*/

const char *
SpatialReference::getAxis(const char *pszTargetKey, int iAxis,
	AxisOrientation *peOrientation) const

{
	if (peOrientation != NULL)
		*peOrientation = AO_Other;

	/* -------------------------------------------------------------------- */
	/*      Find the target node.                                           */
	/* -------------------------------------------------------------------- */
	SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return NULL;

	/* -------------------------------------------------------------------- */
	/*      Find desired child AXIS.                                        */
	/* -------------------------------------------------------------------- */
	SRSNode *poAxis = NULL;
	int iChild, nChildCount = poNode->getChildCount();

	for (iChild = 0; iChild < nChildCount; iChild++)
	{
		SRSNode *poChild = poNode->getChild(iChild);

		if (!cslIEqualString(poChild->getValue(), "AXIS"))
			continue;

		if (iAxis == 0)
		{
			poAxis = poChild;
			break;
		}
		iAxis--;
	}

	if (poAxis == NULL)
		return NULL;

	if (poAxis->getChildCount() < 2)
		return NULL;

	/* -------------------------------------------------------------------- */
	/*      Extract name and orientation if possible.                       */
	/* -------------------------------------------------------------------- */
	if (peOrientation != NULL)
	{
		const char *pszOrientation = poAxis->getChild(1)->getValue();

		if (cslIEqualString(pszOrientation, "NORTH"))
			*peOrientation = AO_North;
		else if (cslIEqualString(pszOrientation, "EAST"))
			*peOrientation = AO_East;
		else if (cslIEqualString(pszOrientation, "SOUTH"))
			*peOrientation = AO_South;
		else if (cslIEqualString(pszOrientation, "WEST"))
			*peOrientation = AO_West;
		else if (cslIEqualString(pszOrientation, "UP"))
			*peOrientation = AO_Up;
		else if (cslIEqualString(pszOrientation, "DOWN"))
			*peOrientation = AO_Down;
		else if (cslIEqualString(pszOrientation, "OTHER"))
			*peOrientation = AO_Other;
		else
		{
			//Debug("OSR", "Unrecognised orientation value '%s'.",
			//	pszOrientation);
		}
	}

	return poAxis->getChild(0)->getValue();
}
/************************************************************************/
/*                         axisEnumToName()                          */
/************************************************************************/

/**
* \brief Return the string representation for the AxisOrientation enumeration.
*
* For example "NORTH" for AO_North.
*
* @return an internal string
*/
const char *axisEnumToName(AxisOrientation eOrientation)

{
	if (eOrientation == AO_North)
		return "NORTH";
	if (eOrientation == AO_East)
		return "EAST";
	if (eOrientation == AO_South)
		return "SOUTH";
	if (eOrientation == AO_West)
		return "WEST";
	if (eOrientation == AO_Up)
		return "UP";
	if (eOrientation == AO_Down)
		return "DOWN";
	if (eOrientation == AO_Other)
		return "OTHER";

	return "UNKNOWN";
}

/************************************************************************/
/*                              setAxes()                               */
/************************************************************************/

/**
* \brief Set the axes for a coordinate system.
*
* Set the names, and orientations of the axes for either a projected
* (PROJCS) or geographic (GEOGCS) coordinate system.
*
* This method is equivalent to the C function OSRSetAxes().
*
* @param pszTargetKey either "PROJCS" or "GEOGCS", must already exist in SRS.
* @param pszXAxisName name of first axis, normally "Long" or "Easting".
* @param eXAxisOrientation normally AO_East.
* @param pszYAxisName name of second axis, normally "Lat" or "Northing".
* @param eYAxisOrientation normally AO_North.
*
* @return true on success or an error code.
*/

bool
SpatialReference::setAxes(const char *pszTargetKey,
	const char *pszXAxisName,
	AxisOrientation eXAxisOrientation,
	const char *pszYAxisName,
	AxisOrientation eYAxisOrientation)

{
	/* -------------------------------------------------------------------- */
	/*      Find the target node.                                           */
	/* -------------------------------------------------------------------- */
	SRSNode  *poNode;

	if (pszTargetKey == NULL)
		poNode = poRoot;
	else
		poNode = ((SpatialReference *) this)->getAttrNode(pszTargetKey);

	if (poNode == NULL)
		return false;

	/* -------------------------------------------------------------------- */
	/*      Strip any existing AXIS children.                               */
	/* -------------------------------------------------------------------- */
	while (poNode->findChild("AXIS") >= 0)
		poNode->destroyChild(poNode->findChild("AXIS"));

	/* -------------------------------------------------------------------- */
	/*      Insert desired axes                                             */
	/* -------------------------------------------------------------------- */
	SRSNode *poAxis = new SRSNode("AXIS");

	poAxis->addChild(new SRSNode(pszXAxisName));
	poAxis->addChild(new SRSNode(axisEnumToName(eXAxisOrientation)));

	poNode->addChild(poAxis);

	poAxis = new SRSNode("AXIS");

	poAxis->addChild(new SRSNode(pszYAxisName));
	poAxis->addChild(new SRSNode(axisEnumToName(eYAxisOrientation)));

	poNode->addChild(poAxis);

	return true;
}


end_gdb_namespace
end_gtl_namespace


