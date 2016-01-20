/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/

#ifndef GTL_ERROR_H
#define GTL_ERROR_H

#include <string>
begin_gtl_namespace

/* ==================================================================== */
/*      Well known error types for GTL.                                         */
/* ==================================================================== */
enum ERRORTYPE
{
	GET_NONE = 0,
	GET_DEBUG = 1,
	GET_WARNING = 2,
	GET_FAILURE = 3,
	GET_FATAL = 4
};


/* ==================================================================== */
/*      Well known error codes for GTL.                                         */
/* ==================================================================== */


enum ERRORCODE{
	GEC_NODE = 0,
	GEC_APPDEFINED,
	GEC_OUTOFMEMORY,
	GEC_FILEIO,
	GEC_OPENFAILED,
	GEC_ILLEGALARG,
	GEC_NOTSUPPORTED,
	GEC_ASSERTIONFAILED,
	GEC_NOWRITEACCESS,
	GEC_USERINTERRUPT,
	GEC_OBJECTNULL
};
/* 100 - 299 reserved for GTL */


typedef void(*ErrorHandler)(ERRORTYPE, int, const char*);

void  error(ERRORTYPE eErrClass, int err_no, const char *fmt, ...);
void  debug(const char *, const char *, ...);

end_gtl_namespace
#endif /* GTL_ERROR_H */

