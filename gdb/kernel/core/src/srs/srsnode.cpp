#include "srsnode.h" 
//derived from gdal
begin_gtl_namespace
begin_gdb_namespace 
/************************************************************************/
/*                            SRSNode()                             */
/************************************************************************/

/**
* Constructor.
*
* @param pszValueIn this optional parameter can be used to initialize
* the value of the node upon creation.  If omitted the node will be created
* with a value of "".  Newly created SRSNodes have no children.
*/

SRSNode::SRSNode(const char * pszValueIn){
	int s = strlen(pszValueIn);
	pszValue =(char*) malloc(s+1);
	memcpy(pszValue, pszValueIn, s + 1);
	nChildren = 0;
	papoChildNodes = 0;
	poParent = 0;
}

/************************************************************************/
/*                            ~SRSNode()                            */
/************************************************************************/

SRSNode::~SRSNode(){
	if(pszValue)
		free(pszValue);
	pszValue = 0;
	clearChildren();
}

/************************************************************************/
/*                          clearChildren()                            */
/************************************************************************/

void SRSNode::clearChildren()

{
	for (int i = 0; i < nChildren; i++){
		free(papoChildNodes[i]);
	}

	if(papoChildNodes)
		free(papoChildNodes);

	papoChildNodes = 0;
	nChildren = 0;
}

/************************************************************************/
/*                           GetChildCount()                            */
/************************************************************************/

/**
* \fn int SRSNode::GetChildCount() const;
*
* Get number of children nodes.
*
* @return 0 for leaf nodes, or the number of children nodes.
*/

/************************************************************************/
/*                              GetChild()                              */
/************************************************************************/

/**
* Fetch requested child.
*
* @param iChild the index of the child to fetch, from 0 to
* GetChildCount() - 1.
*
* @return a pointer to the child SRSNode, or NULL if there is no such
* child.
*/

SRSNode *SRSNode::getChild(int iChild)

{
	if (iChild < 0 || iChild >= nChildren)
		return 0;
	else
		return papoChildNodes[iChild];
}

const SRSNode *SRSNode::getChild(int iChild) const

{
	if (iChild < 0 || iChild >= nChildren)
		return 0;
	else
		return papoChildNodes[iChild];
}

/************************************************************************/
/*                              GetNode()                               */
/************************************************************************/

/**
* Find named node in tree.
*
* This method does a pre-order traversal of the node tree searching for
* a node with this exact value (case insensitive), and returns it.  Leaf
* nodes are not considered, under the assumption that they are just
* attribute value nodes.
*
* If a node appears more than once in the tree (such as UNIT for instance),
* the first encountered will be returned.  Use GetNode() on a subtree to be
* more specific.
*
* @param pszName the name of the node to search for.
*
* @return a pointer to the node found, or NULL if none.
*/

SRSNode *SRSNode::getNode(const char * pszName)

{
	int  i;

	if (this == NULL)
		return NULL;

	if (nChildren > 0 && cslIEqualString(pszName, pszValue))
		return this;

	/* -------------------------------------------------------------------- */
	/*      First we check the immediate children so we will get an         */
	/*      immediate child in preference to a subchild.                    */
	/* -------------------------------------------------------------------- */
	for (i = 0; i < nChildren; i++)
	{
		if (cslIEqualString(papoChildNodes[i]->pszValue, pszName)
			&& papoChildNodes[i]->nChildren > 0)
			return papoChildNodes[i];
	}

	/* -------------------------------------------------------------------- */
	/*      Then get each child to check their children.                    */
	/* -------------------------------------------------------------------- */
	for (i = 0; i < nChildren; i++)
	{
		SRSNode *poNode;

		poNode = papoChildNodes[i]->getNode(pszName);
		if (poNode != NULL)
			return poNode;
	}

	return NULL;
}

const SRSNode *SRSNode::getNode(const char * pszName) const

{
	return ((SRSNode *) this)->getNode(pszName);
}

/************************************************************************/
/*                              AddChild()                              */
/************************************************************************/

