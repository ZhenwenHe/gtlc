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


#pragma  once

#ifdef WIN32 
#define USE_WINSOWS_OS   //Windows OS

#ifdef _DEBUG
#define  _GTL_DEBUG_
#endif


#else
#define USE_LINUX_OS   //Linux OS
#ifndef MAX_PATH
#define MAX_PATH          260
#endif //MAX_PATH
#endif
/*
*本文件实现全局参数配置
*/

/*开始图形渲染引擎支持预定义
  下列变量中只能有一个为1，为1表示当前使用的引擎*/
#define  USE_OPENGL_ENGINE   1 //SGI OpenGL 渲染引擎
#define  USE_DIRECTX_ENGINE  0 //MS DirectX 渲染引擎
#define  USE_MESA3D_ENGINE   0 //Mesa3D 渲染引擎
#define  USE_OSG_ENGINE      0 //OSG    渲染引擎
#define  USE_OPENGINVENTOR_ENGINE 0 //SGI Open Inventor渲染引擎

#define  GTL_VERSION  2
/*结束图形渲染引擎支持预定义*/

#ifdef USE_WINDOWS_OS
#include <windows.h>
#include "minwindef.h"
#endif
