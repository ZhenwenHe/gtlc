#include "globalfuns.h"
#include <gl\gl.h>
#include <gl\glu.h>
#include <math.h>  
#include "sphere3d.h" 
#include "matrix4x4.h"


begin_gtl_namespace
begin_gdb_namespace
/*
* The CPLString class is derived from std::string, so the vast majority
* of the implementation comes from that.  This module is just the extensions
* we add.
*/

/************************************************************************/
/*                               printf()                               */
/************************************************************************/

String & String::printf(const char *pszFormat, ...)

{
	va_list args;

	va_start(args, pszFormat);
	vprintf(pszFormat, args);
	va_end(args);

	return *this;
}
/************************************************************************/
/*                              vprintf()                               */
/************************************************************************/

String &String::vprintf(const char *pszFormat, va_list args)

{
	/* -------------------------------------------------------------------- */
	/*      This implementation for platforms without vsnprintf() will      */
	/*      just plain fail if the formatted contents are too large.        */
	/* -------------------------------------------------------------------- */

#if !defined(HAVE_VSNPRINTF)
	char *pszBuffer = (char *)malloc(30000);
	if (vsprintf(pszBuffer, pszFormat, args) > 29998)
	{
		std::cerr<<"String::vprintf() ... buffer overrun.";
	}
	*this = pszBuffer;
	free(pszBuffer);

	/* -------------------------------------------------------------------- */
	/*      This should grow a big enough buffer to hold any formatted      */
	/*      result.                                                         */
	/* -------------------------------------------------------------------- */
#else
	char szModestBuffer[500];
	int nPR;
	va_list wrk_args;

#ifdef va_copy
	va_copy(wrk_args, args);
#else
	wrk_args = args;
#endif

	nPR = vsnprintf(szModestBuffer, sizeof(szModestBuffer), pszFormat,
		wrk_args);
	if (nPR == -1 || nPR >= (int) sizeof(szModestBuffer) - 1)
	{
		int nWorkBufferSize = 2000;
		char *pszWorkBuffer = (char *)malloc(nWorkBufferSize);

#ifdef va_copy
		va_end(wrk_args);
		va_copy(wrk_args, args);
#else
		wrk_args = args;
#endif
		while ((nPR = vsnprintf(pszWorkBuffer, nWorkBufferSize, pszFormat, wrk_args))
			>= nWorkBufferSize - 1
			|| nPR == -1)
		{
			nWorkBufferSize *= 4;
			pszWorkBuffer = (char *)realloc(pszWorkBuffer,
				nWorkBufferSize);
#ifdef va_copy
			va_end(wrk_args);
			va_copy(wrk_args, args);
#else
			wrk_args = args;
#endif
		}
		*this = pszWorkBuffer;
		free(pszWorkBuffer);
	}
	else
	{
		*this = szModestBuffer;
	}
	va_end(wrk_args);
#endif

	return *this;
}
/************************************************************************/
/*                              format()                               */
/************************************************************************/

/**
* Format double in C locale.
*
* The passed value is formatted using the C locale (period as decimal
* seperator) and appended to the target String.
*
* @param dfValue the value to format.
* @param pszFormat the sprintf() style format to use or omit for default.
* Note that this format string should only include one substitution argument
* and it must be for a double (%f or %g).
*
* @return a reference to the CPLString.
*/

String &String::format(double dfValue, const char *pszFormat)

{
	if (pszFormat == NULL)
		pszFormat = "%g";

	char szWork[512]; // presumably long enough for any number?

	sprintf(szWork, pszFormat, dfValue);
	assert(strlen(szWork) < sizeof(szWork));

	if (strchr(szWork, ',') != NULL)
	{
		char *pszDelim = strchr(szWork, ',');
		*pszDelim = '.';
	}

	*this += szWork;

	return *this;
}

/************************************************************************/
/*                                trim()                                */
/************************************************************************/

/**
* Trim white space.
*
* Trims white space off the let and right of the string.  White space
* is any of a space, a tab, a newline ('\n') or a carriage control ('\r').
*
* @return a reference to the String.
*/

String &String::trim()
{
	size_t iLeft, iRight;
	static const char szWhitespace[] = " \t\r\n";

	iLeft = find_first_not_of(szWhitespace);
	iRight = find_last_not_of(szWhitespace);

	if (iLeft == std::string::npos)
	{
		erase();
		return *this;
	}

	assign(substr(iLeft, iRight - iLeft + 1));

	return *this;
}

/************************************************************************/
/*                               recode()                               */
/************************************************************************/

String &String::recode(const char *pszSrcEncoding,
	const char *pszDstEncoding)

{
	/*if (pszSrcEncoding == NULL)
		pszSrcEncoding = CPL_ENC_UTF8;
	if (pszDstEncoding == NULL)
		pszDstEncoding = CPL_ENC_UTF8;

	if (strcmp(pszSrcEncoding, pszDstEncoding) == 0)
		return *this;

	char *pszRecoded = CPLRecode(c_str(),
		pszSrcEncoding,
		pszDstEncoding);

	if (pszRecoded == NULL)
		return *this;

	assign(pszRecoded);
	free(pszRecoded);*/

	return *this;
}

/************************************************************************/
/*                               ifind()                                */
/************************************************************************/

/**
* Case insensitive find() alternative.
*
* @param str substring to find.
* @param pos offset in the string at which the search starts.
* @return the position of substring in the string or std::string::npos if not found.
* @since GDAL 1.9.0
*/

size_t String::ifind(const std::string & str, size_t pos) const

{
	return ifind(str.c_str(), pos);
}

/**
* Case insensitive find() alternative.
*
* @param s substring to find.
* @param nPos offset in the string at which the search starts.
* @return the position of the substring in the string or std::string::npos if not found.
* @since GDAL 1.9.0
*/

size_t String::ifind(const char *s, size_t nPos) const

{
	const char *pszHaystack = c_str();
	char chFirst = (char) ::tolower(s[0]);
	int nTargetLen = strlen(s);

	if (nPos > size())
		nPos = size();

	pszHaystack += nPos;

	while (*pszHaystack != '\0')
	{
		if (chFirst == ::tolower(*pszHaystack))
		{
			if (cslNIEqualString(pszHaystack, s, nTargetLen))
				return nPos;
		}

		nPos++;
		pszHaystack++;
	}

	return std::string::npos;
}

/************************************************************************/
/*                              toupper()                               */
/************************************************************************/

/**
* Convert to upper case in place.
*/

String &String::toupper()

{
	size_t i;

	for (i = 0; i < size(); i++)
		(*this)[i] = (char) ::toupper((*this)[i]);

	return *this;
}

/************************************************************************/
/*                              tolower()                               */
/************************************************************************/

/**
* Convert to lower case in place.
*/

String &String::tolower()

{
	size_t i;

	for (i = 0; i < size(); i++)
		(*this)[i] = (char) ::tolower((*this)[i]);

	return *this;
}

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

