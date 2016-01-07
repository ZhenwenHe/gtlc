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
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <map>
#include "feature.h"




begin_gtl_namespace
begin_gdb_namespace
/** @defgroup Selector Selector   
*  @{
*/
class CORE_API Selector: protected FeatureWeakPtrVector
{
	typedef FeatureWeakPtrVector BaseType;
public:
	Selector(void);
	virtual ~Selector(void);
public:
	FeatureSharedPtr operator() (size_t i);
	FeatureSharedPtr getAt (size_t i);
	size_t size();
	void append(FeatureSharedPtr p);
	void append(FeatureSharedPtrVector p);
	FeatureSharedPtr removeAt(size_t i);
	void clear();
	FeatureSharedPtr findFeature(FID fid);
	FeatureSharedPtr removeFeature(FID fid);
	long getFeatures(FCLSID fclsid,FeatureSharedPtrVector & vv);
	long getFeatures( FeatureSharedPtrVector & vv);
	bool empty();
	bool exist(FeatureSharedPtr  pp);
	bool exist(FID   fid);
};
typedef  Selector&   SelectorRef;
typedef std::shared_ptr<Selector>  SelectorSharedPtr;
/** @} */ 
end_gdb_namespace
end_gtl_namespace



