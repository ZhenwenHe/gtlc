#include "csl.h"
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
		std::cerr << "String::vprintf() ... buffer overrun.";
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

String &String::recode(const char *pszSrcEncoding, const char *pszDstEncoding)
{
	if (pszSrcEncoding == NULL)
		pszSrcEncoding = GTL_ENC_UTF8;
	if (pszDstEncoding == NULL)
		pszDstEncoding = GTL_ENC_UTF8;

	if (strcmp(pszSrcEncoding, pszDstEncoding) == 0)
		return *this;

	char *pszRecoded = cslRecode(c_str(),
		pszSrcEncoding,
		pszDstEncoding);

	if (pszRecoded == NULL)
		return *this;

	assign(pszRecoded);
	free(pszRecoded);

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

end_gdb_namespace
end_gtl_namespace