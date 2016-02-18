#include "csl.h" 

begin_gtl_namespace
begin_gdb_namespace

//////////////////////////////////////////////////////////////////////
////////////////////C StringList functions
/**********************************************************************
*                       cslAddString()
*
* Append a string to a StringList and return a pointer to the modified
* StringList.
* If the input StringList is NULL, then a new StringList is created.
* Note that CSLAddString performance when building a list is in O(n^2)
* which can cause noticable slow down when n > 10000.
**********************************************************************/
CSTRLIST cslAddString(CSTRLIST papszStrList, const char *pszNewString)
{
	int nItems = 0;

	if (pszNewString == NULL)
		return papszStrList;    /* Nothing to do!*/

								/* Allocate room for the new string */
	if (papszStrList == NULL)
		papszStrList = (char**)calloc(2, sizeof(char*));
	else
	{
		nItems = cslCount(papszStrList);
		papszStrList = (char**)realloc(papszStrList,
			(nItems + 2)*sizeof(char*));
	}

	/* Copy the string in the list */
	papszStrList[nItems] = cslDuplicateString(pszNewString);
	papszStrList[nItems + 1] = NULL;

	return papszStrList;
}

/************************************************************************/
/*                              cslCount()                              */
/************************************************************************/

/**
* Return number of items in a string list.
*
* Returns the number of items in a string list, not counting the
* terminating NULL.  Passing in NULL is safe, and will result in a count
* of zero.
*
* Lists are counted by iterating through them so long lists will
* take more time than short lists.  Care should be taken to avoid using
* cslCount() as an end condition for loops as it will result in O(n^2)
* behavior.
*
* @param papszStrList the string list to count.
*
* @return the number of entries.
*/
int cslCount(CSTRLIST papszStrList)
{
	int nItems = 0;

	if (papszStrList)
	{
		while (*papszStrList != NULL)
		{
			nItems++;
			papszStrList++;
		}
	}

	return nItems;
}

/************************************************************************/
/*                            cslGetString()                             */
/*                                                                      */
/*      Fetches the indicated field, being careful not to crash if      */
/*      the field doesn't exist within this string list.  The           */
/*      returned pointer should not be freed, and doesn't               */
/*      necessarily last long.                                          */
/************************************************************************/

const char * cslGetString(CSTRLIST papszStrList, int iField)
{
	int         i;

	if (papszStrList == NULL || iField < 0)
		return("");

	for (i = 0; i < iField + 1; i++)
	{
		if (papszStrList[i] == NULL)
			return "";
	}

	return(papszStrList[iField]);
}
/**
* Find a string within a string list.
*
* Returns the index of the entry in the string list that contains the
* target string.  The string in the string list must be a full match for
* the target, but the search is case insensitive.
*
* @param papszList the string list to be searched.
* @param pszTarget the string to be searched for.
*
* @return the index of the string within the list or -1 on failure.
*/

int cslIFindString(CSTRLIST papszList, const char * pszTarget)
{
	int         i;

	if (papszList == NULL)
		return -1;

	for (i = 0; papszList[i] != NULL; i++)
	{
		if (cslIEqualString(papszList[i], pszTarget))
			return i;
	}

	return -1;
}

int cslFindString(CSTRLIST papszList, const char * pszTarget)
{
	int         i;

	if (papszList == NULL)
		return -1;

	for (i = 0; papszList[i] != NULL; i++)
	{
		if (cslEqualString(papszList[i], pszTarget))
			return i;
	}

	return -1;
}
/************************************************************************/
/*                             CSLDestroy()                             */
/************************************************************************/

/**
* Free string list.
*
* Frees the passed string list (null terminated array of strings).
* It is safe to pass NULL.
*
* @param papszStrList the list to free.
*/
void cslDestroy(CSTRLIST papszStrList)
{
	char **papszPtr;

	if (papszStrList)
	{
		papszPtr = papszStrList;
		while (*papszPtr != NULL)
		{
			free(*papszPtr);
			papszPtr++;
		}

		free(papszStrList);
		papszStrList = 0;
	}
}

/************************************************************************/
/*                            cslDuplicate()                            */
/************************************************************************/

/**
* Clone a string list.
*
* Efficiently allocates a copy of a string list.  The returned list is
* owned by the caller and should be freed with cslDestroy().
*
* @param papszStrList the input string list.
*
* @return newly allocated copy.
*/

