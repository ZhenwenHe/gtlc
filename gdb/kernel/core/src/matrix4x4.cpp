#include "config.h"
#include "mathex.h"
#include "matrix4x4.h"
#include "quaternion.h"


begin_gtl_namespace
begin_gdb_namespace

const Matrix4x4 Matrix4x4::IDENTITY4X4(1,0,0,0, 0,1,0,0, 0,0,1,0, 0,0,0,1);


Vertex3d operator*(const Vertex3d& lhv, const Matrix4x4& rhm)
{
	return lhv;
}

void Matrix4x4::buildScaleMatrix(const Vertex3d& scaleVec,const Vertex3d& center)
{
	operator=(IDENTITY4X4); 
	setScaleVector(scaleVec);

	Matrix4x4 m4x4 = IDENTITY4X4;
	Matrix4x4 mm = (*this);
	m4x4  -=  mm;

	m4x4.m_dData[15] = 1;

	Vertex3d cen = center;
	m4x4.transformAffine(cen);
	setTranslateVector(cen);
}

void Matrix4x4::buildRotateMatrix(const Matrix3x3& m3x3,const Vertex3d& center)
{
	operator=(IDENTITY4X4); 
	setRotateMatrix(m3x3);

	Matrix4x4 m4x4 = IDENTITY4X4;
	Matrix4x4 mm = (*this);
	m4x4  -=  mm;

	m4x4.m_dData[15] = 1;

	Vertex3d cen = center;
	m4x4.transformAffine(cen);
	setTranslateVector(cen);
}



void Matrix4x4::buildRotateMatrix(const Quaternion& rot, const Vertex3d& center)
{
	operator=(IDENTITY4X4);
	setRotateQuaternion(rot);

	Matrix4x4 m4x4 = IDENTITY4X4;
	Matrix4x4 mm = (*this);
	m4x4 -= mm;

	m4x4.m_dData[15] = 1;

	Vertex3d cen = center;
	m4x4.transformAffine(cen);
	setTranslateVector(cen);
}

Matrix4x4 Matrix4x4::inverseAffine(void) const
{

	double m10 = (*this)[1][0], m11 = (*this)[1][1], m12 = (*this)[1][2];
	double m20 = (*this)[2][0], m21 = (*this)[2][1], m22 = (*this)[2][2];

	double t00 = m22 * m11 - m21 * m12;
	double t10 = m20 * m12 - m22 * m10;
	double t20 = m21 * m10 - m20 * m11;

	double m00 = (*this)[0][0], m01 = (*this)[0][1], m02 = (*this)[0][2];

	double invDet = 1 / (m00 * t00 + m01 * t10 + m02 * t20);

	t00 *= invDet; t10 *= invDet; t20 *= invDet;

	m00 *= invDet; m01 *= invDet; m02 *= invDet;

	double r00 = t00;
	double r01 = m02 * m21 - m01 * m22;
	double r02 = m01 * m12 - m02 * m11;

	double r10 = t10;
	double r11 = m00 * m22 - m02 * m20;
	double r12 = m02 * m10 - m00 * m12;

	double r20 = t20;
	double r21 = m01 * m20 - m00 * m21;
	double r22 = m00 * m11 - m01 * m10;

	double m03 = (*this)[0][3], m13 = (*this)[1][3], m23 = (*this)[2][3];

	double r03 = - (r00 * m03 + r01 * m13 + r02 * m23);
	double r13 = - (r10 * m03 + r11 * m13 + r12 * m23);
	double r23 = - (r20 * m03 + r21 * m13 + r22 * m23);

	return Matrix4x4(
		r00, r01, r02, r03,
		r10, r11, r12, r13,
		r20, r21, r22, r23,
		0,   0,   0,   1);
}



void Matrix4x4::buildTranslateMatrix(const Vertex3d& v)
{
	operator=(IDENTITY4X4);
	setTranslateVector(v);
}

void Matrix4x4::buildTranslateMatrix(double dx, double dy, double dz)
{
	Vertex3d v(dx,dy,dz);
	operator=(IDENTITY4X4);
	setTranslateVector(v);
}
void Matrix4x4::buildTranslateMatrix(Vertex3d & d)
{
	operator=(IDENTITY4X4);
	setTranslateVector(d);
}

void Matrix4x4::buildTransformMatrix(const Vertex3d& position, const Vertex3d& scale, const Quaternion& orientation,
	const Vertex3d& scaleCenter, const Vertex3d& rotateCenter)
{
	// 默认顺序为 先缩放、再旋转，再平移
	operator=(IDENTITY4X4); // 化为单位矩阵

	Matrix4x4 trans4x4;
	trans4x4.buildTranslateMatrix(position);

	Matrix4x4 scale4x4;
	scale4x4.buildScaleMatrix(scale, scaleCenter); // 最后中心缩放

	Matrix4x4 rot4x4; // 直接初始化为旋转矩阵
	rot4x4.buildRotateMatrix(orientation, rotateCenter);

	(*this) *= trans4x4;
	(*this) *= scale4x4;
	(*this) *= rot4x4;

	//SetTransVector(position); // 设置平移
}

end_gdb_namespace
end_gtl_namespace