/**
* Add passed node as a child of target node.
*
* Note that ownership of the passed node is assumed by the node on which
* the method is invoked ... use the Clone() method if the original is to
* be preserved.  New children are always added at the end of the list.
*
* @param poNew the node to add as a child.
*/

void SRSNode::addChild(SRSNode * poNew){
	insertChild(poNew, nChildren);
}

/************************************************************************/
/*                            insertChild()                             */
/************************************************************************/

/**
* Insert the passed node as a child of target node, at the indicated
* position.
*
* Note that ownership of the passed node is assumed by the node on which
* the method is invoked ... use the Clone() method if the original is to
* be preserved.  All existing children at location iChild and beyond are
* push down one space to make space for the new child.
*
* @param poNew the node to add as a child.
* @param iChild position to insert, use 0 to insert at the beginning.
*/

void SRSNode::insertChild(SRSNode * poNew, int iChild)

{
	if (iChild > nChildren)
		iChild = nChildren;

	nChildren++;
	papoChildNodes = (SRSNode **)
		realloc(papoChildNodes, sizeof(void*) * nChildren);

	memmove(papoChildNodes + iChild + 1, papoChildNodes + iChild,
		sizeof(void*) * (nChildren - iChild - 1));

	papoChildNodes[iChild] = poNew;
	poNew->poParent = this;
}

/************************************************************************/
/*                            DestroyChild()                            */
/************************************************************************/

/**
* Remove a child node, and it's subtree.
*
* Note that removing a child node will result in children after it
* being renumbered down one.
*
* @param iChild the index of the child.
*/

void SRSNode::destroyChild(int iChild)

{
	if (iChild < 0 || iChild >= nChildren)
		return;

	delete papoChildNodes[iChild];
	while (iChild < nChildren - 1)
	{
		papoChildNodes[iChild] = papoChildNodes[iChild + 1];
		iChild++;
	}

	nChildren--;
}

/************************************************************************/
/*                             FindChild()                              */
/************************************************************************/

/**
* Find the index of the child matching the given string.
*
* Note that the node value must match pszValue with the exception of
* case.  The comparison is case insensitive.
*
* @param pszValue the node value being searched for.
*
* @return the child index, or -1 on failure.
*/

int SRSNode::findChild(const char * pszValue) const

{
	for (int i = 0; i < nChildren; i++)
	{
		if (cslIEqualString(papoChildNodes[i]->pszValue, pszValue))
			return i;
	}

	return -1;
}

/************************************************************************/
/*                              GetValue()                              */
/************************************************************************/

/**
* \fn const char *SRSNode::GetValue() const;
*
* Fetch value string for this node.
*
* @return A non-NULL string is always returned.  The returned pointer is to
* the internal value of this node, and should not be modified, or freed.
*/

/************************************************************************/
/*                              SetValue()                              */
/************************************************************************/

/**
* Set the node value.
*
* @param pszNewValue the new value to assign to this node.  The passed
* string is duplicated and remains the responsibility of the caller.
*/

void SRSNode::setValue(const char * pszNewValue){
	int s = strlen(pszNewValue);
	free(pszValue);
	pszValue =(char*) malloc(s + 1);
	memcpy(pszValue, pszNewValue, s + 1);
}

/************************************************************************/
/*                               Clone()                                */
/************************************************************************/

/**
* Make a duplicate of this node, and it's children.
*
* @return a new node tree, which becomes the responsiblity of the caller.
*/

SRSNode *SRSNode::clone() const{
	SRSNode *poNew;

	poNew = new SRSNode(pszValue);

	for (int i = 0; i < nChildren; i++)
	{
		poNew->addChild(papoChildNodes[i]->clone());
	}

	return poNew;
}

/************************************************************************/
/*                            NeedsQuoting()                            */
/*                                                                      */
/*      Does this node need to be quoted when it is exported to Wkt?    */
/************************************************************************/

int SRSNode::needsQuoting() const