CSTRLIST cslDuplicate(CSTRLIST papszStrList)
{
	char **papszNewList, **papszSrc, **papszDst;
	int  nLines;

	nLines = cslCount(papszStrList);

	if (nLines == 0)
		return NULL;

	papszNewList = (char **)malloc((nLines + 1)*sizeof(char*));
	papszSrc = papszStrList;
	papszDst = papszNewList;

	while (*papszSrc != NULL)
	{
		*papszDst = cslDuplicateString(*papszSrc);

		papszSrc++;
		papszDst++;
	}
	*papszDst = NULL;

	return papszNewList;
}

char * cslDuplicateString(const char *pszString) {
	char   *pszReturn;
	if (pszString == NULL)
		pszString = "";
	pszReturn = (char *)malloc(strlen(pszString) + 1);
	if (pszReturn == NULL)
		return NULL;
	strcpy(pszReturn, pszString);
	return(pszReturn);
}
/************************************************************************/
/*                         cslTokenizeString()                         */
/************************************************************************/

/**
* Tokenize a string.
*
* This function will split a string into tokens based on specified'
* delimeter(s) with a variety of options.  The returned result is a
* string list that should be freed with CSLDestroy() when no longer
* needed.
*
* The available parsing options are:
*
* - CSLT_ALLOWEMPTYTOKENS: allow the return of empty tokens when two
* delimiters in a row occur with no other text between them.  If not set,
* empty tokens will be discarded;
* - CSLT_STRIPLEADSPACES: strip leading space characters from the token (as
* reported by isspace());
* - CSLT_STRIPENDSPACES: strip ending space characters from the token (as
* reported by isspace());
* - CSLT_HONOURSTRINGS: double quotes can be used to hold values that should
* not be broken into multiple tokens;
* - CSLT_PRESERVEQUOTES: string quotes are carried into the tokens when this
* is set, otherwise they are removed;
* - CSLT_PRESERVEESCAPES: if set backslash escapes (for backslash itself,
* and for literal double quotes) will be preserved in the tokens, otherwise
* the backslashes will be removed in processing.
*
* \b Example:
*
* Parse a string into tokens based on various white space (space, newline,
* tab) and then print out results and cleanup.  Quotes may be used to hold
* white space in tokens.

\code
char **papszTokens;
int i;

papszTokens =
CSLTokenizeString( pszCommand, " \t\n",
CSLT_HONOURSTRINGS | CSLT_ALLOWEMPTYTOKENS );

for( i = 0; papszTokens != NULL && papszTokens[i] != NULL; i++ )
printf( "arg %d: '%s'", papszTokens[i] );
CSLDestroy( papszTokens );
\endcode

* @param pszString the string to be split into tokens.
* @param pszDelimiters one or more characters to be used as token delimeters.
* @param nCSLTFlags an ORing of one or more of the CSLT_ flag values.
*
* @return a string list of tokens owned by the caller.
*/

CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int nCSLTFlags)
{
	if (pszString == NULL)
		return (char **)calloc(sizeof(char *), 1);
	char ** oRetList;
	char        *pszToken;
	int         nTokenMax, nTokenLen;
	int         bHonourStrings = (nCSLTFlags & CSLT_HONOURSTRINGS);
	int         bAllowEmptyTokens = (nCSLTFlags & CSLT_ALLOWEMPTYTOKENS);
	int         bStripLeadSpaces = (nCSLTFlags & CSLT_STRIPLEADSPACES);
	int         bStripEndSpaces = (nCSLTFlags & CSLT_STRIPENDSPACES);

	pszToken = (char *)calloc(10, 1);
	nTokenMax = 10;

	while (pszString != NULL && *pszString != '\0')
	{
		int     bInString = FALSE;
		int     bStartString = TRUE;

		nTokenLen = 0;

		/* Try to find the next delimeter, marking end of token */
		for (; *pszString != '\0'; pszString++)
		{

			/* End if this is a delimeter skip it and break. */
			if (!bInString && strchr(pszDelimiters, *pszString) != NULL)
			{
				pszString++;
				break;
			}

			/* If this is a quote, and we are honouring constant
			strings, then process the constant strings, with out delim
			but don't copy over the quotes */
			if (bHonourStrings && *pszString == '"')
			{
				if (nCSLTFlags & CSLT_PRESERVEQUOTES)
				{
					pszToken[nTokenLen] = *pszString;
					nTokenLen++;
				}

				if (bInString)
				{
					bInString = FALSE;
					continue;
				}
				else
				{
					bInString = TRUE;
					continue;
				}
			}

			/*
			* Within string constants we allow for escaped quotes, but in
			* processing them we will unescape the quotes and \\ sequence
			* reduces to \
			*/
			if (bInString && pszString[0] == '\\')
			{
				if (pszString[1] == '"' || pszString[1] == '\\')
				{
					if (nCSLTFlags & CSLT_PRESERVEESCAPES)
					{
						pszToken[nTokenLen] = *pszString;
						nTokenLen++;
					}

					pszString++;
				}
			}

			/*
			* Strip spaces at the token start if requested.
			*/
			if (!bInString && bStripLeadSpaces
				&& bStartString && isspace((unsigned char)*pszString))
				continue;

			bStartString = FALSE;

			/*
			* Extend token buffer if we are running close to its end.
			*/
			if (nTokenLen >= nTokenMax - 3)
			{
				nTokenMax = nTokenMax * 2 + 10;
				pszToken = (char *)realloc(pszToken, nTokenMax);
			}

			pszToken[nTokenLen] = *pszString;
			nTokenLen++;
		}

		/*
		* Strip spaces at the token end if requested.
		*/
		if (!bInString && bStripEndSpaces)
		{
			while (nTokenLen && isspace((unsigned char)pszToken[nTokenLen - 1]))
				nTokenLen--;
		}

		pszToken[nTokenLen] = '\0';

		/*
		* Add the token.
		*/
		if (pszToken[0] != '\0' || bAllowEmptyTokens)
			cslAddString(oRetList, pszToken);
	}

	/*
	* If the last token was empty, then we need to capture
	* it now, as the loop would skip it.
	*/
	if (*pszString == '\0' && bAllowEmptyTokens && cslCount(oRetList) > 0
		&& strchr(pszDelimiters, *(pszString - 1)) != NULL)
	{
		cslAddString(oRetList, "");
	}

	free(pszToken);

	if (oRetList == NULL)
	{
		// we prefer to return empty lists as a pointer to 
		// a null pointer since some client code might depend on this.
		oRetList = (char**)calloc(sizeof(char**), 1);
	}

	return oRetList;
}

CSTRLIST cslTokenizeString(const char * pszString, const char * pszDelimiters, int bHonourStrings, int bAllowEmptyTokens) {
	int         nFlags = 0;

	if (bHonourStrings)
		nFlags |= CSLT_HONOURSTRINGS;
	if (bAllowEmptyTokens)
		nFlags |= CSLT_ALLOWEMPTYTOKENS;

	return cslTokenizeString(pszString, pszDelimiters, nFlags);
}
/**********************************************************************
*                       cslTokenizeString()
*
* Tokenizes a string and returns a StringList with one string for
* each token.
**********************************************************************/
CSTRLIST cslTokenizeString(const char *pszString)
{
	return cslTokenizeString(pszString, " ", CSLT_HONOURSTRINGS);
}

bool cslEqualString(const char * s1, const char * s2) {
	return strcmp(s1, s2) == 0;
}
bool cslIEqualString(const char * s1, const char * s2) {
	return stricmp(s1, s2) == 0;
}
bool cslNIEqualString(const char * s1, const char * s2, int n) {
	return strnicmp(s1, s2, n);
}
bool cslNEqualString(const char * s1, const char * s2, int n) {
	return strncmp(s1, s2, n);
}
/************************************************************************/
/*                              cslCopyString()                            */
/************************************************************************/

