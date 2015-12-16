/*
* Geosciences Template Library
*
* Copyright (c) 2008
* Zhenwen He (zwhe@hotmail.com)
*
* Permission to use, copy, modify, distribute and sell this software
* and its documentation for any purpose is hereby granted without fee,
* provided that the above copyright notice appear in all copies and
* that both that copyright notice and this permission notice appear
* in supporting documentation.  Zhenwen He  makes no  representations
* about the suitability of this software for any purpose.
* It is provided "as is" without express or implied warranty.
*/
#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <cmath>
#include "config.h"
#include "vertex3d.h"
#include "vertex4d.h"

begin_cn_namespace
begin_edu_namespace
begin_cug_namespace
begin_gdb_namespace
class Matrix3x3;

struct PLANE3D
{
	double A, B, C, D;
};

/** @addtogroup math  Quaternion
*  @{
*/
class CORE_API Quaternion{
	double _v[4];
public:
	Quaternion(){
		_v[0] = 0.0; _v[1] = 0.0; _v[2] = 0.0; _v[3] = 1.0;
	}
	Quaternion(double x, double y, double z, double w)
	{
		_v[0] = x;
		_v[1] = y;
		_v[2] = z;
		_v[3] = w;
	}
	Quaternion(const Vertex4d & v)
	{
		_v[0] = v.x;
		_v[1] = v.y;
		_v[2] = v.z;
		_v[3] = v.w;
	}
	Quaternion operator = (const Quaternion & q){
		_v[0] = q._v[0]; _v[1] = q._v[1]; _v[2] = q._v[2]; _v[3] = q._v[3];
		return *this;
	}
public:
	inline double & operator [] (int i) { return _v[i]; }
	inline double   operator [] (int i) const { return _v[i]; }

	inline double & x() { return _v[0]; }
	inline double & y() { return _v[1]; }
	inline double & z() { return _v[2]; }
	inline double & w() { return _v[3]; }

	inline double x() const { return _v[0]; }
	inline double y() const { return _v[1]; }
	inline double z() const { return _v[2]; }
	inline double w() const { return _v[3]; }
public:
	void toRotateMatrix(Matrix3x3& kRot) const;
	void fromRotateMatrix(const Matrix3x3& kRot);
public:
	/* -------------------------------------------------------------
	BASIC ARITHMETIC METHODS
	Implemented in terms of Vec4s.  Some Vec4 operators, e.g.
	operator* are not appropriate for quaternions (as
	mathematical objects) so they are implemented differently.
	Also define methods for conjugate and the multiplicative inverse.
	------------------------------------------------------------- */
	/// Multiply by scalar 
	inline const Quaternion operator * (double rhs) const
	{
		return Quaternion(_v[0] * rhs, _v[1] * rhs, _v[2] * rhs, _v[3] * rhs);
	}

	/// Unary multiply by scalar 
	inline Quaternion& operator *= (double rhs)
	{
		_v[0] *= rhs;
		_v[1] *= rhs;
		_v[2] *= rhs;
		_v[3] *= rhs;
		return *this;        // enable nesting
	}

	/// Binary multiply 
	inline const Quaternion operator*(const Quaternion& rhs) const
	{
		return Quaternion(rhs._v[3] * _v[0] + rhs._v[0] * _v[3] + rhs._v[1] * _v[2] - rhs._v[2] * _v[1],
			rhs._v[3] * _v[1] - rhs._v[0] * _v[2] + rhs._v[1] * _v[3] + rhs._v[2] * _v[0],
			rhs._v[3] * _v[2] + rhs._v[0] * _v[1] - rhs._v[1] * _v[0] + rhs._v[2] * _v[3],
			rhs._v[3] * _v[3] - rhs._v[0] * _v[0] - rhs._v[1] * _v[1] - rhs._v[2] * _v[2]);
	}

	/// Unary multiply 
	inline Quaternion& operator*=(const Quaternion& rhs)
	{
		double x = rhs._v[3] * _v[0] + rhs._v[0] * _v[3] + rhs._v[1] * _v[2] - rhs._v[2] * _v[1];
		double y = rhs._v[3] * _v[1] - rhs._v[0] * _v[2] + rhs._v[1] * _v[3] + rhs._v[2] * _v[0];
		double z = rhs._v[3] * _v[2] + rhs._v[0] * _v[1] - rhs._v[1] * _v[0] + rhs._v[2] * _v[3];
		_v[3] = rhs._v[3] * _v[3] - rhs._v[0] * _v[0] - rhs._v[1] * _v[1] - rhs._v[2] * _v[2];

		_v[2] = z;
		_v[1] = y;
		_v[0] = x;

		return (*this);            // enable nesting
	}