{
	// non-terminals are never quoted.
	if (getChildCount() != 0)
		return FALSE;

	// As per bugzilla bug 201, the OGC spec says the authority code
	// needs to be quoted even though it appears well behaved.
	if (poParent != NULL && cslIEqualString(poParent->getValue(), "AUTHORITY"))
		return TRUE;

	// As per bugzilla bug 294, the OGC spec says the direction
	// values for the AXIS keywords should *not* be quoted.
	if (poParent != NULL && cslIEqualString(poParent->getValue(), "AXIS")
		&& this != poParent->getChild(0))
		return FALSE;

	// Strings starting with e or E are not valid numeric values, so they
	// need quoting, like in AXIS["E",EAST] 
	if ((pszValue[0] == 'e' || pszValue[0] == 'E'))
		return TRUE;

	// Non-numeric tokens are generally quoted while clean numeric values
	// are generally not. 
	for (int i = 0; pszValue[i] != '\0'; i++)
	{
		if ((pszValue[i] < '0' || pszValue[i] > '9')
			&& pszValue[i] != '.'
			&& pszValue[i] != '-' && pszValue[i] != '+'
			&& pszValue[i] != 'e' && pszValue[i] != 'E')
			return TRUE;
	}

	return FALSE;
}

/************************************************************************/
/*                            exportToWkt()                             */
/************************************************************************/

/**
* Convert this tree of nodes into WKT format.
*
* Note that the returned WKT string should be freed with OGRFree() or
* CPLFree() when no longer needed.  It is the responsibility of the caller.
*
* @param ppszResult the resulting string is returned in this pointer.
*
* @return currently OGRERR_NONE is always returned, but the future it
* is possible error conditions will develop.
*/


bool SRSNode::exportToWkt(char ** ppszResult) const

{
	char        **papszChildrenWkt = NULL;
	int         nLength = strlen(pszValue) + 4;
	int         i;

	/* -------------------------------------------------------------------- */
	/*      Build a list of the WKT format for the children.                */
	/* -------------------------------------------------------------------- */
	papszChildrenWkt = (char **)malloc(sizeof(char*)*(nChildren + 1));
	memset(papszChildrenWkt, 0, sizeof(char*)*(nChildren + 1));

	for (i = 0; i < nChildren; i++)
	{
		papoChildNodes[i]->exportToWkt(papszChildrenWkt + i);
		nLength += strlen(papszChildrenWkt[i]) + 1;
	}

	/* -------------------------------------------------------------------- */
	/*      Allocate the result string.                                     */
	/* -------------------------------------------------------------------- */
	*ppszResult = (char *)malloc(nLength);
	*ppszResult[0] = '\0';

	/* -------------------------------------------------------------------- */
	/*      Capture this nodes value.  We put it in double quotes if        */
	/*      this is a leaf node, otherwise we assume it is a well formed    */
	/*      node name.                                                      */
	/* -------------------------------------------------------------------- */
	if (needsQuoting())
	{
		strcat(*ppszResult, "\"");
		strcat(*ppszResult, pszValue); /* should we do quoting? */
		strcat(*ppszResult, "\"");
	}
	else
		strcat(*ppszResult, pszValue);

	/* -------------------------------------------------------------------- */
	/*      Add the children strings with appropriate brackets and commas.  */
	/* -------------------------------------------------------------------- */
	if (nChildren > 0)
		strcat(*ppszResult, "[");

	for (i = 0; i < nChildren; i++)
	{
		strcat(*ppszResult, papszChildrenWkt[i]);
		if (i == nChildren - 1)
			strcat(*ppszResult, "]");
		else
			strcat(*ppszResult, ",");
	}

	char **papszPtr;
	if (papszChildrenWkt){
		papszPtr = papszChildrenWkt;
		while (*papszPtr != NULL){
			free(*papszPtr);
			papszPtr++;
		}
		free(papszChildrenWkt);
	}
	return true;
}

/************************************************************************/
/*                         exportToPrettyWkt()                          */
/************************************************************************/

