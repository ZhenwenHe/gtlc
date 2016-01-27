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

#include <iosfwd>
#include <ios>
#include <istream>
#include <ostream>
#include <string>    // char traits            
#include <cstddef>   // ptrdiff_t
#include <cmath>
#include "assert.h"
#include "buffer.h"  
begin_gtl_namespace
begin_gdb_namespace
 

class SRSNode
{
	char    *    pszValue;
	SRSNode **   papoChildNodes;
	SRSNode *    poParent;
	int          nChildren;
private:
	int         needsQuoting() const;
	bool        importFromWkt(char **, int nRecLevel, int* pnNodes);
public:
	SRSNode(const char * = 0);
	~SRSNode();
public:
	int         isLeafNode() const { return nChildren == 0; }
	int         getChildCount() const { return nChildren; }
	SRSNode *   getChild(int);
	const SRSNode *getChild(int) const;

	SRSNode *  getNode(const char *);
	const SRSNode *getNode(const char *) const;

	void        insertChild(SRSNode *, int);
	void        addChild(SRSNode *);
	int         findChild(const char *) const;
	void        destroyChild(int);
	void        clearChildren();
	void        stripNodes(const char *);

	const char  *getValue() const { return pszValue; }
	void        setValue(const char *);

	void        makeValueSafe();
	bool        fixupOrdering();

	SRSNode *   clone() const;

	bool      importFromWkt(char **);
	bool      exportToWkt(char **) const;
	bool      exportToPrettyWkt(char **, int = 1) const;

	bool      applyRemapper(const char *pszNode,
		char **papszSrcValues,
		char **papszDstValues,
		int nStepSize = 1,
		int bChildOfHit = FALSE);
};

end_gdb_namespace
end_gtl_namespace