/**
* Copy source string to a destination buffer.
*
* This function ensures that the destination buffer is always NUL terminated
* (provided that its length is at least 1).
*
* This function is designed to be a safer, more consistent, and less error
* prone replacement for strncpy. Its contract is identical to libbsd's strlcpy.
*
* Truncation can be detected by testing if the return value of CPLStrlcpy
* is greater or equal to nDestSize.

\verbatim
char szDest[5];
if (cslCopyString(szDest, "abcde", sizeof(szDest)) >= sizeof(szDest))
fprintf(stderr, "truncation occured !\n");
\endverbatim

* @param pszDest   destination buffer
* @param pszSrc    source string. Must be NUL terminated
* @param nDestSize size of destination buffer (including space for the NUL terminator character)
*
* @return the length of the source string (=strlen(pszSrc))
*
*/
size_t cslCopyString(char* pszDest, const char* pszSrc, size_t nDestSize)
{
	char* pszDestIter = pszDest;
	const char* pszSrcIter = pszSrc;

	if (nDestSize == 0)
		return strlen(pszSrc);

	nDestSize--;
	while (nDestSize != 0 && *pszSrcIter != '\0')
	{
		*pszDestIter = *pszSrcIter;
		pszDestIter++;
		pszSrcIter++;
		nDestSize--;
	}
	*pszDestIter = '\0';
	return pszSrcIter - pszSrc + strlen(pszSrcIter);
}

/************************************************************************/
/*                              cslConcatenateString()                            */
/************************************************************************/

/**
* Appends a source string to a destination buffer.
*
* This function ensures that the destination buffer is always NUL terminated
* (provided that its length is at least 1 and that there is at least one byte
* free in pszDest, that is to say strlen(pszDest_before) < nDestSize)
*
* This function is designed to be a safer, more consistent, and less error
* prone replacement for strncat. Its contract is identical to libbsd's strlcat.
*
* Truncation can be detected by testing if the return value of CPLStrlcat
* is greater or equal to nDestSize.

\verbatim
char szDest[5];
cslConcatenateString(szDest, "ab", sizeof(szDest));
if (cslConcatenateString(szDest, "cde", sizeof(szDest)) >= sizeof(szDest))
fprintf(stderr, "truncation occured !\n");
\endverbatim

* @param pszDest   destination buffer. Must be NUL terminated before running CPLStrlcat
* @param pszSrc    source string. Must be NUL terminated
* @param nDestSize size of destination buffer (including space for the NUL terminator character)
*
* @return the thoretical length of the destination string after concatenation
*         (=strlen(pszDest_before) + strlen(pszSrc)).
*         If strlen(pszDest_before) >= nDestSize, then it returns nDestSize + strlen(pszSrc)
*
*/
size_t cslConcatenateString(char* pszDest, const char* pszSrc, size_t nDestSize)
{
	char* pszDestIter = pszDest;

	while (nDestSize != 0 && *pszDestIter != '\0')
	{
		pszDestIter++;
		nDestSize--;
	}

	return pszDestIter - pszDest + cslCopyString(pszDestIter, pszSrc, nDestSize);
}

/**********************************************************************
*                       CSLAddNameValue()
*
* Add a new entry to a StringList of "Name=Value" pairs,
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* This function does not check if a "Name=Value" pair already exists
* for that name and can generate multiple entryes for the same name.
* Use CSLSetNameValue() if you want each name to have only one value.
*
* Returns the modified stringlist.
**********************************************************************/
CSTRLIST cslAddNameValue(CSTRLIST papszStrList, const char *pszName, const char *pszValue)
{
	char *pszLine;

	if (pszName == NULL || pszValue == NULL)
		return papszStrList;

	pszLine = (char *)malloc(strlen(pszName) + strlen(pszValue) + 2);
	sprintf(pszLine, "%s=%s", pszName, pszValue);
	papszStrList = cslAddString(papszStrList, pszLine);
	free(pszLine);

	return papszStrList;
}
/**
* Assign value to name in StringList.
*
* Set the value for a given name in a StringList of "Name=Value" pairs
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* If there is already a value for that name in the list then the value
* is changed, otherwise a new "Name=Value" pair is added.
*
* @param papszList the original list, the modified version is returned.
* @param pszName the name to be assigned a value.  This should be a well
* formed token (no spaces or very special characters).
* @param pszValue the value to assign to the name.  This should not contain
* any newlines (CR or LF) but is otherwise pretty much unconstrained.  If
* NULL any corresponding value will be removed.
*
* @return modified stringlist.
*/

