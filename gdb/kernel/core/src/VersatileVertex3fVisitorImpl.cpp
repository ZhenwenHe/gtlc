#include "internal_VersatileVertex3fVisitorImpl.h"  
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
float & VersatileVertex3fVisitorImpl::x(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->x;
}
float & VersatileVertex3fVisitorImpl::y(long i){
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->y;
}
float & VersatileVertex3fVisitorImpl::z(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->z;
}
float & VersatileVertex3fVisitorImpl::nx(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->nx;
}
float & VersatileVertex3fVisitorImpl::ny(long i){
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->ny;
}
float & VersatileVertex3fVisitorImpl::nz(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->nz;
}
float & VersatileVertex3fVisitorImpl::r(long i){
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->cR;
}
float & VersatileVertex3fVisitorImpl::g(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->cG;
}
float & VersatileVertex3fVisitorImpl::b(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->cB;
}
float & VersatileVertex3fVisitorImpl::u(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->u;
}
float & VersatileVertex3fVisitorImpl::v(long i) {
	return (((VersatileVertex3f*)(*_vertexlist)) + i)->v;
}

void VersatileVertex3fVisitorImpl::addTail(
	double xx, double yy, double zz,
	double uu, double vv,
	float rr, float gg, float bb,
	double nxx, double nyy, double nzz)
{
	VersatileVertex3f* p = new VersatileVertex3f[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(VersatileVertex3f)*(*_vertexnumber));
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
void VersatileVertex3fVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	VersatileVertex3f* p = new VersatileVertex3f[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(VersatileVertex3f)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}
void VersatileVertex3fVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	VersatileVertex3f* p = new VersatileVertex3f[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(VersatileVertex3f)*(*_vertexnumber));
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
