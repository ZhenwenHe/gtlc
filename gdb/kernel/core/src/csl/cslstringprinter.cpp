#include "csl.h"
begin_gtl_namespace
begin_gdb_namespace

/**
* Copy the string pointed to by pszSrc, NOT including the terminating
* `\\0' character, to the array pointed to by pszDest. Remainder of the
* destination string will be filled with fillChar (the dedault is space characters£©.
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

int StringPrinter::print(char *pszDest, const char *pszSrc, int nMaxLen, char fillChar)
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

/************************************************************************/
/*                      printString()                                */
/************************************************************************/

/**
* Copy the string pointed to by pszSrc, NOT including the terminating
* `\\0' character, to the array pointed to by pszDest.
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

int StringPrinter::print(char *pszDest, const char *pszSrc, int nMaxLen)
{
	char    *pszTemp = pszDest;
	int     nChars = 0;

	if (!pszDest)
		return 0;

	if (!pszSrc)
	{
		*pszDest = '\0';
		return 1;
	}

	while (nChars < nMaxLen && *pszSrc)
	{
		*pszTemp++ = *pszSrc++;
		nChars++;
	}

	return nChars;
}

int  StringPrinter::print(char *pszBuffer, int32_t iValue, int nMaxLen) {
	char    szTemp[64];

	if (!pszBuffer)
		return 0;

	if (nMaxLen >= 64)
		nMaxLen = 63;

#if UINT_MAX == 65535
	sprintf(szTemp, "%*ld", nMaxLen, iValue);
#else
	sprintf(szTemp, "%*d", nMaxLen, iValue);
#endif

	return print(pszBuffer, szTemp, nMaxLen);
}

int  StringPrinter::print(char *pszBuffer, uint64_t iValue, int nMaxLen) {
	char    szTemp[64];

	if (!pszBuffer)
		return 0;

	if (nMaxLen >= 64)
		nMaxLen = 63;

#if defined(__MSVCRT__) || (defined(WIN32) && defined(_MSC_VER))
	sprintf(szTemp, "%*I64d", nMaxLen, iValue);
# elif HAVE_LONG_LONG
	sprintf(szTemp, "%*lld", nMaxLen, (long long)iValue);
	//    sprintf( szTemp, "%*Ld", nMaxLen, (long long) iValue );
#else
	sprintf(szTemp, "%*ld", nMaxLen, iValue);
#endif

	return print(pszBuffer, szTemp, nMaxLen);
}

int  StringPrinter::print(String & outString, double v) {

	return outString.size();
}
int  StringPrinter::print(String & outString, float v) {

	return outString.size();
}

int  StringPrinter::print(String & outString, int8_t  v) {

	return outString.size();
}
int  StringPrinter::print(String & outString, uint8_t v) {

	return outString.size();
}

int  StringPrinter::print(String & outString, int16_t v) {

	return outString.size();
}
int  StringPrinter::print(String & outString, uint16_t v) {

	return outString.size();
}

int  StringPrinter::print(String & outString, int32_t v) {

	return outString.size();
}
int  StringPrinter::print(String & outString, uint32_t v) {

	return outString.size();
}

int  StringPrinter::print(String & outString, int64_t v) {

	return outString.size();
}

int  StringPrinter::print(String & outString, uint64_t v) {

	return outString.size();
}
end_gdb_namespace
end_gtl_namespace
