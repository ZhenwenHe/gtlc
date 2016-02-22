#include "csl.h" 

begin_gtl_namespace
begin_gdb_namespace

#ifdef GTL_RECODE_ICONV
extern void GTLClearRecodeIconvWarningFlags();
extern char *GTLRecodeIconv(const char *, const char *, const char *);
extern char *GTLRecodeFromWCharIconv(const wchar_t *,
	const char *, const char *);
extern wchar_t *GTLRecodeToWCharIconv(const char *,
	const char *, const char *);
#endif /* GTL_RECODE_ICONV */

extern void GTLClearRecodeStubWarningFlags();
extern char *GTLRecodeStub(const char *, const char *, const char *);
extern char *GTLRecodeFromWCharStub(const wchar_t *,const char *, const char *);
extern wchar_t *GTLRecodeToWCharStub(const char *,const char *, const char *);
extern int GTLIsUTF8Stub(const char *, int);

 
/************************************************************************/
/*                             cslRecode()                              */
/************************************************************************/

/**
* Convert a string from a source encoding to a destination encoding.
*
* The only guaranteed supported encodings are GTL_ENC_UTF8, GTL_ENC_ASCII
* and GTL_ENC_ISO8859_1. Currently, the following conversions are supported :
* <ul>
*  <li>GTL_ENC_ASCII -> GTL_ENC_UTF8 or GTL_ENC_ISO8859_1 (no conversion in fact)</li>
*  <li>GTL_ENC_ISO8859_1 -> GTL_ENC_UTF8</li>
*  <li>GTL_ENC_UTF8 -> GTL_ENC_ISO8859_1</li>
* </ul>
*
* If an error occurs an error may, or may not be posted with CPLError().
*
* @param pszSource a NULL terminated string.
* @param pszSrcEncoding the source encoding.
* @param pszDstEncoding the destination encoding.
*
* @return a NULL terminated string which should be freed with CPLFree().
*
* @since GDAL 1.6.0
*/

char *cslRecode(const char *pszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding)

{
	/* -------------------------------------------------------------------- */
	/*      Handle a few common short cuts.                                 */
	/* -------------------------------------------------------------------- */
	if (cslIEqualString(pszSrcEncoding, pszDstEncoding))
		return cslDuplicateString(pszSource);

	if (cslIEqualString(pszSrcEncoding, GTL_ENC_ASCII)
		&& (cslIEqualString(pszDstEncoding, GTL_ENC_UTF8)
			|| cslIEqualString(pszDstEncoding, GTL_ENC_ISO8859_1)))
		return cslDuplicateString(pszSource);

#ifdef GTL_RECODE_ICONV
	/* -------------------------------------------------------------------- */
	/*      GTL_ENC_ISO8859_1 -> GTL_ENC_UTF8                               */
	/*      and GTL_ENC_UTF8 -> GTL_ENC_ISO8859_1 conversions are hadled    */
	/*      very well by the stub implementation which is faster than the   */
	/*      iconv() route. Use a stub for these two ones and iconv()        */
	/*      everything else.                                                */
	/* -------------------------------------------------------------------- */
	if ((cslIEqualString(pszSrcEncoding, GTL_ENC_ISO8859_1)
		&& cslIEqualString(pszDstEncoding, GTL_ENC_UTF8))
		|| (cslIEqualString(pszSrcEncoding, GTL_ENC_UTF8)
			&& cslIEqualString(pszDstEncoding, GTL_ENC_ISO8859_1)))
	{
		return GTLRecodeStub(pszSource, pszSrcEncoding, pszDstEncoding);
	}
	else
	{
		return GTLRecodeIconv(pszSource, pszSrcEncoding, pszDstEncoding);
	}
#else /* GTL_RECODE_STUB */
	return GTLRecodeStub(pszSource, pszSrcEncoding, pszDstEncoding);
#endif /* GTL_RECODE_ICONV */
}

/************************************************************************/
/*                         cslRecodeFromWChar()                         */
/************************************************************************/