bool SRSNode::exportToPrettyWkt(char ** ppszResult, int nDepth) const

{
	char        **papszChildrenWkt = NULL;
	int         nLength = strlen(pszValue) + 4;
	int         i;

	/* -------------------------------------------------------------------- */
	/*      Build a list of the WKT format for the children.                */
	/* -------------------------------------------------------------------- */
	papszChildrenWkt = (char **)malloc(sizeof(char*)*(nChildren + 1));
	memset(papszChildrenWkt, 0, sizeof(char*)*(nChildren + 1));
	for (i = 0; i < nChildren; i++)
	{
		papoChildNodes[i]->exportToPrettyWkt(papszChildrenWkt + i,
			nDepth + 1);
		nLength += strlen(papszChildrenWkt[i]) + 2 + nDepth * 4;
	}

	/* -------------------------------------------------------------------- */
	/*      Allocate the result string.                                     */
	/* -------------------------------------------------------------------- */
	*ppszResult = (char *)malloc(nLength);
	*ppszResult[0] = '\0';

	/* -------------------------------------------------------------------- */
	/*      Capture this nodes value.  We put it in double quotes if        */
	/*      this is a leaf node, otherwise we assume it is a well formed    */
	/*      node name.                                                      */
	/* -------------------------------------------------------------------- */
	if (needsQuoting())
	{
		strcat(*ppszResult, "\"");
		strcat(*ppszResult, pszValue); /* should we do quoting? */
		strcat(*ppszResult, "\"");
	}
	else
		strcat(*ppszResult, pszValue);

	/* -------------------------------------------------------------------- */
	/*      Add the children strings with appropriate brackets and commas.  */
	/* -------------------------------------------------------------------- */
	if (nChildren > 0)
		strcat(*ppszResult, "[");

	for (i = 0; i < nChildren; i++)
	{
		if (papoChildNodes[i]->getChildCount() > 0)
		{
			int  j;

			strcat(*ppszResult, "\n");
			for (j = 0; j < 4 * nDepth; j++)
				strcat(*ppszResult, " ");
		}
		strcat(*ppszResult, papszChildrenWkt[i]);
		if (i < nChildren - 1)
			strcat(*ppszResult, ",");
	}

	if (nChildren > 0)
	{
		if ((*ppszResult)[strlen(*ppszResult) - 1] == ',')
			(*ppszResult)[strlen(*ppszResult) - 1] = '\0';

		strcat(*ppszResult, "]");
	}

	//CSLDestroy(papszChildrenWkt);
	char **papszPtr;
	if (papszChildrenWkt) {
		papszPtr = papszChildrenWkt;
		while (*papszPtr != NULL) {
			free(*papszPtr);
			papszPtr++;
		}
		free(papszChildrenWkt);
	}
	return true;
}

/************************************************************************/
/*                           importFromWkt()                            */
/************************************************************************/

/**
* Import from WKT string.
*
* This method will wipe the existing children and value of this node, and
* reassign them based on the contents of the passed WKT string.  Only as
* much of the input string as needed to construct this node, and it's
* children is consumed from the input string, and the input string pointer
* is then updated to point to the remaining (unused) input.
*
* @param ppszInput Pointer to pointer to input.  The pointer is updated to
* point to remaining unused input text.
*
* @return OGRERR_NONE if import succeeds, or OGRERR_CORRUPT_DATA if it
* fails for any reason.
*/

bool SRSNode::importFromWkt(char ** ppszInput)
{
	int nNodes = 0;
	return importFromWkt(ppszInput, 0, &nNodes);
}

bool SRSNode::importFromWkt(char ** ppszInput, int nRecLevel, int* pnNodes)

