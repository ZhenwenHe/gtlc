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

#ifndef CONFIG_ENV_H
#define CONFIG_ENV_H
 
/* directory framework
gtl (GTL_HOME)  // for development 
 * --gdb
   --gtl
   --doc
   --data (GTL_DATA_HOME) 
   --sdk (GTL_INSTALL_HOME) //for installation
        --images
        --shader
   --build
   --others 
 */

#include <string>
#include <algorithm>

begin_gtl_namespace

/*
 * get GTL home directory
 */
 std::string getHome();
/*
 * get GTL installation home directory
 */
 std::string getInstallHome();

/*
 * get GTL data home directory
 */
 std::string getDataHome();

 /*Fetch a function pointer from a shared library / DLL.*/
 void * getFunctionPointer(const char * pszLibrary, const char * pszSymbolName);
end_gtl_namespace
#endif /* CONFIG_ENV_H */

