#include "internal_versatilevertex3dvisitorImpl.h"
 
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
double & VersatileVertex3dVisitorImpl::x(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->x;
}
double & VersatileVertex3dVisitorImpl::y(long i){
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->y;
}
double & VersatileVertex3dVisitorImpl::z(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->z;
}
double & VersatileVertex3dVisitorImpl::nx(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->nx;
}
double & VersatileVertex3dVisitorImpl::ny(long i){
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->ny;
}
double & VersatileVertex3dVisitorImpl::nz(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->nz;
}
float & VersatileVertex3dVisitorImpl::r(long i){
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->cR;
}
float & VersatileVertex3dVisitorImpl::g(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->cG;
}
float & VersatileVertex3dVisitorImpl::b(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->cB;
}
double & VersatileVertex3dVisitorImpl::u(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->u;
}
double & VersatileVertex3dVisitorImpl::v(long i) {
	return (((VersatileVertex3d*)(*_vertexlist)) + i)->v;
}


void VersatileVertex3dVisitorImpl::addTail(
	double xx, double yy, double zz,
	double uu, double vv,
	float rr, float gg, float bb,
	double nxx, double nyy, double nzz)
{
	VersatileVertex3d* p = new VersatileVertex3d[(*_vertexnumber) + 1];
	memmove((void*)p, (void*)(*_vertexlist), (sizeof(VersatileVertex3d)*(*_vertexnumber)));
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
	u((*_vertexnumber)) = uu;
	v((*_vertexnumber)) = vv;

	(*_vertexnumber)++;
}

void VersatileVertex3dVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	VersatileVertex3d* p = new VersatileVertex3d[(*_vertexnumber) - 1];
	memmove((void*)p, (void*)(*_vertexlist), sizeof(VersatileVertex3d)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}

void VersatileVertex3dVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	VersatileVertex3d* p = new VersatileVertex3d[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(VersatileVertex3d)*(*_vertexnumber));
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

		u((*_vertexnumber) + i) = uu[i];
		v((*_vertexnumber) + i) = vv[i];
	}
	(*_vertexnumber) += pointsnumber;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
