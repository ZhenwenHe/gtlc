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
#include <memory>
#include "assert.h"
#include <cmath>
#include "config.h"
#include "coloredvertex3f.h"
#include "coloredvertex3d.h"
#include "texturedvertex3f.h"
#include "texturedvertex3d.h"
#include "shadedvertex3f.h"
#include "shadedvertex3d.h"
#include "versatilevertex3d.h"
#include "versatilevertex3f.h"



begin_gtl_namespace
begin_gdb_namespace

/** @addtogroup vertex 点的类型
*  @{
*/
enum
{
	VERTEXTYPE_V3f,				// for Vertex3f
	VERTEXTYPE_V3d,				// for Vertex3d

	VERTEXTYPE_V3fT2f,			// for TexturedVertex3f
	VERTEXTYPE_V3dT2d,			// for TexturedVertex3d

	VERTEXTYPE_V3fN3fC3f,		// for ShadedVertex3f
	VERTEXTYPE_V3dN3dC3f,		// for ShadedVertex3d

	VERTEXTYPE_V3fT2fN3fC3f,	// for VersatileVertex3f
	VERTEXTYPE_V3dT2dN3dC3f,	// for VersatileVertex3d
};

class VertexVisitor;
typedef std::shared_ptr<VertexVisitor> VertexVisitorSharedPtr;
typedef std::auto_ptr<VertexVisitor> VertexVisitorAutoPtr;
typedef VertexVisitor* VertexVisitorPtr;
/** @} */
/** @addtogroup vertex VertexVisitor
*  @{
*/
class CORE_API VertexVisitor{

protected:
	/** 顶点列表 */
	void** _vertexlist;

	/** 顶点列表长度，亦即顶点数目 */
	long* _vertexnumber;

	/** 顶点类型编码 */
	unsigned char* _vertextype;
public:
	VertexVisitor();
	VertexVisitor(void * &vl, long &vn, unsigned char &vt);
	inline void set(void * &vl, long &vn, unsigned char &vt);
	inline long getVertexNumber();
	inline unsigned char getType();
	virtual void addTail(double x, double y, double z,
		double u = 0, double v = 0,
		float r = 0, float g = 0, float b = 0,
		double nx = 0, double ny = 0, double nz = 0) = 0;
	virtual void removeTail() = 0;

	virtual void append(const int pointsnumber,
		const double *x, const double *y, const double *z,
		const double *u = 0, const double *v = 0,
		const float *r = 0, const float *g = 0, const float *b = 0,
		const double *nx = 0, const double *ny = 0, const double *nz = 0) = 0;
public:
	static VertexVisitorSharedPtr create(int signal);
	static int releaseVertexArray(unsigned char iVertexType, void*& pVertexPointer);
};


inline void VertexVisitor::set(void * &vl, long &vn, unsigned char &vt){
	_vertexlist = &vl;
	_vertexnumber = &vn;
	_vertextype = &vt;
}

inline long VertexVisitor::getVertexNumber(){ return *_vertexnumber; }

inline unsigned char VertexVisitor::getType() { return *_vertextype; }

/** @} */
/** @addtogroup vertex Vertex3dVisitor
*  @{
*/
class CORE_API Vertex3dVisitor : public VertexVisitor{
public:
	virtual double & x(long i) = 0;
	virtual double & y(long i) = 0;
	virtual double & z(long i) = 0;
};
/** @} */
/** @addtogroup vertex Vertex3fVisitor
*  @{
*/
class CORE_API Vertex3fVisitor : public VertexVisitor{
public:
	virtual float & x(long i) = 0;
	virtual float & y(long i) = 0;
	virtual float & z(long i) = 0;
};
/** @} */

/** @addtogroup vertex TexturedVertex3fVisitor
*  @{
*/
class CORE_API TexturedVertex3fVisitor :public VertexVisitor {
public:
	virtual float & x(long i) = 0;
	virtual float & y(long i) = 0;
	virtual float & z(long i) = 0;
	virtual float & u(long i) = 0;
	virtual float & v(long i) = 0;
};
/** @} */
/** @addtogroup vertex TexturedVertex3dVisitor
*  @{
*/
class CORE_API TexturedVertex3dVisitor :public VertexVisitor {
public:
	virtual double & x(long i) = 0;
	virtual double & y(long i) = 0;
	virtual double & z(long i) = 0;
	virtual double & u(long i) = 0;
	virtual double & v(long i) = 0;
};
/** @} */
/** @addtogroup vertex ShadedVertex3fVisitor
*  @{
*/
class CORE_API ShadedVertex3fVisitor :public VertexVisitor {
public:
	virtual float & x(long i) = 0;
	virtual float & y(long i) = 0;
	virtual float & z(long i) = 0;
	virtual float & nx(long i) = 0;
	virtual float & ny(long i) = 0;
	virtual float & nz(long i) = 0;
	virtual float & r(long i) = 0;
	virtual float & g(long i) = 0;
	virtual float & b(long i) = 0;
};
/** @} */
/** @addtogroup vertex ShadedVertex3dVisitor
*  @{
*/
class CORE_API ShadedVertex3dVisitor :public VertexVisitor {
public:
	virtual double & x(long i) = 0;
	virtual double & y(long i) = 0;
	virtual double & z(long i) = 0;
	virtual double & nx(long i) = 0;
	virtual double & ny(long i) = 0;
	virtual double & nz(long i) = 0;
	virtual float & r(long i) = 0;
	virtual float & g(long i) = 0;
	virtual float & b(long i) = 0;
};
/** @} */
/** @addtogroup vertex VersatileVertex3fVisitor
*  @{
*/
class CORE_API VersatileVertex3fVisitor :public VertexVisitor {
public:
	virtual float & x(long i) = 0;
	virtual float & y(long i) = 0;
	virtual float & z(long i) = 0;
	virtual float & nx(long i) = 0;
	virtual float & ny(long i) = 0;
	virtual float & nz(long i) = 0;
	virtual float & u(long i) = 0;
	virtual float & v(long i) = 0;
	virtual float & r(long i) = 0;
	virtual float & g(long i) = 0;
	virtual float & b(long i) = 0;
};
/** @} */
/** @addtogroup vertex VersatileVertex3dVisitor
*  @{
*/
class CORE_API VersatileVertex3dVisitor :public VertexVisitor {
public:
	virtual double & x(long i) = 0;
	virtual double & y(long i) = 0;
	virtual double & z(long i) = 0;
	virtual double & nx(long i) = 0;
	virtual double & ny(long i) = 0;
	virtual double & nz(long i) = 0;
	virtual double & u(long i) = 0;
	virtual double & v(long i) = 0;
	virtual float & r(long i) = 0;
	virtual float & g(long i) = 0;
	virtual float & b(long i) = 0;
};
/** @} */
end_gdb_namespace
end_gtl_namespace