{
	const char  *pszInput = *ppszInput;
	int         bInQuotedString = FALSE;

	/* Sanity checks */
	if (nRecLevel == 10)
	{
		//return OGRERR_CORRUPT_DATA;
		return false;
	}
	if (*pnNodes == 1000)
	{
		//return OGRERR_CORRUPT_DATA;
		return false;
	}

	/* -------------------------------------------------------------------- */
	/*      Clear any existing children of this node.                       */
	/* -------------------------------------------------------------------- */
	clearChildren();

	/* -------------------------------------------------------------------- */
	/*      Read the ``value'' for this node.                               */
	/* -------------------------------------------------------------------- */
	char        szToken[512];
	int         nTokenLen = 0;

	while (*pszInput != '\0' && nTokenLen < (int) sizeof(szToken) - 1)
	{
		if (*pszInput == '"')
		{
			bInQuotedString = !bInQuotedString;
		}
		else if (!bInQuotedString
			&& (*pszInput == '[' || *pszInput == ']' || *pszInput == ','
				|| *pszInput == '(' || *pszInput == ')'))
		{
			break;
		}
		else if (!bInQuotedString
			&& (*pszInput == ' ' || *pszInput == '\t'
				|| *pszInput == 10 || *pszInput == 13))
		{
			/* just skip over whitespace */
		}
		else
		{
			szToken[nTokenLen++] = *pszInput;
		}

		pszInput++;
	}

	if (*pszInput == '\0' || nTokenLen == sizeof(szToken) - 1)
		return false;// return OGRERR_CORRUPT_DATA;

	szToken[nTokenLen++] = '\0';
	setValue(szToken);

	/* -------------------------------------------------------------------- */
	/*      Read children, if we have a sublist.                            */
	/* -------------------------------------------------------------------- */
	if (*pszInput == '[' || *pszInput == '(')
	{
		do
		{
			SRSNode *poNewChild;
			bool      eErr;

			pszInput++; // Skip bracket or comma.

			poNewChild = new SRSNode();

			(*pnNodes)++;
			eErr = poNewChild->importFromWkt((char **)&pszInput, nRecLevel + 1, pnNodes);
			if (eErr != true)
			{
				delete poNewChild;
				return eErr;
			}

			addChild(poNewChild);

			// swallow whitespace
			while (isspace(*pszInput))
				pszInput++;

		} while (*pszInput == ',');

		if (*pszInput != ')' && *pszInput != ']')
			return false;// return OGRERR_CORRUPT_DATA;

		pszInput++;
	}

	*ppszInput = (char *)pszInput;

	return true;//return OGRERR_NONE;
}

/************************************************************************/
/*                           MakeValueSafe()                            */
/************************************************************************/

/**
* Massage value string, stripping special characters so it will be a
* database safe string.
*
* The operation is also applies to all subnodes of the current node.
*/


void SRSNode::makeValueSafe()

