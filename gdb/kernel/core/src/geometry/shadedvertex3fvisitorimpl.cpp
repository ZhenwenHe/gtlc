#include "internal_shadedvertex3fvisitorimpl.h"




begin_gtl_namespace
begin_gdb_namespace
float & ShadedVertex3fVisitorImpl::x(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->x;
}
float & ShadedVertex3fVisitorImpl::y(long i){
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->y;
}
float & ShadedVertex3fVisitorImpl::z(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->z;
}
float & ShadedVertex3fVisitorImpl::nx(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->nx;
}
float & ShadedVertex3fVisitorImpl::ny(long i){
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->ny;
}
float & ShadedVertex3fVisitorImpl::nz(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->nz;
}
float & ShadedVertex3fVisitorImpl::r(long i){
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->cR;
}
float & ShadedVertex3fVisitorImpl::g(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->cG;
}
float & ShadedVertex3fVisitorImpl::b(long i) {
	return (((ShadedVertex3f*)(*_vertexlist)) + i)->cB;
}

void ShadedVertex3fVisitorImpl::addTail(
	double xx, double yy, double zz,
	double uu, double vv,
	float rr, float gg, float bb,
	double nxx, double nyy, double nzz)
{
	ShadedVertex3f* p = new ShadedVertex3f[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3f)*(*_vertexnumber));

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
void ShadedVertex3fVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	ShadedVertex3f* p = new ShadedVertex3f[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3f)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}

void ShadedVertex3fVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	ShadedVertex3f* p = new ShadedVertex3f[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(ShadedVertex3f)*(*_vertexnumber));
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
end_gtl_namespace


