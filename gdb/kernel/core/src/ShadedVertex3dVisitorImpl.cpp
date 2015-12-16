#include "internal_ShadedVertex3dVisitorImpl.h"
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
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
double & ShadedVertex3dVisitorImpl::x(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->x;
}
double & ShadedVertex3dVisitorImpl::y(long i){
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->y;
}
double & ShadedVertex3dVisitorImpl::z(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->z;
}
double & ShadedVertex3dVisitorImpl::nx(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->nx;
}
double & ShadedVertex3dVisitorImpl::ny(long i){
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->ny;
}
double & ShadedVertex3dVisitorImpl::nz(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->nz;
}
float & ShadedVertex3dVisitorImpl::r(long i){
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->cR;
}
float & ShadedVertex3dVisitorImpl::g(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->cG;
}
float & ShadedVertex3dVisitorImpl::b(long i) {
	return (((ShadedVertex3d*)(*_vertexlist)) + i)->cB;
}
void ShadedVertex3dVisitorImpl::addTail(
	double xx, double yy, double zz,
	double uu, double vv,
	float rr, float gg, float bb,
	double nxx, double nyy, double nzz)
{
	ShadedVertex3d* p = new ShadedVertex3d[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3d)*(*_vertexnumber));

	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;

	x((*_vertexnumber)) = xx;
	y((*_vertexnumber)) = yy;
	z((*_vertexnumber)) = zz;
	nx((*_vertexnumber)) = nxx;
	ny((*_vertexnumber)) = nyy;
	nz((*_vertexnumber)) = nzz;
	r((*_vertexnumber)) = rr;
	g((*_vertexnumber)) = gg;
	b((*_vertexnumber)) = bb;

	(*_vertexnumber)++;
}
void ShadedVertex3dVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	ShadedVertex3d* p = new ShadedVertex3d[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3d)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}

void ShadedVertex3dVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	ShadedVertex3d* p = new ShadedVertex3d[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3d)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));

	(*_vertexlist) = p;
	for (int i = 0; i < pointsnumber; i++)
	{
		x((*_vertexnumber) + i) = xx[i];
		y((*_vertexnumber) + i) = yy[i];
		z((*_vertexnumber) + i) = zz[i];

		nx((*_vertexnumber) + i) = nxx[i];
		ny((*_vertexnumber) + i) = nyy[i];
		nz((*_vertexnumber) + i) = nzz[i];

		r((*_vertexnumber) + i) = rr[i];
		g((*_vertexnumber) + i) = gg[i];
		b((*_vertexnumber) + i) = bb[i];
	}
	(*_vertexnumber) += pointsnumber;
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
