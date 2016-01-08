#include "internal_vertex3dvisitorimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

double & Vertex3dVisitorImpl::x(long i) {
	return (((Vertex3d*)(*_vertexlist)) + i)->x;
}
double & Vertex3dVisitorImpl::y(long i){
	return (((Vertex3d*)(*_vertexlist)) + i)->y;
}
double & Vertex3dVisitorImpl::z(long i) {
	return (((Vertex3d*)(*_vertexlist)) + i)->z;
}
void Vertex3dVisitorImpl::addTail(
	double xx, double yy, double zz,
	double u, double v,
	float r, float g, float b,
	double nx, double ny, double nz)
{

	Vertex3d* p = new Vertex3d[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3d)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));

	(*_vertexlist) = p;
	x((*_vertexnumber)) = xx;
	y((*_vertexnumber)) = yy;
	z((*_vertexnumber)) = zz;

	(*_vertexnumber)++;
}
void Vertex3dVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	Vertex3d* p = new Vertex3d[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3d)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}
void Vertex3dVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	Vertex3d* p = new Vertex3d[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3d)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));

	(*_vertexlist) = p;
	for (int i = 0; i < pointsnumber; i++)
	{
		x((*_vertexnumber) + i) = xx[i];
		y((*_vertexnumber) + i) = yy[i];
		z((*_vertexnumber) + i) = zz[i];
	}
	(*_vertexnumber) += pointsnumber;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace