#include "config.h"
#include "mathex.h"
#include "matrix3x3.h"
#include "quaternion.h"



begin_gtl_namespace
begin_gdb_namespace

const Matrix3x3 Matrix3x3::IDENTITY3X3(1,0,0, 0,1,0, 0,0,1);

// double Matrix3x3::Det () const
// {
// 	double fCofactor00 = (*this)[1][1]*(*this)[2][2] -
// 		(*this)[1][2]*(*this)[2][1];
// 	double fCofactor10 = (*this)[1][2]*(*this)[2][0] -
// 		(*this)[1][0]*(*this)[2][2];
// 	double fCofactor20 = (*this)[1][0]*(*this)[2][1] -
// 		(*this)[1][1]*(*this)[2][0];
// 
// 	double fDet =
// 		(*this)[0][0]*fCofactor00 +
// 		(*this)[0][1]*fCofactor10 +
// 		(*this)[0][2]*fCofactor20;
// 
// 	return fDet;
// }
void Matrix3x3::fromAxisAngle (const Vertex3d& rkAxis, const double& fRadians)
{
	double fCos = cos(fRadians);
	double fSin = sin(fRadians);
	double fOneMinusCos = 1.0-fCos;
	double fX2 = rkAxis.x*rkAxis.x;
	double fY2 = rkAxis.y*rkAxis.y;
	double fZ2 = rkAxis.z*rkAxis.z;
	double fXYM = rkAxis.x*rkAxis.y*fOneMinusCos;
	double fXZM = rkAxis.x*rkAxis.z*fOneMinusCos;
	double fYZM = rkAxis.y*rkAxis.z*fOneMinusCos;
	double fXSin = rkAxis.x*fSin;
	double fYSin = rkAxis.y*fSin;
	double fZSin = rkAxis.z*fSin;

	m_dData[0] = fX2*fOneMinusCos+fCos;
	m_dData[1] = fXYM-fZSin;
	m_dData[2] = fXZM+fYSin;
	m_dData[3] = fXYM+fZSin;
	m_dData[4] = fY2*fOneMinusCos+fCos;
	m_dData[5] = fYZM-fXSin;
	m_dData[6] = fXZM-fYSin;
	m_dData[7] = fYZM+fXSin;
	m_dData[8] = fZ2*fOneMinusCos+fCos;
}

