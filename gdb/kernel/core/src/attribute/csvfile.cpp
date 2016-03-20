#include <stdlib.h>
#include <stdio.h>
#include "csv.h"
#include "globalfuns.h"


begin_gtl_namespace
begin_gdb_namespace

void CommaSeparatedValues::clear() {
	if (papszFieldNames)
		cslDestroy(papszFieldNames);
	if (papszLines)
		free(papszLines);
	if (pszRawData)
		free(pszRawData);
	nLineCount = 0;
	papszFieldNames = 0;
	papszLines = 0;
	pszRawData = 0;
	cSeparator = ',';
}
CommaSeparatedValues::CommaSeparatedValues() {
	nLineCount = 0;
	papszLines = 0;
	pszRawData = 0;
	papszFieldNames = 0;
	cSeparator = ',';
}
CommaSeparatedValues::CommaSeparatedValues(const char * pathname) {
	nLineCount = 0;
	papszLines = 0;
	pszRawData = 0;
	papszFieldNames = 0;
	load(pathname);
}
CommaSeparatedValues::CommaSeparatedValues(const std::string& pathname) {
	nLineCount = 0;
	papszLines = 0;
	pszRawData = 0;
	papszFieldNames = 0;
	load(pathname.c_str());
}

int CommaSeparatedValues::compare(const char * pszFieldValue, const char * pszTarget, CSVCompareCriteria eCriteria) {
	if (eCriteria == CC_ExactString)
	{
		return(strcmp(pszFieldValue, pszTarget) == 0);
	}
	else if (eCriteria == CC_ApproxString)
	{
		return(cslIEqualString(pszFieldValue, pszTarget));
	}
	else if (eCriteria == CC_Integer)
	{
		int i1 = atoi(pszFieldValue);
		int i2 = atoi(pszTarget);
		if (i1 > i2) return 1;
		else if (i1 == i2)
			return 0;
		else
			return -1;
	}
	else if (eCriteria == CC_Decimal)
	{
		double i1 = atof(pszFieldValue);
		double i2 = atof(pszTarget);
		if (i1 > i2) return 1;
		else if (doubleEqual(i1, i2))
			return 0;
		else
			return -1;
	}
	else
		return 0;
}
/** Detect which field separator is used.
*
* Currently, it can detect comma, semicolon or tabulation. In case of
* ambiguity or no separator found, comma will be considered as the separator.
*
* @return ',', ';' or '\t'
*/
char CommaSeparatedValues::detectSeperator(const char* pszLine)
{
	int     bInString = 0;
	char    chDelimiter = '\0';

	for (; *pszLine != '\0'; pszLine++)
	{
		if (!bInString && (*pszLine == ',' || *pszLine == ';' || *pszLine == '\t'))
		{
			if (chDelimiter == '\0')
				chDelimiter = *pszLine;
			else if (chDelimiter != *pszLine)
			{
				chDelimiter = ',';
				break;
			}
		}
		else if (*pszLine == '"')
		{
			if (!bInString || pszLine[1] != '"')
			{
				bInString = !bInString;
				continue;
			}
			else  /* doubled quotes in string resolve to one quote */
			{
				pszLine++;
			}
		}
	}

	if (chDelimiter == '\0')
		chDelimiter = ',';

	return chDelimiter;
}

/************************************************************************/
/*                            tokenizeLine()                            */
/*                                                                      */
/*      Tokenize a CSV line into fields in the form of a string         */
/*      list.  This is used instead of the CPLTokenizeString()          */
/*      because it provides correct CSV escaping and quoting            */
/*      semantics.                                                      */
/************************************************************************/

char ** CommaSeparatedValues::tokenizeLine(const char *pszString, char chDelimiter)
{
	char        **papszRetList = NULL;
	char        *pszToken;
	int         nTokenMax, nTokenLen;

	pszToken = (char *)calloc(10, 1);
	nTokenMax = 10;

	while (pszString != NULL && *pszString != '\0')
	{
		int     bInString = 0;

		nTokenLen = 0;

		/* Try to find the next delimeter, marking end of token */
		for (; *pszString != '\0'; pszString++)
		{

			/* End if this is a delimeter skip it and break. */
			if (!bInString && *pszString == chDelimiter)
			{
				pszString++;
				break;
			}

			if (*pszString == '"')
			{
				if (!bInString || pszString[1] != '"')
				{
					bInString = !bInString;
					continue;
				}
				else  /* doubled quotes in string resolve to one quote */
				{
					pszString++;
				}
			}

			if (nTokenLen >= nTokenMax - 2)
			{
				nTokenMax = nTokenMax * 2 + 10;
				pszToken = (char *)realloc(pszToken, nTokenMax);
			}

			pszToken[nTokenLen] = *pszString;
			nTokenLen++;
		}

		pszToken[nTokenLen] = '\0';
		papszRetList = cslAddString(papszRetList, pszToken);

		/* If the last token is an empty token, then we have to catch
		* it now, otherwise we won't reenter the loop and it will be lost.
		*/
		if (*pszString == '\0' && *(pszString - 1) == chDelimiter)
		{
			papszRetList = cslAddString(papszRetList, "");
		}
	}

	if (papszRetList == NULL)
		papszRetList = (char **)calloc(sizeof(char *), 1);

	free(pszToken);

	return papszRetList;
}

/************************************************************************/
/*                          findNextLine()                           */
/*                                                                      */
/*      Find the start of the next line, while at the same time zero    */
/*      terminating this line.  Take into account that there may be     */
/*      newline indicators within quoted strings, and that quotes       */
/*      can be escaped with a backslash.                                */
/************************************************************************/

char *CommaSeparatedValues::findNextLine(char *pszThisLine)
{
	int  nQuoteCount = 0, i;

	for (i = 0; pszThisLine[i] != '\0'; i++)
	{
		if (pszThisLine[i] == '\"'
			&& (i == 0 || pszThisLine[i - 1] != '\\'))
			nQuoteCount++;

		if ((pszThisLine[i] == 10 || pszThisLine[i] == 13)
			&& (nQuoteCount % 2) == 0)
			break;
	}

	while (pszThisLine[i] == 10 || pszThisLine[i] == 13)
		pszThisLine[i++] = '\0';

	if (pszThisLine[i] == '\0')
		return NULL;
	else
		return pszThisLine + i;
}



/************************************************************************/
/*                             load()                              */
/*                                                                      */
/*      Load entire file into memory and setup index if possible.       */
/************************************************************************/
bool CommaSeparatedValues::load(const char * pathname)
{
	int       nFileLen, i, nMaxLineCount, iLine = 0;
	char *pszThisLine;
	clear();
	/* -------------------------------------------------------------------- */
	/*      Ingest whole file.                                              */
	/* -------------------------------------------------------------------- */
	FILE * fp = NULL;
	if((fp=fopen(pathname, "rb"))==NULL)
		return false;
	fseek(fp, 0, SEEK_END);
	nFileLen = ftell(fp);
	rewind(fp);
	pszRawData = (char *)malloc(nFileLen + 1);
	if ((int)fread(pszRawData, 1, nFileLen, fp) != nFileLen)
	{
		free(pszRawData);
		pszRawData = NULL;
		return false;
	}
	pszRawData[nFileLen] = '\0';

	/* -------------------------------------------------------------------- */
	/*      Get count of newlines so we can allocate line array.            */
	/* -------------------------------------------------------------------- */
	nMaxLineCount = 0;
	for (i = 0; i < nFileLen; i++)
	{
		if (pszRawData[i] == 10)//»»ÐÐ¼üµÄASCIIÂë
			nMaxLineCount++;
	}

	//parse field names
	pszThisLine = pszRawData;
	cSeparator = detectSeperator(pszThisLine);
	papszFieldNames=tokenizeLine(pszThisLine, cSeparator);

	papszLines = (char **)calloc(sizeof(char*), nMaxLineCount);
	/* -------------------------------------------------------------------- */
	/*      Build a list of record pointers into the raw data buffer        */
	/*      based on line terminators.  Zero terminate the line             */
	/*      strings.                                                        */
	/* -------------------------------------------------------------------- */
	/* not skip header line */
	pszThisLine = pszRawData;//findNextLine(pszRawData);
	while (pszThisLine != NULL && iLine < nMaxLineCount)
	{
		papszLines[iLine++] = pszThisLine;
		pszThisLine = findNextLine(pszThisLine);
	}
	nLineCount = iLine;

	/* -------------------------------------------------------------------- */
	/*      We should never need the file handle against, so close it.      */
	/* -------------------------------------------------------------------- */
	fclose(fp);
	return true;
}

bool CommaSeparatedValues::load(const std::string &  pathname) {
	return load(pathname.c_str());
}

end_gdb_namespace
end_gtl_namespace
