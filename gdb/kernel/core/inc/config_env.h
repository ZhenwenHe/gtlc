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
gtl (GTL_HOME)
 * --gdb
   --gtl
   --sdk (GTL_INSTALL_HOME)
        --images
        --share
   --build
   --others 
 */

/*
 * get GTL home directory
 */
static string getHome();
/*
 * get GTL installation home directory
 */
static string getInstallHome();

#endif /* CONFIG_ENV_H */