//-----------------------------------------------------------------------
void Matrix3x3::toAxisAngle (Vertex3d& rkAxis, double& rfRadians) const
{
	// Let (x,y,z) be the unit-length axis and let A be an angle of rotation.
	// The rotation matrix is R = I + sin(A)*P + (1-cos(A))*P^2 where
	// I is the identity and
	//
	//       +-        -+
	//   P = |  0 -z +y |
	//       | +z  0 -x |
	//       | -y +x  0 |
	//       +-        -+
	//
	// If A > 0, R represents a counterclockwise rotation about the axis in
	// the sense of looking from the tip of the axis vector towards the
	// origin.  Some algebra will show that
	//
	//   cos(A) = (trace(R)-1)/2  and  R - R^t = 2*sin(A)*P
	//
	// In the event that A = pi, R-R^t = 0 which prevents us from extracting
	// the axis through P.  Instead note that R = I+2*P^2 when A = pi, so
	// P^2 = (R-I)/2.  The diagonal entries of P^2 are x^2-1, y^2-1, and
	// z^2-1.  We can solve these for axis (x,y,z).  Because the angle is pi,
	// it does not matter which sign you choose on the square roots.

	double fTrace = (*this)[0][0] + (*this)[1][1] + (*this)[2][2];
	double fCos = 0.5*(fTrace-1.0);
	rfRadians = MathEx::acos(fCos);  // in [0,PI]

	if ( rfRadians > double(0.0) )
	{
		if ( rfRadians < double(MathEx::ONE_PI) )
		{
			rkAxis.x = (*this)[2][1]-(*this)[1][2];
			rkAxis.y = (*this)[0][2]-(*this)[2][0];
			rkAxis.z = (*this)[1][0]-(*this)[0][1];
			rkAxis.normalize();
		}
		else
		{
			// angle is PI
			double fHalfInverse;
			//		if ( (*this)[0][0] >= (*this)[1][1] )
			if ( ( (*this)[0][0] - (*this)[1][1] > TOLERANCE ) ||
				( fabs((*this)[0][0] - (*this)[1][1]) < TOLERANCE ) ) //xx_0121
			{
				// r00 >= r11
				//			if ( (*this)[0][0] >= (*this)[2][2] )
				if ( ( (*this)[0][0] - (*this)[2][2] > TOLERANCE ) ||
					( fabs((*this)[0][0] - (*this)[2][2]) < TOLERANCE ) ) //xx_0121
				{
					// r00 is maximum diagonal term
					rkAxis.x = 0.5*sqrt((*this)[0][0] -
						(*this)[1][1] - (*this)[2][2] + 1.0);
					fHalfInverse = 0.5/rkAxis.x;
					rkAxis.y = fHalfInverse*(*this)[0][1];
					rkAxis.z = fHalfInverse*(*this)[0][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5*sqrt((*this)[2][2] -
						(*this)[0][0] - (*this)[1][1] + 1.0);
					fHalfInverse = 0.5/rkAxis.z;
					rkAxis.x = fHalfInverse*(*this)[0][2];
					rkAxis.y = fHalfInverse*(*this)[1][2];
				}
			}
			else
			{
				// r11 > r00
				//		if ( (*this)[1][1] >= (*this)[2][2] ) //xx_0121
				if ( ( (*this)[1][1] - (*this)[2][2] > TOLERANCE ) ||
					( fabs((*this)[1][1] - (*this)[2][2]) < TOLERANCE ) )  
				{
					// r11 is maximum diagonal term
					rkAxis.y = 0.5*sqrt((*this)[1][1] -
						(*this)[0][0] - (*this)[2][2] + 1.0);
					fHalfInverse  = 0.5/rkAxis.y;
					rkAxis.x = fHalfInverse*(*this)[0][1];
					rkAxis.z = fHalfInverse*(*this)[1][2];
				}
				else
				{
					// r22 is maximum diagonal term
					rkAxis.z = 0.5*sqrt((*this)[2][2] -
						(*this)[0][0] - (*this)[1][1] + 1.0);
					fHalfInverse = 0.5/rkAxis.z;
					rkAxis.x = fHalfInverse*(*this)[0][2];
					rkAxis.y = fHalfInverse*(*this)[1][2];
				}
			}
		}
	}
	else
	{
		// The angle is 0 and the matrix is the identity.  Any axis will
		// work, so just use the x-axis.
		rkAxis.x = 1.0;
		rkAxis.y = 0.0;
		rkAxis.z = 0.0;
	}
}

int Matrix3x3::toEulerAnglesXYZ (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz          -cy*sz           sy
	//        cz*sx*sy+cx*sz  cx*cz-sx*sy*sz -cy*sx
	//       -cx*cz*sy+sx*sz  cz*sx+cx*sy*sz  cx*cy

	double d = (*this)[0][2];

	rfPAngle = double(MathEx::asin((*this)[0][2]));
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2(-(*this)[1][2],(*this)[2][2]);
			rfRAngle = MathEx::atan2(-(*this)[0][1],(*this)[0][0]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2((*this)[1][0],(*this)[1][1]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2((*this)[1][0],(*this)[1][1]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}
}
//-----------------------------------------------------------------------
int Matrix3x3::toEulerAnglesXZY (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz          -sz              cz*sy
	//        sx*sy+cx*cy*sz  cx*cz          -cy*sx+cx*sy*sz
	//       -cx*sy+cy*sx*sz  cz*sx           cx*cy+sx*sy*sz

	rfPAngle = MathEx::asin(-(*this)[0][1]);
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2((*this)[2][1],(*this)[1][1]);
			rfRAngle = MathEx::atan2((*this)[0][2],(*this)[0][0]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2(-(*this)[2][0],(*this)[2][2]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2(-(*this)[2][0],(*this)[2][2]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}
	return 0;
}

int Matrix3x3::toEulerAnglesYXZ (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz+sx*sy*sz  cz*sx*sy-cy*sz  cx*sy
	//        cx*sz           cx*cz          -sx
	//       -cz*sy+cy*sx*sz  cy*cz*sx+sy*sz  cx*cy

	rfPAngle = MathEx::asin(-(*this)[1][2]);
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2((*this)[0][2],(*this)[2][2]);
			rfRAngle = MathEx::atan2((*this)[1][0],(*this)[1][1]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2(-(*this)[0][1],(*this)[0][0]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2(-(*this)[0][1],(*this)[0][0]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}
}
//-----------------------------------------------------------------------
int Matrix3x3::toEulerAnglesYZX (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz           sx*sy-cx*cy*sz  cx*sy+cy*sx*sz
	//        sz              cx*cz          -cz*sx
	//       -cz*sy           cy*sx+cx*sy*sz  cx*cy-sx*sy*sz

	rfPAngle = MathEx::asin((*this)[1][0]);
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2(-(*this)[2][0],(*this)[0][0]);
			rfRAngle = MathEx::atan2(-(*this)[1][2],(*this)[1][1]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2((*this)[2][1],(*this)[2][2]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2((*this)[2][1],(*this)[2][2]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}
}
//-----------------------------------------------------------------------
int Matrix3x3::toEulerAnglesZXY (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz-sx*sy*sz -cx*sz           cz*sy+cy*sx*sz
	//        cz*sx*sy+cy*sz  cx*cz          -cy*cz*sx+sy*sz
	//       -cx*sy           sx              cx*cy

	rfPAngle = MathEx::asin((*this)[2][1]);
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2(-(*this)[0][1],(*this)[1][1]);
			rfRAngle = MathEx::atan2(-(*this)[2][0],(*this)[2][2]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2((*this)[0][2],(*this)[0][0]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2((*this)[0][2],(*this)[0][0]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}

}
//-----------------------------------------------------------------------
int Matrix3x3::toEulerAnglesZYX (double& rfYAngle, double& rfPAngle,
								 double& rfRAngle) const
{
	// rot =  cy*cz           cz*sx*sy-cx*sz  cx*cz*sy+sx*sz
	//        cy*sz           cx*cz+sx*sy*sz -cz*sx+cx*sy*sz
	//       -sy              cy*sx           cx*cy

	rfPAngle = MathEx::asin(-(*this)[2][0]);
	if ( rfPAngle < double(MathEx::HALF_PI) )
	{
		if ( rfPAngle > double(-MathEx::HALF_PI) )
		{
			rfYAngle = MathEx::atan2((*this)[1][0],(*this)[0][0]);
			rfRAngle = MathEx::atan2((*this)[2][1],(*this)[2][2]);
			return 1;
		}
		else
		{
			// WARNING.  Not a unique solution.
			double fRmY = MathEx::atan2(-(*this)[0][1],(*this)[0][2]);
			rfRAngle = double(0.0);  // any angle works
			rfYAngle = rfRAngle - fRmY;
			return 0;
		}
	}
	else
	{
		// WARNING.  Not a unique solution.
		double fRpY = MathEx::atan2(-(*this)[0][1],(*this)[0][2]);
		rfRAngle = double(0.0);  // any angle works
		rfYAngle = fRpY - rfRAngle;
		return 0;
	}

}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesXYZ ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	*this = kXMat*(kYMat * kZMat);
}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesXZY ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	*this = kXMat*(kZMat*kYMat);
}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesYXZ ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	*this = kYMat*(kXMat*kZMat);
}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesYZX ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	*this = kYMat*(kZMat*kXMat);
}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesZXY ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	*this = kZMat*(kXMat*kYMat);
}
//-----------------------------------------------------------------------
void Matrix3x3::fromEulerAnglesZYX ( double fYAngle,  double fPAngle,
									double fRAngle)
{
	double fCos, fSin;

	fCos = cos(fYAngle);
	fSin = sin(fYAngle);
	Matrix3x3 kZMat(fCos,-fSin,0.0,fSin,fCos,0.0,0.0,0.0,1.0);

	fCos = cos(fPAngle);
	fSin = sin(fPAngle);
	Matrix3x3 kYMat(fCos,0.0,fSin,0.0,1.0,0.0,-fSin,0.0,fCos);

	fCos = cos(fRAngle);
	fSin = sin(fRAngle);
	Matrix3x3 kXMat(1.0,0.0,0.0,0.0,fCos,-fSin,0.0,fSin,fCos);

	*this = kZMat*(kYMat*kXMat);
}

void Matrix3x3::fromAxes(const Vertex3d& xAxis, const Vertex3d& yAxis, const Vertex3d& zAxis)
{
	setColVector(0,xAxis);
	setColVector(1,yAxis);
	setColVector(2,zAxis);
}

void Matrix3x3::orthonormalize ()
{
	// Algorithm uses Gram-Schmidt orthogonalization.  If 'this' matrix is
	// M = [m0|m1|m2], then orthonormal output matrix is Q = [q0|q1|q2],
	//
	//   q0 = m0/|m0|
	//   q1 = (m1-(q0*m1)q0)/|m1-(q0*m1)q0|
	//   q2 = (m2-(q0*m2)q0-(q1*m2)q1)/|m2-(q0*m2)q0-(q1*m2)q1|
	//
	// where |V| indicates length of vector V and A*B indicates dot
	// product of vectors A and B.

	// compute q0
	double fInvLength = 1/ sqrt((*this)[0][0]*(*this)[0][0]
	+ (*this)[1][0]*(*this)[1][0] +
		(*this)[2][0]*(*this)[2][0]);

	(*this)[0][0] *= fInvLength;
	(*this)[1][0] *= fInvLength;
	(*this)[2][0] *= fInvLength;

	// compute q1
	double fDot0 =
		(*this)[0][0]*(*this)[0][1] +
		(*this)[1][0]*(*this)[1][1] +
		(*this)[2][0]*(*this)[2][1];

	(*this)[0][1] -= fDot0*(*this)[0][0];
	(*this)[1][1] -= fDot0*(*this)[1][0];
	(*this)[2][1] -= fDot0*(*this)[2][0];

	fInvLength = 1 / sqrt((*this)[0][1]*(*this)[0][1] +
		(*this)[1][1]*(*this)[1][1] +
		(*this)[2][1]*(*this)[2][1]);

	(*this)[0][1] *= fInvLength;
	(*this)[1][1] *= fInvLength;
	(*this)[2][1] *= fInvLength;

	// compute q2
	double fDot1 =
		(*this)[0][1]*(*this)[0][2] +
		(*this)[1][1]*(*this)[1][2] +
		(*this)[2][1]*(*this)[2][2];

	fDot0 =
		(*this)[0][0]*(*this)[0][2] +
		(*this)[1][0]*(*this)[1][2] +
		(*this)[2][0]*(*this)[2][2];

	(*this)[0][2] -= fDot0*(*this)[0][0] + fDot1*(*this)[0][1];
	(*this)[1][2] -= fDot0*(*this)[1][0] + fDot1*(*this)[1][1];
	(*this)[2][2] -= fDot0*(*this)[2][0] + fDot1*(*this)[2][1];

	fInvLength = 1 / sqrt((*this)[0][2]*(*this)[0][2] +
		(*this)[1][2]*(*this)[1][2] +
		(*this)[2][2]*(*this)[2][2]);

	(*this)[0][2] *= fInvLength;
	(*this)[1][2] *= fInvLength;
	(*this)[2][2] *= fInvLength;
}

Vertex3d operator*(const Vertex3d& lhv, const Matrix3x3& rhm)
{
	// vertex3d作为行向量
	double dTempX,dTempY,dTempZ;
	dTempX = rhm.getColVector(0) * (lhv);
	dTempY = rhm.getColVector(1) * (lhv);
	dTempZ = rhm.getColVector(2) * (lhv);
	return Vertex3d(dTempX,dTempY,dTempZ);
}

const Matrix3x3 operator+(const Matrix3x3& lhm, const Matrix3x3& rhm)
{
	Matrix3x3 matRes(lhm);
	matRes += rhm;
	return matRes;
}

const Matrix3x3 operator-(const Matrix3x3& lhm, const Matrix3x3& rhm)
{
	Matrix3x3 matRes(lhm);
	matRes -= rhm;
	return matRes;
}

const Matrix3x3 operator+(const Matrix3x3& lhm, double rhd)
{
	Matrix3x3 matRes(lhm);
	matRes += rhd;
	return matRes;
}

const Matrix3x3 operator-(const Matrix3x3& lhm, double rhd)
{
	Matrix3x3 matRes(lhm);
	matRes -= rhd;
	return matRes;
}

const Matrix3x3 operator*(const Matrix3x3& lhm, const Matrix3x3& rhm)
{
	Matrix3x3 matRes(lhm);
	matRes *= rhm;
	return matRes;
}

const Matrix3x3 operator*(const Matrix3x3& lhm, double rhd)
{
	Matrix3x3 matRes(lhm);
	matRes *= rhd;
	return matRes;
}

const Matrix3x3 operator/(const Matrix3x3& lhm, double rhd)
{
	Matrix3x3 matRes(lhm);
	matRes /= rhd;
	return matRes;
}


end_gdb_namespace
end_gtl_namespace