	/// Divide by scalar 
	inline Quaternion operator / (double rhs) const
	{
		double div = 1.0 / rhs;
		return Quaternion(_v[0] * div, _v[1] * div, _v[2] * div, _v[3] * div);
	}

	/// Unary divide by scalar 
	inline Quaternion& operator /= (double rhs)
	{
		double div = 1.0 / rhs;
		_v[0] *= div;
		_v[1] *= div;
		_v[2] *= div;
		_v[3] *= div;
		return *this;
	}

	/// Binary divide 
	inline const Quaternion operator/(const Quaternion& denom) const
	{
		return ((*this) * denom.inverse());
	}

	/// Unary divide 
	inline Quaternion& operator/=(const Quaternion& denom)
	{
		(*this) = (*this) * denom.inverse();
		return (*this);            // enable nesting
	}

	/// Binary addition 
	inline const Quaternion operator + (const Quaternion& rhs) const
	{
		return Quaternion(_v[0] + rhs._v[0], _v[1] + rhs._v[1],
			_v[2] + rhs._v[2], _v[3] + rhs._v[3]);
	}

	/// Unary addition
	inline Quaternion& operator += (const Quaternion& rhs)
	{
		_v[0] += rhs._v[0];
		_v[1] += rhs._v[1];
		_v[2] += rhs._v[2];
		_v[3] += rhs._v[3];
		return *this;            // enable nesting
	}

	/// Binary subtraction 
	inline const Quaternion operator - (const Quaternion& rhs) const
	{
		return Quaternion(_v[0] - rhs._v[0], _v[1] - rhs._v[1],
			_v[2] - rhs._v[2], _v[3] - rhs._v[3]);
	}

	/// Unary subtraction 
	inline Quaternion& operator -= (const Quaternion& rhs)
	{
		_v[0] -= rhs._v[0];
		_v[1] -= rhs._v[1];
		_v[2] -= rhs._v[2];
		_v[3] -= rhs._v[3];
		return *this;            // enable nesting
	}

	/** Negation operator - returns the negative of the quaternion.
	Basically just calls operator - () on the Vec4 */
	inline const Quaternion operator - () const
	{
		return Quaternion(-_v[0], -_v[1], -_v[2], -_v[3]);
	}

	/// Length of the quaternion = sqrt( vec . vec )
	double length() const
	{
		return sqrt(_v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3]);
	}

	/// Length of the quaternion = vec . vec
	double length2() const
	{
		return _v[0] * _v[0] + _v[1] * _v[1] + _v[2] * _v[2] + _v[3] * _v[3];
	}

	/// Conjugate 
	inline Quaternion conj() const
	{
		return Quaternion(-_v[0], -_v[1], -_v[2], _v[3]);
	}

	/// Multiplicative inverse method: q^(-1) = q^*/(q.q^*)
	inline const Quaternion inverse() const
	{
		return conj() / length2();
	}
public:
	/// Set the elements of the Quaternion to represent a rotation of angle
	/// (radians) around the axis (x,y,z)
	void makeRotate(double angle, double x, double y, double z)
	{
		const double epsilon = 0.0000001;

		double length = sqrt(x*x + y*y + z*z);
		if (length < epsilon)
		{
			// ~zero length axis, so reset rotation to zero.
			*this = Quaternion();
			return;
		}

		double inversenorm = 1.0 / length;
		double coshalfangle = cos(0.5*angle);
		double sinhalfangle = sin(0.5*angle);

		_v[0] = x * sinhalfangle * inversenorm;
		_v[1] = y * sinhalfangle * inversenorm;
		_v[2] = z * sinhalfangle * inversenorm;
		_v[3] = coshalfangle;
	}
	/// Set the elements of the Quaternion to represent a rotation of angle
	/// (radians) around the axis (x,y,z)
	void makeRotate(double angle, const Vertex3d& v){
		makeRotate(angle, v.x, v.y, v.z);
	}


	void makeRotate(double angle1, const Vertex3d& axis1,
		double angle2, const Vertex3d& axis2,
		double angle3, const Vertex3d& axis3)
	{
		Quaternion q1; q1.makeRotate(angle1, axis1);
		Quaternion q2; q2.makeRotate(angle2, axis2);
		Quaternion q3; q3.makeRotate(angle3, axis3);

		*this = q1*q2*q3;
	}
};
typedef std::shared_ptr<Quaternion> QuaternionSharedPtr;
/** @} */

end_gdb_namespace
end_cug_namespace
end_edu_namespace
end_cn_namespace