CSTRLIST cslTokenizeString(const char * pszString,	const char * pszDelimiters,	int nCSLTFlags)
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
			cslAddString(oRetList,pszToken);
	}

	/*
	* If the last token was empty, then we need to capture
	* it now, as the loop would skip it.
	*/
	if (*pszString == '\0' && bAllowEmptyTokens && cslCount(oRetList) > 0
		&& strchr(pszDelimiters, *(pszString - 1)) != NULL)
	{
		cslAddString(oRetList,"");
	}

	free(pszToken);

	if (oRetList == NULL)
	{
		// we prefer to return empty lists as a pointer to 
		// a null pointer since some client code might depend on this.
		oRetList=(char**)calloc(sizeof(char**), 1);
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

    nDestSize --;
    while(nDestSize != 0 && *pszSrcIter != '\0')
    {
        *pszDestIter = *pszSrcIter;
        pszDestIter ++;
        pszSrcIter ++;
        nDestSize --;
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

    while(nDestSize != 0 && *pszDestIter != '\0')
    {
        pszDestIter ++;
        nDestSize --;
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
CSTRLIST cslAddNameValue(CSTRLIST papszStrList,	const char *pszName, const char *pszValue)
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

CSTRLIST cslSetNameValue(CSTRLIST papszList,const char *pszName, const char *pszValue)
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
const char *cslFetchNameValue(CSTRLIST papszStrList, const char *pszName)
{
	size_t nLen;

	if (papszStrList == NULL || pszName == NULL)
		return NULL;

	nLen = strlen(pszName);
	while (*papszStrList != NULL)
	{
		if (cslNIEqualString(*papszStrList, pszName, nLen)
			&& ((*papszStrList)[nLen] == '=' ||
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


//读取一行，也即遇到13和10停止
const char * readLine(FILE * fp) {	 
	std::string result; 
	if (readLine(fp, result) > 0)
		return result.c_str();
	else
		return 0;
}
size_t readLine(FILE * fp, std::string & result) {
	int c = fgetc(fp);
	char buf[512];
	int i = 0;
	std::string sz;
	while (c != 10 && c != 13 && !feof(fp)) {
		buf[i++] = (char)c;
		if (i == 512) {
			sz.assign(buf, 512);
			result = result + sz;
			i = 0;
		}
		c = fgetc(fp);
	}
	if (i > 0 && i < 512) {
		sz.assign(buf, i);
		result = result + sz;
	}
	return result.size();
}
/////////////////////////////////////////////////////////////////////

bool doubleEqual(double d1, double d2) {
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
}

bool floatEqual(float d1, float d2) {
	if (fabs(d1 - d2) < TOLERANCE)
		return true;
	else
		return false;
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

void getCurTime(std::string & sz){
	time_t now;
	time(&now);
	tm   t;
	localtime_s(&t, &now);
	sz = numberToString<char, long>(t.tm_year + 1900) + "-" + numberToString<char, long>(t.tm_mon + 1) + "-" + numberToString<char, long>(t.tm_mday) + " " + numberToString<char, long>(t.tm_hour) + ":" + numberToString<char, long>(t.tm_min) + ":" + numberToString<char, long>(t.tm_sec);
}

std::string getCurTime(){
	std::string   sz;
	getCurTime(sz);
	return sz;
}
///////////////////////////////////////////////////////////////////////////////////
bool IsCCW(Vector3d * pdData, unsigned int * ps);
void GetConcavePos(Vector3d * pdData, std::vector<int> * rPS,
	std::vector<int> * cPos);
bool IsPartOfPolygon(unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData,
	std::vector<int> * cPos);
int GetIntersectOfTwoLine(Vector3d A, Vector3d B,
	Vector3d C, Vector3d D,
	Vector3d P);
double GetDistance(const Vector3d A, const Vector3d B);
double Get_Angle_C_Of_ABC(Vector3d A, Vector3d B, Vector3d C);
void SetAtoB(Vector3d B, const Vector3d A);
void GetTriNormal(double dCoef[4], const Vector3d A, const Vector3d B,
	const Vector3d C, const bool bNormal);
double GetAverage(std::vector<double>& daZ, std::vector<double>* paAlpha);
//-------------------------------------------------------------------------
//*************************************************************************
// 矢量运算
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AaddB
// 功    能:	矢量加法 Vr = A + B
// 参    数:	const Vector3d A, const Vector3d B, Vector3d C
//   (入口):	A: 矢量A
//				B: 矢量B
//				C: 矢量C
//   (出口):	C: 矢量C
// 返    回:	无
// 调用方法:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0},C;
//				AaddB(A, B, C);
//				则 C = {3.0, 5.0, 7.0}
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void AaddB(const Vector3d A, const Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	C[0] = A[0] + B[0];
	C[1] = A[1] + B[1];
	C[2] = A[2] + B[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AsubB
// 功    能:	矢量减法 Vr = A - B
// 参    数:	const Vector3d A, const Vector3d B, Vector3d C
//   (入口):	A: 矢量A
//				B: 矢量B
//				C: 矢量C
//   (出口):	C: 矢量C
// 返    回:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0}, C;
//				AsubB(A, B, C);
//				则 C = {-3.0, -3.0, -3.0}
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void AsubB(const Vector3d A, const Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	C[0] = A[0] - B[0];
	C[1] = A[1] - B[1];
	C[2] = A[2] - B[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AdotB
// 功    能:	矢量点积 Vr = A●B
// 参    数:	const Vector3d A, const Vector3d B
//   (入口):	A: 矢量A
//				B: 矢量B
//   (出口):	无
// 返    回:	double: 矢量点积
// 调用方法:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0};
//				double dRes = AdotB(A, B);
//				则 dRes = 14.0
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double AdotB(const Vector3d A, const Vector3d B)
////GeoView_Tech_Mark_End
{
	double dRtn = A[0] * B[0] +
		A[1] * B[1] +
		A[2] * B[2];
	return dRtn;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AxB
// 功    能:	矢量叉积 Vr = A×B
// 参    数:	const Vector3d A, const Vector3d B, Vector3d V
//   (入口):	A: 矢量A
//				B: 矢量B
//				V: 矢量V
//   (出口):	V: 矢量V
// 返    回:	无
// 调用方法:	Vector3d A = {0.0, 1.0, 2.0},
//						B = {3.0, 4.0, 5.0}, V;
//				AxB(A, B, V);
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void AxB(const Vector3d A, const Vector3d B, Vector3d V)
////GeoView_Tech_Mark_End
{
	V[0] = A[1] * B[2] - A[2] * B[1];
	V[1] = A[2] * B[0] - A[0] * B[2];
	V[2] = A[0] * B[1] - A[1] * B[0];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Vec
// 功    能:	坐标向量化
// 参    数:	const double r, const double s, const double t, Vector3d A
//   (入口):	r: X坐标分量
//				s: Y坐标分量
//				t: Z坐标分量
//				A: 向量
//   (出口):	A: 向量,且三个分量分别为r, s, t
// 返    回:	无
// 调用方法:	double r = 1.0, s = 2.0, t = 3.0;
//				Vector3d A;
//				Vec(r, s, t, A);
//				则 A = {1.0, 2.0, 3.0}
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Vec(const double r, const double s, const double t, Vector3d A)
////GeoView_Tech_Mark_End
{
	A[0] = r;
	A[1] = s;
	A[2] = t;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	UnVec
// 功    能:	向量坐标化
// 参    数:	const Vector3d A, double *r, double *s, double *t
//   (入口):	A: 向量
//				r: X向分量指针
//				s: Y向分量指针
//				t: Z向分量指针
//   (出口):	r: X向分量指针
//				s: Y向分量指针
//				t: Z向分量指针
// 返    回:	无
// 调用方法:	double r = 0.0, s = 0.0, t = 0.0;
//				Vector3d A = {1.0, 2.0, 3.0};
//				UnVec(A, &r, &s, &t);
//				则 r = 1.0, s = 2.0, t = 3.0
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void UnVec(const Vector3d A, double *r, double *s, double *t)
////GeoView_Tech_Mark_End
{
	*r = A[0];
	*s = A[1];
	*t = A[2];
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecLen
// 功    能:	矢量长度
// 参    数:	const Vector3d A
//   (入口):	A: 矢量
//   (出口):	无
// 返    回:	double: 矢量的长度(欧氏空间)
// 调用方法:	Vector3d A = {1.0, 2.0, 2.0};
//				double dLength = VecLen(A);
//				则 dLength = 3.0
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double VecLen(const Vector3d A)
////GeoView_Tech_Mark_End
{
	return sqrt(A[0] * A[0] + A[1] * A[1] + A[2] * A[2]);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecNormalize
// 功    能:	矢量归一化
// 参    数:	Vector3d A
//   (入口):	A: 矢量
//   (出口):	A: 归一化后的矢量
// 返    回:	double: 原矢量的长度
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double VecNormalize(Vector3d A)
////GeoView_Tech_Mark_End
{
	double dist = 0.0, invdist = 0.0;

	dist = VecLen(A);
	if (dist>0.0)
	{
		invdist = 1.0 / dist;
		A[0] *= invdist;
		A[1] *= invdist;
		A[2] *= invdist;
	}
	return dist;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecNormalize
// 功    能:	矢量归一化
// 参    数:	Vertex3d &A
//   (入口):	A: 矢量
//   (出口):	A: 归一化后的矢量
// 返    回:	double: 原矢量的长度
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double VecNormalize(Vertex3d &A)
////GeoView_Tech_Mark_End
{
	double invdist, dist = sqrt(A.x*A.x + A.y*A.y + A.z*A.z);
	if (dist>0.0)
	{
		invdist = 1 / dist;
		A.x *= invdist;
		A.y *= invdist;
		A.z *= invdist;
	}
	return dist;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecCopy
// 功    能:	矢量拷贝
// 参    数:	const Vector3d A, Vector3d B
//   (入口):	A:	源矢量
//				B:	目标矢量(返回)
//   (出口):	B:	目标矢量
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-29, Wengzp
// 修改日期:	
//==================================================================//
void VecCopy(const Vector3d A, Vector3d B)
////GeoView_Tech_Mark_End
{
	//	B[0] = A[0];
	//	B[1] = A[1];
	//	B[2] = A[2];
	memcpy(B, A, sizeof(Vector3d));
}

//求取两个矢量的夹角
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecAngle
// 功    能:	求取两个矢量的夹角
// 参    数:	const Vector3d A, Vector3d B
//   (入口):	A:	源矢量
//				B:	源矢量
//   (出口):	
// 返    回:	两矢量的夹角
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-29, Wengzp
// 修改日期:	
//==================================================================//
double VecAngle(Vector3d A, Vector3d B)
////GeoView_Tech_Mark_End
{
	double lfRgn = AdotB(A, B);
	double lfLA = VecLen(A);
	double lfLB = VecLen(B);
	double cosa = lfRgn / (lfLA*lfLB);
	return acos(cosa);
}
//-------------------------------------------------------------------------
//*************************************************************************
// 三角函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Radiansf
// 功    能:	角度转为弧度
// 参    数:	double dDegree
//   (入口):	dDegree: 角度(单位: 度)
//   (出口):	无
// 返    回:	double: 转化后的弧度值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Radiansf(double dDegree)
////GeoView_Tech_Mark_End
{
	return dDegree*PI / 180;
}

////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Degreesf
// 功    能:	弧度转为角度
// 参    数:	double dRadian
//   (入口):	dRadian: 角度(单位: 弧度)
//   (出口):	无
// 返    回:	double: 转化后的角度值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Degreesf(double dRadian)
////GeoView_Tech_Mark_End
{
	return dRadian*PI / 180;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Cosf
// 功    能:	角度的余弦
// 参    数:	double dDegree
//   (入口):	dDegree: 角度(单位: 度)
//   (出口):	无
// 返    回:	double: 余弦值
// 调用方法:	double dRes = Cosf(60.0);
//				则 dRes = 0.5;
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Cosf(double dDegree)
////GeoView_Tech_Mark_End
{
	return cos(Radiansf(dDegree));
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Sinf
// 功    能:	角度的正弦
// 参    数:	double dDegree
//   (入口):	dDegree: 角度(单位: 度)
//   (出口):	无
// 返    回:	double: 余弦值
// 调用方法:	double dRes = Sinf(30.0);
//				则 dRes = 0.5;
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Sinf(double dDegree)
////GeoView_Tech_Mark_End
{
	return sin(Radiansf(dDegree));
}

// 四舍五入
int Roundf(double dX)
{
	if (dX<0)
	{
		return (int)(dX - 0.5);
	}
	else
	{
		return (int)(dX + 0.5);
	}
}

//-------------------------------------------------------------------------
//*************************************************************************
// 插值运算
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Linear2D
// 功    能:	平面线性插值
// 参    数:	double x[], double y[], double tx[], double ty[], int n, int m
//   (入口):	n: 结点数
//				m: 待插值点数
//				x[]: 一维数组, 结点的X坐标值( X0 < X1 < X2 < ... < Xn-1 )
//				y[]: 一维数组, 结点的Y坐标值
//				tx[]: 一维数组, 待插值点的X坐标值
//				ty[]: 一维数组, 待插值点的Y坐标值(返回)
//   (出口):	ty[]: 一维数组, 待插值点的Y坐标值
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Linear2D(double x[], double y[], double tx[], double ty[], int n, int m)
////GeoView_Tech_Mark_End
{
	for (int i = 0, k = 0; i<m; i++)
	{
		k = 0;
		if (tx[i] >= x[n - 1]) k = n - 2;
		else
		{
			while (tx[i]>x[k + 1]) k++;
		}
		ty[i] = (y[k] * (x[k + 1] - tx[i]) + y[k + 1] * (tx[i] - x[k])) / (x[k + 1] - x[k]);
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Linear3D
// 功    能:	三维线性插值
// 参    数:	double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m
//   (入口):	n:	结点的总数
//				m:	每段的分段数
//				x[]: 一维数组, 结点的X坐标值
//				y[]: 一维数组, 结点的Y坐标值
//				z[]: 一维数组, 结点的Z坐标值
//				tx[]: 一维数组, 待插值点(包括结点)的X坐标值(返回)
//				ty[]: 一维数组, 待插值点(包括结点)的Y坐标值(返回)
//				tz[]: 一维数组, 待插值点(包括结点)的Z坐标值(返回)
//   (出口):	tx[]: 一维数组, 待插值点(包括结点)的X坐标值
//				ty[]: 一维数组, 待插值点(包括结点)的Y坐标值
//				tz[]: 一维数组, 待插值点(包括结点)的Z坐标值
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	以每个结点在折线上的距离D为因变量, X, Y, Z 分别作为D的函数进行
//				二维的线性插值; 另外, 待插值点总数是这样确定的: 比如 n 个结点, 
//				则有(n-1)段, 对每段平均分成 m 段, 则待插值点总数为 m(n-1)+1
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Linear3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
////GeoView_Tech_Mark_End
{
	int nTotal, i, j, k;
	double *dis, *disT, deltaDis = 0.0f;

	nTotal = m*(n - 1) + 1;
	dis = (double*)malloc(n*sizeof(double));
	disT = (double*)malloc(nTotal*sizeof(double));
	dis[0] = disT[0] = 0.0f;

	for (i = 1, k = 1; i<n; i++)
	{
		deltaDis = sqrt((x[i] - x[i - 1])*(x[i] - x[i - 1]) +
			(y[i] - y[i - 1])*(y[i] - y[i - 1]) +
			(z[i] - z[i - 1])*(z[i] - z[i - 1]));
		dis[i] = dis[i - 1] + deltaDis;
		deltaDis = deltaDis / m;

		for (j = 1; j <= m; j++, k++)
		{
			disT[k] = disT[k - 1] + deltaDis;
		}
	}

	Linear2D(dis, x, disT, tx, n, nTotal);
	Linear2D(dis, y, disT, ty, n, nTotal);
	Linear2D(dis, z, disT, tz, n, nTotal);

	free(dis);
	free(disT);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetPartsPoint
// 功    能:	获取线的等份点
// 参    数:	std::vector<Vertex3d> &Line, std::vector<Vertex3d> &Parts, unsigned int dwParts = 1
//   (入口):	Line:		线数据
//				Parts:		返回的等份点数据
//				dwParts:	等份数
//   (出口):	Parts:		返回的等份点数据
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2005-10-11, Wengzp: add notes.
// 修改日期:	
//==================================================================//
void GetPartsPoint(std::vector<Vertex3d> &Line, std::vector<Vertex3d> &Parts, unsigned int dwParts)
////GeoView_Tech_Mark_End
{
	unsigned int dwLine = Line.size();
	double dLength = 0.0, // 线条的长度
		dDist = 0.0;	// 线条的等份距离
	unsigned int i = 0, j = 0;
	std::vector<double> daLength;	// 线条的每段长度

	if (dwLine<2 || dwParts<1)
	{
		return;
	}

	// 首先清空返回的数据
	Parts.clear();
	Parts.resize(dwParts + 1);	// 等分点(比等分数多1)	
	daLength.resize(dwLine - 1);	// 每段的长度

	// 计算总长与每段长度
	for (i = 0; i<dwLine - 1; i++)
	{
		daLength[i] = sqrt((Line[i].x - Line[i + 1].x)*(Line[i].x - Line[i + 1].x) +
			(Line[i].y - Line[i + 1].y)*(Line[i].y - Line[i + 1].y) +
			(Line[i].z - Line[i + 1].z)*(Line[i].z - Line[i + 1].z));
		dLength += daLength[i];
	}
	dDist = dLength / dwParts;	// 等分距离

	// 填充数据
	double dL = daLength[0], dD = dDist;
	Parts[0] = Line[0];
	for (i = 1, j = 0; i<dwParts; i++)
	{
		while (dD>dL)
		{
			j++;
			dL += daLength[j];
		}
		Parts[i].x = (Line[j].x*(dL - dD) + Line[j + 1].x*(daLength[j] - dL + dD)) / daLength[j];
		Parts[i].y = (Line[j].y*(dL - dD) + Line[j + 1].y*(daLength[j] - dL + dD)) / daLength[j];
		Parts[i].z = (Line[j].z*(dL - dD) + Line[j + 1].z*(daLength[j] - dL + dD)) / daLength[j];
		dD += dDist;
	}
	Parts[dwParts] = Line[dwLine - 1];


	daLength.clear();
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Spline2D
// 功    能:	平面样条插值
// 参    数:	double x[], double y[], double tx[], double ty[], int n, int m
//   (入口):	n: 结点数
//				m: 待插值点数
//				x[]: 一维数组, 结点的X坐标值( X0 < X1 < X2 < ... < Xn-1 )
//				y[]: 一维数组, 结点的Y坐标值
//				tx[]: 一维数组, 待插值点的X坐标值(返回)
//				ty[]: 一维数组, 待插值点的Y坐标值(返回)
//   (出口):	tx[]: 一维数组, 待插值点的X坐标值
//				ty[]: 一维数组, 待插值点的Y坐标值
// 返    回:	double: 结点X轴围成的区域面积(积分)
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:	徐士良. C常用算法程序集(Edition 2). 北京: 清华大学出版社.1996
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Spline2D(double x[], double y[], double t[], double z[], int n, int m)
////GeoView_Tech_Mark_End
{
	int i, j;
	double h0, h1, alpha, beta, g, *s, *dy, *ddy, *dz, *ddz;

	if (n == 2)/// 如果结点为两个, 则直接线性插值
	{
		Linear2D(x, y, t, z, n, m);
		return 0.0;
	}

	// 变量的意义:
	// g  :	函数在 [X0, Xn-1] 上的积分(围成的面积)
	// dy :	函数在结点处的一阶导数值
	// ddy: 函数在结点处的二阶导数值
	// dz : 函数在待插值点处的一阶导数值
	// ddz: 函数在待插值点处的二阶导数值
	// 在徐中是作为输出的, 由于实际应用时, 没有必要知道导数值, 因此内含不输出
	s = (double*)malloc(n*sizeof(double));
	dy = (double*)malloc(n*sizeof(double));
	ddy = (double*)malloc(n*sizeof(double));
	// 原则上, 第二种边界条件中ddy[0]与ddy[n-1]是用户给定的, 本程序为了方便计算, 
	//		设为0, 表示是光滑的, 这可以满足一般的曲线的样条光滑问题的条件
	ddy[0] = 0.0f;
	ddy[n - 1] = 0.0f;
	dz = (double*)malloc(m*sizeof(double));
	ddz = (double*)malloc(m*sizeof(double));

	dy[0] = -0.5;
	h0 = x[1] - x[0];
	s[0] = 3.0f*(y[1] - y[0]) / (2.0f*h0) - ddy[0] * h0 / 4.0f;
	for (j = 1; j <= n - 2; j++)
	{
		h1 = x[j + 1] - x[j];
		alpha = h0 / (h0 + h1);
		beta = (1.0f - alpha)*(y[j] - y[j - 1]) / h0;
		beta = 3.0f*(beta + alpha*(y[j + 1] - y[j]) / h1);
		dy[j] = -alpha / (2.0f + (1.0f - alpha)*dy[j - 1]);
		s[j] = (beta - (1.0f - alpha)*s[j - 1]);
		s[j] = s[j] / (2.0f + (1.0f - alpha)*dy[j - 1]);
		h0 = h1;
	}
	dy[n - 1] = (3.0f*(y[n - 1] - y[n - 2]) / h1 + ddy[n - 1] * h1 / 2.0f - s[n - 2]) / (2.0f + dy[n - 2]);
	for (j = n - 2; j >= 0; j--)
		dy[j] = dy[j] * dy[j + 1] + s[j];
	for (j = 0; j <= n - 2; j++)
		s[j] = x[j + 1] - x[j];
	for (j = 0; j <= n - 2; j++)
	{
		h1 = s[j] * s[j];
		ddy[j] = 6.0f*(y[j + 1] - y[j]) / h1 - 2.0f*(2.0f*dy[j] + dy[j + 1]) / s[j];
	}
	h1 = s[n - 2] * s[n - 2];
	ddy[n - 1] = 6.f*(y[n - 2] - y[n - 1]) / h1 + 2.f*(2.f*dy[n - 1] + dy[n - 2]) / s[n - 2];
	g = 0.0;
	for (i = 0; i <= n - 2; i++)
	{
		h1 = 0.5f*s[i] * (y[i] + y[i + 1]);
		h1 = h1 - s[i] * s[i] * s[i] * (ddy[i] + ddy[i + 1]) / 24.0f;
		g = g + h1;
	}
	for (j = 0; j <= m - 1; j++)
	{
		if (t[j] >= x[n - 1]) i = n - 2;
		else
		{
			i = 0;
			while (t[j]>x[i + 1]) i = i + 1;
		}
		h1 = (x[i + 1] - t[j]) / s[i];
		h0 = h1*h1;
		z[j] = (3.0f*h0 - 2.0f*h0*h1)*y[i];
		z[j] = z[j] + s[i] * (h0 - h0*h1)*dy[i];
		dz[j] = 6.0f*(h0 - h1)*y[i] / s[i];
		dz[j] = dz[j] + (3.0f*h0 - 2.0f*h1)*dy[i];
		ddz[j] = (6.0f - 12.0f*h1)*y[i] / (s[i] * s[i]);
		ddz[j] = ddz[j] + (2.0f - 6.0f*h1)*dy[i] / s[i];
		h1 = (t[j] - x[i]) / s[i];
		h0 = h1*h1;
		z[j] = z[j] + (3.0f*h0 - 2.0f*h0*h1)*y[i + 1];
		z[j] = z[j] - s[i] * (h0 - h0*h1)*dy[i + 1];
		dz[j] = dz[j] - 6.0f*(h0 - h1)*y[i + 1] / s[i];
		dz[j] = dz[j] + (3.0f*h0 - 2.0f*h1)*dy[i + 1];
		ddz[j] = ddz[j] + (6.0f - 12.0f*h1)*y[i + 1] / (s[i] * s[i]);
		ddz[j] = ddz[j] - (2.0f - 6.0f*h1)*dy[i + 1] / s[i];
	}
	free(s);
	free(dy);
	free(ddy);
	free(dz);
	free(ddz);
	return(g);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Spline3D
// 功    能:	空间样条插值
// 参    数:	double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m
//   (入口):	n:	结点的总数
//				m:	每段的分段数
//				x[]: 一维数组, 结点的X坐标值
//				y[]: 一维数组, 结点的Y坐标值
//				z[]: 一维数组, 结点的Z坐标值
//				tx[]: 一维数组, 待插值点(包括结点)的X坐标值(返回)
//				ty[]: 一维数组, 待插值点(包括结点)的Y坐标值(返回)
//				tz[]: 一维数组, 待插值点(包括结点)的Z坐标值(返回)
//   (出口):	tx[]: 一维数组, 待插值点(包括结点)的X坐标值
//				ty[]: 一维数组, 待插值点(包括结点)的Y坐标值
//				tz[]: 一维数组, 待插值点(包括结点)的Z坐标值
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	以每个结点在折线上的距离D为因变量, X, Y, Z 分别作为D的函数进行二维的
//				样条插值(第二种边界条件)
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Spline3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
////GeoView_Tech_Mark_End
{
	int nTotal, i, j, k;
	double *dis, *disT, deltaDis = 0.0f;

	nTotal = m*(n - 1) + 1;
	dis = (double*)malloc(n*sizeof(double));
	disT = (double*)malloc(nTotal*sizeof(double));
	dis[0] = disT[0] = 0.0f;

	for (i = 1, k = 1; i<n; i++)
	{
		deltaDis = sqrt((x[i] - x[i - 1])*(x[i] - x[i - 1]) +
			(y[i] - y[i - 1])*(y[i] - y[i - 1]) +
			(z[i] - z[i - 1])*(z[i] - z[i - 1]));
		dis[i] = dis[i - 1] + deltaDis;
		deltaDis = deltaDis / m;

		for (j = 1; j <= m; j++, k++)
		{
			disT[k] = disT[k - 1] + deltaDis;
		}
	}

	Spline2D(dis, x, disT, tx, n, nTotal);
	Spline2D(dis, y, disT, ty, n, nTotal);
	Spline2D(dis, z, disT, tz, n, nTotal);

	for (i = 0, j = 0; i<n; i++, j += m)
	{
		tx[j] = x[i];
		ty[j] = y[i];
		tz[j] = z[i];
	}
	free(dis);
	free(disT);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Achol
// 功    能:	平方根法求解对称正定方程组
// 参    数:	double* a, int n, int m, double* d
//   (入口):	a:	n×n数组, 存放系数矩阵(应为对称正定矩阵)
//				n:	方程组的阶数
//				m:	方程组右端常数向量的个数
//				d:	n×m数组, 存放方程组右端m组常数向量
//   (出口):	a:	n×n数组, 上三角部分存放分解后的U矩阵
//				d:	n×m数组, 存放方程组的m组解
// 返    回:	int:	<0: 程序工作失败(矩阵非正定), >0: 正常返回
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	AX = D
// 参    阅:	徐士良.C常用算法程序集(第二版).北京:清华大学出版社, 2001, p17
// 创建日期:	2006-1-17, Wengzp: add notes.
// 修改日期:	
//==================================================================//
int Achol(double* a, int n, int m, double* d)
////GeoView_Tech_Mark_End
{
	int i, j, k, u, v;
	if ((a[0] + 1.0 == 1.0) || (a[0]<0.0))
	{
		return(-2);
	}
	a[0] = sqrt(a[0]);
	for (j = 1; j <= n - 1; j++) a[j] = a[j] / a[0];
	for (i = 1; i <= n - 1; i++)
	{
		u = i*n + i;
		for (j = 1; j <= i; j++)
		{
			v = (j - 1)*n + i;
			a[u] = a[u] - a[v] * a[v];
		}
		if ((a[u] + 1.0 == 1.0) || (a[u]<0.0))
		{
			return(-2);
		}
		a[u] = sqrt(a[u]);
		if (i != (n - 1))
		{
			for (j = i + 1; j <= n - 1; j++)
			{
				v = i*n + j;
				for (k = 1; k <= i; k++)
					a[v] = a[v] - a[(k - 1)*n + i] * a[(k - 1)*n + j];
				a[v] = a[v] / a[u];
			}
		}
	}
	for (j = 0; j <= m - 1; j++)
	{
		d[j] = d[j] / a[0];
		for (i = 1; i <= n - 1; i++)
		{
			u = i*n + i; v = i*m + j;
			for (k = 1; k <= i; k++)
				d[v] = d[v] - a[(k - 1)*n + i] * d[(k - 1)*m + j];
			d[v] = d[v] / a[u];
		}
	}
	for (j = 0; j <= m - 1; j++)
	{
		u = (n - 1)*m + j;
		d[u] = d[u] / a[n*n - 1];
		for (k = n - 1; k >= 1; k--)
		{
			u = (k - 1)*m + j;
			for (i = k; i <= n - 1; i++)
			{
				v = (k - 1)*n + i;
				d[u] = d[u] - a[v] * d[i*m + j];
			}
			v = (k - 1)*n + k - 1;
			d[u] = d[u] / a[v];
		}
	}
	return(2);
}

// 全选主元高斯消去法解线性方程组
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Agauss
// 功    能:	全选主元高斯消去法解线性方程组
// 参    数:	double* a, double* b, int n 
//   (入口):	a:	n×n数组, 存放方程组的系数矩阵, 返回时被破坏
//				b:	一维数组, 长度为n, 存放方程组右端的常数向量
//				n:	方程组的阶数
//   (出口):	a:	n×n数组, 上三角部分存放分解后的矩阵
//				b:	一维数组, 长度为n, 存放方程组的解向量
// 返    回:	int:	0: 失败; 1: 成功
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	徐士良.C常用算法程序集(第二版).北京:清华大学出版社, 2001, p17
// 创建日期:	2006-1-22, zhangdan: add notes.
// 修改日期:	
//==================================================================//
int Agauss(double* a, double* b, int n)
////GeoView_Tech_Mark_End
{
	int *js = NULL, l, k, i, j, is, p, q;
	double d, t;
	js = (int*)malloc(n*sizeof(int));
	l = 1;
	for (k = 0; k <= n - 2; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
		{
			for (j = k; j <= n - 1; j++)
			{
				t = fabs(a[i*n + j]);
				if (t>d)
				{
					d = t; js[k] = j; is = i;
				}
			}
		}
		if (d + 1.0 == 1.0)
			l = 0;
		else
		{
			if (js[k] != k)
				for (i = 0; i <= n - 1; i++)
				{
					p = i*n + k;
					q = i*n + js[k];
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
			if (is != k)
			{
				for (j = k; j <= n - 1; j++)
				{
					p = k*n + j;
					q = is*n + j;
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
				t = b[k]; b[k] = b[is]; b[is] = t;
			}
		}
		if (l == 0)
		{
			free(js);
			printf("fail\n");
			return(0);
		}
		d = a[k*n + k];
		for (j = k + 1; j <= n - 1; j++)
		{
			p = k*n + j;
			a[p] = a[p] / d;
		}
		b[k] = b[k] / d;
		for (i = k + 1; i <= n - 1; i++)
		{
			for (j = k + 1; j <= n - 1; j++)
			{
				p = i*n + j;
				a[p] = a[p] - a[i*n + k] * a[k*n + j];
			}
			b[i] = b[i] - a[i*n + k] * b[k];
		}
	}
	d = a[(n - 1)*n + n - 1];
	if (fabs(d) + 1.0 == 1.0)
	{
		free(js);
		printf("fail\n");
		return(0);
	}
	b[n - 1] = b[n - 1] / d;
	for (i = n - 2; i >= 0; i--)
	{
		t = 0.0;
		for (j = i + 1; j <= n - 1; j++)
		{
			t = t + a[i*n + j] * b[j];
		}
		b[i] = b[i] - t;
	}
	js[n - 1] = n - 1;
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			t = b[k]; b[k] = b[js[k]]; b[js[k]] = t;
		}
	}
	free(js);
	return(1);
}

// 全选主元高斯-约当消去法解线性方程组
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AgaussJordan
// 功    能:	全选主元高斯-约当消去法解线性方程组
// 参    数:	double* a, double* b, int n, int m
//   (入口):	a:	n×n数组, 存放方程组的系数矩阵, 返回时被破坏
//				b:	n×m数组, 存放方程组右端的m组常数向量(一列为一组)
//				n:	方程组的阶数
//				m:	方程组右端常数向量的个数
//   (出口):	a:	n×n数组, 存放方程组的系数矩阵, 返回时被破坏
//				b:	n×m数组, 返回存放的m组解向量
// 返    回:	int:	0: 失败; 1: 成功
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-3-7, Wengzp: add notes.
// 修改日期:	
//==================================================================//
int AgaussJordan(double* a, double* b, int n, int m)
////GeoView_Tech_Mark_End
{
	int *js, l, k, i, j, is, p, q;
	double d, t;
	js = (int*)malloc(n*sizeof(int));
	l = 1;
	for (k = 0; k <= n - 1; k++)
	{
		d = 0.0;
		for (i = k; i <= n - 1; i++)
		{
			for (j = k; j <= n - 1; j++)
			{
				t = fabs(a[i*n + j]);
				if (t>d)
				{
					d = t;
					js[k] = j;
					is = i;
				}
			}
		}
		if (d + 1.0 == 1.0) l = 0;
		else
		{
			if (js[k] != k)
			{
				for (i = 0; i <= n - 1; i++)
				{
					p = i*n + k;
					q = i*n + js[k];
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
			}
			if (is != k)
			{
				for (j = k; j <= n - 1; j++)
				{
					p = k*n + j;
					q = is*n + j;
					t = a[p];
					a[p] = a[q];
					a[q] = t;
				}
				for (j = 0; j <= m - 1; j++)
				{
					p = k*m + j;
					q = is*m + j;
					t = b[p];
					b[p] = b[q];
					b[q] = t;
				}
			}
		}
		if (l == 0)
		{
			free(js);
			printf("fail\n");
			return(0);
		}
		d = a[k*n + k];
		for (j = k + 1; j <= n - 1; j++)
		{
			p = k*n + j;
			a[p] = a[p] / d;
		}
		for (j = 0; j <= m - 1; j++)
		{
			p = k*m + j;
			b[p] = b[p] / d;
		}
		for (j = k + 1; j <= n - 1; j++)
		{
			for (i = 0; i <= n - 1; i++)
			{
				p = i*n + j;
				if (i != k)
					a[p] = a[p] - a[i*n + k] * a[k*n + j];
			}
		}
		for (j = 0; j <= m - 1; j++)
		{
			for (i = 0; i <= n - 1; i++)
			{
				p = i*m + j;
				if (i != k)
					b[p] = b[p] - a[i*n + k] * b[k*m + j];
			}
		}
	}
	for (k = n - 1; k >= 0; k--)
	{
		if (js[k] != k)
		{
			for (j = 0; j <= m - 1; j++)
			{
				p = k*m + j;
				q = js[k] * m + j;
				t = b[p];
				b[p] = b[q];
				b[q] = t;
			}
		}
	}
	free(js);
	return(1);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Isqt2
// 功    能:	多元线性回归分析
// 参    数:	double* x, double* y, int m, int n, double* a, double* dt, double* v
//   (入口):	x:	m×n数组, 每一列存放m个自变量的一组观测值
//				y:	一维数组, 长度为n, 存放随机变量y的n个观测值
//				m:	自变量个数
//				n:	观测数据的组数
//				a:	一维数组, 长度为(m+1), 返回回归系数a0, a1, ..., am-1, am
//				dt:	一维数组, 长度为4, 其中, dt[0]返回偏差平方和q, dt[1]返回平均标准偏差s,
//					dt[2]返回复相关系数r, dt[3]返回回归平方和u
//				v:	一维数组, 长度为m, 返回m个自变量的偏相关系数
//   (出口):	a:	一维数组, 长度为(m+1), 返回回归系数a0, a1, ..., am-1, am
//				dt:	一维数组, 长度为4, 其中, dt[0]返回偏差平方和q, dt[1]返回平均标准偏差s,
//					dt[2]返回复相关系数r, dt[3]返回回归平方和u
//				v:	一维数组, 长度为m, 返回m个自变量的偏相关系数
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	Y = a0X0 + a1X1 + … + am-1Xm-1 + am
// 参    阅:	徐士良.C常用算法程序集(第二版).北京:清华大学出版社, 2001, p234
// 创建日期:	2006-1-17, Wengzp: add notes.
// 修改日期:	
//==================================================================//
void Isqt2(double* x, double* y, int m, int n, double* a, double* dt, double* v)
////GeoView_Tech_Mark_End
{
	int i = 0, j = 0, k = 0, mm = 0;
	double q, e, u, p, yy, s, r, pp, *b;
	b = (double*)malloc((m + 1)*(m + 1)*sizeof(double));
	mm = m + 1;
	b[mm*mm - 1] = n;
	for (j = 0; j <= m - 1; j++)
	{
		p = 0.0;
		for (i = 0; i <= n - 1; i++)
		{
			p = p + x[j*n + i];
		}
		b[m*mm + j] = p;
		b[j*mm + m] = p;
	}
	for (i = 0; i <= m - 1; i++)
	{
		for (j = i; j <= m - 1; j++)
		{
			p = 0.0;
			for (k = 0; k <= n - 1; k++)
			{
				p = p + x[i*n + k] * x[j*n + k];
			}
			b[j*mm + i] = p;
			b[i*mm + j] = p;
		}
	}
	a[m] = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		a[m] = a[m] + y[i];
	}
	for (i = 0; i <= m - 1; i++)
	{
		a[i] = 0.0;
		for (j = 0; j <= n - 1; j++)
		{
			a[i] = a[i] + x[i*n + j] * y[j];
		}
	}
	Achol(b, mm, 1, a);
	yy = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		yy = yy + y[i] / n;
	}
	q = 0.0; e = 0.0; u = 0.0;
	for (i = 0; i <= n - 1; i++)
	{
		p = a[m];
		for (j = 0; j <= m - 1; j++)
		{
			p = p + a[j] * x[j*n + i];
		}
		q = q + (y[i] - p)*(y[i] - p);
		e = e + (y[i] - yy)*(y[i] - yy);
		u = u + (yy - p)*(yy - p);
	}
	s = sqrt(q / n);
	r = sqrt(1.0 - q / e);
	for (j = 0; j <= m - 1; j++)
	{
		p = 0.0;
		for (i = 0; i <= n - 1; i++)
		{
			pp = a[m];
			for (k = 0; k <= m - 1; k++)
			{
				if (k != j) pp = pp + a[k] * x[k*n + i];
			}
			p = p + (y[i] - pp)*(y[i] - pp);
		}
		v[j] = sqrt(1.0 - q / p);
	}
	dt[0] = q; dt[1] = s; dt[2] = r; dt[3] = u;
	free(b); return;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	BHLinker
// 功    能:	钻孔的连接
// 参    数:	unsigned int wNumBH, Vector3d* ptData, std::vector<double> &daX, std::vector<double> &daY, std::vector<double> &daZ
//   (入口):	wNumBH:		钻孔的总数
//				ptData:		钻孔的数据(连接是按照钻孔的先后顺次连接的)
//				daX:		连接线的X坐标数组(返回)
//				daY:		连接线的Y坐标数组(返回)
//				daZ:		连接线的Z坐标数组(返回)
//				bLinear:	是否线性连接(缺省为true)
//				m:			三次样条插值的每段的分段数(缺省为8)
//   (出口):	daX:		连接线的X坐标数组
//				daY:		连接线的Y坐标数组
//				daZ:		连接线的Z坐标数组
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	if 钻孔的数目n≤1, 没必要连接;
//				if n = 2, 则可选择线性连接或者三次样条连接, 并且三次样条连接取此线段的
//					三分点为约束点, 其Z值由IDW法给出(因为线性给出,则 样条结果是线性的);
//				if n≥3, 则可选择线性连接还是三次样条连接(分段数缺省为8)
// 参    阅:
// 创建日期:	2004-3-30, Wengzp
// 修改日期:	
//==================================================================//
void BHLinker(unsigned int wNumBH, Vector3d* ptData, std::vector<double> &daX, std::vector<double> &daY, std::vector<double> &daZ, bool bLinear, int m)
////GeoView_Tech_Mark_End
{
	if (wNumBH <= 0 || ptData == NULL || ((!bLinear) && m<1))
		return;
	if (wNumBH<2) // wNumBH = 1
	{
		daX.push_back(ptData[0][0]);
		daY.push_back(ptData[0][1]);
		daZ.push_back(ptData[0][2]);
		return;
	}
	unsigned int i = 0;
	if (bLinear)	// Linear Linker between boreholes.
	{
		for (i = 0; i<wNumBH; i++)
		{
			daX.push_back(ptData[i][0]);
			daY.push_back(ptData[i][1]);
			daZ.push_back(ptData[i][2]);
		}
	}
	else	// Unlinear linker between boreholes.
	{	//Spline3D(double x[], double y[], double z[], double tx[], double ty[], double tz[], int n, int m)
		double *x = NULL, *y = NULL, *z = NULL, *tx = NULL, *ty = NULL, *tz = NULL;
		if (wNumBH == 2)
		{
			wNumBH = 4;
			x = new double[4];	y = new double[4];	z = new double[4];
			x[0] = ptData[0][0];
			y[0] = ptData[0][1];
			z[0] = ptData[0][2];
			x[3] = ptData[1][0];
			y[3] = ptData[1][1];
			z[3] = ptData[1][2];
			x[1] = (x[0] * 2.0 + x[3]) / 3.0;
			y[1] = (y[0] * 2.0 + y[3]) / 3.0;
			z[1] = (z[0] * 4.0 + z[3]) / 5.0;
			x[2] = (x[0] + x[3] * 2.0) / 3.0;
			y[2] = (y[0] + y[3] * 2.0) / 3.0;
			z[2] = (z[0] + z[3] * 4.0) / 5.0;
		}
		else
		{
			x = new double[wNumBH];	y = new double[wNumBH];	z = new double[wNumBH];
			for (i = 0; i<wNumBH; i++)
			{
				x[i] = ptData[i][0];
				y[i] = ptData[i][1];
				z[i] = ptData[i][2];
			}
		}
		unsigned int wT = m*(wNumBH - 1) + 1;
		tx = new double[wT]; ty = new double[wT]; tz = new double[wT];
		// 三次样条插值
		Spline3D(x, y, z, tx, ty, tz, wNumBH, m);
		// 赋值
		for (i = 0; i<wT; i++)
		{
			daX.push_back(tx[i]);
			daY.push_back(ty[i]);
			daZ.push_back(tz[i]);
		}

		// 释放空间
		if (x)
			delete[] x;
		if (y)
			delete[] y;
		if (z)
			delete[] z;
		if (tx)
			delete[] tx;
		if (ty)
			delete[] ty;
		if (tz)
			delete[] tz;
	}

	return;
}

//-------------------------------------------------------------------------
//*************************************************************************
// 矩阵运算
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	ZeroMatrix
// 功    能:	矩阵清零
// 参    数:	MATRIX4X4 A
//   (入口):	A: 矩阵
//   (出口):	A: 零矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void ZeroMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Initialize the matrix to the following values:
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//		0.0		0.0		0.0		0.0
	//
	int i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++)
			A[i][j] = 0.0;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	DetOfMatrix
// 功    能:	方阵的行列式
// 参    数:	MATRIX4X4 A
//   (入口):	A: 方阵
//   (出口):	无
// 返    回:	double: 方阵A的行列式
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double DetOfMatrix(MATRIX4X4 Src)
////GeoView_Tech_Mark_End
{// 求矩阵(方阵)的行列式
	double dRes = 0.0;

	double a[9];

	for (int j = 0, k = 0, nSign = -1; j<4; j++, k = 0)
	{
		nSign = -nSign;
		for (int m = 1; m<4; m++)
		{
			for (int n = 0; n<4; n++)
			{
				if (n != j)
					a[k++] = Src[m][n];
			}
		}

		dRes += nSign*Src[0][j] * (a[0] * a[4] * a[8] + a[3] * a[7] * a[2] + a[6] * a[5] * a[1]
			- a[2] * a[4] * a[6] - a[1] * a[3] * a[8] - a[0] * a[7] * a[5]);

	}
	return(dRes);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	TraceOfMatrix
// 功    能:	方阵的迹
// 参    数:	MATRIX4X4 A
//   (入口):	A: 方阵
//   (出口):	无
// 返    回:	double: 方阵A的迹
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double TraceOfMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	return(A[0][0] + A[1][1] + A[2][2] + A[3][3]);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	InverseOfMatrix
// 功    能:	求逆矩阵
// 参    数:	MATRIX4X4 Src, MATRIX4X4 InvMatrix
//   (入口):	Src: 方阵
//				InvMatrix: 方阵Src的逆矩阵(返回)
//   (出口):	InvMatrix: 方阵Src的逆矩阵
// 返    回:	double: 方阵Src的行列式(如果为0.0, 则返回的逆矩阵没有意义)
// 调用方法:	MATRIX4X4 Src = {{1.0, 0.0, 0.0, 0.0},
//							   {0.0, 1.0, 0.0, 0.0},
//							   {0.0, 0.0, 1.0, 0.0},
//							   {0.0, 0.0, 0.0, 1.0}}, InvMatrix;
//				double dRes = InverseOfMatrix(Src, InvMatrix);
//				if(dRes>0.0)	// 判断逆矩阵的有效性
//				{
//					double dTrace = TraceOfMatix(InvMatrix);
//				}
//GeoView_Doc_Mark_End  											//
// 主要思路:	求矩阵Src的逆矩阵, 返回矩阵Des的行列式(如 |Src| = 0.0, 则 |Des| = 0.0, 无效)
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double InverseOfMatrix(MATRIX4X4 Src, MATRIX4X4 Des)
////GeoView_Tech_Mark_End
{
	double dDet = DetOfMatrix(Src);
	if (fabs(dDet)<TOLERANCE)  // 行列式为0的矩阵没有逆矩阵
		return(0.0);

	double a[9];
	int k = 0;
	int mSign = 1;
	int nSign = 1;
	for (int i = 0; i<4; i++)
	{
		mSign = -mSign;
		for (int j = 0; j<4; j++)
		{
			nSign = -nSign;
			for (int m = 0; m<4; m++)
			{
				for (int n = 0; n<4; n++)
				{
					if (m != i && n != j)
						a[k++] = Src[m][n];
				}
			}
			k = 0;
			Des[j][i] = mSign*nSign*(a[0] * a[4] * a[8] + a[3] * a[7] * a[2] + a[6] * a[5] * a[1]
				- a[2] * a[4] * a[6] - a[1] * a[3] * a[8] - a[0] * a[7] * a[5]) / dDet;

		}
	}

	return(1.0 / dDet);	// 矩阵与逆矩阵的行列式互为倒数
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	IdentityMatrix
// 功    能:	单位矩阵
// 参    数:	MATRIX4X4 A
//   (入口):	A: 方阵(返回)
//   (出口):	A: 单位矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void IdentityMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Initialize the matrix to the follow values:
	//		1.0		0.0		0.0		0.0
	//		0.0		1.0		0.0		0.0
	//		0.0		0.0		1.0		0.0
	//		0.0		0.0		0.0		1.0
	//
	ZeroMatrix(A);

	for (int i = 0; i<4; i++)
	{
		A[i][i] = 1.0;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Translate3D
// 功    能:	求平移变换矩阵(左乘矩阵)
// 参    数:	double tx, double ty, double tz, MATRIX4X4 A
//   (入口):	tx: 平移变换的X向因子
//				ty: 平移变换的Y向因子
//				tz: 平移变换的Z向因子
//				A:  平移变换矩阵(返回)
//   (出口):	A:  平移变换矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Translate3D(double tx, double ty, double tz, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	//	Translation matrix identified as:
	//		 ----------------
	//		| 1   0   0   Tx |
	//		| 0   1   0   Ty |
	//		| 0   0   1   Tz |
	//		| 0   0   0   1  |
	//		 ----------------
	int i;

	ZeroMatrix(A);
	for (i = 0; i<4; i++)
		A[i][i] = 1.0;

	A[0][3] = tx;
	A[1][3] = ty;
	A[2][3] = tz;
}

// 求比例变换矩阵(左乘矩阵)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Scale3D
// 功    能:	求比例变换矩阵(左乘矩阵)
// 参    数:	double sx, double sy, double sz, MATRIX4X4 A
//   (入口):	sx: 比例变换的X向因子
//				sy: 比例变换的Y向因子
//				sz: 比例变换的Z向因子
//				A:  比例变换矩阵(返回)
//   (出口):	A:  比例变换矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Scale3D(double sx, double sy, double sz, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	// Scaling matrix identified as:
	//		 ----------------
	//		| Sx  0   0   0 |
	//		| 0   Sy  0   0 |
	//		| 0   0   Sz  0 |
	//		| 0   0   0   1 |
	//		 ----------------

	ZeroMatrix(A);
	A[0][0] = sx;
	A[1][1] = sy;
	A[2][2] = sz;
	A[3][3] = 1.0;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Rotate3D
// 功    能:	求旋转变换矩阵(左乘矩阵)
// 参    数:	int m, double Theta, MATRIX4X4 A
//   (入口):	m: 旋转坐标轴代号(0: X轴, 1: Y轴, 2: Z轴)
//				Theta: 旋转角度(单位: 度)
//				A: 旋转变换矩阵(返回)
//   (出口):	A: 旋转变换矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Rotate3D(int m, double Theta, MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	double c, s;

	ZeroMatrix(A);
	c = Cosf(Theta);
	s = Sinf(Theta);

	// Compensate for rounding errors
	if (fabs(c)<TOLERANCE)
		c = 0.0f;
	if (fabs(s)<TOLERANCE)
		s = 0.0f;

	switch (m)
	{
	case 0:
		//	Rotation about the X-Axis matrix identified as:
		//		 -----------------------
		//		| 1     0      0      0 |
		//		| 0     cosX   -sinX  0 |
		//		| 0     sinX   cosX   0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = 1.0;
		A[1][1] = c;
		A[1][2] = -s;
		A[2][1] = s;
		A[2][2] = c;
		A[3][3] = 1.0;
		break;

	case 1:
		//	Rotation about the Y-Axis matrix identified as:
		//		 -----------------------
		//		| cosY  0      sinY   0 |
		//		| 0     1      0      0 |
		//		| -sinY 0      cosY   0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = c;
		A[0][2] = s;
		A[1][1] = 1.0;
		A[2][0] = -s;
		A[2][2] = c;
		A[3][3] = 1.0;
		break;

	case 2:
		//	Rotation about the Z-Axis matrix identified as:
		//		 -----------------------
		//		| cosZ  -sinZ  0      0 |
		//		| sinZ  cosZ   0      0 |
		//		| 0     0      1      0 |
		//		| 0     0      0      1 |
		//		 -----------------------

		A[0][0] = c;
		A[0][1] = -s;
		A[1][0] = s;
		A[1][1] = c;
		A[2][2] = 1.0;
		A[3][3] = 1.0;
		break;
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	MultiplyMatricies
// 功    能:	求矩阵的乘积
// 参    数:	MATRIX4X4 A, MATRIX4X4 B, MATRIX4X4 C
//   (入口):	A: 矩阵, 被乘数
//				B: 矩阵, 乘数
//				C: 矩阵, 乘积(返回)
//   (出口):	C: 乘积矩阵( C = A×B)
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void MultiplyMatricies(MATRIX4X4 A, MATRIX4X4 B, MATRIX4X4 C)
////GeoView_Tech_Mark_End
{
	int   i, j, k;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			for (k = 0, C[i][j] = 0; k<4; k++)
				C[i][j] += A[i][k] * B[k][j];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	MatrixCopy
// 功    能:	矩阵复制
// 参    数:	MATRIX4X4 A, MATRIX4X4 B
//   (入口):	A: 被复制矩阵
//				B: 矩阵(返回)
//   (出口):	B: 矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void MatrixCopy(MATRIX4X4 A, MATRIX4X4 B)
////GeoView_Tech_Mark_End
{
	int  i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
			B[i][j] = A[i][j];
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	TransposeMatrix
// 功    能:	求转置矩阵
// 参    数:	MATRIX4X4 A
//   (入口):	A: 矩阵
//   (出口):	A: 转置后的矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void TransposeMatrix(MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	MATRIX4X4 M;
	int  i, j;

	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++)
			M[j][i] = A[i][j];
	}
	MatrixCopy(M, A);
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	VecTransform
// 功    能:	向量与矩阵的乘法
// 参    数:	Vector3d sx, Vector3d dx, MATRIX4X4 M
//   (入口):	sx: 向量
//				dx: 乘积向量(返回)
//				M:  矩阵
//   (出口):	dx: 乘积向量
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void VecTransform(Vector3d S, Vector3d D, MATRIX4X4 M)
////GeoView_Tech_Mark_End
{
	double x, y, z;

	// Transform the Source vector 'S' by the matrix 'M'
	x = M[0][0] * S[0] + M[0][1] * S[1] + M[0][2] * S[2] + M[0][3];
	y = M[1][0] * S[0] + M[1][1] * S[1] + M[1][2] * S[2] + M[1][3];
	z = M[2][0] * S[0] + M[2][1] * S[1] + M[2][2] * S[2] + M[2][3];

	// Compensate for rounding errors
	if (fabs(x) < TOLERANCE)
		x = 0.0f;
	if (fabs(y) < TOLERANCE)
		y = 0.0f;
	if (fabs(z) < TOLERANCE)
		z = 0.0f;

	D[0] = x;
	D[1] = y;
	D[2] = z;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	glMatrixTo4x4
// 功    能:	OpenGL矩阵化为代数矩阵
// 参    数:	double M[16], MATRIX4X4 A
//   (入口):	M: OpenGL矩阵(实际是16个元素的数组, 且列优先)
//				A: 方阵(返回)
//   (出口):	A: 方阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void glMatrixTo4x4(double M[16], MATRIX4X4 A)
////GeoView_Tech_Mark_End
{
	int i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			A[i][j] = (float)M[(j * 4) + i];
		}
	}
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Matx4x4ToglMatrix
// 功    能:	代数矩阵化为OpenGL矩阵
// 参    数:	MATRIX4X4 A, double M[16]
//   (入口):	A: 方阵
//				M: OpenGL矩阵(返回, 实际是16个元素的数组, 且列优先)
//   (出口):	M: OpenGL矩阵
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Matx4x4ToglMatrix(MATRIX4X4 A, double M[16])
////GeoView_Tech_Mark_End
{
	int i, j;

	for (i = 0; i<4; i++)
	{
		for (j = 0; j<4; j++)
		{
			M[(i * 4) + j] = A[j][i];
		}
	}
}

//-------------------------------------------------------------------------
//*************************************************************************
// 数学计算
// (101) 对非自相交的多边形进行三角剖分
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Triangulate
// 功    能:	对非自相交的多边形进行三角剖分
// 参    数:	Vector3d * pdData, unsigned int * ps, std::vector<int> * pPsArray
//   (入口):	pfData:	Vector3d 型指针变量, 结点数据数组
//				ps: unsigned int 型指针变量, 表示当前多边形对象的拓扑结构, 第 0 个
//						  元素为结点个数, 其后依次为结点在数据中的序号
//				pPsArray: 剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
//   (出口):	pPsArray: 剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void Triangulate(Vector3d * pdData, unsigned int * ps, std::vector<int> * pPsArray)
////GeoView_Tech_Mark_End
{
	// 参数检验
	if ((!pdData) || (!ps) || (!pPsArray))
	{
		return;
	}

	// 只处理有效的填充多边形单元
	if ((ps[0] != 1) || (ps[1]<3))
	{
		return;
	}

	unsigned int i, j, m, n, a, b, c;
	std::vector<int> rPS, cPos;
	bool bResult;

	// 组成当前多边形的节点数
	n = ps[1];

	// 判断多边形的方向，得到逆时针方向数组
	rPS.clear();
	bResult = IsCCW(pdData, ps);
	if (bResult)
	{
		m = n + 2;
		for (i = 2; i<m; i++)
		{
			rPS.push_back(ps[i]);
		}
	}
	else
	{
		for (i = n + 1; i>1; i--)
		{
			rPS.push_back(ps[i]);
		}
	}

	// 定位凹点，取得凹点在节点数组中的位置
	cPos.clear();
	GetConcavePos(pdData, &rPS, &cPos);
	m = cPos.size();
	if (m == n)
	{
		m = 0;
	}

	while (m>0)
	{
		m = cPos.size();
		n = rPS.size();
		// 每次取 (i-1, i, i+1) 这三个点进行处理
		for (i = 0; i<n; i++)
		{
			// 判断当前是否为凹点
			a = 0;
			b = rPS[i];
			for (j = 0; j<m; j++)
			{
				if (b == cPos[j])
				{
					a = 1;
					break;
				}
			}
			// 如果当前点为凹点
			if (a == 1)
			{
				continue;
			}
			// 剥离三角形 ABC：A--rPS[i]，B--rPS[i-1]，C--rPS[i+1]
			// 削去三角形 ABC 的前提：ABC 完全属于多边形
			a = rPS[i];
			if (i == 0)
			{
				b = rPS[n - 1];
				c = rPS[1];
			}
			else if (i == n - 1)
			{
				b = rPS[n - 2];
				c = rPS[0];
			}
			else
			{
				b = rPS[i - 1];
				c = rPS[i + 1];
			}
			if (IsPartOfPolygon(a, b, c, pdData, &cPos) == true)
			{
				pPsArray->push_back(a);
				pPsArray->push_back(c);
				pPsArray->push_back(b);

				//.RemorPSveAt(i);
				rPS.erase(rPS.begin() + i);
				n--;
				i--;

				if (n<4)
				{
					break;
				}
			}
		}

		// 重新定位凹点
		m = 0;
		if (n>3)
		{
			a = cPos.size();
			cPos.clear();
			GetConcavePos(pdData, &rPS, &cPos);
			m = cPos.size();

			if (a == m)
			{
				m = 0;
			}
		}
	}

	// 如果为凸多边形，或是凹多边形的处理结果
	while (n>2)
	{
		for (i = 1; i<n; i++)
		{
			pPsArray->push_back(rPS[i - 1]);
			pPsArray->push_back(rPS[i]);
			pPsArray->push_back(rPS[i + 1]);

			//rPS.RemoveAt(i);
			rPS.erase(rPS.begin() + i);
			n--;
			i--;

			if (n<3)
			{
				return;
			}
		}
	}
}

// 对非自相交的多边形进行三角剖分
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Triangulate
// 功    能:	对非自相交的多边形进行三角剖分
// 参    数:	std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris
//   (入口):	data:	多边形的顶点数据列
//				waPs:	多边形的拓扑结构, 缺省为 [0, 1, 2, 3, ...]
//				waTris:	剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
//   (出口):	waTris:	剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-3-4, Wengzp: add notes.
// 修改日期:	
//==================================================================//
void Triangulate(std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris)
////GeoView_Tech_Mark_End
{
	int nSize = data.size(), nPs = waPs.size();
	if (nSize<3 || (nPs>0 && nPs<5))
	{
		return;
	}
	int i = 0;
	if (nPs == 0)
	{
		for (i = 0; i<nSize; i++)
		{
			waPs.push_back(i);
		}
	}

	Vector3d *pdData = new Vector3d[nSize];
	unsigned int *ps = new unsigned int[nPs + 2];
	for (i = 0; i<nSize; i++)
	{
		pdData[i][0] = data[i].x;	pdData[i][1] = data[i].y;	pdData[i][2] = data[i].z;
	}

	ps[0] = 1;
	ps[1] = nPs;
	for (i = 0; i<nPs; i++)
	{
		ps[i + 2] = waPs[i];
	}
	Triangulate(pdData, ps, &waTris);

	if (pdData)
		delete[] pdData;
	pdData = 0;
	if (ps)
		delete[] ps;
	ps = 0;
}

// 对带洞的2.5维多边形的三角剖分
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Triangulate2
// 功    能:	对带洞的2.5维多边形的三角剖分
// 参    数:	std::vector<Vertex3d>& data, std::vector<int>& waPs, std::vector<int>& waTris
//   (入口):	data:	多边形的顶点数据
//				waPs:	多边形的拓扑结构, 第0为多边形的个数, 其次顺次为外边界的点数以及序号, 再次为
//						内边界的点数以及序号
//				waTris:	剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
//   (出口):	waTris:	剖分后的三角形拓扑结构数组(每三个元素表示一个三角形)
// 返    回:	无
// 调用方法:	
/*////////////////
CGeoMine3DDoc* pDoc = GetDocument();
ASSERT_VALID(pDoc);

std::vector<Vertex3d> dataXYZ, data;
std::vector<int> waPs, waTris;
CGV3dObject* pObj = NULL;
CGV3dLayer* pCurLayer = pDoc->m_Project.GetCurrentLayer();
CObList* pList = pDoc->m_pSelList;
POSITION pos = pList->GetHeadPosition();
unsigned int k=0, wLines = 0;
waPs.push_back(0);
while (pos)
{
pObj = (CGV3dObject*)pList->GetNext(pos);
if(pObj && pObj->GetClass() == CLASS_GV3D_POLYLINE)
{
((CGV3dPolyline*)pObj)->GetAll(data);
dataXYZ.Append(data);
wLines++;
unsigned int wSize = data.size();
waPs.push_back(wSize);
for(unsigned int i=0; i<wSize; i++, k++)
{
waPs.push_back(k);
}
}
}
if(dataXYZ.size()==0)
{
return;
}
waPs[0] = wLines;

Triangulate2(dataXYZ, waPs, waTris);
int nSize = waTris.size();
if(nSize>0)
{
for(int i=nSize-3; i>=0; i-=3)
{
waTris.InsertAt(i, 3);
}
waTris.InsertAt(0, nSize/3);

// 创建多边形
CGV3dPolygon* pGon = new CGV3dPolygon;
pGon->SetAll(dataXYZ);
pGon->SetPolygons(waTris);
pGon->SetColor(255, 0, 0);
pCurLayer->AddObject(pGon);
}
Invalidate(false);
/*//////////
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-3-4, Wengzp: add notes.
// 修改日期:	
//==================================================================//
void Triangulate2(std::vector<Vertex3d>& dataXYZ, std::vector<int>& waPs, std::vector<int>& waTris)
////GeoView_Tech_Mark_End
{
	/*///////////////////////////////////////////////////
	*************   要求内外边界的方向相反 **************
	外边界扩展法: 通过查找外边界与内边界的可见边(只要找到一条就可以了)
	使外边界不断扩展,以致最后只有外边界了, 再调用任意多边形的三角剖分即可.
	对于限制线 ????????
	waPs:	[0]: 多少条边界
	[1]: 外边界的点数
	[2]... : 外边界的序号
	//*//////////////////////////////////////////////////
	std::vector<Vertex3d> data(dataXYZ.size());
	std::copy(dataXYZ.begin(), dataXYZ.end(), data.begin());

	unsigned int nSize = data.size(), nPs = waPs.size();
	if (nSize<3 || nPs<5)
	{
		return;
	}

	unsigned int i = 0, j = 0, k = 0, m = 0, n = 0, nPosOut = 0, nPosIn = 0, nSizeIn = 0, nSizeOut = 0;
	std::vector<int> waOut, *pwaIn = NULL;			// 外边界
	std::vector< std::vector<int>* > arrIns;	// 内边界数组
	for (i = 0; i<nSize; i++)
	{
		data[i].z = 0.0;// 变成XOY平面上
	}

	for (i = 0, k = 2; i<waPs[1]; i++)// 初始外边界
	{
		waOut.push_back(waPs[k++]);
	}
	if (waPs[0]>1)
	{// 存在内边界, 则获取全部的内边界
		arrIns.resize(waPs[0] - 1);
		for (i = 1; i<waPs[0]; i++)
		{
			nSizeIn = waPs[k++];
			std::vector<int>* pwaIn = new std::vector<int>;
			pwaIn->resize(nSizeIn);
			for (j = 0; j<nSizeIn; j++)
			{
				(*pwaIn)[j] = waPs[k++];
			}
			arrIns[i - 1] = pwaIn;
		}
	}

	// 通过查找可见边的方法扩展外边界
	Vector3d A = { 0.0, 0.0, 0.0 }, B = { 0.0, 0.0, 0.0 }, C = { 0.0, 0.0, 0.0 }, D = { 0.0, 0.0, 0.0 },
		P = { 0.0, 0.0, 0.0 }, v1 = { 0.0, 0.0, 0.0 }, v2 = { 0.0, 0.0, 0.0 },
		v0 = { 0.0, 0.0, 0.0 }, v21 = { 0.0, 0.0, 0.0 }, v20 = { 0.0, 0.0, 0.0 };
	int nInterType = 0;
	nSize = arrIns.size();
	while (nSize>0)
	{
		pwaIn = arrIns[0];
		nSizeIn = pwaIn->size();
		nSizeOut = waOut.size();
		// 搜索可见边法扩展外边界
		bool bVisiable = true;
		for (i = 0; i<nSizeOut; i++)
		{
			A[0] = data[waOut[i]].x, A[1] = data[waOut[i]].y;
			for (j = 0; j<nSizeIn; j++)
			{
				bVisiable = true;
				B[0] = data[(*pwaIn)[j]].x, B[1] = data[(*pwaIn)[j]].y;
				AsubB(B, A, v0);

				// 先比较与外边界的可见性
				waOut.push_back(waOut[0]);// 构成封闭的
				for (m = 0; m<nSizeOut; m++)
				{
					C[0] = data[waOut[m]].x, C[1] = data[waOut[m]].y;
					D[0] = data[waOut[m + 1]].x, D[1] = data[waOut[m + 1]].y;

					nInterType = GetIntersectOfTwoLine(A, B, C, D, P);
					if (nInterType == 1)// 相交在两个线段内, 则不可见
					{
						if (GetDistance(P, A)>TOLERANCE)
						{
							bVisiable = false;
							break;
						}
						else
						{// 如果相交在顶点则需要根据绕向判断是否可见
							if (m == 0)
							{
								v1[0] = data[waOut[nSizeOut - 1]].x - data[waOut[0]].x;
								v1[1] = data[waOut[nSizeOut - 1]].y - data[waOut[0]].y;
								///v1[2]=data[waOut[nSizeOut-1]].z-data[waOut[0]].z;
							}
							else
							{
								v1[0] = data[waOut[m - 1]].x - C[0];
								v1[1] = data[waOut[m - 1]].y - C[1];
								///v1[2]=data[waOut[m-1]].z-C[2];
							}
							v2[0] = D[0] - C[0];
							v2[1] = D[1] - C[1];
							///	v2[2]=D[2]-C[2];
							AxB(v2, v1, v21);
							AxB(v2, v0, v20);
							if (AdotB(v21, v20)<TOLERANCE)
							{
								bVisiable = false;
								break;
							}
						}
					}
				}

				waOut.erase(waOut.begin() + nSizeOut);
				if (!bVisiable) continue;	// 如不可见, 则换一条边试试

				// 如外边界可见，则再比较与所有的内边界的可见性
				for (k = 0; k<nSize; k++)
				{
					std::vector<int>* pBIn = arrIns[k];
					unsigned int nSizeBin = pBIn->size();
					pBIn->push_back(pBIn->at(0));
					for (m = 0; m<nSizeBin; m++)
					{
						C[0] = data[(*pBIn)[m]].x;
						C[1] = data[(*pBIn)[m]].y;
						///	C[2]=data[(*pBIn)[m]].z;
						D[0] = data[(*pBIn)[m + 1]].x;
						D[1] = data[(*pBIn)[m + 1]].y;
						///	D[2]=data[(*pBIn)[m+1]].z;

						nInterType = GetIntersectOfTwoLine(A, B, C, D, P);
						if (nInterType == 1)// 相交在两个线段内, 则不可见
						{
							if (k>0 || GetDistance(P, B)>TOLERANCE)
							{
								bVisiable = false;
								break;
							}
						}
					}
					//pBIn->RemoveAt(nSizeBin);
					pBIn->erase(pBIn->begin() + nSizeBin);
					if (!bVisiable) break;	// 如不可见, 则跳出, 要换一条边试试
				}
				if (bVisiable) break;
			}
			if (bVisiable) break;
		}

		// 如找到可见边则扩展外边界且去掉此内边界，否则放到最后
		if (bVisiable)
		{
			nPosOut = i;
			nPosIn = j;
			// 扩展
			for (m = nPosIn, k = nPosOut + 1; m<nSizeIn; m++)
			{
				//waOut.InsertAt(k++, (*pwaIn)[m]);
				waOut.insert(waOut.begin() + k, (*pwaIn)[m]);
				k++;
			}
			for (m = 0; m <= nPosIn; m++)
			{
				//waOut.InsertAt(k++, (*pwaIn)[m]);

				waOut.insert(waOut.begin() + k, (*pwaIn)[m]);
				k++;
			}
			//waOut.InsertAt(k, waOut[nPosOut]);
			waOut.insert(waOut.begin() + k, waOut[nPosOut]);


			pwaIn->clear();
			delete pwaIn;
			pwaIn = NULL;
			//arrIns.RemoveAt(0);
			arrIns.erase(arrIns.begin());
		}
		else
		{
			arrIns[0] = arrIns[nSize - 1];
			arrIns[nSize - 1] = pwaIn;
		}
		nSize = arrIns.size();
	}

	// 任意多边形的三角剖分
	Triangulate(data, waOut, waTris);
	waOut.clear();
}

// (102) 判断以当前结点为顶点的三角形是否为多边形的一部分
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	IsPartOfPolygon
// 功    能:	判断以当前结点为顶点的三角形是否为多边形的一部分
// 参    数:	unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData, std::vector<int> * cPos
//   (入口):	a, b, c: 当前三角形的三个顶点序号
//				pdData: 当前多边形的结点数据
//				cPos: 凹点在拓扑结构数组中的位置
//   (出口):	无
// 返    回:	bool: true, 有凹点落在当前三角形内; 否则false
// 调用方法:	unsigned int a = …, b = …, c = …；
//				Vector3d pdData = new Vector3d […];
//				实例化 pdData ...
//				std::vector<int> cPos;
//				实例化 cPos ...
//				bool bRlt = IsPartOfPolygon(a, b, c, pdData, &cPos);
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
bool IsPartOfPolygon(unsigned int a, unsigned int b, unsigned int c, Vector3d * pdData,
	std::vector<int> * cPos)
	////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!cPos))
	{
		return false;
	}

	// 判断三角形 ABC 是否为完全属于多边形，只需判断是否有凹点落在以 A 为顶点的三角形 ABC 内
	// 要判断是否有凹点落在三角形 ABC 内，只须判断凹点与线段 BC 的张角是否等于凹点与 AB 的张
	// 角、AC 的张角之和
	// 如果相等，表示在三角形 ABC 外，判断其它的点；
	// 如果不等，凹点在三角形 ABC 内，表示三角形 ABC 不完全属于多边形，返回 false

	unsigned int i, k, n;
	Vector3d A, B, C, M;
	double BMC, AMB, AMC;

	A[0] = pdData[a][0];
	A[1] = pdData[a][1];
	A[2] = pdData[a][2];

	B[0] = pdData[b][0];
	B[1] = pdData[b][1];
	B[2] = pdData[b][2];

	C[0] = pdData[c][0];
	C[1] = pdData[c][1];
	C[2] = pdData[c][2];

	n = cPos->size();
	for (i = 0; i<n; i++)
	{
		//如果当前凹点为 C 点，不需要判断
		k = (*cPos)[i];
		if ((b == k) || (c == k))
		{
			continue;
		}

		M[0] = pdData[k][0];
		M[1] = pdData[k][1];
		M[2] = pdData[k][2];

		// 求张角并判断
		BMC = Get_Angle_C_Of_ABC(B, C, M);
		AMB = Get_Angle_C_Of_ABC(A, B, M);
		AMC = Get_Angle_C_Of_ABC(A, C, M);
		if (fabs(BMC + AMB + AMC - 360)<TOLERANCE)
			// if ( fabs(BMC+AMB+AMC-360)<0.001 )
		{
			return false;
		}
	}

	return true;
}

// (103) 定位凹点在拓扑结构数组中的位置
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetConcavePos
// 功    能:	定位多边形的凹点
// 参    数:	Vector3d * pdData, std::vector<int> * rPS, std::vector<int> * cPos
//   (入口):	pdData: 表示当前多边形的结点数据
//				rPS: 重组的拓扑结构数组
//				cPos: 凹点数组(返回)
//   (出口):	cPos: 凹点数组
// 返    回:	无
// 调用方法:	Vector3d pdData = new Vector3d […];
//				std::vector<int> rPS;
//				实例化 rPS…；
//				std::vector<int> cPos;
//				GetConcavePos(pdData, rPS, cPos);
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void GetConcavePos(Vector3d * pdData, std::vector<int> * rPS,
	std::vector<int> * cPos)
	////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!rPS) || (!cPos))
	{
		return;
	}

	unsigned int i, n, a, b, c;
	double A[2], B[2], C[2];

	// rPS：[0, 1, 2..., n-2, n-1]
	n = rPS->size();
	for (i = 0; i<n; i++)
	{
		b = i;
		if (i == 0)
		{
			a = n - 1;
			c = 1;
		}
		else if (i == n - 1)
		{
			a = n - 2;
			c = 0;
		}
		else
		{
			a = i - 1;
			c = i + 1;
		}

		a = (*rPS)[a];
		A[0] = pdData[a][0];
		A[1] = pdData[a][1];

		c = (*rPS)[c];
		C[0] = pdData[c][0];
		C[1] = pdData[c][1];

		b = (*rPS)[b];
		B[0] = pdData[b][0];
		B[1] = pdData[b][1];

		if ((B[0] - A[0])*(C[1] - B[1]) - (B[1] - A[1])*(C[0] - B[0])<0)
		{
			cPos->push_back(b);
		}
	}
}

// (104) 判断当前多边形边界结点方向是否为逆时针
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	IsCCW
// 功    能:	判断当前多边形边界结点方向是否为逆时针
// 参    数:	Vector3d * pdData, unsigned int * ps
//   (入口):	pfData: Vector3d 型的数组, 表示当前多边形的结点数据
//				polygons: unsigned int 型的数组, 表示当前多边形的拓扑结构, 其形式如下:
//					[组成多边形的边界结点数, 第一个结点, 第二个结点..., 最后一个结点]
//   (出口):	无
// 返    回:	true 表示为逆时针的，false 表示为顺时针的
// 调用方法:	unsigned int size = …;
//				Vector3d * pfData = new Vector3d [size];
//				//设置数据数组
//				……
//				unsigned int n=…;//边界结点数
//				unsigned int * polygons = new unsigned int [n];
//				//设置拓扑结构
//				……
//				bool bResult = IsCCW(pfData, polygons);
//GeoView_Doc_Mark_End  											//
// 主要思路:	把多边形分解成 n 个梯形，分别求面积，再相加
//					   n-1
//				area = ∑ [X(i+1)-X(i)] * [Y(i+1)+Y(i)] / 2
//					   i=0
//				如果面积大于 0，为顺时针；小于 0，为逆时针。等于 0，自相交，作为逆时针处理。
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
bool IsCCW(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return true;
	}

	unsigned int i, n;
	double A[2], B[2], area;

	n = ps[0];

	// 如果参数 ps 为一个多边形对象的某一单元
	if (n != 1)
	{
		// 先求出封闭段所围的面积
		A[0] = pdData[ps[n]][0];
		A[1] = pdData[ps[n]][1];
		B[0] = pdData[ps[1]][0];
		B[1] = pdData[ps[1]][1];
		area = (B[0] - A[0]) * (B[1] + A[1]) / 2.0;

		n++;
		for (i = 2; i<n; i++)
		{
			A[0] = B[0];
			A[1] = B[1];

			B[0] = pdData[ps[i]][0];
			B[1] = pdData[ps[i]][1];

			area += (B[0] - A[0]) * (B[1] + A[1]) / 2.0;
		}

		if (area>0)
		{
			return false;
		}

		return true;
	}

	// 如果参数 ps 为一个完整的填充多边形单元拓扑结构
	n = ps[1] + 1;

	// 先求出封闭段所围的面积
	A[0] = pdData[ps[n]][0];
	A[1] = pdData[ps[n]][1];
	B[0] = pdData[ps[2]][0];
	B[1] = pdData[ps[2]][1];
	area = (B[0] - A[0]) * (B[1] + A[1]) / 2.0;

	n++;
	for (i = 3; i<n; i++)
	{
		A[0] = B[0];
		A[1] = B[1];

		B[0] = pdData[ps[i]][0];
		B[1] = pdData[ps[i]][1];

		area += (B[0] - A[0]) * (B[1] + A[1]) / 2.0;
	}

	if (area>0)
	{
		return false;
	}

	return true;
}

// (105) 求三角形 ABC 的角 C 的角度
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Get_Angle_C_Of_ABC
// 功    能:	计算三角形 ABC 的顶角 C 的角度 (0 ~ 180 的值)
// 参    数:	Vector3d A, Vector3d B, Vector3d C
//   (入口):	A: 三角形的顶点A
//				B: 三角形的顶点B
//				C: 三角形的顶点C
//   (出口):	无
// 返    回:	double: 当前三角形 ABC 的顶角 C 的角度(单位: 度)
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double Get_Angle_C_Of_ABC(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	Vector3d CA, CB;

	CA[0] = A[0] - C[0];
	CA[1] = A[1] - C[1];
	CA[2] = A[2] - C[2];

	CB[0] = B[0] - C[0];
	CB[1] = B[1] - C[1];
	CB[2] = B[2] - C[2];

	//根据公式 a·b = |a|·|b|·cosθ
	double result = sqrt(CA[0] * CA[0] + CA[1] * CA[1] + CA[2] * CA[2]);
	if (fabs(result)<TOLERANCE)
	{
		return 180;
	}

	result *= sqrt(CB[0] * CB[0] + CB[1] * CB[1] + CB[2] * CB[2]);
	if (fabs(result)<TOLERANCE)
	{
		return 180;
	}
	else
	{
		result = (CA[0] * CB[0] + CA[1] * CB[1] + CA[2] * CB[2]) / result;
		if (result>1.0) result = 1.0;
		if (result<-1.0) result = -1.0;
		result = acos(result) * 180 / PI;
		return result;
	}
}

// (106) 计算三角形 ABC 的面积
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetAreaOfTri
// 功    能:	计算三角形 ABC 的面积
// 参    数:	Vector3d A, Vector3d B, Vector3d C
//   (入口):	A: 三角形的顶点A
//				B: 三角形的顶点B
//				C: 三角形的顶点C
//   (出口):	无
// 返    回:	double: 三角形 ABC 的面积
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double GetAreaOfTri(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	//----------------------------------
	//                B
	//               / \
		//             /     \
		//           /         \
		//         /             \
		//       A --------------- C
	//----------------------------------
	// 求矢量 BA 与 BC 的叉积的模，该值为平行四边形的面积，三角形的面积为其一半
	Vector3d BA, BC, P;
	AsubB(A, B, BA);
	AsubB(C, B, BC);
	AxB(BA, BC, P);
	// 面积为：
	double area = sqrt(AdotB(P, P)) / 2.0;

	return area;
}

// (107) 计算凸多边形的面积
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetAreaOfConvGon
// 功    能:	计算凸多边形的面积
// 参    数:	Vector3d * pdData, unsigned int * ps
//   (入口):	pfData:	Vector3d型的数组
//				polygons: unsigned int 型的数组
//   (出口):	无
// 返    回:	double: 当前凸多边形的面积
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double GetAreaOfConvGon(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return 0;
	}

	double area = 0;
	Vector3d A, B, C;
	SetAtoB(A, pdData[ps[1]]);
	for (unsigned int i = 2; i<ps[0]; i++)
	{
		SetAtoB(B, pdData[ps[i]]);
		SetAtoB(C, pdData[ps[i + 1]]);
		area += GetAreaOfTri(A, B, C);
	}

	return area;
}

// 计算平面多边形的面积
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetAreaOfPlaneGon
// 功    能:	计算平面多边形的面积
// 参    数:	std::vector<Vertex3d>& xyzs
//   (入口):	xyzs:	平面多边形的顺次顶点坐标
//   (出口):	无
// 返    回:	double: 平面多边形的面积(有可能为负数)
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-3-18, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetAreaOfPlaneGon(std::vector<Vertex3d>& xyzs)
////GeoView_Tech_Mark_End
{
	double dArea = 0.0;
	int nDots = xyzs.size();
	if (nDots<3)
	{
		return dArea;
	}

	Vector3d A = { xyzs[0].x, xyzs[0].y, xyzs[0].z },
		B = { xyzs[1].x, xyzs[1].y, xyzs[1].z },
		C = { xyzs[2].x, xyzs[2].y, xyzs[2].z };
	double dNormal[4] = { 0.0, 0.0, 0.0, 0.0 };
	GetTriNormal(dNormal, A, B, C, true);
	int nType = 2, i = 0;
	double dAbsX = fabs(dNormal[0]),
		dAbsY = fabs(dNormal[1]),
		dAbsZ = fabs(dNormal[2]);
	double dAbsMax = dAbsZ;
	if (dAbsX >= dAbsY && dAbsX >= dAbsZ)
	{
		nType = 0;
		dAbsMax = dAbsX;
	}
	else if (dAbsY >= dAbsX && dAbsY >= dAbsZ)
	{
		nType = 1;
		dAbsMax = dAbsY;
	}

	xyzs.push_back(xyzs[0]);
	switch (nType)
	{
	case 0:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].y * (xyzs[i + 1].z - xyzs[i].z);
		}
	}
	break;
	case 1:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].x * (xyzs[i + 1].z - xyzs[i].z);
		}
	}
	break;
	case 2:
	{
		for (i = 0; i<nDots; i++)
		{
			dArea += xyzs[i].x * (xyzs[i + 1].y - xyzs[i].y);
		}
	}
	break;
	default:;
	}
	xyzs.erase(xyzs.begin() + nDots);

	dArea = 0.5*dArea / dAbsMax;
	return dArea;
}

// (108) 求两点间的距离
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetDistance
// 功    能:	求两点间的距离
// 参    数:	const Vector3d A, const Vector3d B
//   (入口):	A: 点A
//				B: 点B
//   (出口):	无
// 返    回:	double: 点A与点B之间的距离(欧氏距离)
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double GetDistance(const Vector3d A, const Vector3d B)
////GeoView_Tech_Mark_End
{
	if ((A == NULL) || (B == NULL))
	{
		return 0.0f;
	}

	double len = sqrt((A[0] - B[0])*(A[0] - B[0])
		+ (A[1] - B[1])*(A[1] - B[1])
		+ (A[2] - B[2])*(A[2] - B[2]));
	return float(len);
}

// (112) 求 C 点在直线 AB 上的垂足，并返回是否在线段 AB 上
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetVPnt_C_On_AB
// 功    能:	求 C 点在直线 AB 上的垂足，并返回是否在线段 AB 上
// 参    数:	const Vector3d A, const Vector3d B,
//				const Vector3d C, Vector3d P = NULL
//   (入口):	A: 线段A的端点
//				B: 线段B的另一端点
//				C: 空间中任一点
//				P: 垂足(可能返回)
//				pdDist: 点到线段的距离(可能返回)
//   (出口):	P: 垂足(可能返回)
//				pdDist: 点到线段的距离(可能返回)
// 返    回:	true: 垂足在线段 AB 上; false: 垂足在线段 AB 外
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
bool GetVPnt_C_On_AB(const Vector3d A, const Vector3d B,
	const Vector3d C, Vector3d P, double *pdDist)
	////GeoView_Tech_Mark_End
{
	Vector3d v1, v2;
	v1[0] = A[0] - B[0], v1[1] = A[1] - B[1], v1[2] = A[2] - B[2];
	v2[0] = C[0] - B[0], v2[1] = C[1] - B[1], v2[2] = C[2] - B[2];
	double t = AdotB(v1, v1);
	if (fabs(t)<TOLERANCE)
	{
		t = 0;
	}
	else
	{
		t = AdotB(v1, v2) / t;
	}
	if (P)
	{
		P[0] = B[0] + v1[0] * t;
		P[1] = B[1] + v1[1] * t;
		P[2] = B[2] + v1[2] * t;
	}
	if (pdDist)
	{
		*pdDist = GetDistance(C, P);
	}
	if ((t<0) || (t>1))
	{
		return false;
	}

	return true;
}

bool GetVPnt_C_On_AB(const Vertex3d A, const Vertex3d B,
	const Vertex3d C, Vertex3d P, double *pdDist)
{
	bool bOn = false;
	Vector3d PA, PB, PC, PP;
	PA[0] = A.x;	PA[1] = A.y;	PA[2] = A.z;
	PB[0] = B.x;	PB[1] = B.y;	PB[2] = B.z;
	PC[0] = C.x;	PC[1] = C.y;	PC[2] = C.z;

	bOn = GetVPnt_C_On_AB(PA, PB, PC, PP, pdDist);
	P.x = PP[0];	P.y = PP[1];	P.z = PP[2];
	return bOn;
}

// (113) 求 线段 AB 与平面 (a, b, c, d) 的交点，并返回是否在线段 AB 上
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetInterpoint
// 功    能:	求线段 AB 与平面 (a, b, c, d) 的交点，并返回是否在线段 AB 上
// 参    数:	Vector3d A, Vector3d B,
//				double a, double b, double c, double d,
//				Vector3d p
//   (入口):	A, B:		线段的两点
//				a, b, c, d:	平面参数 (ax + by + cz + d = 0)
//				P:			交点(返回)
//   (出口):	P:			交点
// 返    回:	-1, 表示无交点
//				0, 表示交点在 AB 之间
//				1, 表示交点就是 A
//				2, 表示交点就是 B
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
int GetInterpoint(Vector3d A, Vector3d B,
	double a, double b, double c, double d,
	Vector3d p)
	////GeoView_Tech_Mark_End
{
	double fTemp, fTempA, fTempB;
	fTempA = a*A[0] + b*A[1] + c*A[2] + d;
	fTempB = a*B[0] + b*B[1] + c*B[2] + d;
	if (fabs(fTempA - fTempB)<TOLERANCE) // 直线 AB 平行于平面 (a, b, c, d)
	{
		return -1;
	}

	fTemp = fTempB / (fTempB - fTempA);

	if (fabs(fTemp - 0.0f)<TOLERANCE)
	{
		p[0] = B[0];
		p[1] = B[1];
		p[2] = B[2];
		return 2;
	}
	else if (fabs(fTemp - 1.0f)<TOLERANCE)
	{
		p[0] = A[0];
		p[1] = A[1];
		p[2] = A[2];
		return 1;
	}
	else if ((fTemp>0.0f) && (fTemp<1.0f))
	{
		p[0] = fTemp*A[0] + (1 - fTemp)*B[0];
		p[1] = fTemp*A[1] + (1 - fTemp)*B[1];
		p[2] = fTemp*A[2] + (1 - fTemp)*B[2];
		return 0;
	}
	else
	{
		return -1;
	}
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Degreesf
// 功    能:	//求向量在水平面上投影线的方位角
// 参    数:	Point3d v
//   (入口):	v: 向量()
//   (出口):	无
// 返    回:	double: 转化后的角度值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:    将向量单位化,向量的y值为即为其余弦值,调用反余弦函数求出弧度
//              根据弧度求出其角度
// 参    阅:
// 创建日期:	2006-1-23  孙卡
// 修改日期:	2006-1-23
//==================================================================//
double VecsAzimuth(Vector3d v)
{
	VecNormalize(v);
	//先求向量的弧度
	double Radian = 0;
	//然后将弧度转化为角度
	double Degree = 0;

	//第一象限
	if (v[0] >= 0 && v[1] >= 0)
	{
		Radian = acos(v[0]);
	}
	//第二象限
	if (v[0] <= 0 && v[1] >= 0)
	{
		Radian = acos(v[0]);
	}
	//第三象限,
	if (v[0] <= 0 && v[1] <= 0)
	{
		Radian = PI + acos(v[0]);
	}
	//第四象限
	if (v[0] >= 0 && v[1] <= 0)
	{
		Radian = PI + acos(v[0]);
	}
	//将弧度转化为角度
	Degree = Degreesf(Radian);
	return Degree;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	AzimuthsVec
// 功    能:	求方位角的单位向量
// 参    数:	double Azimuth
//   (入口):	Azimuth 单位（度）
//   (出口):	无
// 返    回:    Point3D v
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:    方位角的正弦值即为方位角向量的y值，余弦值即为x值，z值视为0
// 参    阅:
// 创建日期:	2006-1-23  孙卡
// 修改日期:	2006-1-23
//==================================================================//
void AzimuthsVec(double Azimuth, Vector3d Vec)
{
	Vec[0] = cos(Radiansf(Azimuth));
	Vec[1] = sin(Radiansf(Azimuth));
	Vec[2] = 0;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetInterpoint
// 功    能:	求线段 AB 与平面LMN的交点P, 并返回是否在线段AB上
// 参    数:	Vector3d A, Vector3d B,
//				Vector3d L, Vector3d M, Vector3d N,
//				Vector3d P
//   (入口):	A, B:		线段的两点
//				L, M, N:	平面上不共线的三点
//				P:			交点(返回)
//   (出口):	P:			交点
// 返    回:	-1, 表示无交点
//				0, 表示交点在 AB 之间
//				1, 表示交点就是 A
//				2, 表示交点就是 B
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
int GetInterpoint(Vector3d A, Vector3d B,
	Vector3d L, Vector3d M, Vector3d N,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	double a, b, c, d;
	Vector3d v, v1, v2;
	v1[0] = (float)(M[0] - L[0]);
	v1[1] = (float)(M[1] - L[1]);
	v1[2] = (float)(M[2] - L[2]);

	v2[0] = (float)(N[0] - L[0]);
	v2[1] = (float)(N[1] - L[1]);
	v2[2] = (float)(N[2] - L[2]);
	AxB(v1, v2, v);

	a = v[0];
	b = v[1];
	c = v[2];
	d = -(a*L[0] + b*L[1] + c*L[2]);

	return GetInterpoint(A, B, a, b, c, d, P);
}

// (113_2)求 直线 AB 与平面LMN的交点P, 并返回是否在线段AB上
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetPntOfLine_Plane
// 功    能:	求直线 AB 与平面LMN的交点P, 并返回是否在线段AB上
// 参    数:	Vector3d A, Vector3d B,
//				Vector3d L, Vector3d M, Vector3d N,
//				Vector3d P
//   (入口):	A, B:		线段的两点
//				L, M, N:	平面上不共线的三点
//				P:			交点(返回)
//   (出口):	P:			交点
// 返    回:	-1: 无交点
//				0: 有交点, 但不在AB内
//				1: 有交点, 且在AB内
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
int GetPntOfLine_Plane(Vector3d A, Vector3d B,
	Vector3d L, Vector3d M, Vector3d N,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	double a, b, c, d;
	Vector3d v, v1, v2;
	v1[0] = M[0] - L[0];
	v1[1] = M[1] - L[1];
	v1[2] = M[2] - L[2];

	v2[0] = N[0] - L[0];
	v2[1] = N[1] - L[1];
	v2[2] = N[2] - L[2];
	AxB(v1, v2, v);

	a = v[0];
	b = v[1];
	c = v[2];
	d = -(a*L[0] + b*L[1] + c*L[2]);

	double fTemp, fTempA, fTempB;
	fTempA = a*A[0] + b*A[1] + c*A[2] + d;
	fTempB = a*B[0] + b*B[1] + c*B[2] + d;
	if (fabs(fTempA - fTempB)<TOLERANCE) // 直线 AB 平行于平面 (a, b, c, d)
	{
		return -1;
	}

	fTemp = fTempB / (fTempB - fTempA);
	P[0] = fTemp*A[0] + (1 - fTemp)*B[0];
	P[1] = fTemp*A[1] + (1 - fTemp)*B[1];
	P[2] = fTemp*A[2] + (1 - fTemp)*B[2];
	if (fTemp>1.0f || fTemp<0.0f)
	{
		return 0;
	}
	return 1;
}

// (114) 求 点 P 绕直线 AB(from A to B) 旋转 α 角度后的点Q坐标
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetPntRotateOnLine
// 功    能:	求点 P 绕直线 AB(from A to B) 旋转 α 角度后的点Q坐标
// 参    数:	Vector3d A, Vector3d B, Vector3d P, Vector3d Q, float deltaRad
//   (入口):	A, B:		直线AB上的两点
//				P:			点P
//				Q:			P点旋转后的位置点Q(返回)
//				deltaRad:	旋转的角度(单位: 弧度)
//   (出口):	Q:			P点旋转后的位置点Q(返回)
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:	孙家广《计算机图形学》(Edition 3rd) page 372.
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
void GetPntRotateOnLine(Vector3d A, Vector3d B, Vector3d P,
	Vector3d Q, float deltaRad)
	////GeoView_Tech_Mark_End
{
	double a, b, c, v, distance, fb = 0.0f, fc = 1.0f;
	double cosa = (float)cos(deltaRad), sina = (float)sin(deltaRad);
	MATRIX4X4 Rx, InvRx, Ry, InvRy, Rz;
	Vector3d p, q;
	p[0] = P[0] - A[0];
	p[1] = P[1] - A[1];
	p[2] = P[2] - A[2];


	a = B[0] - A[0];
	b = B[1] - A[1];
	c = B[2] - A[2];
	distance = (float)(sqrt(a*a + b*b + c*c));
	a /= distance;
	b /= distance;
	c /= distance;
	v = sqrt(b*b + c*c);
	if (v>TOLERANCE)
	{
		fb = b / v;
		fc = c / v;
	}

	Rx[0][0] = 1.0f;	Rx[0][1] = 0.0f;		Rx[0][2] = 0.0f;		Rx[0][3] = 0.0f;
	Rx[1][0] = 0.0f;	Rx[1][1] = fc;			Rx[1][2] = -fb;			Rx[1][3] = 0.0f;
	Rx[2][0] = 0.0f;	Rx[2][1] = fb;			Rx[2][2] = fc;			Rx[2][3] = 0.0f;
	Rx[3][0] = 0.0f;	Rx[3][1] = 0.0f;		Rx[3][2] = 0.0f;		Rx[3][3] = 1.0f;

	Ry[0][0] = v;		Ry[0][1] = 0.0f;		Ry[0][2] = -a;			Ry[0][3] = 0.0f;
	Ry[1][0] = 0.0f;	Ry[1][1] = 1.0f;		Ry[1][2] = 0.0f;		Ry[1][3] = 0.0f;
	Ry[2][0] = a;		Ry[2][1] = 0.0f;		Ry[2][2] = v;			Ry[2][3] = 0.0f;
	Ry[3][0] = 0.0f;	Ry[3][1] = 0.0f;		Ry[3][2] = 0.0f;		Ry[3][3] = 1.0f;

	Rz[0][0] = cosa;	Rz[0][1] = -sina;		Rz[0][2] = 0.0f;		Rz[0][3] = 0.0f;
	Rz[1][0] = sina;	Rz[1][1] = cosa;		Rz[1][2] = 0.0f;		Rz[1][3] = 0.0f;
	Rz[2][0] = 0.0f;	Rz[2][1] = 0.0f;		Rz[2][2] = 1.0f;		Rz[2][3] = 0.0f;
	Rz[3][0] = 0.0f;	Rz[3][1] = 0.0f;		Rz[3][2] = 0.0f;		Rz[3][3] = 1.0f;

	MatrixCopy(Rx, InvRx);
	TransposeMatrix(InvRx);
	MatrixCopy(Ry, InvRy);
	TransposeMatrix(InvRy);

	VecTransform(p, q, Rx);
	VecTransform(q, p, Ry);
	VecTransform(p, q, Rz);
	VecTransform(q, p, InvRy);
	VecTransform(p, q, InvRx);

	Q[0] = A[0] + q[0];
	Q[1] = A[1] + q[1];
	Q[2] = A[2] + q[2];
}

// (115) 求点数组环绕法线的坐标 ////////////////////////+
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetPntsRotateNormal
// 功    能:	求点数组环绕法线的坐标
// 参    数:	Vector3d Normal, Vector3d O, unsigned int wCnt, Vector3d* pData = NULL
//   (入口):	wCnt:	点数组的总点数
//				pData:	点数组坐标(可能返回)
//				Normal:	法线方向
//				O:		法线上的一点
//   (出口):	无
// 返    回:	Vector3d*:	旋转后的点数组坐标(用完后, 用户必须手动释放)
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	输入点数组坐标(xoy平面上的), 返回经过环绕指定法线旋转后的坐标
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
Vector3d* GetPntsRotateNormal(Vector3d N, Vector3d O, unsigned int wCnt, Vector3d* pData)
////GeoView_Tech_Mark_End
{
	if (pData == NULL)
	{// 如果为空, 则自动创建缺省的点数组
		pData = new Vector3d[wCnt];
		double dRad = 0.0, dTheta = 2 * PI / wCnt;
		for (unsigned int i = 0; i<wCnt; i++, dRad += dTheta)
		{
			pData[i][0] = cos(dRad);
			pData[i][1] = sin(dRad);
			pData[i][2] = 0.0;
		}
	}
	double dX = 0.0, dY = 0.0, dZ = 0.0;

	// 根据孙家广《计算机图形学》(Edition 3rd) page 372.
	double a, b, c, v, distance, fb = 0.0f, fc = 1.0f;
	MATRIX4X4 Rx, InvRx, Ry, InvRy;
	Vector3d p, q;

	a = N[0];
	b = N[1];
	c = N[2];
	distance = (float)(sqrt(a*a + b*b + c*c));
	a /= distance;
	b /= distance;
	c /= distance;
	v = sqrt(b*b + c*c);
	if (v>TOLERANCE)
	{
		fb = b / v;
		fc = c / v;
	}

	Rx[0][0] = 1.0f;	Rx[0][1] = 0.0f;		Rx[0][2] = 0.0f;		Rx[0][3] = 0.0f;
	Rx[1][0] = 0.0f;	Rx[1][1] = fc;			Rx[1][2] = -fb;			Rx[1][3] = 0.0f;
	Rx[2][0] = 0.0f;	Rx[2][1] = fb;			Rx[2][2] = fc;			Rx[2][3] = 0.0f;
	Rx[3][0] = 0.0f;	Rx[3][1] = 0.0f;		Rx[3][2] = 0.0f;		Rx[3][3] = 1.0f;

	Ry[0][0] = v;		Ry[0][1] = 0.0f;		Ry[0][2] = -a;			Ry[0][3] = 0.0f;
	Ry[1][0] = 0.0f;	Ry[1][1] = 1.0f;		Ry[1][2] = 0.0f;		Ry[1][3] = 0.0f;
	Ry[2][0] = a;		Ry[2][1] = 0.0f;		Ry[2][2] = v;			Ry[2][3] = 0.0f;
	Ry[3][0] = 0.0f;	Ry[3][1] = 0.0f;		Ry[3][2] = 0.0f;		Ry[3][3] = 1.0f;

	MatrixCopy(Rx, InvRx);
	TransposeMatrix(InvRx);
	MatrixCopy(Ry, InvRy);
	TransposeMatrix(InvRy);

	for (unsigned int i = 0; i<wCnt; i++)
	{
		p[0] = pData[i][0];
		p[1] = pData[i][1];
		p[2] = pData[i][2];
		VecTransform(p, q, InvRy);
		VecTransform(q, p, InvRx);
		pData[i][0] = O[0] + p[0];
		pData[i][1] = O[1] + p[1];
		pData[i][2] = O[2] + p[2];
	}

	return pData;
}

// (120) 求点 P 到由A、B、C三点确定的平面的垂足Q点坐标
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetPntVerticalPlane
// 功    能:	求点 P 到由A、B、C三点确定的平面的垂足Q点坐标
// 参    数:	Vector3d A, Vector3d B, Vector3d C, Vector3d P, Vector3d Q
//   (入口):	A, B, C:	平面上不共线的三点
//				P:			点P
//				Q:			点P到由平面ABC的垂足(返回)
//   (出口):	Q:			点P到由平面ABC的垂足
// 返    回:	P到平面的距离
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	算法原理:
//				N = (P2-P1)×(P3-P1)
//						--> Normalize
//				Distance = |v3●N|
//				PL = P - (v3●N)N 
// 参    阅:	详见《体积等计算》中的相关内容
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
float GetPntVerticalPlane(Vector3d A, Vector3d B, Vector3d C,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	Vector3d v12, v13, N, v3;
	v12[0] = B[0] - A[0], v12[1] = B[1] - A[1], v12[2] = B[2] - A[2];
	v13[0] = C[0] - A[0], v13[1] = C[1] - A[1], v13[2] = C[2] - A[2];
	v3[0] = P[0] - A[0], v3[1] = P[1] - A[1], v3[2] = P[2] - A[2];

	AxB(v12, v13, N);
	VecNormalize(N);

	double fDistance = AdotB(v3, N);
	Q[0] = P[0] - fDistance*N[0];
	Q[1] = P[1] - fDistance*N[1];
	Q[2] = P[2] - fDistance*N[2];

	return (float)fabs(fDistance);
}

// (121) 判断点P是否在多边形内(二维平面上, 包括在多边形边上)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	IsPntInGon
// 功    能:	判断点 P 是否在多边形内
// 参    数:	unsigned int wCnt, Vector3d * pData, Vector3d P
//   (入口):	wCnt:	多边形的点数
//				pData:	多边形的数据
//				P:		点P
//   (出口):	无
// 返    回:	true: 点P在多边形内; false: 点P不在多边形内
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
bool IsPntInGon(unsigned int wCnt, Vector3d* pData, Vector3d P)
////GeoView_Tech_Mark_End
{
	/// Notes: 如果认为点在多边形边上不算是在多边形内, 则对本程序"// 在边上"等处的
	///			return true; 改为 return false;
	int nCCC = 0;	// 计算交点的个数
	double x1, y1, x2, y2, y;

	for (unsigned int i = 0; i<wCnt; i++)
	{
		if (i == wCnt - 1)
		{
			x1 = pData[i][0]; y1 = pData[i][1];
			x2 = pData[0][0]; y2 = pData[0][1];
		}
		else
		{
			x1 = pData[i][0]; y1 = pData[i][1];
			x2 = pData[i + 1][0]; y2 = pData[i + 1][1];
		}

		if (x1 == x2)	// 排除竖直的情况
		{
			if ((P[0] == x1) && ((P[1] - y1)*(P[1] - y2) <= 0.0f))// 在边上
				return true;
		}
		else if ((P[0] - x1)*(P[0] - x2) <= 0.0f)
		{
			if (P[0] == x1 && P[1] <= y1)
			{
				if (P[1] == y1)		// 在边上
					return true;
				if (x2 > P[0])
					nCCC++;
			}
			else if (P[0] == x2 && P[1] <= y2)
			{
				if (P[1] == y2)		// 在边上
					return true;
				if (x1 > P[0])
					nCCC++;
			}
			else
			{
				y = y1 + (P[0] - x1)*(y2 - y1) / (x2 - x1);
				if (y == P[1])			// 在边上
					return true;
				if (y > P[1])
					nCCC++;
			}
		}
	}

	// 如果交点的个数为奇数, 则P在多边形内
	if (fmod((double)nCCC, (double)2))
		return true;

	return false;
}

// (122) 求线段 AB 与线段 CD 的交点 P
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetIntersectOfTwoLine
// 功    能:	求线段 AB 与线段 CD 的交点 P
// 参    数:	Vector3d A, Vector3d B, Vector3d C, Vector3d D, Vector3d P
//   (入口):	A, B:	线段AB的端点
//				C, D:	线段CD的端点
//				P:		线段AB与线段CD的交点
//   (出口):	P:		线段AB与线段CD的交点
// 返    回:	0: 无交点, 1: 交点在AB且在CD内, 2: 交点只在AB内, 3: 交点只在CD内, 4: 交点不在AB和CD内
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
int GetIntersectOfTwoLine(Vector3d A, Vector3d B,
	Vector3d C, Vector3d D,
	Vector3d P)
	////GeoView_Tech_Mark_End
{
	//// 返回: 0: 无交点, 1: 交点在AB且在CD内, 2: 交点只在AB内, 3: 交点只在CD内, 4: 交点不在AB和CD内
	MATRIX4X4 MM;
	double x1 = A[0], y1 = A[1], z1 = A[2];
	double x2 = B[0], y2 = B[1], z2 = B[2];
	double x3 = C[0], y3 = C[1], z3 = C[2];
	double x4 = D[0], y4 = D[1], z4 = D[2];
	MM[0][0] = A[0], MM[0][1] = A[1], MM[0][2] = A[2], MM[0][3] = 1.0f;
	MM[1][0] = B[0], MM[1][1] = B[1], MM[1][2] = B[2], MM[1][3] = 1.0f;
	MM[2][0] = C[0], MM[2][1] = C[1], MM[2][2] = C[2], MM[2][3] = 1.0f;
	MM[3][0] = D[0], MM[3][1] = D[1], MM[3][2] = D[2], MM[3][3] = 1.0f;

	if (fabs(DetOfMatrix(MM))>TOLERANCE)	// 不共面
	{
		return 0;
	}

	double delta = (x3 - x4)*(y2 - y1) - (x2 - x1)*(y3 - y4);
	double u, v;
	if (fabs(delta)>TOLERANCE)
	{
		u = ((x3 - x1)*(y4 - y3) - (x4 - x3)*(y3 - y1)) / delta;
		v = ((x3 - x1)*(y2 - y1) - (x2 - x1)*(y3 - y1)) / delta;
	}
	else
	{
		delta = (x3 - x4)*(z2 - z1) - (x2 - x1)*(z3 - z4);
		if (fabs(delta)>TOLERANCE)
		{
			u = ((x3 - x1)*(z4 - z3) - (x4 - x3)*(z3 - z1)) / delta;
			v = ((x3 - x1)*(z2 - z1) - (x2 - x1)*(z3 - z1)) / delta;
		}
		else
		{
			delta = (y3 - y4)*(z2 - z1) - (y2 - y1)*(z3 - z4);
			if (fabs(delta)>TOLERANCE)
			{
				u = ((y3 - y1)*(z4 - z3) - (y4 - y3)*(z3 - z1)) / delta;
				v = ((y3 - y1)*(z2 - z1) - (y2 - y1)*(z3 - z1)) / delta;
			}
			else
				return 0;	// 平行或重合
		}
	}
	P[0] = (1.0f - u)*x1 + u*x2;
	P[1] = (1.0f - u)*y1 + u*y2;
	P[2] = (1.0f - u)*z1 + u*z2;
	/// 返回: 0: 无交点, 1: 交点在AB且在CD内, 2: 交点只在AB内, 3: 交点只在CD内, 4: 交点不在AB和CD内
	bool bU = (u >= 0.0f && u <= 1.0f);
	bool bV = (v >= 0.0f && v <= 1.0f);
	if (bU && bV)
	{
		return 1;
	}
	if (bU && !bV)
	{
		return 2;
	}
	if (!bU && bV)
	{
		return 3;
	}
	return 4;
}

// (123) 求过点P的铅垂线与三角形ABC的交点Q(不一定在ABC内)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetIntersectOfTri
// 功    能:	求过点 P 的铅垂线与三角形 ABC 的交点Q(不一定在ABC内)
// 参    数:	Vector3d A, Vector3d B, Vector3d C, Vector3d P, Vector3d Q
//   (入口):	A, B, C:	三角形ABC的顶点
//				P:			点P
//				Q:			过点 P 的铅垂线与三角形 ABC 的交点(返回)
//   (出口):	Q:			过点 P 的铅垂线与三角形 ABC 的交点
// 返    回:	true: 有交点;	false: 无交点
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
bool GetIntersectOfTri(Vector3d A, Vector3d B, Vector3d C,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	double x1 = A[0], y1 = A[1], z1 = A[2],
		x2 = B[0], y2 = B[1], z2 = B[2],
		x3 = C[0], y3 = C[1], z3 = C[2],
		x = P[0], y = P[1];

	MATRIX4X4 MM, MX, MY, deltaM;
	deltaM[0][0] = x1, deltaM[0][1] = y1, deltaM[0][2] = 1.0f, deltaM[0][3] = 0.0f;
	deltaM[1][0] = x2, deltaM[1][1] = y2, deltaM[1][2] = 1.0f, deltaM[1][3] = 0.0f;
	deltaM[2][0] = x3, deltaM[2][1] = y3, deltaM[2][2] = 1.0f, deltaM[2][3] = 0.0f;
	deltaM[3][0] = 0.0f, deltaM[3][1] = 0.0f, deltaM[3][2] = 0.0f, deltaM[3][3] = 1.0f;
	double detOfM = DetOfMatrix(deltaM);
	if (fabs(detOfM)<TOLERANCE)
	{
		return false;	// 无交点
	}
	MM[0][0] = x1, MM[0][1] = y1, MM[0][2] = z1, MM[0][3] = 0.0f;
	MM[1][0] = x2, MM[1][1] = y2, MM[1][2] = z2, MM[1][3] = 0.0f;
	MM[2][0] = x3, MM[2][1] = y3, MM[2][2] = z3, MM[2][3] = 0.0f;
	MM[3][0] = 0.0f, MM[3][1] = 0.0f, MM[3][2] = 0.0f, MM[3][3] = 1.0f;
	MX[0][0] = y1, MX[0][1] = z1, MX[0][2] = 1.0f, MX[0][3] = 0.0f;
	MX[1][0] = y2, MX[1][1] = z2, MX[1][2] = 1.0f, MX[1][3] = 0.0f;
	MX[2][0] = y3, MX[2][1] = z3, MX[2][2] = 1.0f, MX[2][3] = 0.0f;
	MX[3][0] = 0.0f, MX[3][1] = 0.0f, MX[3][2] = 0.0f, MX[3][3] = 1.0f;
	MY[0][0] = x1, MY[0][1] = z1, MY[0][2] = 1.0f, MY[0][3] = 0.0f;
	MY[1][0] = x2, MY[1][1] = z2, MY[1][2] = 1.0f, MY[1][3] = 0.0f;
	MY[2][0] = x3, MY[2][1] = z3, MY[2][2] = 1.0f, MY[2][3] = 0.0f;
	MY[3][0] = 0.0f, MY[3][1] = 0.0f, MY[3][2] = 0.0f, MY[3][3] = 1.0f;

	Q[0] = x;
	Q[1] = y;
	Q[2] = (DetOfMatrix(MM) - x*DetOfMatrix(MX) + y*DetOfMatrix(MY)) / detOfM;

	return true;
}

// (133) 求三角形的法线方向
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
//函 数 名: GetTriNormal
//功    能: 求三角形的法线方向
//参    数: dCoef, A, B, C, bNormal
//    入口: A, B, C，Vector3d 变量，三角形的三个顶点
//			bNormal，bool变量，是否归一化
//    出口: dCoef，double四元素数组，平面方程系数
//返    回: 无
//调用方法:	
//GeoView_Doc_Mark_End  											//
//主要思路:
//参    阅:
//创建日期: 2004-3-8
//修改日期: 2004-05-13，柳庆武，增加法线方向上的平移量.
//			若不需要求，传NULL值，而不是缺省
//==================================================================//
void GetTriNormal(double dCoef[4], const Vector3d A, const Vector3d B,
	const Vector3d C, const bool bNormal)
	////GeoView_Tech_Mark_End
{
	Vector3d v, v1, v2;

	AsubB(B, A, v1);
	AsubB(C, A, v2);
	AxB(v1, v2, v);

	if (bNormal)
	{
		double temp = sqrt(v[0] * v[0] + v[1] * v[1] + v[2] * v[2]);
		// 避免两点重合的三角形的出现  [2006-4-20, 21:06]
		if (temp>TOLERANCE)
		{
			dCoef[0] = v[0] / temp;
			dCoef[1] = v[1] / temp;
			dCoef[2] = v[2] / temp;
		}
	}
	else
	{
		dCoef[0] = v[0];
		dCoef[1] = v[1];
		dCoef[2] = v[2];
	}
	dCoef[3] = -(dCoef[0] * A[0] + dCoef[1] * A[1] + dCoef[2] * A[2]);
}

// //(141)计算三个特殊平面上的凸多边形的面积
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetAreaOfConvexityGon
// 功    能:	计算三个特殊平面上的凸多边形的面积
// 参    数:	const Vector3d* ptdata, const unsigned int* polygons, int nprojection
//   (入口):	ptData:		凸多边形的顶点数据
//				polygons:	凸多边形的拓扑结构
//				nprojection:投影方式
//   (出口):	无
// 返    回:	double: 多边形的面积
// 调用方法:	Vector3d * pfData = new Vector3d […];//要设置具体数据
//				记录多边形的坐标点
//				unsigned int * polygons = new unsigned int […];//要设置具体数据
//				记录凸多边形或者是线条组成多边形的拓朴结构
//				float area = GetAreaOfConvexityGon(ptdata,polygons,0);
//				nprojection记录的是在求在那一个平面上的面积其中0代表XOY，1代表YOZ，2代表XOZ
//GeoView_Doc_Mark_End  											//
// 主要思路:	用梯形算法计算面积
// 参    阅:	《C程序设计》谭浩强	P201
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
double GetAreaOfConvexityGon(const Vector3d* ptdata, const unsigned int* polygons, int nprojection)
//GeoView_Tech_Mark_End
{
	if (nprojection >2 || nprojection<0)
		return 0;
	double area = 0.0;
	double s = 0.0;
	double sarea = 0.0;
	int i;
	Vector3d A, B;
	i = 0;
	int k = polygons[1] - 1;
	while (i <= k)
	{
		if (i == k)
		{
			if (nprojection == 0)
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][1];
				A[2] = ptdata[polygons[i + 2]][2];
				B[0] = ptdata[polygons[2]][0];
				B[1] = ptdata[polygons[2]][1];
				B[2] = ptdata[polygons[2]][2];
			}
			//如果投影到YOZ平面
			else if (nprojection == 1)
			{
				A[0] = ptdata[polygons[i + 2]][1];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][0];
				B[0] = ptdata[polygons[2]][1];
				B[1] = ptdata[polygons[2]][2];
				B[2] = ptdata[polygons[2]][0];
			}
			else
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][1];
				B[0] = ptdata[polygons[2]][0];
				B[1] = ptdata[polygons[2]][2];
				B[2] = ptdata[polygons[2]][1];
			}

		}
		else
		{
			if (nprojection == 0)
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][1];
				A[2] = ptdata[polygons[i + 2]][2];
				B[0] = ptdata[polygons[i + 3]][0];
				B[1] = ptdata[polygons[i + 3]][1];
				B[2] = ptdata[polygons[i + 3]][2];
			}
			else if (nprojection == 1)
			{
				A[0] = ptdata[polygons[i + 2]][1];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][0];
				B[0] = ptdata[polygons[i + 3]][1];
				B[1] = ptdata[polygons[i + 3]][2];
				B[2] = ptdata[polygons[i + 3]][0];
			}
			else
			{
				A[0] = ptdata[polygons[i + 2]][0];
				A[1] = ptdata[polygons[i + 2]][2];
				A[2] = ptdata[polygons[i + 2]][1];
				B[0] = ptdata[polygons[i + 3]][0];
				B[1] = ptdata[polygons[i + 3]][2];
				B[2] = ptdata[polygons[i + 3]][1];
			}
		}
		i++;
		//用梯形算法计算面积（这个方法只是适用于凸多边形）
		sarea = (A[1] + B[1])*(B[0] - A[0]) / 2.0;
		area = area + sarea;
		// 如果是曲线的话，知道曲线方程，可以是任意形式的，
		//转换为显式的方程用积分的方法可以求得,具体可以参照《C程序设计》谭浩强	P201

	}
	area = fabs(area);
	return area;

}

//(142)计算两个数的最小公倍数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetMinMultiple
// 功    能:	计算两个整数的最小公倍数
// 参    数:	int a,int b
//   (入口):	a, b:	两个整数
//   (出口):	无
// 返    回:	int: 最小公倍数
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-3-8
// 修改日期:	2004-3-8
//==================================================================//
int GetMinMultiple(int a, int b)
////GeoView_Tech_Mark_End
{
	int m = (a <= b) ? a : b;
	int n = (a >= b) ? a : b;
	int nMinMul = 1;//记录最小公倍数
	double num = 1.0;
	int i = 1;
	//如果余数大于最小数，不能被整除
	while (num > TOLERANCE)
	{
		nMinMul = i * n;
		num = fmod((double)nMinMul, (double)m);
		i++;
	}
	return nMinMul;
}

// (331) 判断求取凹点的两轴方向
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//函 数 名：GetCmptAxis
//功    能：判断求取凹点的两轴方向
//参    数：p, q, v
//    入口：v, Vector3d变量，多边形法线方向
//    出口：p, q, int变量, 有效二轴向
//返    回：无
//调用方法：Vector3d v = …;
//			int p = …, q = …;
//			GetCmptAxis(p, q, v);
//GeoView_Doc_Mark_End
//主要思路：
//创建日期：2004/04/02	柳庆武
//修改日期：
void GetCmptAxis(int & p, int & q, const double v[4])
//GeoView_Tech_Mark_End
{
	p = 0;
	q = 1;

	// 如果垂直于 X 轴 (平行于YOZ 面)
	if ((fabs(v[1])<TOLERANCE) && (fabs(v[2])<TOLERANCE))
	{
		p = 1;
		q = 2;
	}
	// 如果垂直于 Y 轴 (平行于ZOX 面)
	else if ((fabs(v[0])<TOLERANCE) && (fabs(v[2])<TOLERANCE))
	{
		p = 0;
		q = 2;
	}
}

// (341) 点 P 是否在三角形 ABC 内 (在边上也算)
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//函 数 名：IsInTri
//功    能：点 P 是否在三角形 ABC 内 (在边上也算)
//参    数：P, A, B, C
//    入口：P, Vector3d 变量，要判断的点
//		    A, B, C, Vector3d 变量，三角形顶点坐标
//    出口：无
//返    回：bool值，是否在三角形内 (在边上也算)
//调用方法：Vector3d P = …；
//			Vector3d A = …, B = …, C = …；
//			IsInTri(P, A, B, C);
//GeoView_Doc_Mark_End
//主要思路：
//创建日期：2004/03/17	柳庆武
//修改日期：
bool IsInTri(const Vector3d P, const Vector3d A, const Vector3d B,
	const Vector3d C)
	//GeoView_Tech_Mark_End
{
	Vector3d aa, bb, cc, v;
	int k = 0;
	double dTempA, dTempB;

	// 判断交点是否在三角形里
	AsubB(P, A, aa);
	AsubB(B, C, bb);
	AsubB(B, A, cc);

	v[0] = aa[0] * bb[1] - aa[1] * bb[0];
	v[1] = aa[0] * bb[2] - aa[2] * bb[0];
	v[2] = aa[1] * bb[2] - aa[2] * bb[1];

	k = 0;
	dTempA = fabs(v[0]);
	if (fabs(v[1])>dTempA)
	{
		dTempA = fabs(v[1]);
		k = 1;
	}
	if (fabs(v[2])>dTempA)
	{
		k = 2;
	}

	switch (k)
	{
	case 0:
		dTempA = (cc[0] * bb[1] - cc[1] * bb[0]) / v[0];
		dTempB = (aa[0] * cc[1] - aa[1] * cc[0]) / v[0];
		break;

	case 1:
		dTempA = (cc[0] * bb[2] - cc[2] * bb[0]) / v[1];
		dTempB = (aa[0] * cc[2] - aa[2] * cc[0]) / v[1];
		break;

	case 2:
	default:
		dTempA = (cc[1] * bb[2] - cc[2] * bb[1]) / v[2];
		dTempB = (aa[1] * cc[2] - aa[2] * cc[1]) / v[2];
		break;
	}

	// 如果 dTempA 小于 1，表示在三角形外
	// 如果 dTempA 等于 1，表示在第三边上
	if ((dTempA - 1.0<-TOLERANCE) ||
		(dTempB<-TOLERANCE) ||
		(dTempB - 1.0>TOLERANCE))
		// if ( (dTempA<1)||(dTempB<0)||(dTempB>1) )
	{
		return false;
	}

	return true;
}

// (326) 求线段 MN 与三角形 ABC 的交点
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//函 数 名：GetPntOfMN_ABC
//功    能：求线段 MN 与三角形 ABC 的交点
//参    数：P, M, N, A, B, C, a, b, c, d
//          入口：P, Vector3d 变量，交点坐标
//				  M，N，Vector3d变量，线段 MN 的端点
//				  A，B，C，Vector3d变量，三角形ABC的顶点
//				  a, b, c, d, double变量，三角形ABC所在平面方程的系数
//          出口：无
//返    回：bool值，交点是否位于线段MN上且在三角形ABC内部
//调用方法：Vector3d P;
//			Vector3d M = …, N = …；
//			Vector3d A = …, B = …, C = …；
//			double a = …，b = …，c = …，d = …；
//			GetPntOfMN_ABC(P, M, N, A, B, C, a, b, c, d);
//GeoView_Doc_Mark_End
//主要思路：
//创建日期：2003/10/28	柳庆武
//修改日期：
bool GetPntOfMN_ABC(Vector3d P, const Vector3d M, const Vector3d N,
	const Vector3d A, const Vector3d B, const Vector3d C,
	const double dCoef[4])
	//GeoView_Tech_Mark_End
{
	Vector3d v;
	double dTempA, dTempB;

	// 判断线段 MN 是否与平面相交
	AsubB(N, M, v);
	dTempA = dCoef[0] * v[0] + dCoef[1] * v[1] + dCoef[2] * v[2];
	// 直线平行于平面
	if (fabs(dTempA)<TOLERANCE)
	{
		return false;
	}
	dTempB = (0 - dCoef[0] * M[0] - dCoef[1] * M[1] - dCoef[2] * M[2] - dCoef[3]) / dTempA;
	// 交点不在 MN 上
	if ((dTempB<0) || (dTempB>1))
	{
		return false;
	}

	// 交点坐标
	P[0] = M[0] + v[0] * dTempB;
	P[1] = M[1] + v[1] * dTempB;
	P[2] = M[2] + v[2] * dTempB;

	return IsInTri(P, A, B, C);
}

// (431) 把点 A 给 B
//GeoView_Tech_Mark_Begin
//GeoView_Doc_Mark_Begin
//函 数 名：SetAtoB
//功    能：把点 A 给 B
//参    数：B, A
//    入口：A, Vector3d 变量，源点坐标
//    出口：B, Vector3d 变量，目标点坐标存放地址
//返    回：无
//调用方法：Vector3d A = …, B;
//			SetAtoB(B, A);
//GeoView_Doc_Mark_End
//主要思路：
//创建日期：2004/04/08	柳庆武
//修改日期：
void SetAtoB(Vector3d B, const Vector3d A)
//GeoView_Tech_Mark_End
{
	B[0] = A[0];
	B[1] = A[1];
	B[2] = A[2];
}


//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetVolumeOfPrism
// 功    能:	计算(斜、正、歪)三棱柱的体积V
// 参    数:	Vector3d A, Vector3d B, Vector3d C
//   (入口):	A:	三棱柱的顶面点,	Za >= 0;
//				B:	三棱柱的顶面点,	Zb >= 0;
//				C:	三棱柱的顶面点,	Zc >= 0;
//   (出口):	无
// 返    回:	double:	三棱柱的体积
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	三棱柱的底面在XOY上。
//				如果ABC平行于 XOY面(正斜三棱柱), 则 V = So * Za
//				如果ABC不平行于XOY, 则要求棱柱的边垂直于XOY, 则体积计算如下:
//					V = So * (Za + Zb + Zc)/3.0;
//				总之, 这种三棱柱的体积公式可以归结为:
//
//					V = So * (Za + Zb + Zc)/3.0;
//
//				其中So为三角形ABC的投影三角形面积
//				===================================================
//				应证了"简单就是真理", 如果Zb = 0, Zc = 0, 则就是三棱锥了
// 参    阅:	
// 创建日期:	2004-8-23, Wengzp
// 修改日期:	
//==================================================================//
double GetVolumeOfPrism(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	Vector3d Ao, Bo, Co;	// 分别为ABC在XOY上的投影
	double So, V;		// 投影三角形面积, 三棱柱体积
	Ao[0] = A[0], Ao[1] = A[1], Ao[2] = 0.0;
	Bo[0] = B[0], Bo[1] = B[1], Bo[2] = 0.0;
	Co[0] = C[0], Co[1] = C[1], Co[2] = 0.0;

	So = GetAreaOfTri(Ao, Bo, Co);
	V = So*(A[2] + B[2] + C[2]) / 3.0;

	return V;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetVolumeOfConvGon
// 功    能:	获取凸多边形的体积(多边形、垂面与XOY面围成的空间)
// 参    数:	Vector3d * pdData, unsigned int * ps
//   (入口):	pdData:		凸多边形的数据
//				ps:			凸多边形的拓扑结构数组
//   (出口):	无
// 返    回:	double:		该空间的体积
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2004-8-28, Wengzp
// 修改日期:	
//==================================================================//
double GetVolumeOfConvGon(Vector3d * pdData, unsigned int * ps)
////GeoView_Tech_Mark_End
{
	if ((!pdData) || (!ps))
	{
		return 0;
	}

	double dVol = 0;
	Vector3d A, B, C;
	SetAtoB(A, pdData[ps[1]]);
	for (unsigned int i = 2; i<ps[0]; i++)
	{
		SetAtoB(B, pdData[ps[i]]);
		SetAtoB(C, pdData[ps[i + 1]]);
		dVol += GetVolumeOfPrism(A, B, C);
	}

	return dVol;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	ReAdjustPointList
// 功    能:	重整点列
// 参    数:	unsigned int wCnt, Vector3d* pts, unsigned int wSize, std::vector<Vertex3d>& paAdjust, bool bSplineInterp
//   (入口):	wCnt:			点总数
//				pts:			点列数据
//				wSize:			调整后的点总数
//				paAdjust:		调整后的点列数据
//				bSplineInterp:	是否需要进行样条插值(缺省false)
//   (出口):	paAdjust:		调整后的点列数据
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2005-1-19, Wengzp
// 修改日期:	
//==================================================================//
void ReAdjustPointList(unsigned int wCnt, Vector3d* pts, unsigned int wSize, std::vector<Vertex3d>& paAdjust, bool bSplineInterp)
////GeoView_Tech_Mark_End
{
	if (wCnt == 0 || wSize == 0)	// 没作的必要
		return;
	paAdjust.clear();
	paAdjust.resize(wSize);

	unsigned int i = 0, j = 0, k = 0;
	if (bSplineInterp)
	{// 需进行样条插值

	}
	else
	{// 直接从样本点里获取"有价值点"
		if (wSize == wCnt)
		{// 直接复制数据即可
			for (i = 0; i<wCnt; i++)
			{
				paAdjust[i].x = pts[i][0];
				paAdjust[i].y = pts[i][1];
				paAdjust[i].z = pts[i][2];
			}
			return;
		}

		// 以距离为权
		double dist = 0.0, length = 0.0;
		std::vector<double> daDist;
		std::vector<int> waRepeat;
		waRepeat.resize(wCnt);
		daDist.resize(wCnt);
		daDist[0] = 0.0;
		for (i = 1; i<wCnt; i++)
		{
			dist = sqrt((pts[i][0] - pts[i - 1][0])*(pts[i][0] - pts[i - 1][0]) +
				(pts[i][1] - pts[i - 1][1])*(pts[i][1] - pts[i - 1][1]) +
				(pts[i][2] - pts[i - 1][2])*(pts[i][2] - pts[i - 1][2]));
			daDist[i] = dist;
			length += dist;
		}

		if (wSize>wCnt)
		{// 需要重复某些重要点
			int nReps = wSize - wCnt;
			for (i = 1, k = nReps; i<wCnt; i++)
			{
				j = Roundf(nReps*daDist[i] / length);
				j = j>k ? k : j;
				k -= j;
				waRepeat[i] = j + 1;
			}
			waRepeat[0] = k + 1;
			for (i = 0, k = 0; i<wCnt; i++)
			{
				for (j = 0; j<waRepeat[i]; j++, k++)
				{
					paAdjust[k].x = pts[i][0];
					paAdjust[k].y = pts[i][1];
					paAdjust[k].z = pts[i][2];
				}
			}
		}
		else// wSize<wCnt
		{
			for (j = 0; j<wSize; j++)
			{// 将最大距离的的点标记为 -1.0
				for (i = 1, k = 0, dist = daDist[0]; i<wCnt; i++)
				{
					if (daDist[i]>dist)
					{
						dist = daDist[i];
						k = i;
					}
				}
				daDist[k] = -1.0;
			}
			for (i = 0, k = 0; i<wCnt; i++, k++)
			{
				if (daDist[i]<0)
				{
					paAdjust[k].x = pts[i][0];
					paAdjust[k].y = pts[i][1];
					paAdjust[k].z = pts[i][2];
				}
			}
		}
		waRepeat.clear();
		daDist.clear();
	}
}

// 计算三角形坡度(单位: 度)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetSlopeGradient
// 功    能:	计算三角形坡度(单位: 度)
// 参    数:	Vector3d A, Vector3d B, Vector3d C
//   (入口):	A: 三角形的顶点
//				B: 三角形的顶点
//				C: 三角形的顶点
//   (出口):	无
// 返    回:	double: 坡度, 单位: 度
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	坡度是地面点的法线与铅垂线的夹角。对于三角形来说，坡度就
//				是单位法线的Z分量的反余弦。另外，坡度的范围是: 0度～90度
// 参    阅:	
// 创建日期:	2005-8-24, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetSlopeGradient(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	// 计算三角形的单位法线
	double dN[4] = { 1.0, 0.0, 0.0, 0.0 }, dGradient = 90.0;
	GetTriNormal(dN, A, B, C, true);
	if (dN[2]<0.0)
	{
		dN[2] = -dN[2];
	}

	// 计算三角形的坡度，单位: 弧度
	dGradient = acos(dN[2]);

	// 换算成度
	return Degreesf(dGradient);
}

// 计算三角形坡向(以+x轴为基准, 单位: 度)
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetSlopeDirection
// 功    能:	计算三角形坡向(以+x轴为基准, 单位: 度)
// 参    数:	Vector3d A, Vector3d B, Vector3d C
//   (入口):	A: 三角形的顶点
//				B: 三角形的顶点
//				C: 三角形的顶点
//   (出口):	无
// 返    回:	double: 坡度, 单位: 度, 以+x轴为基准
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	坡向是三角形的向上法线在水平面上的投影方向与正北的夹角, 正北0度,
//				正东90度, 范围: 0度～360度, 这里以+y向为正北
// 参    阅:	
// 创建日期:	2005-8-24, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetSlopeDirection(Vector3d A, Vector3d B, Vector3d C)
////GeoView_Tech_Mark_End
{
	// 计算三角形的单位法线, 并换成坡的向上的法线
	double dN[4] = { 1.0, 0.0, 0.0, 0.0 }, dDirection = 0.0;
	GetTriNormal(dN, A, B, C, false);
	if (dN[2]<0.0)
	{
		dN[0] = -dN[0];
		dN[1] = -dN[1];
		dN[2] = -dN[2];
	}

	// 计算三角形的坡向
	dDirection = Degreesf(atan2(dN[0], dN[1]));
	if (dDirection<0.0)
	{
		dDirection += 360.0;
	}

	return dDirection;
}

// 计算线段到线段的距离
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetDistanceSegment2Segment
// 功    能:	计算线段到线段的距离
// 参    数:	const Vector3d A, const Vector3d B, 
//				const Vector3d C, const Vector3d D, 
//				Vector3d P, Vector3d Q
//   (入口):	A, B: 线段1的两个端点
//				C, D: 线段2的两个端点
//				P:	  最短距离时线段1上的点
//				Q:	  最短距离时线段2上的点
//   (出口):	P:	  最短距离时线段1上的点
//				Q:	  最短距离时线段2上的点
// 返    回:	两条线段之间的距离
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	距离函数: Q(s,t) = as^2 + 2bst + ct^2 + 2ds + 2et + f (s∈[0, 1], t∈[0, 1])
//				如果两条线段不平行, 则Q的图形是抛物柱面。根据s, t的取值, 分区为:
//						4  |  3  |  2
//						--------------
//						5  |  0  |  1
//						--------------
//						6  |  7  |  8
//				分为三类: 1) 0区;	2) 1,3,5,7; 3) 2,4,6,9
//				根据梯度以及偏导等最值原理推倒出线段之间的距离
// 参    阅:	《计算机图形学几何工具算法详解》, page 310.
// 创建日期:	2005-12-21, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetDistanceSegment2Segment(const Vector3d A, const Vector3d B,
	const Vector3d C, const Vector3d D,
	Vector3d P, Vector3d Q)
	////GeoView_Tech_Mark_End
{
	double dDist = 0.0;
	Vector3d M0, M1, M;
	AsubB(B, A, M0);// M0是从A指向B的矢量
	AsubB(D, C, M1);// M1是从C指向D的矢量
	AsubB(A, C, M);	// M是从线段2的端点C指向线段1的端点A的矢量
	double a = AdotB(M0, M0), b = -AdotB(M0, M1), c = AdotB(M1, M1),
		d = AdotB(M0, M), e = -AdotB(M1, M), f = AdotB(M, M),
		s = 0.0, t = 0.0;

	double delta = a*c - b*b;
	if (delta>0.0)// 则两条线段不平行
	{
		s = (b*e - c*d) / delta;
		t = (b*d - a*e) / delta;
		if (s >= 0.0 && s <= 1.0)
		{
			if (t>1.0)	// Zone 3
			{
				s = -(b + d) / a;
			}
			else if (t<0.0)	// Zone 7
			{
				s = -d / a;
			}
		}
		else if (s>1.0)
		{
			if (t >= 0.0 && t <= 1.0)	// Zone 1
			{
				t = -(b + e) / c;
			}
			else if (t>1.0)	// Zone 2
			{
				if (a + b + d>0.0)	// if Qs(1, 1)>0.0, then t=1.0, 同3
				{
					s = -(b + d) / a;
				}
				else			// if Qt(1, 1)>0.0, then s=1.0, 同1
				{
					t = -(b + e) / c;
				}
			}
			else// t<0.0	// Zone 8
			{
				if (a + d>0.0)	// if Qs(1, 0)>0.0, then t=0.0, 同7
				{
					s = -d / a;
				}
				else	// if Qt(1, 0)>0.0, then s = 1.0, 同1
				{
					t = -(b + e) / c;
				}
			}
		}
		else// s<0.0
		{
			if (t >= 0.0 && t <= 1.0)	// Zone 5
			{
				t = -e / c;
			}
			else if (t>1.0)	// Zone 4
			{
				if (b + d>0.0)	// if Qs(0, 1)>0.0, then t=1.0, 同3
				{
					s = -(b + d) / a;;
				}
				else	// if Qt(0, 1)>0.0, then s=0.0, 同5
				{
					t = -e / c;
				}
			}
			else// t<0.0	// Zone 6
			{
				if (d>0.0)	// if Qs(0, 0)>0.0, then t=0.0, 同7
				{
					s = -d / a;
				}
				else	// if Qt(0, 0)>0.0, then s=0.0, 同5
				{
					t = -e / c;
				}
			}
		}
		if (s>1.0)
		{
			s = 1.0;
		}
		else if (s<0.0)
		{
			s = 0.0;
		}
		if (t>1.0)
		{
			t = 1.0;
		}
		else if (t<0.0)
		{
			t = 0.0;
		}

	}
	else// 两条线段平行
	{
		if (b>0.0)
		{
			if (d >= 0.0)
			{
				s = 0.0;
				t = 0.0;
			}
			else if (-d <= a)
			{
				s = -d / a;
				t = 0.0;
			}
			else
			{
				s = 1.0;
				double tmp = -(a + d);
				if (tmp >= b)
				{
					t = 1.0;
				}
				else
				{
					t = tmp / b;
				}
			}
		}
		else
		{
			if (-d >= a)
			{
				s = 1.0;
				t = 0.0;
			}
			else if (d <= 0.0)
			{
				s = -d / a;
				t = 0.0;
			}
			else
			{
				s = 0.0;
				if (d >= -b)
				{
					t = 1.0;
				}
				else
				{
					t = -d / b;
				}
			}
		}
	}
	P[0] = A[0] + s*M0[0];	P[1] = A[1] + s*M0[1];	P[2] = A[2] + s*M0[2];
	Q[0] = C[0] + t*M1[0];	Q[1] = C[1] + t*M1[1];	Q[2] = C[2] + t*M1[2];
	return GetDistance(P, Q);
}

// 获取三棱柱的库容(算法见库容计算模型之地形法)
//		A:			三角形的A点(没有经过调整的)
//		B:			三角形的B点(没有经过调整的)
//		C:			三角形的C点(没有经过调整的)
//		z:			计算高程
//		fpArea:		接触表面积(可能返回)
// 注:
//		详细算法可见"6.4.2库容计算模型——6.4.2.2地形法"
//	返回: 库容
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetGrooveOfTriprism
// 功    能:	获取三棱柱的库容
// 参    数:	Vector3d A, Vector3d B, Vector3d C, float z, double* fpArea
//   (入口):	A:			三角形的A点(没有经过调整的)
//				B:			三角形的B点(没有经过调整的)
//				C:			三角形的C点(没有经过调整的)
//				z:			计算高程
//				dpArea:		接触表面积(可能返回)
//   (出口):	dpArea:		接触表面积
// 返    回:	库容
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:
// 参    阅:
// 创建日期:	2004-1-12
// 修改日期:	2004-1-12
//==================================================================//
double GetGrooveOfTriprism(Vector3d A, Vector3d B, Vector3d C, double z, double* dpArea)
////GeoView_Tech_Mark_End
{
	double vol = 0.0,	// 库容
		dArea = 0.0;	// 接触表面积

	// 调整使得 Za >= Zb >= Zc;
	double dTemp;
	if (A[2]<B[2])
	{
		dTemp = A[0]; A[0] = B[0]; B[0] = dTemp;
		dTemp = A[1]; A[1] = B[1]; B[1] = dTemp;
		dTemp = A[2]; A[2] = B[2]; B[2] = dTemp;
	}
	if (C[2]>A[2])
	{
		dTemp = A[0]; A[0] = C[0]; C[0] = dTemp;
		dTemp = A[1]; A[1] = C[1]; C[1] = dTemp;
		dTemp = A[2]; A[2] = C[2]; C[2] = dTemp;
	}
	if (C[2]>B[2])
	{
		dTemp = C[0]; C[0] = B[0]; B[0] = dTemp;
		dTemp = C[1]; C[1] = B[1]; B[1] = dTemp;
		dTemp = C[2]; C[2] = B[2]; B[2] = dTemp;
	}
	// 三角形的三边长
	double a = sqrt((B[0] - C[0])*(B[0] - C[0]) + (B[1] - C[1])*(B[1] - C[1]) + (B[2] - C[2])*(B[2] - C[2])),
		b = sqrt((A[0] - C[0])*(A[0] - C[0]) + (A[1] - C[1])*(A[1] - C[1]) + (A[2] - C[2])*(A[2] - C[2])),
		c = sqrt((B[0] - A[0])*(B[0] - A[0]) + (B[1] - A[1])*(B[1] - A[1]) + (B[2] - A[2])*(B[2] - A[2]));
	double l = (a + b + c) / 2.0f;
	double dAreaTri = sqrt(l*(l - a)*(l - b)*(l - c));	// 三角形的面积
	double Za = A[2], Zb = B[2], Zc = C[2],	// 存在关系: Za >= Zb >= Zc
		sinA = 2.0*dAreaTri / (b*c),
		sinC = 2.0*dAreaTri / (a*b),
		bh = 2.0*dAreaTri / b;

	Vector3d P = { C[0], C[1], z }, Q;
	double dH = GetPntVerticalPlane(A, B, C, P, Q);

	//////////////////////[2006-1-15, 13:50, Wengzp]//////////////+
	//TODO: 在这里写添加代码的目的
	Vector3d Ao, Bo, Co;	// 分别为ABC在XOY上的投影
	double So;		// 投影三角形面积
	Ao[0] = A[0], Ao[1] = A[1], Ao[2] = 0.0;
	Bo[0] = B[0], Bo[1] = B[1], Bo[2] = 0.0;
	Co[0] = C[0], Co[1] = C[1], Co[2] = 0.0;
	So = GetAreaOfTri(Ao, Bo, Co);
	//////////////////////[2006-1-15, 13:50, Wengzp]//////////////-

	if (So>TOLERANCE && dH > TOLERANCE)
	{
		if (z <= Zc)
		{
			dArea = 0.0f;
			vol = 0.0;
		}
		else if (z <= Zb)
		{
			dArea = a*b*(z - Zc)*(z - Zc)*sinC / (2.0*(Zb - Zc)*(Za - Zc));
			vol = dArea*dH / 3.0;
		}
		else if (z <= Za)
		{
			dArea = b*c*(1.0 - (Za - z)*(Za - z) / ((Za - Zb)*(Za - Zc)))*sinA / 2.0;
			vol = So*(Za - Zb + Za - Zc) / 3.0 - So*(Za - z)
				+ (Za - z)*(dAreaTri - dArea)*So / dAreaTri / 3.0;
		}
		else
		{
			dArea = dAreaTri;
			vol = So*(z - Za + z - Zb + z - Zc) / 3.0;
		}
	}

	if (dpArea)
		*dpArea = dArea;
	return vol;
}


//计算中值
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetMiddle
// 功    能:	统计中值
// 参    数:	std::vector<double>& daX
//   (入口):	daX:	样本点数组

//   (出口):	无
// 返    回:	double:	中值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-4-3, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double GetMiddle(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	int i, j, n, m;
	double temp;
	n = daX.size() - 1;
	for (j = 0; j<n; j++)
	{
		m = n - j;
		for (i = 0; i<m; i++)
		{
			if (daX[i]>daX[i + 1])
			{
				temp = daX[i];
				daX[i] = daX[i + 1];
				daX[i + 1] = temp;
			}
		}
	}
	if (n % 2 == 0)
	{
		return (daX[n / 2 - 1] + daX[n / 2]) / 2;
	}
	else
		return daX[(n - 1) / 2];


}

//计算一维数组最值
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetMinMax
// 功    能:    求最值
// 参    数:	std::vector<double>& daX
//   (入口):	daX:	样本点数组
//              dMinMax[2]: 存放数组的最小最大值

//   (出口):	无
// 返    回:	double:	最小最大值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-5-23, zhangdan: add notes.
// 修改日期:	
//==================================================================//
void GetMinMax(double dMinMax[2], std::vector<double>& daX)
//GeoView_Tech_Mark_End
{
	double dMin, dMax;
	int n = daX.size();
	dMin = dMax = daX[0];

	for (int i = 1; i<n; i++)
	{
		if (daX[i]>dMax)
		{
			dMax = daX[i];
		}

		if (daX[i]<dMin)
		{
			dMin = daX[i];
		}
	}

	dMinMax[0] = dMin;
	dMinMax[1] = dMax;
}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetAverage
// 功    能:	统计平均值
// 参    数:	std::vector<double>& daZ, std::vector<double>* paAlpha
//   (入口):	daZ:		样本点数组
//				paAlpha:	权系数数组(必须与样本点数组同样大小, 否则就计算算术平均)指针, 
//							如为NULL, 则计算算术平均
//   (出口):	无
// 返    回:	double:	平均值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-1-18, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetAverage(std::vector<double>& daZ, std::vector<double>* paAlpha)
////GeoView_Tech_Mark_End
{
	double dAverage = 0.0;
	int i = 0,
		n = daZ.size();
	if (n>0)
	{
		if (paAlpha && (*paAlpha).size() == n)
		{// 加权平均
			for (i = 0; i<n; i++)
			{
				dAverage += (*paAlpha)[i] * daZ[i];
			}
		}
		else
		{// 算术平均
			for (i = 0; i<n; i++)
			{
				dAverage += daZ[i];
			}
			dAverage /= n;
		}
	}
	return dAverage;
}

// 计算方差
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetVariance
// 功    能:	计算方差
// 参    数:	std::vector<double>& daX
//   (入口):	daX:	样本点数组
//   (出口):	无
// 返    回:	double:	样本方差
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	方差有些是除以(n-1), 这里是除以n
// 参    阅:	http://www.huanggao.net/course/40315/ws/SX_13_01_020/
// 创建日期:	2006-1-18, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetVariance(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	double S2 = 0.0, dAver = 0.0;
	int i = 0, n = daX.size();
	if (n>0)
	{
		dAver = GetAverage(daX, 0);
		for (i = 0; i<n; i++)
		{
			S2 += daX[i] * daX[i];
		}
		S2 = S2 / n - dAver*dAver;
	}

	return S2;
}

// 计算标准差
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	GetStandardDeviation
// 功    能:	计算标准差
// 参    数:	std::vector<double>& daX
//   (入口):	daX:	样本点数组
//   (出口):	无
// 返    回:	double:	样本标准差
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	方差有些是除以(n-1), 这里是除以n
// 参    阅:	
// 创建日期:	2006-1-18, Wengzp: add notes.
// 修改日期:	
//==================================================================//
double GetStandardDeviation(std::vector<double>& daX)
////GeoView_Tech_Mark_End
{
	double S2 = GetVariance(daX);
	return sqrt(S2);
}


//伽马函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	lgam1
// 功    能:	计算伽马函数值（积分值）
// 参    数:	 X
//   (入口):	 X:	     实变量
//   (出口):	 无
// 返    回:	double:	 伽马函数值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	计算伽马函数值（积分）——用切比雪夫多项式逼近
// 参    阅:    徐士良. C常用算法程序集(Edition 2). 北京: 清华大学出版社.1996
// 创建日期:	2006-4-5, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double lgam1(double x)
////GeoView_Tech_Mark_End
{
	int i;
	double y, t, s, u;
	static double a[11] = { 0.0000677106, -0.0003442342,
		0.0015397681, -0.0024467480, 0.0109736958,
		-0.0002109075, 0.0742379071, 0.0815782188,
		0.4118402518, 0.4227843370, 1.0 };
	if (x <= 0.0)
	{
		printf("err**x<=0!\n"); return(-1.0);
	}
	y = x;
	if (y <= 1.0)
	{
		t = 1.0 / (y*(y + 1.0)); y = y + 2.0;
	}
	else if (y <= 2.0)
	{
		t = 1.0 / y; y = y + 1.0;
	}
	else if (y <= 3.0) t = 1.0;
	else
	{
		t = 1.0;
		while (y>3.0)
		{
			y = y - 1.0; t = t*y;
		}
	}
	s = a[0]; u = y - 2.0;
	for (i = 1; i <= 10; i++)
		s = s*u + a[i];
	s = s*t;
	return(s);
}


//不完全伽马函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	lgam2
// 功    能:	计算伽马函数值（积分值）
// 参    数:	 X
//   (入口):	 a :    实变量
//               X :	实变量
//   (出口):	 无
// 返    回:	double:	 不完全伽马函数值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:    徐士良. C常用算法程序集(Edition 2). 北京: 清华大学出版社.1996
// 创建日期:	2006-4-5, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double lgam2(double a, double x)
////GeoView_Tech_Mark_End
{
	int n;
	double p, q, d, s, s1, p0, q0, p1, q1, qq;
	//   extern double lgam1();
	if ((a <= 0.0) || (x<0.0))
	{
		if (a <= 0.0) printf("err**a<=0!\n");
		if (x<0.0) printf("err**x<0!\n");
		return(-1.0);
	}
	if (x + 1.0 == 1.0) return(0.0);
	if (x>1.0e+35) return(1.0);
	q = log(x); q = a*q; qq = exp(q);
	if (x<1.0 + a)
	{
		p = a; d = 1.0 / a; s = d;
		for (n = 1; n <= 100; n++)
		{
			p = 1.0 + p; d = d*x / p; s = s + d;
			if (fabs(d)<fabs(s)*1.0e-07)
			{
				s = s*exp(-x)*qq / lgam1(a);
				return(s);
			}
		}
	}
	else
	{
		s = 1.0 / x; p0 = 0.0; p1 = 1.0; q0 = 1.0; q1 = x;
		for (n = 1; n <= 100; n++)
		{
			p0 = p1 + (n - a)*p0; q0 = q1 + (n - a)*q0;
			p = x*p0 + n*p1; q = x*q0 + n*q1;
			if (fabs(q) + 1.0 != 1.0)
			{
				s1 = p / q; p1 = p; q1 = q;
				if (fabs((s1 - s) / s1)<1.0e-07)
				{
					s = s1*exp(-x)*qq / lgam1(a);
					return(1.0 - s);
				}
				s = s1;
			}
			p1 = p; q1 = q;
		}
	}
	printf("a too large !\n");
	s = 1.0 - s*exp(-x)*qq / lgam1(a);
	return(s);
}


// 误差函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	lerrf
// 功    能:	计算误差函数值
// 参    数:	X
//   (入口): 	X:	 实变量
//   (出口):	无
// 返    回:	double:	误差值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	徐士良. C常用算法程序集(Edition 2). 北京: 清华大学出版社.1996	
// 创建日期:	2006-4-5, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double lerrf(double x)
////GeoView_Tech_Mark_End
{
	double y;
	//    extern double lgam2();
	if (x >= 0.0)
		y = lgam2(0.5, x*x);
	else
		y = -lgam2(0.5, x*x);
	return(y);

}

// 正态分布函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	lgass
// 功    能:	计算正态分布函数值(累次积分）
// 参    数:	a , d, X
//   (入口):	 X:	  随机变量数
//               a :  期望（均值）
//               d :  标准差
//   (出口):	无
// 返    回:	double:	正态分布函数值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-4-5, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double lgass(double a, double d, double x)
////GeoView_Tech_Mark_End
{
	double y;
	if (d <= 0.0) d = 1.0e-10;
	y = 0.5 + 0.5*lerrf((x - a) / (sqrt(2.0)*d));
	return(y);

}


// 正态分布函数
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	NormalFunction
// 功    能:	计算正态分布函数值
// 参    数:	a , d, X
//   (入口):	 X:	  随机变量数
//               a :  期望（均值）
//               d :  标准差
//   (出口):	无
// 返    回:	double:	正态分布函数值
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-4-6, zhangdan: add notes.
// 修改日期:	
//==================================================================//
double NormalFunction(double a, double d, double x)
////GeoView_Tech_Mark_End
{
	double y, pi;
	pi = 3.1415926;
	if (d <= 0)
		return 0;
	else
	{
		y = (exp(-(x - a)*(x - a) / (2 * d*d))) / (d*sqrt(2 * pi));
		return y;
	}

}


/*
generate a B-spline open knot vector with multiplicity
equal to the order at the ends.

c            = order of the basis function
n            = the number of defining polygon vertices
nplus2       = index of x() for the first occurence of the maximum knot vector value
nplusc       = maximum value of the knot vector -- $n + c$
x()          = array containing the knot vector
*/

void Knot(int n, int c, int * x)
{
	int nplusc, nplus2, i;
	nplusc = n + c;
	nplus2 = n + 2;

	x[1] = 0;
	for (i = 2; i <= nplusc; i++)
	{
		if ((i > c) && (i < nplus2))
			x[i] = x[i - 1] + 1;
		else
			x[i] = x[i - 1];
	}
}

/*  Subroutine to generate rational B-spline basis functions--open knot vector

C code for An Introduction to NURBS
by David F. Rogers. Copyright (C) 2000 David F. Rogers,
All rights reserved.

Name: rbais
Language: C
Subroutines called: none
Book reference: Chapter 4, Sec. 4. , p 296

c        = order of the B-spline basis function
d        = first term of the basis function recursion relation
e        = second term of the basis function recursion relation
h[]	     = array containing the homogeneous weights
npts     = number of defining polygon vertices
nplusc   = constant -- npts + c -- maximum number of knot values
r[]      = array containing the rationalbasis functions
r[1] contains the basis function associated with B1 etc.
t        = parameter value
temp[]   = temporary array
x[]      = knot vector
*/
void RBasis(int c, double t, int npts, int * x, double * h, double * r)
{
	int nplusc;
	int i, j, k;
	double d, e;
	double sum;
	double * temp = new double[npts + c + 1];

	nplusc = npts + c;

	//calculate the first order nonrational basis functions n[i]
	for (i = 1; i <= nplusc - 1; i++)
	{
		if ((t >= x[i]) && (t < x[i + 1]))
			temp[i] = 1;
		else
			temp[i] = 0;
	}

	//calculate the higher order nonrational basis functions
	for (k = 2; k <= c; k++)
	{
		for (i = 1; i <= nplusc - k; i++)
		{
			if (temp[i] != 0)    /* if the lower order basis function is zero skip the calculation */
				d = ((t - x[i])*temp[i]) / (x[i + k - 1] - x[i]);
			else
				d = 0;

			if (temp[i + 1] != 0)     /* if the lower order basis function is zero skip the calculation */
				e = ((x[i + k] - t)*temp[i + 1]) / (x[i + k] - x[i + 1]);
			else
				e = 0;

			temp[i] = d + e;
		}
	}

	if (t == (double)x[nplusc])
	{		/*    pick up last point	*/
		temp[npts] = 1;
	}

	//calculate sum for denominator of rational basis functions
	sum = 0.0;
	for (j = 1; j <= npts; j++)
	{
		sum = sum + temp[j] * h[j];
	}

	/* form rational basis functions and put in r vector */

	for (i = 1; i <= npts; i++)
	{
		if (sum != 0)
		{
			r[i] = (temp[i] * h[i]) / (sum);
		}
		else
			r[i] = 0;
	}
	if (temp)
	{
		delete[] temp;
		temp = NULL;
	}
}


//generate a rational B-spline curve using an uniform open knot vector

void CalRBSpline(int npts, int k, int p1, std::vector<double>& arConPt,
	std::vector<double> & arCurvePt)//std::vector<double> & xAr,
{
	/*  Subroutine to generate a rational B-spline curve using an uniform open knot vector

	C code for An Introduction to NURBS
	by David F. Rogers. Copyright (C) 2000 David F. Rogers,
	All rights reserved.

	Name: rbspline.c
	Language: C
	Subroutines called: knot.c, rbasis.c, fmtmul.c
	Book reference: Chapter 4, Alg. p. 297

	b[]         = array containing the defining polygon vertices
	b[1] contains the x-component of the vertex
	b[2] contains the y-component of the vertex
	b[3] contains the z-component of the vertex
	h[]			= array containing the homogeneous weighting factors
	k           = order of the B-spline basis function
	nbasis      = array containing the basis functions for a single value of t
	nplusc      = number of knot values
	npts        = number of defining polygon vertices
	p[,]        = array containing the curve points
	p[1] contains the x-component of the point
	p[2] contains the y-component of the point
	p[3] contains the z-component of the point
	p1          = number of points to be calculated on the curve
	t           = parameter value 0 <= t <= npts - k + 1
	x[]         = array containing the knot vector
	*/

	//we can calculate the NURBUS curve,from this function
	if (npts <= 0)
	{
		return;
	}
	int i = 0;
	int number = 0;
	number = arConPt.size();
	if (number != npts * 3)
	{
		return;
	}
	double * b = new double[number + 1];
	for (i = 1; i <= number; i++)
	{
		b[i] = arConPt[i - 1];
	}
	double * h = new double[npts + 1];
	for (i = 1; i <= npts; i++)
	{
		h[i] = 1.0;
	}
	double * p = new double[p1 * 3 + 1];
	for (i = 1; i <= p1 * 3; i++)
	{
		p[i] = 0.0;
	}

	//up to now, we had finished the ready work,now we will get the point in curve
	int j, icount, jcount;
	int i1;
	int * x = new int[npts + k + 1];		/* allows for 20 data points with basis function of order 5 */
	int nplusc;

	double step;
	double t;
	double * nbasis = new double[npts + 1];
	double temp;

	nplusc = npts + k;

	//zero and redimension the knot vector and the basis array
	for (i = 0; i <= npts; i++)
	{
		nbasis[i] = 0.0;
	}
	//	int nxAr = xAr.size();
	//	for (i=0;i<nxAr;i++)
	//	{
	//		x[i] = xAr.GetAt(i);
	//	}
	//generate the uniform open knot vector
	Knot(npts, k, x);
	icount = 0;
	/*    calculate the points on the rational B-spline curve */

	t = 0;
	step = ((double)x[nplusc]) / ((double)(p1 - 1));

	for (i1 = 1; i1 <= p1; i1++)
	{

		if ((double)x[nplusc] - t < 5e-6)
		{
			t = (double)x[nplusc];
		}

		RBasis(k, t, npts, x, h, nbasis);      /* generate the basis function for this value of t */
		for (j = 1; j <= 3; j++)
		{      /* generate a point on the curve */
			jcount = j;
			p[icount + j] = 0.0;

			for (i = 1; i <= npts; i++)
			{ /* Do local matrix multiplication */
				temp = nbasis[i] * b[jcount];
				p[icount + j] = p[icount + j] + temp;

				jcount = jcount + 3;
			}
		}

		icount = icount + 3;
		t = t + step;
	}

	//get the point coordinate
	arCurvePt.clear();
	for (i = 1; i <= p1 * 3; i = i + 3)
	{
		arCurvePt.push_back(p[i]);
		arCurvePt.push_back(p[i + 1]);
		arCurvePt.push_back(p[i + 2]);
	}
	int nNumber = arCurvePt.size();
	//	if ((nNumber%3) != 0)
	//	{
	//		AfxMessageBox("erro",MB_OK);
	//	}

	//clear the memery
	if (b)
	{
		delete[] b;
		b = NULL;
	}
	if (x)
	{
		delete[] x;
		x = NULL;
	}
	if (p)
	{
		delete[] p;
		p = NULL;
	}
	if (h)
	{
		delete[] h;
		h = NULL;
	}
	if (nbasis)
	{
		delete[] nbasis;
		nbasis = NULL;
	}

}


//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	IsPntInGon3D
// 功    能:	判断点 pt 是否在多边形内
// 参    数:	int nNum, Vector3d * pData, Vector3d pt
//				nNum	节点个数
//				pData	节点数组
//				pt		待判断的节点
//   (入口):	无
//   (出口):	无
// 返    回:	如果在内部则返回true，否则返回false
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	矢量方法判断节点是否在多边形内部，在边上也认为在内部
// 参    阅:	
// 创建日期:	2006-7-20, Zhangzhiting: add notes.
// 修改日期:	
//==================================================================//
bool IsPntInGon3D(int nNum, Vector3d * pData, Vector3d pt)
////GeoView_Tech_Mark_End
{
	//符合原则
	if (nNum < 3 || pData == NULL)
	{
		return false;
	}
	//
	//判断是否在节点位置
	/*
	Vector3d ptA,ptB;
	Vector3d v1,v2,vv1,vv2;
	double lfDis = 0.0;
	for (int i=0;i<nNum;i++)
	{
	ptA[0] = pData[i][0],ptA[1] = pData[i][1],ptA[2] = pData[i][2];
	lfDis = GetDistance(ptA,pt);
	if (lfDis < 0.00001)
	{
	return true;
	}
	}

	//获取一个初始的矢量，用于判断
	ptA[0] = pData[0][0],ptA[1] = pData[0][1],ptA[2] = pData[0][2];
	ptB[0] = pData[nNum-1][0],ptB[1] = pData[nNum-1][1],ptB[2] = pData[nNum-1][2];
	AsubB(ptA,pt,vv1);
	AsubB(ptB,pt,vv2);
	AxB(vv1,vv2,v1);
	double lflenA = VecLen(v1);
	double lfAngle = 0.0;
	for (i=0;i<nNum-1;i++)
	{
	ptA[0] = pData[i][0],ptA[1] = pData[i][1],ptA[2] = pData[i][2];
	ptB[0] = pData[i+1][0],ptB[1] = pData[i+1][1],ptB[2] = pData[i+1][2];
	AsubB(ptA,pt,vv1);
	AsubB(ptB,pt,vv2);
	AxB(vv1,vv2,v2);

	lfAngle = AdotB(v1,v2);

	double lflenB = VecLen(v2);
	lflenA = lflenA/(lflenA*lflenB);
	if (lfAngle <= 0)
	{
	return false;
	}
	}
	return true;
	*/
	//用夹角和判断，这个可以判断凹多边形
	Vector3d v1, v2, ptA, ptB;
	double lfAngle = 0.0;
	double lfAllAngle = 0.0;
	Vector3d vvvOne, vvv;
	int i = 0;//循环变量
	for (i = 0; i<nNum; i++)
	{
		ptA[0] = pData[i][0], ptA[1] = pData[i][1], ptA[2] = pData[i][2];
		if (i == nNum - 1)
		{
			ptB[0] = pData[0][0], ptB[1] = pData[0][1], ptB[2] = pData[0][2];
		}
		else
		{
			ptB[0] = pData[i + 1][0], ptB[1] = pData[i + 1][1], ptB[2] = pData[i + 1][2];
		}
		AsubB(ptA, pt, v1);
		AsubB(ptB, pt, v2);
		VecNormalize(v1);
		VecNormalize(v2);
		AxB(v1, v2, vvvOne);
		VecNormalize(vvvOne);//向量归一化
		if (VecLen(vvvOne)>0.000001)
		{
			break;
		}
	}
	for (i = 0; i<nNum; i++)
	{
		ptA[0] = pData[i][0], ptA[1] = pData[i][1], ptA[2] = pData[i][2];
		if (i == nNum - 1)
		{
			ptB[0] = pData[0][0], ptB[1] = pData[0][1], ptB[2] = pData[0][2];
		}
		else
		{
			ptB[0] = pData[i + 1][0], ptB[1] = pData[i + 1][1], ptB[2] = pData[i + 1][2];
		}
		AsubB(ptA, pt, v1);
		AsubB(ptB, pt, v2);
		VecNormalize(v1);
		VecNormalize(v2);
		lfAngle = AdotB(v1, v2);

		AxB(v1, v2, vvv);
		VecNormalize(vvv);//向量归一化
		double dRes = AdotB(vvv, vvvOne);
		double fRadius = 0.0;
		if (lfAngle>-1.0 && lfAngle<1.0)
		{
			fRadius = acos(lfAngle);
		}
		else if (lfAngle <= -1.0)
		{
			fRadius = PI;
		}
		else if (lfAngle >= 1.0)
		{
			fRadius = 0;
		}
		if (fabs(dRes)<0.000001)
		{
			continue;
		}
		if (dRes<0)
		{
			lfAllAngle = lfAllAngle - fRadius;
		}
		else
		{
			lfAllAngle = lfAllAngle + fRadius;
		}
	}
	if (fabs(fabs(lfAllAngle) - 2 * PI) < 0.000001)
	{
		return true;
	}
	else
	{
		return false;
	}

}

//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	MayLineIntersect
// 功    能:	判断二维平面上的两点是否有可能相交，以加快两直线求交的速度
// 参    数:	Vector3d A,Vector3d B,   线段AB的两端点
//              Vector3d C,Vector3d D    线段CD的两端点
//   (入口):	无
//   (出口):	无
// 返    回:	可能相交:true, 不可能:false
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	根据A，B 构建直线公式：aX+bY+c=0;
//              如果(a*C[0]+b*C[1]+c)(a*D[0]+b*D[1]+c)<0，则说明C，D在AB的两侧，则有可能相交
//                        
// 参    阅:	初中数学课本
// 创建日期:	2006-9-4, 孙卡, add notes.
// 修改日期:	
//==================================================================//
bool MayLineIntersect(Vector3d A, Vector3d B, Vector3d C, Vector3d D)
////GeoView_Tech_Mark_End
{
	//根据AB两点，构件aX+bY+c=0方程
	double a = 0;
	double b = 0;
	double c = 0;
	a = B[1] - A[1];
	b = A[0] - B[0];
	c = A[1] * B[0] - B[1] * A[0];
	double dResault = 0;
	dResault = (a*C[0] + b*C[1] + c)*(a*D[0] + b*D[1] + c);
	if (dResault<0)
	{
		return true;
	}
	else
	{
		return false;
	}
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	XYZListToPnt3dList(std::vector<Vertex3d> XYZList,unsigned int uDataSize,Vector3d *Pnt3dList);
// 功    能:	实现Vertex3d的链表到Vector3d链表之间的转换
// 参    数:	无
//   (入口):	无
//   (出口):	无
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-8-18, 孙卡, add notes.
// 修改日期:	
//==================================================================//
////GeoView_Tech_Mark_End
bool XYZListToPnt3dList(std::vector<Vertex3d> &XYZList, int nDataSize, Vector3d *Pnt3dList)
{
	if (XYZList.size()<nDataSize)
	{
		return false;
	}
	for (int i = 0; i<nDataSize; i++)
	{
		Pnt3dList[i][0] = XYZList[i].x;
		Pnt3dList[i][1] = XYZList[i].y;
		Pnt3dList[i][2] = XYZList[i].z;
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	Pnt3dListToXYZList(POINT *Pnt3dList,unsigned int uDataSize,std::vector<Vertex3d> XYZList);
// 功    能:    实现Vector3d链表到Vertex3d链表之间的转换	
// 参    数:	无
//   (入口):	无
//   (出口):	无
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-8-18, 孙卡, add notes.
// 修改日期:	
//==================================================================//
////GeoView_Tech_Mark_End
bool Pnt3dListToXYZList(Vector3d *Pnt3dList, int nDataSize, std::vector<Vertex3d> &XYZList)
{
	if (nDataSize <= 0)
	{
		return false;
	}
	for (int i = 0; i<nDataSize; i++)
	{
		Vertex3d pntTemp;
		pntTemp.x = Pnt3dList[i][0];
		pntTemp.y = Pnt3dList[i][1];
		pntTemp.z = Pnt3dList[i][2];
		XYZList.push_back(pntTemp);
	}
	return true;
}
//////////////////////////////////////////////////////////////////////////
////GeoView_Tech_Mark_Begin
//==================================================================//
//GeoView_Doc_Mark_Begin											//
// 函 数 名:	
// 功    能:	
// 参    数:	无
//   (入口):	无
//   (出口):	无
// 返    回:	无
// 调用方法:	
//GeoView_Doc_Mark_End  											//
// 主要思路:	
// 参    阅:	
// 创建日期:	2006-9-8, 孙卡, add notes.
// 修改日期:	
//==================================================================//
////GeoView_Tech_Mark_End
void PntACopyB(Vector3d A, Vector3d B)
{
	//	A[0]=B[0];	
	//	A[1]=B[1;
	//	A[2]=B[2;
	memcpy(A, B, sizeof(Vector3d));
}
//
////////////////////////////////////////////////////////////////////////////
//////GeoView_Tech_Mark_Begin
////==================================================================//
////GeoView_Doc_Mark_Begin											//
//// 函 数 名:	IsSameEdge
//// 功    能:	
//// 参    数:	EDGE ed1,EDGE ed2
////   (入口):	EDGE ed1,EDGE ed2
////   (出口):	无
//// 返    回:	bool
//// 调用方法:	
////GeoView_Doc_Mark_End  											//
//// 主要思路:	
//// 参    阅:	
//// 创建日期:	2006-9-29, ZhangZhiting: add notes.
//// 修改日期:	
////==================================================================//
// bool IsSameEdge(EDGE ed1,EDGE ed2)
//////GeoView_Tech_Mark_End
//{
//	if (ed1.p1 == ed2.p1 && ed1.p2 == ed2.p2)
//	{
//		return true;
//	}
//	if (ed1.p1 == ed2.p2 && ed1.p2 == ed2.p1)
//	{
//		return true;
//	}
//	return false;
//}
//



/************************************************************************/
/*                            DMSToDegree()                             */
/************************************************************************/

static const char*sym = "NnEeSsWw";
static const double vm[] = { 1.0, 0.0166666666667, 0.00027777778 };
/************************************************************************/
/*                            proj_strtod()                             */
/************************************************************************/
static double proj_strtod(char *nptr, char **endptr)

{
	char c, *cp = nptr;
	double result;

	/*
	* Scan for characters which cause problems with VC++ strtod()
	*/
	while ((c = *cp) != '\0') {
		if (c == 'd' || c == 'D') {

			/*
			* Found one, so NUL it out, call strtod(),
			* then restore it and return
			*/
			*cp = '\0';
			result = strtod(nptr, endptr);
			*cp = c;
			return result;
		}
		++cp;
	}

	/* no offending characters, just handle normally */

	return strtod(nptr, endptr);
}
double DMSToDegree(const char *is)

{
	int sign, n, nl;
	char *p, *s, work[64];
	double v, tv;

	/* copy sting into work space */
	while (isspace((unsigned char)(sign = *is))) ++is;
	for (n = sizeof(work), s = work, p = (char *)is; isgraph(*p) && --n; )
		*s++ = *p++;
	*s = '\0';
	/* it is possible that a really odd input (like lots of leading
	zeros) could be truncated in copying into work.  But ... */
	sign = *(s = work);
	if (sign == '+' || sign == '-') s++;
	else sign = '+';
	for (v = 0., nl = 0; nl < 3; nl = n + 1) {
		if (!(isdigit(*s) || *s == '.')) break;
		if ((tv = proj_strtod(s, &s)) == HUGE_VAL)
			return tv;
		switch (*s) {
		case 'D': case 'd':
			n = 0; break;
		case '\'':
			n = 1; break;
		case '"':
			n = 2; break;
		case 'r': case 'R':
			if (nl) {
				return 0.0;
			}
			++s;
			v = tv;
			goto skip;
		default:
			v += tv * vm[nl];
		skip: n = 4;
			continue;
		}
		if (n < nl) {
			return 0.0;
		}
		v += tv * vm[n];
		++s;
	}
	/* postfix sign */
	if (*s && ((p = (char *)strchr(sym, *s))) != NULL) {
		sign = (p - sym) >= 4 ? '-' : '+';
		++s;
	}
	if (sign == '-')
		v = -v;

	return v;
}


/************************************************************************/
/*                            degreeToDMS()                             */
/*                                                                      */
/*      Translate a decimal degrees value to a DMS string with          */
/*      hemisphere.                                                     */
/************************************************************************/
const char *degreeToDMS(double dfAngle, const char * pszAxis,
	int nPrecision)

{ 
	int         nDegrees, nMinutes;
	double      dfSeconds, dfABSAngle, dfEpsilon;
	char        szFormat[30];
	const char  *pszHemisphere;
	static   char szBuffer[50] = { 0 };


	dfEpsilon = (0.5 / 3600.0) * pow(0.1, nPrecision);

	dfABSAngle = fabs(dfAngle) + dfEpsilon;
	if (dfABSAngle > 361)
	{
		return "Invalid angle";
	}

	nDegrees = (int)dfABSAngle;
	nMinutes = (int)((dfABSAngle - nDegrees) * 60);
	dfSeconds = dfABSAngle * 3600 - nDegrees * 3600 - nMinutes * 60;

	if (dfSeconds > dfEpsilon * 3600.0)
		dfSeconds -= dfEpsilon * 3600.0;

	if (cslIEqualString(pszAxis, "Long") && dfAngle < 0.0)
		pszHemisphere = "W";
	else if (cslIEqualString(pszAxis, "Long"))
		pszHemisphere = "E";
	else if (dfAngle < 0.0)
		pszHemisphere = "S";
	else
		pszHemisphere = "N";

	sprintf(szFormat, "%%3dd%%2d\'%%%d.%df\"%s", nPrecision + 3, nPrecision, pszHemisphere);
	sprintf(szBuffer, szFormat, nDegrees, nMinutes, dfSeconds);

	return(szBuffer);
}

/************************************************************************/
/*                         packedDMSToDegree()                          */
/************************************************************************/

/**
* Convert a packed DMS value (DDDMMMSSS.SS) into decimal degrees.
*
* This function converts a packed DMS angle to seconds. The standard
* packed DMS format is:
*
*  degrees * 1000000 + minutes * 1000 + seconds
*
* Example:     ang = 120025045.25 yields
*              deg = 120
*              min = 25
*              sec = 45.25
*
* The algorithm used for the conversion is as follows:
*
* 1.  The absolute value of the angle is used.
*
* 2.  The degrees are separated out:
*     deg = ang/1000000                    (fractional portion truncated)
*
* 3.  The minutes are separated out:
*     min = (ang - deg * 1000000) / 1000   (fractional portion truncated)
*
* 4.  The seconds are then computed:
*     sec = ang - deg * 1000000 - min * 1000
*
* 5.  The total angle in seconds is computed:
*     sec = deg * 3600.0 + min * 60.0 + sec
*
* 6.  The sign of sec is set to that of the input angle.
*
* Packed DMS values used by the USGS GCTP package and probably by other
* software.
*
* NOTE: This code does not validate input value. If you give the wrong
* value, you will get the wrong result.
*
* @param dfPacked Angle in packed DMS format.
*
* @return Angle in decimal degrees.
*
*/

double packedDMSToDegree(PACKED_DMS_TYPE dfPacked)
{
	double  dfDegrees, dfMinutes, dfSeconds, dfSign;

	dfSign = (dfPacked < 0.0) ? -1 : 1;

	dfSeconds = fabs(dfPacked);
	dfDegrees = floor(dfSeconds / 1000000.0);
	dfSeconds = dfSeconds - dfDegrees * 1000000.0;
	dfMinutes = floor(dfSeconds / 1000.0);
	dfSeconds = dfSeconds - dfMinutes * 1000.0;
	dfSeconds = dfSign * (dfDegrees * 3600.0 + dfMinutes * 60.0 + dfSeconds);
	dfDegrees = dfSeconds / 3600.0;

	return dfDegrees;
}

/************************************************************************/
/*                         degreeToPackedDMS()                          */
/************************************************************************/
/**
* Convert decimal degrees into packed DMS value (DDDMMMSSS.SS).
*
* This function converts a value, specified in decimal degrees into
* packed DMS angle. The standard packed DMS format is:
*
*  degrees * 1000000 + minutes * 1000 + seconds
*
* See also CPLPackedDMSToDec().
*
* @param dfDec Angle in decimal degrees.
*
* @return Angle in packed DMS format.
*
*/

double degreeToPackedDMS(double dfDec)
{
	double  dfDegrees, dfMinutes, dfSeconds, dfSign;

	dfSign = (dfDec < 0.0) ? -1 : 1;

	dfDec = fabs(dfDec);
	dfDegrees = floor(dfDec);
	dfMinutes = floor((dfDec - dfDegrees) * 60.0);
	dfSeconds = (dfDec - dfDegrees) * 3600.0 - dfMinutes * 60.0;

	return dfSign * (dfDegrees * 1000000.0 + dfMinutes * 1000.0 + dfSeconds);
}


end_gdb_namespace
end_gtl_namespace



 