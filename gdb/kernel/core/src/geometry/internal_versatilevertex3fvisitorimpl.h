#pragma once 

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
#include "vertexvisitor.h"


begin_gtl_namespace
begin_gdb_namespace
class  VersatileVertex3fVisitorImpl :public VersatileVertex3fVisitor {
public:
	virtual float & x(long i);
	virtual float & y(long i);
	virtual float & z(long i);
	virtual float & nx(long i);
	virtual float & ny(long i);
	virtual float & nz(long i);
	virtual float & u(long i);
	virtual float & v(long i);
	virtual float & r(long i);
	virtual float & g(long i);
	virtual float & b(long i);
	virtual void addTail(double x, double y, double z,
		double u = 0, double v = 0,
		float r = 0, float g = 0, float b = 0,
		double nx = 0, double ny = 0, double nz = 0);
	virtual void removeTail();
	virtual void append(const int pointsnumber,
		const double *x, const double *y, const double *z,
		const double *u = 0, const double *v = 0,
		const float *r = 0, const float *g = 0, const float *b = 0,
		const double *nx = 0, const double *ny = 0, const double *nz = 0);
};

end_gdb_namespace
end_gtl_namespace


