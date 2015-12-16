#include "vertex3d.h" 
#include "mathex.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
////////////////////////////////////////////////////////////////////////
Vertex3d::Vertex3d(double xx, double yy, double zz) :x(xx), y(yy), z(zz){
}
Vertex3d::Vertex3d() : x(0), y(0), z(0){
}
Vertex3d Vertex3d::operator = (const Vertex3d & c){
	x = c.x; y = c.y; z = c.z;
	return *this;
}

const Vertex3d operator*(const Vertex3d& lhv, double rhd)
{
	Vertex3d vRes = lhv;
	vRes *= rhd;
	return vRes;
}
const Vertex3d operator/(const Vertex3d& lhv, double rhd)
{
	Vertex3d vRes = lhv;
	vRes.x /= rhd;
	vRes.y /= rhd;
	vRes.z /= rhd;

	return vRes;
}

const Vertex3d operator*(double lhd, const Vertex3d& rhv)
{
	Vertex3d vRes = rhv;
	vRes *= lhd;
	return vRes;
}

double operator*(const Vertex3d& lhv, const Vertex3d& rhv)
{
	return lhv.x * rhv.x + lhv.y * rhv.y + lhv.z * rhv.z;
}


Vertex3d operator-(const Vertex3d & c1, const Vertex3d & c2){
	return Vertex3d(c1.x - c2.x, c1.y - c2.y, c1.z - c2.z);
}
Vertex3d operator+(const Vertex3d & c1, const Vertex3d & c2){
	return Vertex3d(c1.x + c2.x, c1.y + c2.y, c1.z + c2.z);
}
bool operator==(const Vertex3d& v1, const Vertex3d& v2){
	if (fabs(v1.x - v2.x) > TOLERANCE || fabs(v1.y - v2.y) > TOLERANCE || fabs(v1.z - v2.z) > TOLERANCE)
		return false;
	else
		return true;
}

bool operator!=(const Vertex3d& v1, const Vertex3d& v2){
	if (fabs(v1.x - v2.x) > TOLERANCE || fabs(v1.y - v2.y) > TOLERANCE || fabs(v1.z - v2.z) > TOLERANCE)
		return true;
	else
		return false;
}

const Vertex3d operator^(const Vertex3d& lhv, const Vertex3d& rhv)
{
	Vertex3d vRes = lhv;
	vRes ^= rhv;
	return vRes;
}

void Vertex3d::operator/=(double rhd)
{
	if (fabs(rhd) < MathEx::TOL)	// 判断除数是否为0，若为0，则不处理
		return;
	else
	{
		x /= rhd;
		y /= rhd;
		z /= rhd;
	}
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace