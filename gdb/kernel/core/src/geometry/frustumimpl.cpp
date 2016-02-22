#include "internal_frustumimpl.h" 
#include "mathex.h"
#include "quaternion.h"
#include "internal_spatialfuns.h"


begin_gtl_namespace
begin_gdb_namespace
 

FrustumImpl::FrustumImpl(void)
{
}

FrustumImpl::~FrustumImpl(void)
{
}

void FrustumImpl::setCamInternals(double nearD, double farD, double angle, double ratio) {

	this->ratio = ratio;
	this->angle = angle;
	this->nearD = nearD;
	this->farD = farD;

	tang = (double)tan(angle* MathEx::dDegToRad * 0.5);

	// 近裁切面的高度和宽度（一半）
	nh = nearD * tang;
	nw = nh * ratio;

	// 远裁切面的高度和宽度（一半）
	fh = farD  * tang;
	fw = fh * ratio;

	// calculate the length of the fov triangle
	double fDepth = nh / tang;// calculate the corner of the screen
	double fCorner = sqrt(nh * nh + nw * nw);// now calculate the new fov
	coneFov = atan(fCorner / fDepth);
}

void FrustumImpl::setCamera(Vertex3d& eye, Vertex3d& cen)
{
	Vertex3d p(eye.x, eye.y, eye.z), l(cen.x, cen.y, cen.z);
	Vertex3d dir, nc, fc, X, Y, Z;

	Z = p - l;
	Z.normalize();

	//这里up分量实际上不起任何作用，因为后面需要重新计算
	Vertex3d u = Vertex3d(0, 0, 1); // 默认就为0，0，1 
	X = Z ^ u;
	X.normalize();

	coneDir = -Z;
	coneCen = p;

	Y = Z ^ X;	// 这才是真正的相机up分量

	nc = p - Z * nearD; // 近裁切面中心点
	fc = p - Z * farD;  // 远裁切面中心点

	sphCen = (nc + fc) / 2;

	Vertex3d ntl, ntr, nbl, nbr, ftl, ftr, fbl, fbr;

	vPlaneVertices[0] = ntl = nc + Y * nh - X * nw;
	vPlaneVertices[1] = ntr = nc + Y * nh + X * nw;
	vPlaneVertices[2] = nbl = nc - Y * nh - X * nw;
	vPlaneVertices[3] = nbr = nc - Y * nh + X * nw;

	vPlaneVertices[4] = ftl = fc + Y * fh - X * fw;
	vPlaneVertices[5] = ftr = fc + Y * fh + X * fw;
	vPlaneVertices[6] = fbl = fc - Y * fh - X * fw;
	vPlaneVertices[7] = fbr = fc - Y * fh + X * fw;

	radius = DPointToPoint(sphCen, ftl);

	// 这里所有plane的法向量朝向frustum内部
	GetPlaneEquationEx(ntr, ntl, ftl, plane[TOP]);

	GetPlaneEquationEx(nbl, nbr, fbr, plane[BOTTOM]);

	GetPlaneEquationEx(ntl, nbl, fbl, plane[LEFT]);

	GetPlaneEquationEx(nbr, ntr, fbr, plane[RIGHT]);

	GetPlaneEquationEx(ntl, ntr, nbr, plane[NEARP]);

	GetPlaneEquationEx(ftr, ftl, fbl, plane[FARP]);


	for (int i = 0; i < 6; i++)
	{
		vPlaneNor[i] = Vertex3d(plane[i].A, plane[i].B, plane[i].C);
		vPlaneNorSign[i][0] = plane[i].A > -MathEx::TOL;
		vPlaneNorSign[i][1] = plane[i].B > -MathEx::TOL;
		vPlaneNorSign[i][2] = plane[i].C > -MathEx::TOL;
	}
}


FrustumImpl::FrustumImpl(const FrustumImpl & f)
{
	for (int i = 0; i < 6; i++)
	{
		plane[i] = f.plane[i];
		vPlaneNor[i] = f.vPlaneNor[i];
		vPlaneNorSign[i][0] = f.vPlaneNorSign[i][0];
		vPlaneNorSign[i][1] = f.vPlaneNorSign[i][1];
		vPlaneNorSign[i][2] = f.vPlaneNorSign[i][2];
	}
	for (int i = 0; i < 8; i++)
		vPlaneVertices[i] = f.vPlaneVertices[i];

	nearD = f.nearD;
	farD = f.farD;
	ratio = f.ratio;
	angle = f.angle;
	tang = f.tang;
	nw = f.nw;
	nh = f.nh;
	fw = f.fw;
	fh = f.fh;

	radius = f.radius;
	sphCen = f.sphCen;

	coneCen = f.coneCen;
	coneFov = f.coneFov;
	coneDir = f.coneDir;
}

void FrustumImpl::set(Vertex3d& eye, Vertex3d& cen, double nearD, double farD, double angle, double ratio)
{
	setCamInternals(nearD, farD, angle, ratio);
	setCamera(eye, cen);
} 

FrustumSharedPtr FrustumImpl::clone()
{
	return FrustumSharedPtr((Frustum *) new FrustumImpl(*this));
}

end_gdb_namespace
end_gtl_namespace



