#include "config.h"
#include "mathex.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace

double const MathEx::TOL = 1e-6;
double const MathEx::TOL_F = 1e-6;
double const MathEx::ONE_PI = 4.0 * atan( 1.0 );

double const MathEx::TWO_PI =  2.0 * MathEx::ONE_PI;
double const MathEx::HALF_PI = 0.5 * MathEx::ONE_PI;
double const MathEx::dDegToRad = MathEx::ONE_PI / 180.0;
double const MathEx::dRadToDeg = 180.0 / MathEx::ONE_PI;
const double MathEx::dDoubleMax = 1e+14;
const double MathEx::dDoubleMin = -1e+14;

MathEx::MathEx(void)
{
}

MathEx::~MathEx(void)
{
}

double MathEx::acos (double dCosAngle)
{
	double dAngle;
	if(fabs(dCosAngle - 1) < MathEx::TOL) 
		dAngle = 0;
	else if(fabs(dCosAngle + 1) < MathEx::TOL)
		dAngle = ONE_PI;
	else
		dAngle = acos( dCosAngle );

	return dAngle;
}
//-----------------------------------------------------------------------
double MathEx::asin (double dSinAngle)
{
	if(fabs(dSinAngle + 1) < MathEx::TOL)
		return (-HALF_PI);
	else if(fabs(dSinAngle + 1) < MathEx::TOL)
		return (HALF_PI);
	else
		return asin(dSinAngle);
}
 
double MathEx::sign (double fValue)
{
	if ( fValue > MathEx::TOL )
		return 1.0;

	if ( fValue < -MathEx::TOL )
		return -1.0;

	return 0.0;
}

double MathEx::radianToDegree(double angle)
{
	return angle * dRadToDeg;
}

double MathEx::degreeToRadian(double angle)
{
	return angle * dDegToRad;
}
end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
