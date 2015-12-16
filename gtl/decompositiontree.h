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
#include <stack>
#include <list>
#include <map>
#include <hash_map>
#include "vertex.h"
#include "envelope.h"
#include "fastlist.h"
#include "pointerarray.h"
#include "omp.h" 
#if GTL_VERSION<=1
#include "decompositiontree1.h"
#else
#include "decompositiontree2.h"
#endif