/**
* Convert wchar_t string to UTF-8.
*
* Convert a wchar_t string into a multibyte utf-8 string.  The only
* guaranteed supported source encoding is GTL_ENC_UCS2, and the only
* guaranteed supported destination encodings are GTL_ENC_UTF8, GTL_ENC_ASCII
* and GTL_ENC_ISO8859_1.  In some cases (ie. using iconv()) other encodings
* may also be supported.
*
* Note that the wchar_t type varies in size on different systems. On
* win32 it is normally 2 bytes, and on unix 4 bytes.
*
* If an error occurs an error may, or may not be posted with CPLError().
*
* @param pwszSource the source wchar_t string, terminated with a 0 wchar_t.
* @param pszSrcEncoding the source encoding, typically GTL_ENC_UCS2.
* @param pszDstEncoding the destination encoding, typically GTL_ENC_UTF8.
*
* @return a zero terminated multi-byte string which should be freed with
* CPLFree(), or NULL if an error occurs.
*
* @since GDAL 1.6.0
*/

char *cslRecodeFromWChar(const wchar_t *pwszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding)

{
#ifdef GTL_RECODE_ICONV
	/* -------------------------------------------------------------------- */
	/*      Conversions from GTL_ENC_UCS2                                   */
	/*      to GTL_ENC_UTF8, GTL_ENC_ISO8859_1 and GTL_ENC_ASCII are well   */
	/*      handled by the stub implementation.                             */
	/* -------------------------------------------------------------------- */
	if ((cslIEqualString(pszSrcEncoding, GTL_ENC_UCS2) || cslIEqualString(pszSrcEncoding, "WCHAR_T"))
		&& (cslIEqualString(pszDstEncoding, GTL_ENC_UTF8)
			|| cslIEqualString(pszDstEncoding, GTL_ENC_ASCII)
			|| cslIEqualString(pszDstEncoding, GTL_ENC_ISO8859_1)))
	{
		return GTLRecodeFromWCharStub(pwszSource,
			pszSrcEncoding, pszDstEncoding);
	}
	else
	{
		return GTLRecodeFromWCharIconv(pwszSource,
			pszSrcEncoding, pszDstEncoding);
	}
#else /* GTL_RECODE_STUB */
	return GTLRecodeFromWCharStub(pwszSource,
		pszSrcEncoding, pszDstEncoding);
#endif /* GTL_RECODE_ICONV */
}

/************************************************************************/
/*                          cslRecodeToWChar()                          */
/************************************************************************/

/**
* Convert UTF-8 string to a wchar_t string.
*
* Convert a 8bit, multi-byte per character input string into a wide
* character (wchar_t) string.  The only guaranteed supported source encodings
* are GTL_ENC_UTF8, GTL_ENC_ASCII and GTL_ENC_ISO8869_1 (LATIN1).  The only
* guaranteed supported destination encoding is GTL_ENC_UCS2.  Other source
* and destination encodings may be supported depending on the underlying
* implementation.
*
* Note that the wchar_t type varies in size on different systems. On
* win32 it is normally 2 bytes, and on unix 4 bytes.
*
* If an error occurs an error may, or may not be posted with CPLError().
*
* @param pszSource input multi-byte character string.
* @param pszSrcEncoding source encoding, typically GTL_ENC_UTF8.
* @param pszDstEncoding destination encoding, typically GTL_ENC_UCS2.
*
* @return the zero terminated wchar_t string (to be freed with CPLFree()) or
* NULL on error.
*
* @since GDAL 1.6.0
*/

wchar_t *cslRecodeToWChar(const char *pszSource,
	const char *pszSrcEncoding,
	const char *pszDstEncoding)

{
#ifdef GTL_RECODE_ICONV
	/* -------------------------------------------------------------------- */
	/*      Conversions to GTL_ENC_UCS2                                     */
	/*      from GTL_ENC_UTF8, GTL_ENC_ISO8859_1 and GTL_ENC_ASCII are well */
	/*      handled by the stub implementation.                             */
	/* -------------------------------------------------------------------- */
	if ((cslIEqualString(pszDstEncoding, GTL_ENC_UCS2) || cslIEqualString(pszDstEncoding, "WCHAR_T"))
		&& (cslIEqualString(pszSrcEncoding, GTL_ENC_UTF8)
			|| cslIEqualString(pszSrcEncoding, GTL_ENC_ASCII)
			|| cslIEqualString(pszSrcEncoding, GTL_ENC_ISO8859_1)))
	{
		return CPLRecodeToWCharStub(pszSource,
			pszSrcEncoding, pszDstEncoding);
	}
	else
	{
		return CPLRecodeToWCharIconv(pszSource,
			pszSrcEncoding, pszDstEncoding);
	}
#else /* GTL_RECODE_STUB */
	return GTLRecodeToWCharStub(pszSource, pszSrcEncoding, pszDstEncoding);
#endif /* GTL_RECODE_ICONV */
}