{
	int         i, j;

	/* -------------------------------------------------------------------- */
	/*      First process subnodes.                                         */
	/* -------------------------------------------------------------------- */
	for (int iChild = 0; iChild < getChildCount(); iChild++)
	{
		getChild(iChild)->makeValueSafe();
	}

	/* -------------------------------------------------------------------- */
	/*      Skip numeric nodes.                                             */
	/* -------------------------------------------------------------------- */
	if ((pszValue[0] >= '0' && pszValue[0] <= '9') || pszValue[0] != '.')
		return;

	/* -------------------------------------------------------------------- */
	/*      Translate non-alphanumeric values to underscores.               */
	/* -------------------------------------------------------------------- */
	for (i = 0; pszValue[i] != '\0'; i++)
	{
		if (!(pszValue[i] >= 'A' && pszValue[i] <= 'Z')
			&& !(pszValue[i] >= 'a' && pszValue[i] <= 'z')
			&& !(pszValue[i] >= '0' && pszValue[i] <= '9'))
		{
			pszValue[i] = '_';
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Remove repeated and trailing underscores.                       */
	/* -------------------------------------------------------------------- */
	for (i = 1, j = 0; pszValue[i] != '\0'; i++)
	{
		if (pszValue[j] == '_' && pszValue[i] == '_')
			continue;

		pszValue[++j] = pszValue[i];
	}

	if (pszValue[j] == '_')
		pszValue[j] = '\0';
	else
		pszValue[j + 1] = '\0';
}

/************************************************************************/
/*                           applyRemapper()                            */
/************************************************************************/

/**
* Remap node values matching list.
*
* Remap the value of this node or any of it's children if it matches
* one of the values in the source list to the corresponding value from
* the destination list.  If the pszNode value is set, only do so if the
* parent node matches that value.  Even if a replacement occurs, searching
* continues.
*
* @param pszNode Restrict remapping to children of this type of node
*                (eg. "PROJECTION")
* @param papszSrcValues a NULL terminated array of source string.  If the
* node value matches one of these (case insensitive) then replacement occurs.
* @param papszDstValues an array of destination strings.  On a match, the
* one corresponding to a source value will be used to replace a node.
* @param nStepSize increment when stepping through source and destination
* arrays, allowing source and destination arrays to be one interleaved array
* for instances.  Defaults to 1.
* @param bChildOfHit Only TRUE if we the current node is the child of a match,
* and so needs to be set.  Application code would normally pass FALSE for this
* argument.
*
* @return returns OGRERR_NONE unless something bad happens.  There is no
* indication returned about whether any replacement occured.
*/

bool SRSNode::applyRemapper(const char *pszNode,
	char **papszSrcValues,
	char **papszDstValues,
	int nStepSize, int bChildOfHit)

{
	int i;

	/* -------------------------------------------------------------------- */
	/*      Scan for value, and replace if our parent was a "hit".          */
	/* -------------------------------------------------------------------- */
	if (bChildOfHit || pszNode == NULL)
	{
		for (i = 0; papszSrcValues[i] != NULL; i += nStepSize)
		{
			if (cslIEqualString(papszSrcValues[i], pszValue)  &&
				!cslIEqualString(papszDstValues[i], ""))
			{
				setValue(papszDstValues[i]);
				break;
			}
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Are the the target node?                                        */
	/* -------------------------------------------------------------------- */
	if (pszNode != NULL)
		bChildOfHit = cslIEqualString(pszValue, pszNode);

	/* -------------------------------------------------------------------- */
	/*      Recurse                                                         */
	/* -------------------------------------------------------------------- */
	for (i = 0; i < getChildCount(); i++)
	{
		getChild(i)->applyRemapper(pszNode, papszSrcValues,
			papszDstValues, nStepSize, bChildOfHit);
	}

	return true;
}

/************************************************************************/
/*                             StripNodes()                             */
/************************************************************************/

/**
* Strip child nodes matching name.
*
* Removes any decendent nodes of this node that match the given name.
* Of course children of removed nodes are also discarded.
*
* @param pszName the name for nodes that should be removed.
*/

void SRSNode::stripNodes(const char * pszName)

{
	/* -------------------------------------------------------------------- */
	/*      Strip any children matching this name.                          */
	/* -------------------------------------------------------------------- */
	while (findChild(pszName) >= 0)
		destroyChild(findChild(pszName));

	/* -------------------------------------------------------------------- */
	/*      Recurse                                                         */
	/* -------------------------------------------------------------------- */
	for (int i = 0; i < getChildCount(); i++)
		getChild(i)->stripNodes(pszName);
}

/************************************************************************/
/*                           FixupOrdering()                            */
/************************************************************************/

/* EXTENSION ... being a OSR extension... is arbitrary placed before the AUTHORITY */
static const char * const apszPROJCSRule[] =
{ "PROJCS", "GEOGCS", "PROJECTION", "PARAMETER", "UNIT", "AXIS", "EXTENSION", "AUTHORITY",
NULL };

static const char * const apszDATUMRule[] =
{ "DATUM", "SPHEROID", "TOWGS84", "AUTHORITY", NULL };

static const char * const apszGEOGCSRule[] =
{ "GEOGCS", "DATUM", "PRIMEM", "UNIT", "AXIS", "AUTHORITY", NULL };

static const char * const apszGEOCCSRule[] =
{ "GEOCCS", "DATUM", "PRIMEM", "UNIT", "AXIS", "AUTHORITY", NULL };

static const char * const apszVERTCSRule[] =
{ "VERT_CS", "VERT_DATUM", "UNIT", "AXIS", "AUTHORITY", NULL };

static const char * const *apszOrderingRules[] = {
	apszPROJCSRule, apszGEOGCSRule, apszDATUMRule, apszGEOCCSRule, apszVERTCSRule, NULL };

/**
* Correct parameter ordering to match CT Specification.
*
* Some mechanisms to create WKT using OGRSpatialReference, and some
* imported WKT fail to maintain the order of parameters required according
* to the BNF definitions in the OpenGIS SF-SQL and CT Specifications.  This
* method attempts to massage things back into the required order.
*
* This method will reorder the children of the node it is invoked on and
* then recurse to all children to fix up their children.
*
* @return OGRERR_NONE on success or an error code if something goes
* wrong.
*/

bool SRSNode::fixupOrdering()

{
	int    i, k;
	/* -------------------------------------------------------------------- */
	/*      Recurse ordering children.                                      */
	/* -------------------------------------------------------------------- */
	for (i = 0; i < getChildCount(); i++)
		getChild(i)->fixupOrdering();

	if (getChildCount() < 3)
		return true;

	/* -------------------------------------------------------------------- */
	/*      Is this a node for which an ordering rule exists?               */
	/* -------------------------------------------------------------------- */
	const char * const * papszRule = NULL;

	for (i = 0; apszOrderingRules[i] != NULL; i++)
	{
		if (cslIEqualString(apszOrderingRules[i][0], pszValue))
		{
			papszRule = apszOrderingRules[i] + 1;
			break;
		}
	}

	if (papszRule == NULL)
		return true;

	/* -------------------------------------------------------------------- */
	/*      If we have a rule, apply it.  We create an array                */
	/*      (panChildPr) with the priority code for each child (derived     */
	/*      from the rule) and we then bubble sort based on this.           */
	/* -------------------------------------------------------------------- */
	int  *panChildKey = (int *)malloc(sizeof(int)*getChildCount());
	memset(panChildKey, 0, sizeof(int)*getChildCount());
	for (i = 1; i < getChildCount(); i++)
	{
		//panChildKey[i] = CSLFindString((char**)papszRule,
		//	getChild(i)->getValue());
		const char * pszTarget = getChild(i)->getValue();
		if (papszRule == 0) {
			panChildKey[i] = -1;
		}
		else {
			for (k = 0; papszRule[k] != NULL; k++) {
				if (cslIEqualString(papszRule[k], pszTarget)) {
					panChildKey[i] = k;
					break;
				}
				else {
					panChildKey[i] = -1;
				}
			}
		}		
		if (panChildKey[i] == -1)
		{
			printf_s("SpatialReference: Found unexpected key %s when trying to order SRS nodes.",getChild(i)->getValue());
		}
	}

	/* -------------------------------------------------------------------- */
	/*      Sort - Note we don't try to do anything with the first child    */
	/*      which we assume is a name string.                               */
	/* -------------------------------------------------------------------- */
	int j, bChange = TRUE;

	for (i = 1; bChange && i < getChildCount() - 1; i++)
	{
		bChange = FALSE;
		for (j = 1; j < getChildCount() - i; j++)
		{
			if (panChildKey[j] == -1 || panChildKey[j + 1] == -1)
				continue;

			if (panChildKey[j] > panChildKey[j + 1])
			{
				SRSNode *poTemp = papoChildNodes[j];
				int          nKeyTemp = panChildKey[j];

				papoChildNodes[j] = papoChildNodes[j + 1];
				papoChildNodes[j + 1] = poTemp;

				nKeyTemp = panChildKey[j];
				panChildKey[j] = panChildKey[j + 1];
				panChildKey[j + 1] = nKeyTemp;

				bChange = TRUE;
			}
		}
	}

	free(panChildKey);

	return true;
}



end_gdb_namespace
end_gtl_namespace