CSTRLIST cslSetNameValue(CSTRLIST papszList, const char *pszName, const char *pszValue)
{
	char **papszPtr;
	size_t nLen;

	if (pszName == NULL)
		return papszList;

	nLen = strlen(pszName);
	papszPtr = papszList;
	while (papszPtr && *papszPtr != NULL)
	{
		if (cslNIEqualString(*papszPtr, pszName, nLen)
			&& ((*papszPtr)[nLen] == '=' ||
				(*papszPtr)[nLen] == ':'))
		{
			/* Found it!
			* Change the value... make sure to keep the ':' or '='
			*/
			char cSep;
			cSep = (*papszPtr)[nLen];

			free(*papszPtr);

			/*
			* If the value is NULL, remove this entry completely/
			*/
			if (pszValue == NULL)
			{
				while (papszPtr[1] != NULL)
				{
					*papszPtr = papszPtr[1];
					papszPtr++;
				}
				*papszPtr = NULL;
			}

			/*
			* Otherwise replace with new value.
			*/
			else
			{
				*papszPtr = (char *)malloc(strlen(pszName) + strlen(pszValue) + 2);
				sprintf(*papszPtr, "%s%c%s", pszName, cSep, pszValue);
			}
			return papszList;
		}
		papszPtr++;
	}

	if (pszValue == NULL)
		return papszList;

	/* The name does not exist yet... create a new entry
	*/
	return cslAddNameValue(papszList, pszName, pszValue);
}

/**********************************************************************
*                       cslFetchNameValue()
*
* In a StringList of "Name=Value" pairs, look for the
* first value associated with the specified name.  The search is not
* case sensitive.
* ("Name:Value" pairs are also supported for backward compatibility
* with older stuff.)
*
* Returns a reference to the value in the StringList that the caller
* should not attempt to free.
*
* Returns NULL if the name is not found.
**********************************************************************/
const char *cslFetchNameValue(CSTRLIST papszStrList, const char *pszName, bool insensetive)
{
	size_t nLen;

	if (papszStrList == NULL || pszName == NULL)
		return NULL;

	nLen = strlen(pszName);
	bool b = false;
	while (*papszStrList != NULL)
	{
		if (insensetive)
			b = cslNIEqualString(*papszStrList, pszName, nLen);
		else
			b= cslNEqualString(*papszStrList, pszName, nLen);

		if (b && ((*papszStrList)[nLen] == '=' ||
				(*papszStrList)[nLen] == ':'))
		{
			return (*papszStrList) + nLen + 1;
		}
		papszStrList++;
	}
	return NULL;
}
/**********************************************************************
*                       cslParseNameValue()
**********************************************************************/

/**
* Parse NAME=VALUE string into name and value components.
*
* Note that if ppszKey is non-NULL, the key (or name) portion will be
* allocated using VSIMalloc(), and returned in that pointer.  It is the
* applications responsibility to free this string, but the application should
* not modify or free the returned value portion.
*
* This function also support "NAME:VALUE" strings and will strip white
* space from around the delimeter when forming name and value strings.
*
* Eventually CSLFetchNameValue() and friends may be modified to use
* CPLParseNameValue().
*
* @param pszNameValue string in "NAME=VALUE" format.
* @param ppszKey optional pointer though which to return the name
* portion.
* @return the value portion (pointing into original string).
*/

const char *cslParseNameValue(const char *pszNameValue, char **ppszKey)

{
	int  i;
	const char *pszValue;

	for (i = 0; pszNameValue[i] != '\0'; i++)
	{
		if (pszNameValue[i] == '=' || pszNameValue[i] == ':')
		{
			pszValue = pszNameValue + i + 1;
			while (*pszValue == ' ' || *pszValue == '\t')
				pszValue++;

			if (ppszKey != NULL)
			{
				*ppszKey = (char *)malloc(i + 1);
				strncpy(*ppszKey, pszNameValue, i);
				(*ppszKey)[i] = '\0';
				while (i > 0 &&
					((*ppszKey)[i] == ' ' || (*ppszKey)[i] == '\t'))
				{
					(*ppszKey)[i] = '\0';
					i--;
				}
			}

			return pszValue;
		}
	}

	return NULL;
}
/************************************************************************/
/*                            cslIFindName()                             */
/************************************************************************/

/**
* Find StringList entry with given key name.
*
* @param papszStrList the string list to search.
* @param pszName the key value to look for (case insensitive).
*
* @return -1 on failure or the list index of the first occurance
* matching the given key.
*/