/************************************************************************/
/*                                 cslIsUTF8()                          */
/************************************************************************/

/**
* Test if a string is encoded as UTF-8.
*
* @param pabyData input string to test
* @param nLen length of the input string, or -1 if the function must compute
*             the string length. In which case it must be null terminated.
* @return TRUE if the string is encoded as UTF-8. FALSE otherwise
*
* @since GDAL 1.7.0
*/
int cslIsUTF8(const char* pabyData, int nLen)
{
	return GTLIsUTF8Stub(pabyData, nLen);
}

/************************************************************************/
/*                          cslForceToASCII()                           */
/************************************************************************/

/**
* Return a new string that is made only of ASCII characters. If non-ASCII
* characters are found in the input string, they will be replaced by the
* provided replacement character.
*
* @param pabyData input string to test
* @param nLen length of the input string, or -1 if the function must compute
*             the string length. In which case it must be null terminated.
* @param chReplacementChar character which will be used when the input stream
*                          contains a non ASCII character. Must be valid ASCII !
*
* @return a new string that must be freed with CPLFree().
*
* @since GDAL 1.7.0
*/
char *cslForceToASCII(const char* pabyData, int nLen, char chReplacementChar)
{
	if (nLen < 0)
		nLen = strlen(pabyData);
	char* pszOutputString = (char*)malloc(nLen + 1);
	int i;
	for (i = 0; i<nLen; i++)
	{
		if (((unsigned char*)pabyData)[i] > 127)
			pszOutputString[i] = chReplacementChar;
		else
			pszOutputString[i] = pabyData[i];
	}
	pszOutputString[i] = '\0';
	return pszOutputString;
}

/************************************************************************/
/*                        cslEncodingCharSize()                         */
/************************************************************************/

/**
* Return bytes per character for encoding.
*
* This function returns the size in bytes of the smallest character
* in this encoding.  For fixed width encodings (ASCII, UCS-2, UCS-4) this
* is straight forward.  For encodings like UTF8 and UTF16 which represent
* some characters as a sequence of atomic character sizes the function
* still returns the atomic character size (1 for UTF8, 2 for UTF16).
*
* This function will return the correct value for well known encodings
* with corresponding GTL_ENC_ values.  It may not return the correct value
* for other encodings even if they are supported by the underlying iconv
* or windows transliteration services.  Hopefully it will improve over time.
*
* @param pszEncoding the name of the encoding.
*
* @return the size of a minimal character in bytes or -1 if the size is
* unknown.
*/

int cslEncodingCharSize(const char *pszEncoding)

{
	if (cslIEqualString(pszEncoding, GTL_ENC_UTF8))
		return 1;
	else if (cslIEqualString(pszEncoding, GTL_ENC_UTF16))
		return 2;
	else if (cslIEqualString(pszEncoding, GTL_ENC_UCS2))
		return 2;
	else if (cslIEqualString(pszEncoding, GTL_ENC_UCS4))
		return 4;
	else if (cslIEqualString(pszEncoding, GTL_ENC_ASCII))
		return 1;
	else if (cslNIEqualString(pszEncoding, "ISO-8859-", 9))
		return 1;
	else
		return -1;
}

/************************************************************************/
/*                    CPLClearRecodeWarningFlags()                      */
/************************************************************************/

void cslClearRecodeWarningFlags()
{
#ifdef GTL_RECODE_ICONV
	GTLClearRecodeIconvWarningFlags();
#endif
	GTLClearRecodeStubWarningFlags();
}


end_gdb_namespace
end_gtl_namespace



