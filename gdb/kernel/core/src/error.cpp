#include <stdlib.h>
#include <stdio.h> 
#include "config.h"

begin_gtl_namespace
void  defaultErrorHandler(ERRORTYPE eErrClass, int nError, const char * pszErrorMsg);
static void *hErrorMutex = NULL;
static void *pErrorHandlerUserData = NULL;
static ErrorHandler pfnErrorHandler = defaultErrorHandler;
static const int DEFAULT_LAST_ERR_MSG_SIZE = 20480;
static const int DEFAULT_MAX_ERRS = 20480;

void  defaultErrorHandler(ERRORTYPE eErrClass, int nError, const char * pszErrorMsg)
{
	static int       bLogInit = FALSE;
	static FILE *    fpLog = stderr;
	static int       nCount = 0;
	static int       nMaxErrors = -1;

	if (eErrClass != GET_DEBUG)
	{
		if (nMaxErrors == -1)
		{
			nMaxErrors = DEFAULT_MAX_ERRS;
		}

		nCount++;
		if (nCount > nMaxErrors && nMaxErrors > 0)
			return;
	}

	if (!bLogInit)
	{
		bLogInit = TRUE;
		fpLog = stderr; 
		fpLog = fopen((getDataHome() + std::string("\\gtl.log")).c_str(), "wt");
		if (fpLog == NULL)
			fpLog = stderr;		
	}

	if (eErrClass == GET_DEBUG)
		fprintf(fpLog, "%s\n", pszErrorMsg);
	else if (eErrClass == GET_WARNING)
		fprintf(fpLog, "Warning %d: %s\n", nError, pszErrorMsg);
	else
		fprintf(fpLog, "ERROR %d: %s\n", nError, pszErrorMsg);

	if (eErrClass != GET_DEBUG
		&& nMaxErrors > 0
		&& nCount == nMaxErrors)
	{
		fprintf(fpLog,
			"More than %d errors or warnings have been reported. "
			"No more will be reported from now.\n",
			nMaxErrors);
	}

	fflush(fpLog);
}


struct ErrorHandlerNode
{
	ErrorHandlerNode   *psNext;
	void                *pUserData;
	ErrorHandler     pfnHandler;
} ;

struct ErrorContext {
	int     nLastErrNo;
	ERRORTYPE  eLastErrType;
	ErrorHandlerNode *psHandlerStack;
	int     nLastErrMsgMax;
	int     nFailureIntoWarning;
	char    szLastErrMsg[DEFAULT_LAST_ERR_MSG_SIZE];
	/* Do not add anything here. szLastErrMsg must be the last field. See realloc() below */
};


void  error(ERRORTYPE eErrClass, int err_no, const char *fmt, ...){

}
void  debug(const char *, const char *, ...){

}
 
end_gtl_namespace