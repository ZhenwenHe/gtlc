#include "vertex3f.h" 



begin_gtl_namespace
begin_gdb_namespace
Vertex3f::Vertex3f(float xx, float yy, float zz) :x(xx), y(yy), z(zz){
}
Vertex3f::Vertex3f() : x(0), y(0), z(0){
}
Vertex3f Vertex3f::operator = (const Vertex3f & c){
	x = c.x; y = c.y; z = c.z;
	return *this;
}

const Vertex3f operator*(const Vertex3f& lhv, float rhd)
{
	Vertex3f vRes = lhv;
	vRes *= rhd;
	return vRes;
}

const Vertex3f operator*(float lhd, const Vertex3f& rhv)
{
	Vertex3f vRes = rhv;
	vRes *= lhd;
	return vRes;
}

float operator*(const Vertex3f& lhv, const Vertex3f& rhv)
{
	return lhv.x * rhv.x + lhv.y * rhv.y + lhv.z * rhv.z;
}

bool operator==(const Vertex3f& v1, const Vertex3f& v2){
	if (fabs(v1.x - v2.x) > TOLERANCE || fabs(v1.y - v2.y) > TOLERANCE || fabs(v1.z - v2.z) > TOLERANCE)
		return false;
	else
		return true;
}

bool operator!=(const Vertex3f& v1, const Vertex3f& v2){
	if (fabs(v1.x - v2.x) > TOLERANCE || fabs(v1.y - v2.y) > TOLERANCE || fabs(v1.z - v2.z) > TOLERANCE)
		return true;
	else
		return false;
}

end_gdb_namespace
end_gtl_namespace