int cslIFindName(CSTRLIST papszStrList, const char *pszName)
{
	size_t nLen;
	int    iIndex = 0;

	if (papszStrList == NULL || pszName == NULL)
		return -1;

	nLen = strlen(pszName);
	while (*papszStrList != NULL)
	{
		if (cslNIEqualString(*papszStrList, pszName, nLen)
			&& ((*papszStrList)[nLen] == '=' ||
				(*papszStrList)[nLen] == ':'))
		{
			return iIndex;
		}
		iIndex++;
		papszStrList++;
	}
	return -1;
}

/************************************************************************/
/*                          cslStringToNumber()                            */
/************************************************************************/

/**
* Converts ASCII string to floating point number using specified delimiter.
*
* This function converts the initial portion of the string pointed to
* by nptr to double floating point representation. This function does the
* same as standard strtod(3), but does not take locale in account. Instead of
* locale defined decimal delimiter you can specify your own one. Also see
* notes for CPLAtof() function.
*
* @param nptr Pointer to string to convert.
* @param endptr If is not NULL, a pointer to the character after the last
* character used in the conversion is stored in the location referenced
* by endptr.
* @param point Decimal delimiter.
*
* @return Converted value, if any.
*/
double cslStringToNumber(const char *nptr, char **endptr, char point)
{
	while (*nptr == ' ')
		nptr++;

	if (nptr[0] == '-')
	{
		if (strcmp(nptr, "-1.#QNAN") == 0 ||
			strcmp(nptr, "-1.#IND") == 0)
			return NAN;

		if (strcmp(nptr, "-inf") == 0 ||
			strcmp(nptr, "-1.#INF") == 0)
			return -INFINITY;
	}
	else if (nptr[0] == '1')
	{
		if (strcmp(nptr, "1.#QNAN") == 0)
			return NAN;
		if (strcmp(nptr, "1.#INF") == 0)
			return INFINITY;
	}
	else if (nptr[0] == 'i' && strcmp(nptr, "inf") == 0)
		return INFINITY;
	else if (nptr[0] == 'n' && strcmp(nptr, "nan") == 0)
		return NAN;

	/* -------------------------------------------------------------------- */
	/*  We are implementing a simple method here: copy the input string     */
	/*  into the temporary buffer, replace the specified decimal delimiter  */
	/*  with the one, taken from locale settings and use standard strtod()  */
	/*  on that buffer.                                                     */
	/* -------------------------------------------------------------------- */
	double      dfValue;
	int         nError;

	char*       pszNumber = cslReplacePointByLocalePoint(nptr, point);

	dfValue = strtod(pszNumber, endptr);
	nError = errno;

	if (endptr)
		*endptr = (char *)nptr + (*endptr - pszNumber);

	if (pszNumber != (char*)nptr)
		free(pszNumber);

	errno = nError;
	return dfValue;
}
double cslStringToNumber(const char *nptr) {
	return cslStringToNumber(nptr, 0, '.');
}
/**
* Scan up to a maximum number of characters from a string and convert
* the result to a long.
*
* @param pszString String containing characters to be scanned. It may be
* terminated with a null character.
*
* @param nMaxLength The maximum number of character to consider as part
* of the number. Less characters will be considered if a null character
* is encountered.
*
* @return Long value, converted from its ASCII form.
*/
long cslStringToInteger(const char *pszString, int nMaxLength )
{
	long    iValue;
	char    *pszValue = (char *)malloc(nMaxLength + 1);

	/* -------------------------------------------------------------------- */
	/*      Compute string into local buffer, and terminate it.             */
	/* -------------------------------------------------------------------- */
	strncpy(pszValue, pszString, nMaxLength);
	pszValue[nMaxLength] = '\0';

	/* -------------------------------------------------------------------- */
	/*      Use atol() to fetch out the result                              */
	/* -------------------------------------------------------------------- */
	iValue = atol(pszValue);

	free(pszValue);
	return iValue;
}

/************************************************************************/
/*                         cslFillString()                         */
/************************************************************************/

/**
* Copy the string pointed to by pszSrc, NOT including the terminating
* `\\0' character, to the array pointed to by pszDest. Remainder of the
* destination string will be filled with fillChar (the dedault is space characters）.
* This is only difference from the PrintString().
*
* @param pszDest Pointer to the destination string buffer. Should be
* large enough to hold the resulting string.
*
* @param pszSrc Pointer to the source buffer.
*
* @param nMaxLen Maximum length of the resulting string. If string length
* is greater than nMaxLen, it will be truncated.
*
* @return Number of characters printed.
*/

int cslFillString(char *pszDest, const char *pszSrc, int nMaxLen, char fillChar)
{
	char    *pszTemp = pszDest;

	if (!pszDest)
		return 0;

	if (!pszSrc)
	{
		memset(pszDest, fillChar, nMaxLen);
		return nMaxLen;
	}

	while (nMaxLen && *pszSrc)
	{
		*pszTemp++ = *pszSrc++;
		nMaxLen--;
	}

	if (nMaxLen)
		memset(pszTemp, fillChar, nMaxLen);

	return nMaxLen;
}

/**
* Converts ASCII string to floating point number using any numeric locale.
*
* This function converts the initial portion of the string pointed to
* by nptr to double floating point representation. This function does the
* same as standard atof(), but it allows a variety of locale representations.
* That is it supports numeric values with either a comma or a period for
* the decimal delimiter.
*
* PS. The M stands for Multi-lingual.
*
* @param nptr The string to convert.
*
* @return Converted value, if any.  Zero on failure.
*/
double cslMultilingualStringToNumber(const char *nptr) {
	int i;
	const static int nMaxSearch = 50;

	for (i = 0; i < nMaxSearch; i++)
	{
		if (nptr[i] == ',')
			return cslStringToNumber(nptr, 0, ',');
		else if (nptr[i] == '.' || nptr[i] == '\0')
			return cslStringToNumber(nptr, 0, '.');
	}

	return cslStringToNumber(nptr, 0, '.');
}
/**
* Test what boolean value contained in the string.
*
* If pszValue is "NO", "FALSE","F", "OFF" or "0" will be returned FALSE.
* Otherwise, TRUE will be returned.
*
* @param pszValue the string should be tested.
*
* @return TRUE or FALSE.
*/

bool cslTestBoolean(const char *pszValue)
{
	if (cslIEqualString(pszValue, "NO")
		|| cslIEqualString(pszValue, "FALSE")
		|| cslIEqualString(pszValue, "F")
		|| cslIEqualString(pszValue, "OFF")
		|| cslIEqualString(pszValue, "0"))
		return FALSE;
	else
		return TRUE;
}
/************************************************************************/
/*                      cslReplacePointByLocalePoint()                  */
/************************************************************************/

char* cslReplacePointByLocalePoint(const char* pszNumber, char point)
{
#if defined(WIN32CE) || defined(__ANDROID__)
	static char byPoint = 0;
	if (byPoint == 0)
	{
		char szBuf[16];
		sprintf(szBuf, "%.1f", 1.0);
		byPoint = szBuf[1];
	}
	if (point != byPoint)
	{
		const char* pszPoint = strchr(pszNumber, point);
		if (pszPoint)
		{
			char* pszNew = CPLStrdup(pszNumber);
			pszNew[pszPoint - pszNumber] = byPoint;
			return pszNew;
		}
	}
#else
	struct lconv *poLconv = localeconv();
	if (poLconv
		&& poLconv->decimal_point
		&& strlen(poLconv->decimal_point) > 0)
	{
		char    byPoint = poLconv->decimal_point[0];

		if (point != byPoint)
		{
			const char* pszPoint = strchr(pszNumber, point);
			if (pszPoint)
			{
				char* pszNew = cslDuplicateString(pszNumber);
				pszNew[pszPoint - pszNumber] = byPoint;
				return pszNew;
			}
		}
	}
#endif
	return (char*)pszNumber;
}

void stringToWString(std::string & s, std::wstring & sd) {
	sd.clear();
	sd.resize(s.length());
	std::wstring::iterator it1 = sd.begin();
	for (std::string::iterator it = s.begin(); it != s.end(); it++, it1++) {
		*it1 = (wchar_t)(*it);
	}
}
//只有处于同一编码方式的情况下才是正确的，如果编码体系不同，则本函数不能正确的执行
void wstringToString(std::wstring & s, std::string & sd) {
	sd.clear();
	for (std::wstring::iterator it = s.begin(); it != s.end(); it++) {
		char h = (char)((*it) >> 8);
		char l = (char)(*it);

		if (h != 0)
			sd.append(1, (char)h);
		sd.append(1, (char)l);
	}
}



///////////////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////////////

end_gdb_namespace
end_gtl_namespace



