#include "internal_vertex3fvisitorimpl.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
float & Vertex3fVisitorImpl::x(long i) {
	return (((Vertex3f*)(*_vertexlist)) + i)->x;
}
float & Vertex3fVisitorImpl::y(long i){
	return (((Vertex3f*)(*_vertexlist)) + i)->y;
}
float & Vertex3fVisitorImpl::z(long i) {
	return (((Vertex3f*)(*_vertexlist)) + i)->z;
}
void Vertex3fVisitorImpl::addTail(
	double xx, double yy, double zz,
	double u, double v,
	float r, float g, float b,
	double nx, double ny, double nz)
{
	Vertex3f* p = new Vertex3f[(*_vertexnumber) + 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3f)*(*_vertexnumber));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	x((*_vertexnumber)) = xx;
	y((*_vertexnumber)) = yy;
	z((*_vertexnumber)) = zz;

	(*_vertexnumber)++;
}
void Vertex3fVisitorImpl::removeTail(){
	if ((*_vertexnumber) == 0) return;
	if ((*_vertexnumber) == 1){
		releaseVertexArray((*_vertextype), (*_vertexlist));
		(*_vertexnumber)--;
		return;
	}
	Vertex3f* p = new Vertex3f[(*_vertexnumber) - 1];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3f)*((*_vertexnumber) - 1));
	releaseVertexArray((*_vertextype), (*_vertexlist));
	(*_vertexlist) = p;
	(*_vertexnumber)--;
}
void Vertex3fVisitorImpl::append(const int pointsnumber,
	const double *xx, const double *yy, const double *zz,
	const double *uu, const double *vv,
	const float *rr, const float *gg, const float *bb,
	const double *nxx, const double *nyy, const double *nzz)
{
	Vertex3f* p = new Vertex3f[(*_vertexnumber) + pointsnumber];
	memcpy((void*)p, (void*)(*_vertexlist), sizeof(Vertex3f)*(*_vertexnumber));
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