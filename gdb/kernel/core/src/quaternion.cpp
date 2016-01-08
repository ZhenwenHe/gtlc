#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "config.h"
#include "vertex3d.h"
#include "vertex4d.h"
#include "matrix3x3.h"
#include "quaternion.h"
begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
void Quaternion::toRotateMatrix(Matrix3x3& kRot) const
{
	double fTx = 2.0*x();
	double fTy = 2.0*y();
	double fTz = 2.0*z();
	double fTwx = fTx*w();
	double fTwy = fTy*w();
	double fTwz = fTz*w();
	double fTxx = fTx*x();
	double fTxy = fTy*x();
	double fTxz = fTz*x();
	double fTyy = fTy*y();
	double fTyz = fTz*y();
	double fTzz = fTz*z();

	kRot[0][0] = 1.0 - (fTyy + fTzz);
	kRot[0][1] = fTxy - fTwz;
	kRot[0][2] = fTxz + fTwy;
	kRot[1][0] = fTxy + fTwz;
	kRot[1][1] = 1.0 - (fTxx + fTzz);
	kRot[1][2] = fTyz - fTwx;
	kRot[2][0] = fTxz - fTwy;
	kRot[2][1] = fTyz + fTwx;
	kRot[2][2] = 1.0 - (fTxx + fTyy);

	kRot.flip();
}
void Quaternion::fromRotateMatrix(const Matrix3x3& kRot)
{
	// Algorithm in Ken Shoemake's article in 1987 SIGGRAPH course notes
	// article "Quaternion Calculus and Fast Animation".

	Matrix3x3 m3x3(kRot);
	m3x3.flip();

	double fTrace = m3x3[0][0] + m3x3[1][1] + m3x3[2][2];
	double fRoot;

	if (fTrace > 0.0)
	{
		// |w| > 1/2, may as well choose w > 1/2
		fRoot = sqrt(fTrace + 1.0);  // 2w
		_v[3] = 0.5*fRoot;
		fRoot = 0.5 / fRoot;  // 1/(4w)
		_v[0] = (m3x3[2][1] - m3x3[1][2])*fRoot;
		_v[1] = (m3x3[0][2] - m3x3[2][0])*fRoot;
		_v[2] = (m3x3[1][0] - m3x3[0][1])*fRoot;
	}
	else
	{
		// |w| <= 1/2
		static size_t s_iNext[3] = { 1, 2, 0 };
		size_t i = 0;
		if (m3x3[1][1] > m3x3[0][0])
			i = 1;
		if (m3x3[2][2] > m3x3[i][i])
			i = 2;
		size_t j = s_iNext[i];
		size_t k = s_iNext[j];

		fRoot = sqrt(m3x3[i][i] - m3x3[j][j] - m3x3[k][k] + 1.0);
		double* apkQuat[3] = { &_v[0], &_v[1], &_v[2] };
		*apkQuat[i] = 0.5*fRoot;
		fRoot = 0.5 / fRoot;
		_v[3] = (m3x3[k][j] - m3x3[j][k])*fRoot;
		*apkQuat[j] = (m3x3[j][i] + m3x3[i][j])*fRoot;
		*apkQuat[k] = (m3x3[k][i] + m3x3[i][k])*fRoot;
	}
}

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
