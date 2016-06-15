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

#pragma once
#include <atomic>
#include <mutex>
#include "feature.h" 



begin_gtl_namespace
begin_gdb_namespace

/** @@defgroup  Console  Console
*  @{
*/
#if(ENABLE_CONSOLE_OUTPUT!=0)
class CORE_API Console
{
public:
	Console();
	virtual ~Console();

public:
	static bool print(const char * fmt, ...);
	static bool print(unsigned int Attrs, const char * fmt, ...);
	static bool setTile(const char * lpTile);
public:

	bool writeString(const string&  lpString);
	bool writeString(unsigned int Attrs, const string &  lpString);
private:
	void * m_hConsole;
	bool   m_bCreate;
	bool   m_bAttrs;
	unsigned int   m_oldColorAttrs;
};
#endif

CORE_API bool CONSOLE_OUTPUT(const char * fmt, ...);
CORE_API bool CONSOLE_OUTPUT(unsigned int Attrs, const char * fmt, ...);
/** @}*/
end_gdb_namespace
end_gtl_namespace


