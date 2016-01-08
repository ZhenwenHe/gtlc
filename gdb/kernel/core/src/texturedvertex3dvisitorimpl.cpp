#include "internal_texturedvertex3dvisitorimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
double & TexturedVertex3dVisitorImpl::x(long i) {
	return (((TexturedVertex3d*)(*_vertexlist)) + i)->x;
}
double & TexturedVertex3dVisitorImpl::y(long i){
	return (((TexturedVertex3d*)(*_vertexlist)) + i)->y;
}
double & TexturedVertex3dVisitorImpl::z(long i) {
	return (((TexturedVertex3d*)(*_vertexlist)) + i)->z;
}
double & TexturedVertex3dVisitorImpl::u(long i){
	return (((TexturedVertex3d*)(*_vertexlist)) + i)->u;
}
double & TexturedVertex3dVisitorImpl::v(long i) {
	return (((TexturedVertex3d*)(*_vertexlist)) + i)->v;
}
void TexturedVertex3dVisitorImpl::addTail(
	double xx, double yy, double zz,
	double uu, double vv,
	float rr, float gg, float bb,
	double nxx, double nyy, double nzz)
{
	TexturedVertex3d* p = new TexturedVertex3d[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(TexturedVertex3d)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;

	x((*_vertexnumber)) = xx;
	y((*_vertexnumber)) = yy;
	z((*_vertexnumber)) = zz;
	u((*_vertexnumber)) = uu;
	v((*_vertexnumber)) == vv;

	(*_vertexnumber)++;
}
void TexturedVertex3dVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	TexturedVertex3d* p = new TexturedVertex3d[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(TexturedVertex3d)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}

void TexturedVertex3dVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	TexturedVertex3d* p = new TexturedVertex3d[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(TexturedVertex3d)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));

	(*_vertexlist) = p;
	for (int i = 0; i < pointsnumber; i++)
	{
		x((*_vertexnumber) + i) = xx[i];
		y((*_vertexnumber) + i) = yy[i];
		z((*_vertexnumber) + i) = zz[i];
		u((*_vertexnumber) + i) = uu[i];
		v((*_vertexnumber) + i) = vv[i];
	}
	(*_vertexnumber) += pointsnumber;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
